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
//  File   : GeometryGUI.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GeometryGUI.h"

// Open CASCADE Includes
#include <Prs3d_Drawer.hxx>
#include <Prs3d_IsoAspect.hxx>

// SALOME Includes
#include "QAD_RightFrame.h"
#include "QAD_Config.h"

#include "OCCViewer_Viewer3d.h"
#include "OCCViewer_ViewPort3d.h"
#include "VTKViewer_ViewFrame.h"

#include "SALOME_Selection.h"

// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

/* The object itself created in the static method 'GetOrCreateGEOMBase()' */
static GEOMContext* GeomGUI = 0;

typedef bool OGE(int, QAD_Desktop*);
typedef bool OMP(QMouseEvent*, QAD_Desktop*, QAD_StudyFrame*);
typedef void BP(const Handle(SALOME_InteractiveObject)&);
typedef bool CP(QAD_Desktop*, QPopupMenu*, const QString&,
		const QString&, const QString&);

//=======================================================================
// function : GeometryGUI()
// purpose  : Constructor
//=======================================================================
GeometryGUI::GeometryGUI() :
  QObject()
{
}


//=======================================================================
// function : ~GeometryGUI()
// purpose  : Destructor
//=======================================================================
GeometryGUI::~GeometryGUI()
{
}


//=======================================================================
// function : GetOrCreateGEOMBase()
// purpose  : Gets or create an object 'GEOMBase' with initialisations
//          : Returns 'GeomGUI' as a pointer
//=======================================================================
GEOMContext* GeometryGUI::GetOrCreateGeometryGUI(QAD_Desktop* desktop)
{
  GeomGUI = GeomGUI->GetOrCreateGeomGUI(desktop);
  return GeomGUI;
}


