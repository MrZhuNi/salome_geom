// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "GEOMGUI_AnnotationMgr.h"

#include <GEOMGUI_AnnotationAttrs.h>
#include <GEOM_Annotation.hxx>
#include <GEOM_Client.hxx>
#include <GEOM_Displayer.h>
#include <GeometryGUI.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <SALOME_Prs.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_Desktop.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>

#include <SOCC_Prs.h>
#include <SOCC_ViewModel.h>

#include <Quantity_Color.hxx>
#include <TCollection_AsciiString.hxx>

#include <TopoDS_Shape.hxx>
#include <gp_Ax3.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <QFont>
#include <QColor>


GEOMGUI_AnnotationMgr::GEOMGUI_AnnotationMgr( SalomeApp_Application* theApplication )
: myApplication( theApplication )
{
}

QString GEOMGUI_AnnotationMgr::GetEntrySeparator()
{
  return "_annotation:";
}

//================================================================
// Function : CreatePresentation
// Purpose  :
//================================================================
SALOME_Prs* GEOMGUI_AnnotationMgr::CreatePresentation( const GEOMGUI_AnnotationAttrs::Properties& theProperty,
                                                       GEOM::GEOM_Object_ptr theObject,
                                                       const QString& theEntry )
{
  Handle ( GEOM_Annotation ) aPresentation = new GEOM_Annotation();
  if ( !theEntry.isEmpty() ) {
    // owner should be set to provide selection mechanizm
    Handle( SALOME_InteractiveObject ) anIO = new SALOME_InteractiveObject();
    anIO->setEntry( theEntry.toLatin1().constData() );
    aPresentation->SetOwner( anIO );
  }

  //aPresentation->SetOwner( new SALOME_InteractiveObject( getEntry( theObject ).c_str(), "GEOM", getName( theObject ).c_str() ) );

  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  const QFont  aFont      = aResMgr->fontValue( "Geometry", "shape_annotation_font", QFont( "Y14.5M-2009", 24 ) );
  const QColor aFontColor = aResMgr->colorValue( "Geometry", "shape_annotation_font_color", QColor( 255, 255, 255 ) );
  const QColor aLineColor = aResMgr->colorValue( "Geometry", "shape_annotation_line_color", QColor( 255, 255, 255 ) );
  const double aLineWidth = aResMgr->doubleValue( "Geometry", "shape_annotation_line_width", 1.0 );
  const int aLineStyle    = aResMgr->integerValue( "Geometry", "shape_annotation_line_style", 0 );
  const bool isAutoHide   = aResMgr->booleanValue( "Geometry", "shape_annotation_autohide", false );

  const Quantity_Color aOcctFontColor( aFontColor.redF(), aFontColor.greenF(), aFontColor.blueF(), Quantity_TOC_RGB );
  const Quantity_Color aOcctLineColor( aLineColor.redF(), aLineColor.greenF(), aLineColor.blueF(), Quantity_TOC_RGB );
  const Standard_Real aFontHeight = aFont.pixelSize() != -1 ? aFont.pixelSize() : aFont.pointSize();

  aPresentation->SetFont( TCollection_AsciiString( aFont.family().toLatin1().data() ) );
  aPresentation->SetTextHeight( aFontHeight );
  aPresentation->SetTextColor( Quantity_Color( aFontColor.redF(), aFontColor.greenF(), aFontColor.blueF(), Quantity_TOC_RGB ) );
  aPresentation->SetLineColor( Quantity_Color( aLineColor.redF(), aLineColor.greenF(), aLineColor.blueF(), Quantity_TOC_RGB ) );
  aPresentation->SetLineWidth( aLineWidth );
  aPresentation->SetLineStyle( static_cast<Aspect_TypeOfLine>( aLineStyle ) );
  aPresentation->SetAutoHide( isAutoHide ? Standard_True : Standard_False );
  aPresentation->SetIsScreenFixed( theProperty.IsScreenFixed );

  TopoDS_Shape aShape = GEOM_Client::get_client().GetShape( GeometryGUI::GetGeomGen(), theObject );
  //TopoDS_Shape aShape;
  //GEOMBase::GetShape( theObject.get(), aShape );
  gp_Ax3 aShapeLCS = gp_Ax3().Transformed( aShape.Location().Transformation() );
  GEOMGUI_AnnotationAttrs::SetupPresentation( aPresentation, theProperty, aShapeLCS );

  // add Prs to preview
  SUIT_ViewWindow* vw = getApplication()->desktop()->activeWindow();
  SOCC_Prs* aPrs =
      dynamic_cast<SOCC_Prs*>( ( ( SOCC_Viewer* ) ( vw->getViewManager()->getViewModel() ) )->CreatePrs(
          0 ) );

  if ( aPrs )
    aPrs->AddObject( aPresentation );

  return aPrs;
}

