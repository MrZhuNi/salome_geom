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
//  File   : RepairGUI_SuppressFacesDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "RepairGUI_SuppressFacesDlg.h"

#include "DisplayGUI.h"


#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

//=================================================================================
// class    : RepairGUI_SuppressFacesDlg()
// purpose  : Constructs a RepairGUI_SuppressFacesDlg  which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
RepairGUI_SuppressFacesDlg::RepairGUI_SuppressFacesDlg( QWidget* parent, 
							    const char* name,
							    RepairGUI* theRepairGUI,
							    SALOME_Selection* Sel,
							    Handle (AIS_InteractiveContext) ic,
							    bool modal,
							    WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_SUPRESS_FACE")));
    QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_SELECT")));
    
    if ( !name )
	setName( "RepairGUI_SuppressFacesDlg" );
    resize( 322, 203 ); 
    setCaption( tr( "GEOM_SUPRESSFACE_TITLE"  ) );
    setSizeGripEnabled( TRUE );
    RepairGUI_SuppressFacesDlgLayout = new QGridLayout( this ); 
    RepairGUI_SuppressFacesDlgLayout->setSpacing( 6 );
    RepairGUI_SuppressFacesDlgLayout->setMargin( 11 );

    /***************************************************************/
    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setTitle( tr( "GEOM_SUPRESSFACE"  ) );
    GroupConstructors->setExclusive( TRUE );
    GroupConstructors->setColumnLayout(0, Qt::Vertical );
    GroupConstructors->layout()->setSpacing( 0 );
    GroupConstructors->layout()->setMargin( 0 );
    GroupConstructorsLayout = new QGridLayout( GroupConstructors->layout() );
    GroupConstructorsLayout->setAlignment( Qt::AlignTop );
    GroupConstructorsLayout->setSpacing( 6 );
    GroupConstructorsLayout->setMargin( 11 );
    Constructor1 = new QRadioButton( GroupConstructors, "Constructor1" );
    Constructor1->setText( tr( ""  ) );
    Constructor1->setPixmap( image0 );
    Constructor1->setChecked( TRUE );
    Constructor1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, Constructor1->sizePolicy().hasHeightForWidth() ) );
    Constructor1->setMinimumSize( QSize( 50, 0 ) );
    GroupConstructorsLayout->addWidget( Constructor1, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupConstructorsLayout->addItem( spacer, 0, 1 );
    RepairGUI_SuppressFacesDlgLayout->addWidget( GroupConstructors, 0, 0 );

    /***************************************************************/
    GroupC1 = new QGroupBox( this, "GroupC1" );
    GroupC1->setTitle( tr( "GEOM_ARGUMENTS"  ) );
    GroupC1->setMinimumSize( QSize( 0, 0 ) );
    GroupC1->setFrameShape( QGroupBox::Box );
    GroupC1->setFrameShadow( QGroupBox::Sunken );
    GroupC1->setColumnLayout(0, Qt::Vertical );
    GroupC1->layout()->setSpacing( 0 );
    GroupC1->layout()->setMargin( 0 );
    GroupC1Layout = new QGridLayout( GroupC1->layout() );
    GroupC1Layout->setAlignment( Qt::AlignTop );
    GroupC1Layout->setSpacing( 6 );
    GroupC1Layout->setMargin( 11 );
    Layout2 = new QHBoxLayout; 
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );
    TextLabelC1A1 = new QLabel( GroupC1, "TextLabelC1A1" );
    TextLabelC1A1->setText( tr( "GEOM_MAIN_OBJECT"  ) );
    TextLabelC1A1->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A1->setFrameShape( QLabel::NoFrame );
    TextLabelC1A1->setFrameShadow( QLabel::Plain );
    Layout2->addWidget( TextLabelC1A1 );
    SelectButtonC1A1 = new QPushButton( GroupC1, "SelectButtonC1A1" );
    SelectButtonC1A1->setText( tr( ""  ) );
    SelectButtonC1A1->setPixmap( image1 );
    SelectButtonC1A1->setToggleButton( FALSE );
    SelectButtonC1A1->setMaximumSize( QSize( 28, 32767 ) );
    Layout2->addWidget( SelectButtonC1A1 );
    LineEditC1A1 = new QLineEdit( GroupC1, "LineEditC1A1" );
    LineEditC1A1->setAlignment( int( QLineEdit::AlignLeft ) );
    Layout2->addWidget( LineEditC1A1 );
    GroupC1Layout->addLayout( Layout2, 0, 0 );
    CheckBox1 = new QCheckBox( GroupC1, "CheckBox1" );
    CheckBox1->setText( tr( "GEOM_SUPRESSFACE_SELECT"  ) );
    CheckBox1->setChecked( FALSE );
    GroupC1Layout->addWidget( CheckBox1, 1, 0 );
    RepairGUI_SuppressFacesDlgLayout->addWidget( GroupC1, 1, 0 );

   /***************************************************************/
    GroupButtons = new QGroupBox( this, "GroupButtons" );
    GroupButtons->setTitle( tr( ""  ) );
    GroupButtons->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, GroupButtons->sizePolicy().hasHeightForWidth() ) );
    GroupButtons->setColumnLayout(0, Qt::Vertical );
    GroupButtons->layout()->setSpacing( 0 );
    GroupButtons->layout()->setMargin( 0 );
    GroupButtonsLayout = new QGridLayout( GroupButtons->layout() );
    GroupButtonsLayout->setAlignment( Qt::AlignTop );
    GroupButtonsLayout->setSpacing( 6 );
    GroupButtonsLayout->setMargin( 11 );
    //
    buttonOk = new QPushButton( GroupButtons, "buttonOk" );
    buttonOk->setText( tr( "GEOM_BUT_OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonOk, 0, 0 );    
    buttonApply = new QPushButton( GroupButtons, "buttonApply" );
    buttonApply->setText( tr( "GEOM_BUT_APPLY"  ) );
    buttonApply->setAutoDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonApply, 0, 1 );    
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupButtonsLayout->addItem( spacer_2, 0, 2 );
    buttonClose = new QPushButton( GroupButtons, "buttonClose" );
    buttonClose->setText( tr( "GEOM_BUT_CLOSE"  ) );
    buttonClose->setAutoDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonClose, 0, 3 );
    RepairGUI_SuppressFacesDlgLayout->addWidget( GroupButtons, 2, 0 );
    myRepairGUI = theRepairGUI;
    /* Initialisations */
    Init(Sel, ic) ;

}


