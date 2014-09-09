// Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "CurveCreator_Utils.hxx"
#include "CurveCreator.hxx"
#include "CurveCreator_UtilsICurve.hxx"

#include <GEOMUtils.hxx>

#include <gp_Pln.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Compound.hxx>

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Line.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_LocalContext.hxx>

#include <Geom_Point.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Line.hxx>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <SelectMgr_Selection.hxx>
#include <Select3D_SensitivePoint.hxx>

#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <TColgp_HArray1OfPnt.hxx>
#include <TColStd_HArray1OfBoolean.hxx>
#include <TColgp_Array1OfVec.hxx>
#include <GeomAPI_Interpolate.hxx>

#include <ProjLib.hxx>
#include <ElSLib.hxx>

#include <math.h>

#include "CurveCreator_ICurve.hxx"

const double LOCAL_SELECTION_TOLERANCE = 0.0001;
const int    SCENE_PIXEL_PROJECTION_TOLERANCE = 10;
const int    SCENE_PIXEL_POINT_TOLERANCE = 5;

//=======================================================================
// function : ConvertClickToPoint()
// purpose  : Returns the point clicked in 3D view
//=======================================================================
void CurveCreator_Utils::ConvertPointToClick( const gp_Pnt& thePoint,
                                              Handle(V3d_View) theView,
                                              int& x, int& y )
{
  theView->Convert(thePoint.X(), thePoint.Y(), thePoint.Z(), x, y );
}


//=======================================================================
// function : ConvertClickToPoint()
// purpose  : Returns the point clicked in 3D view
//=======================================================================
gp_Pnt CurveCreator_Utils::ConvertClickToPoint( int x, int y, Handle(V3d_View) aView )
{
  // the 3D point, that is a projection of the pixels to the XYZ view plane
  //return GEOMUtils::ConvertClickToPoint( x, y, aView );

  // we need the projection to the XOY plane
  // 1. find a point in the plane of the eye and the normal to the plane
  Standard_Real X, Y, Z;
  Quantity_Parameter Vx, Vy, Vz;
  aView->ConvertWithProj( x, y, X, Y, Z, Vx, Vy, Vz );

  // 2. build a ray from the point by the normal to the XOY plane and intersect it
  // The ray equation is the following : p(x,y,z) = p0(x,y,z) + t*V(x,y,z)
  // X,Y,Z - defines p0(x,y,z), Vx,Vy,Vz - defines V(x,y,z)
  // p(x,y,z) - is a searched point, t - should to be calculated by the condition of XOY plane
  // The system of equations is the following:
  // p(x) = p0(x)+t*V(x)
  // p(y) = p0(y)+t*V(y)
  // p(z) = p0(z)+t*V(z)
  // p(z) = 0

  Standard_Real aXp, aYp, aZp;
  //It is not possible to use Precision::Confusion(), because it is e-0.8, but V is sometimes e-6
  Standard_Real aPrec = LOCAL_SELECTION_TOLERANCE;
  if ( fabs( Vz ) > aPrec ) {
    Standard_Real aT = -Z/Vz;
    aXp = X + aT*Vx;
    aYp = Y + aT*Vy;
    aZp = Z + aT*Vz;
  }
  else { // Vz = 0 - the eyed plane is orthogonal to Z plane - XOZ, or YOZ
    aXp = aYp = aZp = 0;
    if ( fabs( Vy ) < aPrec ) // Vy = 0 - the YOZ plane
      aYp = Y;
    else if ( fabs( Vx ) < aPrec ) // Vx = 0 - the XOZ plane
      aXp = X;
  }
  /*std::cout << "ConvertClickToPoint: " << std::endl
            << "XYZ1 = (" << X << ", " << Y << ", " << Z << "); " << std::endl
            << "Vxyz = (" << Vx << ", " << Vy << ", " << Vz << "); " << std::endl
            << "Resp = (" << aXp << ", " << aYp << ", " << aZp << "); " << std::endl;*/

  gp_Pnt ResultPoint( aXp, aYp, aZp );
  return ResultPoint;
}

