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
#include "QAD_Resource.h"
#include "QAD_Config.h"

#include "OCCViewer_ViewPort.h"
#include "OCCViewer_ViewPort3d.h"
#include "OCCViewer_Viewer3d.h"
#include "VTKViewer_ViewFrame.h"

#include "SALOME_Selection.h"
#include "SALOME_ListIteratorOfListIO.hxx"

/* The object itself created in the static method 'GetOrCreateGEOMBase()' */
static GEOMContext* GeomGUI = 0;

typedef bool OGE(int, QAD_Desktop*);
typedef bool OMP(QMouseEvent*, QAD_Desktop*, QAD_StudyFrame*);
typedef void BP(const Handle(SALOME_InteractiveObject)&);

//=================================================================================
// class   : CustomItem
// purpose : Set Font to a text.
//=================================================================================
class CustomItem : public QCustomMenuItem
{
public:
    CustomItem(const QString& s, const QFont& f)
      :string(s), font(f){};
    ~CustomItem(){}
  
    void paint(QPainter* p, const QColorGroup& /*cg*/, bool /*act*/,
	       bool /*enabled*/, int x, int y, int w, int h)
    {
      p->setFont(font);
      p->drawText(x, y, w, h, AlignHCenter | AlignVCenter | ShowPrefix | DontClip, string);
    }

