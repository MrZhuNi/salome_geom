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
//  File   : OperationGUI_FilletDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

using namespace std;
#include "OperationGUI_FilletDlg.h"

#include "DisplayGUI.h"
#include "QAD_Config.h"
#include "QAD_RightFrame.h"
#include "OCCViewer_Viewer3d.h"

#include <TopExp_Explorer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>

//=================================================================================
// class    : OperationGUI_FilletDlg()
// purpose  : Constructs a OperationGUI_FilletDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
OperationGUI_FilletDlg::OperationGUI_FilletDlg(QWidget* parent, const char* name, OperationGUI* theOperationGUI, SALOME_Selection* Sel, Handle(AIS_InteractiveContext) ic, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_FILLET_ALL")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_FILLET_EDGE")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_FILLET_FACE")));
  QPixmap image3(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_FILLET_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_FILLET"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->setPixmap(image2);

  Group1 = new DlgRef_1Sel1Spin(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_FILLET_ALL"));
  Group1->TextLabel1->setText(tr("GEOM_MAIN_OBJECT"));
  Group1->TextLabel2->setText(tr("GEOM_RADIUS"));
  Group1->PushButton1->setPixmap(image3);

  Group2 = new DlgRef_1Sel1Spin(this, "Group2");
  Group2->GroupBox1->setTitle(tr("GEOM_FILLET_EDGES"));
  Group2->TextLabel1->setText(tr("GEOM_MAIN_OBJECT"));
  Group2->TextLabel2->setText(tr("GEOM_RADIUS"));
  Group2->PushButton1->setPixmap(image3);

  Group3 = new DlgRef_1Sel1Spin(this, "Group3");
  Group3->GroupBox1->setTitle(tr("GEOM_FILLET_FACES"));
  Group3->TextLabel1->setText(tr("GEOM_MAIN_OBJECT"));
  Group3->TextLabel2->setText(tr("GEOM_RADIUS"));
  Group3->PushButton1->setPixmap(image3);

  Layout1->addWidget(Group1, 1, 0);
  Layout1->addWidget(Group2, 1, 0);
  Layout1->addWidget(Group3, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myOperationGUI = theOperationGUI;
  Init(ic);
}


//=================================================================================
// function : ~OperationGUI_FilletDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
OperationGUI_FilletDlg::~OperationGUI_FilletDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::Init(Handle(AIS_InteractiveContext) ic)
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = Group1->LineEdit1;

  myRadius = 50.0;
  myOkRadius = true;
  myOkShape = false;
  myIC = ic;
  myLocalContextId = -1;
  myUseLocalContext = false;

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();

  /* min, max, step and decimals for spin boxes */
  Group1->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  Group2->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  Group3->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);

  Group1->SpinBox_DX->SetValue(myRadius);
  Group2->SpinBox_DX->SetValue(myRadius);
  Group3->SpinBox_DX->SetValue(myRadius);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group2->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group3->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(Group1->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group2->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group3->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group1->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group2->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(Group3->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group2->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group3->SpinBox_DX, SLOT(SetStep(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  Group2->hide();
  Group3->hide();
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void OperationGUI_FilletDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  disconnect(mySelection, 0, this, 0);
  myOkShape = false;
  myRadius = 50.0;
  myOkRadius = true;

  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    myIC = v3d->getAISContext();
    if(myUseLocalContext) {
      myIC->CloseLocalContext(myLocalContextId);
      DisplayGUI* myDisplayGUI = new DisplayGUI();
      myDisplayGUI->OnDisplayAll(true);
      this->myUseLocalContext = false;
    }
  }

  switch (constructorId)
    {
    case 0: /* Fillet All */
      {
	Group2->hide();
	Group3->hide();
	resize(0, 0);
	Group1->show();

	myEditCurrentArgument = Group1->LineEdit1;
	Group1->LineEdit1->setText("");
	myShapeType = -1;

	Group1->SpinBox_DX->SetValue(myRadius);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 1: /* Fillet edges */
      {
	Group1->hide();
	Group3->hide();
	resize(0, 0);
	Group2->show();

	myEditCurrentArgument = Group2->LineEdit1;
	Group2->LineEdit1->setText("");
	myShapeType = 6;

	Group2->SpinBox_DX->SetValue(myRadius);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 2: /* Fillet Faces */
      {
	Group1->hide();
	Group2->hide();
	resize(0, 0);
	Group3->show();

	myEditCurrentArgument = Group3->LineEdit1;
	Group3->LineEdit1->setText("");
	myShapeType = 4;

	Group3->SpinBox_DX->SetValue(myRadius);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    }
  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ClickOnOk()
{
  this->ClickOnApply();
  this->ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr(""));
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  bool testResult = false;
  switch(myConstructorId)
    { 
    case 0 : /* Fillet All */
      {	
	if(myOkRadius && myOkShape)
	  testResult = myOperationGUI->OnFilletGetAll(myShape, myRadius, myShapeType, myShapeIOR);
	break;
      }
    case 1 : /* Fillet Edge */
      {	
	if(myOkRadius && myOkShape)
	  testResult = myOperationGUI->OnFilletGetSelected(myShape, myShapeIOR, myRadius, myShapeType, myLocalContextId, myUseLocalContext);
	break;
      }
    case 2 : /* Fillet Face */
      {
	if(myOkRadius && myOkShape)
	  testResult = myOperationGUI->OnFilletGetSelected(myShape, myShapeIOR, myRadius, myShapeType, myLocalContextId, myUseLocalContext);       
	break;
      }
    }

  if(!testResult) 
    myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
  else
    myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  /* Reset all arguments and local context to allow user a new selection ...*/
  this->ResetStateOfDialog();
  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ClickOnCancel()
{
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    myIC = v3d->getAISContext();

    if(myUseLocalContext) {
      myIC->CloseLocalContext(myLocalContextId);
      myUseLocalContext = false;
      DisplayGUI* myDisplayGUI = new DisplayGUI();
      myDisplayGUI->OnDisplayAll(true);
    }
  }
  GEOMBase_Skeleton::ClickOnCancel();
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void OperationGUI_FilletDlg::SelectionIntoArgument()
{
  myGeomBase->EraseSimulationShape();
  myEditCurrentArgument->setText("");
  this->ResetStateOfDialog();
  QString aString = ""; /* name of selection */

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if (nbSel == 1) {
    TopoDS_Shape S;
    Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
    if(!myGeomBase->GetTopoFromSelection(mySelection, S))
      return;
    if(!IO->hasEntry()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SHAPE_IN_STUDY"));
      return;
    }
	
    if(!S.IsNull() &&  S.ShapeType() <= 2) {
      if(IO->IsInstance(STANDARD_TYPE(GEOM_InteractiveObject))) {
	Handle(GEOM_InteractiveObject) GIObject = Handle(GEOM_InteractiveObject)::DownCast(IO);
	myShapeIOR = GIObject->getIOR(); /* the Geom IOR string of selection */
	myEditCurrentArgument->setText(aString);
	myShape = S;
	myOkShape = true;
      }
      
      if(IO->hasEntry()) {
	SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
	SALOMEDS::SObject_var obj = aStudy->FindObjectID(IO->getEntry());
        SALOMEDS::GenericAttribute_var anAttr;
        SALOMEDS::AttributeIOR_var anIOR;
	if(!obj->_is_nil()) {
	  if(obj->FindAttribute(anAttr, "AttributeIOR")) {
            anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	    myShapeIOR = anIOR->Value();
	    myOkShape = true;
	    myShape = S;
	    myEditCurrentArgument->setText(aString);
	  }
	}
      }
    }
    this->MakePreview();
	
  }
  else 
    return;
  
  if(myOkShape && myShapeType!=-1 && myConstructorId != 0) {
    /* local context is defined into the method */
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->PrepareSubShapeSelection(myShapeType, myLocalContextId);  
    myUseLocalContext = true;
  }
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if(send == Group1->LineEdit1)
    myEditCurrentArgument = Group1->LineEdit1;
  else if (send == Group2->LineEdit1)
    myEditCurrentArgument = Group2->LineEdit1;
  else if (send == Group3->LineEdit1)
    myEditCurrentArgument = Group3->LineEdit1;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();  

  if(send == Group1->PushButton1) {
    Group1->LineEdit1->setFocus();
    myEditCurrentArgument = Group1->LineEdit1;
  }
  else if(send == Group2->PushButton1) {
    Group2->LineEdit1->setFocus();
    myEditCurrentArgument = Group2->LineEdit1;
  }
  else if(send == Group3->PushButton1) {
    Group3->LineEdit1->setFocus();
    myEditCurrentArgument = Group3->LineEdit1;
  }
  this->SelectionIntoArgument();
  return;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::DeactivateActiveDialog()
{
    this->ResetStateOfDialog();
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true);
    GEOMBase_Skeleton::DeactivateActiveDialog();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  if(!mySimulationTopoDs.IsNull())
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::enterEvent(QEvent* e)
{
  if(GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::closeEvent(QCloseEvent* e)
{ 
  /* same than click on cancel button */
  this->ClickOnCancel();
  return;
}


//=================================================================================
// function : ResetStateOfDialog()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ResetStateOfDialog()
{
  myOkShape = false;
  myEditCurrentArgument->setText("");

  /* Close its local contact if opened */
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    myIC = v3d->getAISContext();

    if(myUseLocalContext) {
      myIC->CloseLocalContext(myLocalContextId);
      myUseLocalContext = false;
      DisplayGUI* myDisplayGUI = new DisplayGUI();
      myDisplayGUI->OnDisplayAll(true);
    }
  }
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ValueChangedInSpinBox(double newValue)
{
  myGeomBase->EraseSimulationShape(); 
  mySimulationTopoDs.Nullify();

  myRadius = newValue;
  myOkRadius = true;

  MakePreview();
  return;
}


//=================================================================================
// function : MakePreview()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::MakePreview()
{
  TopoDS_Shape tds;
  try {
    BRepFilletAPI_MakeFillet fill(myShape);
    switch (myConstructorId)
      {
      case 0: /* Fillet All */
	{
	  TopExp_Explorer Exp (myShape, TopAbs_EDGE);
	  for(Exp; Exp.More(); Exp.Next()) {
	    TopoDS_Edge E = TopoDS::Edge(Exp.Current());
	    fill.Add(E);
	  }
	  for(int i = 1;i<=fill.NbContours();i++) 
	    fill.SetRadius(myRadius,i);
	
	  tds = fill.Shape();
	  break;
	}
      }
    if(!tds.IsNull()) {
      mySimulationTopoDs = tds;
      myGeomBase->DisplaySimulationShape(mySimulationTopoDs); 
    }
  }  
  catch(Standard_Failure) {
    myGeomBase->EraseSimulationShape(); 
    mySimulationTopoDs.Nullify();
  }
  return;
}
