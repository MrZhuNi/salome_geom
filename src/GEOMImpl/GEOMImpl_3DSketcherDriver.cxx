// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#include <Standard_Stream.hxx>

#include <GEOMImpl_3DSketcherDriver.hxx>
#include <GEOMImpl_I3DSketcher.hxx>
#include <GEOMImpl_Types.hxx>
#include <GEOM_Function.hxx>

#include <GEOMImpl_IMeasureOperations.hxx>

#include <Basics_Utils.hxx>

// OCCT Includes
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pnt.hxx>

#include <Standard_ConstructionError.hxx>

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& GEOMImpl_3DSketcherDriver::GetID()
{
  static Standard_GUID a3DSketcherDriver("FF2BBB54-5D24-4df3-210B-3A678263EA26");
  return a3DSketcherDriver;
}


//=======================================================================
//function : GEOMImpl_3DSketcherDriver
//purpose  :
//=======================================================================
GEOMImpl_3DSketcherDriver::GEOMImpl_3DSketcherDriver()
{
}

//=======================================================================
//function : Execute
//purpose  :
//=======================================================================
Standard_Integer GEOMImpl_3DSketcherDriver::Execute(TFunction_Logbook& log) const
{
  if (Label().IsNull()) return 0;
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());

  GEOMImpl_I3DSketcher aCI (aFunction);

  TopoDS_Shape aShape;

  if (aFunction->GetType() == SKETCHER3D_COORDS) {
    Handle(TColStd_HArray1OfReal) aCoordsArray = aCI.GetCoordinates();
    int anArrayLength = aCoordsArray->Length();

    std::list<gp_Pnt> points;

    for (int i = 0; i <= (anArrayLength-3); i += 3) {
      gp_Pnt aPnt = gp_Pnt(aCoordsArray->Value(i+1), aCoordsArray->Value(i+2), aCoordsArray->Value(i+3));
      if (points.empty() || aPnt.Distance(points.back()) > gp::Resolution())
        points.push_back(aPnt);
    }

    if ( points.size() == 1) { // Only Start Point
      BRepBuilderAPI_MakeVertex mkVertex (points.back());
      aShape = mkVertex.Shape();
    }
    else if ( points.size() > 1) { // Make Wire
      BRepBuilderAPI_MakePolygon aMakePoly;
      std::list<gp_Pnt>::iterator it;
      for (it = points.begin(); it != points.end(); ++it) {
        aMakePoly.Add(*it);
      }

      if (points.size() > 2 && 
          points.back().X() == points.front().X() && 
          points.back().Y() == points.front().Y() && 
          points.back().Z() == points.front().Z())
        aMakePoly.Close();

      if (aMakePoly.IsDone())
        aShape = aMakePoly.Wire();
    }
  }
  else if (aFunction->GetType() == SKETCHER3D_COMMAND) {
    Kernel_Utils::Localizer loc;

    TCollection_AsciiString aCommand = aCI.GetCommand();
    // "3DSketcher:CMD[:CMD[:CMD...]]"

    // Split the command string to separate CMDs
    int icmd = 2;
    TColStd_SequenceOfAsciiString aSequence;
    if (aCommand.Length()) {
      TCollection_AsciiString aToken = aCommand.Token(":", icmd);
      while (aToken.Length() > 0) {
        aSequence.Append(aToken);
        aToken = aCommand.Token(":", ++icmd);
      }
    }

    int nbEdges = 0;
    bool isFirstPointSet = false;
    gp_XYZ p = gp::Origin().XYZ();
    BRepBuilderAPI_MakeVertex MV0 (p);
    TopoDS_Vertex V = TopoDS::Vertex(MV0.Shape());

    gp_XYZ p0 = p;
    TopoDS_Vertex V0 = V;

    bool doClose = false;
    BRepBuilderAPI_MakeWire MW;

    int nbCMDs = aSequence.Length();
    for (icmd = 1; icmd <= nbCMDs; icmd++) {
      TCollection_AsciiString aCMD = aSequence.Value(icmd);

      // Split the CMD into string values
      TColStd_SequenceOfAsciiString aStrVals;
      int ival = 1;
      TCollection_AsciiString aToken = aCMD.Token(" ", ival);
      while (aToken.Length() > 0) {
        aStrVals.Append(aToken);
        aToken = aCMD.Token(" ", ++ival);
      }

      // "TT x y z" : Create segment by point at X & Y or set the first point
      // "T dx dy dz" : Create segment by point with DX & DY
      //
      // "OXY angleX angle2 length" : Create segment by two angles and length
      // "OYZ angleY angle2 length" : Create segment by two angles and length
      // "OXZ angleX angle2 length" : Create segment by two angles and length
      //
      // "WW" : Close Wire (to finish)

      switch (aStrVals.Value(1).Value(1))
      {
      case 'T':
        {
          if (aStrVals.Length() != 4)
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");

          gp_XYZ vp;
          vp.SetX(aStrVals.Value(2).RealValue());
          vp.SetY(aStrVals.Value(3).RealValue());
          vp.SetZ(aStrVals.Value(4).RealValue());

          if (aStrVals.Value(1) == "TT") { // absolute coordinates
            if (!isFirstPointSet) {
              p = vp;
              BRepBuilderAPI_MakeVertex MV (p);
              V = TopoDS::Vertex(MV.Shape());

              p0 = p;
              V0 = V;

              isFirstPointSet = true;
            }
            else {
              if ((vp - p).SquareModulus() > Precision::Confusion()) {
                BRepBuilderAPI_MakeVertex MV (vp);
                TopoDS_Vertex VV = TopoDS::Vertex(MV.Shape());
                BRepBuilderAPI_MakeEdge ME (V, VV);
                MW.Add(ME);
                nbEdges++;

                p = vp;
                V = VV;
              }
            }
          }
          else if (aStrVals.Value(1) == "T") { // relative coordinates
            if (vp.SquareModulus() > Precision::Confusion()) {
              vp = p + vp;

              BRepBuilderAPI_MakeVertex MV (vp);
              TopoDS_Vertex VV = TopoDS::Vertex(MV.Shape());
              BRepBuilderAPI_MakeEdge ME (V, VV);
              MW.Add(ME);
              nbEdges++;

              p = vp;
              V = VV;
            }
          }
          else
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");
        }
        break;
      case 'O':
        {
          if (aStrVals.Length() != 4)
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");

          double anAngle  = aStrVals.Value(2).RealValue() * M_PI/180.0;
          double anAngle2 = aStrVals.Value(3).RealValue() * M_PI/180.0;
          double aLength  = aStrVals.Value(4).RealValue();

          double aProjectedLength = aLength * cos(anAngle2);

          gp_XYZ vp;
          vp.SetX(aStrVals.Value(2).RealValue());
          vp.SetY(aStrVals.Value(3).RealValue());
          vp.SetZ(aStrVals.Value(4).RealValue());

          if (aStrVals.Value(1) == "OXY") {
            vp.SetX(p.X() + aProjectedLength * cos(anAngle));
            vp.SetY(p.Y() + aProjectedLength * sin(anAngle));
            vp.SetZ(p.Z() + aLength * sin(anAngle2));
          }
          else if (aStrVals.Value(1) == "OYZ") {
            vp.SetX(p.X() + aLength * sin(anAngle2));
            vp.SetY(p.Y() + aProjectedLength * cos(anAngle));
            vp.SetZ(p.Z() + aProjectedLength * sin(anAngle));
          }
          else if (aStrVals.Value(1) == "OXZ") {
            vp.SetX(p.X() + aProjectedLength * cos(anAngle));
            vp.SetY(p.Y() + aLength * sin(anAngle2));
            vp.SetZ(p.Z() + aProjectedLength * sin(anAngle));
          }
          else
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");
          
          if ((vp - p).SquareModulus() > Precision::Confusion()) {
            BRepBuilderAPI_MakeVertex MV (vp);
            TopoDS_Vertex VV = TopoDS::Vertex(MV.Shape());
            BRepBuilderAPI_MakeEdge ME (V, VV);
            MW.Add(ME);
            nbEdges++;

            p = vp;
            V = VV;
          }
        }
        break;
      case 'W':
        {
          if (aStrVals.Length() != 1)
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");

          if (aStrVals.Value(1) == "WW")
            doClose = true;
          else
            Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");
        }
        break;
      default:
        {
          Standard_ConstructionError::Raise("3D Sketcher error: Bad format of command.");
        }
      }
    }

    if (doClose &&
        nbEdges > 1 && // as 3D sketcher has only straight edges
        (p - p0).SquareModulus() > Precision::Confusion()) {
      BRepBuilderAPI_MakeEdge ME (V, V0);
      MW.Add(ME);
      nbEdges++;
    }

    if (nbEdges > 0) {
      if (!MW.IsDone())
        Standard_ConstructionError::Raise("3D Sketcher error: Wire construction failed.");

      aShape = MW;
    }
    else {
      if (isFirstPointSet) {
        aShape = V0;
      }
    }
  }
  else {
  }

  if (aShape.IsNull()) return 0;

  aFunction->SetValue(aShape);
  log.SetTouched(Label());
  return 1;
}


//=======================================================================
//function :  GEOMImpl_3DSketcherDriver_Type_
//purpose  :
//=======================================================================
Standard_EXPORT Handle_Standard_Type& GEOMImpl_3DSketcherDriver_Type_()
{

  static Handle_Standard_Type aType1 = STANDARD_TYPE(TFunction_Driver);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TFunction_Driver);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);


  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMImpl_3DSketcherDriver",
                                                         sizeof(GEOMImpl_3DSketcherDriver),
                                                         1,
                                                         (Standard_Address)_Ancestors,
                                                         (Standard_Address)NULL);

  return _aType;
}

//=======================================================================
//function : DownCast
//purpose  :
//=======================================================================
const Handle(GEOMImpl_3DSketcherDriver) Handle(GEOMImpl_3DSketcherDriver)::DownCast(const Handle(Standard_Transient)& AnObject)
{
  Handle(GEOMImpl_3DSketcherDriver) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMImpl_3DSketcherDriver))) {
       _anOtherObject = Handle(GEOMImpl_3DSketcherDriver)((Handle(GEOMImpl_3DSketcherDriver)&)AnObject);
     }
  }

  return _anOtherObject ;
}
