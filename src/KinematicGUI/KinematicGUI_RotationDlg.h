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
//  File   : KinematicGUI_RotationDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef DIALOGBOX_ROTATION_H
#define DIALOGBOX_ROTATION_H

#include "GEOMBase_Skeleton.h"
#include "KinematicGUI_3List3Spin.h"

#include "KinematicGUI.h"

//=================================================================================
// class    : KinematicGUI_RotationDlg
// purpose  :
//=================================================================================
class KinematicGUI_RotationDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    KinematicGUI_RotationDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_RotationDlg();

private:
    void Init();
    void InitValues();
    void SetEnabledValues();

    KinematicGUI* myKinematicGUI;

    GEOM::GEOM_Contact_var myGeomContact;
    GEOM::GEOM_Rotation_var myGeomRotation;

    int myRot1;
    int myRot2;
    int myRot3;

    double myVal1;
    double myVal2;
    double myVal3;

    KinematicGUI_3List3Spin* Group1;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void ComboTextChanged();
    void ValueChangedInSpinBox(double newValue);

};

#endif // DIALOGBOX_ROTATION_H