    QSize sizeHint()
    {
      return QFontMetrics(font).size(AlignHCenter | AlignVCenter | ShowPrefix | DontClip, string);
    }

private:
  QString string;
  QFont font;

};


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

  QMenuBar* Mb = GeomGUI->GetDesktop()->getMainMenuBar();
  bool ViewOCC;

  if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC)
    ViewOCC = true;    
  else if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK)
    ViewOCC = false;
  else
    return false;

  Mb->setItemEnabled(404, ViewOCC);//Sketch
  Mb->setItemEnabled(405, ViewOCC);//Sketch
  Mb->setItemEnabled(406, ViewOCC);//Sketch

  Mb->setItemEnabled(603, ViewOCC);//SuppressFace
  Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
  Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
  Mb->setItemEnabled(414, ViewOCC);// Isos Settings
  
  if (GeomGUI->GetState() == CURRENT_SKETCH && theCommandID != 404 && theCommandID != 4041 && theCommandID != 4042 && theCommandID != 4043 && theCommandID != 4044 && theCommandID != 4045 && theCommandID != 4046 && theCommandID != 4047 && theCommandID != 4048 && theCommandID != 4051 && theCommandID != 4052 && theCommandID != 4053 && theCommandID != 4061 && theCommandID != 4062 && theCommandID != 4063 && theCommandID != 4064 && theCommandID != 4065)
    return false;

  if(theCommandID == 111 ||  // MENU FILE - IMPORT BREP
     theCommandID == 112 ||  // MENU FILE - IMPORT IGES
     theCommandID == 113 ||  // MENU FILE - IMPORT STEP
     theCommandID == 121 ||  // MENU FILE - EXPORT BREP
     theCommandID == 122 ||  // MENU FILE - EXPORT IGES
     theCommandID == 123 ||  // MENU FILE - EXPORT STEP
     theCommandID == 31 ||   // MENU EDIT - COPY
     theCommandID == 33 ||   // MENU EDIT - DELETE
     theCommandID == 411 ||  // MENU SETTINGS - COPY
     theCommandID == 412 ||  // MENU SETTINGS - ADD IN STUDY
     theCommandID == 413 ||  // MENU SETTINGS - SHADING COLOR
     theCommandID == 414 ||  // MENU SETTINGS - ISOS
     theCommandID == 415 ||  // MENU SETTINGS - STEP VALUE FOR SPIN BOXES
     theCommandID == 5103 || // MENU TOOLS - CHECK GEOMETRY
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
  else if(theCommandID == 404 ||  // SKETCHER
	  theCommandID == 4041 || // SKETCHER - POPUP VIEWER - SEGMENT
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
	  theCommandID == 4065) { // SKETCHER - MENU - Y
    //     GEOMBase_Sketcher* mySketcherGUI = new GEOMBase_Sketcher();
    //     mySketcherGUI->OnGUIEvent(theCommandID, parent);
    return true;
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
  else if(theCommandID == 407 ||  // MENU BUILD - EXPLODE
	  theCommandID == 4081 || // MENU BUILD - EDGE
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
	  theCommandID == 5023 || // MENU TRANSFORMATION - MIRROR
	  theCommandID == 5024 || // MENU TRANSFORMATION - SCALE
	  theCommandID == 5025 || // MENU TRANSFORMATION - MULTI-TRANSLATION
	  theCommandID == 5026) { // MENU TRANSFORMATION - MULTI-ROTATION
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

  if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;
  return true;
}


//=================================================================================
// function : 0nMousePress()
// purpose  : [static] manage mouse events
//=================================================================================
bool GeometryGUI::OnMousePress(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  if(GeomGUI->GetState() == CURRENT_SKETCH) {
    GeomGUI->GetSketcher().ValidateEdge();
    if(GeomGUI->GetSketcher().GetmyEdgesNumber() == 1) {
      QMenuBar* Mb = GeomGUI->GetDesktop()->getMainMenuBar();
      QMenuData* pp;
      QMenuItem* item = Mb->findItem(405, &pp);
      pp->setItemEnabled(405, true);  // SKETCH CONTRAINTS
    }
  }
  else if(GeomGUI->GetState() == POINT_METHOD) {
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
// function : OnMouseMove()
// purpose  : [static] manage mouse events
//=================================================================================
bool GeometryGUI::OnMouseMove(QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
  OCCViewer_ViewPort* vp = ((OCCViewer_ViewFrame*)studyFrame->getRightFrame()->getViewFrame())->getViewPort();

  if(GeomGUI->GetState() == CURRENT_SKETCH)
    GeomGUI->GetSketcher().MakeCurrentEdge(pe->x(), pe->y(), ((OCCViewer_ViewPort3d*)vp)->getView());

  return true;
}


//=================================================================================
// function : SetSettings()
// purpose  : [static]
//=================================================================================
bool GeometryGUI::SetSettings(QAD_Desktop* parent)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  /* Shading Color */
  QString SCr = QAD_CONFIG->getSetting("Geometry:SettingsShadingColorRed");
  QString SCg = QAD_CONFIG->getSetting("Geometry:SettingsShadingColorGreen");
  QString SCb = QAD_CONFIG->getSetting("Geometry:SettingsShadingColorBlue");
  if(!SCr.isEmpty() && !SCg.isEmpty() && !SCb.isEmpty())   
    GeomGUI->GetShadingColor() = Quantity_Color (SCr.toInt()/255., SCg.toInt()/255., SCb.toInt()/255., Quantity_TOC_RGB);

  /* Wireframe or Shading */
  int DisplayMode = 0;
  if(parent->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)parent->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
    DisplayMode = ic->DisplayMode();
  }
  else if(parent->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)parent->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
    DisplayMode = myRenderInter->GetDisplayMode();
  }

  if(DisplayMode == 1)
    parent->menuBar()->changeItem(211, tr("GEOM_MEN_WIREFRAME"));
  else
    parent->menuBar()->changeItem(211, tr("GEOM_MEN_SHADING"));

  QMenuData* pp;
  if(parent->menuBar()->findItem(411, &pp))
    pp->removeItem(411);

  /* Add in Study */
  QString AddInStudy = QAD_CONFIG->getSetting("Geometry:SettingsAddInStudy");
  if(!AddInStudy.isEmpty())
    GeomGUI->GetSettings_AddInStudy() = AddInStudy.toInt();
  else
    GeomGUI->GetSettings_AddInStudy() = 1;
  parent->menuBar()->findItem(412, &pp);
  pp->setItemChecked(412, GeomGUI->GetSettings_AddInStudy());

  /* step value */
  QString S = QAD_CONFIG->getSetting("Geometry:SettingsGeomStep");
  if(S.isEmpty())
    QAD_CONFIG->addSetting("Geometry:SettingsGeomStep", "100");
  
  /* isos */
  QAD_Study* ActiveStudy = parent->getActiveStudy();
  int count = ActiveStudy->getStudyFramesCount();

  bool ViewOCC = false;
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

      ViewOCC = true;
    }
  }

  QMenuBar* Mb = GeomGUI->GetDesktop()->getMainMenuBar();

  Mb->setItemEnabled(404, ViewOCC);//Sketch
  Mb->setItemEnabled(405, ViewOCC);//Sketch
  Mb->setItemEnabled(406, ViewOCC);//Sketch

  Mb->setItemEnabled(603, ViewOCC);//SuppressFace
  Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
  Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
  Mb->setItemEnabled(414, ViewOCC);// Isos Settings

  return true;
}

  SALOME_Selection* Sel = SALOME_Selection::Selection(GeomGUI->GetActiveStudy()->getSelection());
  theObject = "";

