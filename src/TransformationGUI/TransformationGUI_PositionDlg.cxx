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
// File   : TransformationGUI_PositionDlg.cxx
// Author : Damien COQUERET, Open CASCADE S.A.S.
//

#include "TransformationGUI_PositionDlg.h"

#include <GEOM_DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <TColStd_MapOfInteger.hxx>

#include <GEOMImpl_Types.hxx>

//=================================================================================
// class    : TransformationGUI_PositionDlg()
// purpose  : Constructs a TransformationGUI_PositionDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
TransformationGUI_PositionDlg::TransformationGUI_PositionDlg
( GeometryGUI* theGeometryGUI, QWidget* parent, bool modal, Qt::WindowFlags fl )
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
  SUIT_ResourceMgr* aResMgr = myGeomGUI->getApp()->resourceMgr();
  QPixmap image0( aResMgr->loadPixmap( "GEOM", tr( "ICON_DLG_POSITION" ) ) );
  QPixmap image1( aResMgr->loadPixmap( "GEOM", tr( "ICON_DLG_POSITION2" ) ) );
  QPixmap imageselect( aResMgr->loadPixmap( "GEOM", tr( "ICON_SELECT") ) );

  setWindowTitle( tr( "GEOM_POSITION_TITLE" ) );

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_POSITION" ) );
  mainFrame()->RadioButton1->setIcon( image0 );
  mainFrame()->RadioButton2->setIcon( image1 );
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();
 
  Group1 = new DlgRef_3Sel3Spin1Check( centralWidget() );
  Group1->SpinBox1->hide();
  Group1->SpinBox2->hide();
  Group1->SpinBox3->hide();
  Group1->TextLabel4->hide();
  Group1->TextLabel5->hide();
  Group1->TextLabel6->hide();
  Group1->GroupBox1->setTitle( tr( "GEOM_ARGUMENTS" ) );
  Group1->TextLabel1->setText( tr( "GEOM_OBJECTS" ) );
  Group1->TextLabel2->setText( tr( "GEOM_START_LCS" ) );
  Group1->TextLabel3->setText( tr( "GEOM_END_LCS" ) );
  Group1->PushButton1->setIcon( imageselect );
  Group1->PushButton2->setIcon( imageselect );
  Group1->PushButton3->setIcon( imageselect );
  Group1->CheckBox1->setText( tr( "GEOM_CREATE_COPY" ) );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( Group1 );
  
  /***************************************************************/

  setHelpFileName( "modify_the_location.htm" );
  
  Init();
}


