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
//  File   : GEOMBase_Context.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMBASE_CONTEXT_H
#define GEOMBASE_CONTEXT_H

// SALOME Includes
#include "QAD_Desktop.h"
#include "QAD_Application.h"

#include "SALOME_Selection.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "GEOM_InteractiveObject.hxx"
#include "GEOM_AISShape.hxx"
#include "GEOM_Actor.h"
#include "GEOM_Sketcher.h"
#include "GEOM_Client.hxx"
#include "GEOM_ShapeTypeFilter.hxx"

// Open CASCADE Includes
#include <AIS_ListOfInteractive.hxx>
#include <gp_Pnt.hxx>
#include <V3d_View.hxx>
#include <Quantity_Color.hxx>
#include <Precision.hxx>

// QT Includes
#include <qapplication.h>

// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)


//=================================================================================
//
//=================================================================================
enum {POINT_METHOD, CURRENT_SKETCH};


//=================================================================================
// class    : GEOMBase_Context
// purpose  :
//=================================================================================
class GEOMBase_Context : public QObject
{
    Q_OBJECT /* for QT compatibility */

public :
    GEOMBase_Context();
    ~GEOMBase_Context();

private :
    int myNbGeom; /* Unique name for a geom entity */
    int myState; /* Identify a method */

    QAD_Desktop* myDesktop;
    QAD_Study* myActiveStudy;
    QDialog* myActiveDialogBox; /* Unique active dialog box */

    GEOM::GEOM_Gen_var myComponentGeom;
    GEOM_Client myShapeReader;
    Standard_CString myFatherior;
    AIS_ListOfInteractive myListDisplayedObject;
    Quantity_Color myShadingColor;

    Sketch mySketcher;

    Handle(AIS_Shape) mySimulationShape; /* AIS shape used only during topo/geom simulations */
    vtkActorCollection* mySimulationActor; /* GEOM Actor used only during topo/geom simulations */

    bool mySettings_AddInStudy;
    bool mySettings_Copy;

public :
    static GEOMBase_Context* GetOrCreateGeomGUI(QAD_Desktop* desktop);
    static GEOMBase_Context* GetGeomGUI();

    static int GetIndex(const TopoDS_Shape& subshape, const TopoDS_Shape& shape, int ShapeType);
    static bool VertexToPoint(const TopoDS_Shape& S, gp_Pnt& P);
    static bool GetShapeTypeString(const TopoDS_Shape& aShape, Standard_CString& aTypeString);
    static bool LinearEdgeExtremities(const TopoDS_Shape& S, gp_Pnt& P1, gp_Pnt& P2);
    static gp_Pnt ConvertClickToPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView);
    /* User dialog 1 parameter returned */
    static double Parameter(Standard_Boolean& res,
			    const char* aValue1 = 0, const char* aTitle1 = 0,
			    const char* aTitle = 0, const double bottom = -1E6,
			    const double top = +1E6, const int decimals = 6);
    void GetBipointDxDyDz(gp_Pnt P1, gp_Pnt P2, double& dx, double& dy, double& dz);

    int& GetNbGeom(){return myNbGeom;};
    int& GetState(){return myState;};
    QAD_Desktop* GetDesktop(){return myDesktop;};
    QAD_Study* GetActiveStudy(){return myActiveStudy;};
    QDialog* GetActiveDialogBox(){return myActiveDialogBox ;}; /* Returns the active DialogBox */
    
    GEOM_Client& GetShapeReader(){return myShapeReader;};
    Standard_CString& GetFatherior(){return myFatherior;};
    AIS_ListOfInteractive& GetListDisplayedObject(){return myListDisplayedObject;};
    Quantity_Color& GetShadingColor(){return myShadingColor;};

    Handle(AIS_Shape)& GetSimulationShape(){return mySimulationShape;};
    bool& GetSettings_AddInStudy(){return mySettings_AddInStudy;};
    bool& GetSettings_Copy(){return mySettings_Copy;};

    Sketch& GetSketcher(){return mySketcher;};

    void SetState(int aState);
    void ResetState(); /* Sets myState = -1 a private field to indicate wich method is active */
    bool AddInStudy(bool selection = false, const Handle(SALOME_InteractiveObject)& anIO = 0);
    void SetActiveDialogBox(QDialog* aDlg);  /* Sets 'myActiveDialogBox' a pointer to the active Dialog Box  */

    bool DefineDlgPosition(QWidget* aDlg, int& x, int& y);
    bool SObjectExist(SALOMEDS::SObject_ptr theFatherObject, const char* IOR);

    /* Selection and objects management */
    TopoDS_Shape GetShapeFromIOR(QString IOR);
    bool GetTopoFromSelection(SALOME_Selection *Sel, TopoDS_Shape& tds);
    int  GetNameOfSelectedIObjects(SALOME_Selection* Sel, QString& aName); 
    GEOM::GEOM_Shape_ptr ConvertIOinGEOMShape(const Handle(SALOME_InteractiveObject)& IO, 
					      Standard_Boolean& testResult);
    Handle(GEOM_AISShape) ConvertIOinGEOMAISShape(const Handle(SALOME_InteractiveObject)& IO,
						  Standard_Boolean& testResult,
						  bool onlyInActiveView = false); 
    Handle(GEOM_AISShape) ConvertIORinGEOMAISShape(const char * IOR,
						   Standard_Boolean& testResult,
						   bool onlyInActiveView = false); 
    GEOM_Actor* ConvertIORinGEOMActor(const char * IOR, Standard_Boolean& testResult,
				      bool onlyInActiveView = false);
    void ConvertListOfIOInListOfIOR(const SALOME_ListIO& aList,
				    GEOM::GEOM_Gen::ListOfIOR& listIOR); 

    /* Method used by dialog boxes called when used has entered a name of object in a LineEdit */
    bool SelectionByNameInDialogs(QWidget* aWidget, const QString& userObjectName, SALOME_Selection *Sel);

    /* Method opening context for any sub shape selection */
    bool PrepareSubShapeSelection(const int SubShapeType, Standard_Integer& returnLocalContextId);
  /* Method opening context for sub shape selection on an argument shape */
    bool PrepareSubShapeSelectionArgumentShape(const TopoDS_Shape& aShape, const int SubShapeType,
					       Standard_Integer& returnLocalContextId);
    /* Define a list of indices of sub shapes selected in a local context */
    bool GetIndexSubShapeSelected(const TopoDS_Shape& ShapeTopo, const int SubShapeType,
				  GEOM::GEOM_Shape::ListOfSubShapeID& ListOfID,
				  Standard_Integer& aLocalContextId, bool& myUseLocalContext);

    void SetDisplayedObjectList();
    bool Display(GEOM::GEOM_Shape_ptr aShape, Standard_CString name = "");

    /* Simulation management */
    bool CreateArrowForLinearEdge(const TopoDS_Shape& tds, TopoDS_Shape& ArrowCone);
    void DisplaySimulationShape(const TopoDS_Shape& S); 
    void EraseSimulationShape();    

    /* Non modal dialog boxes magement */
    void EmitSignalDeactivateDialog();
    void EmitSignalCloseAllDialogs();
    void EmitSignalDefaultStepValueChanged(double newVal);

signals :
    void SignalDeactivateActiveDialog();
    void SignalCloseAllDialogs();
    void SignalDefaultStepValueChanged(double newVal);

};

#endif

