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
//  File   : GEOMBase_Sketcher.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "GEOMBase_Sketcher.h"

#include "QAD_RightFrame.h"
#include "OCCViewer_Viewer3d.h"

#include <BRepTools_WireExplorer.hxx>
#include <TopoDS_Wire.hxx>
#include <BRep_Tool.hxx>
#include <TopExp.hxx>
#include <Geom_Circle.hxx>

//=======================================================================
// function : GEOMBase_Sketcher()
// purpose  : Constructor
//=======================================================================
GEOMBase_Sketcher::GEOMBase_Sketcher() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = myGeomGUI->GetDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
  mySketcher = myGeomGUI->GetSketcher();
}


//=======================================================================
// function : ~GEOMBase_Sketcher()
// purpose  : Destructor
//=======================================================================
GEOMBase_Sketcher::~GEOMBase_Sketcher()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool GEOMBase_Sketcher::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
  QMenuData* pp;

  switch (theCommandID)
    {
    case 404: // SKETCHER
      {
	((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->onViewTop(); // DCQ : 28/02/2002
	
	OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();

	mySketcher = Sketch(v3d->getViewer3d());
	myGeomGUI->SetState(CURRENT_SKETCH);

        QMenuItem* item = Mb->findItem(4061, &pp);
	mySketcher.SetParameterVisibility(LENGTH_PARAMETER, pp->isItemChecked(4061));
	item = Mb->findItem(4062, &pp);
	mySketcher.SetParameterVisibility(ANGLE_PARAMETER, pp->isItemChecked(4062));
	item = Mb->findItem(4063, &pp);
	mySketcher.SetParameterVisibility(RADIUS_PARAMETER, pp->isItemChecked(4063));
	item = Mb->findItem(4064, &pp);
	mySketcher.SetParameterVisibility(XVALUE_PARAMETER, pp->isItemChecked(4064));
	item = Mb->findItem(4065, &pp);
	mySketcher.SetParameterVisibility(YVALUE_PARAMETER, pp->isItemChecked(4065));
	
	mySketcher.SetTransitionStatus(NOCONSTRAINT);
	item = Mb->findItem(4052, &pp);
	pp->setItemChecked(4052, false);
	item = Mb->findItem(4053, &pp);
	pp->setItemChecked(4053, false);
	break;
      }
    case 4041: // SKETCH Segment
      {
	mySketcher.ChangeMode(SEGMENT);
	break;
      }
    case 4042: // SKETCH Arc
      {
	mySketcher.ChangeMode(ARC_CHORD);
	break;
      }
    case 4043: // SKETCH Set Angle
      {
	OnSketchSetAngle();
	break;
      }
    case 4044: // SKETCH Set X
      {
	OnSketchSetx();
	break;
      }
    case 4045: // SKETCH Set Y
      {
	OnSketchSety();
	break;
      }
    case 4046: // SKETCH Delete
      {
	OnSketchDelete();
	break;
      }
    case 4047: // SKETCH End
      {
	OnSketchEnd();
	break;
      }
    case 4048: // SKETCH Close
      {
	OnSketchClose();
	break;
      }
    case 4051: // sketcher Set Plane
      {
	//TO DO
	break;
      }
    case 4052: // sketcher TANGENT
      { 
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	if(pp->isItemChecked(theCommandID) == true) 
	  mySketcher.SetTransitionStatus(TANGENT);
	else
	  mySketcher.SetTransitionStatus(NOCONSTRAINT);

	pp->setItemChecked(4053, false);
	break;
      }
    case 4053: // sketcher PERPENDICULAR
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID,!pp->isItemChecked(theCommandID));
	if(pp->isItemChecked(theCommandID) == true) 
	  mySketcher.SetTransitionStatus(PERPENDICULAR);
	else 
	  mySketcher.SetTransitionStatus(NOCONSTRAINT);
	
	pp->setItemChecked(4052, false);
	break;
      }
    case 4061: // SKETCH OptionsOnofflengthdimension
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	mySketcher.SetParameterVisibility(LENGTH_PARAMETER, pp->isItemChecked(theCommandID));
	break;
      }
    case 4062: // SKETCH OptionsOnoffangledimension
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	mySketcher.SetParameterVisibility(ANGLE_PARAMETER, pp->isItemChecked(theCommandID));
	break;
      }
    case 4063: // SKETCH OptionsOnoffradiusdimension
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	mySketcher.SetParameterVisibility(RADIUS_PARAMETER, pp->isItemChecked(theCommandID));
	break;
      }
    case 4064: // SKETCH OptionsOnoffxdimension
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	mySketcher.SetParameterVisibility(XVALUE_PARAMETER, pp->isItemChecked(theCommandID));
	break;
      }
    case 4065: // SKETCH OptionsOnoffydimension
      {
	QMenuItem* item = Mb->findItem(theCommandID, &pp);
	pp->setItemChecked(theCommandID, !pp->isItemChecked(theCommandID));
	mySketcher.SetParameterVisibility(YVALUE_PARAMETER, pp->isItemChecked(theCommandID));
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
// function : OnSketchSetAngle()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchSetAngle()
{
  Standard_Real anAngle = mySketcher.GetSegmentAngle()/PI180;
  Sketch::fitInResol(anAngle); 
  Standard_Boolean res = false;
  QString Value = QString("%1").arg(anAngle);
  anAngle = myGeomGUI->Parameter(res, Value, tr("GEOM_MEN_ANGLE"), tr("GEOM_MEN_ENTER_ANGLE"),
		      -180.0, +180.0, 6) * PI180;

  if(res) {
    mySketcher.SetSegmentAngle(anAngle);
    QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
    QMenuData* pp;
    QMenuItem* item = Mb->findItem(4052, &pp);
    pp->setItemChecked(4052, false);
    item = Mb->findItem(4053, &pp);
    pp->setItemChecked(4053, false);
  }
  return;
}


//=======================================================================
// function : OnSketchSetx()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchSetx()
{
  Standard_Boolean res = false;
  double X = myGeomGUI->Parameter(res, "0.", tr("GEOM_MEN_X"), tr("GEOM_MEN_SKETCHER_X"),
		       2.0 * Precision::Confusion(), 1E6, 6);
  if(res)
    mySketcher.SetXDimension(X);
  QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
  QMenuData* pp;
  QMenuItem* item = Mb->findItem(4052, &pp);
  pp->setItemChecked(4052, false);
  item = Mb->findItem(4053, &pp);
  pp->setItemChecked(4053, false);
  return;
}


//=======================================================================
// function : OnSketchSety()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchSety()
{
  Standard_Boolean res = false;
  double Y = myGeomGUI->Parameter(res, "0.", tr("GEOM_MEN_Y"), tr("GEOM_MEN_SKETCHER_Y"),
		       2.0 * Precision::Confusion(), 1E6, 6);
  if(res)
    mySketcher.SetYDimension(Y);
  QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
  QMenuData* pp;
  QMenuItem* item = Mb->findItem(4052, &pp);
  pp->setItemChecked(4052, false);
  item = Mb->findItem(4053, &pp);
  pp->setItemChecked(4053, false);
  return;
}


//=======================================================================
// function : OnSketchDelete()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchDelete()
{
  if(mySketcher.GetmyEdgesNumber() == 1) {
    QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
    QMenuData* pp;
    QMenuItem* item = Mb->findItem(406, &pp);
    pp->setItemEnabled(406, false);  // SKETCH CONTRAINTS
    mySketcher.SetTransitionStatus(NOCONSTRAINT);
  }

  if(mySketcher.Delete())
    myGeomGUI->ResetState();
  return;
}


//=======================================================================
// function : OnSketchClose()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchClose()
{
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle(AIS_InteractiveContext) myContext = v3d->getAISContext();

  TopoDS_Wire W = mySketcher.Close();
  if(!W.IsNull()) {
    GEOM::GEOM_Gen::ListOfIOR_var listShapes = new GEOM::GEOM_Gen::ListOfIOR;
    listShapes->length(0);
    unsigned int i = 0;

    BRepTools_WireExplorer Ex(W);
    while(Ex.More()) {
      TopoDS_Edge E = Ex.Current();
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
      }
      else {
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
    myGeomGUI->GetShapeTypeString(S,type);
    Wire->NameType(type);

    if(myGeomGUI->Display(Wire))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  myGeomGUI->ResetState();
  QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
  QMenuData* pp;
  QMenuItem* item = Mb->findItem(406, &pp);
  pp->setItemEnabled(406, false);  // SKETCH CONTRAINTS
  mySketcher.SetTransitionStatus(NOCONSTRAINT);
  return;
}


//=======================================================================
// function : OnSketchEnd()
// purpose  : 
//=======================================================================
void GEOMBase_Sketcher::OnSketchEnd()
{
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) myContext = v3d->getAISContext();

  TopoDS_Wire W = mySketcher.End();
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
    myGeomGUI->GetShapeTypeString(S,type);
    Wire->NameType(type);

    if(myGeomGUI->Display(Wire))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  myGeomGUI->ResetState();
  QMenuBar* Mb = myGeomGUI->GetDesktop()->getMainMenuBar();
  QMenuData* pp;
  QMenuItem* item = Mb->findItem(406, &pp);
  pp->setItemEnabled(406, false);  // SKETCH CONTRAINTS
  mySketcher.SetTransitionStatus(NOCONSTRAINT);
  return;
}
