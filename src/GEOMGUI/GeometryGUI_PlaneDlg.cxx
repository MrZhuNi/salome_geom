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
//  File   : GeometryGUI_PlaneDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GeometryGUI_PlaneDlg.h"

#include <Precision.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>

#include "GeometryGUI.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"

//=================================================================================
// class    : GeometryGUI_PlaneDlg()
// purpose  : Constructs a GeometryGUI_PlaneDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GeometryGUI_PlaneDlg::GeometryGUI_PlaneDlg(QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GeometryGUI_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_PLANE_PV")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_PLANE_DXYZ")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_PLANE_FACE")));
  QPixmap image3(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_PLANE_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_PLANE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->setPixmap(image2);

  GroupPointDirection = new GeometryGUI_2Sel1Spin(this, "GroupPointDirection");
  GroupPointDirection->GroupBox1->setTitle(tr("GEOM_PLANE_PV"));
  GroupPointDirection->TextLabel1->setText(tr("GEOM_POINT"));
  GroupPointDirection->TextLabel2->setText(tr("GEOM_VECTOR"));
  GroupPointDirection->TextLabel3->setText(tr("GEOM_PLANE_SIZE"));
  GroupPointDirection->PushButton1->setPixmap(image3);
  GroupPointDirection->PushButton2->setPixmap(image3);

  GroupPointPlusCoordinates = new GeometryGUI_1Sel4Spin(this, "GroupPointPlusCoordinates" );
  GroupPointPlusCoordinates->GroupBox1->setTitle(tr("GEOM_PLANE_PVC"));
  GroupPointPlusCoordinates->TextLabel1->setText(tr("GEOM_POINT"));
  GroupPointPlusCoordinates->TextLabel2->setText(tr("GEOM_COOR"));
  GroupPointPlusCoordinates->TextLabel3->setText(tr("GEOM_DX"));
  GroupPointPlusCoordinates->TextLabel4->setText(tr("GEOM_DY"));
  GroupPointPlusCoordinates->TextLabel5->setText(tr("GEOM_DZ"));
  GroupPointPlusCoordinates->TextLabel6->setText(tr("GEOM_PLANE_SIZE"));
  GroupPointPlusCoordinates->PushButton1->setPixmap(image3);

  GroupFace = new GeometryGUI_1Sel1Spin(this, "GroupFace");
  GroupFace->GroupBox1->setTitle(tr("GEOM_FACE"));
  GroupFace->TextLabel1->setText(tr("GEOM_SELECTION"));
  GroupFace->TextLabel2->setText(tr("GEOM_PLANE_SIZE"));
  GroupFace->PushButton1->setPixmap(image3);
    
  Layout1->addWidget(GroupPointDirection, 1, 0);
  Layout1->addWidget(GroupPointPlusCoordinates, 1, 0);
  Layout1->addWidget(GroupFace, 1, 0);
  /***************************************************************/

  /* Initialisation */
  Init(Sel);
}


