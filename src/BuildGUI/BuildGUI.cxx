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
//  File   : BooleanGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "BuildGUI.h"

#include "OCCViewer_Viewer3d.h"
#include "VTKViewer_ViewFrame.h"
#include "QAD_RightFrame.h"
#include "GEOM_AssemblyBuilder.h"
#include "SALOMEGUI_ImportOperation.h"

#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>

#include "BuildGUI_SubShapeDlg.h"   // Method SUBSHAPE
#include "BuildGUI_EdgeDlg.h"       // Method EDGE
#include "BuildGUI_WireDlg.h"       // Method WIRE
#include "BuildGUI_FaceDlg.h"       // Method FACE
#include "BuildGUI_ShellDlg.h"      // Method SHELL
#include "BuildGUI_SolidDlg.h"      // Method SOLID
#include "BuildGUI_CompoundDlg.h"   // Method COMPOUND

static BuildGUI* myBuildGUI = 0;

//=======================================================================
// function : BuildGUI()
// purpose  : Constructor
//=======================================================================
BuildGUI::BuildGUI() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~BuildGUI()
// purpose  : Destructor
//=======================================================================
BuildGUI::~BuildGUI()
{
}


//=======================================================================
// function : GetOrCreateGUI()
// purpose  : Gets or create an object 'GUI' with initialisations
//          : Returns 'GUI' as a pointer
//=======================================================================
BuildGUI* BuildGUI::GetOrCreateGUI()
{
  myBuildGUI = new BuildGUI();
  return myBuildGUI;
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool BuildGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  BuildGUI::GetOrCreateGUI();
  myBuildGUI->myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myBuildGUI->myGeomGUI->GetActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 407: // EXPLODE : use ic
      {
	Handle(AIS_InteractiveContext) ic;
	if(myBuildGUI->myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myBuildGUI->myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
	  ic = v3d->getAISContext();
	}
	BuildGUI_SubShapeDlg *aDlg = new BuildGUI_SubShapeDlg(parent, "", myBuildGUI, Sel, ic);
	break ;
      }
    case 4081: // GEOM::EDGE
      {
	BuildGUI_EdgeDlg *aDlg = new BuildGUI_EdgeDlg(parent, "", myBuildGUI, Sel);
	break;
      }
    case 4082: // GEOM::WIRE
      {
	BuildGUI_WireDlg *aDlg = new BuildGUI_WireDlg(parent, "", myBuildGUI, Sel);
	break;
      }
    case 4083: // GEOM::FACE
      {
	BuildGUI_FaceDlg *aDlg = new BuildGUI_FaceDlg(parent, "", myBuildGUI, Sel);
	break;
      }
    case 4084: // GEOM::SHELL
      {
	BuildGUI_ShellDlg *aDlg = new BuildGUI_ShellDlg(parent, "", myBuildGUI, Sel);
	break;
      }
    case 4085: // GEOM::SOLID
      {
	BuildGUI_SolidDlg *aDlg = new BuildGUI_SolidDlg(parent, "", myBuildGUI, Sel);
	break;
      }
    case 4086: // GEOM::COMPOUND
      {
	BuildGUI_CompoundDlg *aDlg = new BuildGUI_CompoundDlg(parent, "", myBuildGUI, Sel);
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
// function : MakeLinearEdgeAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeLinearEdgeAndDisplay(const gp_Pnt P1, const gp_Pnt P2)
{
  try {
    GEOM::PointStruct ps1 = myGeom->MakePointStruct(P1.X(), P1.Y(), P1.Z());
    GEOM::PointStruct ps2 = myGeom->MakePointStruct(P2.X(), P2.Y(), P2.Z());
    GEOM::GEOM_Shape_var result = myGeom->MakeEdge(ps1, ps2);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    result->NameType(tr("GEOM_EDGE"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeWireAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeWireAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeWire(listShapesIOR);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    result->NameType(tr("GEOM_WIRE"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeFaceAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeFaceAndDisplay(GEOM::GEOM_Shape_ptr aWire, const Standard_Boolean wantPlanar)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeFace(aWire, wantPlanar);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    if (wantPlanar)
      result->NameType(tr("GEOM_PLANE"));
    else
      result->NameType(tr("GEOM_FACE"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeShellAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeShellAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeShell(listShapesIOR);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    result->NameType(tr("GEOM_SHELL"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeSolidAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeSolidAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeSolid(listShapesIOR);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    result->NameType(tr("GEOM_SOLID"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeCompoundAndDisplay()
// purpose  :
//=====================================================================================
void BuildGUI::MakeCompoundAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeCompound(listShapesIOR);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE"));
      return;
    }
    result->NameType(tr("GEOM_COMPOUND"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : OnSubShapeGetAll()
// purpose  : Explode a shape in all sub shapes with a SubShapeType
//=====================================================================================
bool BuildGUI::OnSubShapeGetAll(const TopoDS_Shape& ShapeTopo, const char* ShapeTopoIOR, const int SubShapeType)
{
  SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
  SALOMEDS::SObject_var theObj = aStudy->FindObjectIOR(ShapeTopoIOR);
  if(theObj->_is_nil()) {
    myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SHAPE_IN_STUDY"));
    return false;
  }

  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->entry());
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributePixMap_var aPixmap;

  /* We create a sub object for each sub shape as attribute of the main object  */
  /* Each sub object contains list (length=1) containing its index in the main shape */
  GEOM::GEOM_Shape_var aShape = myGeom->GetIORFromString(ShapeTopoIOR);
  GEOM::GEOM_Gen::ListOfGeomShapes_var listGeomShapes = new GEOM::GEOM_Gen::ListOfGeomShapes;
  GEOM::GEOM_Shape_var aResult;
  
  try {
    listGeomShapes = myGeom->SubShapeAll(aShape, SubShapeType);
    if(listGeomShapes->length() < 1) {
      myGeomGUI->GetDesktop()->putInfo (tr("GEOM_PRP_ABORT"));
      return false;
    }
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  
  /* open transaction */
  QAD_Operation* op = new SALOMEGUI_ImportOperation(myGeomGUI->GetActiveStudy());
  op->start();
  
  TopoDS_Shape mainTopo = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
  TopoDS_Shape mainShape;
  bool main = false;
  while(!main) {
    if(aShape->IsMainShape()) {
      mainShape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
      main = true;
    } 
    else
      aShape = myGeom->GetIORFromString(aShape->MainName());
  }
  
  /* Loop on each sub shape created */
  /* int i = 1 ; /* index for the nameType */  
  for(int j=0; j<listGeomShapes->length(); j++) {
    /* Get each sub shape extracted CORBA and OCC */
    aResult = listGeomShapes[j] ;    
    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, aResult);
    
    if (S.IsNull()) {
      myGeomGUI->GetDesktop()->putInfo (tr("GEOM_PRP_ABORT"));
      return false;
    }
    
    /* Set the nameType of sub shape */
    char* nameG = (char *)malloc(20);
    Standard_CString Type;
    if(myGeomGUI->GetShapeTypeString(S, Type)) {
      aResult->NameType(Type);
      sprintf(nameG, "%s_%d", Type, myGeomGUI->GetIndex(S, mainShape, SubShapeType));
    } 
    else {
      aResult->NameType(tr("GEOM_SHAPE"));
      sprintf(nameG, "%s_%d", tr("GEOM_SHAPE").latin1(), myGeomGUI->GetNbGeom()++);
    }
    SALOMEDS::SObject_var SO = aStudy->FindObjectIOR(aResult->Name());

    bool allreadyexist = false;
          
    if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
      OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
      Handle (AIS_InteractiveContext) ic = v3d->getAISContext();
      
      Handle(GEOM_AISShape) result = new GEOM_AISShape(S, nameG);      
      Handle(GEOM_InteractiveObject) IO = new GEOM_InteractiveObject(aResult->Name(), myGeomGUI->GetFatherior(), "GEOM");
      
      MESSAGE ("SO->_is_nil() " << SO->_is_nil())

      if(SO->_is_nil()) {
	SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(theObj);
        anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
        aName = SALOMEDS::AttributeName::_narrow(anAttr);
        aName->SetValue(nameG);
        anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
        anIOR->SetValue(aResult->Name());

	anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
	aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
	MESSAGE(" Type " << S.ShapeType())
	if (S.ShapeType() == TopAbs_COMPOUND)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_COMPOUND");
	else if(S.ShapeType() == TopAbs_COMPSOLID)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_COMPSOLID");
	else if(S.ShapeType() == TopAbs_SOLID)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_SOLID");
	else if(S.ShapeType() == TopAbs_SHELL)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_SHELL");
	else if(S.ShapeType() == TopAbs_FACE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_FACE");
	else if(S.ShapeType() == TopAbs_WIRE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_WIRE");
	else if(S.ShapeType() == TopAbs_EDGE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_EDGE");
	else if(S.ShapeType() == TopAbs_VERTEX)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_VERTEX");

	MESSAGE(" aPixmap->GetPixMap " << aPixmap->GetPixMap())

	SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
	aStudyBuilder->Addreference(newObj1, newObj);
	IO->setEntry(newObj->GetID());

	aResult->StudyShapeId(newObj->GetID());
      }
      else {
	allreadyexist = true;
 	if(!myGeomGUI->SObjectExist(theObj, aResult->Name())) {
	  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(theObj);
	  aStudyBuilder->Addreference(newObj1, SO);
	  IO->setEntry(SO->GetID());
	  aResult->StudyShapeId(SO->GetID());
	}
      }
      
      result->setIO(IO);
      result->setName(nameG);
      if(!allreadyexist)
	ic->Display(result);
      
    }
    else if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_VTK) {
      VTKViewer_RenderWindowInteractor* myRenderInter= ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRWInteractor();
      
      int themode = myRenderInter->GetDisplayMode();
      vtkRenderer *theRenderer = ((VTKViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getRenderer();
      vtkRenderWindow *renWin = theRenderer->GetRenderWindow();
      
      Handle(GEOM_InteractiveObject) IO = new GEOM_InteractiveObject(aResult->Name(), myGeomGUI->GetFatherior(), "GEOM");
      
      if(SO->_is_nil()) {
	SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(theObj);
        anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
        anIOR->SetValue(aResult->Name());
        anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
        aName = SALOMEDS::AttributeName::_narrow(anAttr);
        aName->SetValue(nameG);
	
	anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
	aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
	if(S.ShapeType() == TopAbs_COMPOUND)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_COMPOUND");
	else if(S.ShapeType() == TopAbs_COMPSOLID)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_COMPSOLID");
	else if(S.ShapeType() == TopAbs_SOLID)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_SOLID");
	else if(S.ShapeType() == TopAbs_SHELL)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_SHELL");
	else if(S.ShapeType() == TopAbs_FACE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_FACE");
	else if(S.ShapeType() == TopAbs_WIRE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_WIRE");
	else if(S.ShapeType() == TopAbs_EDGE)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_EDGE");
	else if(S.ShapeType() == TopAbs_VERTEX)
	  aPixmap->SetPixMap("ICON_OBJBROWSER_VERTEX");

	SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
	aStudyBuilder->Addreference(newObj1, newObj);
	IO->setEntry(newObj->GetID());
      }
      else {
	allreadyexist = true;
	if(!myGeomGUI->SObjectExist(theObj, aResult->Name())) {
	  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(theObj);
	  aStudyBuilder->Addreference(newObj1, SO);
	  IO->setEntry(SO->GetID());
	}
      }
      
      if(!allreadyexist) {
	vtkActorCollection* theActors = GEOM_AssemblyBuilder::BuildActors(S,0,themode,Standard_True);
	theActors->InitTraversal();
	vtkActor* anActor = (vtkActor*)theActors->GetNextActor();
	while(!(anActor==NULL)) {
	  GEOM_Actor* GActor = GEOM_Actor::SafeDownCast(anActor);
	  GActor->setIO(IO);
	  GActor->setName(nameG);
	  theRenderer->AddActor(GActor);
	  renWin->Render();
	  anActor = (vtkActor*)theActors->GetNextActor();
	}
      }
    }
  }

  /* commit transaction */
  op->finish();

  myGeomGUI->GetActiveStudy()->updateObjBrowser();
  myGeomGUI->GetDesktop()->putInfo (tr("GEOM_PRP_READY"));
  return true;
}


//=====================================================================================
// function : OnSubShapeGetSelected()
// purpose  :
//=====================================================================================
bool BuildGUI::OnSubShapeGetSelected(const TopoDS_Shape& ShapeTopo, const char* ShapeTopoIOR, const int SubShapeType, Standard_Integer& aLocalContextId, bool& myUseLocalContext)
{
  //* Test the type of viewer */
  if(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() > VIEW_OCC)
    return false;

  SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
  SALOMEDS::SObject_var theObj = aStudy->FindObjectIOR(ShapeTopoIOR);
  if(theObj->_is_nil()) {
    myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SHAPE_IN_STUDY"));
    return false;
  }

  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
  Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
  
  if( myUseLocalContext == false ) {
    /* local context is from DialogBox */
    MESSAGE("Error : No local context opened for sub shapes method" << endl ) ;
    return false ;
  }

  GEOM::GEOM_Shape_var aShape  = myGeom->GetIORFromString( ShapeTopoIOR );
  TopoDS_Shape mainTopo = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);

  TopoDS_Shape mainShape;
  bool main = false;
  while(!main) {
    if(aShape->IsMainShape()) {
      mainShape = myGeomGUI->GetShapeReader().GetShape(myGeom, aShape);
      main = true;
    }
    else
      aShape = myGeom->GetIORFromString(aShape->MainName());
  }

  GEOM::GEOM_Shape::ListOfSubShapeID_var ListOfID = new GEOM::GEOM_Shape::ListOfSubShapeID;
  ic->InitSelected();
  int nbSelected = ic->NbSelected();
  ListOfID->length(nbSelected);
  
  TopoDS_Compound compound;
  ic->InitSelected(); /* to init again */ 
  BRep_Builder B;
  B.MakeCompound(compound);

  int i = 0;
  /* We create a unique compound containing all the sub shapes selected by user as attribute of the main shape */
  /* the compound is homogenous by selection */
  while(ic->MoreSelected()) {
    int index = myGeomGUI->GetIndex(ic->SelectedShape(), mainShape, SubShapeType);
    ListOfID[i] = index;
    B.Add(compound, ic->SelectedShape());
    i++;
    ic->NextSelected();
  }
 
  /* Test if user has selected sub shapes */
  if(ListOfID->length() < 1)
    return false;

  GEOM::GEOM_Shape_var aResult;
  try {
    aResult = myGeom->SubShape(aShape, SubShapeType, ListOfID);
  }
  catch (const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }

  /* local context from DialogBox */
  ic->CloseLocalContext(aLocalContextId);
  myUseLocalContext = false ;

  char* nameG = (char *)malloc(20);
  Standard_CString Type;

  Handle(GEOM_AISShape) result; 
  Handle(GEOM_InteractiveObject) IO;

  if(nbSelected == 1) {
    TopExp_Explorer Exp (compound, TopAbs_ShapeEnum(SubShapeType));
    if(Exp.More()) {
      if(myGeomGUI->GetShapeTypeString(Exp.Current(),Type)) {
	aResult->NameType(Type);
	sprintf (nameG, "%s_%d", Type, myGeomGUI->GetIndex( Exp.Current(), mainTopo, SubShapeType));
      } 
      else {
	aResult->NameType(tr("GEOM_SHAPE"));
	sprintf (nameG, "%s_%d", tr("GEOM_SHAPE").latin1(), myGeomGUI->GetNbGeom()++);
      }
      result = new GEOM_AISShape(Exp.Current(), nameG);
      IO = new GEOM_InteractiveObject(aResult->Name(), myGeomGUI->GetFatherior(), "GEOM");
    }
  }
  else {
    if ( myGeomGUI->GetShapeTypeString(compound,Type)) {
      aResult->NameType(Type);
      sprintf (nameG, "%s_%d", Type, myGeomGUI->GetNbGeom()++);
    } else {
      aResult->NameType(tr("GEOM_SHAPE"));
      sprintf (nameG, "%s_%d", tr("GEOM_SHAPE").latin1(), myGeomGUI->GetNbGeom()++);
    }
    result = new GEOM_AISShape(compound, nameG);
    IO = new GEOM_InteractiveObject(aResult->Name(), myGeomGUI->GetFatherior(), "GEOM");
  }
  
  SALOMEDS::SObject_var SO = aStudy->FindObjectIOR(aResult->Name());
  
  /* open transaction */
  QAD_Operation* op = new SALOMEGUI_ImportOperation(myGeomGUI->GetActiveStudy());
  op->start();
  
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->entry());
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributePixMap_var aPixmap;
 
  bool allreadyexist = false;

  if(SO->_is_nil()) {
    SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(theObj);
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
    anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
    anIOR->SetValue(aResult->Name());
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(result->getName());
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
    aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
    if(result->Shape().ShapeType() == TopAbs_COMPOUND)
      aPixmap->SetPixMap("ICON_OBJBROWSER_COMPOUND");
    else if(result->Shape().ShapeType() == TopAbs_COMPSOLID)
      aPixmap->SetPixMap("ICON_OBJBROWSER_COMPSOLID");
    else if(result->Shape().ShapeType() == TopAbs_SOLID)
      aPixmap->SetPixMap("ICON_OBJBROWSER_SOLID");
    else if(result->Shape().ShapeType() == TopAbs_SHELL)
      aPixmap->SetPixMap("ICON_OBJBROWSER_SHELL");
    else if(result->Shape().ShapeType() == TopAbs_FACE)
      aPixmap->SetPixMap("ICON_OBJBROWSER_FACE");
    else if(result->Shape().ShapeType() == TopAbs_WIRE)
      aPixmap->SetPixMap("ICON_OBJBROWSER_WIRE");
    else if(result->Shape().ShapeType() == TopAbs_EDGE)
      aPixmap->SetPixMap("ICON_OBJBROWSER_EDGE");
    else if(result->Shape().ShapeType() == TopAbs_VERTEX)
      aPixmap->SetPixMap("ICON_OBJBROWSER_VERTEX");

    SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
    aStudyBuilder->Addreference(newObj1, newObj);
    
    IO->setEntry(newObj->GetID());
    aResult->StudyShapeId(newObj->GetID());
  } 
  else {
    allreadyexist = true;
    if(!myGeomGUI->SObjectExist(theObj, aResult->Name())) {
      SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(theObj);
      aStudyBuilder->Addreference(newObj1, SO);
    
      IO->setEntry(SO->GetID());
      aResult->StudyShapeId(SO->GetID());
    }
  }
  
  /* commit transaction */
  op->finish();
  
  result->setIO(IO);
  result->setName(nameG);

  if(!allreadyexist)
    ic->Display(result);

  GEOMBase_Display* myDisplayGUI = new GEOMBase_Display();
  myDisplayGUI->OnDisplayAll(true);

  myGeomGUI->GetActiveStudy()->updateObjBrowser();
  myGeomGUI->GetDesktop()->putInfo (tr("GEOM_PRP_READY"));
  return true;
}


//=====================================================================================
// EXPORTED METHODS
//=====================================================================================
extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {return BuildGUI::OnGUIEvent(theCommandID, parent);}
}
