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
//  File   : PrimitiveGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "PrimitiveGUI.h"

#include "QAD_Application.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "GeometryGUI_BoxDlg.h"      // Method BOX
#include "GeometryGUI_CylinderDlg.h" // Method CYLINDER
#include "GeometryGUI_SphereDlg.h"   // Method SPHERE
#include "GeometryGUI_TorusDlg.h"    // Method TORUS
#include "GeometryGUI_ConeDlg.h"     // Method CONE

//=======================================================================
// function : PrimitiveGUI()
// purpose  : Constructor
//=======================================================================
PrimitiveGUI::PrimitiveGUI() :
  QObject()
{
  myGeomGUI = GeometryGUI::GetGeometryGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~PrimitiveGUI()
// purpose  : Destructor
//=======================================================================
PrimitiveGUI::~PrimitiveGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool PrimitiveGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 3021: // BOX
      {
	GeometryGUI_BoxDlg *aDlg = new GeometryGUI_BoxDlg(parent, "", this, Sel);
	break;
      }
    case 3022: // CYLINDER
      {
	GeometryGUI_CylinderDlg *aDlg = new GeometryGUI_CylinderDlg(parent, "", this, Sel);
	break;
      }
    case 3023: // SPHERE
      {
	GeometryGUI_SphereDlg *aDlg = new GeometryGUI_SphereDlg(parent, "", this, Sel);
	break;
      }
    case 3024: // TORUS
      {
	GeometryGUI_TorusDlg *aDlg = new GeometryGUI_TorusDlg(parent, "", this, Sel);
	break;
      }
    case 3025: // CONE
      {
	GeometryGUI_ConeDlg *aDlg = new GeometryGUI_ConeDlg(parent, "", this, Sel);
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
// function : MakeBoxAndDisplay()
// purpose  : 
//=======================================================================
void PrimitiveGUI::MakeBoxAndDisplay(const gp_Pnt P1, const gp_Pnt P2)
{
  try {
    GEOM::GEOM_Shape_var box = myGeom->MakeBox(P1.X(), P1.Y(), P1.Z(), P2.X(), P2.Y(), P2.Z());
    box->NameType(tr("GEOM_BOX"));
    if(myGeomGUI->Display(box, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
}


//=====================================================================================
// function : MakeCylinderAndDisplay()
// purpose  :
//=====================================================================================
void PrimitiveGUI::MakeCylinderAndDisplay(const gp_Pnt BasePoint, const gp_Dir aDir,
					 const double Radius, const double aHeight)
{
  try {
    if(Radius <= Precision::Confusion() || aHeight <= Precision::Confusion())
      return;

    GEOM::PointStruct pstruct = myGeom->MakePointStruct(BasePoint.X(), BasePoint.Y(), BasePoint.Z());
    GEOM::PointStruct d = myGeom->MakePointStruct(aDir.X(), aDir.Y(), aDir.Z());
    GEOM::DirStruct dstruct = myGeom->MakeDirection(d);
    
    GEOM::GEOM_Shape_var result = myGeom->MakeCylinder(pstruct, dstruct, Radius, aHeight);
    if (result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
      return;
    }
    result->NameType(tr("GEOM_CYLINDER"));
    if(myGeomGUI->Display(result, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeSphere()
// purpose  :
//=====================================================================================
void PrimitiveGUI::MakeSphereAndDisplay(const gp_Pnt aCenterPoint, const double aRadius)
{
  try {
    if(aRadius <= Precision::Confusion())
      return;

    GEOM::GEOM_Shape_ptr result = myGeom->MakeSphere(aCenterPoint.X(),aCenterPoint.Y(),aCenterPoint.Z(), aRadius);
    result->NameType(tr("GEOM_SPHERE"));
    if (myGeomGUI->Display(result, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch (const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeTorusAndDisplay()
// purpose  :
//=====================================================================================
void PrimitiveGUI::MakeTorusAndDisplay(const gp_Pnt BasePoint, const gp_Dir aDir,
				      const double Radius1, const double Radius2)
{
  try {
    if(Radius1 <= Precision::Confusion() || Radius2 <= Precision::Confusion())
      return;
    
    GEOM::PointStruct pstruct = myGeom->MakePointStruct(BasePoint.X(), BasePoint.Y(), BasePoint.Z());
    GEOM::PointStruct d = myGeom ->MakePointStruct(aDir.X(), aDir.Y(), aDir.Z());
    GEOM::DirStruct dstruct = myGeom->MakeDirection(d);
    
    GEOM::GEOM_Shape_var result = myGeom->MakeTorus(pstruct, dstruct, Radius1, Radius2);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
      return;
    }
    result->NameType(tr("GEOM_TORUS"));
    if(myGeomGUI->Display(result, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeConeAndDisplay()
// purpose  :
//=====================================================================================
void PrimitiveGUI::MakeConeAndDisplay(const gp_Pnt BasePoint, const gp_Dir aDir,
				     const double Radius1, const double Radius2, const double aHeight)
{
  try {
    if((Radius1 <= Precision::Confusion() && Radius2 <= Precision::Confusion()) || aHeight <= Precision::Confusion())
      return;

    GEOM::PointStruct pstruct = myGeom->MakePointStruct(BasePoint.X(), BasePoint.Y(), BasePoint.Z());
    GEOM::PointStruct d = myGeom->MakePointStruct(aDir.X(), aDir.Y(), aDir.Z());
    GEOM::DirStruct dstruct = myGeom->MakeDirection(d);

    GEOM::GEOM_Shape_var result = myGeom->MakeCone(pstruct, dstruct, Radius1, Radius2, aHeight);  
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
      return;
    }
    result->NameType(tr("GEOM_CONE"));
    if(myGeomGUI->Display(result, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  } 
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}