void CurveCreator_Utils::constructShape( const CurveCreator_ICurve* theCurve,
                                         TopoDS_Shape& theShape )
{
  BRep_Builder aBuilder;
  TopoDS_Compound aComp;
  aBuilder.MakeCompound( aComp );
  for( int iSection = 0 ; iSection < theCurve->getNbSections() ; iSection++ )
  {
    int theISection = iSection;

    CurveCreator::SectionType aSectType = theCurve->getSectionType( theISection );
    int aPointSize = theCurve->getNbPoints( theISection );
    if ( aPointSize == 0 )
      continue;

    bool aSectIsClosed = theCurve->isClosed( theISection );
    bool isPolyline = aSectType == CurveCreator::Polyline;

    int iPoint = 0;
    gp_Pnt aPrevPoint, aPoint;
    // filters the curve points to skip equal points
    std::vector<gp_Pnt> aPoints;
    CurveCreator_UtilsICurve::getPoint( theCurve, theISection, iPoint, aPoint );
    aPoints.push_back( aPoint );
    aPrevPoint = aPoint;
    iPoint++;
    for( ; iPoint < aPointSize; iPoint++ ) {
      CurveCreator_UtilsICurve::getPoint( theCurve, theISection, iPoint, aPoint );
      if ( !isEqualPoints( aPrevPoint, aPoint ) )
        aPoints.push_back( aPoint );
      aPrevPoint = aPoint;
    }
    int aNbPoints = aPoints.size();

    if ( aNbPoints == 1 ) {
      aPoint = aPoints.front();
      TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex( aPoint ).Vertex();
      aBuilder.Add( aComp, aVertex );
    }
    else if ( aNbPoints > 1 ) {
      Handle(TColgp_HArray1OfPnt) aHCurvePoints = new TColgp_HArray1OfPnt(1, aNbPoints);
      TColgp_Array1OfVec aTangents(1, aNbPoints);
      Handle(TColStd_HArray1OfBoolean) aTangentFlags = new TColStd_HArray1OfBoolean(1, aNbPoints);
      gp_Vec aNullVec(0, 0, 0);

      TopoDS_Edge aPointEdge;
      TopoDS_Vertex aVertex;

      std::vector<gp_Pnt>::const_iterator aPointIt = aPoints.begin(), aPointLast = aPoints.end();
      aPoint = *aPointIt;

      int aHIndex = 1;
      aVertex = BRepBuilderAPI_MakeVertex( aPoint ).Vertex();
      aBuilder.Add( aComp, aVertex );
      if ( !isPolyline ) {
        aHCurvePoints->SetValue( aHIndex, aPoint );
        aTangents.SetValue( aHIndex, aNullVec );
        aTangentFlags->SetValue( aHIndex, Standard_False );
        aHIndex++;
      }

      aPrevPoint = aPoint;
      aPointIt++;
      for( ; aPointIt != aPointLast; aPointIt++ ) {
        aPoint = *aPointIt;
        aVertex = BRepBuilderAPI_MakeVertex( aPoint ).Vertex();
        aBuilder.Add( aComp, aVertex );
        if ( isPolyline ) {
          TopoDS_Edge aPointEdge = BRepBuilderAPI_MakeEdge( aPrevPoint, aPoint ).Edge();
          aBuilder.Add( aComp, aPointEdge );
        }
        else {
          aHCurvePoints->SetValue( aHIndex, aPoint );
          aTangents.SetValue( aHIndex, aNullVec );
          aTangentFlags->SetValue( aHIndex, Standard_False );
          aHIndex++;
        }
        aPrevPoint = aPoint;
      }
      if( aSectIsClosed && ( aNbPoints > 2 ) ) {
        aPoint = aPoints.front();
        aVertex = BRepBuilderAPI_MakeVertex( aPoint ).Vertex();
        aBuilder.Add( aComp, aVertex );
        if ( isPolyline ) {
          aPointEdge = BRepBuilderAPI_MakeEdge( aPrevPoint, aPoint ).Edge();
          aBuilder.Add( aComp, aPointEdge );
        }
      }
      if( !isPolyline ) {
        // compute BSpline
        Handle(Geom_BSplineCurve) aBSplineCurve;
        GeomAPI_Interpolate aGBC(aHCurvePoints, aSectIsClosed, gp::Resolution());
        // correct the spline degree to be as 3 for non-periodic spline if number of points
        // less than 3. It is need to have a knot in each spline point. This knots are used
        // to found a neighbour points when a new point is inserted between two existing.
        if (!aSectIsClosed ) {
          if (aHCurvePoints->Length() == 3)
            aGBC.Load(aTangents, aTangentFlags);
        }

        aGBC.Perform();
        if ( aGBC.IsDone() )
          aBSplineCurve = aGBC.Curve();
        TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge( aBSplineCurve ).Edge();
        TopoDS_Wire aWire = BRepBuilderAPI_MakeWire( anEdge ).Wire();
        aBuilder.Add( aComp, aWire );
      }
    }
  }
  theShape = aComp;
}

