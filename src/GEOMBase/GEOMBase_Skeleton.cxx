//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  GEOM GEOMGUI : GUI for Geometry component
//  File   : GEOMBase_Skeleton.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$
//
#include "GEOMBase_Skeleton.h"

#include "GeometryGUI.h"

#include "SalomeApp_Application.h"
#include "LightApp_Application.h"
#include "LightApp_SelectionMgr.h"
#include "SUIT_Session.h"
#include "SUIT_MessageBox.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>

using namespace std;

//=================================================================================
// class    : GEOMBase_Skeleton()
// purpose  : Constructs a GEOMBase_Skeleton which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GEOMBase_Skeleton::GEOMBase_Skeleton (GeometryGUI* theGeometryGUI, QWidget* parent,
                                      const char* name, bool modal, WFlags fl)
  : DlgRef_Skeleton_QTD( parent, name, modal, WStyle_Customize | WStyle_NormalBorder
                         | WStyle_Title | WStyle_SysMenu | WDestructiveClose ),
   GEOMBase_Helper( dynamic_cast<SUIT_Desktop*>( parent ) ),
   myGeomGUI( theGeometryGUI )
{
  if (!name)
    setName("GEOMBase_Skeleton");

  GroupBoxName->setTitle(tr("GEOM_RESULT_NAME_GRP"));
  NameLabel->setText(tr("GEOM_RESULT_NAME_LBL"));

  GroupBoxPublish->setTitle(tr("GEOM_PUBLISH_RESULT_GRP"));
  CheckBoxRestoreSS->setText(tr("GEOM_RESTORE_SUB_SHAPES"));

  buttonCancel->setText(tr("GEOM_BUT_CLOSE"));
  buttonOk->setText(tr("GEOM_BUT_APPLY_AND_CLOSE"));
  buttonApply->setText(tr("GEOM_BUT_APPLY"));
  buttonHelp->setText(tr("GEOM_BUT_HELP"));

  resize(0, 0);

  Init();
}

//=================================================================================
// function : ~GEOMBase_Skeleton()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GEOMBase_Skeleton::~GEOMBase_Skeleton()
{
  if (myGeomGUI)
    myGeomGUI->SetActiveDialogBox( 0 );
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::Init()
{
  SalomeApp_Application* app = (SalomeApp_Application*)(SUIT_Session::session()->activeApplication());
  if (!myGeomGUI && app)
    myGeomGUI = dynamic_cast<GeometryGUI*>( app->module( "Geometry" ) );

  /* init variables */
  if (myGeomGUI)
    myGeomGUI->SetActiveDialogBox(this);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  if (myGeomGUI)
  {
    connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
    connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));
  }

  // connect help button on a private slot that displays help information
  connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( ClickOnHelp() ) );

  /* displays Dialog */
  RadioButton1->setChecked(TRUE);
  RadioButton4->hide();
  RadioButton5->hide();

  CheckBoxRestoreSS->setChecked(FALSE);
  GroupBoxPublish->hide();
}

//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::ClickOnCancel()
{
  close();
}

//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::LineEditReturnPressed()
{
  if ( !myEditCurrentArgument )
    return;

  /* User name of object input management                          */
  /* If successfull the selection is changed and signal emitted... */
  /* so SelectionIntoArgument() is automatically called.           */
  const QString objectUserName = myEditCurrentArgument->text();
  QWidget* thisWidget = (QWidget*)this;

  if(GEOMBase::SelectionByNameInDialogs(thisWidget, objectUserName, selectedIO()))
     myEditCurrentArgument->setText(objectUserName);
}

//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::DeactivateActiveDialog()
{
  this->setEnabled(false);
  globalSelection();
  if (myGeomGUI) {
    myGeomGUI->SetActiveDialogBox(0);
    disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
  }
}

//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  if (myGeomGUI) myGeomGUI->EmitSignalDeactivateDialog();
  this->setEnabled(true);
  if (myGeomGUI) myGeomGUI->SetActiveDialogBox((QDialog*)this);
}

//=================================================================================
// function : closeEvent()
// purpose  : same than click on cancel button
//=================================================================================
void GEOMBase_Skeleton::closeEvent(QCloseEvent* e)
{
  if (myGeomGUI) {
    disconnect( myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
    myGeomGUI->getApp()->updateActions();
  }
  QDialog::closeEvent( e );
}

//=================================================================================
// function : initName()
// purpose  : initialize the Name field with a string "thePrefix_X" (Vertex_3)
//=================================================================================
void GEOMBase_Skeleton::initName( const char* thePrefix )
{
  if ( thePrefix )
    setPrefix( thePrefix );
  ResultName->setText( GEOMBase::GetDefaultName( getPrefix() ) );
}

//=================================================================================
// function : getNewObjectName()
// purpose  : returns contents of Name field
//=================================================================================
const char* GEOMBase_Skeleton::getNewObjectName() const
{
  return ResultName->text();
}

//=================================================================================
// function : getConstructorId()
// purpose  :
//=================================================================================
int GEOMBase_Skeleton::getConstructorId() const
{
  if ( GroupConstructors != NULL && GroupConstructors->selected() != NULL )
    return GroupConstructors->id( GroupConstructors->selected() );
  return -1;
}

//=================================================================================
// function : ClickOnHelp()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::ClickOnHelp()
{
  LightApp_Application* app = (LightApp_Application*)(SUIT_Session::session()->activeApplication());
  if (app)
    app->onHelpContextModule(myGeomGUI ? app->moduleName(myGeomGUI->moduleName()) : QString(""), myHelpFileName);
  else {
    QString platform;
#ifdef WIN32
    platform = "winapplication";
#else
    platform = "application";
#endif
    SUIT_MessageBox::warn1(0, QObject::tr("WRN_WARNING"),
			   QObject::tr("EXTERNAL_BROWSER_CANNOT_SHOW_PAGE").
			   arg(app->resourceMgr()->stringValue("ExternalBrowser", platform)).arg(myHelpFileName),
			   QObject::tr("BUT_OK"));
  }
}

//=================================================================================
//  function : setHelpFileName()
//  purpose  : set name for help file html
//=================================================================================
void GEOMBase_Skeleton::setHelpFileName(const QString& theName)
{
    myHelpFileName = theName;
}

//=================================================================================
// function : keyPressEvent()
// purpose  :
//=================================================================================
void GEOMBase_Skeleton::keyPressEvent( QKeyEvent* e )
{
  QDialog::keyPressEvent( e );
  if ( e->isAccepted() )
    return;

  if ( e->key() == Key_F1 )
  {
    e->accept();
    ClickOnHelp();
  }
}
