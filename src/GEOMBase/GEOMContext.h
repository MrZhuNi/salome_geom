//  GEOMCONTEXT
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
//  File   : GEOMContext.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef GEOMCONTEXT_H
#define GEOMCONTEXT_H

// SALOME Includes
#include "QAD_Desktop.h"

#include "GEOM_Client.hxx"
#include "GEOM_AISShape.hxx"
#include "GEOM_InteractiveObject.hxx"
#include "GEOM_Actor.h"
#include "GEOM_Sketcher.h"

// Open CASCADE Includes
#include <Quantity_Color.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <OSD_SharedLibrary.hxx>

// QT Includes
#include <qapplication.h>

// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

enum {POINT_METHOD, CURRENT_SKETCH};

//=================================================================================
// class    : GEOMContext
// purpose  :
//=================================================================================
class GEOMContext : public QObject
{
  Q_OBJECT /* for QT compatibility */

public :
  GEOMContext();
  ~GEOMContext();

private :
  QAD_Desktop* myDesktop;
  QAD_Study* myActiveStudy;
  QDialog* myActiveDialogBox; /* Unique active dialog box */

  int myNbGeom; /* Unique name for a geom entity */
  int myState; /* Identify a method */

  GEOM_Client myShapeReader;
  Standard_CString myFatherior;
  AIS_ListOfInteractive myListDisplayedObject;
  Quantity_Color myShadingColor;
  Handle(AIS_Shape) mySimulationShape; /* AIS shape used only during topo/geom simulations */
  Sketch mySketcher;

  bool mySettings_AddInStudy;
  bool mySettings_Copy;

public :
  static GEOMContext* GetOrCreateGeomGUI(QAD_Desktop* desktop);
  static GEOMContext* GetGeomGUI();

  OSD_SharedLibrary myGUILibrary;
  GEOM::GEOM_Gen_var myComponentGeom;
  //GEOM::GEOM_Gen_var* GetComponentGeom(){return myComponentGeom;};

  QAD_Desktop* GetDesktop(){return myDesktop;};
  QAD_Study* GetActiveStudy(){return myActiveStudy;};
  QDialog* GetActiveDialogBox(){return myActiveDialogBox ;}; /* Returns the active DialogBox */
    
  int& GetNbGeom(){return myNbGeom;};
  int& GetState(){return myState;};

  GEOM_Client& GetShapeReader(){return myShapeReader;};
  Standard_CString& GetFatherior(){return myFatherior;};
  AIS_ListOfInteractive& GetListDisplayedObject(){return myListDisplayedObject;};
  Quantity_Color& GetShadingColor(){return myShadingColor;};
  Handle(AIS_Shape) GetSimulationShape(){return mySimulationShape;};
  Sketch& GetSketcher(){return mySketcher;};

  bool& GetSettings_AddInStudy(){return mySettings_AddInStudy;};
  bool& GetSettings_Copy(){return mySettings_Copy;};

  bool LoadLibrary(QString GUILibrary);

  void SetState(int aState);
  void ResetState(); /* Sets myState = -1 a private field to indicate wich method is active */
  void SetActiveDialogBox(QDialog* aDlg);  /* Sets 'myActiveDialogBox' a pointer to the active Dialog Box  */

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

