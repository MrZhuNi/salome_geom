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
//  File   : RepairGUI_CloseContourDlg.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//
#ifndef DIALOGBOX_CloseContour_H
#define DIALOGBOX_CloseContour_H

#include "GEOMBase_Skeleton.h"
#include "DlgRef_1Sel_Ext.h"

#include <TColStd_IndexedMapOfInteger.hxx>

#include <qradiobutton.h>
#include <qbuttongroup.h>

//=================================================================================
// class    : RepairGUI_CloseContourDlg
// purpose  :
//=================================================================================
class RepairGUI_CloseContourDlg : public GEOMBase_Skeleton
{
    Q_OBJECT

public:
    RepairGUI_CloseContourDlg(GeometryGUI* theGeometryGUI, QWidget* parent = 0,
			      const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~RepairGUI_CloseContourDlg();

protected:
    // redefined from GEOMBase_Helper
    virtual GEOM::GEOM_IOperations_ptr createOperation();
    virtual bool isValid( QString& );
    virtual bool execute( ObjectList& objects );

private:
    void Init();
    void enterEvent(QEvent* e);
    void closeEvent(QCloseEvent* e);
    void initSelection();

    bool getIsByVertex() const;

    GEOM::GEOM_Object_var myObject;
    GEOM::short_array_var myWiresInd;

    DlgRef_1Sel_Ext* GroupPoints;
    QButtonGroup*    myIsVertexGr;
    QLabel*          mySelectWiresLbl;
    QPushButton*     mySelectWiresBtn;
    QLineEdit*       mySelectWiresEdt;

private slots:
    void ClickOnOk();
    bool ClickOnApply();

    void ActivateThisDialog();

    void LineEditReturnPressed();
    void SelectionIntoArgument();
    void SetEditCurrentArgument();
};

#endif // DIALOGBOX_CloseContour_H