//=================================================================================
// function : ~RepairGUI_SuppressFacesDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
RepairGUI_SuppressFacesDlg::~RepairGUI_SuppressFacesDlg()
{
    // no need to delete child widgets, Qt does it all for us
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::Init( SALOME_Selection* Sel, Handle (AIS_InteractiveContext) ic )
{

  GroupC1->show();
  myConstructorId = 0 ;
  Constructor1->setChecked( TRUE );
  myEditCurrentArgument = LineEditC1A1 ;
  mySelection = Sel;
  myShape.Nullify() ;

  myIC = ic ;
  myUseLocalContext = false ;
  myOkShape = false ;
  myGeomBase = new GEOMBase() ;
  myGeomGUI = GEOMContext::GetGeomGUI() ;

  /* Select sub shapes mode not checked */
  CheckBox1->setChecked( FALSE );
  myOkSelectSubMode = false ;

  // TODO : previous selection into argument ?

  /* Filter definitions */
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);

  /* signals and slots connections */
  connect( buttonOk,          SIGNAL( clicked() ),    this, SLOT( ClickOnOk() ) );
  connect( buttonApply,       SIGNAL( clicked() ),    this, SLOT( ClickOnApply() ) );
  connect( buttonClose,       SIGNAL( clicked() ),    this, SLOT( ClickOnClose() ) );
  connect( GroupConstructors, SIGNAL( clicked(int) ), this, SLOT( ConstructorsClicked(int) ));

  connect( SelectButtonC1A1,  SIGNAL( clicked() ),                      this, SLOT( SetEditCurrentArgument() ));
  connect( CheckBox1,         SIGNAL( stateChanged(int) ),              this, SLOT( ActivateUserSelection() ));
  connect( mySelection,       SIGNAL( currentSelectionChanged() ),      this, SLOT( SelectionIntoArgument() ));
  connect( myGeomGUI,         SIGNAL( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) ) ;  
  connect( myGeomGUI,         SIGNAL( SignalCloseAllDialogs() ),        this, SLOT( ClickOnClose() ));
 
  /* Move widget on the botton right corner of main widget */
  int x, y ;
  myGeomBase->DefineDlgPosition( this, x, y ) ;
  this->move( x, y ) ;
  this->show() ; /* display Dialog */
  return ;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void RepairGUI_SuppressFacesDlg::ConstructorsClicked(int constructorId)
{
  return ;
}


//=================================================================================
// function : ClickOnOk()
// purpose  : Same than click on apply but close this dialog.
//=================================================================================
void RepairGUI_SuppressFacesDlg::ClickOnOk()
{
  this->ClickOnApply() ;
  accept();

  return ;
}



//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::ClickOnApply()
{
  bool testResult = false ;
  switch(myConstructorId)
    { 
    case 0 :
      {
	if( myOkShape && myOkSelectSubMode ) {
	  testResult = myRepairGUI->OnSuppressFaces( myShape, myShapeIOR, myLocalContextId, myUseLocalContext ) ; 
	}
	if( !testResult ) {
	  myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_ABORT")) ;
	}
	else {
	  myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_DONE")) ;
	}
	/* Reset arguments to allow a new selection */
	this->ResetStateOfDialog() ;
	break ;
      }
    }
  return ;
}



//=================================================================================
// function : ClickOnClose()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::ClickOnClose()
{
  disconnect( mySelection, 0, this, 0 );
  myGeomGUI->ResetState() ;
  
  if(myUseLocalContext) {
    myIC->CloseLocalContext(myLocalContextId) ;
    this->myUseLocalContext = false ;
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true) ;
  }
  reject() ;
  return ;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//          : used only by SelectButtonC1A1 (LineEditC1A1)
//=================================================================================
void RepairGUI_SuppressFacesDlg::SelectionIntoArgument()
{
  
  /* Reset argument and local context when selection as changed */
  this->ResetStateOfDialog() ;
  
  QString aString = ""; /* name of selection */
  
  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString) ;
  if ( nbSel != 1 )
    return ;
  
  /* nbSel == 1 */
  TopoDS_Shape S ;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject() ;

  if( !myGeomBase->GetTopoFromSelection(mySelection, S) )
    return ;
  
  if( !IO->hasEntry() ) {
    myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SHAPE_IN_STUDY")) ;
    return ;
  }
  
  /* Test the exact type of topology to suppress faces into */
  if ( !S.IsNull() && ( S.ShapeType() == TopAbs_SOLID || S.ShapeType() == TopAbs_SHELL || S.ShapeType() == TopAbs_COMPOUND ) ) {
    
    if ( IO->IsInstance(STANDARD_TYPE(GEOM_InteractiveObject)) ) {
      Handle(GEOM_InteractiveObject) GIObject = Handle(GEOM_InteractiveObject)::DownCast( IO );
      myShapeIOR = GIObject->getIOR(); /* the Geom IOR string of selection */
      LineEditC1A1->setText(aString) ;
      myShape = S ;
      myOkShape = true ;
      return;
    } 
    
    if ( IO->hasEntry() ) {
      SALOMEDS::Study_var aStudy = myGeomGUI->GetActiveStudy()->getStudyDocument();
      SALOMEDS::SObject_var obj = aStudy->FindObjectID( IO->getEntry() );
      SALOMEDS::GenericAttribute_var anAttr;
      SALOMEDS::AttributeIOR_var     anIOR;
      if ( !obj->_is_nil() ) {
	if (obj->FindAttribute(anAttr, "AttributeIOR")) {
          anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
	  myShapeIOR = anIOR->Value();
	  myOkShape = true ;
	  myShape = S ;
	  LineEditC1A1->setText(aString) ;
	  return;
	}
      }
    }

  }
  return ;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  switch (myConstructorId)
    {
    case 0: /* default constructor */
      {
	if(send == SelectButtonC1A1) {
	  LineEditC1A1->setFocus() ;
	  myEditCurrentArgument = LineEditC1A1;	  
	  SelectionIntoArgument() ;
	}
	break;
      }
    }
  return ;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::LineEditReturnPressed()
{
  return ;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::DeactivateActiveDialog()
{
  if ( GroupConstructors->isEnabled() ) {
 
   this->ResetStateOfDialog() ;

    disconnect( mySelection, 0, this, 0 );
    GroupConstructors->setEnabled(false) ;
    GroupC1->setEnabled(false) ;
    GroupButtons->setEnabled(false) ;
    myGeomGUI->ResetState() ;    
    myGeomGUI->SetActiveDialogBox(0) ;
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true) ;
  }
  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate other active dialog */
  myGeomGUI->EmitSignalDeactivateDialog() ;
  GroupConstructors->setEnabled(true) ;
  GroupC1->setEnabled(true) ;
  GroupButtons->setEnabled(true) ;
  connect ( mySelection, SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  myGeomGUI->SetActiveDialogBox( (QDialog*)this ) ;
  return ;
}


//=================================================================================
// function : enterEvent()
// purpose  : Mouse enter onto the dialog to activate it
//=================================================================================
void RepairGUI_SuppressFacesDlg::enterEvent(QEvent* e)
{
  if ( GroupConstructors->isEnabled() )
    return ;  
  ActivateThisDialog() ;
  return ;
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void RepairGUI_SuppressFacesDlg::closeEvent( QCloseEvent* e )
{
  /* same than click on cancel button */
  this->ClickOnClose() ;
  return ;
}


//=================================================================================
// function : ActivateUserSelection()
// purpose  : Activate selection of faces when CheckBox1->isChecked()...
//=================================================================================
void RepairGUI_SuppressFacesDlg::ActivateUserSelection()
{
  if ( myGeomGUI->GetActiveStudy()->getActiveStudyFrame()->getTypeView() == VIEW_OCC ) {
    if( !this->myOkShape ) {
      this->ResetStateOfDialog() ;
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SELECT_FIRST")) ;
      return ;
    }
  
    this->myOkSelectSubMode = CheckBox1->isChecked() ;
    
    if( this->myUseLocalContext ) {
      myIC->CloseLocalContext(myLocalContextId) ;
      this->myUseLocalContext = false ;
      DisplayGUI* myDisplayGUI = new DisplayGUI();
      myDisplayGUI->OnDisplayAll(true) ;
    }
    
    if( myOkShape && myOkSelectSubMode ) {
      /* local context is defined into the method : 4 = FACES sub selection */
      DisplayGUI* myDisplayGUI = new DisplayGUI();
      myDisplayGUI->PrepareSubShapeSelection( int(TopAbs_FACE), this->myLocalContextId ) ;    
      myUseLocalContext = true ;
      myGeomGUI->GetDesktop()->putInfo(tr("GEOM_PRP_SELECT_FACE")) ;
    }
  } else {
    CheckBox1->setChecked( false );
  }

  return ;
}



//=================================================================================
// function : ResetStateOfDialog()
// purpose  : Completely reset the state of method including local context
//=================================================================================
void RepairGUI_SuppressFacesDlg::ResetStateOfDialog()
{
  this->myOkShape = false ;
  this->myEditCurrentArgument->setText("") ;

  /* Select sub shapes mode not checked */
  this->myOkSelectSubMode = false ;    
  this->CheckBox1->setChecked( FALSE );

  /* Close its local contact if opened */
  if( this->myUseLocalContext ) {
    myIC->CloseLocalContext(this->myLocalContextId) ;
    this->myUseLocalContext = false ;
    DisplayGUI* myDisplayGUI = new DisplayGUI();
    myDisplayGUI->OnDisplayAll(true) ;
  }
  return ;
}
