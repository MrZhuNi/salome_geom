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
//  File   : GEOMBase_Context.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "GEOMBase_Context.h"

////  SALOME Includes
# include "Utils_ORB_INIT.hxx"
# include "Utils_SINGLETON.hxx"

#include "QAD_RightFrame.h"
#include "QAD_MessageBox.h"

#include "GEOM_AssemblyBuilder.h"
#include "VTKViewer_ViewFrame.h"
#include "OCCViewer_ViewPort3d.h"
#include "OCCViewer_Viewer3d.h"

#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOMEGUI_ImportOperation.h"
#include "SALOMEGUI_NameDlg.h"            

// // Open CASCADE Includes
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRep_Tool.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Geom_Circle.hxx>

#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopoDS_Iterator.hxx>
#include <TopoDS_Compound.hxx>
#include <TopAbs.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <ProjLib.hxx>
#include <ElSLib.hxx>

// // QT Includes
#include <qmessagebox.h>

#include "GEOMBase_Display.h"
#include "GEOMBase_aParameterDlg.h"

/* The object itself created in the static method 'GetOrCreateGEOMBase()' */
static GEOMBase_Context* GeomGUI = 0;

//=======================================================================
// function : GEOMBase_Context()
// purpose  : Constructor
//=======================================================================
GEOMBase_Context::GEOMBase_Context() :
  QObject()
{
  mySettings_AddInStudy = false;
  mySettings_Copy = false;
  myFatherior = "";
}


//=======================================================================
// function : ~GEOMBase_Context()
// purpose  : Destructor
//=======================================================================
GEOMBase_Context::~GEOMBase_Context()
{
}


//=======================================================================
// function : GetGEOMBase_Context() static
// purpose  : Returns current 'GeomGUI' a static pointer
//=======================================================================
GEOMBase_Context* GEOMBase_Context::GetGeomGUI()
{
  return GeomGUI;
}


//=======================================================================
// function : GetOrCreateGEOMBase()
// purpose  : Gets or create an object 'GEOMBase' with initialisations
//          : Returns 'GeomGUI' as a pointer
//=======================================================================
GEOMBase_Context* GEOMBase_Context::GetOrCreateGeomGUI(QAD_Desktop* desktop)
{
  if(GeomGUI == 0) {
    GeomGUI = new GEOMBase_Context;
    GeomGUI->myActiveDialogBox = 0;
    GeomGUI->mySimulationShape = new AIS_Shape(TopoDS_Shape());
    GeomGUI->myState = -1;
    GeomGUI->myDesktop = desktop; 
    GeomGUI->myActiveStudy = desktop->getActiveStudy();
    GeomGUI->mySimulationActor = vtkActorCollection::New();

    GeomGUI->myShadingColor = Quantity_Color(Quantity_NOC_GOLDENROD);

    Engines::Component_var comp = desktop->getEngine("FactoryServer", "GEOM");
    GeomGUI->myComponentGeom = GEOM::GEOM_Gen::_narrow(comp);
    
    /* GetCurrentStudy */
    int studyId = GeomGUI->myActiveStudy->getStudyId();
    GeomGUI->myComponentGeom->GetCurrentStudy(studyId);

    GeomGUI->myNbGeom = GeomGUI->myComponentGeom->NbLabels();
  } 
  else {
    /* study may have changed */
    GeomGUI->myActiveStudy = desktop->getActiveStudy();
  }

  return GeomGUI;
}


//=====================================================================================
// function : GetIndex()
// purpose  : Get the index of a sub shape in a main shape : index start at 1
//=====================================================================================
int GEOMBase_Context::GetIndex(const TopoDS_Shape& subshape, const TopoDS_Shape& shape, int /*ShapeType*/) 
{ 
  if(shape.IsNull() || subshape.IsNull()) 
    return -1; 
  
  int index = 1; 
  if(subshape.ShapeType() == TopAbs_COMPOUND) { 
    TopoDS_Iterator it; 
    TopTools_ListOfShape CL; 
    CL.Append(shape); 
    TopTools_ListIteratorOfListOfShape itC; 
    for(itC.Initialize(CL); itC.More(); itC.Next()) { 
      for(it.Initialize(itC.Value()); it.More(); it.Next()) { 
	if(it.Value().ShapeType() == TopAbs_COMPOUND) {
	  if(it.Value().IsSame(subshape)) 
	    return index; 
	  else 
	    index++; 
	  CL.Append(it.Value()); 
	}
      } 
    }
  }
  else { 
    TopExp_Explorer Exp(shape, subshape.ShapeType()); 
    TopTools_MapOfShape M; 
    while(Exp.More()) {
      if(M.Add(Exp.Current())) { 
	if(Exp.Current().IsSame(subshape)) 
	  return index; 
	index++; 
      } 
      Exp.Next(); 
    } 
  } 
  return -1; 
}


//=================================================================================
// function : VertexToPoint()
// purpose  : If S can be converted in a gp_Pnt returns true and the result is P
//=================================================================================
bool GEOMBase_Context::VertexToPoint(const TopoDS_Shape& S, gp_Pnt& P)
{
  if(S.IsNull() || S.ShapeType() != TopAbs_VERTEX)
    return false;
  P = BRep_Tool::Pnt(TopoDS::Vertex(S));
  return true;
}


//=================================================================================
// function : GetBipointDxDyDz()
// purpose  : 
//=================================================================================
void GEOMBase_Context::GetBipointDxDyDz(gp_Pnt P1, gp_Pnt P2, double& dx, double& dy, double& dz)
{
  dx = P2.X() - P1.X();
  dy = P2.Y() - P1.Y();
  dz = P2.Z() - P1.Z();
  return;
}


