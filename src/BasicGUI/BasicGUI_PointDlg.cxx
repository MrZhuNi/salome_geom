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
//  File   : BasicGUI_PointDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "BasicGUI_PointDlg.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <BRepAlgoAPI.hxx>
#include <Geom_Curve.hxx>

//=================================================================================
// class    : BasicGUI_PointDlg()
// purpose  : Constructs a BasicGUI_PointDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BasicGUI_PointDlg::BasicGUI_PointDlg(QWidget* parent, const char* name, BasicGUI* theBasicGUI, SALOME_Selection* Sel, const Handle (AIS_InteractiveContext)& ic, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_POINT_EDGE")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_POINT")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_POINT_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_POINTS"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_1Sel1Spin(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_PARAM_POINT"));
  GroupPoints->TextLabel1->setText(tr("GEOM_EDGE"));
  GroupPoints->TextLabel2->setText(tr("GEOM_PARAMETER"));
  GroupPoints->PushButton1->setPixmap(image2);

  GroupDimensions = new DlgRef_3Spin(this, "GroupDimensions");
  GroupDimensions->GroupBox1->setTitle(tr("GEOM_COORDINATES"));
  GroupDimensions->TextLabel1->setText(tr("GEOM_X"));
  GroupDimensions->TextLabel2->setText(tr("GEOM_Y"));
  GroupDimensions->TextLabel3->setText(tr("GEOM_Z"));

  Layout1->addWidget(GroupPoints, 1, 0);
  Layout1->addWidget(GroupDimensions, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myBasicGUI = theBasicGUI;
  Init(ic);
}


//=======================================================================
// function : ~BasicGUI_PointDlg()
// purpose  : Destructor
//=======================================================================
BasicGUI_PointDlg::~BasicGUI_PointDlg()
{
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::Init(const Handle(AIS_InteractiveContext)& ic)
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myIC = ic;
  myParameter = 0.50;
  myPoint.SetCoord(0.0, 0.0, 0.0);
  myOkEdge = false;

  myGeomGUI->SetState(POINT_METHOD);

  /*  Vertices Filter for all arguments */
  myEdgeFilter = new GEOM_ShapeTypeFilter(TopAbs_EDGE, myGeom);
  mySelection->AddFilter(myEdgeFilter);

  /* Get setting of step value from file configuration */
  QString St = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  step = St.toDouble();

  /* min, max, step and decimals for spin boxes */
  double specificStep = 0.1;
  GroupPoints->SpinBox_DX->RangeStepAndValidator(-999999.99999, 999999.99999, specificStep, 5);
  GroupDimensions->SpinBox_DX->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupDimensions->SpinBox_DY->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupDimensions->SpinBox_DZ->RangeStepAndValidator(-999.999, 999.999, step, 3);
  GroupPoints->SpinBox_DX->SetValue(myParameter);
  GroupDimensions->SpinBox_DX->SetValue(0.0);
  GroupDimensions->SpinBox_DY->SetValue(0.0);
  GroupDimensions->SpinBox_DZ->SetValue(0.0);

  if (myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    myLocalContextId = myIC->OpenLocalContext();
    myGeomGUI->SetDisplayedObjectList();
    /* sub shapes selection */
    myLocalContextMode = TopAbs_VERTEX;
    myIC->ActivateStandardMode(myLocalContextMode);
    myUseLocalContext = true;
  } else {
    myUseLocalContext = false;
  }

  TopoDS_Shape S;
  bool displayPoint = true;
  if(myGeomGUI->GetTopoFromSelection(mySelection, S)) {
    /* Filter a possibly previous selection and try to put it into coordinates */
    if(myGeomGUI->VertexToPoint(S, myPoint))
      displayPoint = false;
  }
    
  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupDimensions->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupDimensions->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(GroupDimensions->SpinBox_DZ, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

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
void BasicGUI_PointDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  mySelection->ClearFilters();
  myGeomGUI->EraseSimulationShape();
  disconnect(mySelection, 0, this, 0);

  switch (constructorId)
    {
    case 0:
      {
	if (myUseLocalContext == true && myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  myIC->CloseLocalContext(myLocalContextId);
	  myUseLocalContext = false;
	}

	GroupDimensions->hide();
	resize(0, 0);
	GroupPoints->show();

	myEditCurrentArgument = GroupPoints->LineEdit1;
	GroupPoints->LineEdit1->setText("");
	myOkEdge = false;

	/* filter for next selections */
	mySelection->AddFilter(myEdgeFilter);
	connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 1:
      {
	if(myUseLocalContext == false && myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  myLocalContextId = myIC->OpenLocalContext();
	  myGeomGUI->SetDisplayedObjectList();
	  /* sub shapes selection */
	  myLocalContextMode = TopAbs_VERTEX;
	  myIC->ActivateStandardMode(myLocalContextMode);
	  myUseLocalContext = true;
	}

	GroupPoints->hide();
	resize(0, 0);
	GroupDimensions->show();

	/* Display point simulation */
	PointIntoCoordinates(myPoint, true);
	break;
      }
    }
 return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr(""));
  if(mySimulationTopoDs.IsNull())
    return;
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  /* Close local context */
  if (myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    myIC->CloseLocalContext(myLocalContextId);
    myUseLocalContext = false;
  }

  switch(myConstructorId) 
    {
    case 0 :
      {
	if(myOkEdge == true) {
	  /* this constructor method has no idl interface : we use same than constructor 0 */
	  myBasicGUI->MakePointAndDisplay(myPoint.X(), myPoint.Y(), myPoint.Z());
	}
	break;
      }
    case 1 :
      {
	/* Recup args and call method */
	double x = GroupDimensions->SpinBox_DX->GetValue();
	double y = GroupDimensions->SpinBox_DY->GetValue();
	double z = GroupDimensions->SpinBox_DZ->GetValue();
      
	myBasicGUI->MakePointAndDisplay(x,y,z);  /* WARNING : no display if a local context is opened */
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  /* no display if a local context is opened */
	  myLocalContextId = myIC->OpenLocalContext();
	  myGeomGUI->SetDisplayedObjectList();
	  /* sub shapes selection */
	  myLocalContextMode = TopAbs_VERTEX;
	  myIC->ActivateStandardMode(myLocalContextMode);
	  myUseLocalContext = true;
	}
	break;
      }
    }
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed (for constructors not using local context)
//=================================================================================
void BasicGUI_PointDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape();
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */

  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    myOkEdge = false;
    return;
  }

  // nbSel == 1
  TopoDS_Shape S; 
  if(!myGeomGUI->GetTopoFromSelection(mySelection, S))
    return;

  if(S.ShapeType() == TopAbs_EDGE) {
    if(CalculateVertexOnCurve(TopoDS::Edge(S), myParameter, mySimulationTopoDs)) {
      if(myGeomGUI->VertexToPoint(mySimulationTopoDs, myPoint)) {
	GroupPoints->LineEdit1->setText(aString);
	myOkEdge = true;
	myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
      }
    }
  }
  return;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::LineEditReturnPressed()
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
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::SetEditCurrentArgument()
{
  if(myConstructorId != 0)
    return;

  QPushButton* send = (QPushButton*)sender();

  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    mySelection->AddFilter(myEdgeFilter);
    this->SelectionIntoArgument();
  }
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  : to reactivate this dialog box when mouse enter onto the window
//=================================================================================
void BasicGUI_PointDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::ActivateThisDialog( )
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

