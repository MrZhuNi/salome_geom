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
//  File   : KinematicGUI_2Sel2Spin1Check.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef KINEMATICGUI_2SEL2SPIN1CHECK_H
#define KINEMATICGUI_2SEL2SPIN1CHECK_H

#include "KinematicGUI_2Sel2Spin1Check_QTD.h"
#include "DlgRef_SpinBox.h"

class KinematicGUI_2Sel2Spin1Check : public KinematicGUI_2Sel2Spin1Check_QTD
{ 
    Q_OBJECT

public:
    KinematicGUI_2Sel2Spin1Check( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KinematicGUI_2Sel2Spin1Check();

    DlgRef_SpinBox* SpinBox_DX;
    DlgRef_SpinBox* SpinBox_DY;

};

#endif // KINEMATICGUI_2SEL2SPIN1CHECK_H
