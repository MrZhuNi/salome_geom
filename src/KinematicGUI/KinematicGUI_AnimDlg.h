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
//  File   : KinematicGUI_AnimDlg.h
//  Author : Damine COQUERET
//  Module : GEOM
//  $Header: 

#ifndef KINEMATICGUI_ANIMDLG_H
#define KINEMATICGUI_ANIMDLG_H

#include "KinematicGUI_Skeleton_QTD.h"

#include "KinematicGUI.h"

#include <qslider.h>
#include <qgroupbox.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qbuttongroup.h>

class KinematicGUI_AnimDlg : public KinematicGUI_Skeleton_QTD
{ 
    Q_OBJECT

public:
    KinematicGUI_AnimDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, Handle(AIS_InteractiveContext) ic = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_AnimDlg();

private :
    void Init();
    void enterEvent(QEvent * e);
    void closeEvent(QCloseEvent* e);

    KinematicGUI* myKinematicGUI;

    SALOME_Selection* mySelection;      /* User shape selection */
    GEOMBase* myGeomBase;
    GEOMContext* myGeomGUI;  /* Current GeomGUI object */

    GEOM::GEOM_Animation_var myGeomAnimation;
    GEOM::GEOM_Contact_var myGeomContact;
    Handle(AIS_InteractiveContext) myIC;   /* Interactive context */
    Handle(AIS_Shape) myAISFrame;
    Handle(AIS_Shape) myAISShape;

    Kinematic_Animation* myKAnimation;
    Kinematic_Contact* myKContact;
    int myType, myValue, myNbSeq;
    double myDuration;
    gp_Trsf myLoc;
    bool myIsActive;
    void NextFrame();
    void InitAnim();
    void ClearAnim(int ClearPrs);

    QGroupBox* GroupBox1;
    QPushButton* PushButton8;
    QRadioButton* CheckButton1;
    QSlider* Slider1;
    QLabel* TextLabel1;
    QPushButton* PushButton1;
    QLineEdit* LineEdit1;
    QPushButton* PushButton4;
    QPushButton* PushButton6;
    QPushButton* PushButton3;
    QPushButton* PushButton5;
    QPushButton* PushButton7;
    QPushButton* PushButton2;

private slots:
    void ClickOnCancel();
    void SetEditCurrentArgument();
    void SelectionIntoArgument();
    void DeactivateActiveDialog();
    void ActivateThisDialog();
    void ClickOnSlider(int newValue);
    void ClickOnFirst();
    void ClickOnPrevious();
    void ClickOnPlay();
    void ClickOnNext();
    void ClickOnLast();
    void ClickOnShading();
    void ClickOnExport();

protected:
    QGridLayout* GroupBox1Layout;
    QGridLayout* LayoutA;
    QGridLayout* LayoutB;
    QGridLayout* LayoutC;
    QGridLayout* LayoutD;

};

#endif // KINEMATICGUI_ANIMDLG_H
