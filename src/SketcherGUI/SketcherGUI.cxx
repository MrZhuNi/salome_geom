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
//  File   : SketcherGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "SketcherGUI.h"

#include "QAD_RightFrame.h"
#include "OCCViewer_Viewer3d.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include <BRepTools_WireExplorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <BRep_Tool.hxx>
#include <TopExp.hxx>
#include <gp_Circ.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>

#include "SketcherGUI_Dlg.h"      // Sketcher

//=======================================================================
// function : SketcherGUI()
// purpose  : Constructor
//=======================================================================
SketcherGUI::SketcherGUI() :
  QObject()
{
  myGeomBase = new GEOMBase();
  myGeomGUI = GEOMContext::GetGeomGUI();
  myGeom = myGeomGUI->myComponentGeom;

  mySimulationShape1 = new AIS_Shape(TopoDS_Shape());
  mySimulationShape2 = new AIS_Shape(TopoDS_Shape());
}


//=======================================================================
// function : ~SketcherGUI()
// purpose  : Destructor
//=======================================================================
SketcherGUI::~SketcherGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool SketcherGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  SketcherGUI* mySketcherGUI = new SketcherGUI();
  mySketcherGUI->myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(QAD_Application::getDesktop()->getActiveStudy()->getSelection());

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  QMenuBar* Mb = QAD_Application::getDesktop()->getMainMenuBar();

  switch (theCommandID)
    {
    case 404: // SKETCHER
      {
	((OCCViewer_ViewFrame*)QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->onViewTop(); // DCQ : 28/02/2002
	SketcherGUI_Dlg* aDlg = new SketcherGUI_Dlg(parent, "", mySketcherGUI, Sel);
	break;
      }
    default:
      {
	parent->putInfo(tr("GEOM_PRP_COMMAND").arg(theCommandID));
	break;
      }
    }
  return true;
}


//=======================================================================
// function : OnSketchEnd()
// purpose  : 
//=======================================================================
void SketcherGUI::OnSketchEnd(TopoDS_Shape myShape)
{
  TopoDS_Wire W = TopoDS::Wire(myShape);

  if(!W.IsNull()) {
    GEOM::GEOM_Gen::ListOfIOR_var listShapes = new GEOM::GEOM_Gen::ListOfIOR;
    listShapes->length(0);
    unsigned int i = 0;

    BRepTools_WireExplorer Ex(W);
    while(Ex.More()) {
      TopoDS_Edge E = TopoDS::Edge(Ex.Current());
      
      gp_Pnt pt1, pt2;
      pt1 = BRep_Tool::Pnt(TopExp::FirstVertex(E));
      pt2 = BRep_Tool::Pnt(TopExp::LastVertex(E));

      gp_Pnt CenterPoint;
      Handle(Geom_Curve) Curve;
      Handle(Geom_Circle) Circle;
      gp_Circ Circ;
      Standard_Real First,Last;
      
      Curve = BRep_Tool::Curve(E,First,Last);
      if(Curve->IsKind(STANDARD_TYPE(Geom_Circle))) {
	Circle = Handle(Geom_Circle)::DownCast(Curve); // pointer on geom_circ
	Circ = Circle->Circ();                         // gp_Circ
	
	Curve->D0((First + Last) / 2., CenterPoint);

	GEOM::PointStruct pI = myGeom->MakePointStruct(pt1.X(), pt1.Y(), pt1.Z());
	GEOM::PointStruct pC = myGeom->MakePointStruct(CenterPoint.X(), CenterPoint.Y(), CenterPoint.Z());
	GEOM::PointStruct pE = myGeom->MakePointStruct(pt2.X(), pt2.Y(), pt2.Z());

	GEOM::GEOM_Shape_var arc;

	try {
	  arc = myGeom->MakeArc(pI, pC, pE);
	}
	catch (const SALOME::SALOME_Exception& S_ex) {
	  QtCatchCorbaException(S_ex);
	}
		
 	listShapes->length(i+1);
	listShapes[i] = strdup(arc->Name());
	i++;   
      } else {
	GEOM::PointStruct pI = myGeom->MakePointStruct(pt1.X(), pt1.Y(), pt1.Z());
	GEOM::PointStruct pE = myGeom->MakePointStruct(pt2.X(), pt2.Y(), pt2.Z());
	GEOM::GEOM_Shape_var segment;
	
	try {
	  segment = myGeom->MakeEdge(pI,pE);
	}
	catch (const SALOME::SALOME_Exception& S_ex) {
	  QtCatchCorbaException(S_ex);
	}
	
	listShapes->length(i+1);
	listShapes[i] = strdup(segment->Name());
	i++;
      }
      Ex.Next();
    }

    GEOM::GEOM_Shape_var Wire = myGeom->MakeWire(listShapes);
    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, Wire);
    Standard_CString type;
    myGeomBase->GetShapeTypeString(S,type);
    Wire->NameType(type);

    if(myGeomBase->Display(Wire))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  return;
}