//=================================================================================
// function : LinearEdgeExtremities()
// purpose  : If S can be converted in a linear edge and if initial an final points
//          : distance is sufficient, returns true else returns false.
//          : Resulting points are respectively P1 and P2
//=================================================================================
bool GEOMBase_Context::LinearEdgeExtremities(const TopoDS_Shape& S,  gp_Pnt& P1, gp_Pnt& P2)
{
  if(S.IsNull() || S.ShapeType() != TopAbs_EDGE)
    return false;
  BRepAdaptor_Curve curv(TopoDS::Edge(S));
  if(curv.GetType() != GeomAbs_Line)
    return false;

  curv.D0(curv.FirstParameter(), P1);
  curv.D0(curv.LastParameter(), P2);

  if(P1.Distance(P2) <= Precision::Confusion())
    return false;

  return true;
}


//=======================================================================
// function : GetTopoFromSelection()
// purpose  : Define tds from a single selection and retuen true
//=======================================================================
bool GEOMBase_Context::GetTopoFromSelection(SALOME_Selection *Sel, TopoDS_Shape& tds) 
{
  if(Sel->IObjectCount() != 1)
    return false;
  
  Handle(SALOME_InteractiveObject) IO = Sel->firstIObject();
  /* case SObject */
  if(IO->hasEntry()) {
    SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
    SALOMEDS::SObject_var obj = aStudy->FindObjectID(IO->getEntry());
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeIOR_var anIOR;
    if(!obj->_is_nil()) {
      if(obj->FindAttribute(anAttr, "AttributeIOR")) {
	anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	tds = this->GetShapeFromIOR(anIOR->Value());
	if(tds.IsNull())
	  return false;
	else
	  return true;
      }
    }
  }

  if(IO->IsInstance(STANDARD_TYPE(GEOM_InteractiveObject))) {
    Standard_CString ior = "";
    Handle(GEOM_InteractiveObject) GIObject = Handle(GEOM_InteractiveObject)::DownCast(IO);
    ior = GIObject->getIOR();
    tds = this->GetShapeFromIOR(ior);
    if(tds.IsNull())
      return false;
    else
      return true;
  }
  
  return false;
}


//=====================================================================================
// function : PrepareSubShapeSelection()
// purpose  : (localContextId of the method is opened and defined here)
//=====================================================================================
bool GEOMBase_Context::PrepareSubShapeSelection(const int SubShapeType, Standard_Integer& returnLocalContextId)
{
  //* Test the type of viewer */
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;
  
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
  
  /* local context opening */
  this->SetDisplayedObjectList();
  GEOMBase_Display* myDisplayGUI = new GEOMBase_Display();
  myDisplayGUI->OnDisplayOnly();

  returnLocalContextId = ic->OpenLocalContext(Standard_False, Standard_True, Standard_False, Standard_False);
  SALOME_Selection* Sel = SALOME_Selection::Selection(myActiveStudy->getSelection());
  SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
  for(;It.More();It.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();
    Standard_Boolean found;
    Handle(GEOM_AISShape) Shape = this->ConvertIOinGEOMAISShape(IObject, found);
    if(found && SubShapeType >= Shape->Shape().ShapeType()) {
      ic->Load(Shape, (8 - SubShapeType), Standard_True);
      ic->HilightWithColor(Shape, Quantity_NOC_RED);
    }
  }
  myDesktop->putInfo(tr("GEOM_PRP_SELECT_SUBSHAPES"));
  return true;
}


//=====================================================================================
// function : PrepareSubShapeSelectionArgumentShape()
// purpose  : ( localContextId of the method is opened and defined here )
//=====================================================================================
bool GEOMBase_Context::PrepareSubShapeSelectionArgumentShape(const TopoDS_Shape& aShape,const int SubShapeType, Standard_Integer& returnLocalContextId )
{
  //* Test the type of viewer */
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;
  
  if(aShape.IsNull())
    return false ;
  
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
  
  /* local context opening */
  this->SetDisplayedObjectList();
  GEOMBase_Display* myDisplayGUI = new GEOMBase_Display();
  myDisplayGUI->OnDisplayOnly();
  
  returnLocalContextId = ic->OpenLocalContext(Standard_False, Standard_True, Standard_False, Standard_False) ;
  
  Handle(GEOM_AISShape) Shape = new GEOM_AISShape(aShape, "");  
  ic->Display(Shape, 0, (8 - SubShapeType));

  //  Not Load(...) but Display(...)
  //  ic->Load(Shape, (8 - SubShapeType), Standard_True);
  ic->HilightWithColor(Shape, Quantity_NOC_RED);
  
  myDesktop->putInfo(tr("GEOM_PRP_SELECT_SUBSHAPES"));
  return true;
}


//=======================================================================
// function : GetNameOfSelectedIObjects()
// purpose  : Define the name geom++ or other name of mono or multi sel.
//=======================================================================
int GEOMBase_Context::GetNameOfSelectedIObjects(SALOME_Selection* Sel, QString& aName)
{
  int nbSel = Sel->IObjectCount();
  if(nbSel == 1) {
    Handle(SALOME_InteractiveObject) IObject = Sel->firstIObject();
    aName = IObject->getName();
  } 
  else {
    aName = tr("%1_objects").arg(nbSel);
  }
  return nbSel;
}