//=======================================================================
// function : OnGUIEvent() [static]
// purpose  : manage all events on GUI
//=======================================================================
bool GeometryGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  QMenuBar* Mb = QAD_Application::getDesktop()->getMainMenuBar();
  bool ViewOCC;

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
    ViewOCC = true;    
  else if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
    ViewOCC = false;
  else
    return false;

  Mb->setItemEnabled(404, ViewOCC);//SKETCHER
  Mb->setItemEnabled(406, ViewOCC);//SKETCHER

  Mb->setItemEnabled(603, ViewOCC);//SuppressFace
  Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
  Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
  Mb->setItemEnabled(414, ViewOCC);// Isos Settings

  if(theCommandID == 4041 || // SKETCHER - POPUP VIEWER - SEGMENT
     theCommandID == 4042 || // SKETCHER - POPUP VIEWER - ARC
     theCommandID == 4043 || // SKETCHER - POPUP VIEWER - SET ANGLE
     theCommandID == 4044 || // SKETCHER - POPUP VIEWER - SET X
     theCommandID == 4045 || // SKETCHER - POPUP VIEWER - SET Y
     theCommandID == 4046 || // SKETCHER - POPUP VIEWER - DELETE
     theCommandID == 4047 || // SKETCHER - POPUP VIEWER - END
     theCommandID == 4048 || // SKETCHER - POPUP VIEWER - CLOSE
     theCommandID == 4051 || // SKETCHER - MENU - SET PLANE
     theCommandID == 4052 || // SKETCHER - MENU - TANGENT
     theCommandID == 4053 || // SKETCHER - MENU - PERPENDICULAR
     theCommandID == 4061 || // SKETCHER - MENU - LENGTH
     theCommandID == 4062 || // SKETCHER - MENU - ANGLE
     theCommandID == 4063 || // SKETCHER - MENU - RADIUS
     theCommandID == 4064 || // SKETCHER - MENU - X
     theCommandID == 4065) {  // SKETCHER - MENU - Y
    if(!GeomGUI->LoadLibrary("libEntityGUI.so")) 
      return false;
  }
  else {
    if(GeomGUI->myState == 2) {
      Mb->setItemEnabled(405, false);//SKETCHER
      GeomGUI->GetSketcher().Clear();
      GeomGUI->myState = -1;
    }

    if(theCommandID == 111 ||  // MENU FILE - IMPORT BREP
       theCommandID == 112 ||  // MENU FILE - IMPORT IGES
       theCommandID == 113 ||  // MENU FILE - IMPORT STEP
       theCommandID == 121 ||  // MENU FILE - EXPORT BREP
       theCommandID == 122 ||  // MENU FILE - EXPORT IGES
       theCommandID == 123 ||  // MENU FILE - EXPORT STEP
       theCommandID == 31 ||   // MENU EDIT - COPY
       theCommandID == 33 ||   // MENU EDIT - DELETE
       theCommandID == 411 ||  // MENU SETTINGS - ADD IN STUDY
       theCommandID == 412 ||  // MENU SETTINGS - SHADING COLOR
       theCommandID == 413 ||  // MENU SETTINGS - ISOS
       theCommandID == 414 ||  // MENU SETTINGS - STEP VALUE FOR SPIN BOXES
       theCommandID == 5103 || // MENU TOOLS - CHECK GEOMETRY
       theCommandID == 5104 || // MENU TOOLS - LOAD SCRIPT
       theCommandID == 8032 || // POPUP VIEWER - COLOR
       theCommandID == 8033 || // POPUP VIEWER - TRANSPARENCY
       theCommandID == 8034 || // POPUP VIEWER - ISOS
       theCommandID == 804 ||  // POPUP VIEWER - ADD IN STUDY
       theCommandID == 901 ||  // OBJECT BROWSER - RENAME
       theCommandID == 9024) { // OBJECT BROWSER - OPEN
      if(!GeomGUI->LoadLibrary("libGEOMToolsGUI.so")) 
	return false;
    }
    else if(theCommandID == 211 ||  // MENU VIEW - WIREFRAME/SHADING
	    theCommandID == 212 ||  // MENU VIEW - DISPLAY ALL
	    theCommandID == 213 ||  // MENU VIEW - DISPLAY ONLY
	    theCommandID == 214 ||  // MENU VIEW - ERASE ALL
	    theCommandID == 215 ||  // MENU VIEW - ERASE ONLY
	    theCommandID == 8031) { // POPUP VIEWER - WIREFRAME/SHADING
      if(!GeomGUI->LoadLibrary("libDisplayGUI.so")) 
	return false;
    }
    else if(theCommandID == 4011 || // MENU BASIC - POINT
	    theCommandID == 4012 || // MENU BASIC - LINE
	    theCommandID == 4013 || // MENU BASIC - CIRCLE
	    theCommandID == 4014 || // MENU BASIC - ELLIPSE
	    theCommandID == 4015 || // MENU BASIC - ARC
	    theCommandID == 4016 || // MENU BASIC - VECTOR
	    theCommandID == 4017 || // MENU BASIC - PLANE
	    theCommandID == 4018) { // MENU BASIC - WPLANE
      if(!GeomGUI->LoadLibrary("libBasicGUI.so")) 
	return false;
    }
    else if(theCommandID == 4021 || // MENU PRIMITIVE - BOX
	    theCommandID == 4022 || // MENU PRIMITIVE - CYLINDER
	    theCommandID == 4023 || // MENU PRIMITIVE - SPHERE
	    theCommandID == 4024 || // MENU PRIMITIVE - TORUS
	    theCommandID == 4025) { // MENU PRIMITIVE - CONE
      if(!GeomGUI->LoadLibrary("libPrimitiveGUI.so"))
	return false;
    }
    else if(theCommandID == 4031 || // MENU GENERATION - PRISM
	    theCommandID == 4032 || // MENU GENERATION - REVOLUTION
	    theCommandID == 4033 || // MENU GENERATION - FILLING
	    theCommandID == 4034) { // MENU GENERATION - PIPE
      if(!GeomGUI->LoadLibrary("libGenerationGUI.so")) 
	return false;
    }
    else if(theCommandID == 404 ||  // SKETCHER
	    theCommandID == 407) {  // MENU ENTITY - EXPLODE
      if(!GeomGUI->LoadLibrary("libEntityGUI.so")) 
	return false;
    }
    else if(theCommandID == 4081 || // MENU BUILD - EDGE
	    theCommandID == 4082 || // MENU BUILD - WIRE
	    theCommandID == 4083 || // MENU BUILD - FACE
	    theCommandID == 4084 || // MENU BUILD - SHELL
	    theCommandID == 4085 || // MENU BUILD - SOLID
	    theCommandID == 4086) { // MENU BUILD - COMPUND
      if(!GeomGUI->LoadLibrary("libBuildGUI.so")) 
	return false;
    }
    else if(theCommandID == 5011 || // MENU BOOLEAN - FUSE
	    theCommandID == 5012 || // MENU BOOLEAN - COMMON
	    theCommandID == 5013 || // MENU BOOLEAN - CUT
	    theCommandID == 5014) { // MENU BOOLEAN - SECTION
      if(!GeomGUI->LoadLibrary("libBooleanGUI.so")) 
	return false;
    }
    else if(theCommandID == 5021 || // MENU TRANSFORMATION - TRANSLATION
	    theCommandID == 5022 || // MENU TRANSFORMATION - ROTATION
	    theCommandID == 5023 || // MENU TRANSFORMATION - POSITION
	    theCommandID == 5024 || // MENU TRANSFORMATION - MIRROR
	    theCommandID == 5025 || // MENU TRANSFORMATION - SCALE
	    theCommandID == 5026 || // MENU TRANSFORMATION - MULTI-TRANSLATION
	    theCommandID == 5027) { // MENU TRANSFORMATION - MULTI-ROTATION
      if(!GeomGUI->LoadLibrary("libTransformationGUI.so")) 
	return false;
    }
    else if(theCommandID == 503 ||  // MENU OPERATION - PARTITION
	    theCommandID == 504 ||  // MENU OPERATION - ARCHIMEDE
	    theCommandID == 505 ||  // MENU OPERATION - FILLET
	    theCommandID == 506) {  // MENU OPERATION - CHAMFER
      if(!GeomGUI->LoadLibrary("libOperationGUI.so")) 
	return false;
    }
    else if(theCommandID == 601 ||  // MENU REPAIR - SEWING
	    theCommandID == 602 ||  // MENU REPAIR - ORIENTATION
	    theCommandID == 603 ||  // MENU REPAIR - SUPPRESS FACES
	    theCommandID == 604) {  // MENU REPAIR - SUPPRESS HOLE
      if(!GeomGUI->LoadLibrary("libRepairGUI.so")) 
	return false;
    }
    else if(theCommandID == 611 ||  // MENU KINEMATIC - ASSEMBLY
	    theCommandID == 61201 ||  // MENU KINEMATIC - EMBEDDING
	    theCommandID == 61202 ||  // MENU KINEMATIC - PIVOT
	    theCommandID == 61203 ||  // MENU KINEMATIC - SLIDE
	    theCommandID == 61204 ||  // MENU KINEMATIC - SLIDING PIVOT
	    theCommandID == 61205 ||  // MENU KINEMATIC - SPHERICAL
	    theCommandID == 61206 ||  // MENU KINEMATIC - PLANE
	    theCommandID == 61207 ||  // MENU KINEMATIC - ANNULAR
	    theCommandID == 61208 ||  // MENU KINEMATIC - RECTILINEAR
	    theCommandID == 61209 ||  // MENU KINEMATIC - PONCTUAL
	    theCommandID == 61210 ||  // MENU KINEMATIC - HELICOIDAL
	    theCommandID == 6131 ||  // MENU KINEMATIC - POSITION
	    theCommandID == 6132 ||  // MENU KINEMATIC - ROTATION
	    theCommandID == 6133 ||  // MENU KINEMATIC - TRANSLATION
	    theCommandID == 614 ||  // MENU KINEMATIC - ANIMATION
	    theCommandID == 6141) {  // MENU KINEMATIC - RUN ANIMATION
      if(!GeomGUI->LoadLibrary("libKinematicGUI.so")) 
	return false;
    }
    else if(theCommandID == 701 ||  // MENU MEASURE - PROPERTIES
	    theCommandID == 702 ||  // MENU MEASURE - CDG
	    theCommandID == 703 ||  // MENU MEASURE - INERTIA
	    theCommandID == 7041 || // MENU MEASURE - BOUNDING BOX
	    theCommandID == 7042 || // MENU MEASURE - MIN DISTANCE
	    theCommandID == 705 ||  // MENU MEASURE - TOLERANCE
	    theCommandID == 706 ||  // MENU MEASURE - WHATIS
	    theCommandID == 707) {  // MENU MEASURE - CHECK
      if(!GeomGUI->LoadLibrary("libMeasureGUI.so")) 
	return false;
    }
    else
      return false;
  }
  
  //Load Function OnGUIEvent
  OSD_Function osdF = GeomGUI->myGUILibrary.DlSymb("OnGUIEvent");
  OGE (*f1) = NULL;
  if(osdF != NULL) {
    f1 = (bool (*) (int, QAD_Desktop*)) osdF;
    (*f1)(theCommandID, parent);
  }
  else
    return false;

  return true;
}


