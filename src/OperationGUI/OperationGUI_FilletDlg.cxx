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
#include "QAD_MessageBox.h"

#include <AIS_InteractiveContext.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <Precision.hxx>

#include <Standard_ErrorHandler.hxx> 
#include <Standard_Failure.hxx>

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

  Layout1->addWidget(Group1, 1, 0);
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

  myRadius = 5.0;
  myOkShape = false;
  myIC = ic;
  myLocalContextId = -1;
  myUseLocalContext = false;

  double SpecificStep = 10.0;
  /* min, max, step and decimals for spin boxes */
  Group1->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, SpecificStep, 3);
  Group1->SpinBox_DX->SetValue(myRadius);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group1->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group1->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), Group1->SpinBox_DX, SLOT(SetStep(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
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
  myRadius = 5.0;

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myUseLocalContext) {
    myIC->CloseLocalContext(myLocalContextId);
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true);
    myUseLocalContext = false;
  }

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() != VIEW_OCC) {
    myConstructorId = constructorId = 0; //No subshape selection if viewer is not OCC
    RadioButton1->setChecked(TRUE);
  }
  
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  Group1->LineEdit1->setText("");
  Group1->SpinBox_DX->SetValue(myRadius);
  switch (constructorId)
    {
    case 0: /* Fillet All */
      {
	Group1->GroupBox1->setTitle(tr("GEOM_FILLET_ALL"));
	myShapeType = -1;
	break;
      }
    case 1: /* Fillet edges */
      {
	Group1->GroupBox1->setTitle(tr("GEOM_FILLET_EDGES"));
	myShapeType = 6;
	break;
      }
    case 2: /* Fillet Faces */
      {
	Group1->GroupBox1->setTitle(tr("GEOM_FILLET_FACES"));
	myShapeType = 4;
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
  buttonApply->setFocus();
  QApplication::setOverrideCursor(Qt::waitCursor);
  QAD_Application::getDesktop()->putInfo(tr(""));
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  bool testResult = false;
  switch(myConstructorId)
    { 
    case 0 : /* Fillet All */
      {	
	if(myOkShape)
	  testResult = myOperationGUI->OnFilletGetAll(myShape, myRadius, myShapeType, myShapeIOR);
	break;
      }
    case 1 : /* Fillet Edge */
      {	
	if(myOkShape)
	  testResult = myOperationGUI->OnFilletGetSelected(myShape, myShapeIOR, myRadius, myShapeType, myLocalContextId, myUseLocalContext);
	break;
      }
    case 2 : /* Fillet Face */
      {
	if(myOkShape)
	  testResult = myOperationGUI->OnFilletGetSelected(myShape, myShapeIOR, myRadius, myShapeType, myLocalContextId, myUseLocalContext);       
	break;
      }
    }

  if(!testResult) 
    QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
  else
    QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  /* Reset all arguments and local context to allow user a new selection ...*/
  this->ResetStateOfDialog();
  QApplication::restoreOverrideCursor();
  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::ClickOnCancel()
{
  this->ResetStateOfDialog();
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
  mySimulationTopoDs.Nullify();
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
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_SHAPE_IN_STUDY"));
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
	SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
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
    if(myOkShape && QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myConstructorId == 0)
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
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();  

  if(send == Group1->PushButton1)
    Group1->LineEdit1->setFocus();

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
  GEOMBase_Skeleton::DeactivateActiveDialog();
  return;
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
  QApplication::restoreOverrideCursor();

  /* Close its local contact if opened */
  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myUseLocalContext) {
    myIC->CloseLocalContext(myLocalContextId);
    myUseLocalContext = false;
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true);
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

  if(myOkShape && QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC && myConstructorId == 0)
    this->MakePreview();
  return;
}


//=================================================================================
// function : MakePreview()
// purpose  :
//=================================================================================
void OperationGUI_FilletDlg::MakePreview()
{
  QApplication::setOverrideCursor(Qt::waitCursor);
  TopoDS_Shape tds;

  try {
    BRepFilletAPI_MakeFillet fill(myShape);
    switch(myConstructorId) 
      {
      case 0: /* Fillet All */
	{
	  TopExp_Explorer Exp(myShape, TopAbs_EDGE);
	  for(Exp; Exp.More(); Exp.Next()) {
	    TopoDS_Edge E = TopoDS::Edge(Exp.Current());
	    fill.Add(E);
	  }
	  
	  for(int i = 1; i <= fill.NbContours(); i++) {
	    try {
	      //fill.SetRadius(myRadius,i,i);
	      fill.SetRadius(myRadius,i);
	    }  
	    catch(Standard_Failure) {
	      QApplication::restoreOverrideCursor();
	      QAD_MessageBox::warn1 (QAD_Application::getDesktop(), tr("GEOM_WRN_WARNING"), tr("GEOM_FILLET_ABORT").arg(myRadius), tr("GEOM_BUT_OK"));
	      QApplication::setOverrideCursor(Qt::waitCursor);
	      myGeomBase->EraseSimulationShape(); 
	      mySimulationTopoDs.Nullify();
	      Group1->SpinBox_DX->SetValue(5.0);
	      QApplication::restoreOverrideCursor();
	      return;
	    }
	  }
	  tds = fill.Shape();
	  break;
	}
      //    case 1: /* Fillet edges */
      //    case 2: /* Fillet Faces */
      }

    if(!tds.IsNull()) {
      mySimulationTopoDs = tds;
      myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
    }
  }  
  catch(Standard_Failure) {
    QApplication::restoreOverrideCursor();
    QAD_MessageBox::warn1 (QAD_Application::getDesktop(), tr("GEOM_WRN_WARNING"), tr("GEOM_FILLET_ABORT").arg(myRadius), tr("GEOM_BUT_OK"));
    QApplication::setOverrideCursor(Qt::waitCursor);
    myGeomBase->EraseSimulationShape(); 
    mySimulationTopoDs.Nullify();
    Group1->SpinBox_DX->SetValue(5.0);
  }

  QApplication::restoreOverrideCursor();
  return;
}
