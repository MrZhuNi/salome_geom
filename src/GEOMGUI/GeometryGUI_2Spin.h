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
//  File   : GeometryGUI_2Spin.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMETRYGUI_2SPIN_H
#define GEOMETRYGUI_2SPIN_H

#include "GeometryGUI_2Spin_QTD.h"
#include "GeometryGUI_SpinBox.h"

class GeometryGUI_2Spin : public GeometryGUI_2Spin_QTD
{ 
    Q_OBJECT

public:
    GeometryGUI_2Spin(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~GeometryGUI_2Spin();

    GeometryGUI_SpinBox* SpinBox_DX;
    GeometryGUI_SpinBox* SpinBox_DY;

};

#endif // GEOMETRYGUI_2SPIN_H
