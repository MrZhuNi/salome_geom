// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  GEOM OBJECT : interactive object for Geometry entities visualization
//  File   : GEOM_Annotation.hxx
//  Module : GEOM
//
#include <GEOM_Annotation.hxx>

// OCCT includes
#include <AIS_InteractiveContext.hxx>
#include <Font_FTFont.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>
#include <Graphic3d_ArrayOfSegments.hxx>
#include <Graphic3d_Camera.hxx>
#include <Graphic3d_HorizontalTextAlignment.hxx>
#include <Graphic3d_VerticalTextAlignment.hxx>
#include <Graphic3d_Vec4.hxx>
#include <OpenGl_Context.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <OpenGl_Group.hxx>
#include <OpenGl_PrimitiveArray.hxx>
#include <OpenGl_Structure.hxx>
#include <OpenGl_Text.hxx>
#include <OpenGl_View.hxx>
#include <OpenGl_Workspace.hxx>
#include <Prs3d_Root.hxx>
#include <Prs3d_Text.hxx>
#include <Select3D_SensitiveBox.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <V3d_Viewer.hxx>

IMPLEMENT_STANDARD_RTTIEXT( GEOM_Annotation, AIS_InteractiveObject )

// =======================================================================
// function : Constructor
// purpose  :
// =======================================================================
GEOM_Annotation::GEOM_Annotation() : AIS_InteractiveObject()
{
  SetPosition( gp_Pnt( 0.0, 0.0, 0.0 ) );
  SetScreenFixed( Standard_False );
  SetAttachPoint( gp_Pnt( 0.0, 0.0, 0.0 ) );
  SetDisplayMode( 0 );
  SetZLayer( Graphic3d_ZLayerId_Top );
  SetAutoHide( Standard_True );
  SetHilightMode( HighlightAll );

  Handle(Prs3d_TextAspect) aTextAspect = new Prs3d_TextAspect();
  aTextAspect->SetHeight( 20.0 );
  aTextAspect->SetColor( Quantity_Color( 1.0, 1.0, 1.0, Quantity_TOC_RGB ) );
  myDrawer->SetTextAspect( aTextAspect );

  Handle(Prs3d_LineAspect) aLineAspect =
    new Prs3d_LineAspect( Quantity_NOC_WHITE, Aspect_TOL_SOLID, 1.0 );
  myDrawer->SetLineAspect( aLineAspect );
}

// =======================================================================
// function : SetText
// purpose  :
// =======================================================================
void GEOM_Annotation::SetText( const TCollection_ExtendedString& theText )
{
  if (myText != theText)
  {
    SetToUpdate();
  }

  myText = theText;
}

// =======================================================================
// function : SetPosition
// purpose  :
// =======================================================================
void GEOM_Annotation::SetPosition( const gp_Pnt& thePosition )
{
  myPosition = thePosition;

  UpdatePersistence();
}

// =======================================================================
// function : SetScreenFixed
// purpose  :
// =======================================================================
void GEOM_Annotation::SetScreenFixed( const Standard_Boolean theIsFixed )
{
  myIsScreenFixed = theIsFixed;

  UpdatePersistence();
}

// =======================================================================
// function : SetAttachPoint
// purpose  :
// =======================================================================
void GEOM_Annotation::SetAttachPoint( const gp_Pnt& thePoint )
{
  myAttach = thePoint;
}

// =======================================================================
// function : SetColor
// purpose  :
// =======================================================================
void GEOM_Annotation::SetColor( const Quantity_Color& theColor )
{
  SetTextColor( theColor );
  SetLineColor( theColor );
}

// =======================================================================
// function : SetTextColor
// purpose  :
// =======================================================================
void GEOM_Annotation::SetTextColor( const Quantity_Color& theColor )
{
  myDrawer->TextAspect()->SetColor( theColor );

  SetToUpdate();
}

// =======================================================================
// function : SetLineColor
// purpose  :
// =======================================================================
void GEOM_Annotation::SetLineColor( const Quantity_Color& theColor )
{
  myDrawer->LineAspect()->SetColor( theColor );

  SetToUpdate();
}

// =======================================================================
// function : SetTextHeight
// purpose  :
// =======================================================================
void GEOM_Annotation::SetTextHeight( const Standard_Real theHeight )
{
  if (GetTextHeight() != theHeight)
  {
    myDrawer->TextAspect()->SetHeight( theHeight );

    SetToUpdate ();
  }
}

