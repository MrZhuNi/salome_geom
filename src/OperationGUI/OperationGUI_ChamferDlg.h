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
// File   : OperationGUI_ChamferDlg.h
// Author : Damien COQUERET, Open CASCADE S.A.S.
//

#ifndef OPERATIONGUI_CHAMFERDLG_H
#define OPERATIONGUI_CHAMFERDLG_H

#include <GEOMBase_Skeleton.h>

#include <TColStd_IndexedMapOfInteger.hxx>

#include <QMap>

class QDoubleSpinBox;
class QGroupBox;
class QPushButton;
class QLineEdit;
class QDoubleSpinBox;
class QGridLayout;

//=================================================================================
// class    : OperationGUI_ChamferDlg
// purpose  :
//=================================================================================
class OperationGUI_ChamferDlg : public GEOMBase_Skeleton
{ 
  Q_OBJECT

  enum { MainObj1, MainObj2, Face1, Face2, MainObj3, Faces };
  enum { SpinBox1, SpinBox21, SpinBox22, SpinBox31, SpinBox32 };

public:
  OperationGUI_ChamferDlg( GeometryGUI*, QWidget* );
  virtual ~OperationGUI_ChamferDlg();

protected:
  // redefined from GEOMBase_Helper
  virtual                             GEOM::GEOM_IOperations_ptr createOperation();
  virtual                             bool isValid( QString& );
  virtual                             bool execute( ObjectList& );    

private slots:
  void                                ClickOnOk();
  bool                                ClickOnApply();
  void                                ActivateThisDialog();
  void                                LineEditReturnPressed();
  void                                SelectionIntoArgument();
  void                                SetEditCurrentArgument();
  void                                ValueChangedInSpinBox( double );
  void                                ConstructorsClicked( int );

private:
  void                                Init();
  void                                enterEvent( QEvent* );
  void                                reset();
  void                                createSelWg( const QString&, QPixmap&, QWidget*, 
						   QGridLayout*, const int );
  void                                activateSelection();
  void                                enableWidgets();
  
private:
  int                                 myConstructorId;
  
  GEOM::GEOM_Object_var               myShape; 
  QMap< int, int >                    myFace;  // indexes of faces from second tab ( Face1,2 )
  TColStd_IndexedMapOfInteger         myFaces; // indexes of faces from first tab ( Faces )
  
  QGroupBox*                          myGrp1;
  QGroupBox*                          myGrp2;
  QGroupBox*                          myGrp3;
  
  QMap< int, QPushButton* >           mySelBtn;
  QMap< int, QLineEdit* >             mySelName;
  QMap< int, QDoubleSpinBox* >        mySpinBox;
};

#endif // OPERATIONGUI_CHAMFERDLG_H
