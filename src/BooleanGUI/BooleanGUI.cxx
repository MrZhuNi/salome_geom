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
#include "BooleanGUI.h"

#include "QAD_Application.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "BooleanGUI_FuseDlg.h"    // Method FUSE
#include "BooleanGUI_CommonDlg.h"  // Method COMMON
#include "BooleanGUI_CutDlg.h"     // Method CUT
#include "BooleanGUI_SectionDlg.h" // Method SECTION

//=======================================================================
// function : BooleanGUI()
// purpose  : Constructor
//=======================================================================
BooleanGUI::BooleanGUI() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~BooleanGUI()
// purpose  : Destructor
//=======================================================================
BooleanGUI::~BooleanGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool BooleanGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 4011: // FUSE
      {
	BooleanGUI_FuseDlg *aDlg = new BooleanGUI_FuseDlg(parent, "", this, Sel);
	break;
      }
    case 4012: // COMMON
      {
	BooleanGUI_CommonDlg *aDlg = new BooleanGUI_CommonDlg(parent, "", this, Sel);
	break;
      }
    case 4013: // CUT
      {
	BooleanGUI_CutDlg *aDlg = new BooleanGUI_CutDlg(parent, "", this, Sel);
	break;
      }
    case 4014: // SECTION
      {
	BooleanGUI_SectionDlg *aDlg = new BooleanGUI_SectionDlg(parent, "", this, Sel);
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
// function : MakeBooleanAndDisplay()
// purpose  : 
//=======================================================================
void BooleanGUI::MakeBooleanAndDisplay(GEOM::GEOM_Shape_ptr Shape1, GEOM::GEOM_Shape_ptr Shape2, const short operation)
{
  try {
    GEOM::GEOM_Shape_ptr result = myGeom->MakeBoolean(Shape1, Shape2, operation);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  

    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, result);
    Standard_CString type;
    myGeomGUI->GetShapeTypeString(S,type);
    result->NameType(type);

    if (myGeomGUI->Display(result, ""))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch (const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}
