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
//  File   : GeometryGUI_SphereDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_SPHERE_H
#define DIALOGBOX_SPHERE_H

#include "GeometryGUI_Skeleton.h"
#include "GeometryGUI_1Sel1Spin.h"
#include "GeometryGUI_1Spin.h"

#include "PrimitiveGUI.h"

#include <gp_Pnt.hxx>

//=================================================================================
// class    : GeometryGUI_SphereDlg
// purpose  :
//=================================================================================
class GeometryGUI_SphereDlg : public GeometryGUI_Skeleton
{ 
    Q_OBJECT

public:
    GeometryGUI_SphereDlg(QWidget* parent = 0, const char* name = 0, PrimitiveGUI* thePrimitiveGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~GeometryGUI_SphereDlg();

private :
    void Init();
    void enterEvent(QEvent* e);

    PrimitiveGUI* myPrimitiveGUI;

    double step;
    int myConstructorId;
    Handle(GEOM_ShapeTypeFilter) myVertexFilter;  /* filter for selection */

    gp_Pnt myPoint1;   /* Points containing the vector */
    Standard_Real myRadius;
    bool myOkRadius;   /* Are true when myPoint is defined */
    bool myOkPoint1;

    GeometryGUI_1Sel1Spin* GroupPoints;
    GeometryGUI_1Spin* GroupDimensions;

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

#endif // DIALOGBOX_SPHERE_H
