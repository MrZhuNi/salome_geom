// Copyright (C) 2015  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

// GEOM GEOMGUI : GUI for Geometry component
// File   : MeasureGUI_ShapeStatisticsDlg.h
// Author : Alexander KOVALEV, Open CASCADE (alexander.kovalev@opencascade.com)
//
#ifndef MEASUREGUI_SHAPESTATISTICSDLG_H
#define MEASUREGUI_SHAPESTATISTICSDLG_H

// GEOM includes
#include <GEOMBase_Helper.h>
#include "GEOM_GenericObjPtr.h"

// Qt includes
#include <QDialog>
#include <QLineEdit>
#include <QPointer>
#include <QGroupBox>

// Qtx includes
#include <QtxIntSpinBox.h>
#include <QtxComboBox.h>

class Plot2d_Histogram;

//==========================================================================
// class    : MeasureGUI_ShapeStatisticsDlg
// purpose  :
//==========================================================================

class MeasureGUI_ShapeStatisticsDlg : public QDialog, public GEOMBase_Helper
{
  Q_OBJECT

public:
  MeasureGUI_ShapeStatisticsDlg( QWidget*, TopoDS_Shape aShape = TopoDS_Shape(), TopAbs_ShapeEnum aSubShapeType = TopAbs_SHAPE );
  ~MeasureGUI_ShapeStatisticsDlg();
  
private slots:
  void                    onEditMainShape();
  void                    clickOnCompute();
  void                    clickOnPlot();
  void                    clickOnCreateGroups();
  void                    clickOnHelp();
  
private:
  void                    fillTypes( bool, bool, bool );
  void                    updateTypes( QList<GEOM::GeomObjPtr> theShapes );
  TopAbs_ShapeEnum        currentType();
  bool                    checkInput();

private:
  SalomeApp_Application*     myApp;
  QPointer<SUIT_ViewWindow>  myViewWindow;
  QLineEdit*                 myEditMainShape;
  QtxComboBox*               myCBTypes;
  std::list<TopoDS_Shape>    myShapes;
  GEOM::GEOM_Object_var      myMainObj;
  QtxIntSpinBox*             myNbIntervals;
  QGroupBox*                 myScalarRangeBox;
  QLineEdit*                 myMin;
  QLineEdit*                 myMax;
  QPushButton*               myButtonPlot;
  QPushButton*               myButtonCreateGr;
  Plot2d_Histogram*          myHistogram;

};

#endif // MEASUREGUI_SHAPESTATISTICSDLG_H
