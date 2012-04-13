// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef ADVANCEDGUI_PIPETSHAPEDLG_H
#define ADVANCEDGUI_PIPETSHAPEDLG_H

#include <GEOMBase_Skeleton.h>

class DlgRef_1Spin;
class DlgRef_2Spin;
class DlgRef_3Spin;
class DlgRef_3Sel;
class DlgRef_6Sel;
class QCheckBox;
class QLineEdit;
class QLabel;

//=================================================================================
// class    : AdvancedGUI_PipeTShapeDlg
// purpose  :
//=================================================================================
class AdvancedGUI_PipeTShapeDlg: public GEOMBase_Skeleton {
Q_OBJECT

public:
	AdvancedGUI_PipeTShapeDlg(GeometryGUI*, QWidget* = 0);
	~AdvancedGUI_PipeTShapeDlg();

protected:
	// redefined from GEOMBase_Helper
	virtual GEOM::GEOM_IOperations_ptr createOperation();
	virtual bool isValid(QString&);
	virtual bool execute(ObjectList&);
	virtual void restoreSubShapes(SALOMEDS::Study_ptr, SALOMEDS::SObject_ptr);

private:
	void Init();
	void enterEvent(QEvent*);
    void resizeEvent(QResizeEvent *event);
    void updateTshapeScreenshotLabel();
	bool CheckCompatiblePosition(GEOM::GEOM_Object_var theP1, 
                                 GEOM::GEOM_Object_var theP2,
                                 GEOM::GEOM_Object_var theP3, double theTolerance);

private:
	QPixmap imagePipeTShape;
    QLabel* tshapeScreenShotLabel;
	DlgRef_3Spin* MainTubeGroupParams;
	DlgRef_3Spin* IncidentTubeGroupParams;
	DlgRef_2Spin* ChamferGroupParams;
	DlgRef_1Spin* FilletGroupParams;
	DlgRef_6Sel*  JunctionPointsSel;
    QPixmap imageImp;
    QString CssNormal, CssAcceptable, CssRefused;
	QCheckBox* HexMeshCheckBox;
	GEOM::GEOM_Object_var myPoint1, myPoint2, myPoint3;
	bool myOkPoint1, myOkPoint2, myOkPoint3;
	bool isPreview;
	ObjectList pipeTShapeGroupObjects;

private slots:
	void ClickOnOk();
	bool ClickOnApply();
	void ActivateThisDialog();
	void SelectionIntoArgument();
	void SetEditCurrentArgument();
	void DisplayPreview(const bool activate = false, const bool update = true,
			const bool toRemoveFromEngine = true, const double lineWidth = -1,
			const int displayMode = -1, const int color = -1);
	virtual void processPreview();
	void SetPosition(bool);
	void ChamferOrFillet(bool);
	void SetDoubleSpinBoxStep(double);
	void ValueChangedInSpinBox( double );
	void UpdatePicture(QWidget* old, QWidget* now);
    void ApplyNewDimensions();
    bool executeNoCheck( ObjectList& objects );
};

#endif // ADVANCEDGUI_PIPETSHAPEDLG_H
