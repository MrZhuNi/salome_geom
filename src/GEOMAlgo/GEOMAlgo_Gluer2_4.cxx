// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
//

// File:     GEOMAlgo_Gluer2_4.cxx
// Author:   Peter KURNEV


#include <GEOMAlgo_Gluer2.hxx>

#include <Precision.hxx>

#include <gp.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Dir2d.hxx>

#include <TopLoc_Location.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>

#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>

#include <Geom2d_Curve.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <GeomAPI_ProjectPointOnCurve.hxx>

#include <BRep_Builder.hxx>
#include <BRep_ListOfCurveRepresentation.hxx>
#include <BRep_ListIteratorOfListOfCurveRepresentation.hxx>
#include <BRep_CurveRepresentation.hxx>
#include <BRep_GCurve.hxx>
#include <BRep_TEdge.hxx>
#include <BRep_CurveOnSurface.hxx>
#include <BRep_Tool.hxx>

#include <BRepLib.hxx>

#include <BOPTools_Tools2D.hxx>
#include <GEOMAlgo_Tools3D.hxx>
#include <BOPTools_Tools2D.hxx>


static
  Standard_Boolean IsClosed(const TopoDS_Edge& aE,
			    const TopoDS_Face& aF);
static
  Standard_Integer UpdateClosedEdge(const TopoDS_Edge& aEold,
				    const TopoDS_Edge& aEnew,
				    const TopoDS_Face& aF, 
				    const Handle(IntTools_Context)& aCtx);

static
  Standard_Integer ConvertExistingPCurve(const TopoDS_Edge& aEold,
					 const TopoDS_Edge& aEnew,
					 const TopoDS_Face& aF,
					 const Handle(IntTools_Context)& aCtx);
static
  Standard_Boolean IsToReverse(const TopoDS_Edge& aEold,
			     const TopoDS_Edge& aEnew,
			     const Handle(IntTools_Context)& aCtx);

static
  void  UpdateEdge(const TopoDS_Edge& E, 
		 const Handle(Geom2d_Curve)& C, 
		 const TopoDS_Face& F, 
		 const Standard_Real Tol);
static
  void  UpdateEdge(const TopoDS_Edge& E, 
		   const Handle(Geom2d_Curve)& C, 
		   const Handle(Geom_Surface)& S, 
		   const TopLoc_Location& L,
		   const Standard_Real Tol);

static 
  void UpdateCurves(BRep_ListOfCurveRepresentation& lcr,
		    const Handle(Geom2d_Curve)& C,
		    const Handle(Geom_Surface)& S,
		    const TopLoc_Location& L);

//=======================================================================
//function : BuildPCurveForEdgeOnFace
//purpose  :
//=======================================================================
Standard_Integer 
  GEOMAlgo_Gluer2::BuildPCurveForEdgeOnFace(const TopoDS_Edge& aEold,
					    const TopoDS_Edge& aEnew,
					    const TopoDS_Face& aF,
					    const Handle(IntTools_Context)& aCtx)
					    
