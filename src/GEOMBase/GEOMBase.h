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
//  File   : GEOMBase.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMBASE_H
#define GEOMBASE_H

#include "GEOMContext.h"
#include "SALOME_Selection.h"

//=================================================================================
// class    : GEOMBase
// purpose  :
//=================================================================================
class GEOMBase : public QObject
{
  Q_OBJECT /* for QT compatibility */
    
public :
  GEOMBase();
  ~GEOMBase();

  bool Display(GEOM::GEOM_Shape_ptr aShape, Standard_CString name = "");
  bool AddInStudy(bool selection = false, const Handle(SALOME_InteractiveObject)& anIO = 0);

  /* Selection and objects management */
  int GetIndex(const TopoDS_Shape& subshape, const TopoDS_Shape& shape, int ShapeType);
  TopoDS_Shape GetShapeFromIOR(QString IOR);
  bool GetTopoFromSelection(SALOME_Selection *Sel, TopoDS_Shape& tds);
  int GetNameOfSelectedIObjects(SALOME_Selection* Sel, QString& aName); 
  bool GetShapeTypeString(const TopoDS_Shape& aShape, Standard_CString& aTypeString);

  /* Convertions */
  GEOM::GEOM_Shape_ptr ConvertIOinGEOMShape(const Handle(SALOME_InteractiveObject)& IO, 
					    Standard_Boolean& testResult);
  Handle(GEOM_AISShape) ConvertIOinGEOMAISShape(const Handle(SALOME_InteractiveObject)& IO,
						Standard_Boolean& testResult,
						bool onlyInActiveView = false); 
  void ConvertListOfIOInListOfIOR(const SALOME_ListIO& aList,
				  GEOM::GEOM_Gen::ListOfIOR& listIOR); 

  /* Geometry */
  bool VertexToPoint(const TopoDS_Shape& S, gp_Pnt& P);

  /* Used just by Plane and Prism */
  bool LinearEdgeExtremities(const TopoDS_Shape& S, gp_Pnt& P1, gp_Pnt& P2);
  void GetBipointDxDyDz(gp_Pnt P1, gp_Pnt P2, double& dx, double& dy, double& dz);

  /* User dialog 1 parameter returned */
  double Parameter(Standard_Boolean& res,
		   const char* aValue1 = 0, const char* aTitle1 = 0,
		   const char* aTitle = 0, const double bottom = -1E6,
		   const double top = +1E6, const int decimals = 6);

  void SetDisplayedObjectList();

  /* Simulation management */
  bool CreateArrowForLinearEdge(const TopoDS_Shape& tds, TopoDS_Shape& ArrowCone);
  void DisplaySimulationShape(const TopoDS_Shape& S); 
  void EraseSimulationShape();

  /* Method used by dialog boxes called when used has entered a name of object in a LineEdit */
  bool SelectionByNameInDialogs(QWidget* aWidget, const QString& userObjectName, SALOME_Selection *Sel);
  bool DefineDlgPosition(QWidget* aDlg, int& x, int& y);

  GEOMContext* myGeomGUI;
  GEOM::GEOM_Gen_var myGeom;   /* Current Geom Component */

};

#endif

