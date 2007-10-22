// GEOM GEOMGUI : GUI for Geometry component
//
// Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License. 
// 
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details. 
// 
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// File   : MeasureGUI.cxx
// Author : Damien COQUERET, Open CASCADE S.A.S.
//

#include "MeasureGUI.h"

#include <GeometryGUI.h>
#include <SUIT_Desktop.h>
#include <SalomeApp_Application.h>

#include "MeasureGUI_PropertiesDlg.h"    // Method PROPERTIES
#include "MeasureGUI_CenterMassDlg.h"    // Method CENTER MASS
#include "MeasureGUI_InertiaDlg.h"       // Method INERTIA
#include "MeasureGUI_BndBoxDlg.h"        // Method BNDBOX
#include "MeasureGUI_DistanceDlg.h"      // Method DISTANCE
#include "MeasureGUI_MaxToleranceDlg.h"  // Method MAXTOLERANCE
#include "MeasureGUI_WhatisDlg.h"        // Method WHATIS
#include "MeasureGUI_CheckShapeDlg.h"    // Method CHECKSHAPE
#include "MeasureGUI_CheckCompoundOfBlocksDlg.h" // Method CHECKCOMPOUND
#include "MeasureGUI_PointDlg.h"         // Method POINTCOORDINATES

#include <QApplication>

//=======================================================================
// function : MeasureGUI()
// purpose  : Constructor
//=======================================================================
MeasureGUI::MeasureGUI( GeometryGUI* parent ) : GEOMGUI( parent )
{
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
bool MeasureGUI::OnGUIEvent( int theCommandID, SUIT_Desktop* parent )
{
  SalomeApp_Application* app = getGeometryGUI()->getApp();
  if ( !app ) return false;

  getGeometryGUI()->EmitSignalDeactivateDialog();

  QDialog* dlg = 0;
  switch ( theCommandID ) {
  case 701:
    dlg = new MeasureGUI_PropertiesDlg( getGeometryGUI(), parent );
    break; // LENGTH, AREA AND VOLUME
  case 702:
    dlg = new MeasureGUI_CenterMassDlg( getGeometryGUI(), parent );
    break; // CENTER MASS
  case 703:
    dlg = new MeasureGUI_InertiaDlg( getGeometryGUI(), parent );
    break; // INERTIA
  case 7041:
    dlg = new MeasureGUI_BndBoxDlg( getGeometryGUI(), parent );
    break; // BOUNDING BOX
  case 7042:
    dlg = new MeasureGUI_DistanceDlg( getGeometryGUI(), parent );
    break; // MIN DISTANCE
  case 705: 
    dlg = new MeasureGUI_MaxToleranceDlg( getGeometryGUI(), parent );
    break; // MAXTOLERANCE
  case 706:
    dlg = new MeasureGUI_WhatisDlg( getGeometryGUI(), parent );
    break; // WHATIS
  case 707:
    dlg = new MeasureGUI_CheckShapeDlg( getGeometryGUI(), parent );
    break; // CHECKSHAPE
  case 7072:
    dlg = new MeasureGUI_CheckCompoundOfBlocksDlg( getGeometryGUI(), parent );
    break; // CHECKCOMPOUND
  case 708:
    dlg = new MeasureGUI_PointDlg( getGeometryGUI(), parent );
    break; // POINT COORDINATES
  default: 
    app->putInfo( tr( "GEOM_PRP_COMMAND" ).arg( theCommandID ) ); 
    break;
  }
  if ( dlg ) {
    dlg->updateGeometry();
    dlg->resize( dlg->minimumSize() );
    dlg->show();
  }
  return true;
}


//=====================================================================================
// EXPORTED METHODS
//=====================================================================================
extern "C"
{
#ifdef WIN32
  __declspec( dllexport )
#endif
  GEOMGUI* GetLibGUI( GeometryGUI* parent )
  {
    return new MeasureGUI( parent );
  }
}
