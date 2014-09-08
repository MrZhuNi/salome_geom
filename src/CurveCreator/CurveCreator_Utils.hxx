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

#ifndef CURVECREATOR_UTILS_H
#define CURVECREATOR_UTILS_H

#include "CurveCreator_Macro.hxx"
#include "CurveCreator_ICurve.hxx"

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx> // TODO: remove
#include <V3d_View.hxx>
#include <gp_Pnt.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS_Shape.hxx>

#include <list>
#include <vector> // TODO: remove


class CurveCreator_Utils
{
public:

    /*!
   * \brief Returns the point clicked in 3D view.
   *
   * \param x The X coordinate in the view.
   * \param y The Y coordinate in the view.
   * \param theView View where the given point takes place.
   * \retval gp_Pnt Returns the point clicked in 3D view
   */
  CURVECREATOR_EXPORT static void ConvertPointToClick( const gp_Pnt& thePoint,
                                                       Handle(V3d_View) theView,
                                                       int& x, int& y );

  /*!
   * \brief Returns the point clicked in 3D view.
   *
   * \param x The X coordinate in the view.
   * \param y The Y coordinate in the view.
   * \param theView View where the given point takes place.
   * \retval gp_Pnt Returns the point clicked in 3D view
   */
  CURVECREATOR_EXPORT static gp_Pnt ConvertClickToPoint( int x, int y,
                                                         Handle(V3d_View) theView );

  /**
   * Generates shape on the curve
   * \param theCurve a curve object, that contains data
   * \param theShape a generated shape
   */
  CURVECREATOR_EXPORT static void constructShape( const CurveCreator_ICurve* theCurve,
                                                  TopoDS_Shape& theShape );

  /**
   * Find selected points in the context
   * \param theContext the viewer context
   * \param theCurve a curve object, that contains data
   */
  CURVECREATOR_EXPORT static void getSelectedPoints( Handle(AIS_InteractiveContext) theContext,
                                         const CurveCreator_ICurve* theCurve,
                                         CurveCreator_ICurve::SectionToPointList& thePoints );

  /**
   * Set selected points to the context
   * \param theContext the viewer context
   * \param theCurve a curve object, that contains data
   * \param thePoints the curve point indices to be selected in the context
   */
  CURVECREATOR_EXPORT static void setSelectedPoints(
                                         Handle(AIS_InteractiveContext) theContext,
                                         const CurveCreator_ICurve* theCurve,
                                         const CurveCreator_ICurve::SectionToPointList& thePoints =
                                               CurveCreator_ICurve::SectionToPointList() );

  /*!
   * \brief Sets the local point context for the 3D viewer.
   * \param theCurve a curve object, that contains data
   * \param theContext the viewer context
   * \param theOpen The flag to open or close the local context.
   */
  CURVECREATOR_EXPORT static void setLocalPointContext(
                                        const CurveCreator_ICurve* theCurve,
                                        Handle(AIS_InteractiveContext) theContext,
                                        const bool theOpen );

  /**
   * Checks whether the point belongs to the OCC object
   * \param theObject a line or shape with a bspline inside
   * \param theX the X coordinate in the view.
   * \param theY the Y coordinate in the view.
   * \param thePoint the output point to be append to the model curve
   * \param thePoint1 the output point to bound the line where a new point should be inserted
   * \param thePoint2 the output point to bound the line where a new point should be inserted
   */
  CURVECREATOR_EXPORT static bool pointOnObject( Handle(V3d_View) theView,
                                                 Handle(AIS_InteractiveObject) theObject,
                                                 const int theX, const int theY,
                                                 gp_Pnt& thePoint, gp_Pnt& thePoint1,
                                                 gp_Pnt& thePoint2 );

protected:
  /*
   * Returns whether the clicked point belong to the curve or has a very near projection
   * \param theX the X coordinate of a point clicked in the OCC viewer
   * \param theY the Y coordinate of a point clicked in the OCC viewer
   * \param theCurve a geometry curve
   * \param theOutPoint a found projected point on the curve
  */
  CURVECREATOR_EXPORT static bool hasProjectPointOnCurve(
                                                 Handle(V3d_View) theView,
                                                 const int theX, const int theY,
                                                 const Handle(Geom_Curve)& theCurve,
                                                 Standard_Real& theParameter,
                                                 int& theDelta );

  /*
   * Returns whether the X and Y coordinates is in the pixel tolerance
   * \param theX the X coordinate of the first point
   * \param theY the Y coordinate of the first point
   * \param theOtherX the X coordinate of the second point
   * \param theOtherY the Y coordinate of the second point
   * \param theTolerance the tolerance to compare
   * \param theDelta the sum of the a square of X and a square of Y
   * \returns whether the points are provide to the pixel tolerance
  */
  CURVECREATOR_EXPORT static bool isEqualPixels( const int theX, const int theY,
                                                 const int theOtherX, const int theOtherY,
                                                 const double theTolerance, int& theDelta );


  /*
   * Returns whether the points are the same
   * \param thePoint the first point
   * \param theOtherPoint the second point
   * \returns whether the points are provide to the pixel tolerance
  */
  CURVECREATOR_EXPORT static bool isEqualPoints( const gp_Pnt& thePoint,
                                                 const gp_Pnt& theOtherPoint );
};

#endif // CURVECREATOR_UTILS_H
