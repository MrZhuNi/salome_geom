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
//  File   : GeometryGUI_BoxDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_BOX_H
#define DIALOGBOX_BOX_H

#include "GeometryGUI_Skeleton.h"
#include "GeometryGUI_2Sel_QTD.h"
#include "GeometryGUI_3Spin.h"

#include <gp_Pnt.hxx>

//=================================================================================
// class    : GeometryGUI_BoxDlg
// purpose  :
//=================================================================================
class GeometryGUI_BoxDlg : public GeometryGUI_Skeleton
{
    Q_OBJECT

public:
    GeometryGUI_BoxDlg(QWidget* parent = 0, const char* name = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~GeometryGUI_BoxDlg();

private :
    void Init(SALOME_Selection* Sel);
    void enterEvent(QEvent* e);
    bool TestBoxDimensions(gp_Pnt P1, gp_Pnt P2);

    double step;
    int myConstructorId;
    Handle(GEOM_ShapeTypeFilter) myVertexFilter;  /* filter for selection */

    gp_Pnt myPoint1;   /* Points containing the vector */ 
    gp_Pnt myPoint2;
    bool myOkPoint1;   /* true when myPoint is defined */
    bool myOkPoint2;

    GeometryGUI_2Sel_QTD* GroupPoints;
    GeometryGUI_3Spin* GroupDimensions;

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

#endif // DIALOGBOX_BOX_H
