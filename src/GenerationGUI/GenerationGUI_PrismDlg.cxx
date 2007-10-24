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
// File   : GenerationGUI_PrismDlg.cxx
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//

#include "GenerationGUI_PrismDlg.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <GEOMImpl_Types.hxx>

//=================================================================================
// class    : GenerationGUI_PrismDlg()
// purpose  : Constructs a GenerationGUI_PrismDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GenerationGUI_PrismDlg::GenerationGUI_PrismDlg( GeometryGUI* theGeometryGUI, QWidget* parent,
						bool modal, Qt::WindowFlags fl )
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_PRISM" ) ) );
  QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );
  QPixmap image2( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_PRISM_2P" ) ) );

  setWindowTitle( tr( "GEOM_EXTRUSION_TITLE" ) );

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_EXTRUSION" ) );
  mainFrame()->RadioButton1->setIcon( image0 );
  mainFrame()->RadioButton2->setIcon( image2 );
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();

  GroupPoints = new DlgRef_2Sel1Spin2Check( centralWidget() );
  GroupPoints->CheckButton1->hide();
  GroupPoints->GroupBox1->setTitle( tr( "GEOM_EXTRUSION_BSV" ) );
  GroupPoints->TextLabel1->setText( tr( "GEOM_BASE" ) );
  GroupPoints->TextLabel2->setText( tr( "GEOM_VECTOR" ) );
  GroupPoints->TextLabel3->setText( tr( "GEOM_HEIGHT" ) );
  GroupPoints->PushButton1->setIcon( image1 );
  GroupPoints->PushButton2->setIcon( image1 );
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );
  GroupPoints->CheckButton2->setText( tr( "GEOM_REVERSE" ) );

  GroupPoints2 = new DlgRef_3Sel( centralWidget() );

  GroupPoints2->GroupBox1->setTitle( tr( "GEOM_EXTRUSION_BSV_2P" ) );
  GroupPoints2->TextLabel1->setText( tr( "GEOM_BASE" ) );
  GroupPoints2->TextLabel2->setText( tr( "GEOM_POINT_I" ).arg( 1 ) );
  GroupPoints2->TextLabel3->setText( tr( "GEOM_POINT_I" ).arg( 2 ) );
  GroupPoints2->PushButton1->setIcon( image1 );
  GroupPoints2->PushButton2->setIcon( image1 );
  GroupPoints2->PushButton3->setIcon( image1 );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( GroupPoints );
  layout->addWidget( GroupPoints2 );
  /***************************************************************/

  setHelpFileName( "extrusion.htm" );

  /* Initialisations */
  Init();
}