//=================================================================================
// function : ~TransformationGUI_PositionDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
TransformationGUI_PositionDlg::~TransformationGUI_PositionDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::Init()
{  
  /* init variables */
  Group1->LineEdit1->setReadOnly( true );
  Group1->LineEdit2->setReadOnly( true );
  Group1->LineEdit3->setReadOnly( true );
  
  myStartLCS = GEOM::GEOM_Object::_nil();
  myEndLCS = GEOM::GEOM_Object::_nil();
    
  // Activate Create a Copy mode
  Group1->CheckBox1->setChecked( true );
  CreateCopyModeChanged( true );

  /* signals and slots connections */
  connect( buttonOk(),    SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(), SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );

  connect( this, SIGNAL( constructorsClicked( int ) ), this, SLOT( ConstructorsClicked( int ) ) );

  connect( Group1->PushButton1, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
  connect( Group1->PushButton2, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
  connect( Group1->PushButton3, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );

  connect( Group1->LineEdit1,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( Group1->LineEdit2,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  connect( Group1->LineEdit3,   SIGNAL( returnPressed() ), this, SLOT( LineEditReturnPressed() ) );
  
  connect( Group1->CheckBox1,   SIGNAL( toggled( bool ) ), this, SLOT( CreateCopyModeChanged( bool ) ) );
  
  connect( myGeomGUI->getApp()->selectionMgr(), 
	   SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  initName( tr( "GEOM_POSITION" ) );

  ConstructorsClicked( 0 );
}



//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void TransformationGUI_PositionDlg::ConstructorsClicked( int constructorId )
{
  disconnect( myGeomGUI->getApp()->selectionMgr(), 0, this, 0 );
  
  globalSelection();
  myEditCurrentArgument = Group1->LineEdit1;
  Group1->LineEdit2->clear();
  Group1->LineEdit3->clear();
  myStartLCS = GEOM::GEOM_Object::_nil();
  myEndLCS = GEOM::GEOM_Object::_nil();
  
  switch ( constructorId ) {
  case 0:
    Group1->LineEdit2->hide();
    Group1->TextLabel2->hide();
    Group1->PushButton2->hide();
    break;
  case 1:
    Group1->LineEdit2->show();
    Group1->TextLabel2->show();
    Group1->PushButton2->show();
    break;
  }
  
  qApp->processEvents();
  updateGeometry();
  resize( minimumSize() );

  connect( myGeomGUI->getApp()->selectionMgr(),
	   SIGNAL( currentSelectionChanged() ), this, SLOT(SelectionIntoArgument() ) );
  SelectionIntoArgument();
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool TransformationGUI_PositionDlg::ClickOnApply()
{
  if ( !onAccept( Group1->CheckBox1->isChecked() ) )
    return false;
  
  initName();
  ConstructorsClicked( getConstructorId() );
  return true;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void TransformationGUI_PositionDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText( "" );
  QString aName;

  if ( myEditCurrentArgument == Group1->LineEdit1 ) {
    int aNbSel = GEOMBase::GetNameOfSelectedIObjects( selectedIO(), aName );
    if ( aNbSel < 1 ) {
      myObjects.length( 0 );
      displayPreview();
      return;
    }
    GEOMBase::ConvertListOfIOInListOfGO( selectedIO(), myObjects );
    if ( !myObjects.length() ) {
      displayPreview();
      return;
    }
  }
  else if ( myEditCurrentArgument == Group1->LineEdit2 ) {
    myStartLCS = GEOM::GEOM_Object::_nil();
    if ( IObjectCount() != 1 ) {
      displayPreview();
      return;
    }

    Standard_Boolean testResult = Standard_False;
    myStartLCS = GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );
    if ( !testResult || CORBA::is_nil( myStartLCS ) ) {
      displayPreview();
      return;
    }
    aName = GEOMBase::GetName( myStartLCS );
  }
  else if ( myEditCurrentArgument == Group1->LineEdit3 ) {
    myEndLCS = GEOM::GEOM_Object::_nil();
    if ( IObjectCount() != 1 ) {
      displayPreview();
      return;
    }
    
    Standard_Boolean testResult = Standard_False;
    myEndLCS = GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );
    if ( !testResult || CORBA::is_nil( myEndLCS ) ) {
      displayPreview();
      return;
    }
    aName = GEOMBase::GetName( myEndLCS );
  }

  myEditCurrentArgument->setText( aName );
  displayPreview(); 
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if ( send == Group1->LineEdit1 || send == Group1->LineEdit2 || 
       send == Group1->LineEdit3 ) {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  
  if ( send == Group1->PushButton1 ) {
    myEditCurrentArgument = Group1->LineEdit1;
    globalSelection();
  }
  else if ( send == Group1->PushButton2 ) {
    myEditCurrentArgument = Group1->LineEdit2;
    TColStd_MapOfInteger aMap;
    aMap.Add( GEOM_PLANE );
    aMap.Add( GEOM_MARKER );
    globalSelection( aMap );
  }
  else if ( send == Group1->PushButton3 ) {
    myEditCurrentArgument = Group1->LineEdit3;
    TColStd_MapOfInteger aMap;
    aMap.Add( GEOM_PLANE );
    aMap.Add( GEOM_MARKER );
    globalSelection( aMap );
  }

  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect( myGeomGUI->getApp()->selectionMgr(), 
	   SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  ConstructorsClicked( getConstructorId() );
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void TransformationGUI_PositionDlg::enterEvent( QEvent* )
{
  if ( !mainFrame()->GroupConstructors->isEnabled() )
    ActivateThisDialog();
}


//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr  TransformationGUI_PositionDlg::createOperation()
{
  return getGeomEngine()->GetITransformOperations( getStudyId() );
}


//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool TransformationGUI_PositionDlg::isValid( QString& /*msg*/ )
{
  bool res;
  if ( getConstructorId() == 0 )
    res = !( myObjects.length() == 0 || myEndLCS->_is_nil() );
  else
    res = !( myObjects.length() == 0 || myStartLCS->_is_nil() || myEndLCS->_is_nil() );

  return res;
}


//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool TransformationGUI_PositionDlg::execute( ObjectList& objects )
{
  bool res = false;
  bool toCreateCopy = IsPreview() || Group1->CheckBox1->isChecked();
  GEOM::GEOM_Object_var anObj;
  
  switch ( getConstructorId() ) {
  case 0:
    {
      for ( int i = 0; i < myObjects.length(); i++ ) {
	if ( toCreateCopy )
	  anObj = GEOM::GEOM_ITransformOperations::_narrow( getOperation() )->PositionShapeCopy( myObjects[i], myObjects[i], myEndLCS );
	else
	  anObj = GEOM::GEOM_ITransformOperations::_narrow( getOperation() )->PositionShape( myObjects[i], myObjects[i], myEndLCS );
	
	if ( !anObj->_is_nil() )
	  objects.push_back( anObj._retn() );
      }
      res = true;
      break;
    }
  case 1:
    {
      for ( int i = 0; i < myObjects.length(); i++ ) {
	if ( toCreateCopy )
	  anObj = GEOM::GEOM_ITransformOperations::_narrow( getOperation() )->PositionShapeCopy( myObjects[i], myStartLCS, myEndLCS );
	else
	  anObj = GEOM::GEOM_ITransformOperations::_narrow( getOperation() )->PositionShape( myObjects[i], myStartLCS, myEndLCS );
	if ( !anObj->_is_nil() )
	  objects.push_back( anObj._retn() );
      }
      res = true;
      break;
    }
  }
  
  return res;
}

//=================================================================================
// function : closeEvent
// purpose  :
//=================================================================================
void  TransformationGUI_PositionDlg::closeEvent( QCloseEvent* e )
{
  GEOMBase_Skeleton::closeEvent( e );
}


//=================================================================================
// function :  CreateCopyModeChanged()
// purpose  :
//=================================================================================
void TransformationGUI_PositionDlg::CreateCopyModeChanged( bool isCreateCopy )
{
  mainFrame()->GroupBoxName->setEnabled( isCreateCopy );
}