//   myGeomGUI->SetState(POINT_METHOD);
//   if( myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC ) {
//     OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
//     myIC = v3d->getAISContext();
//     myLocalContextId = myIC->OpenLocalContext();
//     myGeomGUI->SetDisplayedObjectList();
//     /* sub shapes selection */
//     myLocalContextMode = TopAbs_VERTEX ;
//     myIC->ActivateStandardMode(myLocalContextMode);
//     myUseLocalContext = true ;
//   }

  if(!mySimulationTopoDs.IsNull())
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void BasicGUI_PointDlg::ValueChangedInSpinBox(double newValue)
{
  myGeomGUI->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();
  double vx, vy, vz;
  if(send == GroupDimensions->SpinBox_DX) {
    vx = newValue;
    vy = GroupDimensions->SpinBox_DY->GetValue();
    vz = GroupDimensions->SpinBox_DZ->GetValue();
  } else if(send == GroupDimensions->SpinBox_DY) {
    vx = GroupDimensions->SpinBox_DX->GetValue();
    vy = newValue;
    vz = GroupDimensions->SpinBox_DZ->GetValue();
  } else if(send == GroupDimensions->SpinBox_DZ) {
    vx = GroupDimensions->SpinBox_DX->GetValue();
    vy = GroupDimensions->SpinBox_DY->GetValue();
    vz = newValue;
  }  else if(send == GroupPoints->SpinBox_DX) {
    myParameter = newValue;
  } else
    return;

  switch(myConstructorId) 
    {
    case 0: // default constructor 
      {
	this->SelectionIntoArgument();	
	break;
      }
    case 1: 
      {
	myPoint.SetCoord(vx, vy, vz);
	mySimulationTopoDs = BRepBuilderAPI_MakeVertex (myPoint).Shape();
	myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
	break;
      }
    }
  return;
}


//=================================================================================
// function : CalculateVertexOnCurve()
// purpose  : Calculate a Vertex on the curve given by 'anEdge'.
//          : The position of resultVertex is given by aParameter.
//          : For a linear edge, aParameter=0.0 gives the first vertex of edge
//          :                    aParameter=1.0 gives the last vertex of edge
//          :                    aParameter=0.5 gives the vertex on the middle of edge
//          : It is possible to get vertices out of edge using values > 1.0 or < 0.0
//=================================================================================
bool BasicGUI_PointDlg::CalculateVertexOnCurve(const TopoDS_Edge& anEdge, const Standard_Real aParameter, TopoDS_Shape& resultVertex) 
{
  if(anEdge.IsNull() || !BRepAlgoAPI::IsValid(anEdge))
    return false;

  Standard_Real first, last;
  Handle(Geom_Curve) curv = BRep_Tool::Curve(anEdge, first, last);
  if(!curv->IsCN(0))
    return false;
  
  Standard_Real param;
  if(anEdge.Orientation() == TopAbs_FORWARD)
    param = first + (last-first) * aParameter;
  else
    param = last + (first-last) * aParameter;

  gp_Pnt paramPoint;
  curv->D0(param, paramPoint);
  resultVertex = BRepBuilderAPI_MakeVertex(paramPoint);
  return true;
}


//=======================================================================
// funcion  : PointIntoCoordinates()
// purpose  : Sets user point coordinates into this dialog Spin boxes
//          : and displays it or not according to 'bool displayPoint'
//=======================================================================
void BasicGUI_PointDlg::PointIntoCoordinates(gp_Pnt P, bool displayPoint)
{
  GroupDimensions->SpinBox_DX->SetValue(P.X());
  GroupDimensions->SpinBox_DY->SetValue(P.Y());
  GroupDimensions->SpinBox_DZ->SetValue(P.Z());
  this->myPoint.SetCoord(P.X(), P.Y(), P.Z());
  if(displayPoint) {
    mySimulationTopoDs = BRepBuilderAPI_MakeVertex(P).Shape();
    myGeomGUI->DisplaySimulationShape(mySimulationTopoDs);
  }
  return;
}