bool GEOMGUI_AnnotationMgr::IsDisplayed( const QString& theEntry, const int theIndex, SALOME_View* theView ) const
{
  SALOME_View* aView = viewOrActiveView( theView );
  if ( !aView || !myVisualized.contains( aView ) )
    return false;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  if ( !anEntryToAnnotation.contains( theEntry ) )
    return false;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];
  if ( !anAnnotationToPrs.contains( theIndex ) )
    return false;

  return true;
}

//=======================================================================
// function : GEOMGUI_AnnotationMgr::Display
// purpose  : Displays annotation shape presentation in view. It creates an annotation presentation
// and stores it in an internal container
//=======================================================================
void GEOMGUI_AnnotationMgr::Display( const QString& theEntry, const int theIndex, SALOME_View* theView )
{
  if ( IsDisplayed( theEntry, theIndex ) )
    return;

  SALOME_View* aView = viewOrActiveView( theView );
  if ( !aView )
    return;

  GEOMGUI_AnnotationAttrs::Properties aProperty;
  GEOM::GEOM_Object_ptr anObject;
  getObject( theEntry, theIndex, anObject, aProperty );

  // display presentation in the viewer
  QString anEntry = QString("%1%2%3").arg(theEntry).arg(GetEntrySeparator()).arg(theIndex);
  SALOME_Prs* aPrs = CreatePresentation( aProperty, anObject, anEntry );
  aView->Display( getDisplayer(), aPrs );
  getDisplayer()->UpdateViewer();

  EntryToAnnotations anEntryToMap;
  if ( myVisualized.contains( aView ) )
    anEntryToMap = myVisualized[aView];

  // store displayed parameters to an internal container
  AnnotationToPrs anAnnotationToPrsMap;
  if ( anEntryToMap.contains( theEntry ) )
    anAnnotationToPrsMap = anEntryToMap[theEntry];
  anAnnotationToPrsMap[theIndex] = aPrs;
  anEntryToMap[theEntry] = anAnnotationToPrsMap;
  myVisualized[aView] = anEntryToMap;

  // change persistent for the entry: set visible state in true for indices which presentations are shown
  storeVisibleState( theEntry, theView, theIndex );
}

void GEOMGUI_AnnotationMgr::Erase( const QString& theEntry, const int theIndex, SALOME_View* theView )
{
  SALOME_View* aView = viewOrActiveView( theView );
  if ( !aView )
    return;

  if ( !myVisualized.contains( aView ) )
    return;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  if ( !anEntryToAnnotation.contains( theEntry ) )
    return;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];
  if ( !anAnnotationToPrs.contains( theIndex ) )
    return;

  // erase presentation from the viewer
  SALOME_Prs* aPrs = anAnnotationToPrs[theIndex];
  aView->Erase( getDisplayer(), aPrs );
  getDisplayer()->UpdateViewer();

  // remove displayed parameters from an internal container
  anAnnotationToPrs.remove( theIndex );
  anEntryToAnnotation[theEntry] = anAnnotationToPrs;
  if (anAnnotationToPrs.isEmpty()) {
    anEntryToAnnotation.remove( theEntry );
  }
  else {
    anEntryToAnnotation[theEntry] = anAnnotationToPrs;
  }
  myVisualized[aView] = anEntryToAnnotation;

  // change persistent for the entry: set visible state in true for indices which presentations are shown
  storeVisibleState( theEntry, theView, theIndex );
}

void GEOMGUI_AnnotationMgr::DisplayVisibleAnnotations( const QString& theEntry, SALOME_View* theView )
{
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );
  if ( !aShapeAnnotations.IsNull() ) {
    const int aCount = aShapeAnnotations->GetNbAnnotation();
    for ( int anIndex = 0; anIndex < aCount; ++anIndex )
    {
      if ( aShapeAnnotations->GetIsVisible( anIndex ) )
        Display( theEntry, anIndex, theView );
    }
  }
}

