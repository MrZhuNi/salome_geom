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
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_VECTOR_2P" ) ) );

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_VECTOR"));
  mainFrame()->RadioButton1->setIcon(image0);
  mainFrame()->RadioButton2->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();

  myCurve = new CurveCreator_Curve( CurveCreator::Dim2d );
  myEditorWidget = new CurveCreator_Widget (centralWidget(), myCurve);
  myAddElementBox = new QGroupBox (tr("ADD_ELEMENT"), centralWidget());

  QBoxLayout* anAddElementLayout = new QVBoxLayout( myAddElementBox );

  anAddElementLayout->setMargin( 0 );
  anAddElementLayout->setSpacing( 6 );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );

  layout->setMargin( 0 );
  layout->setSpacing( 6 );
  layout->addWidget( myEditorWidget );
  layout->addWidget( myAddElementBox );

  /***************************************************************/

  setHelpFileName( "create_vector_page.html" );

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
  SalomeApp_Application *anApp        = myGeomGUI->getApp();
  OCCViewer_ViewManager *aViewManager = dynamic_cast<OCCViewer_ViewManager*>
    (anApp->getViewManager(OCCViewer_Viewer::Type(), true));

  myEditorWidget->setOCCViewer( aViewManager ? aViewManager->getOCCViewer() : 0 );

  connect(myEditorWidget, SIGNAL(subOperationStarted(QWidget*, bool)),
          this,           SLOT(processStartedSubOperation(QWidget*, bool)));
  connect(myEditorWidget, SIGNAL(subOperationFinished(QWidget*)),
          this,           SLOT(processFinishedSubOperation(QWidget*)));

  myAddElementBox->hide();
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
  return true;
}

void EntityGUI_PolylineDlg::processStartedSubOperation( QWidget* theWidget, bool theIsEdit )
{
  myEditorWidget->setEnabled( false );

  myAddElementBox->setTitle( theIsEdit ? tr( "EDIT_ELEMENT" ) : tr( "ADD_ELEMENT" ) );
  QBoxLayout* anAddElementLayout = dynamic_cast<QBoxLayout*>( myAddElementBox->layout() );
  anAddElementLayout->addWidget( theWidget );

  theWidget->show();
  myAddElementBox->show();
}

void EntityGUI_PolylineDlg::processFinishedSubOperation( QWidget* theWidget )
{
  myEditorWidget->setEnabled( true );

  QBoxLayout* anAddElementLayout = dynamic_cast<QBoxLayout*>( myAddElementBox->layout() );
  anAddElementLayout->removeWidget( theWidget );

  theWidget->hide();
  myAddElementBox->hide();
}

/**
 * Redirect the delete action to editor widget
 */
void EntityGUI_PolylineDlg::deleteSelected()
{
  myEditorWidget->removeSelected();
}

/**
 * Checks whether there are some to delete
 */
bool EntityGUI_PolylineDlg::deleteEnabled()
{
  return myEditorWidget->removeEnabled();
}