//=================================================================================
// function : OnKeyPress()
// purpose  : [static]
//=================================================================================
bool GeometryGUI::OnKeyPress(QKeyEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;
  else
  return true;
}


//=================================================================================
// function : OnMouseMove()
// purpose  : [static] manage mouse events
//=================================================================================
bool GeometryGUI::OnMouseMove(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  if(GeomGUI->myState == 2) {
    OCCViewer_ViewPort* vp = ((OCCViewer_ViewFrame*)studyFrame->getRightFrame()->getViewFrame())->getViewPort();
    GeomGUI->GetSketcher().MakeCurrentEdge(pe->x(), pe->y(), ((OCCViewer_ViewPort3d*)vp)->getView());
  }

  return true;
}


//=================================================================================
// function : 0nMousePress()
// purpose  : [static] manage mouse events
//=================================================================================
bool GeometryGUI::OnMousePress(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  if(GeomGUI->myState == 2) {
    GeomGUI->GetSketcher().ValidateEdge();
    if(GeomGUI->GetSketcher().GetmyEdgesNumber() == 1) {
      QMenuBar* Mb = QAD_Application::getDesktop()->getMainMenuBar();
      Mb->setItemEnabled(405, true);  // SKETCH CONTRAINTS
    }
  }
  else if(GeomGUI->myState == 0) {
    if(!GeomGUI->LoadLibrary("libBasicGUI.so")) 
      return false;

    //Load Function OnMousePress
    OSD_Function osdF = GeomGUI->myGUILibrary.DlSymb("OnMousePress");
    OMP (*f1) = NULL;
    if(osdF != NULL) {
      f1 = (bool (*) (QMouseEvent*, QAD_Desktop*, QAD_StudyFrame*)) osdF;
      (*f1)(pe, parent, studyFrame);
    }
  }

  return false;
}