void GEOMGUI_AnnotationMgr::EraseVisibleAnnotations( const QString& theEntry, SALOME_View* theView )
{
  SALOME_View* aView = viewOrActiveView( theView );
  if ( !myVisualized.contains( aView ) )
    return;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  if ( !anEntryToAnnotation.contains( theEntry ) )
    return;
  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];

  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );

  const int aCount = aShapeAnnotations->GetNbAnnotation();
  for ( int anIndex = 0; anIndex < aCount; ++anIndex )
  {
    if ( !anAnnotationToPrs.contains( anIndex ) )
      continue;

    // erase presentation from the viewer
    SALOME_Prs* aPrs = anAnnotationToPrs[anIndex];
    aView->Erase( getDisplayer(), aPrs );
  }
  getDisplayer()->UpdateViewer();
  anEntryToAnnotation.remove( theEntry );
  myVisualized[aView] = anEntryToAnnotation;
}

Handle(SALOME_InteractiveObject) GEOMGUI_AnnotationMgr::FindInteractiveObject( const QString& theEntry,
                                                                               const int theIndex,
                                                                               SALOME_View* theView ) const
{
  Handle(SALOME_InteractiveObject) anIO;

  SALOME_View* aView = viewOrActiveView( theView );
  if ( !myVisualized.contains( aView ) )
    return anIO;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  if ( !anEntryToAnnotation.contains( theEntry ) )
    return anIO;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];
  if ( !anAnnotationToPrs.contains(theIndex) )
    return anIO;

  SALOME_Prs* aPrs = anAnnotationToPrs[theIndex];
  SOCC_Prs* anOCCPrs = dynamic_cast<SOCC_Prs*>( aPrs );
  if ( !anOCCPrs )
    return anIO;

  AIS_ListOfInteractive anIOs;
  anOCCPrs->GetObjects( anIOs );
  AIS_ListIteratorOfListOfInteractive anIter( anIOs );
  for ( ; anIter.More() && anIO.IsNull(); anIter.Next() ) {
    Handle(AIS_InteractiveObject) aPrs = anIter.Value();
    if ( aPrs->GetOwner() )
      anIO = Handle(SALOME_InteractiveObject)::DownCast(aPrs->GetOwner());
  }
  return anIO;
}

int GEOMGUI_AnnotationMgr::FindAnnotationIndex( Handle(SALOME_InteractiveObject) theIO,
                                                SALOME_View* theView )
{
  int anIndex = -1;

  SALOME_View* aView = viewOrActiveView( theView );
  if ( !myVisualized.contains( aView ) )
    return anIndex;

  QString anEntry = theIO->getEntry();

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  if ( !anEntryToAnnotation.contains( anEntry ) )
    return anIndex;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[anEntry];
  //typedef QMap<int, SALOME_Prs*> AnnotationToPrs;
  AnnotationToPrs::const_iterator anIt = anAnnotationToPrs.begin(),
                                  aLast = anAnnotationToPrs.end();
  for (; anIt != aLast && anIndex < 0; anIt++) {
    SALOME_Prs* aPrs = anIt.value();
    SOCC_Prs* anOCCPrs = dynamic_cast<SOCC_Prs*>( aPrs );
    if ( !anOCCPrs )
      continue;

    AIS_ListOfInteractive anIOs;
    anOCCPrs->GetObjects( anIOs );
    AIS_ListIteratorOfListOfInteractive anIter( anIOs );
    for ( ; anIter.More() && anIndex < 0; anIter.Next() ) {
      Handle(AIS_InteractiveObject) aPrs = anIter.Value();
      if ( aPrs->GetOwner() ) {
        Handle(SALOME_InteractiveObject) aPrsOwner = Handle(SALOME_InteractiveObject)::DownCast(aPrs->GetOwner());
        if ( aPrsOwner == theIO )
          anIndex = anIt.key();
      }
    }
  }

  return anIndex;
}

//=======================================================================
// function : GEOMGUI_AnnotationMgr::RemoveView
// purpose  : 
//=======================================================================
void GEOMGUI_AnnotationMgr::RemoveView( SALOME_View* theView )
{
  if ( !theView && myVisualized.contains( theView ) )
    myVisualized.remove( theView );
}

QString GEOMGUI_AnnotationMgr::getDisplayedIndicesInfo( const QString& theEntry, SALOME_View* theView ) const
{
  QString aDisplayedIndices;

  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  if ( !aSObj )
  {
    return aDisplayedIndices;
  }
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );
  if ( !aShapeAnnotations.IsNull() )
  {
    const int aCount = aShapeAnnotations->GetNbAnnotation();
    QStringList anIndices;
    for ( int anIndex = 0; anIndex < aCount; ++anIndex )
    {
      if (IsDisplayed( theEntry, anIndex, theView ) )
        anIndices.append( QString::number(anIndex) );
    }
    aDisplayedIndices = anIndices.join(";");
  }
  return aDisplayedIndices;
}

