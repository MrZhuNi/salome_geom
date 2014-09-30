// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

#include "EntityGUI_PolylineDlg.h"
#include <CurveCreator_Curve.hxx>
#include <CurveCreator_Widget.h>
#include <DlgRef.h>
#include <GeometryGUI.h>

#include <OCCViewer_ViewManager.h>
#include <SalomeApp_Application.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>

#include <QGroupBox>
#include <QVBoxLayout>


//=================================================================================
// function : Constructor
// purpose  :
//=================================================================================
EntityGUI_PolylineDlg::EntityGUI_PolylineDlg
        (GeometryGUI* theGeometryGUI, QWidget* parent, bool modal, Qt::WindowFlags fl)
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_CC_POLYLINE" ) ) );

  setWindowTitle(tr("POLYLINE_DLG_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("POLYLINE_TITLE"));
  mainFrame()->RadioButton1->setIcon(image0);
  mainFrame()->RadioButton2->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();

  myCurve = new CurveCreator_Curve( CurveCreator::Dim2d );
  myEditorWidget = new CurveCreator_Widget (centralWidget(), myCurve);
  myAddElementBox = new QGroupBox (tr("POLYLINE_ADD_SECTION"), centralWidget());

  QBoxLayout* anAddElementLayout = new QVBoxLayout( myAddElementBox );

  anAddElementLayout->setMargin( 0 );
  anAddElementLayout->setSpacing( 6 );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );

  layout->setMargin( 0 );
  layout->setSpacing( 6 );
  layout->addWidget( myEditorWidget );
  layout->addWidget( myAddElementBox );

  /***************************************************************/

  setHelpFileName( "create_polyline_page.html" );

  /* Initialisations */
  Init();
}

