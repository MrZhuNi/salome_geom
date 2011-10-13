//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#ifndef ADVANCEDGUI_SMOOTHINGSURFACEDLG_H
#define ADVANCEDGUI_SMOOTHINGSURFACEDLG_H

#include <GEOMBase_Skeleton.h>
#include "GEOM_GenericObjPtr.h"

#include <list>

class DlgRef_1Sel1Check;

//=================================================================================
// class    : AdvancedGUI_SmoothingSurfaceDlg
// purpose  :
//=================================================================================
class AdvancedGUI_SmoothingSurfaceDlg : public GEOMBase_Skeleton
{
  Q_OBJECT

public:
  AdvancedGUI_SmoothingSurfaceDlg( GeometryGUI*, QWidget* = 0 );
  ~AdvancedGUI_SmoothingSurfaceDlg();

protected:
  // redefined from GEOMBase_Helper
  virtual GEOM::GEOM_IOperations_ptr createOperation();
  virtual bool                       isValid( QString& );
  virtual bool                       execute( ObjectList& );

private:
  void                               Init();
  void                               enterEvent( QEvent* );
  void                               synchronize( QList<GEOM::GeomObjPtr>& left, QList<GEOM::GeomObjPtr>& right );

private:
  DlgRef_1Sel1Check*                 GroupPoints;
  QList<GEOM::GeomObjPtr>            myPoints;

private slots:
  void                               ClickOnOk();
  bool                               ClickOnApply();
  
  void                               ActivateThisDialog();
  void                               DeactivateActiveDialog();
  
  void                               CheckButtonToggled();
  void                               SelectionIntoArgument();
  void                               SetEditCurrentArgument();
  //void                               ValueChangedInSpinBox();
  //void                               SetDoubleSpinBoxStep( double );
};

#endif // ADVANCEDGUI_SMOOTHINGSURFACEDLG_H
