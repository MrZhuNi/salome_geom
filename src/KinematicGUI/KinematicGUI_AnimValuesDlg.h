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
//  File   : KinematicGUI_AnimValuesDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef DIALOGBOX_ANIMVALUES_H
#define DIALOGBOX_ANIMVALUES_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_SpinBox.h"

#include "KinematicGUI.h"

#include <qlistbox.h>

//=================================================================================
// class    : KinematicGUI_AnimValuesDlg
// purpose  :
//=================================================================================
class KinematicGUI_AnimValuesDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    KinematicGUI_AnimValuesDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_AnimValuesDlg();

private:
    void Init();
    void enterEvent(QEvent * e);

    KinematicGUI* myKinematicGUI;

    GEOM::GEOM_Animation_var myGeomAnimation;
    GEOM::GEOM_Contact_var myGeomContact;
    GEOM::ListOfDouble_var myList;
    bool myOkAnimation;
    bool myOkContact;

    QGroupBox* GroupBox1;
    QLabel* TextLabel1;
    QLabel* TextLabel2;
    QLabel* TextLabel3;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QLabel* TextLabel6;
    QLabel* TextLabel7;
    QLabel* TextLabel8;
    QLabel* TextLabel9;
    QLabel* TextLabel10;
    QListBox* ListBox1;
    QLineEdit* LineEdit1;
    QPushButton* PushButton1;
    DlgRef_SpinBox* SpinBox1;
    DlgRef_SpinBox* SpinBox2;
    DlgRef_SpinBox* SpinBox3;
    DlgRef_SpinBox* SpinBox4;
    DlgRef_SpinBox* SpinBox5;
    DlgRef_SpinBox* SpinBox6;
    DlgRef_SpinBox* SpinBox7;
    DlgRef_SpinBox* SpinBox8;
    DlgRef_SpinBox* SpinBox9;
    DlgRef_SpinBox* SpinBox10;
    DlgRef_SpinBox* SpinBox11;
    DlgRef_SpinBox* SpinBox12;

private slots:
    void ClickOnOk();
    void ClickOnApply();
    void SetEditCurrentArgument();
    void SelectionIntoArgument();
    void ActivateThisDialog();
    void SetEnabledValues();
    void ValueChangedInSpinBox(double newValue);
    void ClickOnListBox(int Item);

protected:
    QGridLayout* GroupBox1Layout;
    QGridLayout* LayoutD;
    QGridLayout* LayoutC;
    QGridLayout* LayoutB;

};

#endif // DIALOGBOX_CONTACT_H
