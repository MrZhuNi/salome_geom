// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

// File   : GEOMGUI_MaterialAction.cxx
// Author : Roman NIKOLAEV, Open CASCADE S.A.S. (roman.nikolaev@opencascade.com)

#include "GEOMGUI.h"
#include "GEOMGUI_MaterialAction.h"
#include "GeometryGUI.h"

#include <GEOM_Constants.h>
#include <GeometryGUI_Operations.h>
#include <LightApp_Application.h>
#include <LightApp_SelectionMgr.h>
#include <LightApp_Study.h>
#include <Material_Model.h>
#include <Material_ResourceMgr.h>
#include <QtxAction.h>
#include <SUIT_Desktop.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_ViewManager.h>
#include <SalomeApp_Application.h>

#include <QMenu>
#include <QSignalMapper>


/*!
 *  GEOMGUI_MaterialAction::GEOMGUI_MaterialAction
 *  Constructor
 */
GEOMGUI_MaterialAction::GEOMGUI_MaterialAction(const QString& text, const QIcon& icon, const QString& menuText,
					       int accel, QObject* parent, bool toggle, const QString& shortcutAction) : 
  QtxAction( text, icon, menuText, accel, parent, toggle , shortcutAction){

  LightApp_Application* anApp = dynamic_cast<LightApp_Application*> ( SUIT_Session::session()->activeApplication() );
  myMaterialMenu = new QMenu();
  
  //connections
  connect(anApp, SIGNAL( preferenceChanged( const QString&, const QString&, const QString& ) ),
	  this, SLOT( trackActionState(const QString&, const QString& , const QString& ) ) );
  connect(myMaterialMenu, SIGNAL( aboutToShow() ), this, SLOT( buildMaterialMenu() ) );
  
  trackActionState( QString( "Geometry" ), QString( "Geometry" ), QString( "predef_materials" ) );
}

/*!
 *  GEOMGUI_MaterialAction::GEOMGUI_MaterialAction
 *  Destructor
 */
GEOMGUI_MaterialAction::~GEOMGUI_MaterialAction() {
}


/*!
 *  GEOMGUI_MaterialAction::buildMaterialMenu
 *  Build material menu immediately before rendering
 */
void GEOMGUI_MaterialAction::buildMaterialMenu(){
  myMaterialMenu->clear();

  LightApp_Application* anApp = dynamic_cast<LightApp_Application*> ( SUIT_Session::session()->activeApplication() );
  if ( !anApp )
    return;

  GeometryGUI* aModule = dynamic_cast<GeometryGUI*>( anApp->activeModule() );
  if( !aModule )
    return;

  SALOME_ListIO lst;
  aModule->getApp()->selectionMgr()->selectedObjects( lst );
  QSignalMapper* signalMapper = new QSignalMapper( myMaterialMenu );

  //Get current material model for the object
  QVariant v;
  if ( anApp->activeViewManager() ) {
    LightApp_Study* aStudy = dynamic_cast<LightApp_Study*>( anApp->activeStudy() );
    if( aStudy ) {
      v = aStudy->getObjectProperty( anApp->activeViewManager()->getGlobalId(), lst.Last()->getEntry(), GEOM::propertyName( GEOM::Material ), QVariant() );
    }
  }
  QString curModel = "";
  if ( v.canConvert<QString>() ) curModel = v.toString();
  // get list of all predefined materials
  QStringList materials = Material_ResourceMgr::resourceMgr()->materials();
  bool found = false;
  foreach ( QString material, materials )
    {
      QAction* menAct = myMaterialMenu->addAction( material );
      connect(menAct, SIGNAL( toggled( bool ) ), signalMapper, SLOT( map() ) );
      signalMapper->setMapping( menAct, material );
      menAct->setCheckable( true );
      // Set checked if this material is current
      Material_Model aModel;
      aModel.fromResources( material );
      if ( !found && aModel.toProperties() == curModel ) {
	menAct->setChecked( true );
	found = true;
      }
    }
  myMaterialMenu->insertAction( myMaterialMenu->addSeparator(), aModule->action(  GEOMOp::OpPredefMaterCustom ) );
  myMaterialMenu->insertSeparator( aModule->action(  GEOMOp::OpPredefMaterCustom ) );
  connect( signalMapper, SIGNAL( mapped( const QString & ) ),
	   aModule, SLOT( OnSetMaterial( const QString & ) ) );
}

/*!
 *  GEOMGUI_MaterialAction::trackActionState
 *  Track state of this action after preferences change: simple action or action with submenu
 */
void GEOMGUI_MaterialAction::trackActionState(const QString& module, const QString& section, const QString& parameter ) {
  if (module == "Geometry" && section == "Geometry" && parameter == "predef_materials" ) {

    bool isPredefMat = SUIT_Session::session()->resourceMgr()->booleanValue( "Geometry", "predef_materials" );
    if (isPredefMat) {
      setMenu( myMaterialMenu );
    } else {
      setMenu( NULL );
    }
  }
}
