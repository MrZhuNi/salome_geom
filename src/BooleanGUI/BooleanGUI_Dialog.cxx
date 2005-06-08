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
//  File   : BooleanGUI_Dialog.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "BooleanGUI_Dialog.h"
#include "BooleanGUI.h"
#include "DlgRef_2Sel_QTD.h"

#include "QAD_Desktop.h"

#include <qlabel.h>

//=================================================================================
// class    : BooleanGUI_Dialog()
// purpose  : Constructs a BooleanGUI_Dialog which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BooleanGUI_Dialog::BooleanGUI_Dialog( const int theOperation, QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, fl),
   myOperation( theOperation )
{
  QPixmap image0;
  QString aTitle, aCaption;
  switch ( myOperation )
  {
  	case BooleanGUI::COMMON:
   	  image0 = QPixmap( QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_COMMON")));
			aTitle = tr("GEOM_COMMON");
			aCaption = tr("GEOM_COMMON_TITLE");
			break;
		case BooleanGUI::CUT:
   	  image0 = QPixmap( QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_CUT")));
			aTitle = tr("GEOM_CUT");
			aCaption = tr("GEOM_CUT_TITLE");
			break;
		case BooleanGUI::FUSE:
   	  image0 = QPixmap( QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_FUSE")));
			aTitle = tr("GEOM_FUSE");
			aCaption = tr("GEOM_FUSE_TITLE");
			break;
		case BooleanGUI::SECTION:
   	  image0 = QPixmap( QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_DLG_SECTION")));
			aTitle = tr("GEOM_SECTION");
			aCaption = tr("GEOM_SECTION_TITLE");
			break;
	}
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption( aCaption );

  /***************************************************************/
  GroupConstructors->setTitle( aTitle );
  RadioButton1->setPixmap( image0 );
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  myGroup = new DlgRef_2Sel_QTD(this, "GroupCommon");
  myGroup->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  if ( myOperation != BooleanGUI::CUT )
  {
  	myGroup->TextLabel1->setText(tr("GEOM_OBJECT_I").arg("1"));
  	myGroup->TextLabel2->setText(tr("GEOM_OBJECT_I").arg("2"));
  }
  else
  {
  	myGroup->TextLabel1->setText(tr("GEOM_MAIN_OBJECT"));
  	myGroup->TextLabel2->setText(tr("GEOM_TOOL_OBJECT"));
  }
  
  myGroup->PushButton1->setPixmap(image1);
  myGroup->PushButton2->setPixmap(image1);
  myGroup->LineEdit1->setReadOnly( true );
  myGroup->LineEdit2->setReadOnly( true );

  Layout1->addWidget(myGroup, 2, 0);
  /***************************************************************/

  /* Initialisation */
  Init();
}


//=================================================================================
// function : ~BooleanGUI_Dialog()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BooleanGUI_Dialog::~BooleanGUI_Dialog()
{
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BooleanGUI_Dialog::Init()
{
  /* init variables */
  myEditCurrentArgument = myGroup->LineEdit1;

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(myGroup->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(myGroup->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(myGroup->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(myGroup->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  initName( GroupConstructors->title() );

  globalSelection( GEOM_ALLSHAPES );
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BooleanGUI_Dialog::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BooleanGUI_Dialog::ClickOnApply()
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
void BooleanGUI_Dialog::SelectionIntoArgument()
{
  myEditCurrentArgument->setText( "" );

  if ( mySelection->IObjectCount() != 1 )
  {
    if      ( myEditCurrentArgument == myGroup->LineEdit1 )   myObject1 = GEOM::GEOM_Object::_nil();
    else if ( myEditCurrentArgument == myGroup->LineEdit2 )   myObject2 = GEOM::GEOM_Object::_nil();
    return;
  }

  // nbSel == 1
  Standard_Boolean aRes = Standard_False;
  GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject( mySelection->firstIObject(), aRes );
  if ( !CORBA::is_nil( aSelectedObject ) && aRes && GEOMBase::IsShape( aSelectedObject ) )
  {
    myEditCurrentArgument->setText( GEOMBase::GetName( aSelectedObject ) );
    if      ( myEditCurrentArgument == myGroup->LineEdit1 )   myObject1 = aSelectedObject;
    else if ( myEditCurrentArgument == myGroup->LineEdit2 )   myObject2 = aSelectedObject;
  }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BooleanGUI_Dialog::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if      ( send == myGroup->PushButton1 ) myEditCurrentArgument = myGroup->LineEdit1;
  else if ( send == myGroup->PushButton2 ) myEditCurrentArgument = myGroup->LineEdit2;

  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}



//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BooleanGUI_Dialog::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if ( send == myGroup->LineEdit1 || send == myGroup->LineEdit2 )
  {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BooleanGUI_Dialog::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  globalSelection( GEOM_ALLSHAPES );
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void BooleanGUI_Dialog::enterEvent(QEvent * e)
{
  if ( !GroupConstructors->isEnabled() )
    ActivateThisDialog();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BooleanGUI_Dialog::createOperation()
{
  return getGeomEngine()->GetIBooleanOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BooleanGUI_Dialog::isValid( QString& msg )
{
  return !CORBA::is_nil( myObject1 ) && !CORBA::is_nil( myObject2 );
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BooleanGUI_Dialog::execute( ObjectList& objects )
{
  GEOM::GEOM_Object_var anObj;
 
  anObj = GEOM::GEOM_IBooleanOperations::_narrow( getOperation() )->MakeBoolean( myObject1, myObject2, myOperation );
  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return true;
}
