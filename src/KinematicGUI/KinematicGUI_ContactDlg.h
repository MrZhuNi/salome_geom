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
//  File   : KinematicGUI_ContactDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef DIALOGBOX_CONTACT_H
#define DIALOGBOX_CONTACT_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_SpinBox.h"

#include "KinematicGUI.h"

//=================================================================================
// class    : KinematicGUI_ContactDlg
// purpose  :
//=================================================================================
class KinematicGUI_ContactDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    KinematicGUI_ContactDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_ContactDlg();

private:
    void Init();
    void enterEvent(QEvent * e);

    KinematicGUI* myKinematicGUI;

    GEOM::GEOM_Assembly_var myGeomAssembly;
    GEOM::GEOM_Shape_var myGeomShape1;          /* is myShape1 */
    GEOM::GEOM_Shape_var myGeomShape2;          /* is myShape2 */
    bool myOkAssembly;
    bool myOkShape1;
    bool myOkShape2;            /* to check when arguments are defined */
    int myType;
    double myStep;

    QGroupBox* GroupBox1;
    QLabel* TextLabel1;
    QLabel* TextLabel3;
    QLabel* TextLabel2;
    QLabel* TextLabel4;
    QLabel* TextLabel5;
    QPushButton* PushButton1;
    QPushButton* PushButton2;
    QPushButton* PushButton3;
    QLineEdit* LineEdit1;
    QLineEdit* LineEdit2;
    QLineEdit* LineEdit3;
    QComboBox* ComboBox1;
    DlgRef_SpinBox* SpinBox_DX;


private slots:
    void ClickOnOk();
    void ClickOnApply();
    void SetEditCurrentArgument();
    void SelectionIntoArgument();
    void LineEditReturnPressed();
    void ActivateThisDialog();
    void ComboTextChanged();
    void ValueChangedInSpinBox(double newValue);

protected:
    QGridLayout* GroupBox1Layout;
    QGridLayout* LayoutA;
    QGridLayout* LayoutB;
    QGridLayout* LayoutC;

};

#endif // DIALOGBOX_CONTACT_H
