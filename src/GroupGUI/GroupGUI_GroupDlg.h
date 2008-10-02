// GEOM GEOMGUI : GUI for Geometry component
//
// Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// File   : GroupGUI_GroupDlg.h
// Author : Sergey ANIKIN, Open CASCADE S.A.S. (sergey.anikin@opencascade.com)
//

#ifndef GROUPGUI_GROUPDLG_H
#define GROUPGUI_GROUPDLG_H

#include <GEOMBase_Skeleton.h>

#include <TopAbs_ShapeEnum.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>

class QGroupBox;
class QLineEdit;
class QListWidget;
class QButtonGroup;

//=================================================================================
// class    : GroupGUI_GroupDlg
// purpose  :
//=================================================================================
class GroupGUI_GroupDlg : public GEOMBase_Skeleton
{
  Q_OBJECT

public:
  typedef enum {
    CreateGroup,
    EditGroup
  } Mode;

  GroupGUI_GroupDlg( Mode mode, GeometryGUI*, QWidget* parent = 0 );
  ~GroupGUI_GroupDlg();

protected:
  // redefined from GEOMBase_Helper
  virtual GEOM::GEOM_IOperations_ptr  createOperation();
  virtual bool                        isValid( QString& );
  virtual bool                        execute( ObjectList& );
  virtual GEOM::GEOM_Object_ptr       getFather( GEOM::GEOM_Object_ptr );
  
  void                                closeEvent( QCloseEvent* );

private slots:
  void                                ClickOnOk();
  bool                                ClickOnApply();
  void                                ActivateThisDialog();
  void                                LineEditReturnPressed();
  void                                SelectionIntoArgument();
  void                                SetEditCurrentArgument();
  void                                ConstructorsClicked( int );
  
  void                                selectAllSubShapes();
  void                                add();
  void                                remove();
  void                                selectionChanged();
  
private:
  void                                Init();
  void                                enterEvent( QEvent* );
  int                                 subSelectionWay() const;    
  TopAbs_ShapeEnum                    getShapeType() const;
  void                                setShapeType( const TopAbs_ShapeEnum );
  void                                activateSelection();
  void                                updateState();
  void                                highlightSubShapes();
  void                                onGetInPlace();
  void                                setInPlaceObj( GEOM::GEOM_Object_var );

private:
  Mode                                myMode;
  bool                                myBusy;
  GEOM::GEOM_Object_var               myMainObj;
  GEOM::GEOM_Object_var               myGroup;
  GEOM::GEOM_Object_var               myInPlaceObj;
  int                                 myInPlaceObjSelectWay;
  TColStd_DataMapOfIntegerInteger     myMain2InPlaceIndices;

  QPushButton*                        mySelBtn;
  QLineEdit*                          myMainName;
  QPushButton*                        mySelBtn2;
  QLineEdit*                          myShape2Name;
  QGroupBox*                          mySelectionWayGroupBox;
  QButtonGroup*                       mySelectionWayGroup;
  QPushButton*                        mySelAllBtn;
  QPushButton*                        myAddBtn;
  QPushButton*                        myRemBtn;
  QListWidget*                        myIdList;
};

#endif
