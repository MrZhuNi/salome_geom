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
//  File   : GEOMBase_Display.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "GEOMBase_Display.h"

#include "QAD_RightFrame.h"
#include "GEOM_AssemblyBuilder.h"
#include "VTKViewer_ViewFrame.h"
#include "OCCViewer_ViewFrame.h"
#include "OCCViewer_Viewer3d.h"

#include "SALOMEGUI_ImportOperation.h"
#include "SALOME_ListIteratorOfListIO.hxx"
#include <AIS_ListIteratorOfListOfInteractive.hxx>

//=======================================================================
// function : DisplayGUI()
// purpose  : Constructor
//=======================================================================
GEOMBase_Display::GEOMBase_Display() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~GEOMBase_Display()
// purpose  : Destructor
//=======================================================================
GEOMBase_Display::~GEOMBase_Display()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool GEOMBase_Display::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  switch (theCommandID)
    {
    case 211: // SHADING/WIREFRAME - MENU
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
	  QApplication::setOverrideCursor(waitCursor);

	  VTKViewer_RenderWindowInteractor* myRenderInter= ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();

	  int themode = myRenderInter->GetDisplayMode();
	  if(themode==0) {
	    myRenderInter->SetDisplayMode(1);
	    myGeomGUI->GetDesktop()->menuBar()->changeItem(211, tr("GEOM_MEN_WIREFRAME"));
	  } 
	  else {
	    myRenderInter->SetDisplayMode(0);
	    myGeomGUI->GetDesktop()->menuBar()->changeItem(211, tr("GEOM_MEN_SHADING"));
	  }

	  QApplication::restoreOverrideCursor(); 	  
	} 
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  QApplication::setOverrideCursor(Qt::waitCursor);

	  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
	  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
	  AIS_DisplayMode mode = (AIS_DisplayMode)ic->DisplayMode();
	  AIS_DisplayMode newmode = (mode == AIS_WireFrame ? AIS_Shaded : AIS_WireFrame);

	  AIS_ListOfInteractive List;
	  ic->DisplayedObjects(List);
	  AIS_ListOfInteractive List1;
	  ic->ObjectsInCollector(List1);
	  List.Append(List1);
	  
	  AIS_ListIteratorOfListOfInteractive ite(List);
	  while(ite.More()) {
	    if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	      Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
	      ic->SetDisplayMode(aSh,Standard_Integer(newmode),true);
	    }
	    ite.Next();
	  }

	  ic->SetDisplayMode(newmode, Standard_False);
	  if(newmode == 1)
	    myGeomGUI->GetDesktop()->menuBar()->changeItem(211, tr("GEOM_MEN_WIREFRAME"));
	  else
	    myGeomGUI->GetDesktop()->menuBar()->changeItem(211, tr("GEOM_MEN_SHADING"));
	  
	  QApplication::restoreOverrideCursor();
	}
	break;
      }
    case 212: // DISPLAY ALL - MENU
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
	  ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor()->DisplayAll();
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
	  this->OnDisplayAll();
	break;
      }
    case 214: // ERASE ALL - MENU
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
	  ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor()->EraseAll();
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
	  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();

	  ic->EraseAll(Standard_True, Standard_False);
	  ic->Display(v3d->getTrihedron());
	}
      	break;
      }
    case 8021: // WIREFRAME/SHADING - POPUP VIEWER
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
	  // VTK
	  VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
	  
	  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());
	  SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
	  
	  QApplication::setOverrideCursor(Qt::waitCursor);
	  for(;It.More();It.Next()) {
	    Handle(SALOME_InteractiveObject) IOS = It.Value();
	    myRenderInter->SwitchRepresentation(IOS, false);
	  }
	  myRenderInter->Render();
	  QApplication::restoreOverrideCursor();
	}
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  QApplication::setOverrideCursor(Qt::waitCursor);
	  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());
	  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
	  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
	  SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
	  for(;It.More();It.Next()) {
	    Handle(SALOME_InteractiveObject) IObject = It.Value();
	    Standard_Boolean found;
	    Handle(GEOM_AISShape) Shape = myGeomGUI->ConvertIOinGEOMAISShape(IObject, found, true);
            if(!Shape.IsNull()) {
  	      AIS_DisplayMode mode = (AIS_DisplayMode)Shape->DisplayMode();
	      if(mode == -1)
	        mode = (AIS_DisplayMode)ic->DisplayMode();

	      QApplication::setOverrideCursor(Qt::waitCursor);
	      ic->SetDisplayMode(Shape, mode == AIS_WireFrame ? AIS_Shaded : AIS_WireFrame, false);
	    }
	    QApplication::restoreOverrideCursor();
	  }
	  ic->UpdateCurrentViewer();
	}
	QApplication::restoreOverrideCursor();
	break;
      }
    case 8022 : // ERASE - POPUP VIEWER
      {
	this->OnErase();
	break;
      }
    case 8023: // DISPLAY ONLY - POPUP VIEWER
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
	  this->OnVTKDisplayOnly();
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
	  this->OnDisplayOnly();
	break;
      }
    case 9022 : // ERASE - OBJBROSER POPUP
      {
	this->OnErase();
	break;
      }
    case 9023 : // DISPLAY ONLY - OBJBROSER POPUP
      {
	if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
	  this->OnVTKDisplayOnly();
	else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
	  this->OnDisplayOnly();
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


//=====================================================================================
// function : OnDisplayAll()
// purpose  :
//=====================================================================================
void GEOMBase_Display::OnDisplayAll(bool onlyPreviousDisplayedObject)
{
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return;

  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) myContext = v3d->getAISContext();

  myContext->Display(v3d->getTrihedron());

  if(!onlyPreviousDisplayedObject) {
    AIS_ListOfInteractive List1;
    myContext->ObjectsInCollector(List1);
    AIS_ListIteratorOfListOfInteractive ite1(List1);
    while(ite1.More()) {
      if(ite1.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite1.Value());
	if(aSh->hasIO()) {
	  Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(aSh->getIO());
	  if(v3d->isInViewer(GIO, true)) {
	    myContext->Display(aSh);
	  }
	}
      }
      ite1.Next();
    }
  }
  else {
    AIS_ListIteratorOfListOfInteractive ite(myGeomGUI->GetListDisplayedObject());
    while(ite.More()) {
      if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
	if (aSh->hasIO()) {
	  Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(aSh->getIO());
	  if (v3d->isInViewer(GIO,true)) {
	    myContext->Display(aSh);
	  }
	}
      }
      ite.Next();
    }
  }
  return;
}


