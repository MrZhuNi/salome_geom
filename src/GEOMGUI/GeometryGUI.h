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
//  File   : GeometryGUI.h
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#ifndef GeometryGUI_HeaderFile
#define GeometryGUI_HeaderFile

// SALOME Includes
#include "QAD_Desktop.h"
#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "GEOM_InteractiveObject.hxx"
#include "GEOM_AISShape.hxx"
#include "GEOM_Actor.h"
#include "GEOM_Sketcher.h"

#include "GEOM_Client.hxx"
#include <AIS_ListOfInteractive.hxx>

// Open CASCADE Includes
#include <AIS_InteractiveContext.hxx>
#include <Standard.hxx>
#include <gp_Pnt.hxx>
#include <V3d_View.hxx>
#include <Quantity_Color.hxx>

// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)


//=================================================================================
//
//=================================================================================
  enum  {
    POINT_METHOD,
    CURRENT_SKETCH
  } ;


//=================================================================================
// class    : GeometryGUI
// purpose  :
//=================================================================================
class GeometryGUI : public QObject
{
  Q_OBJECT /* for QT compatibility */

private :
  GEOM::GEOM_Gen_var    myComponentGeom;
  GEOM_Client           myShapeReader;
  QAD_Desktop*          myDesktop;
  QAD_Study*            myActiveStudy;
  QDialog*              myActiveDialogBox; /* Unique active dialog box */
  Handle(AIS_Shape)     mySimulationShape; /* AIS shape used only during topo/geom simulations */
  vtkActorCollection*   mySimulationActor; /* GEOM Actor used only during topo/geom simulations */
  int                   myNbGeom ;         /* Unique name for a geom entity */
  int                   myState ;          /* Identify a method */
  Sketch                mySketcher;

  Quantity_Color        myShadingColor;

  AIS_ListOfInteractive myListDisplayedObject;

  bool mySettings_AddInStudy;
  bool mySettings_Copy;
  Standard_CString myFatherior;


public :

  GeometryGUI();
  ~GeometryGUI();

  static       GeometryGUI* GetOrCreateGeometryGUI( QAD_Desktop* desktop );
  static       GeometryGUI* GetGeometryGUI() ;

  QAD_Study*   GetActiveStudy();
  QAD_Desktop* GetDesktop() ;
  GEOM_Client& GetShapeReader();

  QDialog*     GetActiveDialogBox();               /* Returns the active DialogBox */
  void         SetActiveDialogBox(QDialog* aDlg);  /* Sets 'myActiveDialogBox' a pointer to the active Dialog Box  */

  void         SetState(int aState);
  void         ResetState();                 /* Sets myState = -1 a private field to indicate wich method is active */

  bool AddInStudy( bool selection = false, const Handle(SALOME_InteractiveObject)& anIO = 0 );
  bool DefineDlgPosition(QWidget* aDlg, int& x, int& y) ;
  bool SObjectExist(SALOMEDS::SObject_ptr theFatherObject, const char* IOR);

  static int    GetIndex(const TopoDS_Shape& subshape, const TopoDS_Shape& shape, int ShapeType) ;
  static bool   VertexToPoint(const TopoDS_Shape& S, gp_Pnt& P) ;
  static void   GetBipointDxDyDz( gp_Pnt P1, gp_Pnt P2, double& dx, double& dy, double& dz) ;
  static bool   GetShapeTypeString( const TopoDS_Shape& aShape,  Standard_CString& aTypeString ) ;
  static bool   LinearEdgeExtremities( const TopoDS_Shape& S, gp_Pnt& P1, gp_Pnt& P2) ;
  static gp_Pnt ConvertClickToPoint( Standard_Real x, Standard_Real y, Handle(V3d_View) aView ) ;
  /* User dialog 1 parameter returned */
  static double Parameter( Standard_Boolean& res,
			   const char* aValue1   = 0,
			   const char* aTitle1   = 0,
			   const char* aTitle    = 0,
			   const double bottom   = -1E6,
			   const double top      = +1E6,
			   const int  decimals   = 6 ) ;