//=================================================================================
// function : DefinePopup()
// purpose  : [static]
//=================================================================================
void GeometryGUI::DefinePopup(QString & theContext, QString & theParent, QString & theObject)
{
  GeometryGUI::GetOrCreateGeometryGUI(QAD_Application::getDesktop());

  SALOME_Selection* Sel = SALOME_Selection::Selection(GeomGUI->GetActiveStudy()->getSelection());
  theObject = "";

  if(Sel->IObjectCount() == 1) {
    Handle(SALOME_InteractiveObject) IO = Sel->firstIObject();
    if(IO->hasEntry()) {
      SALOMEDS::SObject_var sobj = GeomGUI->GetActiveStudy()->getStudyDocument()->FindObjectID(IO->getEntry());
      if(!sobj->_is_nil()) {
	SALOMEDS::SComponent_var scomp = sobj->GetFatherComponent();
	if(strcmp(scomp->GetID(), IO->getEntry()) == 0) {
	  // component is selected
	  theObject = "Component";
	}
      }
    }
  }

  if((theParent.compare("Viewer")==0)) {
    if(GeomGUI->GetState() == CURRENT_SKETCH) 
      theContext = "Sketch";
    else {
      if(Sel->IObjectCount() > 0)
	theContext = "";
      else
	theContext = "NothingSelected";
    }
  } 
  else 
    theContext = "";

  return;
}

  if((theParent.compare("Viewer")==0)) {
    if(GeomGUI->GetState() == CURRENT_SKETCH) 
      theContext = "Sketch";
    else {
      if(Sel->IObjectCount() > 0)
	theContext = "";
      else
	theContext = "NothingSelected";
    }
  } 
  else 
    theContext = "";

