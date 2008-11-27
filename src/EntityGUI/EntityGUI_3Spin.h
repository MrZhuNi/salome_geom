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
//  GEOM GEOMGUI : GUI for Geometry component
//  File   : EntityGUI_3Spin.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 
//
#ifndef ENTITYGUI_3SPIN_H
#define ENTITYGUI_3SPIN_H

#include "GEOM_EntityGUI.hxx"

#include "EntityGUI_3Spin_QTD.h"
#include "DlgRef_SpinBox.h"

class GEOM_ENTITYGUI_EXPORT EntityGUI_3Spin : public EntityGUI_3Spin_QTD
{ 
    Q_OBJECT

public:
    EntityGUI_3Spin( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~EntityGUI_3Spin();

    DlgRef_SpinBox* SpinBox_DX;
    DlgRef_SpinBox* SpinBox_DY;
    DlgRef_SpinBox* SpinBox_DZ;

};

#endif // ENTITYGUI_3SPIN_H