// =======================================================================
// function : SetFontAspect
// purpose  :
// =======================================================================
void GEOM_Annotation::SetFontAspect( const Font_FontAspect theFontAspect )
{
  if (GetFontAspect() != theFontAspect)
  {
    myDrawer->TextAspect()->Aspect()->SetTextFontAspect( theFontAspect );

    SetToUpdate();
  }
}

// =======================================================================
// function : SetFont
// purpose  :
// =======================================================================
void GEOM_Annotation::SetFont( const TCollection_AsciiString& theFont )
{
  if (GetFont() != theFont)
  {
    myDrawer->TextAspect()->Aspect()->SetFont( theFont );

    SetToUpdate();
  }
}

// =======================================================================
// function : SetLineWidth
// purpose  :
// =======================================================================
void GEOM_Annotation::SetLineWidth( const Standard_Real theLineWidth )
{
  if (GetLineWidth() != theLineWidth)
  {
    myDrawer->LineAspect()->SetWidth( theLineWidth );

    SetToUpdate();
  }
}

// =======================================================================
// function : SetLineStyle
// purpose  :
// =======================================================================
void GEOM_Annotation::SetLineStyle( const Aspect_TypeOfLine theStyle )
{
  if (GetLineStyle() != theStyle)
  {
    myDrawer->LineAspect()->SetTypeOfLine( theStyle );

    SetToUpdate();
  }
}

// =======================================================================
// function : UpdatePersistence
// purpose  :
// =======================================================================
void GEOM_Annotation::UpdatePersistence()
{
  if (!myIsScreenFixed)
  {
    AIS_InteractiveObject::SetTransformPersistence( Graphic3d_TMF_ZoomPers | Graphic3d_TMF_RotatePers, myPosition );
  }
  else
  {
    AIS_InteractiveObject::SetTransformPersistence( Graphic3d_TMF_2d, gp_Pnt( 0.0, 0.0, 0.0 ) );
  }
}

// =======================================================================
// function : Compute
// purpose  :
// =======================================================================
void GEOM_Annotation::Compute( const Handle(PrsMgr_PresentationManager3d)& /*thePresentationManager*/,
                               const Handle(Prs3d_Presentation)&           thePresentation,
                               const Standard_Integer                      theMode )
{
  thePresentation->Clear();

  if (theMode < 0)
  {
    return;
  }

  Handle(OpenGl_Group) aGroup = Handle(OpenGl_Group)::DownCast( Prs3d_Root::NewGroup( thePresentation ) );
  if (aGroup.IsNull())
  {
    return;
  }

  Handle(Prs3d_TextAspect) anAsp = myDrawer->TextAspect();
  NCollection_String aUtfText( myText.ToExtString() );
  OpenGl_Annotation* aAnnotationDraw =
    new OpenGl_Annotation( this, static_cast<Standard_Integer>( anAsp->Height() ), aGroup->GlStruct()->GlDriver() );

  aGroup->AddElement( aAnnotationDraw );
  aGroup->SetGroupPrimitivesAspect( myDrawer->TextAspect()->Aspect() );
  aGroup->SetGroupPrimitivesAspect( myDrawer->LineAspect()->Aspect() );

  const NCollection_Handle<Bnd_Box> aBoundingBox = TextBoundingBox();
  const gp_Pnt aBoxMin = aBoundingBox->CornerMin();
  const gp_Pnt aBoxMax = aBoundingBox->CornerMax();
  aGroup->ChangeBoundingBox() = Graphic3d_BndBox4f (
    Graphic3d_Vec4( static_cast<Standard_ShortReal>( aBoxMin.X() ),
                    static_cast<Standard_ShortReal>( aBoxMin.Y() ),
                    static_cast<Standard_ShortReal>( aBoxMin.Z() ), 1.0F ),
    Graphic3d_Vec4( static_cast<Standard_ShortReal>( aBoxMax.X() ),
                    static_cast<Standard_ShortReal>( aBoxMax.Y() ),
                    static_cast<Standard_ShortReal>( aBoxMax.Z() ), 1.0F ) );
}

// =======================================================================
// function : ComputeSelection
// purpose  :
// =======================================================================
void GEOM_Annotation::ComputeSelection( const Handle(SelectMgr_Selection)& theSelection,
                                        const Standard_Integer             theMode )
{
  if (theMode != 0)
  {
    return;
  }

  theSelection->Clear();

  const NCollection_Handle<Bnd_Box> aBoundingBox = TextBoundingBox();
  const Handle(SelectMgr_EntityOwner) anEntityOwner = new SelectMgr_EntityOwner( this, 10 );
  const Handle(Select3D_SensitiveBox) aSensitive = new Select3D_SensitiveBox( anEntityOwner, *aBoundingBox.get() );
  theSelection->Add( aSensitive );
}