class CompareSectionToPoint
{
public:
  CompareSectionToPoint( const int theISection = -1, const int theIPoint = -1 )
    : mySectionId( theISection ), myPointId( theIPoint ) {};
  ~CompareSectionToPoint() {}

  bool operator < ( const CompareSectionToPoint& theOther ) const
  {
    bool isLess = mySectionId < theOther.mySectionId;
    if ( !isLess && mySectionId == theOther.mySectionId )
      isLess = myPointId < theOther.myPointId;
    return isLess;
  }

private:
  int mySectionId;
  int myPointId;
};


void CurveCreator_Utils::getSelectedPoints( Handle(AIS_InteractiveContext) theContext,
                                            const CurveCreator_ICurve* theCurve,
                                            CurveCreator_ICurve::SectionToPointList& thePoints )
{
  thePoints.clear();

  std::list<float> aSelectedPoints;
  gp_Pnt aPnt;
  std::map<CompareSectionToPoint, int> aPointsMap;

  CurveCreator_ICurve::SectionToPointList aPoints;
  for ( theContext->InitSelected(); theContext->MoreSelected(); theContext->NextSelected() ) {
    TopoDS_Vertex aVertex;
    TopoDS_Shape aShape = theContext->SelectedShape();
    if ( !aShape.IsNull() && aShape.ShapeType() == TopAbs_VERTEX )
      aVertex = TopoDS::Vertex( theContext->SelectedShape() );

    if ( aVertex.IsNull() )
      continue;
    aPnt = BRep_Tool::Pnt( aVertex );

    CurveCreator_UtilsICurve::findSectionsToPoints( theCurve, aPnt.X(), aPnt.Y(), aPoints );
    CurveCreator_ICurve::SectionToPointList::const_iterator anIt = aPoints.begin(),
                                                            aLast = aPoints.end();
    CompareSectionToPoint aPoint;
    for ( ; anIt != aLast; anIt++ ) {
      aPoint = CompareSectionToPoint( (*anIt).first, (*anIt).second );
      if ( aPointsMap.find( aPoint ) != aPointsMap.end() )
        continue;
      aPointsMap[aPoint] = 0;

      thePoints.push_back( *anIt );
    }
  }
}

void CurveCreator_Utils::setSelectedPoints( Handle(AIS_InteractiveContext) theContext,
                                            const CurveCreator_ICurve* theCurve,
                                            const CurveCreator_ICurve::SectionToPointList& thePoints )
{
  if ( !theCurve )
    return;

  Handle(AIS_InteractiveObject) anAIS = theCurve->getAISObject();
  if ( anAIS.IsNull() )
    return;
  Handle(AIS_Shape) anAISShape = Handle(AIS_Shape)::DownCast( anAIS );
  if ( anAISShape.IsNull() )
    return;

  //ASL: we convert list of point indices to list of points coordinates
  int aSize = thePoints.size();
  std::vector<gp_Pnt> aPntsToSelect( aSize );

  CurveCreator_ICurve::SectionToPointList::const_iterator
                     aPIt = thePoints.begin(), aPLast = thePoints.end();
  CurveCreator_ICurve::SectionToPoint aSToPoint;
  for( int i=0; aPIt != aPLast; aPIt++, i++ )
  {
    gp_Pnt aPntToSelect;
    CurveCreator_UtilsICurve::getPoint( theCurve, aPIt->first, aPIt->second, aPntToSelect );
    aPntsToSelect[i] = aPntToSelect;
  }

  theContext->ClearSelected( Standard_False );
  //ASL: we switch off automatic highlight to improve performance of selection
  theContext->SetAutomaticHilight( Standard_False );

  Handle_SelectMgr_Selection aSelection = anAISShape->Selection( AIS_Shape::SelectionMode( TopAbs_VERTEX ) );
  for( aSelection->Init(); aSelection->More(); aSelection->Next() )
  {
    Handle_SelectBasics_SensitiveEntity aSenEntity = aSelection->Sensitive();
    Handle_Select3D_SensitivePoint aSenPnt = Handle_Select3D_SensitivePoint::DownCast( aSenEntity );

    gp_Pnt anOwnerPnt = aSenPnt->Point();
    Handle_SelectMgr_EntityOwner anOwner = Handle_SelectMgr_EntityOwner::DownCast( aSenPnt->OwnerId() );


    CurveCreator_ICurve::SectionToPointList::const_iterator anIt = thePoints.begin(),
                                                                   aLast = thePoints.end();
    bool isFound = false;
    for( int i=0; i<aSize; i++ )
    {
      bool isIntersect = fabs( aPntsToSelect[i].X() - anOwnerPnt.X() ) < LOCAL_SELECTION_TOLERANCE &&
                         fabs( aPntsToSelect[i].Y() - anOwnerPnt.Y() ) < LOCAL_SELECTION_TOLERANCE;
      if( isIntersect )
      {
        theContext->AddOrRemoveSelected( anOwner, Standard_False );
        break;
      }
    }
  }

  //ASL: we switch on again automatic highlight (otherwise selection will not be shown)
  //     and call HilightPicked to draw selected owners
  theContext->SetAutomaticHilight( Standard_True );
  theContext->LocalContext()->HilightPicked( Standard_True );
}