//=================================================================================
// function : ~GeometryGUI_PlaneDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GeometryGUI_PlaneDlg::~GeometryGUI_PlaneDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::Init( SALOME_Selection* Sel )
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = GroupPointDirection->LineEdit1;

  myPoint1.SetCoord(0.0, 0.0, 0.0);
  myOkPoint1 = myOkDirection = myOkCoordinates = myOkPlanarFace = false;

  /* Filters definition */
  myVertexFilter = new GEOM_ShapeTypeFilter(TopAbs_VERTEX, myGeom);
  myEdgeFilter = new GEOM_ShapeTypeFilter(TopAbs_EDGE, myGeom);
  myFaceFilter = new GEOM_FaceFilter(StdSelect_Plane, myGeom);
  /* Filter for the next selection */
  mySelection->AddFilter(myVertexFilter);

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();
  this->myTrimSize = 2000.0;
  
  /* min, max, step and decimals for spin boxes */
  GroupPointDirection->SpinBox_DX->RangeStepAndValidator(+0.001, 10000000.0, step, 5);
  GroupPointDirection->SpinBox_DX->SetValue(myTrimSize);

  GroupPointPlusCoordinates->SpinBox_DX->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupPointPlusCoordinates->SpinBox_DY->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupPointPlusCoordinates->SpinBox_DZ->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupPointPlusCoordinates->SpinBox_DX->SetValue(1.0);
  GroupPointPlusCoordinates->SpinBox_DY->SetValue(1.0);
  GroupPointPlusCoordinates->SpinBox_DZ->SetValue(1.0); 
  GroupPointPlusCoordinates->SpinBox_S->RangeStepAndValidator(+0.001, 10000000.0, step, 5);
  GroupPointPlusCoordinates->SpinBox_S->SetValue(myTrimSize) ;

  GroupFace->SpinBox_DX->RangeStepAndValidator(+0.001, 10000000.0, step, 5);
  GroupFace->SpinBox_DX->SetValue(myTrimSize);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), SLOT(ConstructorsClicked(int)));

  connect(GroupPointDirection->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPointDirection->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPointPlusCoordinates->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupFace->PushButton1, SIGNAL(clicked()), this, SLOT( SetEditCurrentArgument()));

  connect(GroupPointDirection->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPointDirection->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPointPlusCoordinates->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupFace->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPointPlusCoordinates->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPointPlusCoordinates->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPointPlusCoordinates->SpinBox_DZ, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPointPlusCoordinates->SpinBox_S, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupPointDirection->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupFace->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* Displays Dialog */
  GroupPointPlusCoordinates->hide();
  GroupFace->hide();
  GroupPointDirection->show();
  this->show();

  return ;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void GeometryGUI_PlaneDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  mySelection->ClearFilters();
  myGeomGUI->EraseSimulationShape();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  myOkPoint1 = myOkDirection = myOkCoordinates = myOkPlanarFace = false;

  switch (constructorId)
    {
    case 0: /* plane from a point and a direction (vector, edge...) */
      {	
	GroupPointPlusCoordinates->hide();
	GroupFace->hide();
	resize(0, 0);
	GroupPointDirection->show();

	myEditCurrentArgument = GroupPointDirection->LineEdit1;
	GroupPointDirection->LineEdit1->setText(tr(""));
	GroupPointDirection->LineEdit2->setText(tr(""));

	/* for the first argument */
	mySelection->AddFilter(myVertexFilter);
	break;
      }
    case 1: /* plane from a point and vector coordinates */
      {
	GroupPointDirection->hide();
	GroupFace->hide();
	resize(0, 0);
	GroupPointPlusCoordinates->show();

	myEditCurrentArgument = GroupPointPlusCoordinates->LineEdit1;
	GroupPointPlusCoordinates->LineEdit1->setText(tr(""));
	GroupPointPlusCoordinates->SpinBox_DX->SetValue(1.0);
	GroupPointPlusCoordinates->SpinBox_DY->SetValue(1.0);
	GroupPointPlusCoordinates->SpinBox_DZ->SetValue(1.0);
	myOkCoordinates = true;

	/* for the first argument */
	mySelection->AddFilter(myVertexFilter);
	break;
      } 
    case 2: /* plane from a planar face selection */
      {
	GroupPointDirection->hide();
	GroupPointPlusCoordinates->hide();
	resize(0, 0);
	GroupFace->show();

	myEditCurrentArgument = GroupFace->LineEdit1;
	GroupFace->LineEdit1->setText(tr(""));

	/* for the first argument */
	mySelection->AddFilter(myFaceFilter);
	break;
      }
    }
  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return ;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr(""));
  if (mySimulationTopoDs.IsNull())
    return;
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  switch(myConstructorId)
    { 
    case 0 : /* args are myPoint1 and myDx, myDy, myDz from a vector(edge) */
      {	
	if(myOkPoint1 && myOkDirection)
	  myGeomGUI->MakePlaneAndDisplay(myPoint1, myDx, myDy, myDz, myTrimSize);
	break;
      }
    case 1 : /* args are myPoint1 and myDx, myDy, myDz from a Spin Box */
      {	
	if(myOkPoint1)
	  myGeomGUI->MakePlaneAndDisplay(myPoint1, myDx, myDy, myDz, myTrimSize);
	break;
      }
    case 2 :  /* arg is a planar face selection */
      {
	if(myOkPlanarFace)
	  myGeomGUI->MakePlaneAndDisplay(myPoint1, myDx, myDy, myDz, myTrimSize);
	break;
      }
    }
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void GeometryGUI_PlaneDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape(); 
  mySimulationTopoDs.Nullify();
  QString aString = "";
  
  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    switch (myConstructorId) 
      {
       case 0:
	 {
	   if(myEditCurrentArgument == GroupPointDirection->LineEdit1) {
	     GroupPointDirection->LineEdit1->setText("");
	     myOkPoint1 = false ;
	   }
	   else if(myEditCurrentArgument == GroupPointDirection->LineEdit2) {
	     GroupPointDirection->LineEdit2->setText("");
	     myOkDirection = false ;
	   }
	   break;
	 }
      case 1:
	{
	  if(myEditCurrentArgument == GroupPointPlusCoordinates->LineEdit1) {
	    GroupPointPlusCoordinates->LineEdit1->setText("") ;
	    myOkPoint1 = false ;
	  }
	  break;
	}
      case 2:
	{
	  if(myEditCurrentArgument == GroupFace->LineEdit1) {
	    GroupFace->LineEdit1->setText("") ;
	    if(aString.compare("") == 0)
	      myOkPlanarFace = false;
	    else
	      myOkPlanarFace = true;
	  }
	  break;
	}
      }
    return ;
  }

  /*  nbSel == 1  */ 
  TopoDS_Shape S; 
  if(!myGeomGUI->GetTopoFromSelection(mySelection, S))
    return ;
 
  /* FIRST CONSTRUCTOR */
  if(myEditCurrentArgument == GroupPointDirection->LineEdit1 && myGeomGUI->VertexToPoint(S, myPoint1)) {
    GroupPointDirection->LineEdit1->setText(aString);
    myOkPoint1 = true;
  }
  else if( myEditCurrentArgument == GroupPointDirection->LineEdit2) {
    /* We verify if the selection is a linear edge */
    gp_Pnt Pfirst, Plast;
    if( myGeomGUI->LinearEdgeExtremities(S, Pfirst, Plast)) {    
      myGeomGUI->GetBipointDxDyDz(Pfirst, Plast, myDx, myDy, myDz);
      GroupPointDirection->LineEdit2->setText(aString);
      myOkDirection = true;
      this->myTrimSize = GroupPointDirection->SpinBox_DX->GetValue();
    }
  }
  
  /* SECOND CONSTRUCTOR */
  else if(myEditCurrentArgument == GroupPointPlusCoordinates->LineEdit1 && myGeomGUI->VertexToPoint(S, myPoint1)) {
    GroupPointPlusCoordinates->LineEdit1->setText(aString);
    /* Get arguments */
    myDx = GroupPointPlusCoordinates->SpinBox_DX->GetValue();
    myDy = GroupPointPlusCoordinates->SpinBox_DY->GetValue();
    myDz = GroupPointPlusCoordinates->SpinBox_DZ->GetValue();
    this->myTrimSize = GroupPointPlusCoordinates->SpinBox_S->GetValue();
    myOkPoint1 = true;    
    myOkCoordinates = true;
  }
 
  /* THIRD CONSTRUCTOR */
  else if(myEditCurrentArgument == GroupFace->LineEdit1) {
    if(myOkPlanarFace) {
      GroupFace->LineEdit1->setText(aString);
      BRepAdaptor_Surface surf(TopoDS::Face(S));
      gp_Pln Plane = surf.Plane();

      gp_Pnt myPoint1 = Plane.Location();
      gp_Ax1 ax = Plane.Axis();
      myDx = (ax.Direction()).X();
      myDy = (ax.Direction()).Y();
      myDz = (ax.Direction()).Z();
      this->myTrimSize = GroupFace->SpinBox_DX->GetValue();
    }
  }

  /* Call method simulation */    
  if((myOkPoint1 && myOkDirection) || (myOkPoint1 && myOkCoordinates) || myOkPlanarFace) {
    if(myDx*myDx + myDy*myDy + myDz*myDz > Precision::Confusion()*Precision::Confusion())
      MakePlaneSimulationAndDisplay(myPoint1, myDx, myDy, myDz, myTrimSize) ;
  }
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();  
  mySelection->ClearFilters() ;

  switch (myConstructorId)
    {
    case 0:
      {	
	if(send == GroupPointDirection->PushButton1) {
	  GroupPointDirection->LineEdit1->setFocus();
	  myEditCurrentArgument = GroupPointDirection->LineEdit1;
	  mySelection->AddFilter(myVertexFilter);
	}
	else if(send == GroupPointDirection->PushButton2) {
	  GroupPointDirection->LineEdit2->setFocus();
	  myEditCurrentArgument = GroupPointDirection->LineEdit2;	
	  /* Edge filter here */
	  mySelection->AddFilter(myEdgeFilter);
	  SelectionIntoArgument();
	}	
	break;
      }
    case 1:
      {	
	if(send == GroupPointPlusCoordinates->PushButton1) {
	  GroupPointPlusCoordinates->LineEdit1->setFocus();
	  myEditCurrentArgument = GroupPointPlusCoordinates->LineEdit1;
	  /* Vertex filter here */
	  mySelection->AddFilter(myVertexFilter);
	  SelectionIntoArgument();
	}
	break;
      }
    case 2:
      {
	if(send == GroupFace->PushButton1) {
	  GroupFace->LineEdit1->setFocus();
	  myEditCurrentArgument = GroupFace->LineEdit1;
	  /* Face filter here */
	  mySelection->AddFilter(myFaceFilter);
	  SelectionIntoArgument();
	}
	break;
      }
    }
  return ;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();  
  if(send == GroupPointDirection->LineEdit1)
    myEditCurrentArgument = GroupPointDirection->LineEdit1;
  else if (send == GroupPointDirection->LineEdit2)
    myEditCurrentArgument = GroupPointDirection->LineEdit2; 
  else if (send == GroupPointPlusCoordinates->LineEdit1)
    myEditCurrentArgument = GroupPointPlusCoordinates->LineEdit1;
  else if (send == GroupFace->LineEdit1)
    myEditCurrentArgument = GroupFace->LineEdit1;
  else
    return;

  GeometryGUI_Skeleton::LineEditReturnPressed();
  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::ActivateThisDialog()
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
void GeometryGUI_PlaneDlg::enterEvent(QEvent* e)
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
void GeometryGUI_PlaneDlg::ValueChangedInSpinBox( double newValue )
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  QObject* send = (QObject*)sender() ; 
  
  if(send == GroupPointPlusCoordinates->SpinBox_DX) {
    myDx = newValue;
    myDy = GroupPointPlusCoordinates->SpinBox_DY->GetValue();
    myDz = GroupPointPlusCoordinates->SpinBox_DZ->GetValue();
  } else if(send == GroupPointPlusCoordinates->SpinBox_DY) {
    myDx = GroupPointPlusCoordinates->SpinBox_DX->GetValue();
    myDy = newValue;
    myDz = GroupPointPlusCoordinates->SpinBox_DZ->GetValue();
  } else if(send == GroupPointPlusCoordinates->SpinBox_DZ) {
    myDx = GroupPointPlusCoordinates->SpinBox_DX->GetValue();
    myDy = GroupPointPlusCoordinates->SpinBox_DY->GetValue();
    myDz = newValue;
  } else if(send == GroupPointDirection->SpinBox_DX || send == GroupPointPlusCoordinates->SpinBox_S || send == GroupFace->SpinBox_DX) {
    myTrimSize = newValue;
  } else
    return;

  if((myOkPoint1 && myOkDirection) || (myOkPoint1 && myOkCoordinates) || myOkPlanarFace) {
    if (myDx*myDx + myDy*myDy + myDz*myDz > Precision::Confusion() * Precision::Confusion())
      MakePlaneSimulationAndDisplay( myPoint1, myDx, myDy, myDz, myTrimSize);
  }

  return ;
}


//=================================================================================
// function : MakePlaneSimulationAndDisplay(()
// purpose  :
//=================================================================================
void GeometryGUI_PlaneDlg::MakePlaneSimulationAndDisplay(const gp_Pnt& P1,
							 const Standard_Real dx,
							 const Standard_Real dy, 
							 const Standard_Real dz, 
							 const Standard_Real trimsize)
{
  try {
    gp_Dir aDirection(dx, dy, dz);
    /* We make a trimmed plane */
    gp_Pln gplane(P1, aDirection);
    mySimulationTopoDs = BRepBuilderAPI_MakeFace(gplane, -trimsize, +trimsize, -trimsize, +trimsize);
  }
  catch(Standard_Failure) {
    MESSAGE( "Exception catched in MakePlaneSimulation" << endl );
    return;
  }

  myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  return ;
}
