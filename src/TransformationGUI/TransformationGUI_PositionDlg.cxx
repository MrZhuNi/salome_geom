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
//  File   : TransformationGUI_PositionDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "TransformationGUI_PositionDlg.h"

#include "DisplayGUI.h"

#include <AIS_InteractiveContext.hxx>

//=================================================================================
// class    : TransformationGUI_PositionDlg()
// purpose  : Constructs a TransformationGUI_PositionDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
TransformationGUI_PositionDlg::TransformationGUI_PositionDlg(QWidget* parent,  const char* name, TransformationGUI* theTransformationGUI, SALOME_Selection* Sel, Handle(AIS_InteractiveContext) ic, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_POS_POINT")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_POS_VECT")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_POS_FACE")));
  QPixmap image3(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_POSITION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_POSITION"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->setPixmap(image2);

  Group1 = new DlgRef_2Sel_QTD(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_POS_VERTEX"));
  Group1->TextLabel1->setText(tr("GEOM_OBJECT_I").arg("1"));
  Group1->TextLabel2->setText(tr("GEOM_OBJECT_I").arg("2"));
  Group1->PushButton1->setPixmap(image3);
  Group1->PushButton2->setPixmap(image3);

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myTransformationGUI = theTransformationGUI;
  Init(ic);
}


//=================================================================================
// function : ~TransformationGUI_PositionDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
TransformationGUI_PositionDlg::~TransformationGUI_PositionDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::Init(Handle (AIS_InteractiveContext) ic)
{
  /* init variables */
  myConstructorId = 0;
  myShapeType = 7;
  myEditCurrentArgument = Group1->LineEdit1;

  myOkBase1 = myOkBase2 = myOkObj1 = myOkObj2 = false;
  myIC = ic;
  myLocalContextId = -1;
  myUseLocalContext = false;

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group1->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(Group1->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group1->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument())) ;

  /* displays Dialog */
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void TransformationGUI_PositionDlg::ConstructorsClicked(int constructorId)
{
  resize(0, 0);
  myConstructorId = constructorId;
  disconnect(mySelection, 0, this, 0);
  myOkBase1 = myOkBase2 = myOkObj1 = myOkObj2 = false;
  myEditCurrentArgument = Group1->LineEdit1;
  Group1->LineEdit1->setText("");
  Group1->LineEdit2->setText("");

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myUseLocalContext) {
    myIC->CloseLocalContext(myLocalContextId);
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true);
    myUseLocalContext = false;
  }

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  if(constructorId == 0) {
    Group1->GroupBox1->setTitle(tr("GEOM_POS_VERTEX"));
    myShapeType = 7; //Vertex
  }
  else if(constructorId == 1) {
    Group1->GroupBox1->setTitle(tr("GEOM_POS_EDGE"));
    myShapeType = 6; //Edge
  }
  else if(constructorId == 2) {
    Group1->GroupBox1->setTitle(tr("GEOM_POS_FACE"));
    myShapeType = 4; //Face
  }

 return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ClickOnOk()
{
  this->ClickOnApply();
  this->ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  if(myEditCurrentArgument == Group1->LineEdit2 && myUseLocalContext) {
    if(myOkBase2) {
      myIC->InitSelected();
      if(myIC->NbSelected() == 1) {
	myIC->InitSelected();
	myIC->MoreSelected();
	myObj2 = myIC->SelectedShape();
	myOkObj2 = true;
      }
    }
  }
  else if(myEditCurrentArgument == Group1->LineEdit1 && myUseLocalContext) {
    if(myOkBase1) {
      myIC->InitSelected();
      if(myIC->NbSelected() == 1) {
	myIC->InitSelected();
	myIC->MoreSelected();
	myObj1 = myIC->SelectedShape();
	myOkObj1 = true;
      }
    }
  }

  if(myOkBase1 && myOkBase2 && myOkObj1 && myOkObj2)
    myTransformationGUI->MakePositionAndDisplay(myGeomShape1, myGeomShape2, myShape1, myShape2, myObj1, myObj2);

  this->ResetStateOfDialog();
  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ClickOnCancel()
{
  this->ResetStateOfDialog();
  GEOMBase_Skeleton::ClickOnCancel();
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void TransformationGUI_PositionDlg::SelectionIntoArgument()
{
  if(myEditCurrentArgument == Group1->LineEdit1 && myUseLocalContext) {
    if(myOkBase2) {
      myIC->InitSelected();
      if(myIC->NbSelected() == 1) {
	myIC->InitSelected();
	myIC->MoreSelected();
	myObj2 = myIC->SelectedShape();
	myOkObj2 = true;
      }
    }
  }
  else if(myEditCurrentArgument == Group1->LineEdit2 && myUseLocalContext) {
    if(myOkBase1) {
      myIC->InitSelected();
      if(myIC->NbSelected() == 1) {
	myIC->InitSelected();
	myIC->MoreSelected();
	myObj1 = myIC->SelectedShape();
	myOkObj1 = true;
      }
    }
  }

  this->ResetStateOfDialog();
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */
  
  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1)
    return;
  
  // nbSel == 1
  TopoDS_Shape S; 
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  if(!myGeomBase->GetTopoFromSelection(mySelection, S))
    return;

  if(myEditCurrentArgument == Group1->LineEdit1) {
    myShape1 = S;
    myGeomShape1 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
    if(!testResult)
      return;
    myEditCurrentArgument->setText(aString);
    myOkBase1 = true;
  }
  else if(myEditCurrentArgument == Group1->LineEdit2) {
    myShape2 = S;
    myGeomShape2 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
    if(!testResult)
      return;
    myEditCurrentArgument->setText(aString);
    myOkBase2 = true;
  }

  /* local context is defined into the method */
  DisplayGUI* myDisplayGUI = new DisplayGUI();
  myDisplayGUI->PrepareSubShapeSelection(myShapeType, myLocalContextId);  
  myUseLocalContext = true;

  return; 
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if(send == Group1->PushButton1) {
    Group1->LineEdit1->setFocus();
    myEditCurrentArgument = Group1->LineEdit1;
    myOkBase1 = false;
    myOkObj1 = false;
  }
  else if(send == Group1->PushButton2) {
    Group1->LineEdit2->setFocus();
    myEditCurrentArgument = Group1->LineEdit2;
    myOkBase2 = false;
    myOkObj2 = false;
  }
  mySelection->ClearIObjects();
  this->SelectionIntoArgument();

  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();
  if(send == Group1->LineEdit1)
    myEditCurrentArgument = Group1->LineEdit1;
  else if (send == Group1->LineEdit2)
    myEditCurrentArgument = Group1->LineEdit2;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::DeactivateActiveDialog()
{
  this->ResetStateOfDialog();
  GEOMBase_Skeleton::DeactivateActiveDialog();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  Group1->LineEdit1->setFocus();
  myEditCurrentArgument = Group1->LineEdit1;
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::closeEvent(QCloseEvent* e)
{ 
  /* same than click on cancel button */
  this->ClickOnCancel();
  return;
}


//=================================================================================
// function : ResetStateOfDialog()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ResetStateOfDialog()
{
  /* Close its local contact if opened */
  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myUseLocalContext) {
    myIC->CloseLocalContext(myLocalContextId);
    myUseLocalContext = false;
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll();
  }
  return;
}
