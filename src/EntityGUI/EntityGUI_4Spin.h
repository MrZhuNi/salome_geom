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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : EntityGUI_4Spin.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef ENTITYGUI_4SPIN_H
#define ENTITYGUI_4SPIN_H

#include "GEOM_EntityGUI.hxx"

#include "EntityGUI_4Spin_QTD.h"

class DlgRef_SpinBox;

class GEOM_ENTITYGUI_EXPORT EntityGUI_4Spin : public QWidget, public Ui::EntityGUI_4Spin_QTD
{ 
    Q_OBJECT

public:
    EntityGUI_4Spin( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
    ~EntityGUI_4Spin();

    DlgRef_SpinBox* SpinBox_DX;
    DlgRef_SpinBox* SpinBox_DY;
    DlgRef_SpinBox* SpinBox_DZ;
    DlgRef_SpinBox* SpinBox_DS;

};

#endif // ENTITYGUI_4SPIN_H