//=======================================================================
// function : ConvertIOinGEOMAISShape()
// purpose  :
//=======================================================================
Handle(GEOM_AISShape) GEOMBase_Context::ConvertIOinGEOMAISShape(const Handle(SALOME_InteractiveObject)& IO, Standard_Boolean& testResult, bool onlyInActiveView)
{
  Handle(GEOM_AISShape) res;
  int nbSf = myActiveStudy->getStudyFramesCount();
  for(int i = 0; i < nbSf; i++) {
    QAD_StudyFrame* sf = myActiveStudy->getStudyFrame(i);
    if(sf->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)sf->getRightFrame()->getViewFrame())->getViewer();
      Handle (AIS_InteractiveContext) ic = v3d->getAISContext();

      AIS_ListOfInteractive List;
      ic->DisplayedObjects(List);
      AIS_ListOfInteractive List1;
      ic->ObjectsInCollector(List1);
      List.Append(List1);
      
      AIS_ListIteratorOfListOfInteractive ite(List);
      while(ite.More()) {
	if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	  Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
	  if(aSh->hasIO()) {
	    Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(aSh->getIO());
	    if(GIO->isSame(IO)) {
	      if(onlyInActiveView) {
		if(sf == myActiveStudy->getActiveStudyFrame()) {
		  testResult = true;
		  return aSh;
		}
	      } 
	      else {
		testResult = true;
		return aSh;
	      }
	    }
	  }
	}
	ite.Next();
      }  
    }
  }
  testResult = false;
  return res;
}


//=======================================================================
// function : ConvertIORinGEOMAISShape()
// purpose  :
//=======================================================================
Handle(GEOM_AISShape) GEOMBase_Context::ConvertIORinGEOMAISShape(const char * IOR, Standard_Boolean& testResult, bool onlyInActiveView)
{
  Handle(GEOM_AISShape) resultShape;
  testResult = false;
  int nbSf = myActiveStudy->getStudyFramesCount();
  for(int i = 0; i < nbSf; i++) {
    QAD_StudyFrame* sf = myActiveStudy->getStudyFrame(i);
    if(sf->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)sf->getRightFrame()->getViewFrame())->getViewer();
      Handle (AIS_InteractiveContext) ic = v3d->getAISContext();

      AIS_ListOfInteractive List;
      ic->DisplayedObjects(List);
      AIS_ListOfInteractive List1;
      ic->ObjectsInCollector(List1);
      List.Append(List1);

      AIS_ListIteratorOfListOfInteractive ite(List);
      while(ite.More()) {
	if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	  Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
	  if(aSh->hasIO()) {
	    Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(aSh->getIO());
	    Standard_CString theIOR = GIO->getIOR();
	    if(strcmp(IOR, theIOR) == 0) {
	      if(onlyInActiveView) {
		if(sf == myActiveStudy->getActiveStudyFrame()) {
		  testResult = true;
		  resultShape = aSh;
		  return resultShape; 
		}
	      } 
	      else {
		testResult = true;
		resultShape = aSh;
		return resultShape; 
	      }
	    }
	  }
	}
	ite.Next();
      }  
    }
  }
  return  resultShape;
}


//=======================================================================
// function : ConvertIORinGEOMActor()
// purpose  :
//=======================================================================
GEOM_Actor* GEOMBase_Context::ConvertIORinGEOMActor(const char* IOR, Standard_Boolean& testResult, bool onlyInActiveView)
{
  int nbSf = myActiveStudy->getStudyFramesCount();
  for(int i = 0; i < nbSf; i++) {
    QAD_StudyFrame* sf = myActiveStudy->getStudyFrame(i);
    if(sf->getTypeView() == VIEW_VTK) {
      vtkRenderer* Renderer = ((VTKViewer_ViewFrame*)sf->getRightFrame()->getViewFrame())->getRenderer();
      vtkActorCollection* theActors = Renderer->GetActors();
      theActors->InitTraversal();
      vtkActor *ac = theActors->GetNextActor();
      while(!(ac==NULL)) {
	if( ac->IsA("GEOM_Actor")) {
	  GEOM_Actor* anActor = GEOM_Actor::SafeDownCast(ac);
	  if(anActor->hasIO()) {
	    Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(anActor->getIO());
	    Standard_CString theIOR = GIO->getIOR();
	    if(strcmp(IOR, theIOR) == 0) {
	      if(onlyInActiveView) {
		if(sf == myActiveStudy->getActiveStudyFrame()) {
		  testResult = true;
		  return anActor;
		}
	      } 
	      else {
		testResult = true;
		return anActor;
	      }
	    }
	  }
	}
	ac = theActors->GetNextActor();
      }
    }
  }
  testResult = false;
  return GEOM_Actor::New();
}


//=======================================================================
// function : ConvertIOinGEOMShape()
// purpose  :
//=======================================================================
GEOM::GEOM_Shape_ptr GEOMBase_Context::ConvertIOinGEOMShape(const Handle(SALOME_InteractiveObject)& IO, Standard_Boolean& testResult)
{
  GEOM::GEOM_Shape_ptr aShape;
  testResult = false;

  /* case SObject */
  if(IO->hasEntry()) {
    SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
    SALOMEDS::SObject_var obj = aStudy->FindObjectID(IO->getEntry());
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeIOR_var anIOR;
    if(!obj->_is_nil()) {
      if(obj->FindAttribute(anAttr, "AttributeIOR")) {
	anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	aShape = myComponentGeom->GetIORFromString(anIOR->Value());
	testResult = true;
	return aShape;
      }
    }
  }
  /* case Graphical Object */
  if(IO->IsInstance(STANDARD_TYPE(GEOM_InteractiveObject))) {
    Handle(GEOM_InteractiveObject) GIObject = Handle(GEOM_InteractiveObject)::DownCast(IO);
    Standard_CString ior = GIObject->getIOR();
    testResult = true;
    aShape = myComponentGeom->GetIORFromString(ior);
    return aShape;
  } 
  return aShape;
}


