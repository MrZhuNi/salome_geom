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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org
//
//
//
//  File   : BlocksGUI_PropagateDlg.cxx
//  Author : VKN
//  Module : GEOM
//  $Header$

using namespace std;
#include "BlocksGUI_PropagateDlg.h"

#include "QAD_Desktop.h"
#include "QAD_SpinBoxDbl.h"

#include "OCCViewer_Viewer3d.h"
#include "DlgRef_1Sel_Ext.h"
#include "SALOME_ListIteratorOfListIO.hxx"

#include "GEOMImpl_Types.hxx"



//=================================================================================
// class    : BlocksGUI_PropagateDlg()
// purpose  : Constructs a BlocksGUI_PropagateDlg  which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BlocksGUI_PropagateDlg::BlocksGUI_PropagateDlg(QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_PROPAGATE")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_PROPAGATE_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_PROPAGATE_TITLE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_1Sel_Ext(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_PROPAGATE"));
  GroupPoints->TextLabel1->setText(tr("GEOM_SELECTED_SHAPE"));
  GroupPoints->PushButton1->setPixmap(image1);
  GroupPoints->LineEdit1->setReadOnly( true );
  Layout1->removeChild( GroupBoxName );
  Layout1->addWidget(GroupPoints, 1, 0);

  /***************************************************************/

  Init();
}


//=================================================================================
// function : ~BlocksGUI_PropagateDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BlocksGUI_PropagateDlg::~BlocksGUI_PropagateDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myObject = GEOM::GEOM_Object::_nil();

  myGeomGUI->SetState( 0 );
  globalSelection( GEOM_COMPOUND );

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

}


//=================================================================================
// function : ClickOnOk()
// purpose  : Same than click on apply but close this dialog.
//=================================================================================
void BlocksGUI_PropagateDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}



//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BlocksGUI_PropagateDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;

  initName();

  GroupPoints->LineEdit1->setText("");
  myObject = GEOM::GEOM_Object::_nil();

  globalSelection( GEOM_COMPOUND );

  return true;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::ClickOnCancel()
{
  GEOMBase_Skeleton::ClickOnCancel();
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection
//=================================================================================
void BlocksGUI_PropagateDlg::SelectionIntoArgument()
{
  erasePreview();
  myEditCurrentArgument->setText("");
  myObject = GEOM::GEOM_Object::_nil();

  if ( mySelection->IObjectCount() == 1 ) {
    Handle(SALOME_InteractiveObject) anIO = mySelection->firstIObject();
    Standard_Boolean aRes;
    myObject = GEOMBase::ConvertIOinGEOMObject( anIO, aRes );
    if ( aRes )
      myEditCurrentArgument->setText( GEOMBase::GetName( myObject ) );
  }
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::SetEditCurrentArgument()
{
  const QObject* send = sender();
  if ( send == GroupPoints->PushButton1 )  {
    myEditCurrentArgument->setFocus();
    SelectionIntoArgument();
  }
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::LineEditReturnPressed()
{
  const QObject* send = sender();
  if( send == GroupPoints->LineEdit1 ) {
    myEditCurrentArgument = GroupPoints->LineEdit1;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::DeactivateActiveDialog()
{
  myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::DeactivateActiveDialog();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  GroupPoints->LineEdit1->setText("");
  myObject = GEOM::GEOM_Object::_nil();

  myGeomGUI->SetState( 0 );
  globalSelection( GEOM_COMPOUND );
}


//=================================================================================
// function : enterEvent()
// purpose  : Mouse enter onto the dialog to activate it
//=================================================================================
void BlocksGUI_PropagateDlg::enterEvent(QEvent* e)
{
  if ( !GroupConstructors->isEnabled() )
    ActivateThisDialog();
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void BlocksGUI_PropagateDlg::closeEvent(QCloseEvent* e)
{
  myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::closeEvent( e );
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BlocksGUI_PropagateDlg::createOperation()
{
  return getGeomEngine()->GetIBlocksOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BlocksGUI_PropagateDlg::isValid( QString& msg )
{
  return !myObject->_is_nil() ;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BlocksGUI_PropagateDlg::execute( ObjectList& objects )
{
  bool aResult = false;
  GEOM::ListOfGO_var aCurrList = GEOM::GEOM_IBlocksOperations::_narrow( getOperation() )->Propagate( myObject );
  for ( int i = 0, n = aCurrList->length(); i < n; i++ )
  {
    //NOT yet implemented
  }

  return aResult;
}
