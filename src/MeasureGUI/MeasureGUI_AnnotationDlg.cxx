// Copyright ( C ) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright ( C ) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or ( at your option ) any later version.
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

//  File   : MeasureGUI_AnnotationDlg.cxx

#include "MeasureGUI.h"
#include "MeasureGUI_AnnotationDlg.h"
#include "MeasureGUI_AnnotationInteractor.h"

#include <DlgRef.h>
#include <GEOMBase.h>
#include <GEOMBase_Skeleton.h>
#include <GEOM_Displayer.h>
#include <GeometryGUI.h>
#include <GEOMGUI_AnnotationMgr.h>

#include <SOCC_Prs.h>
#include <SOCC_ViewModel.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <SalomeApp_Tools.h>
#include <SalomeApp_IntSpinBox.h>
#include <SalomeApp_DoubleSpinBox.h>

#include <LightApp_SelectionMgr.h>

#include <OCCViewer_ViewModel.h>
#include <OCCViewer_ViewManager.h>
#include <OCCViewer_ViewWindow.h>
#include <SVTK_ViewModel.h>
#include <SALOME_Prs.h>
#include <SALOME_ListIO.hxx>

#include <SUIT_Desktop.h>
#include <SUIT_MessageBox.h>
#include <SUIT_OverrideCursor.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStack>
#include <QTableWidget>
#include <QVBoxLayout>

#include <AIS_InteractiveContext.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <Bnd_Box.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>

#include <SelectMgr_ViewerSelector.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TColStd_MapOfInteger.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

#include <NCollection_Handle.hxx>

#include <limits>

#include <SALOMEDS_wrap.hxx>
#include <GEOMImpl_Types.hxx>

#ifdef max
#undef max
#endif

//=======================================================================
//function : MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::MeasureGUI_AnnotationDlg( GeometryGUI* theGeometryGUI, const bool theIsCreate,
                                                   QWidget* parent, bool modal, Qt::WindowFlags fl )
: GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl ),
  myIsCreation( theIsCreate ), myShapeNameModified( false )
{
  myEditCurrentArgument = 0;

  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  QPixmap iconSelect( resMgr->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );

  setWindowTitle( myIsCreation ? tr( "CREATE_ANNOTATION_TITLE" ) : tr( "EDIT_ANNOTATION_TITLE" ) );

  // Shape type button group
  mainFrame()->GroupConstructors->hide();

  // Field name
  mainFrame()->GroupBoxName->setTitle( tr( "ANNOTATION_NAME" ) );

  // Field properties
  QGroupBox* propGroup = new QGroupBox( tr( "ANNOTATION_PROPERTIES" ), centralWidget() );
  QGridLayout* propLayout = new QGridLayout( propGroup );
  propLayout->setMargin( 9 );
  propLayout->setSpacing( 6 );

  QLabel* textLabel = new QLabel( tr( "ANNOTATION_TEXT" ), propGroup );
  myTextEdit = new QLineEdit( propGroup );
  propLayout->addWidget( textLabel, 0, 0 );
  propLayout->addWidget( myTextEdit, 0, 1, 1, 2 );

  // shape
  QLabel* shapeLabel = new QLabel( tr( "ANNOTATION_SHAPE" ), propGroup );
  myShapeSelBtn = new QPushButton( propGroup );
  myShapeSelBtn->setIcon( iconSelect );
  myShapeSelBtn->setEnabled( myIsCreation );
  myShapeName = new QLineEdit( propGroup );
  myShapeName->setReadOnly( true );
  myShapeName->setEnabled( myIsCreation );

  // data type
  QLabel* typeLabel = new QLabel( tr( "ANNOTATION_TYPE" ), propGroup );
  myTypeCombo = new QComboBox( propGroup );
  myTypeCombo->addItem( tr( "3D" ), 0 );
  myTypeCombo->addItem( tr( "2D" ), 1 );
  myTypeCombo->setCurrentIndex( 0 ); // 3D, not fixed

  propLayout->addWidget( shapeLabel, 1, 0 );
  propLayout->addWidget( myShapeSelBtn, 1, 1 );
  propLayout->addWidget( myShapeName, 1, 2 );
  propLayout->addWidget( typeLabel, 2, 0, 1, 2 );
  propLayout->addWidget( myTypeCombo, 2, 2 );
  propLayout->setColumnStretch( 2, 5 );

  QLabel* shapeTypeLabel = new QLabel( tr( "ANNOTATION_SUB_SHAPE" ), propGroup );
  mySubShapeTypeCombo = new QComboBox( propGroup );
  mySubShapeTypeCombo->addItem( tr( "WHOLE_SHAPE" ), TopAbs_SHAPE );
  mySubShapeTypeCombo->addItem( tr( "GEOM_VERTEX" ), TopAbs_VERTEX );
  mySubShapeTypeCombo->addItem( tr( "GEOM_EDGE" ), TopAbs_EDGE );
  mySubShapeTypeCombo->addItem( tr( "GEOM_FACE" ), TopAbs_FACE );
  mySubShapeTypeCombo->addItem( tr( "GEOM_SOLID" ), TopAbs_SOLID );
  mySubShapeTypeCombo->setCurrentIndex( 0 ); // SHAPE

  propLayout->addWidget( shapeTypeLabel, 3, 0 );
  propLayout->addWidget( mySubShapeTypeCombo, 3, 1, 1, 2 );

  // sub-shape
  mySubShapeSelBtn = new QPushButton( propGroup );
  mySubShapeSelBtn->setIcon( iconSelect );
  mySubShapeSelBtn->setEnabled( myIsCreation );
  mySubShapeName = new QLineEdit( propGroup );
  mySubShapeName->setReadOnly( true );
  mySubShapeName->setEnabled( myIsCreation );

  propLayout->addWidget( mySubShapeSelBtn, 4, 1 );
  propLayout->addWidget( mySubShapeName, 4, 2 );

  // Field properties
  QGroupBox* styleGroup = new QGroupBox( tr( "ANNOTATION_STYLE" ),
      centralWidget() );
  QGridLayout* styleLayout = new QGridLayout( styleGroup );
  styleLayout->setMargin( 9 );
  styleLayout->setSpacing( 6 );

  QLabel* fontLabel = new QLabel( tr( "ANNOTATION_FONT" ), styleGroup );
  styleLayout->addWidget( fontLabel, 0, 0 );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 );
  layout->setSpacing( 6 );
  layout->addWidget( propGroup );
  layout->addWidget( styleGroup );

  setHelpFileName( "annotation_page.html" );

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  connect( aSelMgr, SIGNAL( currentSelectionChanged() ), this,
           SLOT( SelectionIntoArgument() ) );
  connect( buttonOk(), SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(), SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );

  myInteractor = new MeasureGUI_AnnotationInteractor( theGeometryGUI, parent );
  myInteractor->Enable();

  connect( myInteractor, SIGNAL( SignalInteractionFinished( Handle_GEOM_Annotation ) ),
           this, SLOT( onDragged( Handle_GEOM_Annotation ) ) );

  Init();
}

//=======================================================================
//function : ~MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::~MeasureGUI_AnnotationDlg() {
}

