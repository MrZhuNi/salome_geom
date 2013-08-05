// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

// File:        CurveCreator_PointItem.h
// Created:     Wed Jul  10 12:17:40 2013
// Author:      Sergey KHROMOV
//


#ifndef _CurveCreator_PointItem_HeaderFile
#define _CurveCreator_PointItem_HeaderFile


#include <QListWidgetItem>
#include <CurveCreator.hxx>


#define ITEM_TYPE_XY  QListWidgetItem::UserType + 1
#define ITEM_TYPE_XYZ QListWidgetItem::UserType + 2
#define ROLE_X        Qt::UserRole + 1
#define ROLE_Y        Qt::UserRole + 2
#define ROLE_Z        Qt::UserRole + 3

/**
 * This class represents a list widget item to facilitate storage
 * of 2d or 3d points in a list widget. The type of a stored point
 * (2d or 3d) is determined by a constructor chosen to create an object.
 */
class CurveCreator_PointItem : public QListWidgetItem
{

public:

  /**
   * Constructor. Initializes the object with 2d point.
   */
  CurveCreator_PointItem(const CurveCreator::TypeCoord theX,
                         const CurveCreator::TypeCoord theY,
                         QListWidget * theParent = 0);

  /**
   * Constructor. Initializes the object with 3d point.
   */
  CurveCreator_PointItem(const CurveCreator::TypeCoord theX,
                         const CurveCreator::TypeCoord theY,
                         const CurveCreator::TypeCoord theZ,
                         QListWidget * theParent = 0);

  /**
   * Destructor
   */
  ~CurveCreator_PointItem();

  /**
   * This static method is used to construct the text to be displayed by this
   * item for 2d point.
   */
  static QString getText(const CurveCreator::TypeCoord theX,
                         const CurveCreator::TypeCoord theY);

  /**
   * This static method is used to construct the text to be displayed by this
   * item for 3d point.
   */
  static QString getText(const CurveCreator::TypeCoord theX,
                         const CurveCreator::TypeCoord theY,
                         const CurveCreator::TypeCoord theZ);

  /**
   * This method sets the coordinates for 2d point. If the object has type
   * 3d point this method does nothing.
   */
  void setCoord(const CurveCreator::TypeCoord theX,
                const CurveCreator::TypeCoord theY);

  /**
   * This method sets the coordinates for 3d point. If the object has type
   * 2d point this method does nothing.
   */
  void setCoord(const CurveCreator::TypeCoord theX,
                const CurveCreator::TypeCoord theY,
                const CurveCreator::TypeCoord theZ);

  /**
   * Query the X coordinate of the point.
   */
  CurveCreator::TypeCoord getX() const;

  /**
   * Query the Y coordinate of the point.
   */
  CurveCreator::TypeCoord getY() const;

  /**
   * Query the Z coordinate of the point. Return 0 for the type 2d point.
   */
  CurveCreator::TypeCoord getZ() const;

  /**
   * Returns true if the type of the stored point is 3d; false otherwise.
   */
  bool is3d() const;

};

#endif
