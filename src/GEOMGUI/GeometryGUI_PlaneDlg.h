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
//  File   : GeometryGUI_PlaneDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_PLANE_H
#define DIALOGBOX_PLANE_H

#include "GeometryGUI_Skeleton.h"
#include "GeometryGUI_2Sel1Spin.h"
#include "GeometryGUI_1Sel4Spin.h"
#include "GeometryGUI_1Sel1Spin.h"

#include "GEOM_FaceFilter.hxx"
#include <gp_Pnt.hxx>

//=================================================================================
// class    : GeometryGUI_PlaneDlg
// purpose  :
//=================================================================================
class GeometryGUI_PlaneDlg : public GeometryGUI_Skeleton
{ 
    Q_OBJECT

public:
    GeometryGUI_PlaneDlg(QWidget* parent = 0, const char* name = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~GeometryGUI_PlaneDlg();

private :
    void Init(SALOME_Selection* Sel);
    void enterEvent(QEvent* e);

    double step;
    int myConstructorId;
    Handle(GEOM_ShapeTypeFilter) myVertexFilter;  /* Filters selection */
    Handle(GEOM_ShapeTypeFilter) myEdgeFilter;
    Handle(GEOM_FaceFilter) myFaceFilter;

    gp_Pnt myPoint1;   /* Point on the plane */

    Standard_Real myDx;
    Standard_Real myDy;
    Standard_Real myDz;
    Standard_Real myTrimSize;

    bool myOkPoint1;   /* true when argument is defined */
    bool myOkDirection;
    bool myOkCoordinates;
    bool myOkPlanarFace;

    GeometryGUI_2Sel1Spin* GroupPointDirection;
    GeometryGUI_1Sel4Spin* GroupPointPlusCoordinates;
    GeometryGUI_1Sel1Spin* GroupFace;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void ActivateThisDialog();
    void SelectionIntoArgument();
    void LineEditReturnPressed();
    void SetEditCurrentArgument();
    void ConstructorsClicked(int constructorId);
    void ValueChangedInSpinBox(double newValue);
    void MakePlaneSimulationAndDisplay(const gp_Pnt& P,
				       const Standard_Real dx,
				       const Standard_Real dy,
				       const Standard_Real dz,
				       const Standard_Real trimSize);

};

#endif // DIALOGBOX_PLANE_H
