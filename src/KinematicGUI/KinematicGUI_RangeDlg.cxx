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
//  File   : KinematicGUI_RangeDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "KinematicGUI_RangeDlg.h"

using namespace std;

//=================================================================================
// class    : KinematicGUI_RangeDlg()
// purpose  : Constructs a KinematicGUI_RangeDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_RangeDlg::KinematicGUI_RangeDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, int type, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_RANGE_TITLE"));

  /***************************************************************/
  if(type == 0)
    GroupConstructors->setTitle(tr("GEOM_ANG_RANGE"));
  else if(type == 1)
    GroupConstructors->setTitle(tr("GEOM_LIN_RANGE"));
  RadioButton1->setText(tr("GEOM_RANGE_X"));
  RadioButton2->setText(tr("GEOM_RANGE_Y"));
  RadioButton3->setText(tr("GEOM_RANGE_Z"));

  Group1 = new DlgRef_1Sel2Spin(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  Group1->TextLabel1->setText(tr("GEOM_CONTACT"));
  Group1->TextLabel2->setText(tr("GEOM_MIN"));
  Group1->TextLabel3->setText(tr("GEOM_MAX"));
  Group1->PushButton1->setPixmap(image0);

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myType = type;
  myKinematicGUI = theKinematicGUI;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_RangeDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_RangeDlg::~KinematicGUI_RangeDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::Init()
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = Group1->LineEdit1;
  myOkShape = false;

  myMinValX = 0.0;
  myMaxValX = 0.0;
  myMinValY = 0.0;
  myMaxValY = 0.0;
  myMinValZ = 0.0;
  myMaxValZ = 0.0;

  double step = 5.0;
  Group1->SpinBox_DX->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_DY->RangeStepAndValidator(-99999.999, 99999.999, step, 3);

  Group1->SpinBox_DX->SetValue(myMinValX);
  Group1->SpinBox_DY->SetValue(myMaxValX);

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group1->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group1->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DY, SLOT(SetStep(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  this->SelectionIntoArgument();
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : SetEnabledValues()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::SetEnabledValues()
{
  bool IsOk = true;
  int aType = myGeomContact->GetType();

  GEOM::ListOfDouble_var aList;
  if(myType == 0) /* Angular */
    aList = myGeomContact->GetAngularRange();
  else if(myType == 1) /* Linear */
    aList = myGeomContact->GetLinearRange();

  myMinValX = aList[0];
  myMaxValX = aList[1];
  myMinValY = aList[2];
  myMaxValY = aList[3];
  myMinValZ = aList[4];
  myMaxValZ = aList[5];
  
  if(myConstructorId == 0) { // X
    Group1->SpinBox_DX->SetValue(aList[0]);
    Group1->SpinBox_DY->SetValue(aList[1]);
  }
  else if(myConstructorId == 1) { // Y
    Group1->SpinBox_DX->SetValue(aList[2]);
    Group1->SpinBox_DY->SetValue(aList[3]);
  }
  else if(myConstructorId == 2) { // Z
    Group1->SpinBox_DX->SetValue(aList[4]);
    Group1->SpinBox_DY->SetValue(aList[5]);
  }

  if(myType == 0) { //ANGULAR
    if(myConstructorId == 0 && (aType == 2 || aType == 5)) // X
      IsOk = false;
    else if(myConstructorId == 1 && (aType == 1 || aType == 2 || aType == 3 || aType == 5 || aType == 7 || aType == 9)) // Y
      IsOk = false;
    else if(myConstructorId == 2 && (aType == 1 || aType == 2 || aType == 3 || aType == 9)) // Z
      IsOk = false;
  }
  else if(myType == 1) { //LINEAR
    if(myConstructorId == 0 && (aType == 1 || aType == 4 || aType == 8)) // X
      IsOk = false;
    else if(myConstructorId == 1 && (aType == 1 || aType == 2 || aType == 3 || aType == 4 || aType == 6 || aType == 9)) // Y
      IsOk = false;
    else if(myConstructorId == 2 && (aType == 1 || aType == 2 || aType == 3 || aType == 4 || aType == 5 || aType == 6 || aType == 7 || aType == 9)) // Z
      IsOk = false;
  }

  if(aType == 0)
    IsOk = false;

  Group1->TextLabel2->setEnabled(IsOk);
  Group1->TextLabel3->setEnabled(IsOk);
  Group1->SpinBox_DX->setEnabled(IsOk);
  Group1->SpinBox_DY->setEnabled(IsOk);

  return;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void KinematicGUI_RangeDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  this->SelectionIntoArgument();
  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::ClickOnApply()
{
  buttonApply->setFocus();
  QAD_Application::getDesktop()->putInfo(tr(""));

  if(myType == 0 && myOkShape) /* Angular */
    myKinematicGUI->SetAngularRange(myGeomContact, myMinValX, myMaxValX,
				    myMinValY, myMaxValY, myMinValZ, myMaxValZ);
  else if(myType == 1 && myOkShape) /* Linear */
    myKinematicGUI->SetLinearRange(myGeomContact, myMinValX, myMaxValX,
				   myMinValY, myMaxValY, myMinValZ, myMaxValZ);

  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void KinematicGUI_RangeDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */

  myMinValX = 0.0;
  myMaxValX = 0.0;
  myMinValY = 0.0;
  myMaxValY = 0.0;
  myMinValZ = 0.0;
  myMaxValZ = 0.0;
  Group1->SpinBox_DX->SetValue(0.0);
  Group1->SpinBox_DY->SetValue(0.0);

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
      myOkShape = false;
      return;
  }

  /*  nbSel == 1  */
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();

  myGeomContact = myGeomBase->ConvertIOinContact(IO, testResult);
  if(!testResult)
    return;

  Group1->LineEdit1->setText(aString);
  myOkShape = true;
  this->SetEnabledValues();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();  

  if(send == Group1->PushButton1)
    Group1->LineEdit1->setFocus();

  this->SelectionIntoArgument();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::enterEvent(QEvent* e)
{
  if(GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_RangeDlg::ValueChangedInSpinBox(double newValue)
{
  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();

  if(myConstructorId == 0) {
    if(send == Group1->SpinBox_DX) {
      if(newValue <= myMaxValX)
	myMinValX = newValue;
      else
	Group1->SpinBox_DX->SetValue(myMinValX);
    }
    else if(send == Group1->SpinBox_DY) {
      if(newValue >= myMinValX)
	myMaxValX = newValue;
      else
	Group1->SpinBox_DY->SetValue(myMaxValX);
    }
  }
  else if(myConstructorId == 1) {
    if(send == Group1->SpinBox_DX) {
      if(newValue <= myMaxValY)
	myMinValY = newValue;
      else
	Group1->SpinBox_DX->SetValue(myMinValY);
    }
    else if(send == Group1->SpinBox_DY) {
      if(newValue >= myMinValY)
	myMaxValY = newValue;
      else
	Group1->SpinBox_DY->SetValue(myMaxValY);
    }
  }
  else if(myConstructorId == 2) {
    if(send == Group1->SpinBox_DX) {
      if(newValue <= myMaxValZ)
	myMinValZ = newValue;
      else
	Group1->SpinBox_DX->SetValue(myMinValZ);
    }
    else if(send == Group1->SpinBox_DY) {
      if(newValue >= myMinValZ)
	myMaxValZ = newValue;
      else
	Group1->SpinBox_DY->SetValue(myMaxValZ);
    }
  }

  return;
}
