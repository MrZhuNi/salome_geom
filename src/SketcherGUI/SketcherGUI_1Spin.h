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
//  File   : SketcherGUI_1Spin.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef SKETCHERGUI_1SPIN_H
#define SKETCHERGUI_1SPIN_H

#include "SketcherGUI_1Spin_QTD.h"
#include "DlgRef_SpinBox.h"

class SketcherGUI_1Spin : public SketcherGUI_1Spin_QTD
{ 
    Q_OBJECT

public:
    SketcherGUI_1Spin(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
    ~SketcherGUI_1Spin();

    DlgRef_SpinBox* SpinBox_DX;

};

#endif // SKETCHERGUI_1SPIN_H