//=======================================================================
// function : ConvertListOfIOInListOfIOR()
// purpose  : 
//=======================================================================
void GEOMBase_Context::ConvertListOfIOInListOfIOR(const SALOME_ListIO& aList, GEOM::GEOM_Gen::ListOfIOR& listIOR)
{
  int nbSel = aList.Extent();  
  listIOR.length(nbSel);
  int j=0;
  SALOME_ListIteratorOfListIO It(aList);
  for(int i=0; It.More(); It.Next(), i++) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();
    if(IObject->hasEntry()) {
      SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
      SALOMEDS::SObject_var obj = aStudy->FindObjectID(IObject->getEntry());
      SALOMEDS::GenericAttribute_var anAttr;
      SALOMEDS::AttributeIOR_var anIOR;
      if(!obj->_is_nil() && obj->FindAttribute(anAttr, "AttributeIOR")) {
	anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
	CORBA::ORB_var& _orb = init.orb();
	CORBA::String_var theValue = anIOR->Value();
	CORBA::Object_var theObj = _orb->string_to_object(theValue);
	if(theObj->_is_a("IDL:GEOM/GEOM_Shape:1.0")) {
	  listIOR[j] = strdup(theValue);
	  j++;
	}
      }
    } 
    else if(IObject->IsInstance(STANDARD_TYPE(GEOM_InteractiveObject))) {
      Handle(GEOM_InteractiveObject) GIObject = Handle(GEOM_InteractiveObject)::DownCast(IObject);
      Standard_CString ior = GIObject->getIOR();
      listIOR[j] = strdup(ior);
      j++;
    }
  }
  listIOR.length(j);
}


//=================================================================================
// function : GetShapeTypeString()
// purpose  : for a single shape
//=================================================================================
bool GEOMBase_Context::GetShapeTypeString(const TopoDS_Shape& aShape, Standard_CString& aTypeString)
{
  if(aShape.IsNull()) {
    aTypeString = "aNullShape";
    return false;
  }
  switch(aShape.ShapeType())
    {
    case TopAbs_COMPOUND:
      {
	aTypeString = strdup(tr("GEOM_COMPOUND"));
	return true;
      }
    case  TopAbs_COMPSOLID:
      {
	aTypeString = strdup(tr("GEOM_COMPOUNDSOLID")) ; 
	return true ;
      }
    case TopAbs_SOLID:
      {
	aTypeString = strdup(tr("GEOM_SOLID")) ; 
	return true ;
      }
    case TopAbs_SHELL:
      {
	aTypeString = strdup(tr("GEOM_SHELL")) ; 
	return true ;
      }
    case TopAbs_FACE:
      { 
	BRepAdaptor_Surface surf(TopoDS::Face(aShape));
	if(surf.GetType() == GeomAbs_Plane) {
	  aTypeString = strdup(tr("GEOM_PLANE"));
	  return true;
	}
	else if(surf.GetType() == GeomAbs_Cylinder) {
	  aTypeString = strdup(tr("GEOM_SURFCYLINDER"));
	  return true;
	}
	else if(surf.GetType() == GeomAbs_Sphere) {
	  aTypeString = strdup(tr("GEOM_SURFSPHERE"));
	  return true ;
	}
	else if(surf.GetType() == GeomAbs_Torus) {
	  aTypeString = strdup(tr("GEOM_SURFTORUS"));
	  return true ;
	}
	else if(surf.GetType() == GeomAbs_Cone) {
	  aTypeString = strdup(tr("GEOM_SURFCONE"));
	  return true ;
	}
	else {
	  aTypeString = strdup(tr("GEOM_FACE"));
	  return true;
	}
      }
    case TopAbs_WIRE:
      {
	aTypeString = strdup(tr("GEOM_WIRE")); 
	return true;
      }
    case TopAbs_EDGE:
      {
	BRepAdaptor_Curve curv(TopoDS::Edge(aShape));
	if(curv.GetType() == GeomAbs_Line) {
	  if((Abs(curv.FirstParameter()) >= 1E6) || (Abs(curv.LastParameter()) >= 1E6))
	    aTypeString = strdup(tr("GEOM_LINE"));
	  else
	    aTypeString = strdup(tr("GEOM_EDGE"));
	  return true;
	}
	else if(curv.GetType() == GeomAbs_Circle) {
	  if(curv.IsClosed())
	    aTypeString = strdup(tr("GEOM_CIRCLE"));
	  else
	    aTypeString = strdup(tr("GEOM_ARC"));
	return true;
      } 
	else {
	  aTypeString = strdup(tr("GEOM_EDGE"));
	  return true;
	}
      }
    case TopAbs_VERTEX:
      {
	aTypeString = strdup(tr("GEOM_VERTEX"));
	return true;
      }
    case TopAbs_SHAPE:
      {
	aTypeString = strdup(tr("GEOM_SHAPE"));
	return true;
      }
    }
  return false;
}


//=================================================================================
// function : CreateArrowForLinearEdge()
// purpose  : Create a cone topology to be used to display an arrow in the middle
//          : of an edge showing its orientation. (For simulation and Viewer OCC only)
//=================================================================================
bool GEOMBase_Context::CreateArrowForLinearEdge(const TopoDS_Shape& tds, TopoDS_Shape& ArrowCone)
{
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() != VIEW_OCC || tds.ShapeType() != TopAbs_EDGE)
    return false;
  
  OCCViewer_ViewPort* vp = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewPort();
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)vp)->getView();
  Standard_Real Width, Height;
  view3d->Size(Width, Height);
  const Standard_Real aHeight = (Width + Height) / 50.0;
  
  try {
    Standard_Real first, last;
    Handle(Geom_Curve) curv = BRep_Tool::Curve(TopoDS::Edge(tds), first, last);
    if(!curv->IsCN(1))
      return false;

    const Standard_Real param = (first+last) / 2.0;
    gp_Pnt middleParamPoint;
    gp_Vec V1;
    curv->D1( param, middleParamPoint, V1);
    if(V1.Magnitude() < Precision::Confusion())
      return false;
    
    /* Topology orientation not geom orientation */
    if(tds.Orientation() == TopAbs_REVERSED)
      V1 *= -1.0;
    
    gp_Ax2 anAxis( middleParamPoint, gp_Dir(V1));
    const Standard_Real radius1 = aHeight / 5.0;
    if(radius1 > 10.0 * Precision::Confusion() && aHeight > 10.0 * Precision::Confusion()) {
      ArrowCone = BRepPrimAPI_MakeCone( anAxis, radius1, 0.0, aHeight ).Shape();
      return true;
    }
  }
  catch(Standard_Failure) {
    // OCC failures are hard to catch in GUI.
    // This  because of the position for  #include <Standard_ErrorHandler.hxx> that is very critic to find 
    // in SALOME environment : compilation error !
  }
  return false;
}