  /* Managed by IAPP */
  Standard_EXPORT  bool OnGUIEvent ( int theCommandID, QAD_Desktop* parent) ; 
  Standard_EXPORT  bool OnMousePress ( QMouseEvent* pe, QAD_Desktop* parent, 
					      QAD_StudyFrame* studyFrame );
  Standard_EXPORT  bool OnMouseMove  ( QMouseEvent* pe, QAD_Desktop* parent, 
					      QAD_StudyFrame* studyFrame );
  Standard_EXPORT  bool OnKeyPress   ( QKeyEvent* pe, QAD_Desktop* parent, 
					      QAD_StudyFrame* studyFrame );
  Standard_EXPORT  void activeStudyChanged ( QAD_Desktop* parent ); 
  Standard_EXPORT  bool SetSettings ( QAD_Desktop* parent );
  Standard_EXPORT  void DefinePopup( QString & theContext, 
					    QString & theParent, 
					    QString & theObject );
  Standard_EXPORT  bool CustomPopup ( QAD_Desktop* parent,
					     QPopupMenu* popup,
					     const QString& theContext,
					     const QString& theParent,
					     const QString& theObject );
  Standard_EXPORT  void BuildPresentation(const Handle(SALOME_InteractiveObject)&);


  /* Selection and objects management */
  TopoDS_Shape GetShapeFromIOR( QString IOR );
  bool GetTopoFromSelection(SALOME_Selection *Sel, TopoDS_Shape& tds) ;
  int  GetNameOfSelectedIObjects( SALOME_Selection* Sel, QString& aName ) ; 
  GEOM::GEOM_Shape_ptr ConvertIOinGEOMShape( const Handle(SALOME_InteractiveObject)& IO, 
				       Standard_Boolean& testResult ) ;  

  Handle(GEOM_AISShape) ConvertIOinGEOMAISShape( const Handle(SALOME_InteractiveObject)& IO, 
						 Standard_Boolean& testResult,
						 bool onlyInActiveView = false ) ; 
  Handle(GEOM_AISShape) ConvertIORinGEOMAISShape( const char * IOR, 
						  Standard_Boolean& testResult,
						  bool onlyInActiveView = false ) ; 

  GEOM_Actor* ConvertIORinGEOMActor( const char * IOR, 
				     Standard_Boolean& testResult,
				     bool onlyInActiveView = false ) ;
 
  void ConvertListOfIOInListOfIOR( const SALOME_ListIO& aList, 
				   GEOM::GEOM_Gen::ListOfIOR& listIOR ) ; 

  /* Method used by dialog boxes called when used has entered a name of object in a LineEdit */
  bool SelectionByNameInDialogs( QWidget* aWidget, const QString& userObjectName, SALOME_Selection *Sel ) ;

  /* Method opening context for any sub shape selection */
  bool PrepareSubShapeSelection( const int SubShapeType, 
				 Standard_Integer& returnLocalContextId ) ;

  /* Method opening context for sub shape selection on an argument shape */
  bool PrepareSubShapeSelectionArgumentShape( const TopoDS_Shape& aShape,
					      const int SubShapeType, 
					      Standard_Integer& returnLocalContextId ) ;

  
  /* Define a list of indices of sub shapes selected in a local context */
  bool GetIndexSubShapeSelected( const TopoDS_Shape& ShapeTopo,
				 const int SubShapeType,
				 GEOM::GEOM_Shape::ListOfSubShapeID& ListOfID,
				 Standard_Integer& aLocalContextId,
				 bool& myUseLocalContext ) ;
  
  /* Methods for sub shapes explode */
  bool OnSubShapeGetAll( const TopoDS_Shape& ShapeTopo, const char* ShapeTopoIOR, const int SubShapeType) ;  
  bool OnSubShapeGetSelected( const TopoDS_Shape& ShapeTopo,
			      const char* ShapeTopoIOR,
			      const int SubShapeType,
			      Standard_Integer& aLocalContextId,
			      bool& myUseLocalContext ) ;


  /* Import and export topology methods */
  bool Import();
  bool Export(); 

  void OnEditCopy();
  void OnEditDelete();

