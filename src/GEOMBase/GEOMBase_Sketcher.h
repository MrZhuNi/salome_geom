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
//  File   : GEOMBase_Sketcher.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMBASE_SKETCHER_H
#define GEOMBASE_SKETCHER_H

#include "GEOMBase.h"

//=================================================================================
// class    : GEOMBase_Sketcher
// purpose  :
//=================================================================================
class GEOMBase_Sketcher : public QObject
{
  Q_OBJECT /* for QT compatibility */

public :
  GEOMBase_Sketcher();
  ~GEOMBase_Sketcher();

  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent);

  /* Sketcher management */
  void OnSketchSetAngle();
  void OnSketchSetx();
  void OnSketchSety();
  
  void OnSketchDelete();
  void OnSketchClose();
  void OnSketchEnd();

private:
  GEOMBase* myGeomBase;
  GEOMContext* myGeomGUI;
  GEOM::GEOM_Gen_var myGeom;   /* Current Geom Component */
  Sketch mySketcher;

};

#endif