void GEOMGUI_AnnotationMgr::setDisplayedIndicesInfo( const QString& theEntry, SALOME_View* theView,
                                                     const QString theIndicesInfo )
{
  if ( theIndicesInfo.isEmpty() )
    return;

  QStringList anIndices = theIndicesInfo.split( ";" );
  for ( int i = 0, aCount = anIndices.size(); i < aCount; i++ ) {
    Display( theEntry, anIndices[i].toInt(), theView );
  }
}

GEOM_Displayer* GEOMGUI_AnnotationMgr::getDisplayer() const
{
  LightApp_Module* aModule = dynamic_cast<LightApp_Module*>( getApplication()->activeModule() );
  return dynamic_cast<GEOM_Displayer*>( aModule->displayer() );
}

SALOME_View* GEOMGUI_AnnotationMgr::viewOrActiveView(SALOME_View* theView) const
{
  SALOME_View* aView = theView;
  if ( !aView ) {
    SalomeApp_Application* anApp = getApplication();
    SUIT_ViewWindow* anActiveWindow = anApp->desktop()->activeWindow();
    if (anActiveWindow)
      aView = dynamic_cast<SALOME_View*>(anActiveWindow->getViewManager()->getViewModel());
  }
  return aView;
}

void GEOMGUI_AnnotationMgr::getObject( const QString& theEntry, const int theIndex,
                                       GEOM::GEOM_Object_ptr& theObject,
                                       GEOMGUI_AnnotationAttrs::Properties& theProperty )
{
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );
  if ( !aShapeAnnotations.IsNull() ) {
    aShapeAnnotations->GetProperties( theIndex, theProperty );

    theObject = GEOM::GEOM_Object::_narrow( GeometryGUI::ClientSObjectToObject(aSObj) );
  }
}

void GEOMGUI_AnnotationMgr::storeVisibleState( const QString& theEntry, SALOME_View* theView, const int theIndex )
{
  SALOME_View* aView = viewOrActiveView( theView );
  if ( !aView || !myVisualized.contains( aView ) )
    return;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];
  AnnotationToPrs anAnnotationToPrs;
  if ( anEntryToAnnotation.contains( theEntry ) )
    anAnnotationToPrs = anEntryToAnnotation[theEntry];


  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );
  if ( !aShapeAnnotations.IsNull() ) {
    
    bool aVisible = anAnnotationToPrs.contains( theIndex );
    aShapeAnnotations->SetIsVisible( theIndex, aVisible );
  }
}


//=======================================================================
// function : GEOMGUI_AnnotationMgr::getEntry
// purpose  : 
//=======================================================================
std::string GEOMGUI_AnnotationMgr::getEntry( const GEOM::GEOM_Object_ptr theObject )
{
  SUIT_Session* session = SUIT_Session::session();
  SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( session->activeApplication() );
  if ( app )
  {
    CORBA::String_var IOR = app->orb()->object_to_string( theObject );
    if ( strcmp(IOR.in(), "") != 0 )
    {
      SalomeApp_Study* study = ( SalomeApp_Study* )app->activeStudy();
      _PTR(SObject) SO ( study->studyDS()->FindObjectIOR( std::string(IOR) ) );
      if ( SO )
        return SO->GetID();
    }
  }
  return std::string();
}

//=======================================================================
// function : GEOMGUI_AnnotationMgr::getName
// purpose  : 
//=======================================================================
std::string GEOMGUI_AnnotationMgr::getName( const GEOM::GEOM_Object_ptr theObject )
{
  SUIT_Session* session = SUIT_Session::session();
  SalomeApp_Application* app = dynamic_cast<SalomeApp_Application*>( session->activeApplication() );
  if ( app )
  {
    CORBA::String_var IOR = app->orb()->object_to_string( theObject );
    if ( strcmp(IOR.in(), "") != 0 )
    {
      SalomeApp_Study* study = ( SalomeApp_Study* )app->activeStudy();
      _PTR(SObject) aSObj ( study->studyDS()->FindObjectIOR( std::string(IOR) ) );

      _PTR(GenericAttribute) anAttr;

      if ( aSObj && aSObj->FindAttribute( anAttr, "AttributeName") )
      {
        _PTR(AttributeName) aNameAttr( anAttr );
        return aNameAttr->Value();
      }
    }
  }
  return std::string();
}
