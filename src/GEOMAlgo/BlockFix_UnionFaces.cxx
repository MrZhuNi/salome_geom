// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
// File:	BlockFix_UnionFaces.cxx
// Created:	Tue Dec  7 17:15:42 2004
// Author:	Pavel DURANDIN
//		Open CASCADE SA 2004

#include <BlockFix_UnionFaces.ixx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>
#include <TopTools_MapOfShape.hxx>
#include <ShapeBuild_ReShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>  
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <Geom_Surface.hxx>
#include <BRep_Tool.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <BRep_Builder.hxx>
#include <TopTools_MapIteratorOfMapOfShape.hxx>
#include <TopoDS_Vertex.hxx>
#include <ShapeFix_Face.hxx>
#include <BRep_Tool.hxx>
#include <ShapeExtend_WireData.hxx>
#include <ShapeAnalysis_WireOrder.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <Geom2d_Line.hxx>
#include <gp_XY.hxx>
#include <gp_Pnt2d.hxx>
#include <ShapeBuild_Edge.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS_Vertex.hxx>

#include <ShapeFix_Wire.hxx>
#include <ShapeFix_Edge.hxx>

#include <Geom_RectangularTrimmedSurface.hxx>
#include <BRepTools.hxx>

#include <TColGeom_HArray2OfSurface.hxx>
#include <ShapeExtend_CompositeSurface.hxx>
#include <ShapeFix_ComposeShell.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <ShapeFix_SequenceOfWireSegment.hxx>
#include <ShapeFix_WireSegment.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Iterator.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_SphericalSurface.hxx>

  
//=======================================================================
//function : BlockFix_UnionFaces
//purpose  : 
//=======================================================================

BlockFix_UnionFaces::BlockFix_UnionFaces()
     : myTolerance(Precision::Confusion())
{
}


//=======================================================================
//function : GetTolearnce
//purpose  : 
//=======================================================================

Standard_Real& BlockFix_UnionFaces::GetTolerance()
{
  return myTolerance;
}


//=======================================================================
//function : AddOrdinaryEdges
//purpose  : auxilary
//=======================================================================
// adds edges from the shape to the sequence
// seams and equal edges are dropped
// Returns true if one of original edges dropped
static Standard_Boolean AddOrdinaryEdges(TopTools_SequenceOfShape& edges,
                                         const TopoDS_Shape aShape,
                                         Standard_Integer& anIndex)
{
  //map of edges
  TopTools_MapOfShape aNewEdges;
  //add edges without seams
  for(TopExp_Explorer exp(aShape,TopAbs_EDGE); exp.More(); exp.Next()) {
    TopoDS_Shape edge = exp.Current();
    if(aNewEdges.Contains(edge))
      aNewEdges.Remove(edge);
    else
      aNewEdges.Add(edge);
  }

  Standard_Boolean isDropped = Standard_False;
  //merge edges and drop seams
  for(Standard_Integer i = 1; i <= edges.Length(); i++) {
    TopoDS_Shape current = edges(i);
    if(aNewEdges.Contains(current)) {
               
      aNewEdges.Remove(current);
      edges.Remove(i);
      i--;
      
      if(!isDropped) {
        isDropped = Standard_True;
        anIndex = i;
      }
    }
  }
          
  //add edges to the sequemce
  for(TopTools_MapIteratorOfMapOfShape anIter(aNewEdges); anIter.More(); anIter.Next())
    edges.Append(anIter.Key());
  
  return isDropped;
}


//=======================================================================
//function : ClearRts
//purpose  : auxilary
//=======================================================================
static Handle(Geom_Surface) ClearRts(const Handle(Geom_Surface)& aSurface)
{
  if(aSurface->IsKind(STANDARD_TYPE(Geom_RectangularTrimmedSurface))) {
    Handle(Geom_RectangularTrimmedSurface) rts = 
      Handle(Geom_RectangularTrimmedSurface)::DownCast(aSurface);
    return rts->BasisSurface();
  }
  return aSurface;
}


//=======================================================================
//function : Perform
//purpose  : 
//=======================================================================