// =======================================================================
// function : TextBoundingBox
// purpose  :
// =======================================================================
NCollection_Handle<Bnd_Box> GEOM_Annotation::TextBoundingBox() const
{
  Handle(Prs3d_TextAspect) anAsp = myDrawer->TextAspect();
  Bnd_Box aBox;
  Font_FTFont aFont;
  unsigned int aResolution = GetContext()->CurrentViewer()->DefaultRenderingParams().Resolution;
  if ( aFont.Init( anAsp->Aspect()->Font().ToCString(),
                   anAsp->Aspect()->GetTextFontAspect(),
                  (unsigned int)anAsp->Height(),
                  aResolution ) )
  {
    const NCollection_String aText( (Standard_Utf16Char* )myText.ToExtString() );
    const Font_Rect aFontRect = aFont.BoundingBox( aText, Graphic3d_HTA_LEFT, Graphic3d_VTA_BOTTOM );
    NCollection_Handle<Bnd_Box> aBox = new Bnd_Box();
    aBox->Add( gp_Pnt( 0.0, 0.0, 0.0 ) );
    aBox->Add( gp_Pnt( aFontRect.Width(), aFontRect.Height(), 0.0 ) );
    return aBox;
  }

  return NCollection_Handle<Bnd_Box>();
}

// =======================================================================
// subclass : OpenGl_Annotation
// function : Constructor
// purpose  : 
// =======================================================================
GEOM_Annotation::OpenGl_Annotation::OpenGl_Annotation( GEOM_Annotation* theAnnotation,
                                                       const Standard_Integer theTextHeight,
                                                       const OpenGl_GraphicDriver* theDriver )
: OpenGl_Element(),
  myAISObject( theAnnotation ),
  myText( theAnnotation->myText.ToExtString() ),
  myTextLineY( 0.f ),
  myTextDPI( 0 )
{
  // graphical resources for drawing text and underline
  myTextParams.Height = theTextHeight;
  myTextParams.HAlign = Graphic3d_HTA_LEFT;
  myTextParams.VAlign = Graphic3d_VTA_BOTTOM;
  myTextDraw = new OpenGl_Text( myText.ToCString(), OpenGl_Vec3(), myTextParams );
  myTextLineDraw = new OpenGl_PrimitiveArray( theDriver );

  // graphical resources for drawing extension line and marker
  Handle(Graphic3d_ArrayOfSegments)
  aExtVertexArray = new Graphic3d_ArrayOfSegments( 2 );
  aExtVertexArray->AddVertex( 0.0, 0.0, 0.0 );
  aExtVertexArray->AddVertex( 0.0, 0.0, 1.0 );
  myExtLineDraw = new OpenGl_PrimitiveArray( theDriver, Graphic3d_TOPA_SEGMENTS,
    aExtVertexArray->Indices(), aExtVertexArray->Attributes(), aExtVertexArray->Bounds() );

  Handle(Graphic3d_ArrayOfPoints)
  aExtMakerArray = new Graphic3d_ArrayOfPoints( 1 );
  aExtMakerArray->AddVertex( 0.0, 0.0, 1.0 );
  myExtMarkerDraw = new OpenGl_PrimitiveArray( theDriver, Graphic3d_TOPA_POINTS,
    aExtMakerArray->Indices(), aExtMakerArray->Attributes(), aExtMakerArray->Bounds() );
}

// =======================================================================
// subclass : OpenGl_Annotation
// function : Destructor
// purpose  : 
// =======================================================================
GEOM_Annotation::OpenGl_Annotation::~OpenGl_Annotation()
{
  Release( NULL );
}

// =======================================================================
// subclass : OpenGl_Annotation
// function : Release
// purpose  : Releases GL resources with the given GL context.
// =======================================================================
void GEOM_Annotation::OpenGl_Annotation::Release( OpenGl_Context* theCtx )
{
  if (myTextDraw)
  {
    myTextDraw->Release( theCtx );
    myTextLineDraw->Release( theCtx );
    myExtLineDraw->Release( theCtx );
    myExtMarkerDraw->Release( theCtx );
  }
  myTextDraw      = NULL;
  myTextLineDraw  = NULL;
  myExtLineDraw   = NULL;
  myExtMarkerDraw = NULL;
}