//=================================================================================
// function : CustomPopup()
// purpose  : [static]
//=================================================================================
bool GeometryGUI::CustomPopup(QAD_Desktop* parent, QPopupMenu* popup, const QString& theContext,
			      const QString& theParent, const QString& theObject)
{ 
  GeometryGUI::GetOrCreateGeometryGUI(parent);

  /* Deactivate any non modal dialog box to get the neutral point */
  GeomGUI->EmitSignalDeactivateDialog();
  
  Engines::Component_var comp = GeomGUI->GetDesktop()->getEngine("FactoryServer", "GEOM");
  GEOM::GEOM_Gen_var myComponentGeom = GEOM::GEOM_Gen::_narrow(comp);

  SALOME_Selection* Sel = SALOME_Selection::Selection(GeomGUI->GetActiveStudy()->getSelection());
  int nbSel = Sel->IObjectCount();
  
  if((nbSel == 0) && (theContext.compare("Sketch")!=0))
    return false;

  if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
    OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
    Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
    if(theParent.compare("Viewer")==0) {
      if(theContext.compare("Sketch")==0) {
	SketchStatus myCS = GeomGUI->GetSketcher().GetCurrentStatus();
	popup->setCheckable(TRUE);
	if(myCS == SEGMENT) {
	  popup->setItemChecked(4041,true);  //Sketch Segment Menu
	  popup->setItemChecked(4042,false); //Sketch Arc Menu
	}
	else if(myCS == ARC_CHORD) {
	  popup->setItemChecked(4041,false); //Sketch Segment Menu
	  popup->setItemChecked(4042,true);  //Sketch Arc Menu
	}   
	return true;
      }
      else if(theObject.compare("Component") == 0) {
	popup->removeItem(QAD_DisplayOnly_Popup_ID);
	return true;
      }
      else {
	QFont f = QApplication::font();
	f.setBold( TRUE );
	if(nbSel==1) {
	  Handle(SALOME_InteractiveObject) IObject = Sel->firstIObject();
	  popup->removeItem(QAD_TopLabel_Popup_ID);
	  popup->insertItem(new CustomItem (QString(IObject->getName()), f), QAD_TopLabel_Popup_ID, 0);
	  if(IObject->hasEntry())
	    popup->setItemEnabled(804, false); //Add in Study Menu
	  else
	    popup->setItemEnabled(804, true);  //Add in Study Menu
    
// 	  if(IObject->IsKind(STANDARD_TYPE(GEOM_InteractiveObject))) {
// 	    Standard_Boolean found;
// 	    Handle(GEOM_AISShape) Result = GeomGUI->ConvertIOinGEOMAISShape(IObject, found, true);

// 	    if(found) {
// 	      if(Result->DisplayMode() == 1)
// 		popup->changeItem(8031, tr("GEOM_MEN_WIREFRAME")); //Shading/Wireframe Menu
// 	      else
// 		popup->changeItem(8031, tr("GEOM_MEN_SHADING"));   //Shading/Wireframe Menu
// 	    }
// 	  }

	  if(!(v3d->isInViewer(IObject) && v3d->isVisible(IObject)))
	    popup->removeItem(QAD_Erase_Popup_ID);
	  else
	    popup->removeItem(QAD_Display_Popup_ID);    
	}
	else {
	  popup->removeItem(QAD_DisplayOnly_Popup_ID);
	  popup->removeItem(QAD_TopLabel_Popup_ID);
	  popup->insertItem(new CustomItem (tr("GEOM_MEN_POPUP_NAME").arg(nbSel), f), QAD_TopLabel_Popup_ID, 0);
	  popup->setItemEnabled(804, false); //Add in Study Menu
	}
      }
      return true;
    }
    else if(theParent.compare("ObjectBrowser")==0) {
      popup->removeItem(QAD_TopLabel_Popup_ID);
      int id = popup->idAt(0); // separator
      if(id < 0)
	popup->removeItem(id);
	    
      // checking for GEOM label in the selected list
      SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
      Handle(SALOME_InteractiveObject) anIObject;

      bool useSubItems = false;
      bool needOpen = false;
      bool needDisplay = false;
      bool needErase = false;
      SALOMEDS::GenericAttribute_var aTmpAttr;
      for(;It.More();It.Next()) {
	anIObject = It.Value();
	if(!anIObject->hasEntry())
	  continue;

	if(v3d->isInViewer(anIObject) && v3d->isVisible(anIObject)) 
	  needErase = true; 
	else 
	  needDisplay = true;
	SALOMEDS::SObject_var obj = GeomGUI->GetActiveStudy()->getStudyDocument()->FindObjectID(anIObject->getEntry());
	if(!obj->_is_nil()) {
	  GEOM::GEOM_Shape_var aShape;
	  if(obj->FindAttribute(aTmpAttr, "AttributeIOR")) {
	    char *str = SALOMEDS::AttributeIOR::_narrow(aTmpAttr)->Value();
	    if(str && strlen(str))
	      aShape = myComponentGeom->GetIORFromString(str);
	  }
	  else if(obj->FindAttribute(aTmpAttr, "AttributePersistentRef"))
	    needOpen = true;
	  
	  if(aShape->_is_nil()) {
	    SALOMEDS::ChildIterator_var anIter = GeomGUI->GetActiveStudy()->getStudyDocument()->NewChildIterator(obj);
	    while(anIter->More()) {
	      SALOMEDS::SObject_var subobj = anIter->Value();
	      if(subobj->FindAttribute(aTmpAttr, "AttributeIOR")) {
		useSubItems = true;
		needErase = true;
		needDisplay = true;
	      }
	      else if(subobj->FindAttribute(aTmpAttr, "AttributePersistentRef")) {
		needOpen = true;
		useSubItems = true;
	      }
	      anIter->Next();
	    }
	  }
	}
      }
      // remove useless popup items
      if(nbSel != 1)
	popup->removeItem(901); // rename
      if(useSubItems)
	popup->removeItem(QAD_DisplayOnly_Popup_ID); // display only

      if(!needOpen) {
	int index = popup->indexOf(9024);  // open
	popup->removeItem(9024);
	popup->removeItemAt(index); // separator under Open

	if (!needDisplay)
	  popup->removeItem(QAD_Display_Popup_ID);
	if (!needErase)
	  popup->removeItem(QAD_Erase_Popup_ID);

	if(!needDisplay && !needErase) {
	  int id = popup->idAt(popup->count()-1); // last item
	  popup->removeItem(id); // separator
	}
      }
      else {
	popup->removeItem(QAD_DisplayOnly_Popup_ID); // display only
	popup->removeItem(QAD_Display_Popup_ID);
	popup->removeItem(QAD_Erase_Popup_ID);
	if (nbSel!=1) {
	  int id = popup->idAt(popup->count()-1); // last item
	  popup->removeItem(id); // separator
	}
      }
      return true;
    }
  }
  else if(GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
    // MESSAGE ( " CUSTOM POPUP VIEWER VTK ")
    if((theParent.compare("Viewer")==0)) {
      popup->setItemEnabled(8034, false); //Isos Menu
      if(theObject.compare("Component") == 0) {
	popup->removeItem(QAD_DisplayOnly_Popup_ID);
	return true;
      }

      //int id = popup->idAt(0);
      QFont f = QApplication::font();
      f.setBold( TRUE );
  
      if(nbSel==1) {
	Handle(SALOME_InteractiveObject) IObject = Sel->firstIObject();
	popup->removeItem(QAD_TopLabel_Popup_ID);
	popup->insertItem(new CustomItem (QString(IObject->getName()), f), QAD_TopLabel_Popup_ID, 0);
	if(IObject->hasEntry()) {
	  popup->setItemEnabled(804, false); //Add in Study Menu
	  SALOMEDS::Study_var aStudy = GeomGUI->GetActiveStudy()->getStudyDocument();
	  SALOMEDS::SObject_var SO = aStudy->FindObjectID(IObject->getEntry());
	  SALOMEDS::GenericAttribute_var anAttr;
	  SALOMEDS::AttributeIOR_var anIOR;
// 	  if(!SO->_is_nil()) {
// 	    if(SO->FindAttribute(anAttr, "AttributeIOR")) {
// 	      anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
// 	      Standard_Boolean found;
// 	      GEOM_Actor* Result = GeomGUI->ConvertIORinGEOMActor(anIOR->Value(), found, true);
// 	      if(found) {
// 		if(Result->getDisplayMode() == 1)
// 		  popup->changeItem(8031, tr("GEOM_MEN_WIREFRAME")); //Shading/Wireframe Menu
// 		else
// 		  popup->changeItem(8031, tr("GEOM_MEN_SHADING"));   //Shading/Wireframe Menu
			      
// 		if(!Result->GetVisibility())
// 		  popup->removeItem(QAD_Erase_Popup_ID);
// 		else
// 		  popup->removeItem(QAD_Display_Popup_ID);		  
// 	      }
// 	      else
// 		popup->removeItem(QAD_Erase_Popup_ID);
// 	    }
// 	  }
	}
	else
	  popup->setItemEnabled(804, true); //Add in Study Menu
      }
      else {
	popup->removeItem(QAD_DisplayOnly_Popup_ID);
	popup->removeItem(QAD_TopLabel_Popup_ID);
	popup->insertItem( new CustomItem (tr("GEOM_MEN_POPUP_NAME").arg(nbSel), f), QAD_TopLabel_Popup_ID, 0);
	popup->setItemEnabled(804, false); //Add in Study Menu
      }
      return true;
    }
    else if(theParent.compare("ObjectBrowser")==0) {
      popup->removeItem(QAD_TopLabel_Popup_ID);
      int id = popup->idAt(0); // separator
      if(id < 0)
	popup->removeItem(id);

      // checking for GEOM label in the selected list
      SALOME_ListIteratorOfListIO It(Sel->StoredIObjects());
      Handle(SALOME_InteractiveObject) anIObject;
      
      bool useSubItems = false;
      bool needOpen = false;
      bool needDisplay = false;
      bool needErase = false;
      SALOMEDS::GenericAttribute_var aTmpAttr;
      VTKViewer_RenderWindowInteractor* myRenderInter = ((VTKViewer_ViewFrame*)GeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
      for(;It.More();It.Next()) {
	anIObject = It.Value();
	if(!anIObject->hasEntry())
	  continue;

	if(myRenderInter->isInViewer(anIObject) && myRenderInter->isVisible(anIObject)) 
	  needErase = true; 
	else 
	  needDisplay = true;

	SALOMEDS::SObject_var obj = GeomGUI->GetActiveStudy()->getStudyDocument()->FindObjectID( anIObject->getEntry());
	if(!obj->_is_nil()) {
	  GEOM::GEOM_Shape_var aShape;
	  if(obj->FindAttribute(aTmpAttr, "AttributeIOR")) {
	    char *str = SALOMEDS::AttributeIOR::_narrow(aTmpAttr)->Value();
	    if(str && strlen(str))
	      aShape = myComponentGeom->GetIORFromString(str);
	  }
	  else if(obj->FindAttribute(aTmpAttr, "AttributePersistentRef"))
	    needOpen = true;

	  if(aShape->_is_nil()) {
	    SALOMEDS::ChildIterator_var anIter = GeomGUI->GetActiveStudy()->getStudyDocument()->NewChildIterator(obj);
	    while(anIter->More()) {
	      SALOMEDS::SObject_var subobj = anIter->Value();
	      if(subobj->FindAttribute(aTmpAttr, "AttributeIOR")) {
		useSubItems = true;
		needDisplay = true;
		needErase = true;
	      }
	      else if(subobj->FindAttribute(aTmpAttr, "AttributePersistentRef")) {
		needOpen = true;
		useSubItems = true;
	      }
	      anIter->Next();
	    }
	  }
	}
      }
      // remove useless popup items
      if(nbSel != 1)
	popup->removeItem(901); // rename
      if(useSubItems)
	popup->removeItem(QAD_DisplayOnly_Popup_ID); // display only

      if(!needOpen) {
	int index = popup->indexOf(9024); // open
	popup->removeItem(9024);
	popup->removeItemAt(index); // separator under Open
	  
	if(!needDisplay)
	  popup->removeItem(QAD_Display_Popup_ID);
	if(!needErase)
	  popup->removeItem(QAD_Erase_Popup_ID);
	if(!needDisplay && !needErase) {
	  int id = popup->idAt(popup->count()-1); // last item
	  popup->removeItem(id); // separator
	}
      }
      else {
	popup->removeItem(QAD_DisplayOnly_Popup_ID); // display only
	popup->removeItem(QAD_Display_Popup_ID);
	popup->removeItem(QAD_Erase_Popup_ID);
	if(nbSel!=1) {
	  int id = popup->idAt(popup->count()-1); // last item
	  popup->removeItem(id); // separator
	}
      }
      return true;
    }
  } 
  else { // other viewer types not supported.
    while(1) {
      int id = popup->idAt(0);
      if(id <= QAD_TopLabel_Popup_ID && id != -1)
	popup->removeItemAt(0);
      else
	break;
    }
    popup->removeItem(QAD_DisplayOnly_Popup_ID);	
    popup->removeItem(QAD_Display_Popup_ID);
    popup->removeItem(QAD_Erase_Popup_ID);
    int id = popup->idAt(popup->count()-1); // last item
    if(id < 0 && id != -1)
      popup->removeItem(id); // separator
    return false;
  }
  return false;
}


//=================================================================================
// function : activeStudyChanged()
// purpose  : static
//=================================================================================
void GeometryGUI::activeStudyChanged(QAD_Desktop* parent)
{
  GeometryGUI::GetOrCreateGeometryGUI(parent); 

  if(GeomGUI != 0) {
    if(GeomGUI->GetState() == CURRENT_SKETCH) {
      GeomGUI->GetSketcher().Clear();
      GeomGUI->ResetState();
    }

    QMenuBar* Mb = GeomGUI->GetDesktop()->getMainMenuBar();
    bool ViewOCC = false;
    if(GeomGUI->GetDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) 
      ViewOCC = true;

    Mb->setItemEnabled(404, ViewOCC);//Sketch
    Mb->setItemEnabled(405, ViewOCC);//Sketch
    Mb->setItemEnabled(406, ViewOCC);//Sketch

    Mb->setItemEnabled(603, ViewOCC);//SuppressFace
    Mb->setItemEnabled(604, ViewOCC);//SuppressHole
  
    Mb->setItemEnabled(413, ViewOCC);// ShadingColor Settings
    Mb->setItemEnabled(414, ViewOCC);// Isos Settings

    //GeomGUI->EraseSimulationShape();
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
  else
    return;
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