//=================================================================================
// function : Init()
// purpose  : fills annotation properties with default values( in create mode ) or
// the values of modified object
//=================================================================================
void MeasureGUI_AnnotationDlg::Init()
{
  if ( myIsCreation ) {
    initName( tr( "ANNOTATION_PREFIX" ) );

    // default presentation values
    myAnnotationProperties.Name = getNewObjectName();
    myAnnotationProperties.Position = gp_Pnt( 250, 250, 250 );
    myAnnotationProperties.Text = tr( "ANNOTATION_PREFIX" );
    myAnnotationProperties.IsVisible = false;
    myAnnotationProperties.IsScreenFixed = false;
    myAnnotationProperties.Attach = gp_Pnt( 0, 0, 0 );
    myAnnotationProperties.ShapeIndex = -1;
    myAnnotationProperties.ShapeType = ( int ) TopAbs_SHAPE;

    // update internal controls and fields following to default values
    activateSelectionArgument( myShapeSelBtn );
    myTextEdit->setText( myAnnotationProperties.Text );
    myShapeNameModified = false;
    myTypeCombo->setCurrentIndex( 0 );

    int aSubShapeTypeIndex = -1;
    int aTypesCount = aTypesCount = mySubShapeTypeCombo->count();
    for ( int i = 0; i < aTypesCount && aSubShapeTypeIndex < 0; i++ ) {
      int aType = mySubShapeTypeCombo->itemData( i ).toInt();
      if ( aType == myAnnotationProperties.ShapeType )
        aSubShapeTypeIndex = i;
    }
    mySubShapeTypeCombo->setCurrentIndex( aSubShapeTypeIndex );

    mySelectionMode = ( TopAbs_ShapeEnum ) myAnnotationProperties.ShapeType;
    SelectionIntoArgument();
    updateSubShapeEnableState();

    // connect controls
    connect( myShapeSelBtn, SIGNAL( clicked() ), this,
        SLOT( SetEditCurrentArgument() ) );
    connect( mySubShapeSelBtn, SIGNAL( clicked() ), this,
        SLOT( SetEditCurrentArgument() ) );

    connect( myTextEdit, SIGNAL( textChanged( const QString& ) ), this, SLOT( onTextChange() ) );
    connect( myTypeCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onTypeChange() ) );
    connect( mySubShapeTypeCombo, SIGNAL( currentIndexChanged( int ) ),
        this, SLOT( onSubShapeTypeChange() ) );

    //SelectionIntoArgument();
  } else { // edition

  }
  redisplayPreview();
}

//=================================================================================
// function : activateSelection
// purpose  : Activate local selection
//=================================================================================
void MeasureGUI_AnnotationDlg::activateSelection()
{
  globalSelection( GEOM_ALLOBJECTS );
  if ( !myShape->_is_nil() && mySelectionMode != TopAbs_SHAPE ) {
    localSelection( myShape.get(), mySelectionMode );
  }
}

