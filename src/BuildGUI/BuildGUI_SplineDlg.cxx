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
//  File   : BuildGUI_SplineDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "BuildGUI_SplineDlg.h"

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <Geom_BezierCurve.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <BRep_Tool.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>

//=================================================================================
// class    : BuildGUI_SplineDlg()
// purpose  : Constructs a BuildGUI_SplineDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BuildGUI_SplineDlg::BuildGUI_SplineDlg(QWidget* parent, const char* name, BuildGUI* theBuildGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_BEZIER")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_BSPLINE")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_SPLINE_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_SPLINE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  GroupBezier = new DlgRef_1Sel_QTD(this, "GroupBezier");
  GroupBezier->GroupBox1->setTitle(tr("GEOM_BEZIER"));
  GroupBezier->TextLabel1->setText(tr("GEOM_POINTS"));
  GroupBezier->PushButton1->setPixmap(image2);

  GroupBSpline = new DlgRef_1Sel_QTD(this, "GroupBSpline");
  GroupBSpline->GroupBox1->setTitle(tr("GEOM_BSPLINE"));
  GroupBSpline->TextLabel1->setText(tr("GEOM_POINTS"));
  GroupBSpline->PushButton1->setPixmap(image2);

  Layout1->addWidget(GroupBezier, 1, 0);
  Layout1->addWidget(GroupBSpline, 1, 0);
  /***************************************************************/

  /* Initialisations */
  myBuildGUI = theBuildGUI;
  Init();
}


//=================================================================================
// function : ~BuildGUI_SplineDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BuildGUI_SplineDlg::~BuildGUI_SplineDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::Init()
{
  /* init variables */
  myConstructorId = 0;
  myEditCurrentArgument = GroupBezier->LineEdit1;
  myOkListShapes = false;

  /*  Vertices Filter for all arguments */
  myVertexFilter = new GEOM_ShapeTypeFilter(TopAbs_VERTEX, myGeom);
  mySelection->AddFilter(myVertexFilter);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupBezier->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupBSpline->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  GroupBSpline->hide();
  GroupBezier->show();
  this->show();

  return;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void BuildGUI_SplineDlg::ConstructorsClicked(int constructorId)
{
  myConstructorId = constructorId;
  mySelection->ClearFilters();
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();
  disconnect(mySelection, 0, this, 0);
  myOkListShapes = false;

  switch (constructorId)
    {
    case 0:
      {
	GroupBSpline->hide();
	resize(0, 0);
	GroupBezier->show();

	myEditCurrentArgument = GroupBezier->LineEdit1;
	GroupBezier->LineEdit1->setText("");
	break;
      }
    case 1:
      {
	GroupBezier->hide();
	resize(0, 0);
	GroupBSpline->show();

	myEditCurrentArgument = GroupBSpline->LineEdit1;
	GroupBSpline->LineEdit1->setText("");
	break;
      }
    }
  /* filter for next selection */
  mySelection->AddFilter(myVertexFilter);
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));
  if (mySimulationTopoDs.IsNull())
    return;
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  switch(myConstructorId)
    {
    case 0 :
      {
	if(myOkListShapes)
	  myBuildGUI->MakeBezierAndDisplay(myListShapes);
	break;
      }
    case 1 :
      {
	if(myOkListShapes)
	  myBuildGUI->MakeBSplineAndDisplay(myListShapes);
	break;
      }
    }
  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void BuildGUI_SplineDlg::SelectionIntoArgument()
{
  myGeomBase->EraseSimulationShape();
  myEditCurrentArgument->setText("");
  QString aString = ""; /* name of selection */

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel < 2) {
    myOkListShapes = false;
    return;
  }
  
  myGeomBase->ConvertListOfIOInListOfIOR(mySelection->StoredIObjects(), myListShapes);
  myEditCurrentArgument->setText(aString);
  myOkListShapes = true;

  this->MakeSplineSimulationAndDisplay();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if(send == GroupBezier->PushButton1) {
    GroupBezier->LineEdit1->setFocus();
    myEditCurrentArgument = GroupBezier->LineEdit1;
  }
  else if(send == GroupBSpline->PushButton1) {
    GroupBSpline->LineEdit1->setFocus();
    myEditCurrentArgument = GroupBSpline->LineEdit1;
  }
  mySelection->AddFilter(myVertexFilter);
  this->SelectionIntoArgument();

  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  mySelection->AddFilter(myVertexFilter);
  if(!mySimulationTopoDs.IsNull())
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : MakeSplineSimulationAndDisplay()
// purpose  :
//=================================================================================
void BuildGUI_SplineDlg::MakeSplineSimulationAndDisplay() 
{
  myGeomBase->EraseSimulationShape();
  mySimulationTopoDs.Nullify();

  if(!myOkListShapes)
    return;

  try {
    TColgp_Array1OfPnt CurvePoints(1, myListShapes.length());
    SALOME_ListIteratorOfListIO It(mySelection->StoredIObjects());
    int i = 1;
    for(;It.More(); It.Next()) {
      Standard_Boolean found;
      Handle(GEOM_AISShape) AISShape = myGeomBase->ConvertIOinGEOMAISShape(It.Value(), found);      
      if(!found)
	return;
      TopoDS_Shape Shape = AISShape->Shape();
      if(Shape.IsNull())
	return;
      if(Shape.ShapeType() == TopAbs_VERTEX) {
	const gp_Pnt& P = BRep_Tool::Pnt(TopoDS::Vertex(Shape));
	CurvePoints.SetValue(i, P);
      }
      i++;
    }
    switch(myConstructorId)
      {
    case 0 :
      {
	Handle(Geom_BezierCurve) GBC = new Geom_BezierCurve(CurvePoints);
	mySimulationTopoDs = BRepBuilderAPI_MakeEdge(GBC);
	break;
      }
    case 1 :
      {
	GeomAPI_PointsToBSpline GBC(CurvePoints);
	mySimulationTopoDs = BRepBuilderAPI_MakeEdge(GBC);
	break;
      }
    }
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs);
  }
  catch(Standard_Failure) {
    MESSAGE("Exception catched in MakeSplineSimulationAndDisplay");
    return;
  }
  return;
}
