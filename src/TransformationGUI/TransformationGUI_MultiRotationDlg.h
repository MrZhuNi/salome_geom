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
//  File   : TransformationGUI_MultiRotationDlg.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef DIALOGBOX_MULTIROTATION_H
#define DIALOGBOX_MULTIROTATION_H

#include "TransformationGUI.h"

#include "GEOM_EdgeFilter.hxx"
#include "DlgRef_SpinBox.h"

#include <gp_Vec.hxx>
#include <gp_Dir.hxx>

#include <qvariant.h>
#include <qdialog.h>
#include <qvalidator.h>

class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QRadioButton;

//=================================================================================
// class    : TransformationGUI_MultiRotationDlg
// purpose  :
//=================================================================================
class TransformationGUI_MultiRotationDlg : public QDialog
{ 
    Q_OBJECT

public:
    TransformationGUI_MultiRotationDlg( QWidget* parent = 0, const char* name = 0, TransformationGUI* theTransformationGUI = 0, SALOME_Selection* Sel = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TransformationGUI_MultiRotationDlg();

private :
    TransformationGUI* myTransformationGUI;
    GEOM::GEOM_Gen_var        myGeom ;                /* Current Geom object */   
    GEOMBase_Context*                 myGeomGUI ;             /* Current GeomGUI object */ 
    TopoDS_Shape                 mySimulationTopoDs ;    /* Shape used for simulation display */
    SALOME_Selection*            mySelection ;           /* User shape selection */
    TopoDS_Shape                 myBase ;
    GEOM::GEOM_Shape_var               myGeomShape ;           /* is myBase */

    gp_Vec                       myVec ;
    int                          myNbTimes1 ;
    int                          myNbTimes2 ;
    Standard_Real                myAng ;
    Standard_Real                myStep ;
    gp_Dir                       myDir ;
    gp_Pnt                       myLoc ;
   
    bool                         myOkBase ; 
    bool                         myOkDir ;
    bool                         myOkAng ;

    QLineEdit*                   myEditCurrentArgument;  /* Current LineEdit */   
    int                          myConstructorId ;       /* Current constructor id = radio button id */ 
    Handle(GEOM_EdgeFilter)      myEdgeFilter;           /* Filter selection */

    void closeEvent( QCloseEvent* e ) ;
    void enterEvent( QEvent* e);
    void Init( SALOME_Selection* Sel ) ;
    void MakeMultiRotationSimulationAndDisplay() ;

    QButtonGroup* GroupConstructors;
    QRadioButton* Constructor1;
    QRadioButton* Constructor2;

    QGroupBox* GroupButtons;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* buttonApply;

    QGroupBox* GroupC1;
    QPushButton* SelectButtonC1A1;
    QPushButton* SelectButtonC1A2;
    QLabel* TextLabelC1A1;
    QLabel* TextLabelC1A2;
    QLabel* TextLabelC1A3;
    QLineEdit* LineEditC1A1;
    QLineEdit* LineEditC1A2;
    QSpinBox* SpinBox_C1A3;            /* int : Nb times */

    QGroupBox* GroupC2;
    QPushButton* SelectButtonC2A1;
    QPushButton* SelectButtonC2A2;
    QLabel* TextLabelC2A1;
    QLabel* TextLabelC2A2;
    QLabel* TextLabelC2A3;
    QLabel* TextLabelC2A4;
    QLabel* TextLabelC2A5;
    QLabel* TextLabelC2A6;
   
    QLineEdit* LineEditC2A1; 
    QLineEdit* LineEditC2A2; 
    DlgRef_SpinBox* SpinBox_C2A3 ;  /* double : angle       */
    QSpinBox* SpinBox_C2A4;              /* int    : Nb times 1  */ 
    DlgRef_SpinBox* SpinBox_C2A5 ;  /* double : step value  */
    QSpinBox* SpinBox_C2A6;              /* int    : Nb times 2  */ 

    QCheckBox* CheckBoxReverse;

private slots:

    void ConstructorsClicked(int constructorId);
    void ClickOnOk();
    void ClickOnCancel();
    void ClickOnApply();
    void SetEditCurrentArgument() ;
    void SelectionIntoArgument() ;
    void LineEditReturnPressed() ;
    void DeactivateActiveDialog() ;
    void ActivateThisDialog() ;
    void ReverseAngle(int) ;
    void ValueChangedInSpinBox( double newValue ) ; /* for TransformationGUI_SpinBox       */
    void ValueChangedInt( int newIntValue ) ;       /* for QT spin box ! not const ! */

protected:
    QGridLayout* TransformationGUI_MultiRotationDlgLayout;
    QGridLayout* GroupConstructorsLayout;
    QGridLayout* GroupButtonsLayout;
    QGridLayout* GroupC1Layout;
    QGridLayout* GroupC2Layout;
};

#endif // DIALOGBOX_MULTIROTATION_H
