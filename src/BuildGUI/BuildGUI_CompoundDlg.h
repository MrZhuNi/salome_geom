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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : GEOMBase_CompoundDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM

#ifndef DIALOGBOX_COMPOUND_H
#define DIALOGBOX_COMPOUND_H

#include "GEOM_BuildGUI.hxx"

#include "GEOMBase_Skeleton.h"
#include "DlgRef_1Sel_QTD.h"

//=================================================================================
// class    : BuildGUI_CompoundDlg
// purpose  :
//=================================================================================
class GEOM_BUILDGUI_EXPORT BuildGUI_CompoundDlg : public GEOMBase_Skeleton
{ 
    Q_OBJECT

public:
    BuildGUI_CompoundDlg(GeometryGUI* theGeometryGUI, QWidget* parent = 0,
			 const char* name = 0, bool modal = FALSE, Qt::WindowFlags fl = 0);
    ~BuildGUI_CompoundDlg();

protected:
    // redefined from GEOMBase_Helper
    virtual GEOM::GEOM_IOperations_ptr createOperation();
    virtual bool isValid( QString& msg );
    virtual bool execute( ObjectList& objects );

private:
    void Init();
    void enterEvent(QEvent* e);

    GEOM::ListOfGO myShapes;
    bool           myOkShapes;

    Ui::DlgRef_1Sel_QTD* GroupShapes;
    
private slots:
    void ClickOnOk();
    bool ClickOnApply();
    void ActivateThisDialog();
    void SelectionIntoArgument();
    void SetEditCurrentArgument();
};

#endif // DIALOGBOX_COMPOUND_H