//=====================================================================================
// function : DisplaySimulationShape() 
// purpose  : Displays 'this->mySimulationShape' a pure graphical shape from a TopoDS_Shape
//=====================================================================================
void SketcherGUI::DisplaySimulationShape(const TopoDS_Shape& S1, const TopoDS_Shape& S2) 
{
  //NRI DEBUG : 14/02/2002
  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return;
	
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
  try {
    if(!S1.IsNull()) {
      /* erase any previous */
      ic->Erase(mySimulationShape1, Standard_True, Standard_False);
      ic->ClearPrs(mySimulationShape1);
      
      mySimulationShape1 = new AIS_Shape(TopoDS_Shape());
      mySimulationShape1->Set(S1);
      mySimulationShape1->SetColor(Quantity_NOC_RED);
    
      ic->Deactivate(mySimulationShape1);
      ic->Display(mySimulationShape1, Standard_False);
      mySimulationShape1->UnsetColor();
    }
    if(!S2.IsNull()) {
      ic->Erase(mySimulationShape2, Standard_True, Standard_False);
      ic->ClearPrs(mySimulationShape2);

      mySimulationShape2 = new AIS_Shape(TopoDS_Shape());
      mySimulationShape2->Set(S2);
      mySimulationShape2->SetColor(Quantity_NOC_VIOLET);

      ic->Deactivate(mySimulationShape2);
      ic->Display(mySimulationShape2, Standard_False);
      mySimulationShape2->UnsetColor();
    }
    ic->UpdateCurrentViewer();
  }
  catch(Standard_Failure) {
    MESSAGE("Exception catched in SketcherGUI::DisplaySimulationShape ");
  } 
  return;
}


//==================================================================================
// function : EraseSimulationShape()
// purpose  : Clears the display of 'mySimulationShape' a pure graphical shape
//==================================================================================
void SketcherGUI::EraseSimulationShape()
{
  int count = QAD_Application::getDesktop()->getActiveStudy()->getStudyFramesCount();
  for(int i = 0; i < count; i++) {
    if(QAD_Application::getDesktop()->getActiveStudy()->getStudyFrame(i)->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)QAD_Application::getDesktop()->getActiveStudy()->getStudyFrame(i)->getRightFrame()->getViewFrame())->getViewer();
      Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
      ic->Erase(mySimulationShape1, Standard_True, Standard_False);
      ic->ClearPrs(mySimulationShape1);
      ic->Erase(mySimulationShape2, Standard_True, Standard_False);
      ic->ClearPrs(mySimulationShape2);
      ic->UpdateCurrentViewer();
    } 
  }
}


//=====================================================================================
// EXPORTED METHODS
//=====================================================================================
extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {return SketcherGUI::OnGUIEvent(theCommandID, parent);}
}
