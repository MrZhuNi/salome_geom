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

#include <QFont>
#include <QColor>


GEOMGUI_AnnotationMgr::GEOMGUI_AnnotationMgr( SalomeApp_Application* theApplication )
: myApplication( theApplication )
{
}

SALOME_Prs* GEOMGUI_AnnotationMgr::CreatePresentation( const GEOMGUI_AnnotationAttrs::Properties& theProperty,
                                                       GEOM::GEOM_Object_ptr theObject )
{
  Handle ( GEOM_Annotation ) aPresentation = new GEOM_Annotation();

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
  aPresentation->SetScreenFixed( theProperty.IsScreenFixed );

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

//void GEOMGUI_AnnotationMgr::DisplayPresentation( SALOME_Prs* thePresentation )
//{

//}

/*void GEOMGUI_AnnotationMgr::erasePresentation( SALOME_Prs* thePresentation )
{

}*/

bool GEOMGUI_AnnotationMgr::IsDisplayed( const QString& theEntry, const int theIndex )
{
  SalomeApp_Application* anApp = getApplication();
  SUIT_ViewWindow* anActiveWindow = anApp->desktop()->activeWindow();
  SALOME_View* aView = dynamic_cast<SALOME_View*>(anActiveWindow->getViewManager()->getViewModel());

  if ( !myVisualized.contains( aView ) )
    return false;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];

  if ( !anEntryToAnnotation.contains( theEntry ) )
    return false;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];
  if ( !anAnnotationToPrs.contains( theIndex ) )
    return false;

  return true;
}

void GEOMGUI_AnnotationMgr::Display( const QString& theEntry, const int theIndex )
{
  if ( IsDisplayed( theEntry, theIndex ) )
    return;

  SalomeApp_Application* anApp = getApplication();
  SUIT_ViewWindow* anActiveWindow = anApp->desktop()->activeWindow();
  SALOME_View* aView = dynamic_cast<SALOME_View*>(anActiveWindow->getViewManager()->getViewModel());

  GEOMGUI_AnnotationAttrs::Properties aProperty;
  GEOM::GEOM_Object_ptr anObject;
  getObject( theEntry, theIndex, anObject, aProperty );

  SALOME_Prs* aPrs = CreatePresentation( aProperty, anObject );
  aView->Display( getDisplayer(), aPrs );
  getDisplayer()->UpdateViewer();

  EntryToAnnotations anEntryToMap;
  if ( myVisualized.contains( aView ) )
    anEntryToMap = myVisualized[aView];

  AnnotationToPrs anAnnotationToPrsMap;
  if ( anEntryToMap.contains( theEntry ) )
    anAnnotationToPrsMap = anEntryToMap[theEntry];
  anAnnotationToPrsMap[theIndex] = aPrs;

  anEntryToMap[theEntry] = anAnnotationToPrsMap;
  myVisualized[aView] = anEntryToMap;
}

void GEOMGUI_AnnotationMgr::Erase( const QString& theEntry, const int theIndex )
{
  SalomeApp_Application* anApp = getApplication();
  SUIT_ViewWindow* anActiveWindow = anApp->desktop()->activeWindow();
  SALOME_View* aView = dynamic_cast<SALOME_View*>(anActiveWindow->getViewManager()->getViewModel());

  if ( !myVisualized.contains( aView ) )
    return;

  EntryToAnnotations anEntryToAnnotation = myVisualized[aView];

  if ( !anEntryToAnnotation.contains( theEntry ) )
    return;

  AnnotationToPrs anAnnotationToPrs = anEntryToAnnotation[theEntry];
  if ( !anAnnotationToPrs.contains( theIndex ) )
    return;

  SALOME_Prs* aPrs = anAnnotationToPrs[theIndex];
  aView->Erase( getDisplayer(), aPrs );
  getDisplayer()->UpdateViewer();

  anAnnotationToPrs.remove( theIndex );
  anEntryToAnnotation[theEntry] = anAnnotationToPrs;
  myVisualized[aView] = anEntryToAnnotation;
}

GEOM_Displayer* GEOMGUI_AnnotationMgr::getDisplayer() const
{
  LightApp_Module* aModule = dynamic_cast<LightApp_Module*>( getApplication()->activeModule() );
  return dynamic_cast<GEOM_Displayer*>( aModule->displayer() );
}

void GEOMGUI_AnnotationMgr::getObject( const QString& theEntry, const int theIndex,
                                       GEOM::GEOM_Object_ptr& theObject,
                                       GEOMGUI_AnnotationAttrs::Properties& theProperty )
{
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( getApplication()->activeStudy() );
  _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( theEntry.toStdString() );
  const Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations = GEOMGUI_AnnotationAttrs::FindAttributes( aSObj );

  aShapeAnnotations->GetProperties( theIndex, theProperty );

  theObject = GEOM::GEOM_Object::_narrow( GeometryGUI::ClientSObjectToObject(aSObj) );
}

