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
//  File   : GEOMBase_Tools.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMBASE_TOOLS_H
#define GEOMBASE_TOOLS_H

#include "GEOMBase_Context.h"

//=================================================================================
// class    : GEOMBase_Tools
// purpose  :
//=================================================================================
class GEOMBase_Tools : public QObject
{
  Q_OBJECT /* for QT compatibility */

public :
  GEOMBase_Tools();
  ~GEOMBase_Tools();

  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent);

  /* Import and export topology methods */
  bool Import();
  bool Export(); 
  
  void OnEditCopy();
  void OnEditDelete();

private:
  GEOMBase_Context* myGeomGUI;
  GEOM::GEOM_Gen_var myGeom;   /* Current Geom Component */

};

#endif