//=================================================================================
// function : Destructor
// purpose  :
//=================================================================================
EntityGUI_PolylineDlg::~EntityGUI_PolylineDlg()
{
  delete myCurve;
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::Init()
{
  initName(tr("POLYLINE_NAME"));

  SalomeApp_Application *anApp        = myGeomGUI->getApp();
  OCCViewer_ViewManager *aViewManager = dynamic_cast<OCCViewer_ViewManager*>
    (anApp->getViewManager(OCCViewer_Viewer::Type(), true));

  myEditorWidget->setOCCViewer( aViewManager ? aViewManager->getOCCViewer() : 0 );

  connect(myEditorWidget, SIGNAL(subOperationStarted(QWidget*, bool)),
          this,           SLOT(processStartedSubOperation(QWidget*, bool)));
  connect(myEditorWidget, SIGNAL(subOperationFinished(QWidget*)),
          this,           SLOT(processFinishedSubOperation(QWidget*)));
  connect(myEditorWidget, SIGNAL(curveModified()),
          this,           SLOT(onUpdatePreview()));
  connect(buttonOk(),    SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  myAddElementBox->hide();

  SelectionIntoArgument();
}

//=================================================================================
// function : Clear
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::Clear()
{
  delete myCurve;

  myCurve = new CurveCreator_Curve( CurveCreator::Dim2d );
  myEditorWidget->setCurve(myCurve);
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::GetCurveParams(GEOM::ListOfListOfDouble &theCoords,
                                           GEOM::string_array       &theNames,
                                           GEOM::short_array        &theTypes,
                                           GEOM::ListOfBool         &theCloseds)
{
  const int aNbSec = myCurve->getNbSections();
  int       i;
  int       j;

  theCoords.length(aNbSec);
  theNames.length(aNbSec);
  theTypes.length(aNbSec);
  theCloseds.length(aNbSec);

  for (i = 0; i < aNbSec; ++i) {
    // Set coordinates
    CurveCreator::Coordinates aCoords   = myCurve->getPoints(i);
    const int                 aNbPoints = aCoords.size();

    theCoords[i].length(aNbPoints);

    for (j = 0; j < aNbPoints; ++j) {
      theCoords[i][j] = aCoords[j];
    }

    // Set section type
    const CurveCreator::SectionType aType = myCurve->getSectionType(i);

    switch (aType) {
      case CurveCreator::Spline:
        theTypes[i] = GEOM::Interpolation;
        break;
      case CurveCreator::Polyline:
      default:
        theTypes[i] = GEOM::Polyline;
        break;
    }

    // Set section names and closed flags.
    theNames[i]   = CORBA::string_dup(myCurve->getSectionName(i).c_str());
    theCloseds[i] = myCurve->isClosed(i);
  }
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr EntityGUI_PolylineDlg::createOperation()
{
  return getGeomEngine()->GetICurvesOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool EntityGUI_PolylineDlg::isValid( QString& msg )
{
  return true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool EntityGUI_PolylineDlg::execute( ObjectList& objects )
{
  GEOM::GEOM_ICurvesOperations_var anOper =
    GEOM::GEOM_ICurvesOperations::_narrow(getOperation());

  // Get the polyline creation parameters.
  GEOM::ListOfListOfDouble aCoords;
  GEOM::string_array       aNames;
  GEOM::short_array        aTypes;
  GEOM::ListOfBool         aCloseds;

  GetCurveParams(aCoords, aNames, aTypes, aCloseds);

  // Temporary code: get Working Plane.
  GEOM::GEOM_IBasicOperations_var aBasicOp = getGeomEngine()->GetIBasicOperations( getStudyId() );
  GEOM::GEOM_Object_var           aWPlane  = aBasicOp->MakeMarker( 0,0,0,
                                                                   1,0,0,
                                                                   0,1,0 );

  // Perform operation
  GEOM::GEOM_Object_var anObj = anOper->MakePolyline2DOnPlane
    (aCoords, aNames, aTypes, aCloseds, aWPlane);

  if (!anObj->_is_nil()) {
    objects.push_back(anObj._retn());
  }

  return true;
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::ClickOnOk()
{
  setIsApplyAndClose( true );

  if (ClickOnApply())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool EntityGUI_PolylineDlg::ClickOnApply()
{
  if (!onAccept())
    return false;

  initName();

  return true;
}

//=================================================================================
// function : processStartedSubOperation
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::processStartedSubOperation( QWidget* theWidget, bool theIsEdit )
{
  myEditorWidget->setEnabled( false );

  myAddElementBox->setTitle( theIsEdit ? tr( "POLYLINE_EDIT_SECTION" ) : tr( "POLYLINE_ADD_SECTION" ) );
  QBoxLayout* anAddElementLayout = dynamic_cast<QBoxLayout*>( myAddElementBox->layout() );
  anAddElementLayout->addWidget( theWidget );

  theWidget->show();
  myAddElementBox->show();
}


//=================================================================================
// function : processFinishedSubOperation
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::processFinishedSubOperation( QWidget* theWidget )
{
  myEditorWidget->setEnabled( true );

  QBoxLayout* anAddElementLayout = dynamic_cast<QBoxLayout*>( myAddElementBox->layout() );
  anAddElementLayout->removeWidget( theWidget );

  theWidget->hide();
  myAddElementBox->hide();
}

//=================================================================================
// function : execute
// purpose  : Redirect the delete action to editor widget
//=================================================================================
void EntityGUI_PolylineDlg::deleteSelected()
{
  myEditorWidget->removeSelected();
}

//=================================================================================
// function : deleteEnabled
// purpose  : Checks whether there are some to delete
//=================================================================================
bool EntityGUI_PolylineDlg::deleteEnabled()
{
  return myEditorWidget->removeEnabled();
}

//=================================================================================
// function : SelectionIntoArgument
// purpose  : Called when selection is changed
//=================================================================================
void EntityGUI_PolylineDlg::SelectionIntoArgument()
{
/*
  GEOM::GeomObjPtr aSelectedObject = getSelected( TopAbs_SHAPE );
  TopoDS_Shape aShape;

  if ( aSelectedObject && GEOMBase::GetShape( aSelectedObject.get(), aShape ) && !aShape.IsNull() ) {
    if (aShape.ShapeType() == TopAbs_FACE) {
      QString aName = GEOMBase::GetName( aSelectedObject.get() );
      myGroup->LineEdit1->setText( aName );

      // clear selection
      disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
      myGeomGUI->getApp()->selectionMgr()->clearSelected();
      connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
              this, SLOT(SelectionIntoArgument()));

      myFace = aSelectedObject;
    }
  }

  displayPreview(true);
*/
}

//=================================================================================
// function : ActivateThisDialog
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();

  SelectionIntoArgument();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void EntityGUI_PolylineDlg::enterEvent (QEvent*)
{
  if (!mainFrame()->GroupConstructors->isEnabled())
    ActivateThisDialog();
}

//=================================================================================
// function : onUpdatePreview
// purpose  : 
//=================================================================================
void EntityGUI_PolylineDlg::onUpdatePreview()
{
  displayPreview(true);
}