//=======================================================================
// function : SelectionByNameInDialogs()
// purpose  : Called when user has entered a name of object in a LineEdit.
//          : The selection is changed. Dialog box will receive the 
//          : corresponding signal to manage this event.
//=======================================================================
bool GEOMBase_Context::SelectionByNameInDialogs(QWidget* aWidget, const QString& objectUserName, SALOME_Selection* Sel)
{
  /* Find SObject with name in component GEOM */
  SALOMEDS::Study_var ST = this->myActiveStudy->getStudyDocument();
  SALOMEDS::Study::ListOfSObject_var listSO = new SALOMEDS::Study::ListOfSObject;
  listSO = ST->FindObjectByName(objectUserName, "GEOM");
  
  if(listSO->length() < 1) {
    const QString caption  = tr("GEOM_WRN_WARNING");
    const QString text = tr("GEOM_NAME_INCORRECT");
    const QString button0  = tr("GEOM_BUT_OK");
    QMessageBox::warning(aWidget, caption, text, button0);
    return false;
  }
  /* More than one object with same name */
  if(listSO->length() > 1) {
    const QString caption  = tr("GEOM_WRN_WARNING");
    const QString text = tr("GEOM_IDENTICAL_NAMES_SELECT_BY_MOUSE");
    const QString button0  = tr("GEOM_BUT_OK") ;
    QMessageBox::warning(aWidget, caption, text, button0) ;
    return false;
  }

  SALOMEDS::SObject_var theObj = listSO[0];
  /* Create a SALOME_InteractiveObject with a SALOME::SObject */
  Standard_CString anEntry = theObj->GetID();
  Handle(SALOME_InteractiveObject) SI = new SALOME_InteractiveObject(anEntry, "GEOM", strdup(objectUserName));

  /* Add as a selected object       */
  /* Clear any previous selection : */
  /* Warning the LineEdit is purged because of signal currentSelectionChanged ! */ 
  Sel->ClearIObjects(); 
  Sel->AddIObject(SI);
  return true;
}


//=======================================================================
// function : SetState()
// purpose  : Sets myState = aState a private field indicating which methode is active
//=======================================================================
void GEOMBase_Context::SetState(int aState)
{
  this->myState = aState;
  return;
}


//=======================================================================
// function : ResetState()
// purpose  : Sets myState = -1 a private field indicating which methode is active
//=======================================================================
void GEOMBase_Context::ResetState()
{
  this->myState = -1;
  return;
}


//=======================================================================
// function : EmitSignalDeactivateDialog()
// purpose  : Emit a signal to deactivate the active dialog Box
//=======================================================================
void GEOMBase_Context::EmitSignalDeactivateDialog()
{
  emit this->SignalDeactivateActiveDialog();
  return;
}


//=======================================================================
// function : EmitSignalCloseAllDialogs()
// purpose  : Emit a signal to deactivate all non modal dialogs box
//=======================================================================
void GEOMBase_Context::EmitSignalCloseAllDialogs()
{
  emit this->SignalCloseAllDialogs();
  return;
}


//=======================================================================
// function : EmitSignalDefaultStepValueChanged()
// purpose  : 
//=======================================================================
void GEOMBase_Context::EmitSignalDefaultStepValueChanged(double newVal)
{
  emit this->SignalDefaultStepValueChanged(newVal);
  return;
}


//=======================================================================
// function : SetActiveDialogBox()
// purpose  : 
//=======================================================================
void GEOMBase_Context::SetActiveDialogBox(QDialog* aDlg)
{
  this->myActiveDialogBox = (QDialog*)aDlg;
  return;
}


//=====================================================================================
// function : GetIndexSubShapeSelected()
// purpose  : Define a ListOfID of sub shapes selected in ShapeTopo with SubShapeType
//          : Method used by Dialogs
//=====================================================================================
bool GEOMBase_Context::GetIndexSubShapeSelected(const TopoDS_Shape& ShapeTopo, const int SubShapeType, GEOM::GEOM_Shape::ListOfSubShapeID& ListOfID, Standard_Integer& aLocalContextId, bool& myUseLocalContext)
{
  //* Test the type of viewer */
  if(this->myActiveStudy->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;
  
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) ic = v3d->getAISContext();

  ic->InitSelected();
  int nbSelected = ic->NbSelected();
  ListOfID.length(nbSelected);
  
  //***************** DEFINE INDEX OF EACH SELECTION *********************//
  int i = 0;
  ic->InitSelected(); /* to restart */
  while(ic->MoreSelected()) {
    /* Find index of sub shape into main shape */
    TopExp_Explorer Exp (ShapeTopo, TopAbs_ShapeEnum(SubShapeType));
    int index = 1;
    bool found = false;
    while( Exp.More()) {
      if((Exp.Current()).IsSame(ic->SelectedShape())) {
	found = true;
	break;
      }
      index++;
      Exp.Next();
    }
    if(!found) {
      /* Manage local context from DialogBox */
      ic->CloseLocalContext(aLocalContextId);
      myUseLocalContext = false;
      return false;
    }
    ListOfID[i] = index;
    i++;
    ic->NextSelected();
  }
    //***************** END  *********************//

  /* Manage local context from DialogBox */
  ic->CloseLocalContext(aLocalContextId);
  myUseLocalContext = false;
  
  return true;
}


