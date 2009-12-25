//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// GEOM GEOMGUI : GUI for Geometry component
// File   : GEOMToolsGUI.h
// Author : Damien COQUERET, Open CASCADE S.A.S.
//
#ifndef GEOMTOOLSGUI_H
#define GEOMTOOLSGUI_H

#include "GEOM_ToolsGUI.hxx"

#include <GEOMGUI.h>

#include <SALOMEDSClient.hxx>

class GEOM_Displayer;
class SALOME_View;
class SALOME_ListIO;

#include <QList>

//=================================================================================
// class    : GEOMToolsGUI
// purpose  :
//=================================================================================
class GEOMTOOLSGUI_EXPORT GEOMToolsGUI : public GEOMGUI
{
public:
  GEOMToolsGUI( GeometryGUI* ); // hide constructor to avoid direct creation
  ~GEOMToolsGUI();

  bool         OnGUIEvent( int, SUIT_Desktop* );
  virtual void deactivate();

private:
  // Import and export topology methods
  bool         Import();
  bool         Export();

  void         OnEditCopy();
  void         OnEditDelete();

  void         OnSettingsColor();
  void         OnRename();
  void         OnCheckGeometry();

  // Popup commands
  void         OnAutoColor();
  void         OnDisableAutoColor();
  void         OnColor();
  void         OnTransparency();
  void         OnNbIsos();
  void         OnOpen();
  void         OnSelectOnly(int mode);
  void         OnShowHideChildren( bool );
  void         OnPointMarker();

  // Recursive deletion of object with children
  void         removeObjectWithChildren( _PTR(SObject),
                                         _PTR(Study),
                                         QList<SALOME_View*>,
                                         GEOM_Displayer* );
};

#endif // GEOMTOOLSGUI_H
