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
//  File   : KinematicGUI_PositionDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "KinematicGUI_PositionDlg.h"

//=================================================================================
// class    : KinematicGUI_PositionDlg()
// purpose  : Constructs a KinematicGUI_PositionDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_PositionDlg::KinematicGUI_PositionDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_CONTACT")));

  setCaption(tr("GEOM_KPOSITION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_KPOSITION"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  Group1 = new KinematicGUI_12Spin(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  Group1->TextLabel1->setText(tr("GEOM_POS"));
  Group1->TextLabel2->setText(tr("GEOM_X"));
  Group1->TextLabel3->setText(tr("GEOM_Y"));
  Group1->TextLabel4->setText(tr("GEOM_Z"));
  Group1->TextLabel5->setText(tr("GEOM_VX"));
  Group1->TextLabel6->setText(tr("GEOM_X"));
  Group1->TextLabel7->setText(tr("GEOM_Y"));
  Group1->TextLabel8->setText(tr("GEOM_Z"));
  Group1->TextLabel9->setText(tr("GEOM_VY"));
  Group1->TextLabel10->setText(tr("GEOM_X"));
  Group1->TextLabel11->setText(tr("GEOM_Y"));
  Group1->TextLabel12->setText(tr("GEOM_Z"));
  Group1->TextLabel13->setText(tr("GEOM_VZ"));
  Group1->TextLabel14->setText(tr("GEOM_X"));
  Group1->TextLabel15->setText(tr("GEOM_Y"));
  Group1->TextLabel16->setText(tr("GEOM_Z"));

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_PositionDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_PositionDlg::~KinematicGUI_PositionDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_PositionDlg::Init()
{
  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  double step = St.toDouble();
  double specificstep = 1.0;

  Group1->SpinBox_11->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_12->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_13->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_21->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_22->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_23->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_31->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_32->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_33->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_41->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_42->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);
  Group1->SpinBox_43->RangeStepAndValidator(-99999.999, 99999.999, specificstep, 3);

  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  myGeomContact = myGeomBase->ConvertIOinContact(IO, testResult);
  if(!testResult)
    ClickOnCancel();

  GEOM::ListOfDouble_var aList = myGeomContact->GetPosition();

  P0.x = aList[0];
  P0.y = aList[1];
  P0.z = aList[2];
  VX.PS.x = aList[3];
  VX.PS.y = aList[4];
  VX.PS.z = aList[5];
  VY.PS.x = aList[6];
  VY.PS.y = aList[7];
  VY.PS.z = aList[8];
  VZ.PS.x = aList[9];
  VZ.PS.y = aList[10];
  VZ.PS.z = aList[11];

  Group1->SpinBox_11->SetValue(P0.x);
  Group1->SpinBox_12->SetValue(P0.y);
  Group1->SpinBox_13->SetValue(P0.z);
  Group1->SpinBox_21->SetValue(VX.PS.x);
  Group1->SpinBox_22->SetValue(VX.PS.y);
  Group1->SpinBox_23->SetValue(VX.PS.z);
  Group1->SpinBox_31->SetValue(VY.PS.x);
  Group1->SpinBox_32->SetValue(VY.PS.y);
  Group1->SpinBox_33->SetValue(VY.PS.z);
  Group1->SpinBox_41->SetValue(VZ.PS.x);
  Group1->SpinBox_42->SetValue(VZ.PS.y);
  Group1->SpinBox_43->SetValue(VZ.PS.z);

  SetEnabledValues();

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(Group1->SpinBox_11, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_12, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_13, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_21, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_22, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_23, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_31, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_32, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_33, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_41, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_42, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group1->SpinBox_43, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_11, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_12, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_13, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_21, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_22, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_23, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_31, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_32, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_33, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_41, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_42, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_43, SLOT(SetStep(double)));

  /* displays Dialog */
  Group1->show();
  this->show();
  return;
}


//=================================================================================
// function : SetEnabledValues()
// purpose  :
//=================================================================================
void KinematicGUI_PositionDlg::SetEnabledValues()
{
  int type = myGeomContact->GetType();

  if(type == 0) { //EMBEDDING
    Group1->SpinBox_21->hide();
    Group1->SpinBox_22->hide();
    Group1->SpinBox_23->hide();
    Group1->TextLabel5->hide();
    Group1->TextLabel6->hide();
    Group1->TextLabel7->hide();
    Group1->TextLabel8->hide();
  }

  if(type == 0 || type == 1 || type == 3 || type == 5 || type == 9) { //EMBEDDING || PIVOT || SLIDING PIVOT || PLANE || HELICOIDAL
    Group1->SpinBox_31->hide();
    Group1->SpinBox_32->hide();
    Group1->SpinBox_33->hide();
    Group1->TextLabel9->hide();
    Group1->TextLabel10->hide();
    Group1->TextLabel11->hide();
    Group1->TextLabel12->hide();
  }

  if(type == 0 || type == 1 || type == 3 || type == 5 || type == 7 || type == 9) { //EMBEDDING  || PIVOT || SLIDING PIVOT || PLANE || RECTILINEAR || HELICOIDAL
    Group1->SpinBox_41->hide();
    Group1->SpinBox_42->hide();
    Group1->SpinBox_43->hide();
    Group1->TextLabel13->hide();
    Group1->TextLabel14->hide();
    Group1->TextLabel15->hide();
    Group1->TextLabel16->hide();
  }

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_PositionDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return ;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_PositionDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  myKinematicGUI->SetPosition(myGeomContact, P0, VX, VY, VZ);

  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_PositionDlg::ValueChangedInSpinBox(double newValue)
{
  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();

  if(send == Group1->SpinBox_11)
    P0.x = newValue;
  else if(send == Group1->SpinBox_12)
    P0.y = newValue;
  else if(send == Group1->SpinBox_13)
    P0.z = newValue;
  else if(send == Group1->SpinBox_21)
    VX.PS.x = newValue;
  else if(send == Group1->SpinBox_22)
    VX.PS.y = newValue;
  else if(send == Group1->SpinBox_23)
    VX.PS.z = newValue;
  else if(send == Group1->SpinBox_31)
    VY.PS.x = newValue;
  else if(send == Group1->SpinBox_32)
    VY.PS.y = newValue;
  else if(send == Group1->SpinBox_33)
    VY.PS.z = newValue;
  else if(send == Group1->SpinBox_41)
    VZ.PS.x = newValue;
  else if(send == Group1->SpinBox_42)
    VZ.PS.y = newValue;
  else if(send == Group1->SpinBox_43)
    VZ.PS.z = newValue;

  return;
}