  void OnVTKDisplayOnly();
  void OnDisplayOnly();
  void OnDisplayAll(bool onlyPreviousDisplayedObject = false);
  void SetDisplayedObjectList();
  bool Display(GEOM::GEOM_Shape_ptr aShape, Standard_CString name = "");

  /* Simulation management */
  bool CreateArrowForLinearEdge(const TopoDS_Shape& tds, TopoDS_Shape& ArrowCone);
  void DisplaySimulationShape(const TopoDS_Shape& S); 
  void EraseSimulationShape();    


  void MakePointAndDisplay(const double x, const double y, const double z);
  void MakeLineAndDisplay(const gp_Pnt InitPoint, const gp_Pnt LastPoint);
  void MakeCircleAndDisplay(const gp_Pnt CenterPoint, const gp_Dir dir, const Standard_Real Radius);
  void MakeEllipseAndDisplay(const gp_Pnt CenterPoint, const gp_Dir dir, const Standard_Real Major_Radius,
			     const Standard_Real Minor_Radius);
  void MakeArcAndDisplay(gp_Pnt InitPoint, gp_Pnt CirclePoint, gp_Pnt EndPoint);
  void MakeVectorAndDisplay(const gp_Pnt P1, const gp_Pnt P2);
  void MakePlaneAndDisplay(const gp_Pnt P1, const Standard_Real dx, const Standard_Real dy,
			   const Standard_Real dz, const Standard_Real TrimSize);
  void MakeWorkingPlane(const gp_Pnt P, const gp_Dir D);
  

  void MakePrismAndDisplay(GEOM::GEOM_Shape_ptr BaseShape, const gp_Pnt P1, const gp_Pnt P2);
  void MakeRevolutionAndDisplay(GEOM::GEOM_Shape_ptr Shape, const gp_Pnt loc, 
				const gp_Dir dir, Standard_Real revolAngle);
  void MakeFillingAndDisplay(GEOM::GEOM_Shape_ptr SectionShape, const short mindeg, const short maxdeg, 
			     const double tol3d, const double tol2d, const short nbiter);
  void MakePipeAndDisplay(GEOM::GEOM_Shape_ptr aPath, GEOM::GEOM_Shape_ptr aBase);