//=====================================================================================
// function : OnVTKDisplayOnly()
// purpose  :
//=====================================================================================
void GEOMBase_Display::OnVTKDisplayOnly()
{
  QApplication::setOverrideCursor(Qt::waitCursor);

  // Erase all not selected actors
  vtkRenderer* aren = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();
  vtkActorCollection* theActors = aren->GetActors();
  theActors->InitTraversal();
  vtkActor *ac = theActors->GetNextActor();
  while(!(ac==NULL)) {
    if(ac->IsA("SALOME_Actor")) {
      SALOME_Actor* anActor = SALOME_Actor::SafeDownCast(ac);
      if(!anActor->isHighlighted())
	anActor->VisibilityOff();
    }
    ac = theActors->GetNextActor();
  }

  // Display selection
  SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->entry());
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributeIOR_var anIOR;

  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());
  SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
	  
  for(;It.More();It.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();
    SALOMEDS::SObject_var obj = aStudy->FindObjectID( IObject->getEntry() );

    VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();

    if(myRenderInter->isInViewer(IObject)) {
      myRenderInter->Display(IObject);
    }
    else {
      // Create new actor
      if(!obj->_is_nil()) {
	if(!obj->FindAttribute(anAttr, "AttributeIOR")) 
	  break;
	// If selected object contains displayable subobjects, then do nothing
	SALOMEDS::ChildIterator_var anIter = myGeomGUI->GetActiveStudy()->getStudyDocument()->NewChildIterator(obj);
	SALOMEDS::GenericAttribute_var aTmpAttr;
	
	anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	GEOM::GEOM_Shape_var aShape = myGeom->GetIORFromString(anIOR->Value());
	if(CORBA::is_nil(aShape)) continue;
	TopoDS_Shape Shape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);

	if(obj->FindAttribute(anAttr, "AttributeName")) {
          aName = SALOMEDS::AttributeName::_narrow(anAttr);
	  // open transaction
	  QAD_Operation* op = new SALOMEGUI_ImportOperation(myGeomGUI->GetActiveStudy());
	  op->start();

	  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
	  aStudyBuilder->Addreference(newObj1, obj);
	  // commit transaction
	  op->finish();
		  
	  vtkRenderer* theRenderer = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();
	  int themode = myRenderInter->GetDisplayMode();
	  vtkActorCollection* theActors = GEOM_AssemblyBuilder::BuildActors(Shape, 0, themode, Standard_True);
	  theActors->InitTraversal();
	  vtkActor* anActor = (vtkActor*)theActors->GetNextActor();
	  while(!(anActor==NULL)) {
	    GEOM_Actor* GActor = GEOM_Actor::SafeDownCast(anActor);
	    GActor->setIO(IObject);
	    GActor->setName(IObject->getName());
		    
	    theRenderer->AddActor(GActor);
	    vtkRenderWindow *renWin
	      = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer()->GetRenderWindow();
	    renWin->Render();
	    anActor = (vtkActor*)theActors->GetNextActor();
	  }
	}
      }
    }
  }
  myGeomGUI->GetActiveStudy()->updateObjBrowser(true);

  QApplication::restoreOverrideCursor();
  return;		       
}    


