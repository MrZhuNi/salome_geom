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
//  File   : SketcherGUI_3Spin.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef SKETCHERGUI_3SPIN_H
#define SKETCHERGUI_3SPIN_H

#include "SketcherGUI_3Spin_QTD.h"
#include "DlgRef_SpinBox.h"

class SketcherGUI_3Spin : public SketcherGUI_3Spin_QTD
{ 
    Q_OBJECT

public:
    SketcherGUI_3Spin( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_3Spin();

    DlgRef_SpinBox* SpinBox_DX;
    DlgRef_SpinBox* SpinBox_DY;
    DlgRef_SpinBox* SpinBox_DZ;

};

#endif // SKETCHERGUI_3SPIN_H
