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
//  File   : KinematicGUI_RotationDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "KinematicGUI_RotationDlg.h"

//=================================================================================
// class    : KinematicGUI_RotationDlg()
// purpose  : Constructs a KinematicGUI_RotationDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_RotationDlg::KinematicGUI_RotationDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  //QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_COMMON")));
  //QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_KROTATION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_KROTATION"));
  //RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  Group1 = new KinematicGUI_3List3Spin(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  Group1->TextLabel1->setText(tr("GEOM_1ROT"));
  Group1->TextLabel2->setText(tr("GEOM_VALUE"));
  Group1->TextLabel3->setText(tr("GEOM_2ROT"));
  Group1->TextLabel4->setText(tr("GEOM_VALUE"));
  Group1->TextLabel5->setText(tr("GEOM_3ROT"));
  Group1->TextLabel6->setText(tr("GEOM_VALUE"));

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_RotationDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_RotationDlg::~KinematicGUI_RotationDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::Init()
{
  double step = 5.0;

  Group1->SpinBox_DX->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_DY->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_DZ->RangeStepAndValidator(-99999.999, 99999.999, step, 3);

  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  myGeomContact = myGeomBase->ConvertIOinContact(IO, testResult);
  if(!testResult)
    ClickOnCancel();

  myGeomRotation = GEOM::GEOM_Rotation::_narrow(myGeomContact->GetRotation());

  myVal1 = myGeomRotation->GetVal1();
  myVal2 = myGeomRotation->GetVal2();
  myVal3 = myGeomRotation->GetVal3();

  Group1->SpinBox_DX->SetValue(myVal1);
  Group1->SpinBox_DY->SetValue(myVal2);
  Group1->SpinBox_DZ->SetValue(myVal3);

  myRot1 = myGeomRotation->GetRot1();
  myRot2 = myGeomRotation->GetRot2();
  myRot3 = myGeomRotation->GetRot3();

  Group1->ComboBox1->insertItem("X");
  Group1->ComboBox1->insertItem("Y");
  Group1->ComboBox1->insertItem("Z");
  Group1->ComboBox2->insertItem("Y");
  Group1->ComboBox2->insertItem("Z");
  Group1->ComboBox3->insertItem("Z");

  Group1->ComboBox1->setCurrentItem(myRot1 - 1);

  InitValues();
  SetEnabledValues();

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(Group1->ComboBox1, SIGNAL(activated(int)), this, SLOT(ComboTextChanged()));
  connect(Group1->ComboBox2, SIGNAL(activated(int)), this, SLOT(ComboTextChanged()));
  connect(Group1->ComboBox3, SIGNAL(activated(int)), this, SLOT(ComboTextChanged()));

  connect(Group1->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_DZ, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DY, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DZ, SLOT(SetStep(double)));

  /* displays Dialog */
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : InitValues()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::InitValues()
{
  if(myRot1 == 1) {
    Group1->ComboBox2->changeItem("Y", 0);
    Group1->ComboBox2->changeItem("Z", 1);
    if(myRot2 == 2) {
      Group1->ComboBox3->changeItem("Z", 0);
      Group1->ComboBox2->setCurrentItem(0);
    }
    else if(myRot2 == 3) {
      Group1->ComboBox3->changeItem("Y", 0);
      Group1->ComboBox2->setCurrentItem(1);
    }
  }
  else if(myRot1 == 2) {
    Group1->ComboBox2->changeItem("X", 0);
    Group1->ComboBox2->changeItem("Z", 1);
    if(myRot2 == 1) {
      Group1->ComboBox3->changeItem("Z", 0);
      Group1->ComboBox2->setCurrentItem(0);
    }
    else if(myRot2 == 3) {
      Group1->ComboBox3->changeItem("X", 0);
      Group1->ComboBox2->setCurrentItem(1);
    }
  }
  else if(myRot1 == 3) {
    Group1->ComboBox2->changeItem("X", 0);
    Group1->ComboBox2->changeItem("Y", 1);
    if(myRot2 == 1) {
      Group1->ComboBox3->changeItem("Y", 0);
      Group1->ComboBox2->setCurrentItem(0);
    }
    else if(myRot2 == 2) {
      Group1->ComboBox3->changeItem("X", 0);
      Group1->ComboBox2->setCurrentItem(1);
    }
  }
 return;
}


//=================================================================================
// function : InitValues()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::SetEnabledValues()
{
  int type = myGeomContact->GetType();

  if(type == 1 || type == 3 || type == 5 || type == 9) { //PIVOT || SLIDING PIVOT || PLANE || HELICOIDAL
    Group1->TextLabel3->setEnabled(false);
    Group1->ComboBox2->setEnabled(false);
    Group1->TextLabel4->setEnabled(false);
    Group1->SpinBox_DY->setEnabled(false);
    Group1->TextLabel5->setEnabled(false);
    Group1->ComboBox3->setEnabled(false);
    Group1->TextLabel6->setEnabled(false);
    Group1->SpinBox_DZ->setEnabled(false);
  }
  else if(type == 7) { //RECTILINEAR
    Group1->TextLabel5->setEnabled(false);
    Group1->ComboBox3->setEnabled(false);
    Group1->TextLabel6->setEnabled(false);
    Group1->SpinBox_DZ->setEnabled(false);
  }

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  myKinematicGUI->SetRotation(myGeomContact, myRot1, myRot2, myRot3,
			      myVal1, myVal2, myVal3);

  return;
}


//=================================================================================
// function : ComboTextChanged()
// purpose  : 
//=================================================================================
void KinematicGUI_RotationDlg::ComboTextChanged()
{
  QComboBox* send = (QComboBox*)sender();

  if(send == Group1->ComboBox1) {
    myRot1 = Group1->ComboBox1->currentItem() + 1;
    if(myRot1 == 1) { //X
      myRot2 = 2; //Y
      myRot3 = 3; //Z
    } else if(myRot1 == 2) { //Y
      myRot2 = 1;//X
      myRot3 = 3;//Z
    } else if(myRot1 == 3) { //Z
      myRot2 = 1;//X
      myRot3 = 2;//Y
    }
  } else if(send == Group1->ComboBox2) {
    if(myRot1 == 1) {
      if(Group1->ComboBox2->currentItem() == 0) {
	myRot2 = 2;
	myRot3 = 3;
      } else if(Group1->ComboBox2->currentItem() == 1) {
	myRot2 = 3;
	myRot3 = 2;
      }
    } else if(myRot1 == 2) {
      if(Group1->ComboBox2->currentItem() == 0) {
	myRot2 = 1;
	myRot3 = 3;
      } else if(Group1->ComboBox2->currentItem() == 1) {
	myRot2 = 3;
	myRot3 = 1;
      }
    } else if(myRot1 == 3) {
      if(Group1->ComboBox2->currentItem() == 0) {
	myRot2 = 1;
	myRot3 = 2;
      } else if(Group1->ComboBox2->currentItem() == 1) {
	myRot2 = 2;
	myRot3 = 1;
      }
    }
  }

  InitValues();
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_RotationDlg::ValueChangedInSpinBox(double newValue)
{
  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();

  if(send == Group1->SpinBox_DX)
    myVal1 = newValue;
  else if(send == Group1->SpinBox_DY)
    myVal2 = newValue;
  else if(send == Group1->SpinBox_DZ)
    myVal3 = newValue;

  return;
}