TopoDS_Shape BlockFix_UnionFaces::Perform(const TopoDS_Shape& Shape)
{
  Handle(ShapeBuild_ReShape) myContext = new ShapeBuild_ReShape;
  TopoDS_Shape aResShape = myContext->Apply(Shape);

  // processing each solid
  TopExp_Explorer exps;
  for(exps.Init(Shape, TopAbs_SOLID); exps.More(); exps.Next()) {
    TopoDS_Solid aSolid = TopoDS::Solid(exps.Current());

    // creating map of edge faces
    TopTools_IndexedDataMapOfShapeListOfShape aMapEdgeFaces;
    TopExp::MapShapesAndAncestors(aSolid, TopAbs_EDGE, TopAbs_FACE, aMapEdgeFaces);

    // map of processed shapes
    TopTools_MapOfShape aProcessed;
  
    Handle(ShapeBuild_ReShape) aContext = new ShapeBuild_ReShape;

    Standard_Integer NbModif=0;
    Standard_Boolean hasFailed = Standard_False;
    Standard_Real tol = Min(Max(Precision::Confusion(), myTolerance/10.),0.1);
    // processing each face
    TopExp_Explorer exp;
    //for( exp.Init(Shape, TopAbs_FACE); exp.More(); exp.Next()) {
    for( exp.Init(aSolid, TopAbs_FACE); exp.More(); exp.Next()) {
      TopoDS_Face aFace = TopoDS::Face(exp.Current().Oriented(TopAbs_FORWARD));
      
      if(aProcessed.Contains(aFace))
        continue;
    
      Standard_Integer dummy;
      TopTools_SequenceOfShape edges;
      AddOrdinaryEdges(edges,aFace,dummy);
    
      TopTools_SequenceOfShape faces;
      faces.Append(aFace);
    
      //surface and location to construct result
      TopLoc_Location aBaseLocation;
      Handle(Geom_Surface) aBaseSurface = BRep_Tool::Surface(aFace,aBaseLocation);
      aBaseSurface = ClearRts(aBaseSurface);

      // find adjacent faces to union
      Standard_Integer i;
      for( i = 1; i <= edges.Length(); i++) {
        TopoDS_Edge edge = TopoDS::Edge(edges(i));
        if(BRep_Tool::Degenerated(edge))
          continue;
      
        const TopTools_ListOfShape& aList = aMapEdgeFaces.FindFromKey(edge);
        TopTools_ListIteratorOfListOfShape anIter(aList);
        for( ; anIter.More(); anIter.Next()) {
          TopoDS_Face anCheckedFace = TopoDS::Face(anIter.Value().Oriented(TopAbs_FORWARD));
          if(anCheckedFace.IsSame(aFace))
            continue;
        
          if(aProcessed.Contains(anCheckedFace))
            continue;
        
          if(IsSameDomain(aFace,anCheckedFace)) {
          
            if(aList.Extent() != 2) {
              // non mainfold case is not processed
              continue;
            }
          
            // replacing pcurves
            TopoDS_Face aMockUpFace;
            BRep_Builder B;
            B.MakeFace(aMockUpFace,aBaseSurface,aBaseLocation,0.);
            MovePCurves(aMockUpFace,anCheckedFace);
            
            if(AddOrdinaryEdges(edges,aMockUpFace,dummy)) {
              // sequence edges is modified
              i = dummy;
            }
            
            faces.Append(anCheckedFace);
            aProcessed.Add(anCheckedFace);
            break;
          }
        }
      }
    
      // all faces collected in the sequence. Perform union of faces
      if(faces.Length() > 1) {
        NbModif++;
        TopoDS_Face aResult;
        BRep_Builder B;
        B.MakeFace(aResult,aBaseSurface,aBaseLocation,0);
        Standard_Integer nbWires = 0;
      
        // connecting wires
        while(edges.Length()>0) {
        
          Standard_Boolean isEdge3d = Standard_False;
          nbWires++;
          TopTools_MapOfShape aVertices;
          TopoDS_Wire aWire;
          B.MakeWire(aWire);
        
          TopoDS_Edge anEdge = TopoDS::Edge(edges(1));
          edges.Remove(1);
        
          isEdge3d |= !BRep_Tool::Degenerated(anEdge);
          B.Add(aWire,anEdge);
          TopoDS_Vertex V1,V2;
          TopExp::Vertices(anEdge,V1,V2);
          aVertices.Add(V1);
          aVertices.Add(V2);
        
          Standard_Boolean isNewFound = Standard_False;
          do {
            isNewFound = Standard_False;
            for(Standard_Integer j = 1; j <= edges.Length(); j++) {
              anEdge = TopoDS::Edge(edges(j));
              TopExp::Vertices(anEdge,V1,V2);
              if(aVertices.Contains(V1) || aVertices.Contains(V2)) {
                isEdge3d |= !BRep_Tool::Degenerated(anEdge);
                aVertices.Add(V1);
                aVertices.Add(V2);
                B.Add(aWire,anEdge);
                edges.Remove(j);
                j--;
                isNewFound = Standard_True;
              }
            }
          } while (isNewFound);
        
          // sorting eny type of edges
          aWire = TopoDS::Wire(aContext->Apply(aWire));
                
          TopoDS_Face tmpF = TopoDS::Face(aContext->Apply(faces(1).Oriented(TopAbs_FORWARD)));
          Handle(ShapeFix_Wire) sfw = new ShapeFix_Wire(aWire,tmpF,Precision::Confusion());
          sfw->FixReorder();
          Standard_Boolean isDegRemoved = Standard_False;
          if(!sfw->StatusReorder ( ShapeExtend_FAIL )) {
            // clear degenerated edges if at least one with 3d curve exist
            if(isEdge3d) {
              Handle(ShapeExtend_WireData) sewd = sfw->WireData();
              for(Standard_Integer j = 1; j<=sewd->NbEdges();j++) {
                TopoDS_Edge E = sewd->Edge(j);
                if(BRep_Tool::Degenerated(E)) {
                  sewd->Remove(j);
                  isDegRemoved = Standard_True;
                  j--;
                }
              }
            }
            sfw->FixShifted();
            if(isDegRemoved)
              sfw->FixDegenerated();
          }
          TopoDS_Wire aWireFixed = sfw->Wire();
          aContext->Replace(aWire,aWireFixed);
          // add resulting wire
          if(isEdge3d) {
            B.Add(aResult,aWireFixed);
          }
          else  {
            // sorting edges
            Handle(ShapeExtend_WireData) sbwd = sfw->WireData();
            Standard_Integer nbEdges = sbwd->NbEdges();
            // sort degenerated edges and create one edge instead of several ones
            ShapeAnalysis_WireOrder sawo(Standard_False, 0);
            ShapeAnalysis_Edge sae;
            Standard_Integer aLastEdge = nbEdges;
            for(Standard_Integer j = 1; j <= nbEdges; j++) {
              Standard_Real f,l;
              //smh protection on NULL pcurve
              Handle(Geom2d_Curve) c2d;
              if(!sae.PCurve(sbwd->Edge(j),tmpF,c2d,f,l)) {
                aLastEdge--;
                continue;
              }
              sawo.Add(c2d->Value(f).XY(),c2d->Value(l).XY());
            }
            sawo.Perform();
            
            // constructind one degenerative edge
            gp_XY aStart, anEnd, tmp;
            Standard_Integer nbFirst = sawo.Ordered(1);
            TopoDS_Edge anOrigE = TopoDS::Edge(sbwd->Edge(nbFirst).Oriented(TopAbs_FORWARD));
            ShapeBuild_Edge sbe;
            TopoDS_Vertex aDummyV;
            TopoDS_Edge E = sbe.CopyReplaceVertices(anOrigE,aDummyV,aDummyV);
            sawo.XY(nbFirst,aStart,tmp);
            sawo.XY(sawo.Ordered(aLastEdge),tmp,anEnd);
          
            gp_XY aVec = anEnd-aStart;
            Handle(Geom2d_Line) aLine = new Geom2d_Line(aStart,gp_Dir2d(anEnd-aStart));

            B.UpdateEdge(E,aLine,tmpF,0.);
            B.Range(E,tmpF,0.,aVec.Modulus());
            Handle(Geom_Curve) C3d;
            B.UpdateEdge(E,C3d,0.);
            B.Degenerated(E,Standard_True);
            TopoDS_Wire aW;
            B.MakeWire(aW);
            B.Add(aW,E);
            B.Add(aResult,aW);
          }
        
        }
      
        // perform substitution of face
        aContext->Replace(aContext->Apply(aFace),aResult);
      
      
        ShapeFix_Face sff (aResult);
        //Intializing by tolerances
        sff.SetPrecision(myTolerance);
        sff.SetMinTolerance(tol);
        sff.SetMaxTolerance(Max(1.,myTolerance*1000.));
        //Setting modes
        sff.FixOrientationMode() = 0;
        //sff.FixWireMode() = 0;
        sff.SetContext(aContext);
        // Applying the fixes
        sff.Perform();
        if(sff.Status(ShapeExtend_FAIL)) 
        hasFailed = Standard_True;
      
        // breaking down to several faces
        TopoDS_Shape theResult = aContext->Apply(aResult);
        for(TopExp_Explorer aFaceExp(theResult,TopAbs_FACE);aFaceExp.More();aFaceExp.Next()) {
          TopoDS_Face aCurrent = TopoDS::Face(aFaceExp.Current().Oriented(TopAbs_FORWARD));
          Handle(TColGeom_HArray2OfSurface) grid = new TColGeom_HArray2OfSurface ( 1, 1, 1, 1 );
          grid->SetValue ( 1, 1, aBaseSurface );
          Handle(ShapeExtend_CompositeSurface) G = new ShapeExtend_CompositeSurface ( grid );
          ShapeFix_ComposeShell CompShell;
          CompShell.Init ( G, aBaseLocation, aCurrent, ::Precision::Confusion() );//myPrecision 
          CompShell.SetContext( aContext );
          
          TopTools_SequenceOfShape parts;
          ShapeFix_SequenceOfWireSegment wires;
          for(TopExp_Explorer W_Exp(aCurrent,TopAbs_WIRE);W_Exp.More();W_Exp.Next()) {
            Handle(ShapeExtend_WireData) sbwd = 
              new ShapeExtend_WireData ( TopoDS::Wire(W_Exp.Current() ));
            ShapeFix_WireSegment seg ( sbwd, TopAbs_REVERSED );
            wires.Append(seg);
          }
                
          CompShell.DispatchWires ( parts,wires );
          for (Standard_Integer j=1; j <= parts.Length(); j++ ) {
            ShapeFix_Face aFixOrient(TopoDS::Face(parts(j)));
            aFixOrient.SetContext(aContext);
            aFixOrient.FixOrientation();
          }
        
          TopoDS_Shape CompRes;
          if ( faces.Length() !=1 ) {
            TopoDS_Shell S;
            B.MakeShell ( S );
            for ( i=1; i <= parts.Length(); i++ ) 
              B.Add ( S, parts(i) );
            CompRes = S;
          }
          else CompRes = parts(1);
          
          aContext->Replace(aCurrent,CompRes);
        }
      
        // remove the remaining faces
        for(i = 2; i <= faces.Length(); i++)
          aContext->Remove(faces(i));
      }
    }
  
    //TopoDS_Shape aResult = Shape;
    if(NbModif>0) {
      TopoDS_Shape aResult = aSolid;
      if(!hasFailed) {
        aResult = aContext->Apply(aSolid);
    
        ShapeFix_Edge sfe;
        for(exp.Init(aResult,TopAbs_EDGE); exp.More(); exp.Next()) {
          TopoDS_Edge E = TopoDS::Edge(exp.Current());
          sfe.FixVertexTolerance (E);
          // ptv add fix same parameter
          sfe.FixSameParameter(E, myTolerance);
        }
        
        myContext->Replace(aSolid,aResult);
      }
    }
//    else {
      for( exp.Init(aSolid, TopAbs_FACE); exp.More(); exp.Next()) {
        TopoDS_Face aFace = TopoDS::Face(exp.Current().Oriented(TopAbs_FORWARD));
        Handle(ShapeFix_Wire) sfw = new ShapeFix_Wire;
        sfw->SetContext(myContext);
        sfw->SetPrecision(myTolerance);
        sfw->SetMinTolerance(myTolerance);
        sfw->SetMaxTolerance(Max(1.,myTolerance*1000.));
        sfw->SetFace(aFace);
        for ( TopoDS_Iterator iter(aFace,Standard_False); iter.More(); iter.Next()) { 
          TopoDS_Wire wire = TopoDS::Wire ( iter.Value() );
          sfw->Load(wire);
          sfw->FixReorder();
          sfw->FixShifted();
        }
      }
//    }

  } // end processing each solid

  aResShape = myContext->Apply(Shape);
  return aResShape;
}


