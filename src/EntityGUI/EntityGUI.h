// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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

// GEOM GEOMGUI : GUI for Geometry component
// File   : EntityGUI.h
// Author : Damien COQUERET, Open CASCADE S.A.S.
//
#ifndef ENTITYGUI_H
#define ENTITYGUI_H

#include <GEOMGUI.h>

#include <SALOMEDSClient.hxx>

#include <AIS_Shape.hxx>
#include <V3d_View.hxx>

class TopoDS_Shape;


//=================================================================================
// class    : EntityGUI
// purpose  :
//=================================================================================
class EntityGUI : public GEOMGUI
{
public :
  EntityGUI( GeometryGUI* );
  ~EntityGUI();

  bool              OnGUIEvent( int, SUIT_Desktop* );
  bool              OnMousePress( QMouseEvent* pe, SUIT_Desktop* parent, SUIT_ViewWindow* theViewWindow );
  
  gp_Pnt            ConvertClickToPoint( int x, int y, Handle(V3d_View) aView );

  void              DisplaySimulationShape( const TopoDS_Shape&, const TopoDS_Shape& ); 
  void              EraseSimulationShape();

  // Methods for sub shapes explode
  bool              SObjectExist( const _PTR(SObject)&, const char* );

public:
  // AIS shape used only during topo/geom simulations
  Handle(AIS_Shape) mySimulationShape1;
  Handle(AIS_Shape) mySimulationShape2;
  
};

#endif // ENTITYGUI_H
