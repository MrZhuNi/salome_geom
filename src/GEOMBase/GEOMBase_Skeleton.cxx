//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : GEOMBase_Skeleton.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "GEOMBase_Skeleton.h"

//=================================================================================
// class    : GEOMBase_Skeleton()
// purpose  : Constructs a GEOMBase_Skeleton which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GEOMBase_Skeleton::GEOMBase_Skeleton(QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl)
  :DlgRef_Skeleton_QTD(parent, name, modal, fl)
{
  if (!name)
    setName("GEOMBase_Skeleton");

  buttonCancel->setText(tr("GEOM_BUT_CLOSE"));
  buttonOk->setText(tr("GEOM_BUT_OK"));
  buttonApply->setText(tr("GEOM_BUT_APPLY"));

  GroupMedium->close(TRUE);
  resize(0, 0);

  Init(Sel);
}


//=================================================================================
// function : ~GEOMBase_Skeleton()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GEOMBase_Skeleton::~GEOMBase_Skeleton()
{
  // no need to delete child widgets, Qt does it all for us
  this->destroy(TRUE, TRUE);
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::Init(SALOME_Selection* Sel)
{
  /* init variables */
  mySelection = Sel;
  mySimulationTopoDs.Nullify();

  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  myGeomGUI->SetActiveDialogBox((QDialog*)this);
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  /* Move widget on the botton right corner of main widget */
//   int x, y;
//   myGeomGUI->DefineDlgPosition( this, x, y );

  /* displays Dialog */
  RadioButton1->setChecked(TRUE);

  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::ClickOnCancel()
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  mySelection->ClearFilters();
  disconnect(mySelection, 0, this, 0);

  myGeomGUI->ResetState();
  reject();

  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::LineEditReturnPressed()
{
  /* User name of object input management                          */
  /* If successfull the selection is changed and signal emitted... */
  /* so SelectionIntoArgument() is automatically called.           */
  const QString objectUserName = myEditCurrentArgument->text();
  QWidget* thisWidget = (QWidget*)this;
  if( myGeomGUI->SelectionByNameInDialogs(thisWidget, objectUserName, mySelection))
    myEditCurrentArgument->setText(objectUserName);

  return;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::DeactivateActiveDialog()
{
  this->setEnabled(false);
  mySelection->ClearFilters();
  disconnect(mySelection, 0, this, 0);
  myGeomGUI->EraseSimulationShape();
  myGeomGUI->SetActiveDialogBox(0);
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog();
  this->setEnabled(true);
  myGeomGUI->SetActiveDialogBox((QDialog*)this);
  return;
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::closeEvent(QCloseEvent* e)
{
  /* same than click on cancel button */
  this->ClickOnCancel();
  return;
}