//=======================================================================
//function : IsSameDomain
//purpose  : 
//=======================================================================

Standard_Boolean BlockFix_UnionFaces::IsSameDomain(const TopoDS_Face& aFace,
                                                   const TopoDS_Face& aCheckedFace) const
{
  //checking the same handless
  TopLoc_Location L1, L2;
  Handle(Geom_Surface) S1, S2;
  
  S1 = BRep_Tool::Surface(aFace,L1);
  S2 = BRep_Tool::Surface(aCheckedFace,L2);
  
  return (S1 == S2 && L1 == L2);
}


//=======================================================================
//function : MovePCurves
//purpose  : 
//=======================================================================

void BlockFix_UnionFaces::MovePCurves(TopoDS_Face& aTarget,
                                      const TopoDS_Face& aSource) const
{
  BRep_Builder B;
  for(TopExp_Explorer wexp(aSource,TopAbs_WIRE);wexp.More();wexp.Next()) {
    Handle(ShapeFix_Wire) sfw = new ShapeFix_Wire(TopoDS::Wire(wexp.Current()), 
                                                  aTarget, Precision::Confusion());
    sfw->FixReorder();
    Standard_Boolean isReoredFailed = sfw->StatusReorder ( ShapeExtend_FAIL );
    sfw->FixEdgeCurves();
    if(isReoredFailed)
      continue;
    
    sfw->FixShifted();
    sfw->FixDegenerated();
    
    // remove degenerated edges from not degenerated points
    ShapeAnalysis_Edge sae;
    Handle(ShapeExtend_WireData) sewd = sfw->WireData();
    for(Standard_Integer i = 1; i<=sewd->NbEdges();i++) {
      TopoDS_Edge E = sewd->Edge(i);
      if(BRep_Tool::Degenerated(E)&&!sae.HasPCurve(E,aTarget)) {
        sewd->Remove(i);
        i--;
      }
    }
    
    TopoDS_Wire ResWire = sfw->Wire();
    B.Add(aTarget,ResWire);
  }
}
     
