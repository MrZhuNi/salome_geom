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
//  File   : KinematicGUI_TranslationDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "KinematicGUI_TranslationDlg.h"

//=================================================================================
// class    : KinematicGUI_TranslationDlg()
// purpose  : Constructs a KinematicGUI_TranslationDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_TranslationDlg::KinematicGUI_TranslationDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_CONTACT")));

  setCaption(tr("GEOM_KTRANSLATION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_KTRANSLATION"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  Group1 = new DlgRef_3Spin(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  Group1->TextLabel1->setText(tr("GEOM_X"));
  Group1->TextLabel2->setText(tr("GEOM_Y"));
  Group1->TextLabel3->setText(tr("GEOM_Z"));

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_TranslationDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_TranslationDlg::~KinematicGUI_TranslationDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_TranslationDlg::Init()
{
  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  double step = St.toDouble();

  Group1->SpinBox_DX->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_DY->RangeStepAndValidator(-99999.999, 99999.999, step, 3);
  Group1->SpinBox_DZ->RangeStepAndValidator(-99999.999, 99999.999, step, 3);

  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  myGeomContact = myGeomBase->ConvertIOinContact(IO, testResult);
  if(!testResult)
    ClickOnCancel();

  myGeomTranslation = GEOM::GEOM_Translation::_narrow(myGeomContact->GetTranslation());

  myValX = myGeomTranslation->GetValX();
  myValY = myGeomTranslation->GetValY();
  myValZ = myGeomTranslation->GetValZ();

  Group1->SpinBox_DX->SetValue(myValX);
  Group1->SpinBox_DY->SetValue(myValY);
  Group1->SpinBox_DZ->SetValue(myValZ);

  SetEnabledValues();

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

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
void KinematicGUI_TranslationDlg::SetEnabledValues()
{
  int type = myGeomContact->GetType();

  if(type == 2 || type == 3 || type == 6 || type == 9) { //SLIDE || SLIDING PIVOT || ANNULAR || HELICOIDAL
    Group1->TextLabel2->setEnabled(false);
    Group1->SpinBox_DY->setEnabled(false);
    Group1->TextLabel3->setEnabled(false);
    Group1->SpinBox_DZ->setEnabled(false);
  }
  else if(type == 7) { //RECTILINEAR
    Group1->TextLabel2->setEnabled(false);
    Group1->SpinBox_DY->setEnabled(false);
  }
  else if(type == 5 || type == 8) { //PLANE || PONCTUAL
    Group1->TextLabel1->setEnabled(false);
    Group1->SpinBox_DX->setEnabled(false);
  }

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_TranslationDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return ;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_TranslationDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  myKinematicGUI->SetTranslation(myGeomContact, myValX, myValY, myValZ);

  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_TranslationDlg::ValueChangedInSpinBox(double newValue)
{
  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();

  if(send == Group1->SpinBox_DX)
    myValX = newValue;
  else if(send == Group1->SpinBox_DY)
    myValY = newValue;
  else if(send == Group1->SpinBox_DZ)
    myValZ = newValue;

  return;
}