//=================================================================================
// function : getShapeType()
// purpose  :
//=================================================================================
TopAbs_ShapeEnum MeasureGUI_AnnotationDlg::getShapeType() const
{
  return ( TopAbs_ShapeEnum ) mySubShapeTypeCombo->itemData(
      mySubShapeTypeCombo->currentIndex() ).toInt();
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::ClickOnOk()
{
  setIsApplyAndClose( true );
  if ( ClickOnApply() )
    ClickOnCancel();
  setIsApplyAndClose( false );
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::ClickOnApply()
{
  if ( !isApplyAndClose() ) {
    setIsDisableBrowsing( true );
    setIsDisplayResult( false );
  }

  QString msg;
  if ( !isValid( msg ) ) {
    showError( msg );
    return false;
  }

  SUIT_OverrideCursor wc;
  SUIT_Session::session()->activeApplication()->putInfo( "" );

  try {
    if ( openCommand() )
      if ( !execute( /*isApplyAndClose()*/ ) ) {
        abortCommand();
        showError();
        return false;
      }
  } catch ( const SALOME::SALOME_Exception& e ) {
    SalomeApp_Tools::QtCatchCorbaException( e );
    abortCommand();
    return false;
  }
  commitCommand();

  if ( !isApplyAndClose() ) {
    setIsDisableBrowsing( false );
    setIsDisplayResult( true );
  }

  if ( !myShape->_is_nil() ) {
    redisplay( myShape.get() );
  }

  if ( myIsCreation ) {

   if ( !isApplyAndClose() )
     Init();
  }

  return true;
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  : process click on shape/sub-shape button. It stores as current edit argument
// the corresponded line edit, set focus in it and unpress other button if it was pressed
//=================================================================================
void MeasureGUI_AnnotationDlg::SetEditCurrentArgument()
{
  QPushButton* aSelectButton = ( QPushButton* ) sender();

  activateSelectionArgument( aSelectButton );

  SelectionIntoArgument();
}

//=================================================================================
// function : activateSelectionArgument()
// purpose  : it stores as current edit argument the corresponded line edit,
// sets the focus on it and unpresses other button if it was pressed
//=================================================================================
void MeasureGUI_AnnotationDlg::activateSelectionArgument
(
  QPushButton* theSelectionButton ) {
  QPushButton* anOtherButton = 0;
  if ( theSelectionButton == myShapeSelBtn ) {
    myEditCurrentArgument = myShapeName;
    anOtherButton = mySubShapeSelBtn;
  } else if ( theSelectionButton == mySubShapeSelBtn ) {
    myEditCurrentArgument = mySubShapeName;
    anOtherButton = myShapeSelBtn;
  } else
    myEditCurrentArgument = 0;

  if ( myEditCurrentArgument )
    myEditCurrentArgument->setFocus();

  theSelectionButton->setDown( true );
  anOtherButton->setDown( false );
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed. Sets the current selection in the
// annotation property and redisplays presentation
//=================================================================================
void MeasureGUI_AnnotationDlg::SelectionIntoArgument()
{
  if ( myIsCreation && myEditCurrentArgument )
  {
    myEditCurrentArgument->setText( "" );

    GEOM::GeomObjPtr anObj = getSelected( mySelectionMode );

    gp_Pnt anAttachPoint( 0, 0, 0 );
    int aSubShapeIndex = -1;
    if ( myEditCurrentArgument == myShapeName ) { // Selection of a shape is active
      if ( anObj->_is_nil() || mySelectionMode != TopAbs_SHAPE ) {
        myShape = GEOM::GEOM_Object::_nil();
      } else {
        myShape = anObj;
        QString aName = GEOMBase::GetName( anObj.get() );
        myEditCurrentArgument->setText( aName );
        if ( !myShapeNameModified ) {
          myTextEdit->setText( aName );
          onTextChange();
        }
      }

      bool aNullShape = myShape->_is_nil();
      mySubShapeTypeCombo->setEnabled( !aNullShape );
      updateSubShapeEnableState();

      activateSelection();

      if ( !aNullShape ) {

        TopoDS_Shape aShape;
        GEOMBase::GetShape( myShape.get(), aShape );

        if ( !getPickedPoint( anAttachPoint ) ) {

          anAttachPoint = getAttachPoint( aShape );
        }

        gp_Ax3 aShapeLCS = gp_Ax3().Transformed( aShape.Location().Transformation() );
        gp_Trsf aToShapeLCS;
        aToShapeLCS.SetTransformation( gp_Ax3(), aShapeLCS );
        anAttachPoint.Transform( aToShapeLCS );
      }
    } else if ( myEditCurrentArgument == mySubShapeName ) {
      if ( !myShape->_is_nil() ) {

        if ( anObj->_is_nil() ) {
          myEditCurrentArgument->setText( QString() );
        }
        else {

          QString aName = GEOMBase::GetName( anObj.get() );
          myEditCurrentArgument->setText( aName );

          TopTools_IndexedMapOfShape aMainMap;
          TopoDS_Shape aMainShape;
          TopoDS_Shape aSubShape;
          GEOMBase::GetShape( myShape.get(), aMainShape );
          GEOMBase::GetShape( anObj.get(), aSubShape );
          TopExp::MapShapes( aMainShape, getShapeType(), aMainMap );

          if ( aMainMap.Contains( aSubShape ) ) {
            aSubShapeIndex = aMainMap.FindIndex( aSubShape );
          }

          if ( !aSubShape.IsNull() ) {

            TopoDS_Shape aShape;
            GEOMBase::GetShape( myShape.get(), aShape );

            if ( !getPickedPoint( anAttachPoint ) ) {

              anAttachPoint = getAttachPoint( aSubShape );
            }

            gp_Ax3 aShapeLCS = gp_Ax3().Transformed( aShape.Location().Transformation() );
            gp_Trsf aToShapeLCS;
            aToShapeLCS.SetTransformation( gp_Ax3(), aShapeLCS );
            anAttachPoint.Transform( aToShapeLCS );
          }
        }
      }
      myAnnotationProperties.ShapeIndex = aSubShapeIndex;
    }
    myAnnotationProperties.Attach = anAttachPoint;
  }
  redisplayPreview();
}

//=======================================================================
//function : closeEvent
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::closeEvent( QCloseEvent* theEv )
{
  if ( myInteractor ) {
    myInteractor->Disable();
  }
  GEOMBase_Skeleton::closeEvent( theEv );
}

//=======================================================================
//function : onTextChange
//purpose  : change annotation text
//=======================================================================
void MeasureGUI_AnnotationDlg::onTextChange()
{
  myAnnotationProperties.Text = myTextEdit->text();
  if ( !myShapeNameModified )
    myShapeNameModified = true;

  redisplayPreview();
}

//=======================================================================
//function : onTypeChange
//purpose  : change annotation type: 2D or 3D
//=======================================================================
void MeasureGUI_AnnotationDlg::onTypeChange()
{
  myAnnotationProperties.IsScreenFixed = myTypeCombo->currentIndex() == 1;
  redisplayPreview();
}

//=======================================================================
//function : onSubShapeTypeChange
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::onSubShapeTypeChange()
{
  const TopAbs_ShapeEnum aShapeType = getShapeType();
  
  activateSelectionArgument( aShapeType == TopAbs_SHAPE ? myShapeSelBtn : mySubShapeSelBtn );

  myAnnotationProperties.ShapeType = aShapeType;

  if ( aShapeType != mySelectionMode ) {
    mySubShapeName->setText( "" );
    myAnnotationProperties.ShapeIndex = -1;
    mySelectionMode = aShapeType;
  }

  updateSubShapeEnableState();

  activateSelection();
  redisplayPreview();
}

//=================================================================================
// function : onDragged
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::onDragged( Handle_GEOM_Annotation theAnnotation )
{
  const PrsList& aPreview = getPreview();
  PrsList::const_iterator anIt = aPreview.cbegin();
  for ( ; anIt != aPreview.cend(); ++anIt ) {

    AIS_ListOfInteractive aIObjects;
    ((SOCC_Prs*)(*anIt))->GetObjects( aIObjects );
    AIS_ListOfInteractive::Iterator aIOIt( aIObjects );
    for ( ; aIOIt.More(); aIOIt.Next() ) {

      if ( aIOIt.Value() == theAnnotation ) {

        TopoDS_Shape aShape;
        GEOMBase::GetShape( myShape.get(), aShape );
        gp_Ax3 aShapeLCS = gp_Ax3().Transformed( aShape.Location().Transformation() );
        gp_Trsf aToShapeLCS;
        aToShapeLCS.SetTransformation( gp_Ax3(), aShapeLCS );

        if ( !myAnnotationProperties.IsScreenFixed ) {
          myAnnotationProperties.Position = theAnnotation->GetPosition().Transformed( aToShapeLCS );
        }
        else {
          myAnnotationProperties.Position = theAnnotation->GetPosition();
        }

        return;
      }
    }
  }
}

#define RETURN_WITH_MSG( a, b ) \
  if ( ( a ) ) { \
    theMessage += ( b ); \
    return false; \
  }

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr MeasureGUI_AnnotationDlg::createOperation()
{
  return getGeomEngine()->GetILocalOperations( getStudyId() );
}

//=================================================================================
// function : isValid()
// purpose  : Verify validity of input data
//=================================================================================
bool MeasureGUI_AnnotationDlg::isValid( QString& theMessage )
{
  SalomeApp_Study* study = getStudy();
  RETURN_WITH_MSG( !study, tr( "GEOM_NO_STUDY" ) )
  RETURN_WITH_MSG( study->studyDS()->GetProperties()->IsLocked(),
      tr( "GEOM_STUDY_LOCKED" ) )

  if ( myIsCreation ) {
    RETURN_WITH_MSG( myShape->_is_nil(), tr( "NO_SHAPE" ) )
  } else {
    //RETURN_WITH_MSG( CORBA::is_nil( myShape ), tr( "NO_FIELD" ) )
  }

  if ( getShapeType() != TopAbs_SHAPE ) {
    if ( myIsCreation ) {
      RETURN_WITH_MSG( myAnnotationProperties.ShapeIndex < 0, tr( "NO_SUB_SHAPE" ) )
    } else {
      //RETURN_WITH_MSG( CORBA::is_nil( myShape ), tr( "NO_FIELD" ) )
    }
  }
  return true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::execute()
{
  QString anError;
  if ( !isValid( anError ) )
    return false;

  if ( myIsCreation ) {

    SalomeApp_Study* aStudy = getStudy();

    _PTR(SObject) aSObj = aStudy->studyDS()->FindObjectID( myShape->GetStudyEntry() );

    Handle(GEOMGUI_AnnotationAttrs) aShapeAnnotations =
      GEOMGUI_AnnotationAttrs::FindOrCreateAttributes( aSObj, aStudy );

    myAnnotationProperties.Name = getNewObjectName(); // update here as we do not listen name modification
    myAnnotationProperties.IsVisible = true; // initially created annotation is hidden

    aShapeAnnotations->Append( myAnnotationProperties );

    myGeomGUI->emitAnnotationsUpdated( QString( myShape->GetStudyEntry() ) );

    erasePreview( true );
    myGeomGUI->GetAnnotationMgr()->Display( myShape->GetStudyEntry(), aShapeAnnotations->GetNbAnnotation()-1 );
  }
  else {
    /*SalomeApp_Study* aStudy = getStudy();
    myAnnotationProperties = aStudy->getObjectProperty( GEOM::sharedPropertiesId(),
                                   myShape->GetStudyEntry(),
                                   GEOM::propertyName( GEOM::ShapeAnnotations ),
                                   QVariant() )
                                   .value<GEOMGUI_ShapeAnnotations>();
    mySavedPropertyState.SaveToAttribute( aStudy, myEditObject->GetStudyEntry() );*/
  }
  return true;
}

//=================================================================================
// function : buildPrs
// purpose  : creates annotation presentation object and corresponded SALOME presentation
//=================================================================================
SALOME_Prs* MeasureGUI_AnnotationDlg::buildPrs()
{
  SALOME_Prs* aPrs = myGeomGUI->GetAnnotationMgr()->CreatePresentation( myAnnotationProperties, myShape.get() );
  /*
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
  aPresentation->SetScreenFixed( myAnnotationProperties.IsScreenFixed );
  aPresentation->SetDepthCulling( Standard_False );

  TopoDS_Shape aShape;
  GEOMBase::GetShape( myShape.get(), aShape );
  gp_Ax3 aShapeLCS = gp_Ax3().Transformed( aShape.Location().Transformation() );
  GEOMGUI_AnnotationAttrs::SetupPresentation( aPresentation, myAnnotationProperties, aShapeLCS );

  // add Prs to preview
  SUIT_ViewWindow* vw =
      SUIT_Session::session()->activeApplication()->desktop()->activeWindow();
  SOCC_Prs* aPrs =
      dynamic_cast<SOCC_Prs*>( ( ( SOCC_Viewer* ) ( vw->getViewManager()->getViewModel() ) )->CreatePrs(
          0 ) );

  if ( aPrs )
    aPrs->AddObject( aPresentation );
  */
  return aPrs;
}

//=================================================================================
// function : updateSubShapeEnableState
// purpose  : creates annotation presentation object and corresponded SALOME presentation
//=================================================================================
void MeasureGUI_AnnotationDlg::updateSubShapeEnableState()
{
  bool isWholeShape = getShapeType() == TopAbs_SHAPE;
  bool aNullShape = myShape->_is_nil();
  mySubShapeSelBtn->setEnabled( !aNullShape && !isWholeShape );
  mySubShapeName->setEnabled( !aNullShape && !isWholeShape );
}

//=================================================================================
// function : redisplayPreview
// purpose  : creates annotation presentation object and corresponded SALOME presentation
//=================================================================================
void MeasureGUI_AnnotationDlg::redisplayPreview()
{
  QString aMess;
  if ( !isValid( aMess ) ) {
    erasePreview( true );
    return;
  }

  erasePreview( false );

  try {
    SUIT_OverrideCursor wc;
    getDisplayer()->SetToActivate( true );

    if ( SALOME_Prs* aPrs = buildPrs() )
      displayPreview( aPrs );
  } catch ( const SALOME::SALOME_Exception& e ) {
    SalomeApp_Tools::QtCatchCorbaException( e );
  } catch ( ... ) {
  }
}

//=================================================================================
// function : getPickedPoint
// purpose  : finds picked point in active viewer on the selected shape
//=================================================================================
bool MeasureGUI_AnnotationDlg::getPickedPoint( gp_Pnt& thePnt )
{
  const SUIT_ViewWindow* anActiveView = GEOMBase_Helper::getActiveView();
  if ( !anActiveView )
    return false;

  const OCCViewer_ViewWindow* anOccView = qobject_cast<const OCCViewer_ViewWindow*>( anActiveView );
  if ( !anOccView || !anOccView->underMouse() )
    return false;

  OCCViewer_ViewManager* aVM = ( OCCViewer_ViewManager* )anOccView->getViewManager();
  OCCViewer_Viewer* aViewer = aVM->getOCCViewer();

  Handle(AIS_InteractiveContext) anAISContext = aViewer->getAISContext();
  Handle(SelectMgr_ViewerSelector) aSelector;
  if ( anAISContext->HasOpenedContext() )
    aSelector = anAISContext->LocalSelector();
  else
    aSelector = anAISContext->MainSelector();

  if ( aSelector->NbPicked() < 1 )
    return false;

  thePnt = aSelector->PickedPoint( 1 );
  return true;
}

//=================================================================================
// function : getAttachPoint
// purpose  : computes default attachment point on the shape
//=================================================================================
gp_Pnt MeasureGUI_AnnotationDlg::getAttachPoint( const TopoDS_Shape& theShape )
{
  TopoDS_Shape aAttachShape;
  if ( theShape.ShapeType() == TopAbs_COMPOUND )
  {
    QStack< NCollection_Handle<TopoDS_Iterator> > aItStack;
    aItStack.push( NCollection_Handle<TopoDS_Iterator>( new TopoDS_Iterator( theShape ) ) );
    while ( aAttachShape.IsNull() && !aItStack.empty() )
    {
      NCollection_Handle<TopoDS_Iterator> anIt = aItStack.top();
      if ( !anIt->More() )
      {
        aItStack.pop();
      }
      else
      {
        const TopoDS_Shape& aShape = anIt->Value();
        if ( aShape.ShapeType() != TopAbs_COMPSOLID )
        {
          aAttachShape = aShape;
        }
        else
        {
          aItStack.push( NCollection_Handle<TopoDS_Iterator>( new TopoDS_Iterator( aShape ) ) );
        }
      }
    }
  }
  else
  {
    aAttachShape = theShape;
  }

  if ( aAttachShape.ShapeType() == TopAbs_COMPSOLID
    || aAttachShape.ShapeType() == TopAbs_SOLID
    || aAttachShape.ShapeType() == TopAbs_SHELL )
  {
    Bnd_Box aBox;
    BRepBndLib::Add( aAttachShape, aBox );
    const gp_Pnt aMin = aBox.CornerMin();
    const gp_Pnt aMax = aBox.CornerMax();
    return gp_Pnt( (aMin.X() + aMax.X()) / 2.0,
                   (aMin.Y() + aMax.Y()) / 2.0,
                   (aMin.Z() + aMax.Z()) / 2.0 );
  }
  else if ( aAttachShape.ShapeType() == TopAbs_FACE )
  {
    BRepAdaptor_Surface aFace( TopoDS::Face( aAttachShape ) );
    const Standard_Real aU1 = aFace.FirstUParameter();
    const Standard_Real aU2 = aFace.LastUParameter();
    const Standard_Real aV1 = aFace.FirstVParameter();
    const Standard_Real aV2 = aFace.LastVParameter();
    return aFace.Value( ( aU1 + aU2 ) / 2.0, ( aV1 + aV2 ) / 2.0 );
  }
  else if ( aAttachShape.ShapeType() == TopAbs_WIRE )
  {
    BRepAdaptor_CompCurve aWire( TopoDS::Wire( aAttachShape ) );
    const Standard_Real aP1 = aWire.FirstParameter();
    const Standard_Real aP2 = aWire.LastParameter();
    return aWire.Value( ( aP1 + aP2 ) / 2.0 );
  }
  else if ( aAttachShape.ShapeType() == TopAbs_EDGE )
  {
    BRepAdaptor_Curve aEdge( TopoDS::Edge( aAttachShape ) );
    const Standard_Real aP1 = aEdge.FirstParameter();
    const Standard_Real aP2 = aEdge.LastParameter();
    return aEdge.Value( ( aP1 + aP2 ) / 2.0 );
  }
  else if ( aAttachShape.ShapeType() == TopAbs_VERTEX )
  {
    return BRep_Tool::Pnt( TopoDS::Vertex( aAttachShape ) );
  }

  return gp_Pnt( 0.0, 0.0, 0.0 );
}
