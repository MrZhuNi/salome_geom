// GEOM GEOMGUI : GUI for Geometry component
//
// Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// File   : BasicGUI_VectorDlg.cxx
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//

#include "BasicGUI_VectorDlg.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <GEOMImpl_Types.hxx>

//=================================================================================
// class    : BasicGUI_VectorDlg()
// purpose  : Constructs a BasicGUI_VectorDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BasicGUI_VectorDlg::BasicGUI_VectorDlg( GeometryGUI* theGeometryGUI, QWidget* parent,
					bool modal, Qt::WindowFlags fl )
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_VECTOR_2P" ) ) );
  QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_VECTOR_DXYZ" ) ) );
  QPixmap image2( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );

  setWindowTitle( tr( "GEOM_VECTOR_TITLE" ) );

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_VECTOR"));
  mainFrame()->RadioButton1->setIcon(image0);
  mainFrame()->RadioButton2->setIcon(image1);
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();

  GroupPoints = new DlgRef_2Sel( centralWidget() );

  GroupPoints->GroupBox1->setTitle( tr( "GEOM_POINTS" ) );
  GroupPoints->TextLabel1->setText( tr( "GEOM_POINT_I" ).arg( 1 ) );
  GroupPoints->TextLabel2->setText( tr( "GEOM_POINT_I" ).arg( 2 ) );
  GroupPoints->PushButton1->setIcon( image2 );
  GroupPoints->PushButton2->setIcon( image2 );

  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );

  GroupDimensions = new DlgRef_3Spin1Check( centralWidget() );
  GroupDimensions->GroupBox1->setTitle( tr( "GEOM_COORDINATES" ) );
  GroupDimensions->TextLabel1->setText( tr( "GEOM_DX" ) );
  GroupDimensions->TextLabel2->setText( tr( "GEOM_DY" ) );
  GroupDimensions->TextLabel3->setText( tr( "GEOM_DZ" ) );
  GroupDimensions->CheckButton1->setText( tr( "GEOM_REVERSE_VECTOR" ) );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( GroupPoints );
  layout->addWidget( GroupDimensions );
  /***************************************************************/

  setHelpFileName( "vector.htm" );
  
  /* Initialisations */
  Init();
}