//=================================================================================
// function : SetSettings()
// purpose  : [static]
//=================================================================================
bool GeometryGUI::SetSettings(QAD_Desktop* parent)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);
  QMenuBar* Mb = QAD_Application::getDesktop()->getMainMenuBar();
  QAD_Study* ActiveStudy = parent->getActiveStudy();

  /* Wireframe or Shading */
  int DisplayMode = 0;
  bool ViewOCC = false;
  if(ActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)ActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
    DisplayMode = ic->DisplayMode();
    ViewOCC = true;
  }
  else if(ActiveStudy->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)ActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
    DisplayMode = myRenderInter->GetDisplayMode();
  }

  if(DisplayMode == 1)
    Mb->changeItem(211, tr("GEOM_MEN_WIREFRAME"));
  else
    Mb->changeItem(211, tr("GEOM_MEN_SHADING"));


  /* Add in Study */
  QString AddInStudy = QAD_CONFIG->getSetting("Geometry:SettingsAddInStudy");
  int Settings_AddInStudy;
  if(!AddInStudy.isEmpty())
    Settings_AddInStudy = AddInStudy.toInt();
  else
    Settings_AddInStudy = 1;
  Mb->setItemChecked(411, Settings_AddInStudy);


  /* step value */
  QString S = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  if(S.isEmpty())
    QAD_CONFIG->addSetting("Geometry:SettingsGeomStep", "100");


  /* isos */
  int count = ActiveStudy->getStudyFramesCount();
  for(int i = 0; i < count; i++) {
    if(ActiveStudy->getStudyFrame(i)->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)ActiveStudy->getStudyFrame(i)->getRightFrame()->getViewFrame())->getViewer();
      Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
      
      QString IsoU = QAD_CONFIG->getSetting("Geometry:SettingsIsoU");
      QString IsoV = QAD_CONFIG->getSetting("Geometry:SettingsIsoV");
      if(!IsoU.isEmpty())
	ic->DefaultDrawer()->UIsoAspect()->SetNumber(IsoU.toInt());
      if(!IsoV.isEmpty())
	ic->DefaultDrawer()->VIsoAspect()->SetNumber(IsoV.toInt());
    }
  }

  Mb->setItemEnabled(404, ViewOCC);//SKETCHER
  Mb->setItemEnabled(405, false);//SKETCHER
  Mb->setItemEnabled(406, ViewOCC);//SKETCHER

  Mb->setItemEnabled(603, ViewOCC);//SuppressFace
  Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
  Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
  Mb->setItemEnabled(414, ViewOCC);// Isos Settings

  return true;
}


