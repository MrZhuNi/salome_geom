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
#include "KinematicGUI_Anim_QTD.h"

#include "KinematicGUI.h"

#include <qwidget.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qslider.h>
#include <qthread.h>

class MyThread : public QObject, public QThread
{
    Q_OBJECT

public:
    MyThread(Kinematic_Animation* KAnimation = 0, Handle(AIS_InteractiveContext) ic = 0);
    ~MyThread();

private:
    bool myIsActive;
    Handle(AIS_Shape) myAISFrame;
    Handle(AIS_InteractiveContext) myIC;   /* Interactive context */
    gp_Trsf myLoc;

public:
    Kinematic_Animation* myKAnimation;

    int myValue;
    virtual void run();
    void startAnimation();
    void stopAnimation();
    void InitAnim();
    void ClearAnim(int ClearPrs);
    void NextFrame();

signals:
    void frameChanged();
    void stopped();

};


class KinematicGUI_AnimDlg : public KinematicGUI_Skeleton_QTD
{ 
    Q_OBJECT

public:
    KinematicGUI_AnimDlg(QWidget* parent = 0, const char* name = 0, KinematicGUI* theKinematicGUI = 0, SALOME_Selection* Sel = 0, Handle(AIS_InteractiveContext) ic = 0, bool modal = FALSE, WFlags fl = 0);
    ~KinematicGUI_AnimDlg();

private :
    void Init(SALOME_Selection* Sel, Handle(AIS_InteractiveContext) ic);
    void enterEvent(QEvent * e);
    void closeEvent(QCloseEvent* e);

    KinematicGUI* myKinematicGUI;

    MyThread* myAnimator;

    SALOME_Selection* mySelection;      /* User shape selection */
    GEOM::GEOM_Gen_var myGeom;          /* Current GeomI object */
    GEOMBase* myGeomBase;
    GEOMContext* myGeomGUI;  /* Current GeomGUI object */

    KinematicGUI_Anim_QTD* Group1;

private slots:
    void ClickOnCancel();
    void DeactivateActiveDialog();
    void ActivateThisDialog();
    void ClickOnSlider(int newValue);
    void ClickOnFirst();
    void ClickOnPrevious();
    void ClickOnStop();
    void ClickOnPlay();
    void OnNext();
    void ClickOnNext();
    void ClickOnLast();
    void ClickOnShading();
    void ClickOnExport();

};

#endif // KINEMATICGUI_ANIMDLG_H