//=======================================================================
// function : DefineDlgPosition()
// purpose  : Define x and y the default position for a dialog box
//=======================================================================
bool GEOMBase_Context::DefineDlgPosition(QWidget* aDlg, int& x, int& y)
{
  /* Here the position is on the bottom right corner - 10 */
  QAD_Desktop* PP = QAD_Application::getDesktop();
  x = abs(PP->x() + PP->size().width() - aDlg->size().width() - 10);
  y = abs(PP->y() + PP->size().height() - aDlg->size().height() - 10);
  return true;  
}


//=======================================================================
// function : ConvertClickToPoint()
// purpose  : Returns the point clicked in 3D view
//=======================================================================
gp_Pnt GEOMBase_Context::ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView)
{
  V3d_Coordinate XEye, YEye, ZEye, XAt, YAt, ZAt;
  aView->Eye(XEye, YEye, ZEye);

  aView->At(XAt, YAt, ZAt);
  gp_Pnt EyePoint(XEye, YEye, ZEye);
  gp_Pnt AtPoint(XAt, YAt, ZAt);

  gp_Vec EyeVector(EyePoint, AtPoint);
  gp_Dir EyeDir(EyeVector);

  gp_Pln PlaneOfTheView = gp_Pln(AtPoint,EyeDir);
  Standard_Real X, Y, Z;
  aView->Convert(x, y, X, Y, Z);
  gp_Pnt ConvertedPoint(X, Y, Z);

  gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView, ConvertedPoint);
  gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(), ConvertedPointOnPlane.Y(), PlaneOfTheView);
  return ResultPoint;
}


//================================================================================
// function : SetDisplayedObjectList()
// purpose  :
//================================================================================
void GEOMBase_Context::SetDisplayedObjectList()
{
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return;

  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  myListDisplayedObject.Clear();

  Handle (AIS_InteractiveContext) aContext = v3d->getAISContext();
  aContext->DisplayedObjects(myListDisplayedObject);
}


//=====================================================================================
// function : DisplaySimulationShape() 
// purpose  : Displays 'this->mySimulationShape' a pure graphical shape from a TopoDS_Shape
//=====================================================================================
void GEOMBase_Context::DisplaySimulationShape(const TopoDS_Shape& S) 
{
  if(S.IsNull())
    return;
  	
  //NRI DEBUG : 14/02/2002
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return;
	
  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
  
  try {
    /* erase any previous */
    ic->Erase(this->mySimulationShape, Standard_True, Standard_False);
    ic->ClearPrs(this->mySimulationShape);
    this->mySimulationShape = new AIS_Shape(TopoDS_Shape());
    this->mySimulationShape->Set(S);
    this->mySimulationShape->SetColor(Quantity_NOC_VIOLET);
    ic->Deactivate(this->mySimulationShape);
    ic->Display(this->mySimulationShape, Standard_False);
    ic->UpdateCurrentViewer();
  }
  catch(Standard_Failure) {
    MESSAGE("Exception catched in GEOMBase_Context::DisplaySimulationShape ");
  } 
  this->mySimulationShape->UnsetColor();
  return;
}


//==================================================================================
// function : EraseSimulationShape()
// purpose  : Clears the display of 'mySimulationShape' a pure graphical shape
//==================================================================================
void GEOMBase_Context::EraseSimulationShape()
{
  int count = myActiveStudy->getStudyFramesCount();
  for(int i = 0; i < count; i++) {
    if(myActiveStudy->getStudyFrame(i)->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getStudyFrame(i)->getRightFrame()->getViewFrame())->getViewer();
      Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
      ic->Erase(this->mySimulationShape, Standard_True, Standard_False);
      ic->ClearPrs(this->mySimulationShape);
      ic->UpdateCurrentViewer();
    } 
  }
}


//=====================================================================================
// function : Display()
// purpose  : Displays a CORBA shape
//=====================================================================================
bool GEOMBase_Context::Display(GEOM::GEOM_Shape_ptr aShape, Standard_CString name)
{
  // MESSAGE ( "GEOMBase_Context::Display init ") 
  Handle(GEOM_InteractiveObject) IO;
  SALOME_Selection* Sel = SALOME_Selection::Selection(myActiveStudy->getSelection());
  Sel->ClearIObjects();

  if(aShape->_is_nil()) {
    QAD_MessageBox::warn1 (QAD_Application::getDesktop(), tr("GEOM_WRN_WARNING"), tr("GEOM_PRP_ABORT"), tr("GEOM_BUT_OK"));
    return false;
  }
  
  TopoDS_Shape shape = myShapeReader.GetShape(myComponentGeom, aShape);
  if(shape.IsNull())
    return false;

  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributePixMap_var aPixmap;

  SALOMEDS::SComponent_var father = aStudy->FindComponent("GEOM");
  int aLocked = false;
  if(father->_is_nil()) {
    QAD_Operation* op = new SALOMEGUI_ImportOperation(myActiveStudy);
    op->start();
    aLocked = aStudy->GetProperties()->IsLocked();
    if(aLocked) 
      aStudy->GetProperties()->SetLocked(false);
    father = aStudyBuilder->NewComponent("GEOM");
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(QAD_Application::getDesktop()->getComponentUserName("GEOM"));
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributePixMap");
    aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
    aPixmap->SetPixMap("ICON_OBJBROWSER_Geometry");
    myActiveStudy->updateObjBrowser();
    if(aLocked)
      aStudy->GetProperties()->SetLocked(true);
    op->finish();
  }

  aStudyBuilder->DefineComponentInstance(father, myComponentGeom);
  father->ComponentIOR(myFatherior);

  TCollection_AsciiString nameG("");
  Standard_CString Type;
  if(TCollection_AsciiString(name).IsEqual(Standard_CString(""))) {
    if(TCollection_AsciiString(aShape->NameType()).IsEqual(Standard_CString(""))) {
      Standard_CString type;
      GetShapeTypeString(shape,type);
      aShape->NameType(type);
      nameG += TCollection_AsciiString(type) + TCollection_AsciiString("_") + TCollection_AsciiString(myNbGeom++) + TCollection_AsciiString("\0");
    } 
    else
      nameG += TCollection_AsciiString(aShape->NameType()) + TCollection_AsciiString("_") + TCollection_AsciiString(myNbGeom++) + TCollection_AsciiString("\0");
  } 
  else 
    nameG = TCollection_AsciiString(name);
  
  // VTK
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
    
    vtkRenderer *theRenderer = ((VTKViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();
    int themode = myRenderInter->GetDisplayMode();

    vtkActorCollection* theActors = GEOM_AssemblyBuilder::BuildActors(shape, 0, themode, Standard_True);
    theActors->InitTraversal();
    vtkActor* anActor = (vtkActor*)theActors->GetNextActor();

    IO = new GEOM_InteractiveObject(aShape->Name(), myFatherior, "GEOM");
    while(!(anActor==NULL)) {
      GEOM_Actor* GActor = GEOM_Actor::SafeDownCast(anActor);
      GActor->setIO(IO);
      GActor->setName(nameG.ToCString());
      
      theRenderer->AddActor(GActor);
      anActor = (vtkActor*)theActors->GetNextActor();
    }
  } 
  // OCC
  else if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
    Handle(GEOM_AISShape) theResult = new GEOM_AISShape(shape, nameG.ToCString());
    theResult->SetShadingColor(myShadingColor);
    IO = new GEOM_InteractiveObject(aShape->Name(), myFatherior, "GEOM");
    theResult->setIO(IO);
    theResult->setName(nameG.ToCString());
    
    /* Precaution : close any local context to permit the display */
    if(ic->HasOpenedContext())
      ic->CloseAllContexts();

    ic->Display(theResult);
  }

  Sel->AddIObject(IO, false);
  myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->Repaint();
  if(mySettings_AddInStudy)
    AddInStudy(false, IO);

  return true;
}


