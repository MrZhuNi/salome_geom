//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  GEOM GEOMGUI : GUI for Geometry component
//  File   : PrimitiveGUI_ConeDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//
#ifndef DIALOGBOX_CONE_H
#define DIALOGBOX_CONE_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_2Sel3Spin.h"
#include "DlgRef_3Spin.h"

//=================================================================================
// class    : PrimitiveGUI_ConeDlg
// purpose  :
//=================================================================================
class PrimitiveGUI_ConeDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    PrimitiveGUI_ConeDlg(GeometryGUI* theGeometryGUI, QWidget* parent = 0,
			 const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~PrimitiveGUI_ConeDlg();

protected:
    // redefined from GEOMBase_Helper
    virtual GEOM::GEOM_IOperations_ptr createOperation();
    virtual bool isValid( QString& );
    virtual bool execute( ObjectList& objects );
    virtual void addSubshapesToStudy();
    virtual void closeEvent( QCloseEvent* e );

private:
    void Init();
    void enterEvent(QEvent* e);
    double getRadius1() const;
    double getRadius2() const;
    double getHeight() const;

    GEOM::GEOM_Object_var myPoint, myDir;
    
    DlgRef_2Sel3Spin* GroupPoints;
    DlgRef_3Spin* GroupDimensions;

private slots:
    void ClickOnOk();
    bool ClickOnApply();
    void ClickOnCancel();
    void ActivateThisDialog();
    void DeactivateActiveDialog();
    void LineEditReturnPressed();
    void SelectionIntoArgument();
    void SetEditCurrentArgument();
    void ConstructorsClicked(int);
    void ValueChangedInSpinBox();
};

#endif // DIALOGBOX_CONE_H
