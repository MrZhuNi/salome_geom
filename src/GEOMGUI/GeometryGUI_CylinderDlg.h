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
//  File   : GeometryGUI_CylinderDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_CYLINDER_H
#define DIALOGBOX_CYLINDER_H

#include "GeometryGUI_Skeleton.h"
#include "GeometryGUI_2Sel2Spin.h"
#include "GeometryGUI_2Spin.h"

#include "PrimitiveGUI.h"

#include "GEOM_EdgeFilter.hxx"

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

//=================================================================================
// class    : GeometryGUI_CylinderDlg
// purpose  :
//=================================================================================
class GeometryGUI_CylinderDlg : public GeometryGUI_Skeleton
{ 
    Q_OBJECT

public:
    GeometryGUI_CylinderDlg(QWidget* parent = 0, const char* name = 0, PrimitiveGUI* thePrimitiveGUI = 0, SALOME_Selection* Sel = 0,  bool modal = FALSE, WFlags fl = 0);
    ~GeometryGUI_CylinderDlg();

private:
    void Init();
    void enterEvent(QEvent* e);
    void MakeCylinderSimulationAndDisplay();

    PrimitiveGUI* myPrimitiveGUI;

    double step;
    int myConstructorId;
    Handle(GEOM_ShapeTypeFilter) myVertexFilter;
    Handle(GEOM_EdgeFilter) myEdgeFilter;  /* Filter selection */

    gp_Pnt myPoint1;   /* topology used  */
    gp_Dir myDir;
    bool myOkPoint1;   /* to check when arguments is defined */
    bool myOkDir;

    Standard_Real myRadius;
    Standard_Real myHeight;
    bool myOkRadius;
    bool myOkHeight;

    GeometryGUI_2Sel2Spin* GroupPoints;
    GeometryGUI_2Spin* GroupDimensions;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void ActivateThisDialog();
    void LineEditReturnPressed();
    void SelectionIntoArgument();
    void SetEditCurrentArgument();
    void ConstructorsClicked(int constructorId);
    void ValueChangedInSpinBox(double newValue);

};

#endif // DIALOGBOX_CYLINDER_H
