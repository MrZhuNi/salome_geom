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

// File:        CurveCreator_PointItem.cxx
// Created:     Wed Jul  10 12:17:47 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_PointItem.h>
#include <QVariant>


//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_PointItem::CurveCreator_PointItem
                        (const CurveCreator::TypeCoord  theX,
                         const CurveCreator::TypeCoord  theY,
                               QListWidget             *theParent)
  : QListWidgetItem(theParent, ITEM_TYPE_XY)
{
  setCoord(theX, theY);
}

//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_PointItem::CurveCreator_PointItem
                        (const CurveCreator::TypeCoord  theX,
                         const CurveCreator::TypeCoord  theY,
                         const CurveCreator::TypeCoord  theZ,
                               QListWidget             *theParent)
  : QListWidgetItem(theParent, ITEM_TYPE_XYZ)
{
  setCoord(theX, theY, theZ);
}

//=======================================================================
// function: Destructor
// purpose:
//=======================================================================
CurveCreator_PointItem::~CurveCreator_PointItem()
{
}

//=======================================================================
// function: getText
// purpose:
//=======================================================================
QString CurveCreator_PointItem::getText(const CurveCreator::TypeCoord theX,
                                        const CurveCreator::TypeCoord theY)
{
  return QObject::tr("CC_PNT_ITEM_X_Y").arg(theX).arg(theY);
}

//=======================================================================
// function: getText
// purpose:
//=======================================================================
QString CurveCreator_PointItem::getText(const CurveCreator::TypeCoord theX,
                                        const CurveCreator::TypeCoord theY,
                                        const CurveCreator::TypeCoord theZ)
{
  return QObject::tr("CC_PNT_ITEM_X_Y_Z").arg(theX).arg(theY).arg(theZ);
}

//=======================================================================
// function: setCoord
// purpose:
//=======================================================================
void CurveCreator_PointItem::setCoord(const CurveCreator::TypeCoord theX,
                                      const CurveCreator::TypeCoord theY)
{
  if (!is3d()) {
    setData(ROLE_X, theX);
    setData(ROLE_Y, theY);
    setText(getText(theX, theY));
  }
}

//=======================================================================
// function: setCoord
// purpose:
//=======================================================================
void CurveCreator_PointItem::setCoord(const CurveCreator::TypeCoord theX,
                                      const CurveCreator::TypeCoord theY,
                                      const CurveCreator::TypeCoord theZ)
{
  if (is3d()) {
    setData(ROLE_X, theX);
    setData(ROLE_Y, theY);
    setData(ROLE_Z, theZ);
    setText(getText(theX, theY, theZ));
  }
}

//=======================================================================
// function: getX
// purpose:
//=======================================================================
CurveCreator::TypeCoord CurveCreator_PointItem::getX() const
{
  return data(ROLE_X).value<CurveCreator::TypeCoord>();
}

//=======================================================================
// function: getY
// purpose:
//=======================================================================
CurveCreator::TypeCoord CurveCreator_PointItem::getY() const
{
  return data(ROLE_Y).value<CurveCreator::TypeCoord>();
}

//=======================================================================
// function: getZ
// purpose:
//=======================================================================
CurveCreator::TypeCoord CurveCreator_PointItem::getZ() const
{
  return (is3d() ? data(ROLE_Z).value<CurveCreator::TypeCoord>() : 0.);
}

//=======================================================================
// function: is3d
// purpose:
//=======================================================================
bool CurveCreator_PointItem::is3d() const
{
  return (type() == ITEM_TYPE_XYZ);
}