{
  Standard_Boolean bHasOld;
  Standard_Integer iRet;
  Standard_Real aT1, aT2, aTol;
  Handle(Geom2d_Curve) aC2D;
  //
  iRet=0;
  //
  bHasOld=BOPTools_Tools2D::HasCurveOnSurface(aEnew, aF, aC2D, aT1, aT2, aTol);
  if (bHasOld) {
    return iRet;
  }
  //
  iRet=ConvertExistingPCurve(aEold, aEnew, aF, aCtx);
  if (iRet){
    iRet=1;
  }
  return iRet;
}
//=======================================================================
//function : ConvertExistingPCurve
//purpose  : 
//=======================================================================
Standard_Integer ConvertExistingPCurve(const TopoDS_Edge& aEold,
				      const TopoDS_Edge& aEnew,
				      const TopoDS_Face& aF, 
				      const Handle(IntTools_Context)& aCtx)
{
  Standard_Boolean bIsToReverse, bIsClosed;
  Standard_Integer iRet;
  Standard_Real aTol, aT1, aT2;
  Handle(Geom2d_Curve) aC2Dold, aC2DoldC;
  Handle(Geom2d_TrimmedCurve) aC2DoldCT;
  BRep_Builder aBB;
  //
  iRet=0;
  //
  aC2Dold=BRep_Tool::CurveOnSurface(aEold, aF, aT1, aT2);
  if (aC2Dold.IsNull()){
    iRet=1;
    return iRet;
  }
  //
  aC2DoldC=Handle(Geom2d_Curve)::DownCast(aC2Dold->Copy());
  //
  bIsToReverse=IsToReverse(aEold, aEnew, aCtx);
  if (bIsToReverse) {
    aC2DoldC->Reverse();
  }
  //
  aTol=BRep_Tool::Tolerance(aEnew);
  //
  aC2DoldCT=new Geom2d_TrimmedCurve(aC2DoldC, aT1, aT2);
  //
  UpdateEdge(aEnew, aC2DoldCT, aF, aTol);
  //
  aBB.SameParameter(aEnew, Standard_False);
  //
  BRepLib::SameRange(aEnew);
  //
  BRepLib::SameParameter(aEnew);
  //
  bIsClosed=IsClosed(aEold, aF);
  if (bIsClosed) {
    iRet=UpdateClosedEdge(aEold, aEnew, aF, aCtx);
  }
  //
  return iRet;
}
//=======================================================================
//function : UpdateClosedEdge
//purpose  : 
//=======================================================================
Standard_Integer UpdateClosedEdge(const TopoDS_Edge& aEold,
				  const TopoDS_Edge& aEnew,
				  const TopoDS_Face& aF, 
				  const Handle(IntTools_Context)& aCtx)
{
  Standard_Boolean bUClosed, bRevOrder;
  Standard_Integer aNbPoints, iRet;
  Standard_Real aTS1, aTS2, aTS, aScPr, aUS1, aVS1, aUS2, aVS2, aT, aU, aV;
  Standard_Real aT1, aT2, aTol;
  gp_Pnt2d aP2DS1, aP2DS2, aP2D; 
  gp_Vec2d aV2DT, aV2D, aV2DS1, aV2DS2;
  gp_Pnt aP;
  Handle(Geom2d_Curve) aC2D, aC2DS1, aC2DS2, aC2Dnew, aC2DoldCT;				     
  Handle(Geom2d_Curve) aC2Dold;				     
  Handle(Geom2d_TrimmedCurve) aC2DTnew;
  Handle(Geom_Surface) aS;
  TopoDS_Edge aES;
  BRep_Builder aBB;
  //
  iRet=0;
  aTol=BRep_Tool::Tolerance(aEnew);
  //
  // aC2DoldCT is alone p-curve of aEnew that we've built
  // The task is to build closed p-curves for aEnew
  aC2DoldCT=BRep_Tool::CurveOnSurface(aEnew, aF, aT1, aT2);
  //
  // aC2Dold is p-curve of aEold
  aC2Dold=BRep_Tool::CurveOnSurface(aEold, aF, aT1, aT2);
  //
  // As aEold is closed on aF, it is possible to retrieve
  // the two p-curves:  
  //  aC2DS1 -first p-curve 
  //  aC2DS2 -second p-curve
  aES=aEold;
  aES.Orientation(TopAbs_FORWARD);
  aC2DS1=BRep_Tool::CurveOnSurface(aES, aF, aTS1, aTS2);
  //
  aES.Orientation(TopAbs_REVERSED);
  aC2DS2=BRep_Tool::CurveOnSurface(aES, aF, aTS1, aTS2);
  //
  aTS=BOPTools_Tools2D::IntermediatePoint(aTS1, aTS2);
  //
  aC2DS1->D1(aTS, aP2DS1, aV2DS1);
  aC2DS2->D1(aTS, aP2DS2, aV2DS2);
  //
  // aV2DS12 - translation vector 
  gp_Vec2d aV2DS12(aP2DS1, aP2DS2);
  gp_Dir2d aD2DS12(aV2DS12);
  const gp_Dir2d& aD2DX=gp::DX2d();
  //
  // Directoion of closeness: U-Closed or V-Closed 
  aScPr=aD2DS12*aD2DX;
  bUClosed=Standard_True;
  if (fabs(aScPr) < aTol) {
    bUClosed=!bUClosed;
  }
  //
  aP2DS1.Coord(aUS1, aVS1);
  aP2DS2.Coord(aUS2, aVS2);
  //
  // aP - some 3D-point on seam edge of the surface aS
  aS=BRep_Tool::Surface(aF);
  aS->D0(aUS1, aVS1, aP);
  //
  GeomAPI_ProjectPointOnCurve& aProjPC=aCtx->ProjPC(aEnew);
  //
  aProjPC.Perform(aP);
  aNbPoints=aProjPC.NbPoints();
  if (!aNbPoints) {
    iRet=2;
    return iRet;
  }
  //
  // aT - parameter for aP on the curves of aEnew
  aT=aProjPC.LowerDistanceParameter();
  //
  aC2D=aC2DoldCT;
  aC2D->D1(aT, aP2D, aV2D);
  aP2D.Coord(aU, aV);
  //
  aC2Dnew=Handle(Geom2d_Curve)::DownCast(aC2D->Copy());
  aC2DTnew=new Geom2d_TrimmedCurve(aC2Dnew, aT1, aT2);
  //
  aV2DT=aV2DS12;
  if (!bUClosed) {    // V Closed
    if (fabs(aV-aVS2)<aTol) {
      aV2DT.Reverse();
    }
  }
  else {   // U Closed
    if (fabs(aU-aUS2)<aTol) {
      aV2DT.Reverse();
    }
  }
  //
  // Translate aC2DTnew
  aC2DTnew->Translate(aV2DT);
  //
  // 4 Order the 2D curves
  bRevOrder=Standard_False;
  aScPr=aV2D*aV2DS1;
  if(aScPr<0.) {
    bRevOrder=!bRevOrder;
  }
  //
  if (!bRevOrder) {
    aBB.UpdateEdge(aEnew, aC2D, aC2DTnew, aF, aTol);
  }
  else {
    aBB.UpdateEdge(aEnew, aC2DTnew, aC2D , aF, aTol);
  }
  return iRet;
}
//=======================================================================
//function : UpdateEdge
//purpose  : 
//=======================================================================
void UpdateEdge(const TopoDS_Edge& aE, 
		const Handle(Geom2d_Curve)& aC, 
		const TopoDS_Face& aF, 
		const Standard_Real aTol) 
{
  TopLoc_Location aLoc;
  const Handle(Geom_Surface)& aS = BRep_Tool::Surface(aF, aLoc);
  UpdateEdge(aE, aC, aS,aLoc, aTol);
}
//=======================================================================
//function : UpdateEdge
//purpose  : 
//=======================================================================
void UpdateEdge(const TopoDS_Edge& aE, 
		const Handle(Geom2d_Curve)& aC, 
		const Handle(Geom_Surface)& aS, 
		const TopLoc_Location& aLoc,
		const Standard_Real aTol) 
     
