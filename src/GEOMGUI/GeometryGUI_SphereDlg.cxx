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
//  File   : GeometryGUI_SphereDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GeometryGUI_SphereDlg.h"

#include <BRepPrimAPI_MakeSphere.hxx>

#include "GeometryGUI.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"

//=================================================================================
// class    : GeometryGUI_SphereDlg()
// purpose  : Constructs a GeometryGUI_SphereDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GeometryGUI_SphereDlg::GeometryGUI_SphereDlg(QWidget* parent,  const char* name, PrimitiveGUI* thePrimitiveGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GeometryGUI_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_SPHERE_P")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_SPHERE_DXYZ")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_SPHERE_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_SPHERE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  GroupPoints = new GeometryGUI_1Sel1Spin(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_SPHERE_CR"));
  GroupPoints->TextLabel1->setText(tr("GEOM_CENTER"));
  GroupPoints->TextLabel2->setText(tr("GEOM_RADIUS"));
  GroupPoints->PushButton1->setPixmap(image2);

  GroupDimensions = new GeometryGUI_1Spin(this, "GroupDimensions");
  GroupDimensions->GroupBox1->setTitle(tr("GEOM_SPHERE_RO"));
  GroupDimensions->TextLabel1->setText(tr("GEOM_RADIUS"));

  Layout1->addWidget(GroupPoints, 1, 0);
  Layout1->addWidget(GroupDimensions, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myPrimitiveGUI = thePrimitiveGUI;
  Init();
}


//=================================================================================
// function : ~GeometryGUI_SphereDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GeometryGUI_SphereDlg::~GeometryGUI_SphereDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::Init()
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myPoint1.SetCoord(0.0, 0.0, 0.0);
  myRadius = 100.0;
  myOkPoint1 = myOkRadius = false;

  /*  Vertices Filter for all arguments */
  myVertexFilter = new GEOM_ShapeTypeFilter(TopAbs_VERTEX, myGeom);
  mySelection->AddFilter(myVertexFilter);

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();

  /* min, max, step and decimals for spin boxes */
  GroupPoints->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  GroupDimensions->SpinBox_DX->RangeStepAndValidator(0.001, 999.999, step, 3);
  GroupPoints->SpinBox_DX->SetValue(myRadius);
  GroupDimensions->SpinBox_DX->SetValue(myRadius);
  
  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupDimensions->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument())) ;

  /* displays Dialog */
  GroupDimensions->hide();
  GroupPoints->show();
  this->show();

  return;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void GeometryGUI_SphereDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  mySelection->ClearFilters();
  myGeomGUI->EraseSimulationShape();
  disconnect(mySelection, 0, this, 0);
  myRadius = 100.0;
  myOkRadius = true;

  switch (constructorId)
    {
    case 0:
      {
	GroupDimensions->hide();
	resize(0, 0);
	GroupPoints->show();

	myEditCurrentArgument = GroupPoints->LineEdit1;
	GroupPoints->LineEdit1->setText("");

	GroupPoints->SpinBox_DX->SetValue(myRadius);
	myOkPoint1 = false;

	/* filter for next selections */
	mySelection->AddFilter(myVertexFilter);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 1:
      {
	GroupPoints->hide();
	resize( 0, 0 );
	GroupDimensions->show();

	GroupDimensions->SpinBox_DX->SetValue(myRadius);
	myPoint1.SetCoord(0.0, 0.0, 0.0); /* at origin */
	myOkPoint1 = true;

	mySimulationTopoDs = BRepPrimAPI_MakeSphere(myPoint1, myRadius).Shape();
	myGeomGUI->DisplaySimulationShape(mySimulationTopoDs); 
	break;
      }
    }
 return ;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr(""));
  if (mySimulationTopoDs.IsNull())
    return;
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  if(myOkPoint1 && myOkRadius)
    myPrimitiveGUI->MakeSphereAndDisplay(myPoint1, myRadius);
  return ;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void GeometryGUI_SphereDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape();
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */

  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    if(myEditCurrentArgument == GroupPoints->LineEdit1) {
      GroupPoints->LineEdit1->setText("");
      myOkPoint1 = false;
    }
    return;
  }
  
  /* nbSel == 1 ! */
  TopoDS_Shape S; 
  if(!myGeomGUI->GetTopoFromSelection(mySelection, S))
    return;
    
  /* Constructor 1 treatment */
  if(myEditCurrentArgument == GroupPoints->LineEdit1 && myGeomGUI->VertexToPoint(S, myPoint1)) {
    GroupPoints->LineEdit1->setText(aString);
    myOkPoint1 = true;
  }
  
  if(myOkPoint1 && myOkRadius) {
    mySimulationTopoDs = BRepPrimAPI_MakeSphere(myPoint1, myRadius).Shape();
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs); 
  }
  return ;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if(send == GroupPoints->LineEdit1)
    myEditCurrentArgument = GroupPoints->LineEdit1;
  else
    return;

  GeometryGUI_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::SetEditCurrentArgument()
{
  if(myConstructorId != 0)
    return;

  QPushButton* send = (QPushButton*)sender();

  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    mySelection->AddFilter(myVertexFilter);
    this->SelectionIntoArgument();
  }

  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::ActivateThisDialog()
{
  GeometryGUI_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  if(!mySimulationTopoDs.IsNull())
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void GeometryGUI_SphereDlg::enterEvent(QEvent* e)
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
void GeometryGUI_SphereDlg::ValueChangedInSpinBox(double newValue)
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  myRadius = newValue;
  myOkRadius = true;
  
  if (myOkPoint1 && myOkRadius) {
    mySimulationTopoDs = BRepPrimAPI_MakeSphere(myPoint1, myRadius).Shape();
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs); 
  }
  return;
}