//=================================================================================
// function : ~BasicGUI_VectorDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BasicGUI_VectorDlg::~BasicGUI_VectorDlg()
{  
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::Init()
{
    /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myPoint1 = GEOM::GEOM_Object::_nil();
  myPoint2 = GEOM::GEOM_Object::_nil();

  /* Get setting of step value from file configuration */
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue( "Geometry", "SettingsGeomStep", 100 );
 
  /* min, max, step and decimals for spin boxes */
  initSpinBox( GroupDimensions->SpinBox_DX, COORD_MIN, COORD_MAX, step, 3 );
  initSpinBox( GroupDimensions->SpinBox_DY, COORD_MIN, COORD_MAX, step, 3 );
  initSpinBox( GroupDimensions->SpinBox_DZ, COORD_MIN, COORD_MAX, step, 3 );

  double dx( 0. ), dy( 0. ), dz( 200. );
  GroupDimensions->SpinBox_DX->setValue( dx );
  GroupDimensions->SpinBox_DY->setValue( dy );
  GroupDimensions->SpinBox_DZ->setValue( dz );

  GroupDimensions->CheckButton1->setChecked( false );

  /* signals and slots connections */
  connect( myGeomGUI, SIGNAL( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) );
  connect( myGeomGUI, SIGNAL( SignalCloseAllDialogs() ),        this, SLOT( ClickOnCancel() ) );
  
  connect( buttonOk(),     SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(),  SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );

  connect( this,           SIGNAL( constructorsClicked( int ) ), this, SLOT( ConstructorsClicked( int ) ) );

  connect( GroupPoints->PushButton1, SIGNAL( clicked() ), this, SLOT( SetEditCurrentArgument() ) );
  connect( GroupPoints->PushButton2, SIGNAL( clicked() ), this, SLOT( SetEditCurrentArgument() ) );

  connect( GroupPoints->LineEdit1, SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( GroupPoints->LineEdit2, SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );

  connect( GroupDimensions->SpinBox_DX, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );
  connect( GroupDimensions->SpinBox_DY, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );
  connect( GroupDimensions->SpinBox_DZ, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );

  // VSR: TODO ->>
  connect( myGeomGUI, SIGNAL( SignalDefaultStepValueChanged( double ) ), GroupDimensions->SpinBox_DX, SLOT( SetStep( double ) ) );
  connect( myGeomGUI, SIGNAL( SignalDefaultStepValueChanged( double ) ), GroupDimensions->SpinBox_DY, SLOT( SetStep( double ) ) );
  connect( myGeomGUI, SIGNAL( SignalDefaultStepValueChanged( double ) ), GroupDimensions->SpinBox_DZ, SLOT( SetStep( double ) ) );
  // <<-

  connect( GroupDimensions->CheckButton1, SIGNAL( stateChanged( int ) ), this, SLOT( ReverseVector( int ) ) );

  connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(), 
	   SIGNAL( currentSelectionChanged() ), this, SLOT(SelectionIntoArgument() ) );

  initName( tr("GEOM_VECTOR") );

  ConstructorsClicked( 0 );
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void BasicGUI_VectorDlg::ConstructorsClicked( int constructorId )
{
  disconnect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(), 0, this, 0 );
  myPoint1 = GEOM::GEOM_Object::_nil();
  myPoint2 = GEOM::GEOM_Object::_nil();

  switch ( constructorId ) {
  case 0:
    {
      GroupDimensions->hide();
      GroupPoints->show();
      
      myEditCurrentArgument = GroupPoints->LineEdit1;
      GroupPoints->LineEdit1->setText( "" );
      GroupPoints->LineEdit2->setText( "" );
      
      globalSelection( GEOM_POINT );
      connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(), 
	       SIGNAL( currentSelectionChanged() ), this, SLOT(SelectionIntoArgument() ) );
      break;
    }
  case 1:
    {
      GroupPoints->hide();
      GroupDimensions->show();
      
      double dx( 0. ), dy( 0. ), dz( 0. ); 
      GroupDimensions->SpinBox_DX->setValue( dx );
      GroupDimensions->SpinBox_DY->setValue( dy );
      GroupDimensions->SpinBox_DZ->setValue( dz );
      
      GroupDimensions->CheckButton1->setChecked( false );
      break;
    }
  }

  qApp->processEvents();
  updateGeometry();
  resize( minimumSize() );

  displayPreview();
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BasicGUI_VectorDlg::ClickOnApply()
{
  buttonApply()->setFocus();

  if ( !onAccept() )
    return false;

  initName();
  if ( getConstructorId() != 1 )
  	ConstructorsClicked( getConstructorId() );
  return true;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void BasicGUI_VectorDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText( "" );

  if ( IObjectCount() != 1 ) {
    if ( myEditCurrentArgument == GroupPoints->LineEdit1 )
      myPoint1 = GEOM::GEOM_Object::_nil();
    else if ( myEditCurrentArgument == GroupPoints->LineEdit2 )
      myPoint2 = GEOM::GEOM_Object::_nil();
    return;
  }

  // nbSel == 1 
  Standard_Boolean aRes = Standard_False;
  GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject( firstIObject(), aRes );
  if ( !CORBA::is_nil( aSelectedObject ) && aRes ) {
    myEditCurrentArgument->setText( GEOMBase::GetName( aSelectedObject ) );
    if      ( myEditCurrentArgument == GroupPoints->LineEdit1 ) myPoint1 = aSelectedObject;
    else if ( myEditCurrentArgument == GroupPoints->LineEdit2 ) myPoint2 = aSelectedObject;
  }

  displayPreview();
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  if      ( send == GroupPoints->PushButton1 ) myEditCurrentArgument = GroupPoints->LineEdit1;
  else if ( send == GroupPoints->PushButton2 ) myEditCurrentArgument = GroupPoints->LineEdit2;
  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::LineEditReturnPressed()
{ 
  QLineEdit* send = (QLineEdit*)sender();
  if      ( send == GroupPoints->LineEdit1 ) myEditCurrentArgument = GroupPoints->LineEdit1;
  else if ( send == GroupPoints->LineEdit2 ) myEditCurrentArgument = GroupPoints->LineEdit2;
  else return;
  GEOMBase_Skeleton::LineEditReturnPressed();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(), 
	   SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
	
  ConstructorsClicked( getConstructorId() );
}

//=================================================================================
// function : DeactivateActiveDialog()
// purpose  : public slot to deactivate if active
//=================================================================================
void BasicGUI_VectorDlg::DeactivateActiveDialog()
{
  // myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::DeactivateActiveDialog();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::enterEvent( QEvent* )
{
  if ( !mainFrame()->GroupConstructors->isEnabled() )
    ActivateThisDialog();
}

//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void BasicGUI_VectorDlg::ValueChangedInSpinBox( double newValue )
{
  displayPreview();
}

//=================================================================================
// function : ReverseVector()
// purpose  : 'state' not used here
//=================================================================================
void BasicGUI_VectorDlg::ReverseVector( int state )
{
  double dx = -GroupDimensions->SpinBox_DX->value();
  double dy = -GroupDimensions->SpinBox_DY->value();
  double dz = -GroupDimensions->SpinBox_DZ->value();

  GroupDimensions->SpinBox_DX->setValue( dx );
  GroupDimensions->SpinBox_DY->setValue( dy );
  GroupDimensions->SpinBox_DZ->setValue( dz );
  
  displayPreview();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BasicGUI_VectorDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetIBasicOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BasicGUI_VectorDlg::isValid( QString& msg )
{
  return getConstructorId() == 0 ? !myPoint1->_is_nil() && !myPoint2->_is_nil() : true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BasicGUI_VectorDlg::execute( ObjectList& objects )
{
  bool res = false;
  
  GEOM::GEOM_Object_var anObj;

  switch ( getConstructorId() ) {
  case 0 :
    {
      anObj = GEOM::GEOM_IBasicOperations::_narrow( getOperation() )->MakeVectorTwoPnt( myPoint1, myPoint2 );
      res = true;
      break;
    }
  case 1 :
    {
      double dx = GroupDimensions->SpinBox_DX->value();
      double dy = GroupDimensions->SpinBox_DY->value();
      double dz = GroupDimensions->SpinBox_DZ->value();
      anObj = GEOM::GEOM_IBasicOperations::_narrow( getOperation() )->MakeVectorDXDYDZ( dx, dy, dz );
      res = true;
      break;
    }
  }

  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return res;
}

