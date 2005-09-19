#include "GEOMGUI_Selection.h"

#include "GeometryGUI.h"
#include "GEOM_Displayer.h"

#include <LightApp_DataOwner.h>
#include <SalomeApp_Study.h>

#include <OCCViewer_ViewModel.h>

#include <SUIT_Session.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>
#include <SUIT_ViewModel.h>

#include <SALOMEDSClient_SObject.hxx>
#include <SALOMEDSClient_Study.hxx>

#include <SALOME_Prs.h>
#include <SALOME_InteractiveObject.hxx>

#include <SOCC_Prs.h>
#include <SVTK_Prs.h>
#include <SALOME_Actor.h>

#include <OCCViewer_ViewModel.h>
#include <VTKViewer_ViewModel.h>

#include "GEOMImpl_Types.hxx"

// OCCT Includes
#include <AIS.hxx>
#include <AIS_InteractiveObject.hxx>
#include <AIS_ListOfInteractive.hxx>

// VTK Includes
#include <vtkActorCollection.h>

GEOMGUI_Selection::GEOMGUI_Selection()
{
}

GEOMGUI_Selection::~GEOMGUI_Selection()
{
}

QtxValue GEOMGUI_Selection::globalParam( const QString& p ) const
{
  if ( p == "isOCC" ) return QtxValue( activeViewType() == OCCViewer_Viewer::Type() );
 
  return LightApp_Selection::globalParam( p );
}

QtxValue GEOMGUI_Selection::param( const int ind, const QString& p ) const
{
  if      ( p == "isVisible"   )    return QtxValue( isVisible( ind ) );
  else if ( p == "type"        )    return QtxValue( typeName( ind ) );
  else if ( p == "displaymode" )    return QtxValue( displayMode( ind ) );

  return LightApp_Selection::param( ind, p );
}

QString GEOMGUI_Selection::typeName( const int index ) const
{
  if ( isComponent( index ) )
    return "Component";
  GEOM::GEOM_Object_var anObj = getObject( index );
  if ( !CORBA::is_nil( anObj ) ) {
    const int aGeomType = anObj->GetType();
    if ( aGeomType == GEOM_GROUP )
      return "Group";
    else
      return "Shape";
  }
  return "Unknown";
}

bool GEOMGUI_Selection::isVisible( const int index ) const
{
  GEOM::GEOM_Object_var obj = getObject( index );
  SALOME_View* view = GEOM_Displayer::GetActiveView();
  if ( !CORBA::is_nil( obj ) && view ) {
    Handle(SALOME_InteractiveObject) io = new SALOME_InteractiveObject( entry( index ).latin1(), "GEOM", "TEMP_IO" );
    return view->isVisible( io );
  }
  return false;
}

QString GEOMGUI_Selection::displayMode( const int index ) const
{
  SALOME_View* view = GEOM_Displayer::GetActiveView();
  QString viewType = activeViewType();
  if ( view /*fix for 9320==>*/&& ( viewType == OCCViewer_Viewer::Type() || viewType == VTKViewer_Viewer::Type() ) ) {
    SALOME_Prs* prs = view->CreatePrs( entry( index ) );
    if ( prs ) {
      if ( viewType == OCCViewer_Viewer::Type() ) { // assuming OCC
	SOCC_Prs* occPrs = (SOCC_Prs*) prs;
	AIS_ListOfInteractive lst;
	occPrs->GetObjects( lst );
	if ( lst.Extent() ) {
	  Handle(AIS_InteractiveObject) io = lst.First();
	  if ( !io.IsNull() ) {
	    int dm = io->DisplayMode();
            if ( dm == AIS_WireFrame )
	      return "Wireframe";
	    else if ( dm == AIS_Shaded )
	      return "Shading";
	    else { // return default display mode of AIS_InteractiveContext
	      OCCViewer_Viewer* occViewer = (OCCViewer_Viewer*) SUIT_Session::session()->activeApplication()->desktop(
                                            )->activeWindow()->getViewManager()->getViewModel();
	      Handle(AIS_InteractiveContext) ic = occViewer->getAISContext();
	      dm = ic->DisplayMode();
	      if ( dm == AIS_WireFrame )
		return "Wireframe";
	      else if ( dm == AIS_Shaded )
		return "Shading";
	    }
	  }
	}
      }
      else if ( viewType == VTKViewer_Viewer::Type() ) { // assuming VTK
	SVTK_Prs* vtkPrs = (SVTK_Prs*) prs;
	vtkActorCollection* lst = vtkPrs->GetObjects();
	if ( lst ) {
	  lst->InitTraversal();
	  vtkActor* actor = lst->GetNextActor();
	  if ( actor ) {
	    SALOME_Actor* salActor = dynamic_cast<SALOME_Actor*>( actor );
	    if ( salActor ) {
	      int dm = salActor->getDisplayMode();
	      if ( dm == 0 )
		return "Wireframe";
	      else if ( dm == 1 )
		return "Shading";
	    } // if ( salome actor )
	  } // if ( actor )
	} // if ( lst == vtkPrs->GetObjects() )
      } // if VTK
    }
  }
  return "";
}

bool GEOMGUI_Selection::isComponent( const int index ) const
{
  SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>
    (SUIT_Session::session()->activeApplication()->activeStudy());

  if ( appStudy && index >= 0 && index < count() )  {
    _PTR(Study) study = appStudy->studyDS();
    QString anEntry = entry( index );

    if ( study && !anEntry.isNull() ) {
      _PTR(SObject) aSO( study->FindObjectID( anEntry.latin1() ) );
      if ( aSO && aSO->GetFatherComponent() )
	return aSO->GetFatherComponent()->GetIOR() == aSO->GetIOR();
    }
  }
  return false;
}

GEOM::GEOM_Object_ptr GEOMGUI_Selection::getObject( const int index ) const
{
  SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>
    (SUIT_Session::session()->activeApplication()->activeStudy());

  if (appStudy && index >= 0 && index < count()) {
    _PTR(Study) study = appStudy->studyDS();
    QString anEntry = entry(index);

    if (study && !anEntry.isNull()) {
      _PTR(SObject) aSO (study->FindObjectID(anEntry.latin1()));
      if (aSO) {
        CORBA::Object_var anObj = GeometryGUI::ClientSObjectToObject(aSO);
	return GEOM::GEOM_Object::_narrow(anObj);
      }
    }
  }
  return GEOM::GEOM_Object::_nil();
}