//=================================================================================
// function : DefinePopup()
// purpose  : [static]
//=================================================================================
void GeometryGUI::DefinePopup(QString & theContext, QString & theParent, QString & theObject)
{
  GeometryGUI::GetOrCreateGeometryGUI(QAD_Application::getDesktop());

  QAD_Study* ActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection(ActiveStudy->getSelection());
  theObject = "";
  theContext = "";


  if((theParent.compare("Viewer") == 0)) {
    if(GeomGUI->myState == 2) 
      theContext = "Sketch";
    else if(Sel->IObjectCount() == 0)
      theContext = "NothingSelected";
  }

  if(Sel->IObjectCount() == 1) {
    Handle(SALOME_InteractiveObject) IO = Sel->firstIObject();
    if(IO->hasEntry()) {
      SALOMEDS::SObject_var sobj = ActiveStudy->getStudyDocument()->FindObjectID(IO->getEntry());
      if(!sobj->_is_nil()) {
	SALOMEDS::SComponent_var scomp = sobj->GetFatherComponent();
	if(strcmp(scomp->GetID(), IO->getEntry()) == 0) {
	  // component is selected
	  theObject = "Component";
	  return;
	}

	SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
	SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
	SALOMEDS::GenericAttribute_var anAttr;
	SALOMEDS::AttributeComment_var aType;
	anAttr = aStudyBuilder->FindOrCreateAttribute(sobj, "AttributeComment");
	aType = SALOMEDS::AttributeComment::_narrow(anAttr);
	QString val = QString(strdup(aType->Value()));
	if(val == "Kinematic_Assembly")
	  theObject = "Assembly";
	else if(val == "Kinematic_Contact")
	  theObject = "Contact";
	else if(val == "Kinematic_Animation")
	  theObject = "Animation";
	return;
      }
    }
  }

  return;
}