  void MakeLinearEdgeAndDisplay(const gp_Pnt P1, const gp_Pnt P2);
  void MakeWireAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR);
  void MakeFaceAndDisplay(GEOM::GEOM_Shape_ptr aWire, const Standard_Boolean wantPlanar);
  void MakeShellAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR);
  void MakeSolidAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR);
  void MakeCompoundAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR);


  /* Remove faces in a shape */
  bool OnSuppressFaces( const TopoDS_Shape& ShapeTopo,
			const char* ShapeTopoIOR,
			const Standard_Integer& aLocalContextId,
			bool& myUseLocalContext ) ;
  /* Remove an hole in a topology  (ListOfIdEndFace may be an empty list ) */  
  bool OnSuppressHole( const char* ShapeTopoIOR,
		       const GEOM::GEOM_Shape::ListOfSubShapeID& ListOfIdFace,
		       const GEOM::GEOM_Shape::ListOfSubShapeID& ListOfIdWire,
		       const GEOM::GEOM_Shape::ListOfSubShapeID& ListOfIdEndFace ) ;  
  /* Remove one or more holes in a face or a shell */
  bool OnSuppressHolesInFaceOrShell( const char* ShapeTopoIOR,
				     const GEOM::GEOM_Shape::ListOfSubShapeID& ListOfIdWires ) ;
  /* Create a face corresponding to a hole on a shape */
  bool OnFillingHole( const TopoDS_Shape& MainShape,
		      const char* ShapeTopoIOR,
		      const Standard_Integer& aLocalContextId,
		      bool& myUseLocalContext ) ;


  void MakeTranslationAndDisplay(GEOM::GEOM_Shape_ptr Shape, const gp_Vec V);
  void MakeRotationAndDisplay(GEOM::GEOM_Shape_ptr Shape, const gp_Pnt loc,
			      const gp_Dir dir, const Standard_Real angle);
  void MakeMirrorAndDisplay(GEOM::GEOM_Shape_ptr Shape1, GEOM::GEOM_Shape_ptr Shape2);
  void MakeScaleAndDisplay(GEOM::GEOM_Shape_ptr Shape, const gp_Pnt centralPoint,
			   const Standard_Real factor);
  void MakeMultiTranslation1DAndDisplay(GEOM::GEOM_Shape_ptr Shape,
					const gp_Dir Dir, const double Step, const short NbTimes);
  void MakeMultiTranslation2DAndDisplay(GEOM::GEOM_Shape_ptr Shape,
					const gp_Dir Dir1, const double Step1, const short NbTimes1,
					const gp_Dir Dir2, const double Step2, const short NbTimes2);
  void MakeMultiRotation1DAndDisplay(GEOM::GEOM_Shape_ptr Shape,
				     const gp_Dir Dir, const gp_Pnt Loc, const short NbTimes);
  void MakeMultiRotation2DAndDisplay(GEOM::GEOM_Shape_ptr Shape,
				     const gp_Dir Dir, const gp_Pnt Loc, const double Ang,
				     const short NbTimes1, const double Step, const short NbTimes2);


  void MakePartitionAndDisplay(const GEOM::GEOM_Gen::ListOfIOR& listShapesIOR,
			       const GEOM::GEOM_Gen::ListOfIOR& listToolsIOR,
			       const GEOM::GEOM_Gen::ListOfIOR& listKeepInsIOR,
			       const GEOM::GEOM_Gen::ListOfIOR& listRemoveInsIOR,
			       const GEOM::shape_type limit ) ;
  void Archimede(const Handle(SALOME_InteractiveObject)& IO, const double aWeight,
		  const double aWaterDensity, const double aMeshingDeflection);
  /* Method for Fillet */
  bool OnFilletGetAll(const TopoDS_Shape& ShapeTopo, const double Radius, const int SubShapeType, const char* ShapeTopoIOR) ;
  bool OnFilletGetSelected(const TopoDS_Shape& ShapeTopo,
			   const char* ShapeTopoIOR,
			   const double Radius,
			   const int SubShapeType,
			   Standard_Integer& aLocalContextId,
			   bool& myUseLocalContext );
  /* Methods for Chamfer */
  bool OnChamferGetAll(const TopoDS_Shape& ShapeTopo, const double D1, const double D2, const int SubShapeType, const char* ShapeTopoIOR) ;
  bool OnChamferGetSelected(const TopoDS_Shape& ShapeTopo,
			    const char* ShapeTopoIOR,
			    const double D1, const double D2, const int SubShapeType,
			    Standard_Integer& aLocalContextId,
			    bool& myUseLocalContext ) ;


  void MakeSewingAndDisplay(GEOM::GEOM_Gen::ListOfIOR& listShapesIOR, 
			    const Standard_Real precision);
  void MakeOrientationChangeAndDisplay(GEOM::GEOM_Shape_ptr Shape);


  void MakeCDGAndDisplay( GEOM::GEOM_Shape_ptr Shape );
 
  /* Sketcher management */
  void OnSketchSegment();
  void OnSketchArc();  
  void OnSketchSetAngle();
  void OnSketchSetx();
  void OnSketchSety();
  
  void OnSketchDelete();
  void OnSketchClose();
  void OnSketchEnd();
  
  void OnSketchOptionsOnoffangledimension();
  void OnSketchOptionsOnofflengthdimension();
  void OnSketchOptionsOnoffradiusdimension();
  void OnSketchOptionsOnoffxdimension();
  void OnSketchOptionsOnoffydimension();
  
  void OnSettingsNoconstraint();
  void OnSettingsPerpendicular();
  void OnSettingsTangent();

  /* Non modal dialog boxes magement */
  void EmitSignalDeactivateDialog() ;
  void EmitSignalCloseAllDialogs() ;

signals:
  void SignalDeactivateActiveDialog() ;
  void SignalCloseAllDialogs() ;
  void SignalDefaultStepValueChanged( double newVal ) ;
};

#endif

