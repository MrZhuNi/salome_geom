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
//  File   : KinematicGUI_TranslationDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef DIALOGBOX_TRANSLATION_H
#define DIALOGBOX_TRANSLATION_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_3Spin.h"

#include "KinematicGUI.h"

//=================================================================================
// class    : KinematicGUI_TranslationDlg
// purpose  :
//=================================================================================
class KinematicGUI_TranslationDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    KinematicGUI_TranslationDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_TranslationDlg();

private:
    void Init();
    void SetEnabledValues();

    KinematicGUI* myKinematicGUI;

    GEOM::GEOM_Contact_var myGeomContact;
    GEOM::GEOM_Translation_var myGeomTranslation;

    double myValX;
    double myValY;
    double myValZ;

    DlgRef_3Spin* Group1;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void ValueChangedInSpinBox(double newValue);

};

#endif // DIALOGBOX_TRANSLATION_H