// =======================================================================
// subclass : OpenGl_Annotation
// function : Render
// purpose  : Renders the annotation graphical elements.
// =======================================================================
void GEOM_Annotation::OpenGl_Annotation::Render( const Handle(OpenGl_Workspace)& theWorkspace ) const
{
  const Handle(OpenGl_Context)& aContext = theWorkspace->GetGlContext();

  // ---------------------------------------------------------------------
  // initialize text's font and configure some properties when DPI changes
  // ---------------------------------------------------------------------
  const unsigned int aDPI = theWorkspace->View()->RenderingParams().Resolution;
  if (myTextDPI != aDPI)
  {
    const OpenGl_AspectText* anAspect = theWorkspace->AspectText();

    // getting string size will also initialize font library
    Standard_ShortReal aWidth, aHeight, aDescent;
    myTextDraw->StringSize( aContext, myText, *anAspect, myTextParams, aDPI, aWidth, aHeight, aDescent );
    myTextDPI = aDPI;
    myTextLineY = ceil( aHeight * -0.2f );

    Handle(Graphic3d_ArrayOfSegments)
    aVertexArray = new Graphic3d_ArrayOfSegments( 2 );
    aVertexArray->AddVertex( 0.0f, myTextLineY, 0.0f );
    aVertexArray->AddVertex( aWidth, myTextLineY, 0.0f );
    myTextLineDraw->InitBuffers( aContext, Graphic3d_TOPA_SEGMENTS,
      aVertexArray->Indices(), aVertexArray->Attributes(), aVertexArray->Bounds() );
  }

  // ---------------------------------------------
  // perform view culling test by attachment point
  // ---------------------------------------------
  const OpenGl_Vec4 aAttach( static_cast<float>( myAISObject->myAttach.X() ),
                             static_cast<float>( myAISObject->myAttach.Y() ),
                             static_cast<float>( myAISObject->myAttach.Z() ), 1.F );

  const Handle(Graphic3d_Camera) aCamera = theWorkspace->View()->Camera();
  const OpenGl_Mat4& aCameraProjMat = aCamera->ProjectionMatrixF();
  const OpenGl_Mat4& aCameraViewMat = aCamera->OrientationMatrixF();
  const OpenGl_Vec4 aAttachView = aCameraViewMat * aAttach;
  if (myAISObject->myIsAutoHide)
  {
    const OpenGl_Vec4 aAttachClip = aCameraProjMat * aAttachView;
    if (abs( aAttachClip.x() ) > aAttachClip.w()
     || abs( aAttachClip.y() ) > aAttachClip.w()
     || abs( aAttachClip.z() ) > aAttachClip.w())
    {
      return;
    }
  }

  // -------------------------------------------------------------
  // render text label in current persistence matrix and underline
  // -------------------------------------------------------------
  myTextDraw->Render( theWorkspace );

  // ----------------------------------------------
  // render underline in current persistence matrix
  // ----------------------------------------------
  const bool toHighlight = theWorkspace->ToHighlight();
  if (toHighlight && myAISObject->myHilightMode == HighlightLabel)
  {
    theWorkspace->SetHighlight( false );
    theWorkspace->ApplyAspectLine();
  }
  myTextLineDraw->Render( theWorkspace );

  // ------------------------------------------------------------
  // render dynamic extension line using synthetic transformation
  // ------------------------------------------------------------
  const OpenGl_Mat4& aViewMat = aContext->WorldViewState.Current();
  const OpenGl_Vec4  aHingeView = aViewMat * OpenGl_Vec4( 0.0f, myTextLineY, 0.0f, 1.0f );

  // prepare matrix to specify geometry of extension line in view space
  // by multiplication of unit z coordinate vector on given matrix.
  OpenGl_Mat4 aExtGeometryMat;
  aExtGeometryMat.SetColumn( 2, aAttachView - aHingeView );
  aExtGeometryMat.SetColumn( 3, aHingeView );

  aContext->ModelWorldState.Push();
  aContext->ModelWorldState.SetIdentity();
  aContext->WorldViewState.Push();
  aContext->WorldViewState.SetCurrent( aExtGeometryMat );
  aContext->ApplyModelViewMatrix();

  myExtLineDraw->Render( theWorkspace );
  myExtMarkerDraw->Render( theWorkspace );

  aContext->ModelWorldState.Pop();
  aContext->WorldViewState.Pop();
  aContext->ApplyModelViewMatrix();

  if (toHighlight != theWorkspace->ToHighlight())
  {
    theWorkspace->SetHighlight( toHighlight );
  }
}