//=======================================================================
// function : setLocalPointContext
// purpose  : Open/close the viewer local context
//=======================================================================
void CurveCreator_Utils::setLocalPointContext( const CurveCreator_ICurve* theCurve,
                                               Handle(AIS_InteractiveContext) theContext,
                                               const bool theOpen )
{
  if ( !theContext )
    return;

  if ( theOpen ) {
    // Open local context if there is no one
    if ( !theContext->HasOpenedContext() ) {
      theContext->ClearCurrents( false );
      theContext->OpenLocalContext( false/*use displayed objects*/, true/*allow shape decomposition*/ );
    }
    // load the curve AIS object to the local context with the point selection
    Handle(AIS_InteractiveObject) anAIS = theCurve->getAISObject();
    if ( !anAIS.IsNull() )
    {
      if ( anAIS->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
      {
        theContext->Load( anAIS, -1/*selection mode*/, true/*allow decomposition*/ );
        theContext->Activate( anAIS, AIS_Shape::SelectionMode( (TopAbs_ShapeEnum)TopAbs_VERTEX ) );
      }
    }
  }
  else {
    if ( theContext->HasOpenedContext() )
      theContext->CloseAllContexts();
  }
}

bool CurveCreator_Utils::pointOnObject( Handle(V3d_View) theView,
                                        Handle(AIS_InteractiveObject) theObject,
                                        const int theX, const int theY,
                                        gp_Pnt& thePoint,
                                        gp_Pnt& thePoint1, gp_Pnt& thePoint2 )
{
  bool isFullFound = false;

  if ( theObject.IsNull() || theView.IsNull() )
    return isFullFound;
  Handle(AIS_Shape) aShape = Handle(AIS_Shape)::DownCast( theObject );
  if ( aShape.IsNull() )
    return isFullFound;
  const TopoDS_Compound& aCompound = TopoDS::Compound( aShape->Shape() );
  if ( aCompound.IsNull() )
    return isFullFound;

  gp_Pnt aCurPoint, aCurPoint1, aCurPoint2;
  gp_Pnt aFoundPoint, aFoundPnt1, aFoundPnt2;
  Standard_Real aParameter;
  bool isFound = false;
  int aDelta, aMinDelta = 2*SCENE_PIXEL_PROJECTION_TOLERANCE*SCENE_PIXEL_PROJECTION_TOLERANCE;
  TopExp_Explorer anExp( aCompound, TopAbs_EDGE );
  for ( ; anExp.More(); anExp.Next())
  {
    const TopoDS_Edge& anEdge = TopoDS::Edge(anExp.Current());
    if ( anEdge.IsNull() )
      continue;
    Standard_Real aFirst, aLast;
    Handle(Geom_Curve) aCurve = BRep_Tool::Curve( anEdge, aFirst, aLast );
    if ( aCurve->IsKind( STANDARD_TYPE(Geom_BSplineCurve) ) ) {
      Handle(Geom_BSplineCurve) aBSplineCurve =
                          Handle(Geom_BSplineCurve)::DownCast( aCurve );
      if ( !aBSplineCurve.IsNull() ) {
        isFound = hasProjectPointOnCurve( theView, theX, theY, aBSplineCurve,
                                          aParameter, aDelta );
        if ( isFound ) {
          aCurPoint = aBSplineCurve->Value( aParameter );
          Standard_Integer anI1, anI2;
          aBSplineCurve->LocateU( aParameter, LOCAL_SELECTION_TOLERANCE, anI1, anI2 );
          aCurPoint1 = aBSplineCurve->Value( aBSplineCurve->Knot( anI1 ) );
          aCurPoint2 = aBSplineCurve->Value( aBSplineCurve->Knot( anI2 ) );
        }
      }
    }
    else { // a curve built on a polyline edge
      Handle(Geom_Line) aGLine = Handle(Geom_Line)::DownCast( aCurve );
      if ( aGLine.IsNull() )
        continue;
      isFound = hasProjectPointOnCurve( theView, theX, theY, aGLine, aParameter,
                                        aDelta );
      if ( isFound ) {
        aCurPoint = aGLine->Value( aParameter );
        TopoDS_Vertex V1, V2;
        TopExp::Vertices( anEdge, V1, V2, Standard_True );
        if ( V1.IsNull() || V2.IsNull() )
          continue;
        aCurPoint1 = BRep_Tool::Pnt(V1);
        aCurPoint2 = BRep_Tool::Pnt(V2);

        // check that the projected point is on the bounded curve
        gp_Vec aVec1( aCurPoint1, aCurPoint );
        gp_Vec aVec2( aCurPoint2, aCurPoint );
        isFound = fabs( aVec1.Angle( aVec2 ) - M_PI ) < LOCAL_SELECTION_TOLERANCE;
      }
    }
    if ( isFound && aMinDelta >= aDelta ) {
      aMinDelta = aDelta;

      isFullFound = true;
      aFoundPnt1 = aCurPoint1;
      aFoundPnt2 = aCurPoint2;
      aFoundPoint = aCurPoint;
    }
  }
  if ( isFullFound ) {
    int aX, anY, aX1, anY1, aX2, anY2;
    int aDelta;
    CurveCreator_Utils::ConvertPointToClick( aFoundPoint, theView, aX, anY );
    CurveCreator_Utils::ConvertPointToClick( aFoundPnt1, theView, aX1, anY1 );
    CurveCreator_Utils::ConvertPointToClick( aFoundPnt2, theView, aX2, anY2 );

    isFullFound = !isEqualPixels( aX, anY, aX1, anY1, SCENE_PIXEL_POINT_TOLERANCE, aDelta ) &&
                  !isEqualPixels( aX, anY, aX2, anY2, SCENE_PIXEL_POINT_TOLERANCE, aDelta );
    if ( isFullFound ) {
      thePoint = aFoundPoint;
      thePoint1 = aFoundPnt1;
      thePoint2 = aFoundPnt2;
    }
  }
  return isFullFound;
}

bool CurveCreator_Utils::hasProjectPointOnCurve( Handle(V3d_View) theView,
                                                 const int theX, const int theY,
                                                 const Handle(Geom_Curve)& theCurve,
                                                 Standard_Real& theParameter,
                                                 int& theDelta )
{
  bool isFound = false;
  if ( theView.IsNull() )
    return isFound;

  gp_Pnt aPoint = CurveCreator_Utils::ConvertClickToPoint( theX, theY, theView );

  GeomAPI_ProjectPointOnCurve aProj( aPoint, theCurve );
  Standard_Integer aNbPoint = aProj.NbPoints();
  if (aNbPoint > 0) {
    for (Standard_Integer j = 1; j <= aNbPoint && !isFound; j++) {
      gp_Pnt aNewPoint = aProj.Point( j );
      theParameter = aProj.Parameter( j );

      int aX, anY;
      CurveCreator_Utils::ConvertPointToClick( aNewPoint, theView, aX, anY );

      isFound = isEqualPixels( aX, anY, theX, theY, SCENE_PIXEL_PROJECTION_TOLERANCE, theDelta );
    }
  }
  return isFound;
}

bool CurveCreator_Utils::isEqualPixels( const int theX, const int theY, const int theOtherX,
                                        const int theOtherY, const double theTolerance, int& theDelta )
{
  int aXDelta = abs( theX - theOtherX );
  int anYDelta = abs( theY - theOtherY );

  theDelta = aXDelta*aXDelta + anYDelta*anYDelta;

  return aXDelta < theTolerance && anYDelta < theTolerance;
}

bool CurveCreator_Utils::isEqualPoints( const gp_Pnt& thePoint, const gp_Pnt& theOtherPoint )
{
  return theOtherPoint.IsEqual( thePoint, LOCAL_SELECTION_TOLERANCE );
}
