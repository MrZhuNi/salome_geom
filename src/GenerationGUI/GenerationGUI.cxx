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
//  File   : GenerationGUI.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "GenerationGUI.h"

#include "GenerationGUI_PrismDlg.h"     // Method PRISM
#include "GenerationGUI_RevolDlg.h"     // Method REVOL
#include "GenerationGUI_FillingDlg.h"   // Method FILLING
#include "GenerationGUI_PipeDlg.h"      // Method PIPE

//=======================================================================
// function : GenerationGUI()
// purpose  : Constructor
//=======================================================================
GenerationGUI::GenerationGUI() :
  QObject()
{
  myGeomGUI = GEOMBase_Context::GetGeomGUI();
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
}


//=======================================================================
// function : ~GenerationGUI()
// purpose  : Destructor
//=======================================================================
GenerationGUI::~GenerationGUI()
{
}


//=======================================================================
// function : OnGUIEvent()
// purpose  : 
//=======================================================================
bool GenerationGUI::OnGUIEvent(int theCommandID, QAD_Desktop* parent)
{
  myGeomGUI->EmitSignalDeactivateDialog();
  SALOME_Selection* Sel = SALOME_Selection::Selection(myGeomGUI->GetActiveStudy()->getSelection());

  switch (theCommandID)
    {
    case 4031: // PRISM
      {	
  	GenerationGUI_PrismDlg *aDlg = new GenerationGUI_PrismDlg(parent, "", this, Sel);
	break;
      }
    case 4032: // REVOL
      {	
  	GenerationGUI_RevolDlg *aDlg = new GenerationGUI_RevolDlg(parent, "", this, Sel);
	break;
      }
    case 4033: // FILLING
      {	
  	GenerationGUI_FillingDlg *aDlg = new GenerationGUI_FillingDlg(parent, "", this, Sel);
	break;
      }
    case 4034: // PIPE
      {	
  	GenerationGUI_PipeDlg *aDlg = new  GenerationGUI_PipeDlg(parent, "", this, Sel);
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
// function : MakePrismAndDisplay()
// purpose  : P1 and P2 is to define a vector for prism
//=======================================================================
void GenerationGUI::MakePrismAndDisplay(GEOM::GEOM_Shape_ptr BaseShape, const gp_Pnt P1, const gp_Pnt P2)
{
  try {
    if(BaseShape->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }

    GEOM::PointStruct PS1 = myGeom->MakePointStruct(P1.X(), P1.Y(), P1.Z());
    GEOM::PointStruct PS2 = myGeom->MakePointStruct(P2.X(), P2.Y(), P2.Z());
    GEOM::GEOM_Shape_ptr result = myGeom->MakePrism(BaseShape, PS1, PS2);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  

    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, result);
    Standard_CString type;
    myGeomGUI->GetShapeTypeString(S,type);
    result->NameType(type);

    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=======================================================================================
// function : MakeRevolutionAndDisplay()
// purpose  :
//=======================================================================================
void GenerationGUI::MakeRevolutionAndDisplay(GEOM::GEOM_Shape_ptr Shape, const gp_Pnt loc, const gp_Dir dir, const Standard_Real revolAngle)
{
  try {
    GEOM::AxisStruct axis = myGeom->MakeAxisStruct(loc.X(), loc.Y(), loc.Z(), dir.X(), dir.Y(), dir.Z());
    GEOM::GEOM_Shape_ptr result = myGeom->MakeRevolution(Shape, axis, revolAngle) ;
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
      return;
    }

    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, result);
    Standard_CString type;
    myGeomGUI->GetShapeTypeString(S,type);
    result->NameType(type);

    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
  }
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakeFillingAndDisplay()
// purpose  :
//=====================================================================================
void GenerationGUI::MakeFillingAndDisplay(GEOM::GEOM_Shape_ptr SectionShape, const short mindeg,
					  const short maxdeg, const double tol3d,
					  const double tol2d, const short nbiter)
{  
  try {
    GEOM::GEOM_Shape_ptr result = myGeom->MakeFilling(SectionShape, mindeg, maxdeg,
						      tol3d, tol2d, nbiter);
    if(result->_is_nil()) {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_NULLSHAPE")); 
      return;
    }  

    TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, result);
    Standard_CString type;
    myGeomGUI->GetShapeTypeString(S,type);
    result->NameType(type);

    if(myGeomGUI->Display(result))
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
    return;  
  } 
  catch(const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}


//=====================================================================================
// function : MakePipeAndDisplay()
// purpose  :
//=====================================================================================
void GenerationGUI::MakePipeAndDisplay(GEOM::GEOM_Shape_ptr aPath, GEOM::GEOM_Shape_ptr aBase) 
{
  try {
    GEOM::GEOM_Shape_ptr result = myGeom->MakePipe(aPath, aBase);
    if (!result->_is_nil() && myGeomGUI->Display(result)) {
      TopoDS_Shape S = myGeomGUI->GetShapeReader().GetShape(myGeom, result);
      Standard_CString type;
      myGeomGUI->GetShapeTypeString(S,type);
      result->NameType(type);

      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE"));
    }
    else {
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT"));
    }
  }
  catch (const SALOME::SALOME_Exception& S_ex) {
    QtCatchCorbaException(S_ex);
  }
  return;
}
