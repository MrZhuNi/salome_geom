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
//  File   : GeometryGUI_SolidDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef DIALOGBOX_SOLID_H
#define DIALOGBOX_SOLID_H

#include "GeometryGUI_Skeleton.h"
#include "GeometryGUI_1Sel_QTD.h"

//=================================================================================
// class    : GeometryGUI_SolidDlg
// purpose  :
//=================================================================================
class GeometryGUI_SolidDlg : public GeometryGUI_Skeleton
{ 
    Q_OBJECT

public:
    GeometryGUI_SolidDlg(QWidget* parent = 0, const char* name = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~GeometryGUI_SolidDlg();

private:
    void Init(SALOME_Selection* Sel);
    void enterEvent(QEvent * e);

    Handle(GEOM_ShapeTypeFilter) myShellFilter;  /* filter for selection */
    GEOM::GEOM_Gen::ListOfIOR myListShapes;
    bool myOkListShapes;          /* to check when arguments is defined */

    GeometryGUI_1Sel_QTD* GroupSolid;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void SetEditCurrentArgument();
    void LineEditReturnPressed();
    void SelectionIntoArgument();
    void ActivateThisDialog();

};

#endif // DIALOGBOX_SOLID_H