//=================================================================================
// function : CustomPopup()
// purpose  : [static]
//=================================================================================
bool GeometryGUI::CustomPopup(QAD_Desktop* parent, QPopupMenu* popup, const QString& theContext,
			      const QString& theParent, const QString& theObject)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    if(theParent.compare("Viewer") == 0) {
      if(theContext.compare("Sketch") == 0) {
	SketchStatus myCS = GeomGUI->GetSketcher().GetCurrentStatus();
	popup->setCheckable(TRUE);
	if(myCS == SEGMENT) {
	  popup->setItemChecked(4041, true);  //Sketch Segment Menu
	  popup->setItemChecked(4042, false); //Sketch Arc Menu
	}
	else if(myCS == ARC_CHORD) {
	  popup->setItemChecked(4041, false); //Sketch Segment Menu
	  popup->setItemChecked(4042, true);  //Sketch Arc Menu
	}   
	return true;
      }
    }
  }

  if(!GeomGUI->LoadLibrary("libGEOMBase.so")) 
    return false;

  //Load Function BuildPresentation
  OSD_Function osdF = GeomGUI->myGUILibrary.DlSymb("CustomPopup");
  CP (*f1) = NULL;
  if(osdF != NULL) {
    f1 = (bool (*) (QAD_Desktop*, QPopupMenu*, const QString&,
		    const QString&, const QString&)) osdF;
    (*f1)(parent, popup, theContext, theParent, theObject);
  }
  else
    return false;

  return true;
}


//=================================================================================
// function : activeStudyChanged()
// purpose  : static
//=================================================================================
void GeometryGUI::activeStudyChanged(QAD_Desktop* parent)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent); 

  if(GeomGUI != 0) {
    QMenuBar* Mb = QAD_Application::getDesktop()->getMainMenuBar();
    if(GeomGUI->myState == 2) {
      Mb->setItemEnabled(405, false);//SKETCHER
      GeomGUI->GetSketcher().Clear();
      GeomGUI->myState = -1;
    }

    bool ViewOCC = false;
    if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) 
      ViewOCC = true;

    Mb->setItemEnabled(404, ViewOCC);//SKETCHER
    Mb->setItemEnabled(406, ViewOCC);//SKETCHER

    Mb->setItemEnabled(603, ViewOCC);//SuppressFace
    Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
    Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
    Mb->setItemEnabled(414, ViewOCC);// Isos Settings

    GeomGUI->EmitSignalCloseAllDialogs();
    GeomGUI = 0;
  }
  return;
}


//=======================================================================
// function : BuildPresentation()
// purpose  : static
//=======================================================================
void GeometryGUI::BuildPresentation(const Handle(SALOME_InteractiveObject)& theIO)
{
  GeometryGUI::GetOrCreateGeometryGUI(QAD_Application::getDesktop());

  if(!GeomGUI->LoadLibrary("libDisplayGUI.so")) 
    return;

  //Load Function BuildPresentation
  OSD_Function osdF = GeomGUI->myGUILibrary.DlSymb("BuildPresentation");
  BP (*f1) = NULL;
  if(osdF != NULL) {
    f1 = (void (*) (const Handle(SALOME_InteractiveObject)&)) osdF;
    (*f1)(theIO);
  }
  return;
}


//=================================================================================
// EXPORTED METHODS
//=================================================================================
extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {return GeometryGUI::OnGUIEvent(theCommandID, parent);}

  bool OnKeyPress(QKeyEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {return GeometryGUI::OnKeyPress(pe, parent, studyFrame);}

  bool OnMousePress(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {return GeometryGUI::OnMousePress(pe, parent, studyFrame);}

  bool OnMouseMove(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
  {return GeometryGUI::OnMouseMove(pe, parent, studyFrame);}

  bool SetSettings(QAD_Desktop* parent)
  {return GeometryGUI::SetSettings(parent);}

  bool customPopup(QAD_Desktop* parent, QPopupMenu* popup, const QString & theContext,
		     const QString & theParent, const QString & theObject)
  {return GeometryGUI::CustomPopup(parent, popup, theContext, theParent, theObject);}

  void definePopup(QString & theContext, QString & parent, QString & theObject)
  {GeometryGUI::DefinePopup(theContext, parent, theObject);}
  
  bool activeStudyChanged(QAD_Desktop* parent)
  {GeometryGUI::activeStudyChanged(parent);}

  void buildPresentation(const Handle(SALOME_InteractiveObject)& theIO)
  {GeometryGUI::BuildPresentation(theIO);}

  void supportedViewType(int* buffer, int bufferSize)
  {
    if(!buffer || !bufferSize) return;
    buffer[0] = (int)VIEW_OCC;
    if (--bufferSize) buffer[1] = (int)VIEW_VTK;
  }
}