{
  const Handle(BRep_TEdge)& aTE = *((Handle(BRep_TEdge)*) &aE.TShape());
  TopLoc_Location aLocX = aLoc.Predivided(aE.Location());
  
  BRep_ListOfCurveRepresentation& aLCR=aTE->ChangeCurves();
  UpdateCurves(aLCR, aC, aS, aLocX);
  
  aTE->UpdateTolerance(aTol);
  aTE->Modified(Standard_True);
}
//=======================================================================
//function : UpdateCurves
//purpose  : 
//=======================================================================
void UpdateCurves(BRep_ListOfCurveRepresentation& aLCR,
		  const Handle(Geom2d_Curve)& aC,
		  const Handle(Geom_Surface)& aS,
		  const TopLoc_Location& aLoc)
{
  Handle(BRep_CurveRepresentation) aCR;
  Handle(BRep_GCurve) aGC;
  BRep_ListIteratorOfListOfCurveRepresentation aItLCR;
  //
  // search the range of the 3d curve
  // and remove any existing representation
  aItLCR.Initialize(aLCR);
  while (aItLCR.More()) {
    aCR = aItLCR.Value();
    aGC = Handle(BRep_GCurve)::DownCast(aCR);
    if (!aGC.IsNull()) {
      if (aGC->IsCurveOnSurface(aS, aLoc)) {
        // remove existing curve on surface
        aLCR.Remove(aItLCR);
      }
      else {
        aItLCR.Next();
      }
    }
    else {
      aItLCR.Next();
    }
  }
  //
  if (! aC.IsNull()) {
    Handle(BRep_CurveOnSurface) aCS = new BRep_CurveOnSurface(aC, aS, aLoc);
    aLCR.Append(aCS);
  }
}
//=======================================================================
//function : IsToReverse
//purpose  : 
//=======================================================================
Standard_Boolean IsToReverse(const TopoDS_Edge& aEold,
			     const TopoDS_Edge& aEnew,
			     const Handle(IntTools_Context)& theContext)
{
  Standard_Boolean bRet, bFlag, bIsDegenerated;
  Standard_Real aTnew, aTold, aScPr, aTa, aTb, aT1, aT2;
  gp_Vec aVold, aVnew;
  Handle(Geom_Curve) aCold, aCnew;
  gp_Vec aVE, aVS;
  gp_Pnt aP;
  //
  bRet=Standard_False;
  //
  bIsDegenerated=(BRep_Tool::Degenerated(aEold) ||
                  BRep_Tool::Degenerated(aEnew));
  if (bIsDegenerated) {
    return bRet;
  }
  //
  aCold=BRep_Tool::Curve(aEold, aT1, aT2);
  aCnew=BRep_Tool::Curve(aEnew, aTa, aTb);
  //
  if (aCold==aCnew) {
    return bRet;
  }
  //
  aTnew=BOPTools_Tools2D::IntermediatePoint(aTa, aTb);
  aCnew->D1(aTnew, aP, aVnew);
  aVnew.Normalize(); 
  //
  bFlag=theContext->ProjectPointOnEdge(aP, aEold, aTold);
  aCold->D1(aTold, aP, aVold);
  aVold.Normalize(); 
  //
  aScPr=aVnew*aVold;
  bRet=(aScPr<0.);
  //
  return bRet;
}
//=======================================================================
//function : IsClosed
//purpose  :
//=======================================================================
Standard_Boolean IsClosed(const TopoDS_Edge& aE,
			  const TopoDS_Face& aF)
{
  Standard_Boolean bRet;
  //
  bRet=BRep_Tool::IsClosed(aE, aF);
  if (bRet) {
    Standard_Integer iCnt;
    //
    iCnt=0;
    TopExp_Explorer aExp(aF, TopAbs_EDGE);
    for (; aExp.More(); aExp.Next()) {
      const TopoDS_Shape& aEx=aExp.Current();
      if(aEx.IsSame(aE)) {
	++iCnt;
      }
    }
    bRet=(iCnt==2);
  }
  return bRet;
}