//=================================================================================
// function : ~GenerationGUI_PrismDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GenerationGUI_PrismDlg::~GenerationGUI_PrismDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );

  GroupPoints2->LineEdit1->setReadOnly( true );
  GroupPoints2->LineEdit2->setReadOnly( true );
  GroupPoints2->LineEdit3->setReadOnly( true );

  //myOkBase = myOkVec = false;
  myOkBase = myOkVec = myOkPnt1 = myOkPnt2 = false;
  
  /* Get setting of step value from file configuration */
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue( "Geometry", "SettingsGeomStep", 100 );

  /* min, max, step and decimals for spin boxes & initial values */
  initSpinBox( GroupPoints->SpinBox_DX, COORD_MIN, COORD_MAX, step, 3 );
  GroupPoints->SpinBox_DX->setValue( 100.0 );

  /* signals and slots connections */
  connect( buttonOk(),    SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(), SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );

  connect( this,          SIGNAL( constructorsClicked( int ) ), this, SLOT( ConstructorsClicked( int ) ) );

  connect( GroupPoints->PushButton1, SIGNAL( clicked() ), this, SLOT( SetEditCurrentArgument() ) );
  connect( GroupPoints->PushButton2, SIGNAL( clicked() ), this, SLOT( SetEditCurrentArgument() ) );

  connect( GroupPoints->LineEdit1, SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( GroupPoints->LineEdit2, SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );

  connect( GroupPoints->SpinBox_DX, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox() ) );
  // VSR: TODO ->>
  connect( myGeomGUI, SIGNAL( SignalDefaultStepValueChanged( double ) ), GroupPoints->SpinBox_DX, SLOT( SetStep( double ) ) );
  // <<-

  connect( GroupPoints->CheckButton2, SIGNAL( toggled( bool ) ),      this, SLOT( onReverse() ) );

  connect( GroupPoints2->PushButton1, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
  connect( GroupPoints2->PushButton2, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
  connect( GroupPoints2->PushButton3, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );

  connect( GroupPoints2->LineEdit1,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( GroupPoints2->LineEdit2,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( GroupPoints2->LineEdit3,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );


  connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(),
	   SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  initName( tr( "GEOM_EXTRUSION" ) );

  globalSelection( GEOM_ALLSHAPES );
  ConstructorsClicked( 0 );
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void GenerationGUI_PrismDlg::ConstructorsClicked( int constructorId )
{
  disconnect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(), 0, this, 0 );
  
  switch ( constructorId ) {
  case 0:
    {
      globalSelection( GEOM_POINT );
      GroupPoints2->hide();
      GroupPoints->show();
      
      //GroupPoints->LineEdit1->setFocus();
      //myEditCurrentArgument = GroupPoints->LineEdit1;
      //displayPreview();
      
      myEditCurrentArgument = GroupPoints->LineEdit1;
      GroupPoints->LineEdit1->setText( "" );
      GroupPoints->LineEdit2->setText( "" );
      GroupPoints2->LineEdit1->setText( "" );
      GroupPoints2->LineEdit2->setText( "" );
      GroupPoints2->LineEdit3->setText( "" );
      myPoint1 = myPoint2 = myBase = myVec = GEOM::GEOM_Object::_nil();
      myOkBase = myOkVec = myOkPnt1 = myOkPnt2 = false;
      
      break;
    }
  case 1:
    {
      GroupPoints->hide();
      GroupPoints2->show();
      
      myEditCurrentArgument = GroupPoints2->LineEdit1;
      GroupPoints2->LineEdit1->setText( "" );
      GroupPoints2->LineEdit2->setText( "" );
      GroupPoints2->LineEdit3->setText( "" );
      myPoint1 = myPoint2 = myBase = myVec = GEOM::GEOM_Object::_nil();
      myOkBase = myOkVec = myOkPnt1 = myOkPnt2 = false;
      GroupPoints->LineEdit1->setText("");
      GroupPoints->LineEdit2->setText("");
      
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
void GenerationGUI_PrismDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;

  initName();
  return true;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void GenerationGUI_PrismDlg::SelectionIntoArgument()
{
  erasePreview();
  myEditCurrentArgument->setText( "" );

  if ( getConstructorId() == 0 ) {

    if ( IObjectCount() != 1 ) {
      if ( myEditCurrentArgument == GroupPoints->LineEdit1 )
        myOkBase = false;
      else if ( myEditCurrentArgument == GroupPoints->LineEdit2 )
        myOkVec = false;
      return;
    }

    // nbSel == 1
    Standard_Boolean testResult = Standard_False;
    GEOM::GEOM_Object_ptr aSelectedObject =
      GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );

    if ( !testResult )
      return;

    if ( myEditCurrentArgument == GroupPoints->LineEdit1 ) {
      myBase = aSelectedObject;
      myOkBase = true;
    }
    else if ( myEditCurrentArgument == GroupPoints->LineEdit2 ) {
      myVec = aSelectedObject;
      myOkVec = true;
    }
    myEditCurrentArgument->setText( GEOMBase::GetName( aSelectedObject ) );

    displayPreview();
  }

  else { // getConstructorId()==1 - extrusion using 2 points

    if ( IObjectCount() != 1 ) {
      if ( myEditCurrentArgument == GroupPoints2->LineEdit1 )
        myOkBase = false;
      else if ( myEditCurrentArgument == GroupPoints2->LineEdit2 ) {
	myPoint1 = GEOM::GEOM_Object::_nil();
        myOkPnt1 = false;
      }
      else if ( myEditCurrentArgument == GroupPoints2->LineEdit3 ) {
	myPoint2 = GEOM::GEOM_Object::_nil();
        myOkPnt2 = false;
      }
      return;
    }

    // nbSel == 1
    Standard_Boolean testResult = Standard_False;
    GEOM::GEOM_Object_var aSelectedObject =
      GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );

    if ( !testResult || CORBA::is_nil( aSelectedObject ) )
      return;

    if ( myEditCurrentArgument == GroupPoints2->LineEdit1 ) {
      myBase = aSelectedObject;
      myOkBase = true;
    }
    else if ( myEditCurrentArgument == GroupPoints2->LineEdit2 ) {
      myPoint1 = aSelectedObject;
      myOkPnt1 = true;
    }
    else if ( myEditCurrentArgument == GroupPoints2->LineEdit3 ) {
      myPoint2 = aSelectedObject;
      myOkPnt2 = true;
    }

    myEditCurrentArgument->setText( GEOMBase::GetName( aSelectedObject ) );

    displayPreview();
  }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  globalSelection( GEOM_ALLSHAPES );

  if ( send == GroupPoints->PushButton1 ) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
  }
  else if ( send == GroupPoints->PushButton2 ) {
    GroupPoints->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit2;
    globalSelection( GEOM_LINE );
  }
  else if ( send == GroupPoints2->PushButton1 ) {
    GroupPoints2->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit1;
  }
  else if ( send == GroupPoints2->PushButton2 ) {
    GroupPoints2->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit2;
    globalSelection( GEOM_POINT );
  }
  else if ( send == GroupPoints2->PushButton3 ) {
    GroupPoints2->LineEdit3->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit3;
    globalSelection( GEOM_POINT );
  }
  SelectionIntoArgument();
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if ( send == GroupPoints->LineEdit1 ||
       send == GroupPoints->LineEdit2 ||
       send == GroupPoints2->LineEdit1 ||
       send == GroupPoints2->LineEdit2 ||
       send == GroupPoints2->LineEdit3 ) {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}

//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void GenerationGUI_PrismDlg::enterEvent( QEvent* )
{
  if ( !mainFrame()->GroupConstructors->isEnabled() )
    ActivateThisDialog();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  globalSelection( GEOM_ALLSHAPES );
  connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(),
	   SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  //GroupPoints->LineEdit1->setFocus();
  //myEditCurrentArgument = GroupPoints->LineEdit1;
  //displayPreview();
  ConstructorsClicked( getConstructorId() );
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::ValueChangedInSpinBox()
{
  displayPreview();
}


//=================================================================================
// function : getHeight()
// purpose  :
//=================================================================================
double GenerationGUI_PrismDlg::getHeight() const
{
  return GroupPoints->SpinBox_DX->value();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr GenerationGUI_PrismDlg::createOperation()
{
  return getGeomEngine()->GetI3DPrimOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::isValid( QString& )
{
  return myOkBase && ( myOkVec || (myOkPnt1 && myOkPnt2) );
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::execute( ObjectList& objects )
{
  GEOM::GEOM_Object_var anObj;

  switch ( getConstructorId() ) {
  case 0 :
    anObj = GEOM::GEOM_I3DPrimOperations::_narrow( getOperation() )->
      MakePrismVecH( myBase, myVec, getHeight() );
    break;
  case 1 :
    anObj = GEOM::GEOM_I3DPrimOperations::_narrow( getOperation() )->
      MakePrismTwoPnt( myBase, myPoint1, myPoint2 );
    break;
  }
  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return true;
}


//=================================================================================
// function :  onReverse()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::onReverse()
{
  double anOldValue = GroupPoints->SpinBox_DX->value();
  GroupPoints->SpinBox_DX->setValue( -anOldValue );
}