//=====================================================================================
// function : AddInStudy() 
// purpose  : anIOShape or a selected shape
//=====================================================================================
bool GEOMBase_Context::AddInStudy(bool selection, const Handle(SALOME_InteractiveObject)& anIO)
{
  SALOME_Selection* Sel = SALOME_Selection::Selection(myActiveStudy->getSelection());
  if (!(!mySettings_AddInStudy || selection)) {
    Sel->ClearIObjects();
  }

  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributePixMap_var aPixmap;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributeSelectable_var aSelAttr;
  
  SALOMEDS::SComponent_var father = aStudy->FindComponent("GEOM");
  int aLocked = false;
  if(father->_is_nil()) {
    QAD_Operation* op = new SALOMEGUI_ImportOperation(myActiveStudy);
    op->start();
    aLocked = aStudy->GetProperties()->IsLocked();
    if(aLocked)
      aStudy->GetProperties()->SetLocked(false);
    father = aStudyBuilder->NewComponent("GEOM");
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue( QAD_Application::getDesktop()->getComponentUserName("GEOM"));
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributePixMap");
    aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
    aPixmap->SetPixMap("ICON_OBJBROWSER_Geometry");
    if (aLocked)
      aStudy->GetProperties()->SetLocked(true);
    op->finish();
  }

  aStudyBuilder->DefineComponentInstance(father, myComponentGeom);
  father->ComponentIOR(myFatherior);
  
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(myActiveStudy->getActiveStudyFrame()->entry());
  
  Handle(GEOM_AISShape) GAISShape;
  GEOM_Actor* GActor;
  Handle(GEOM_InteractiveObject) GIO;
  bool found = false;

  // VTK
  if (myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    vtkRenderer *Renderer = ((VTKViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();

    vtkActorCollection* theActors = Renderer->GetActors();
    theActors->InitTraversal();
    vtkActor *ac = theActors->GetNextActor();
    while(!(ac==NULL)) {
      if(ac->IsA("GEOM_Actor")) {
	GEOM_Actor* anActor = GEOM_Actor::SafeDownCast(ac);
	if(anActor->hasIO()) {
	  Handle(SALOME_InteractiveObject) IO = anActor->getIO();
	  if(IO->IsKind(STANDARD_TYPE(GEOM_InteractiveObject))) {
	    GIO = Handle(GEOM_InteractiveObject)::DownCast(IO);
	    if(anIO->isSame(GIO)) {
	      found = true;
	      GActor = anActor;
	      break;
	    }
	  }
	}
      }
      ac = theActors->GetNextActor();
    }
    
    if(!found)
      return false;
  }
  // OCC
  else if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
    
    AIS_ListOfInteractive List;
    ic->DisplayedObjects(List);
    AIS_ListIteratorOfListOfInteractive ite(List);
    while(ite.More()) {
      if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
	if(aSh->hasIO()) {
	  Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	  if ( IO->IsKind(STANDARD_TYPE(GEOM_InteractiveObject))) {
	    GIO = Handle(GEOM_InteractiveObject)::DownCast(IO);
	    if(anIO->isSame(GIO)) {
	      found = true;
	      GAISShape = aSh;
	      break;
	    }
	  }
	}
      }
      ite.Next();
    }

    if(!found)
      return false;
  }

  if(!mySettings_AddInStudy || selection) {
    QString Name = SALOMEGUI_NameDlg::getName(QAD_Application::getDesktop(), anIO->getName());
    if(!Name.isEmpty()) {
      // VTK
      if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
	GActor->setName(strdup(Name.latin1()));
      // OCC
      else if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
	GAISShape->setName(strdup(Name.latin1()));
    } 
    else
      return false;
  }

  // open transaction
  QAD_Operation* op = new SALOMEGUI_ImportOperation(myActiveStudy);
  op->start();

  SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(father);

  GEOM::GEOM_Shape_var aShape = myComponentGeom->GetIORFromString(GIO->getIOR());

  /* For the shape inserted into the study we set its field 'studyshapeid'    */
  /* so the shape will contain its corresponding entry in the study Ocaf doc. */
  aShape->StudyShapeId(newObj->GetID());

  GIO->setEntry(newObj->GetID());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
  anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
  anIOR->SetValue(aShape->Name());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
  aName = SALOMEDS::AttributeName::_narrow(anAttr);

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
  if(aShape->ShapeType() == GEOM::COMPOUND)
    aPixmap->SetPixMap("ICON_OBJBROWSER_COMPOUND");
  else if(aShape->ShapeType() == GEOM::COMPSOLID)
    aPixmap->SetPixMap("ICON_OBJBROWSER_COMPSOLID");
  else if(aShape->ShapeType() == GEOM::SOLID)
    aPixmap->SetPixMap("ICON_OBJBROWSER_SOLID");
  else if(aShape->ShapeType() == GEOM::SHELL)
    aPixmap->SetPixMap("ICON_OBJBROWSER_SHELL");
  else if(aShape->ShapeType() == GEOM::FACE)
    aPixmap->SetPixMap("ICON_OBJBROWSER_FACE");
  else if(aShape->ShapeType() == GEOM::WIRE)
    aPixmap->SetPixMap("ICON_OBJBROWSER_WIRE");
  else if(aShape->ShapeType() == GEOM::EDGE)
    aPixmap->SetPixMap("ICON_OBJBROWSER_EDGE");
  else if(aShape->ShapeType() == GEOM::VERTEX)
    aPixmap->SetPixMap("ICON_OBJBROWSER_VERTEX");

  // VTK
  if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    GActor->setIO(GIO);
    aName->SetValue(GActor->getName());
  }
  // OCC
  else if(myActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    GAISShape->setIO(GIO);
    aName->SetValue(GAISShape->getName());
  }

  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
  aStudyBuilder->Addreference(newObj1,newObj);

  GEOM::GEOM_Gen::ListOfIOR_var listIOR = new GEOM::GEOM_Gen::ListOfIOR;
  listIOR = myComponentGeom->GetReferencedObjects(aShape);

  if(listIOR->length()>0) {
    SALOMEDS::SObject_var Arguments = aStudyBuilder->NewObject(newObj);
    anAttr = aStudyBuilder->FindOrCreateAttribute(Arguments, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(tr("GEOM_ARGUMENTS"));
    anAttr = aStudyBuilder->FindOrCreateAttribute(Arguments, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);

    bool ObjectReferenced = false;
    for (unsigned int ind = 0; ind < listIOR->length();ind++) {
      SALOMEDS::SObject_var theObj = aStudy->FindObjectIOR(listIOR[ind]);

      if(!theObj->_is_nil()) {
	SALOMEDS::SObject_var RefObject = aStudyBuilder->NewObject(Arguments);
	aStudyBuilder->Addreference(RefObject, theObj);
	ObjectReferenced = true;
      }
    }

    if(!ObjectReferenced)
      aStudyBuilder->RemoveObject(Arguments);
  }
  op->finish();

  if(!mySettings_AddInStudy || selection)
    myActiveStudy->updateObjBrowser();
  else {
    myActiveStudy->updateObjBrowser(false);
    Sel->AddIObject(GIO);
  }
  return true;
}


