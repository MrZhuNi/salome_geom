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
//  File   : MeasureGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "MeasureGUI.h"

#include "MeasureGUI_PropertiesDlg.h"    // Method PROPERTIES
#include "MeasureGUI_CenterMassDlg.h"    // Method CENTER MASS
#include "MeasureGUI_InertiaDlg.h"       // Method INERTIA
#include "MeasureGUI_BndBoxDlg.h"        // Method BNDBOX
#include "MeasureGUI_DistanceDlg.h"      // Method DISTANCE
#include "MeasureGUI_MaxToleranceDlg.h"  // Method MAXTOLERANCE
#include "MeasureGUI_WhatisDlg.h"        // Method WHATIS
#include "MeasureGUI_CheckShape.h"       // Method CHECKSHAPE

//=======================================================================
// function : MeasureGUI()
// purpose  : Constructor
//=======================================================================
MeasureGUI::MeasureGUI() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~MeasureGUI()
// purpose  : Destructor
//=======================================================================
MeasureGUI::~MeasureGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool MeasureGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 601: // PROPERTIES (Length, surface, volume)
      {
	MeasureGUI_PropertiesDlg *aDlg = new MeasureGUI_PropertiesDlg(parent, "", Sel);
	break;
      }
    case 604: // CDG : Center of mass
      {
	MeasureGUI_CenterMassDlg *aDlg = new MeasureGUI_CenterMassDlg(parent, "", this, Sel);
	break;
      }
    case 605: // INERTIA
      {
	MeasureGUI_InertiaDlg *aDlg = new MeasureGUI_InertiaDlg(parent, "", Sel);
	break;
      }
    case 6060: // BOUNDING BOX
      {
	MeasureGUI_BndBoxDlg *aDlg = new MeasureGUI_BndBoxDlg(parent, "", Sel);
	break;
      }
    case 6061: // MIN DISTANCE
      {
	MeasureGUI_DistanceDlg *aDlg = new MeasureGUI_DistanceDlg(parent, "", Sel);
	break;
      }
    case 607: // MAXTOLERANCE
      {
	MeasureGUI_MaxToleranceDlg *aDlg = new MeasureGUI_MaxToleranceDlg(parent, "", Sel);
	break;
      }
    case 608: // WHATIS
      {
	MeasureGUI_WhatisDlg *aDlg = new MeasureGUI_WhatisDlg(parent, "", Sel);
	break;
      }
    case 609: // CHECKSHAPE
     {
       MeasureGUI_CheckShape *aDlg = new MeasureGUI_CheckShape(parent, "", Sel);
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
// function : MakeCDGAndDisplay()
// purpose  :
//=====================================================================================
void MeasureGUI::MakeCDGAndDisplay(GEOM::GEOM_Shape_ptr Shape)
{
  try {
    GEOM::GEOM_Shape_var result = myGeom->MakeCDG(Shape);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
      return;
    }
    result->NameType(tr("GEOM_POINT"));
    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }  
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}
