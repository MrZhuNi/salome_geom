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
//  File   : GenerationGUI_RevolDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GenerationGUI_RevolDlg.h"

#include <BRepAdaptor_Curve.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include "QAD_Config.h"

//=================================================================================
// class    : GenerationGUI_RevolDlg()
// purpose  : Constructs a GenerationGUI_RevolDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GenerationGUI_RevolDlg::GenerationGUI_RevolDlg(QWidget* parent, const char* name, GenerationGUI* theGenerationGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_REVOL")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_REVOLUTION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_REVOLUTION"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_2Sel1Spin1Check(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  GroupPoints->TextLabel1->setText(tr("GEOM_OBJECT"));
  GroupPoints->TextLabel2->setText(tr("GEOM_AXIS"));
  GroupPoints->TextLabel3->setText(tr("GEOM_ANGLE"));
  GroupPoints->CheckButton1->setText(tr("GEOM_REVERSE"));
  GroupPoints->PushButton1->setPixmap(image1);
  GroupPoints->PushButton2->setPixmap(image1);

  Layout1->addWidget(GroupPoints, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myGenerationGUI = theGenerationGUI;
  Init();
}


//=================================================================================
// function : ~GenerationGUI_RevolDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GenerationGUI_RevolDlg::~GenerationGUI_RevolDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myAngle = 45.0;
  myOkBase = myOkAxis = false;

  myEdgeFilter = new GEOM_ShapeTypeFilter(TopAbs_EDGE, myGeom);

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();

  /* min, max, step and decimals for spin boxes & initial values */
  GroupPoints->SpinBox_DX->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupPoints->SpinBox_DX->SetValue(myAngle);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPoints->CheckButton1, SIGNAL(stateChanged(int)), this, SLOT(ReverseVector(int)));
  
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
void GenerationGUI_RevolDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr(""));
  if (mySimulationTopoDs.IsNull())
    return;
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  if(myOkBase && myOkAxis)
    myGenerationGUI->MakeRevolutionAndDisplay(myGeomShape, myLoc, myDir, myAngle*PI180); 
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void GenerationGUI_RevolDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */
  
  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    if(myEditCurrentArgument == GroupPoints->LineEdit1)
      myOkBase = false;
    else if(myEditCurrentArgument == GroupPoints->LineEdit2)
      myOkAxis = false;
    return;
  }
  
  // nbSel == 1
  TopoDS_Shape S; 
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  if(!myGeomGUI->GetTopoFromSelection(mySelection, S))
    return;
    
  if(myEditCurrentArgument == GroupPoints->LineEdit1) {
    myGeomShape = myGeomGUI->ConvertIOinGEOMShape(IO, testResult);
    if(!testResult)
      return;
    TopAbs_ShapeEnum aType = S.ShapeType();
    if(aType != TopAbs_VERTEX && aType != TopAbs_EDGE && aType != TopAbs_WIRE && aType != TopAbs_FACE && aType != TopAbs_SHELL && aType != TopAbs_COMPOUND)
      return;
    myEditCurrentArgument->setText(aString);
    myOkBase = true;
    myBase = S;
  }
  else if(myEditCurrentArgument == GroupPoints->LineEdit2) {
    BRepAdaptor_Curve curv(TopoDS::Edge(S));
    myDir = curv.Line().Direction();
    myLoc = curv.Line().Location();
    myEditCurrentArgument->setText(aString);
    myOkAxis = true;
  }

  if(myOkBase && myOkAxis)
    this->MakeRevolutionSimulationAndDisplay();
  return; 
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  mySelection->ClearFilters();

  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
  }
  else if(send == GroupPoints->PushButton2) {
    GroupPoints->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit2;
    mySelection->AddFilter(myEdgeFilter);
  }
  this->SelectionIntoArgument();

  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();
  if(send == GroupPoints->LineEdit1)
    myEditCurrentArgument = GroupPoints->LineEdit1;
  else if (send == GroupPoints->LineEdit2)
    myEditCurrentArgument = GroupPoints->LineEdit2;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  if(!mySimulationTopoDs.IsNull())
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::ValueChangedInSpinBox(double newValue)
{  
  myAngle = newValue;
  if(myOkBase && myOkAxis)
    this->MakeRevolutionSimulationAndDisplay();
  return;
}


//=================================================================================
// function : ReverseAngle()
// purpose  : 'state' not used here
//=================================================================================
void GenerationGUI_RevolDlg::ReverseAngle(int state)
{
  myAngle = -myAngle;
  GroupPoints->SpinBox_DX->SetValue(myAngle);
  if(myOkBase && myOkAxis)
    this->MakeRevolutionSimulationAndDisplay();
  return;
}


//=================================================================================
// function : MakeRevolutionSimulationAndDisplay()
// purpose  :
//=================================================================================
void GenerationGUI_RevolDlg::MakeRevolutionSimulationAndDisplay() 
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  TopAbs_ShapeEnum aType = myBase.ShapeType();
  if(aType != TopAbs_VERTEX && aType != TopAbs_EDGE && aType != TopAbs_WIRE && aType != TopAbs_FACE && aType != TopAbs_SHELL && aType !=TopAbs_COMPOUND)
    return;

  try {
    gp_Ax1 AX(myLoc, myDir);
    mySimulationTopoDs = BRepPrimAPI_MakeRevol(myBase, AX, myAngle*PI180);
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  }
  catch(Standard_Failure) {
    MESSAGE("Exception catched in MakeRevolutionSimulationAndDisplay");
    return;
  }
  return;
}