//=====================================================================================
// function : GetShapeFromIOR()
// purpose  : exist also as static method !
//=====================================================================================
TopoDS_Shape GEOMBase_Context::GetShapeFromIOR(QString IOR)
{
  TopoDS_Shape result;
  if(IOR.stripWhiteSpace().isEmpty())
    return result;
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  CORBA::ORB_var& _orb = init.orb() ;
  CORBA::Object_var obj = _orb->string_to_object((char*)(IOR.latin1()));
  if(CORBA::is_nil(obj))
    return result;
  GEOM::GEOM_Shape_var aShape = myComponentGeom->GetIORFromString(IOR);
  if(!aShape->_is_nil())
    result = myShapeReader.GetShape(myComponentGeom, aShape);
  return result;
}


//=======================================================================
// function : Parameter()
// purpose  : return a parameter (float) from a dialog box
//
//  avalue1    : is a float or integer used as a default value displayed
//  aTitle1    : is the title for aValue1
//  aTitle     : is the main title
//  bottom     : maximum value to be entered
//  top	       : minimum value to be entered
//  decimals   : number of decimals
//=======================================================================
double GEOMBase_Context::Parameter(Standard_Boolean& res, const char* aValue1, const char* aTitle1, const char* aTitle, const double bottom, const double top, const int decimals)
{
  GEOMBase_aParameterDlg * Dialog = new GEOMBase_aParameterDlg(aValue1, aTitle1, QAD_Application::getDesktop(), aTitle, TRUE, 0, bottom, top, decimals);
  int r = Dialog->exec();
  float X = 0.0;
  if(r == QDialog::Accepted) {
    res = Standard_True;
    X = Dialog->getValue();
  } 
  else
    res = Standard_False;
  delete Dialog;
  return X;
}


//=====================================================================================
// function : SObjectExist()
// purpose  :
//=====================================================================================
bool GEOMBase_Context::SObjectExist(SALOMEDS::SObject_ptr theFatherObject, const char* IOR) {
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(theFatherObject);
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeIOR_var anIOR;
  for(; it->More();it->Next()) {
    SALOMEDS::SObject_var SO= it->Value();
    if(SO->FindAttribute(anAttr, "AttributeIOR")) {
      anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
      if(strcmp( anIOR->Value(), IOR ) == 0)
	return true;
    }
    if(SO->ReferencedObject(RefSO)) {
      if(RefSO->FindAttribute(anAttr, "AttributeIOR")) {
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	if(strcmp(anIOR->Value(), IOR) == 0)
	  return true;
      }
    }
  }
  return false;
}
