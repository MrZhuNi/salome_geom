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
//  File   : GenerationGUI_FillingDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GenerationGUI_FillingDlg.h"

#include "QAD_Config.h"

//=================================================================================
// class    : GenerationGUI_FillingDlg()
// purpose  : Constructs a GenerationGUI_FillingDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GenerationGUI_FillingDlg::GenerationGUI_FillingDlg(QWidget* parent, const char* name, GenerationGUI* theGenerationGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_FILLING")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_FILLING_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_FILLING"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_1Sel5Spin(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  GroupPoints->TextLabel1->setText(tr("GEOM_FILLING_COMPOUND"));
  GroupPoints->TextLabel2->setText(tr("GEOM_FILLING_MIN_DEG"));
  GroupPoints->TextLabel3->setText(tr("GEOM_FILLING_TOL_2D"));
  GroupPoints->TextLabel4->setText(tr("GEOM_FILLING_NB_ITER"));
  GroupPoints->TextLabel5->setText(tr("GEOM_FILLING_MAX_DEG"));
  GroupPoints->TextLabel6->setText(tr("GEOM_FILLING_TOL_3D"));
  GroupPoints->PushButton1->setPixmap(image1);

  Layout1->addWidget(GroupPoints, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myGenerationGUI = theGenerationGUI;
  Init();
}


//=================================================================================
// function : ~GenerationGUI_FillingDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GenerationGUI_FillingDlg::~GenerationGUI_FillingDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myMinDeg = 2;
  myMaxDeg = 5;
  myTol3D = 0.0001;
  myTol2D = 0.0001;
  myNbIter = 5;
  myOkSectionShape = false;

  myCompoundFilter = new GEOM_ShapeTypeFilter(TopAbs_COMPOUND, myGeom);
  mySelection->AddFilter(myCompoundFilter);

  double SpecificStep1 = 1;
  double SpecificStep2 = 0.00001;
  /* min, max, step and decimals for spin boxes & initial values */
  GroupPoints->SpinBox_1->RangeStepAndValidator(1.0, 999.999, SpecificStep1, 3);
  GroupPoints->SpinBox_2->RangeStepAndValidator(0.00001, 10000.0, SpecificStep2, 5);
  GroupPoints->SpinBox_3->RangeStepAndValidator(1.0, 999.999, SpecificStep1, 3);
  GroupPoints->SpinBox_4->RangeStepAndValidator(1.0, 999.999, SpecificStep1, 3);
  GroupPoints->SpinBox_5->RangeStepAndValidator(0.00001, 10000.0, SpecificStep2, 5);

  GroupPoints->SpinBox_1->SetValue(myMinDeg);
  GroupPoints->SpinBox_2->SetValue(myTol2D);
  GroupPoints->SpinBox_3->SetValue(myNbIter);
  GroupPoints->SpinBox_4->SetValue(myMaxDeg);
  GroupPoints->SpinBox_5->SetValue(myTol3D);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument())) ;

  /* displays Dialog */
  GroupPoints->show();
  this->show();

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  myMinDeg = GroupPoints->SpinBox_1->value();
  myTol2D = GroupPoints->SpinBox_2->value(); 
  myNbIter = GroupPoints->SpinBox_3->value();
  myMaxDeg = GroupPoints->SpinBox_4->value();
  myTol3D = GroupPoints->SpinBox_5->value(); 

  if(myOkSectionShape)	  
    myGenerationGUI->MakeFillingAndDisplay(myGeomShape, myMinDeg, myMaxDeg, myTol3D, myTol2D, myNbIter);
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void GenerationGUI_FillingDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */
  
  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    if(myEditCurrentArgument == GroupPoints->LineEdit1)
      myOkSectionShape = false;
    return;
  }
  
  // nbSel == 1
  TopoDS_Shape S; 
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  if(!myGeomBase->GetTopoFromSelection(mySelection, S))
    return;
  
  if(myEditCurrentArgument == GroupPoints->LineEdit1 && S.ShapeType() == TopAbs_COMPOUND) {
    myEditCurrentArgument->setText(aString);
    myGeomShape = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
    if(!testResult)
      return;
    myOkSectionShape = true;
  }
  /* no simulation */
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  mySelection->ClearFilters();

  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    mySelection->AddFilter(myCompoundFilter);
    this->SelectionIntoArgument();
  }

  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();
  if(send == GroupPoints->LineEdit1)
    myEditCurrentArgument = GroupPoints->LineEdit1;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  mySelection->AddFilter(myCompoundFilter);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void GenerationGUI_FillingDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}