//=====================================================================================
// function : OnDisplayOnly()
// purpose  :
//=====================================================================================
void GEOMBase_Display::OnDisplayOnly()
{
  if (myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return;

  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
  Handle (AIS_InteractiveContext) ic = v3d->getAISContext();

  AIS_ListOfInteractive List;
  ic->DisplayedObjects(List);
  AIS_ListIteratorOfListOfInteractive ite(List);
  while(ite.More()) {
    if(ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
      Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
      if (!ic->IsSelected( aSh )) {
	ic->Erase(aSh, Standard_True, Standard_True);
      }
    }
    ite.Next();
  }

  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  SALOME_ListIteratorOfListIO It1(Sel->StoredIObjects());
  for(;It1.More();It1.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It1.Value();

    SALOMEDS::SObject_var fatherSF = 
      aStudy->FindObjectID(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->entry());
    if(v3d->isInViewer(IObject, true)) {
      AIS_ListOfInteractive List1;
      ic->ObjectsInCollector(List1);
      AIS_ListIteratorOfListOfInteractive ite1(List1);
      while(ite1.More()) {
	if(ite1.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
	  Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite1.Value());
	  if (aSh->hasIO()) {
	    Handle(GEOM_InteractiveObject) GIO = Handle(GEOM_InteractiveObject)::DownCast(aSh->getIO());
	    if (IObject->isSame(GIO)) {
	      ic->Display(aSh);
	      ic->AddOrRemoveCurrentObject(aSh, true);
	      break;
	    }
	  }
	}
	ite1.Next();
      }
    }
    else {
      if (IObject->hasEntry()) {
	SALOMEDS::SObject_var obj = aStudy->FindObjectID(IObject->getEntry());
        SALOMEDS::GenericAttribute_var anAttr;
        SALOMEDS::AttributeName_var aName;
        SALOMEDS::AttributeIOR_var anIOR;
	if (!obj->_is_nil()) {
	  if (obj->FindAttribute(anAttr, "AttributeIOR")) {
	    // this SObject may be GEOM module root SObject

	    anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	    GEOM::GEOM_Shape_var aShape = myGeom->GetIORFromString(anIOR->Value());
	    if(CORBA::is_nil(aShape)) continue;
	    TopoDS_Shape Shape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
	    
	    if(obj->FindAttribute(anAttr, "AttributeName")) {
	      aName = SALOMEDS::AttributeName::_narrow(anAttr);
	      // open transaction
	      QAD_Operation* op = new SALOMEGUI_ImportOperation(myGeomGUI->GetActiveStudy());
	      op->start();
	      
	      SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
	      aStudyBuilder->Addreference(newObj1, obj);
	      // commit transaction
	      op->finish();
	    }
	    
	    Handle(GEOM_AISShape) aSh = new GEOM_AISShape(Shape, aName->Value());
	    Handle(GEOM_InteractiveObject) IO = new GEOM_InteractiveObject(anIOR->Value(), myGeomGUI->GetFatherior(),"GEOM");
		
	    IO->setEntry(obj->GetID());
	    aSh->setIO(IO);
	    aSh->setName(aName->Value());
	    ic->Display(aSh);
	    ic->AddOrRemoveCurrentObject(aSh, true);
	  }
	}  
      }
    }
  }
}


