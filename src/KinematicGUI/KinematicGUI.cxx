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
//  File   : KinematicGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "KinematicGUI.h"

#include "QAD_RightFrame.h"
#include "OCCViewer_Viewer3d.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"
#include "SALOMEGUI_ImportOperation.h"

#include <list>
#include <map>

#include "KinematicGUI_ContactDlg.h"     // Method CONTACT
#include "KinematicGUI_PositionDlg.h"    // Method POSITION
#include "KinematicGUI_RangeDlg.h"       // Method RANGE
#include "KinematicGUI_AnimationDlg.h"   // Method ANIMATION
#include "KinematicGUI_AnimValuesDlg.h"  // Method SET VALUES
#include "KinematicGUI_AnimDlg.h"        // Method Play ANIMATION

using namespace std;

//=======================================================================
// function : KinematicGUI()
// purpose  : Constructor
//=======================================================================
KinematicGUI::KinematicGUI() :
  QObject()
{
  myGeomBase = new GEOMBase();
  myGeomGUI = GEOMContext::GetGeomGUI();
  myGeom = myGeomGUI->myComponentGeom;
}


//=======================================================================
// function : ~KinematicGUI()
// purpose  : Destructor
//=======================================================================
KinematicGUI::~KinematicGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool KinematicGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  KinematicGUI* myKinematicGUI = new KinematicGUI();
  myKinematicGUI->myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(QAD_Application::getDesktop()->getActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 611: // ASSEMBLY
      {
	myKinematicGUI->AddNewAssembly();
	break;
      }
    case 6121: // CONTACT
      {
	KinematicGUI_ContactDlg *aDlg = new KinematicGUI_ContactDlg(parent, "", myKinematicGUI, Sel);
	break;
      }
    case 6122: // POSITION
      {
	KinematicGUI_PositionDlg *aDlg = new KinematicGUI_PositionDlg(parent, "", myKinematicGUI, Sel, true);
	break;
      }
    case 6123: // ANGULAR RANGE
      {
	KinematicGUI_RangeDlg *aDlg = new KinematicGUI_RangeDlg(parent, "", myKinematicGUI, Sel, 0);
	break;
      }
    case 6124: // LINEAR RANGE
      {
	KinematicGUI_RangeDlg *aDlg = new KinematicGUI_RangeDlg(parent, "", myKinematicGUI, Sel, 1);
	break;
      }
    case 613: // ANIMATION
      {
	KinematicGUI_AnimationDlg *aDlg = new KinematicGUI_AnimationDlg(parent, "", myKinematicGUI, Sel);
	break;
      }
    case 6131: // SET VALUES
      {
	KinematicGUI_AnimValuesDlg *aDlg = new KinematicGUI_AnimValuesDlg(parent, "", myKinematicGUI, Sel);
	break;
      }
    case 6132: // RUN ANIMATION
      {
	if(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC) {
	  Handle(AIS_InteractiveContext) ic;
	  OCCViewer_Viewer3d* v3d = ((OCCViewer_ViewFrame*)QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewer();
	  ic = v3d->getAISContext();
	  KinematicGUI_AnimDlg *aDlg = new KinematicGUI_AnimDlg(parent, "", myKinematicGUI, Sel, ic);
	}
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
// function : AddNewAssembly()
// purpose  : 
//=======================================================================
void KinematicGUI::AddNewAssembly()
{
  try {
    GEOM::GEOM_Assembly_ptr result = myGeom->InitAssembly();
    if(result->_is_nil()) {
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  

    if(this->AddAssemblyInStudy(result))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================
// function : AddContact()
// purpose  : 
//=======================================================================
void KinematicGUI::AddContact(GEOM::GEOM_Assembly_ptr Ass, GEOM::GEOM_Shape_ptr Shape1,
			      GEOM::GEOM_Shape_ptr Shape2, int type, double step)
{
  try {
    GEOM::GEOM_Contact_ptr result = myGeom->AddContact(Ass, Shape1, Shape2, type, step);
    if(result->_is_nil()) {
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  
    
    if(this->AddContactInStudy(Ass, result))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================
// function : AddAnimation()
// purpose  : 
//=======================================================================
void KinematicGUI::AddAnimation(GEOM::GEOM_Assembly_ptr Ass, GEOM::GEOM_Shape_ptr Shape1,
				double Duration, int NbSeq)
{
  try {
    GEOM::GEOM_Animation_ptr result = myGeom->AddAnimation(Ass, Shape1, Duration, NbSeq);
    if(result->_is_nil()) {
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  

    if(this->AddAnimationInStudy(result))
      QAD_Application::getDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : AddAssemblyInStudy()
// purpose  : 
//=====================================================================================
bool KinematicGUI::AddAssemblyInStudy(GEOM::GEOM_Assembly_ptr aAssembly)
{
  SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributePixMap_var aPixmap;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributeComment_var aType;

  SALOMEDS::SComponent_var father = aStudy->FindComponent("GEOM");
  int aLocked = false;
  if(father->_is_nil()) {
    QAD_Operation* op = new SALOMEGUI_ImportOperation(QAD_Application::getDesktop()->getActiveStudy());
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
    if(aLocked)
      aStudy->GetProperties()->SetLocked(true);
    op->finish();
  }

  aStudyBuilder->DefineComponentInstance(father, myGeom);
  father->ComponentIOR(myGeomGUI->GetFatherior());
  
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->entry());
  
  TCollection_AsciiString nameG("Assembly_");
  nameG += TCollection_AsciiString(myGeomGUI->myNbGeom++) + TCollection_AsciiString("\0");

  // open transaction
  QAD_Operation* op = new SALOMEGUI_ImportOperation(QAD_Application::getDesktop()->getActiveStudy());
  op->start();

  SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(father);
  /* For the shape inserted into the study we set its field 'studyshapeid'    */
  /* so the shape will contain its corresponding entry in the study Ocaf doc. */
  aAssembly->StudyShapeId(newObj->GetID());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
  anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
  anIOR->SetValue(aAssembly->Name());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
  aName = SALOMEDS::AttributeName::_narrow(anAttr);
  aName->SetValue(nameG.ToCString());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
  aType = SALOMEDS::AttributeComment::_narrow(anAttr);
  aType->SetValue("Kinematic_Assembly");

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
  aPixmap->SetPixMap("ICON_OBJBROWSER_ASSEMBLY");

  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
  aStudyBuilder->Addreference(newObj1, newObj);

  op->finish();

  QAD_Application::getDesktop()->getActiveStudy()->updateObjBrowser();
  return true;
}


//=====================================================================================
// function : AddContactInStudy()
// purpose  : 
//=====================================================================================
bool KinematicGUI::AddContactInStudy(GEOM::GEOM_Assembly_ptr Ass, GEOM::GEOM_Contact_ptr aContact)
{
  SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributeName_var aTypeName;
  SALOMEDS::AttributePixMap_var aPixmap;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributeComment_var aType;
  SALOMEDS::AttributeSelectable_var aSelAttr;

  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->entry());

  TCollection_AsciiString nameG("Contact_");
  nameG += TCollection_AsciiString(myGeomGUI->myNbGeom++) + TCollection_AsciiString("\0");

  // open transaction
  QAD_Operation* op = new SALOMEGUI_ImportOperation(QAD_Application::getDesktop()->getActiveStudy());
  op->start();

  Standard_CString AssIOR = aStudy->ConvertObjectToIOR(Ass);
  SALOMEDS::SObject_var theAss = aStudy->FindObjectIOR(AssIOR);
  SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(theAss);
  /* For the shape inserted into the study we set its field 'studyshapeid'    */
  /* so the shape will contain its corresponding entry in the study Ocaf doc. */
  aContact->StudyShapeId(newObj->GetID());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
  anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
  anIOR->SetValue(aContact->Name());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
  aName = SALOMEDS::AttributeName::_narrow(anAttr);
  aName->SetValue(nameG.ToCString());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
  aType = SALOMEDS::AttributeComment::_narrow(anAttr);
  aType->SetValue("Kinematic_Contact");

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
  aPixmap->SetPixMap("ICON_OBJBROWSER_CONTACT");

  SALOMEDS::SObject_var RefType = aStudyBuilder->NewObject(newObj);
  anAttr = aStudyBuilder->FindOrCreateAttribute(RefType, "AttributeName");
  aTypeName = SALOMEDS::AttributeName::_narrow(anAttr);
  aTypeName->SetValue(this->GetNameFromType(aContact->GetType()).ToCString());
  anAttr = aStudyBuilder->FindOrCreateAttribute(RefType, "AttributeSelectable");
  aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
  aSelAttr->SetSelectable(false);

  Standard_CString Shape1IOR = aStudy->ConvertObjectToIOR(aContact->GetShape1());
  SALOMEDS::SObject_var theSh1 = aStudy->FindObjectIOR(Shape1IOR);
  if(!theSh1->_is_nil()) {
    SALOMEDS::SObject_var RefObject1 = aStudyBuilder->NewObject(newObj);
    aStudyBuilder->Addreference(RefObject1, theSh1);
  }

  Standard_CString Shape2IOR = aStudy->ConvertObjectToIOR(aContact->GetShape2());
  SALOMEDS::SObject_var theSh2 = aStudy->FindObjectIOR(Shape2IOR);
  if(!theSh2->_is_nil()) {
    SALOMEDS::SObject_var RefObject2 = aStudyBuilder->NewObject(newObj);
    aStudyBuilder->Addreference(RefObject2, theSh2);
  }

  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
  aStudyBuilder->Addreference(newObj1, newObj);

  op->finish();

  QAD_Application::getDesktop()->getActiveStudy()->updateObjBrowser();
  return true;
}


//=====================================================================================
// function : AddAnimationInStudy()
// purpose  : 
//=====================================================================================
bool KinematicGUI::AddAnimationInStudy(GEOM::GEOM_Animation_ptr aAnimation)
{
  SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  SALOMEDS::StudyBuilder_var aStudyBuilder = aStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var aName;
  SALOMEDS::AttributePixMap_var aPixmap;
  SALOMEDS::AttributeIOR_var anIOR;
  SALOMEDS::AttributeComment_var aType;

  SALOMEDS::SComponent_var father = aStudy->FindComponent("GEOM");
  aStudyBuilder->DefineComponentInstance(father, myGeom);
  father->ComponentIOR(myGeomGUI->GetFatherior());
  
  SALOMEDS::SObject_var fatherSF = aStudy->FindObjectID(QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->entry());
  
  TCollection_AsciiString nameG("Animation_");
  nameG += TCollection_AsciiString(myGeomGUI->myNbGeom++) + TCollection_AsciiString("\0");

  // open transaction
  QAD_Operation* op = new SALOMEGUI_ImportOperation(QAD_Application::getDesktop()->getActiveStudy());
  op->start();

  SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(father);
  /* For the shape inserted into the study we set its field 'studyshapeid'    */
  /* so the shape will contain its corresponding entry in the study Ocaf doc. */
  aAnimation->StudyShapeId(newObj->GetID());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeIOR");
  anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
  anIOR->SetValue(aAnimation->Name());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
  aName = SALOMEDS::AttributeName::_narrow(anAttr);
  aName->SetValue(nameG.ToCString());

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
  aType = SALOMEDS::AttributeComment::_narrow(anAttr);
  aType->SetValue("Kinematic_Animation");

  anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributePixMap");
  aPixmap = SALOMEDS::AttributePixMap::_narrow(anAttr);
  aPixmap->SetPixMap("ICON_OBJBROWSER_ANIMATION");

  Standard_CString assIOR = aStudy->ConvertObjectToIOR(aAnimation->GetAssembly());
  SALOMEDS::SObject_var theAss = aStudy->FindObjectIOR(assIOR);
  if(!theAss->_is_nil()) {
    SALOMEDS::SObject_var RefObject = aStudyBuilder->NewObject(newObj);
    aStudyBuilder->Addreference(RefObject, theAss);
  }

  Standard_CString Shape1IOR = aStudy->ConvertObjectToIOR(aAnimation->GetFrame());
  SALOMEDS::SObject_var theSh1 = aStudy->FindObjectIOR(Shape1IOR);
  if(!theSh1->_is_nil()) {
    SALOMEDS::SObject_var RefObject1 = aStudyBuilder->NewObject(newObj);
    aStudyBuilder->Addreference(RefObject1, theSh1);
  }

  SALOMEDS::SObject_var newObj1 = aStudyBuilder->NewObject(fatherSF);
  aStudyBuilder->Addreference(newObj1, newObj);

  op->finish();

  QAD_Application::getDesktop()->getActiveStudy()->updateObjBrowser();
  return true;
}


//=======================================================================
// function : SetPosition()
// purpose  : 
//=======================================================================
void KinematicGUI::SetPosition(GEOM::GEOM_Contact_ptr aContact, GEOM::PointStruct P0,
			       GEOM::DirStruct VX, GEOM::DirStruct VY, GEOM::DirStruct VZ)
{
  try {
    aContact->SetPosition(P0.x, P0.y, P0.z, VX.PS.x, VX.PS.y, VX.PS.z,
			  VY.PS.x, VY.PS.y, VY.PS.z, VZ.PS.x, VZ.PS.y, VZ.PS.z);
    myGeom->SetPosition(aContact);
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================
// function : SetAngularRange()
// purpose  : 
//=======================================================================
void KinematicGUI::SetAngularRange(GEOM::GEOM_Contact_ptr aContact,
				   double MinValX, double MaxValX, double MinValY,
				   double MaxValY, double MinValZ, double MaxValZ)
{
  try {
    aContact->SetAngularRange(MinValX, MaxValX, MinValY, MaxValY, MinValZ, MaxValZ);
    myGeom->SetAngularRange(aContact);
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================
// function : SetLinearRange()
// purpose  : 
//=======================================================================
void KinematicGUI::SetLinearRange(GEOM::GEOM_Contact_ptr aContact,
				  double MinValX, double MaxValX, double MinValY,
				  double MaxValY, double MinValZ, double MaxValZ)
{
  try {
    aContact->SetLinearRange(MinValX, MaxValX, MinValY, MaxValY, MinValZ, MaxValZ);
    myGeom->SetLinearRange(aContact);
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================
// function : SetDisplacement()
// purpose  : 
//=======================================================================
void KinematicGUI::SetDisplacement(GEOM::GEOM_Animation_ptr anAnimation,
				   GEOM::GEOM_Contact_ptr aContact,
				   const GEOM::ListOfDouble& aList)
{
  try {
    anAnimation->SetDisplacement(aContact, aList);
    myGeom->SetDisplacement(anAnimation, aContact);
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : GetNameFromType()
// purpose  : 
//=====================================================================================
TCollection_AsciiString KinematicGUI::GetNameFromType(int type)
{
  TCollection_AsciiString name("");
  switch (type)
    {
    case 0:
      {	name = "Embedding"; break; }
    case 1:
      {	name = "Pivot";	break; }
    case 2:
      {	name = "Slide";	break; }
    case 3:
      {	name = "Sliding Pivot";	break; }
    case 4:
      {	name = "Spherical"; break; }
    case 5:
      {	name = "Plane";	break; }
    case 6:
      {	name = "Annular"; break; }
    case 7:
      {	name = "Rectilinear"; break; }
    case 8:
      {	name = "Ponctual"; break; }
    case 9:
      {	name = "Helicoidal"; break; }
    default:
      {	name = "Contact"; break; }
    }
  return name;
}


//=======================================================================
// function : CreateContact()
// purpose  : 
//=======================================================================
Kinematic_Contact* KinematicGUI::CreateContact(GEOM::GEOM_Contact_ptr aContact)
{
  GEOM::GEOM_Shape_var myShape1 = aContact->GetShape1();
  GEOM::GEOM_Shape_var myShape2 = aContact->GetShape2();
  int Type = aContact->GetType();
  double Step = aContact->GetStep();

  GEOM::ListOfDouble_var aPosList = aContact->GetPosition();
  GEOM::ListOfDouble_var aARList = aContact->GetAngularRange();
  GEOM::ListOfDouble_var aLRList = aContact->GetLinearRange();

  TopoDS_Shape aShape1 = myGeomGUI->GetShapeReader().GetShape(myGeom, myShape1);
  TopoDS_Shape aShape2 = myGeomGUI->GetShapeReader().GetShape(myGeom, myShape2);
  Kinematic_Contact* KContact = new Kinematic_Contact(aShape1, aShape2, Type, Step);

  gp_Pnt Center(aPosList[0], aPosList[1], aPosList[2]);
  gp_Dir aDirX(aPosList[3], aPosList[4], aPosList[5]);
  gp_Dir aDirY(aPosList[6], aPosList[7], aPosList[8]);
  gp_Dir aDirZ(aPosList[9], aPosList[10], aPosList[11]);
  KContact->GetPosition().SetOrigin(Center);
  KContact->GetPosition().SetDirX(aDirX);
  KContact->GetPosition().SetDirY(aDirY);
  KContact->GetPosition().SetDirZ(aDirZ);

  KContact->GetAngularRange().SetRangeX(aARList[0], aARList[1]);
  KContact->GetAngularRange().SetRangeY(aARList[2], aARList[3]);
  KContact->GetAngularRange().SetRangeZ(aARList[4], aARList[5]);

  KContact->GetLinearRange().SetRangeX(aLRList[0], aLRList[1]);
  KContact->GetLinearRange().SetRangeY(aLRList[2], aLRList[3]);
  KContact->GetLinearRange().SetRangeZ(aLRList[4], aLRList[5]);

  return KContact;
}


//=======================================================================
// function : CreateAssembly()
// purpose  : 
//=======================================================================
Kinematic_Assembly* KinematicGUI::CreateAssembly(GEOM::GEOM_Assembly_ptr aAssembly)
{
  int NbContact = aAssembly->NbContacts();
  GEOM::ListOfContact_var aContactList = aAssembly->GetContactList();
  Kinematic_Assembly* KAssembly = new Kinematic_Assembly();

  for(int i = 0; i < NbContact; i++) {
    GEOM::GEOM_Contact_var myContact = aContactList[i];
    Kinematic_Contact* KContact = CreateContact(myContact);
    KAssembly->AddContact(KContact);
  }
  return KAssembly;
}


//=======================================================================
// function : CreateAnimation()
// purpose  : 
//=======================================================================
Kinematic_Animation* KinematicGUI::CreateAnimation(GEOM::GEOM_Animation_ptr aAnimation)
{
  GEOM::GEOM_Shape_var aFrame = aAnimation->GetFrame();
  double Duration = aAnimation->GetDuration();
  int NbSeq = aAnimation->GetNbSeq();

  GEOM::GEOM_Assembly_var myAssembly = aAnimation->GetAssembly();
  Kinematic_Assembly* KAssembly = CreateAssembly(myAssembly);
  TopoDS_Shape myFrame = myGeomGUI->GetShapeReader().GetShape(myGeom, aFrame);

  Kinematic_Animation* KAnimation = new Kinematic_Animation(KAssembly, myFrame, Duration, NbSeq);
  
  GEOM::ListOfContact_var aContactList = myAssembly->GetContactList();
  for(int i = 0; i < myAssembly->NbContacts(); i++) {
    GEOM::GEOM_Contact_var aContact = aContactList[i];
    GEOM::ListOfDouble_var aList = aAnimation->GetDisplacement(aContact);
    list <double> aKList;
    for(int k = 0; k < 12; k++)
      aKList.push_back(aList[k]);
    KAnimation->SetDisplacement(i, aKList);
  }

  return KAnimation;
}


//=====================================================================================
// EXPORTED METHODS
//=====================================================================================
extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {return KinematicGUI::OnGUIEvent(theCommandID, parent);}
}
