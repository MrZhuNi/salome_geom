// GEOM GEOMGUI : GUI for Geometry component
//
// Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// File   : BasicGUI_PlaneDlg.h
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//

#ifndef BASICGUI_PLANEDLG_H
#define BASICGUI_PLANEDLG_H

#include <GEOMBase_Skeleton.h>

class DlgRef_1Sel1Spin;
class DlgRef_2Sel1Spin;
class DlgRef_3Sel1Spin;

//=================================================================================
// class    : BasicGUI_PlaneDlg
// purpose  :
//=================================================================================
class BasicGUI_PlaneDlg : public GEOMBase_Skeleton
{ 
  Q_OBJECT

public:
  BasicGUI_PlaneDlg( GeometryGUI*, QWidget* = 0,
		     const char* = 0, bool = false, Qt::WindowFlags = 0 );
  ~BasicGUI_PlaneDlg();
  
protected:
  // redefined from GEOMBase_Helper
  virtual GEOM::GEOM_IOperations_ptr createOperation();
  virtual bool                       isValid( QString& );
  virtual bool                       execute( ObjectList& );
  
  virtual void                       closeEvent( QCloseEvent* );

private:
  void                               Init();
  void                               enterEvent( QEvent* );
  double                             getSize() const;

private:
  GEOM::GEOM_Object_var              myPoint, myDir, myPoint1, myPoint2, myPoint3, myFace;

  DlgRef_2Sel1Spin*                  GroupPntDir;
  DlgRef_3Sel1Spin*                  Group3Pnts;
  DlgRef_1Sel1Spin*                  GroupFace;

private slots:
  void                               ClickOnOk();
  void                               ClickOnCancel();
  bool                               ClickOnApply();
  
  void                               ActivateThisDialog();
  void                               DeactivateActiveDialog();
  
  void                               SelectionIntoArgument();
  void                               LineEditReturnPressed();
  void                               SetEditCurrentArgument();
  void                               ConstructorsClicked( int );
  void                               ValueChangedInSpinBox( double );
};

#endif // BASICGUI_PLANEDLG_H