//=====================================================================================
// function : OnErase()
// purpose  :
//=====================================================================================
void GEOMBase_Display::OnErase()
{
  SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    // VTK
    SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
    Handle(SALOME_InteractiveObject) anIObject;
    for(;It.More();It.Next()) {
      anIObject = It.Value();
      VTKViewer_RenderWindowInteractor* myRenderInter= ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();

      if(myRenderInter->isInViewer(anIObject)) {
	myRenderInter->Erase(anIObject);
      } 
      else {
	SALOMEDS::SObject_var obj = aStudy->FindObjectID(anIObject->getEntry());
	SALOMEDS::GenericAttribute_var anAttr;
	SALOMEDS::AttributeIOR_var anIOR;
	if(!obj->_is_nil()) {
	  if(obj->FindAttribute(anAttr, "AttributeIOR")) {
	    // this SObject may be GEOM module root SObject
	    SALOMEDS::ChildIterator_var anIter = myGeomGUI->GetActiveStudy()->getStudyDocument()->NewChildIterator(obj);
	    bool useSubItems = false;
	    while(anIter->More() && !useSubItems) {
	      SALOMEDS::SObject_var subobj = anIter->Value();
	      SALOMEDS::GenericAttribute_var aTmpAttr;
	      if(subobj->FindAttribute(aTmpAttr, "AttributeIOR")) {
		anAttr = aTmpAttr;
		obj = subobj;
		useSubItems = true;
	      } 
	      else
		anIter->Next();
	    }

	    while(useSubItems?anIter->More():!anAttr->_is_nil()) { 
	      anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	      GEOM::GEOM_Shape_var aShape = myGeom->GetIORFromString(anIOR->Value());
	      TopoDS_Shape Shape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
	      if(obj->FindAttribute(anAttr, "AttributeName")) {
		// searchin for already displayed objects with the same shape
		vtkRenderer* theRenderer = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();
		vtkActorCollection* theAllActors = theRenderer->GetActors();
		theAllActors->InitTraversal();
		vtkActor* actor = (vtkActor*)theAllActors->GetNextActor();
		Handle(SALOME_InteractiveObject) anIObj;
		// don't create new study object if it already exists
		bool isDisplayed = false;
		while(!(actor==NULL)) {
		  SALOME_Actor* Gactor = SALOME_Actor::SafeDownCast(actor);
		  if(Gactor!=NULL) {
		    if(Gactor->hasIO()) {
		      if(strcmp(Gactor->getIO()->getEntry(),obj->GetID())==0) {
			isDisplayed = true;
			anIObj = Gactor->getIO();
			if(!anIObj.IsNull())
			  myRenderInter->Erase(anIObj);
		      }
		    }
		  }
		  actor=(vtkActor*)(theAllActors->GetNextActor());
		}
	      }
	      if(useSubItems) {
		anIter->Next();
		anAttr=SALOMEDS::GenericAttribute::_nil();
		while(anIter->More() && anAttr->_is_nil()) {
		  SALOMEDS::SObject_var subobject = anIter->Value();
		  SALOMEDS::GenericAttribute_var aTmpAttribute;
		  if(subobject->FindAttribute(aTmpAttribute, "AttributeIOR")) {
		    anAttr = aTmpAttribute;
		    obj = subobject;
		  }
		  else
		    anIter->Next();
		}
	      } 
	      else
		anAttr = SALOMEDS::GenericAttribute::_nil();
	    } 
	  }
	}
      }
    }
  }
  else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    // OCC
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
    SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
    Handle(SALOME_InteractiveObject) anIObject;
    for(;It.More();It.Next()) {
      anIObject = It.Value();
      if(v3d->isInViewer(anIObject, true)) {
	Standard_Boolean found;
	Handle(GEOM_AISShape) aSh = myGeomGUI->ConvertIOinGEOMAISShape(anIObject, found, true);
	if(found) {
	  ic->Erase(aSh);
	  ic->AddOrRemoveCurrentObject(aSh, true);
	}
      } 
      else {
	SALOMEDS::SObject_var obj = aStudy->FindObjectID(anIObject->getEntry());
	SALOMEDS::GenericAttribute_var anAttr;
	SALOMEDS::AttributeIOR_var anIOR;
	if(!obj->_is_nil()) {
	  if(obj->FindAttribute(anAttr, "AttributeIOR")) {
	    // this SObject may be GEOM module root SObject
	    SALOMEDS::ChildIterator_var anIter = myGeomGUI->GetActiveStudy()->getStudyDocument()->NewChildIterator(obj);
	    bool useSubItems = false;
	    while(anIter->More() && !useSubItems) {
	      SALOMEDS::SObject_var subobj = anIter->Value();
	      SALOMEDS::GenericAttribute_var aTmpAttr;
	      if(subobj->FindAttribute(aTmpAttr, "AttributeIOR")) {
		anAttr = aTmpAttr;
		obj = subobj;
		useSubItems = true;
	      } 
	      else
		anIter->Next();
	    }

	    while(useSubItems?anIter->More():!anAttr->_is_nil()) { 
	      anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	      GEOM::GEOM_Shape_var aShape = myGeom->GetIORFromString(anIOR->Value());
	      TopoDS_Shape Shape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
	      if(obj->FindAttribute(anAttr, "AttributeName")) {
		// searchin for already displayed objects with the same shape
		AIS_ListOfInteractive aDisplayed;
		ic->DisplayedObjects(aDisplayed);
		AIS_ListIteratorOfListOfInteractive anIObjects(aDisplayed);
		Handle(AIS_Shape) anAISShape;
		for(;anIObjects.More();anIObjects.Next()) {
		  anAISShape = Handle(AIS_Shape)::DownCast(anIObjects.Value());
		  if(!anAISShape.IsNull()) {
		    if(anAISShape->Shape().IsSame(Shape))
		      break;
		    anAISShape.Nullify();
		  }
		}
		if(!anAISShape.IsNull()) {
		  if(ic->IsDisplayed(anAISShape)) 
		    ic->Erase(anAISShape);
		}
	      }
	      if(useSubItems) {
		anIter->Next();
		anAttr=SALOMEDS::GenericAttribute::_nil();
		while(anIter->More() && anAttr->_is_nil()) {
		  SALOMEDS::SObject_var subobject = anIter->Value();
		  SALOMEDS::GenericAttribute_var aTmpAttribute;
		  if(subobject->FindAttribute(aTmpAttribute, "AttributeIOR")) {
		    anAttr = aTmpAttribute;
		    obj = subobject;
		  }
		  else
		    anIter->Next();
		}
	      }
	      else
		anAttr = SALOMEDS::GenericAttribute::_nil();
	    }
	  }
	}
      }
    }
  }
  Sel->ClearIObjects();
}
