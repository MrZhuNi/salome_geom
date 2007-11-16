//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : BasicGUI_LineDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#include "BasicGUI_LineDlg.h"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "SalomeApp_Application.h"
#include "LightApp_SelectionMgr.h"

#include <qlabel.h>

#include "GEOMImpl_Types.hxx"

#include "utilities.h"

using namespace std;

//=================================================================================
// class    : BasicGUI_LineDlg()
// purpose  : Constructs a BasicGUI_LineDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BasicGUI_LineDlg::BasicGUI_LineDlg(GeometryGUI* theGeometryGUI, QWidget* parent,
                                   const char* name, bool modal, WFlags fl)
  :GEOMBase_Skeleton(theGeometryGUI, parent, name, modal, WStyle_Customize |
                     WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM",tr("ICON_DLG_LINE_2P")));
  QPixmap image1(SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM",tr("ICON_SELECT")));
  QPixmap image2(SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM",tr("ICON_DLG_LINE_2F")));

  setCaption(tr("GEOM_LINE_TITLE"));

  QGroupBox* aFrame = new QGroupBox( 1, Qt::Horizontal, this );
  aFrame->setInsideMargin( 0 );
  aFrame->setFrameStyle( QFrame::NoFrame );

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_LINE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image2);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_2Sel_QTD( aFrame, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_POINTS"));
  GroupPoints->TextLabel1->setText(tr("GEOM_POINT_I").arg("1"));
  GroupPoints->TextLabel2->setText(tr("GEOM_POINT_I").arg("2"));
  GroupPoints->PushButton1->setPixmap(image1);
  GroupPoints->PushButton2->setPixmap(image1);
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );

  GroupFaces = new DlgRef_2Sel_QTD( aFrame, "GroupFaces");
  GroupFaces->GroupBox1->setTitle(tr("GEOM_FACES"));
  GroupFaces->TextLabel1->setText(tr("GEOM_FACE_I").arg("1"));
  GroupFaces->TextLabel2->setText(tr("GEOM_FACE_I").arg("2"));
  GroupFaces->PushButton1->setPixmap(image1);
  GroupFaces->PushButton2->setPixmap(image1);
  GroupFaces->LineEdit1->setReadOnly( true );
  GroupFaces->LineEdit2->setReadOnly( true );

  Layout1->addWidget(aFrame, 2, 0);
  /***************************************************************/

  setHelpFileName("line.htm");

  Init();
}


//=================================================================================
// function : ~BasicGUI_LineDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BasicGUI_LineDlg::~BasicGUI_LineDlg()
{  
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myPoint1 = myPoint2 = GEOM::GEOM_Object::_nil();

  // myGeomGUI->SetState( 0 );
  globalSelection( GEOM_POINT );
	
  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupFaces->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupFaces->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupFaces->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupFaces->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(),
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  
  ConstructorsClicked (0);
  initName( tr("GEOM_LINE") );
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}

//=======================================================================
// function : ClickOnCancel()
// purpose  :
//=======================================================================
void BasicGUI_LineDlg::ClickOnCancel()
{
  GEOMBase_Skeleton::ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BasicGUI_LineDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;
  
  initName();
  return true;
}

//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void BasicGUI_LineDlg::ConstructorsClicked(int constructorId)
{  
  switch ( constructorId )
  {
  case 0:
    {
      localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
      myEditCurrentArgument = GroupPoints->LineEdit1;
      myEditCurrentArgument->setText("");
      myPoint1 = GEOM::GEOM_Object::_nil();
      myPoint2 = GEOM::GEOM_Object::_nil();
      globalSelection( GEOM_POINT );
      GroupPoints->show();
      GroupFaces->hide();
      break;
    }
  case 1:
    {
      localSelection( GEOM::GEOM_Object::_nil(), TopAbs_FACE );
      myEditCurrentArgument = GroupFaces->LineEdit1;
      myEditCurrentArgument->setText("");
      myFace1 = GEOM::GEOM_Object::_nil();
      myFace2 = GEOM::GEOM_Object::_nil();
      globalSelection( GEOM_FACE );
      GroupPoints->hide();
      GroupFaces->show();
      break;
    }
   }

  updateGeometry();
  resize( minimumSize() );
  SelectionIntoArgument();
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void BasicGUI_LineDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");

  if ( IObjectCount() != 1 ) 
  {
    if ( myEditCurrentArgument == GroupPoints->LineEdit1 )      myPoint1 = GEOM::GEOM_Object::_nil();
    else if ( myEditCurrentArgument == GroupPoints->LineEdit2 ) myPoint2 = GEOM::GEOM_Object::_nil();
    else if ( myEditCurrentArgument == GroupFaces->LineEdit1 ) myFace1 = GEOM::GEOM_Object::_nil();
    else if ( myEditCurrentArgument == GroupFaces->LineEdit2 ) myFace2 = GEOM::GEOM_Object::_nil();
    return;
  }

  // nbSel == 1 
  Standard_Boolean aRes = Standard_False;
  GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject( firstIObject(), aRes );
  if ( !CORBA::is_nil( aSelectedObject ) && aRes )
  {
    myEditCurrentArgument->setText( GEOMBase::GetName( aSelectedObject ) );
    if      ( myEditCurrentArgument == GroupPoints->LineEdit1 ) myPoint1 = aSelectedObject;
    else if ( myEditCurrentArgument == GroupPoints->LineEdit2 ) myPoint2 = aSelectedObject;
    else if ( myEditCurrentArgument == GroupFaces->LineEdit1 )  myFace1 = aSelectedObject;
    else if ( myEditCurrentArgument == GroupFaces->LineEdit2 )  myFace2 = aSelectedObject;
  }
  
  displayPreview();
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  if      ( send == GroupPoints->PushButton1 ) myEditCurrentArgument = GroupPoints->LineEdit1;
  else if ( send == GroupPoints->PushButton2 ) myEditCurrentArgument = GroupPoints->LineEdit2;
  else if ( send == GroupFaces->PushButton1 ) myEditCurrentArgument = GroupFaces->LineEdit1;
  else if ( send == GroupFaces->PushButton2 ) myEditCurrentArgument = GroupFaces->LineEdit2;
  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if      ( send == GroupPoints->LineEdit1 ) myEditCurrentArgument = GroupPoints->LineEdit1;
  else if ( send == GroupPoints->LineEdit2 ) myEditCurrentArgument = GroupPoints->LineEdit2;
  else if ( send == GroupFaces->LineEdit1 ) myEditCurrentArgument = GroupFaces->LineEdit1;
  else if ( send == GroupFaces->LineEdit2 ) myEditCurrentArgument = GroupFaces->LineEdit2;
  else return;
  GEOMBase_Skeleton::LineEditReturnPressed();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(), 
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  // myGeomGUI->SetState( 0 );
  globalSelection( GEOM_POINT );

  myEditCurrentArgument = GroupPoints->LineEdit1;
  myEditCurrentArgument->setFocus();

  GroupPoints->LineEdit1->setText( "" );
  GroupPoints->LineEdit2->setText( "" );
  GroupFaces->LineEdit1->setText( "" );
  GroupFaces->LineEdit2->setText( "" );
  myPoint1 = myPoint2 = myFace1 = myFace2 = GEOM::GEOM_Object::_nil();
}

//=================================================================================
// function : DeactivateActiveDialog()
// purpose  : public slot to deactivate if active
//=================================================================================
void BasicGUI_LineDlg::DeactivateActiveDialog()
{
  // myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::DeactivateActiveDialog();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::enterEvent(QEvent* e)
{
  if ( !GroupConstructors->isEnabled() )
    ActivateThisDialog();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BasicGUI_LineDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetIBasicOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BasicGUI_LineDlg::isValid( QString& msg )
{
  const int id = getConstructorId();
  if ( id == 0 )
    return !myPoint1->_is_nil() && !myPoint2->_is_nil();
  else if ( id == 1 )
    return !myFace1->_is_nil() && !myFace2->_is_nil();
  return false;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BasicGUI_LineDlg::execute( ObjectList& objects )
{
  bool res = false;
  GEOM::GEOM_Object_var anObj;
  switch ( getConstructorId() )
    {
    case 0 :
      {
	anObj = GEOM::GEOM_IBasicOperations::_narrow( getOperation() )->MakeLineTwoPnt( myPoint1, myPoint2 );
	res = true;
	break;
      }
    case 1 :
      {
	anObj = GEOM::GEOM_IBasicOperations::_narrow( getOperation() )->MakeLineTwoFaces( myFace1, myFace2 );
	res = true;
	break;
      }
    }
  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return res;
}

//=================================================================================
// function : closeEvent
// purpose  :
//=================================================================================
void BasicGUI_LineDlg::closeEvent( QCloseEvent* e )
{
  GEOMBase_Skeleton::closeEvent( e );
}

