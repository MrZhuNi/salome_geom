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
//  File   : TransformationGUI_RotationDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "TransformationGUI_RotationDlg.h"

#include "QAD_Config.h"

#include <BRepAdaptor_Curve.hxx>

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
#include <qvalidator.h>
#include <qpixmap.h>


//=================================================================================
// class    : TransformationGUI_RotationDlg()
// purpose  : Constructs a TransformationGUI_RotationDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
TransformationGUI_RotationDlg::TransformationGUI_RotationDlg( QWidget* parent,  const char* name, TransformationGUI* theTransformationGUI, SALOME_Selection* Sel, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_ROTATION")));
    QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_SELECT")));

    if ( !name )
	setName( "TransformationGUI_RotationDlg" );
    resize( 303, 251 ); 
    setCaption( tr( "GEOM_ROTATION_TITLE"  ) );
    setSizeGripEnabled( TRUE );
    TransformationGUI_RotationDlgLayout = new QGridLayout( this ); 
    TransformationGUI_RotationDlgLayout->setSpacing( 6 );
    TransformationGUI_RotationDlgLayout->setMargin( 11 );

    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setTitle( tr( "GEOM_ROTATION"  ) );
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
    TransformationGUI_RotationDlgLayout->addWidget( GroupConstructors, 0, 0 );

    /***************************************************************/
    GroupButtons = new QGroupBox( this, "GroupButtons" );
    GroupButtons->setGeometry( QRect( 10, 10, 281, 48 ) ); 
    GroupButtons->setTitle( tr( ""  ) );
    GroupButtons->setColumnLayout(0, Qt::Vertical );
    GroupButtons->layout()->setSpacing( 0 );
    GroupButtons->layout()->setMargin( 0 );
    GroupButtonsLayout = new QGridLayout( GroupButtons->layout() );
    GroupButtonsLayout->setAlignment( Qt::AlignTop );
    GroupButtonsLayout->setSpacing( 6 );
    GroupButtonsLayout->setMargin( 11 );
    buttonCancel = new QPushButton( GroupButtons, "buttonCancel" );
    buttonCancel->setText( tr( "GEOM_BUT_CLOSE"  ) );
    buttonCancel->setAutoDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonCancel, 0, 3 );
    buttonApply = new QPushButton( GroupButtons, "buttonApply" );
    buttonApply->setText( tr( "GEOM_BUT_APPLY"  ) );
    buttonApply->setAutoDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonApply, 0, 1 );
    QSpacerItem* spacer_9 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupButtonsLayout->addItem( spacer_9, 0, 2 );
    buttonOk = new QPushButton( GroupButtons, "buttonOk" );
    buttonOk->setText( tr( "GEOM_BUT_OK"  ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    GroupButtonsLayout->addWidget( buttonOk, 0, 0 );
    TransformationGUI_RotationDlgLayout->addWidget( GroupButtons, 2, 0 );

    /***************************************************************/
    GroupC1 = new QGroupBox( this, "GroupC1" );
    GroupC1->setTitle( tr( "GEOM_ARGUMENTS"  ) );
    GroupC1->setFrameShape( QGroupBox::Box );
    GroupC1->setFrameShadow( QGroupBox::Sunken );
    GroupC1->setColumnLayout(0, Qt::Vertical );
    GroupC1->layout()->setSpacing( 0 );
    GroupC1->layout()->setMargin( 0 );
    GroupC1Layout = new QGridLayout( GroupC1->layout() );
    GroupC1Layout->setAlignment( Qt::AlignTop );
    GroupC1Layout->setSpacing( 6 );
    GroupC1Layout->setMargin( 11 );
    SelectButtonC1A2 = new QPushButton( GroupC1, "SelectButtonC1A2" );
    SelectButtonC1A2->setText( tr( ""  ) );
    SelectButtonC1A2->setPixmap( image1 );
    SelectButtonC1A2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0,
						  SelectButtonC1A2->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( SelectButtonC1A2, 1, 1 );
    
    LineEditC1A1 = new QLineEdit( GroupC1, "LineEditC1A1" );
    GroupC1Layout->addMultiCellWidget( LineEditC1A1, 0, 0, 2, 3 );
    LineEditC1A2 = new QLineEdit( GroupC1, "LineEditC1A2" );
    GroupC1Layout->addMultiCellWidget( LineEditC1A2, 1, 1, 2, 3 );
    SelectButtonC1A1 = new QPushButton( GroupC1, "SelectButtonC1A1" );
    SelectButtonC1A1->setText( tr( ""  ) );
    SelectButtonC1A1->setPixmap( image1 );
    SelectButtonC1A1->setToggleButton( FALSE );
    SelectButtonC1A1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0,
						  SelectButtonC1A1->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( SelectButtonC1A1, 0, 1 );

    TextLabelC1A1 = new QLabel( GroupC1, "TextLabelC1A1" );
    TextLabelC1A1->setText( tr( "GEOM_OBJECT"  ) );
    TextLabelC1A1->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A1->setFrameShape( QLabel::NoFrame );
    TextLabelC1A1->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A1, 0, 0 );
    TextLabelC1A2 = new QLabel( GroupC1, "TextLabelC1A2" );
    TextLabelC1A2->setText( tr( "GEOM_AXIS"  ) );
    TextLabelC1A2->setMinimumSize( QSize( 50, 0 ) );
    GroupC1Layout->addWidget( TextLabelC1A2, 1, 0 );

    SpinBox_C1A3 = new DlgRef_SpinBox( GroupC1, "GeomSpinBox_C1A3" ) ;
    GroupC1Layout->addWidget( SpinBox_C1A3, 2, 3 );

    TextLabelC1A3 = new QLabel( GroupC1, "TextLabelC1A3" );
    TextLabelC1A3->setText( tr( "GEOM_ANGLE"  ) );
    TextLabelC1A3->setMinimumSize( QSize( 50, 0 ) );
    GroupC1Layout->addWidget( TextLabelC1A3, 2, 2 );
    CheckBoxReverse = new QCheckBox( GroupC1, "CheckBoxReverse" );
    CheckBoxReverse->setText( tr( "GEOM_REVERSE"  ) );
    GroupC1Layout->addMultiCellWidget( CheckBoxReverse, 2, 2, 0, 1 );
    TransformationGUI_RotationDlgLayout->addWidget( GroupC1, 1, 0 );
    myTransformationGUI = theTransformationGUI;
    Init(Sel) ; /* Initialisations */
}

//=================================================================================
// function : ~TransformationGUI_RotationDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
TransformationGUI_RotationDlg::~TransformationGUI_RotationDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::Init( SALOME_Selection* Sel )
{

  /* Get setting of step value from file configuration */
  double step ;
  QString St = QAD_CONFIG->getSetting( "Geometry:SettingsGeomStep" ) ;
  step = St.toDouble() ;

  /* min, max, step and decimals for spin boxes */
  SpinBox_C1A3->RangeStepAndValidator( -999.999, 999.999, step, 3 ) ;
  SpinBox_C1A3->SetValue( 45.0 ) ;  /* = myAngle */
  myAngle = 45.0 ;

  GroupC1->show();
  myConstructorId = 0 ;
  Constructor1->setChecked( TRUE );
  myEditCurrentArgument = LineEditC1A1 ;	
  mySelection = Sel;
  myGeomGUI = GEOMBase_Context::GetGeomGUI() ;
  myOkBase = myOkAxis = false ;
  mySimulationTopoDs.Nullify() ;
  myBase.Nullify() ;
  myGeomGUI->SetActiveDialogBox( (QDialog*)this ) ;

  // TODO : set previous selection into argument ?

  /* Filter definitions */
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);
  myEdgeFilter = new GEOM_EdgeFilter( StdSelect_Line, myGeom );

  /* signals and slots connections */
  connect( buttonOk, SIGNAL( clicked() ),     this, SLOT( ClickOnOk() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  connect( buttonApply, SIGNAL( clicked() ), this, SLOT(ClickOnApply() ) );
  connect( GroupConstructors, SIGNAL(clicked(int) ), SLOT( ConstructorsClicked(int) ) );  
  connect( SelectButtonC1A1, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;
  connect( SelectButtonC1A2, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;

  connect( LineEditC1A1, SIGNAL ( returnPressed() ), this, SLOT( LineEditReturnPressed() ) ) ;
  connect( LineEditC1A2, SIGNAL ( returnPressed() ), this, SLOT( LineEditReturnPressed() ) ) ;

  connect( SpinBox_C1A3, SIGNAL ( valueChanged( double) ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;
  connect( CheckBoxReverse, SIGNAL (stateChanged(int) ), this, SLOT( ReverseAngle(int) ) ) ;
  
  connect( myGeomGUI, SIGNAL ( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) ) ;  
  connect( mySelection, SIGNAL( currentSelectionChanged() ),     this, SLOT( SelectionIntoArgument() ) );
  /* to close dialog if study change */
  connect( myGeomGUI, SIGNAL ( SignalCloseAllDialogs() ), this, SLOT( ClickOnCancel() ) ) ;
 
  /* Move widget on the botton right corner of main widget */
  int x, y ;
  myGeomGUI->DefineDlgPosition( this, x, y ) ;
  this->move( x, y ) ;
  this->show() ; /* displays Dialog */

  return ;
}

//=================================================================================
// function : ReverseAngle()
// purpose  : 'state' not used here
//=================================================================================
void TransformationGUI_RotationDlg::ReverseAngle(int state)
{
  myAngle = -myAngle ;
  SpinBox_C1A3->SetValue( myAngle ) ;
  if( myOkBase && myOkAxis ) {
    MakeRotationSimulationAndDisplay( myBase ) ;
  }
  else {
    myGeomGUI->EraseSimulationShape() ; 
    mySimulationTopoDs.Nullify() ;
  }
  return ;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void TransformationGUI_RotationDlg::ConstructorsClicked(int constructorId)
{
  /* only a constructor now */
  return ;
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::ClickOnOk()
{
  this->ClickOnApply() ;
  this->ClickOnCancel() ;
  return ;
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::ClickOnApply()
{
  myGeomGUI->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  myGeomGUI->GetDesktop()->putInfo( tr("") ) ;

  switch(myConstructorId)
    { 
    case 0 :
      { 
	if(myOkBase && myOkAxis) {	  
	  myTransformationGUI->MakeRotationAndDisplay( myGeomShape, myLoc, myDir, myAngle*PI180) ; 
	}
	break ;
      }
    }
  return ;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::ClickOnCancel()
{
  mySelection->ClearFilters() ;
  myGeomGUI->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  disconnect( mySelection, 0, this, 0 );
  myGeomGUI->ResetState() ;
  reject() ;
  return ;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void TransformationGUI_RotationDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape() ; 
  mySimulationTopoDs.Nullify() ;

  QString aString = ""; /* name of future selection */

  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString) ;
  if ( nbSel != 1 ) {
    if ( myEditCurrentArgument == LineEditC1A1 ) {
      LineEditC1A1->setText("") ;
      myOkBase = false ;
    }
    else if ( myEditCurrentArgument == LineEditC1A2 ) { 
      LineEditC1A2->setText("") ;
      myOkAxis = false ;
    }
    return ;
  }

  /* nbSel == 1 ! */
  TopoDS_Shape S;
  Standard_Boolean testResult ;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject() ;
  
  if( !myGeomGUI->GetTopoFromSelection(mySelection, S) )
    return ;  
 
  if ( myEditCurrentArgument == LineEditC1A1 ) {
    myGeomShape = myGeomGUI->ConvertIOinGEOMShape(IO, testResult) ;
    if( !testResult )
      return ;
    LineEditC1A1->setText(aString) ;
    myBase = S ;
    myOkBase = true ;
  }    
  else if ( myEditCurrentArgument == LineEditC1A2 /*&& myGeomGUI->LinearLocationAndDirection(S, myLoc, myDir) */) {
    BRepAdaptor_Curve curv(TopoDS::Edge(S));
    myDir = curv.Line().Direction();
    myLoc = curv.Line().Location();
    LineEditC1A2->setText(aString) ;
    myOkAxis = true ;
  }

  if( myOkBase && myOkAxis ) {
    MakeRotationSimulationAndDisplay( myBase) ;
  }
  return ;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  switch (myConstructorId)
    {
    case 0: /* default constructor */
      {	
	if(send == SelectButtonC1A1) {
	  LineEditC1A1->setFocus() ;
	  myEditCurrentArgument = LineEditC1A1;
	  mySelection->ClearFilters() ;
	}
	else if(send == SelectButtonC1A2) {
	  LineEditC1A2->setFocus() ;	  
	  myEditCurrentArgument = LineEditC1A2;
	  mySelection->AddFilter(myEdgeFilter) ;
	}	
	SelectionIntoArgument() ;
	break;
      }
    }
  return ;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();  
  if( send == LineEditC1A1 )
    myEditCurrentArgument = LineEditC1A1 ;
  else if ( send == LineEditC1A2 )
    myEditCurrentArgument = LineEditC1A2 ; 
  else
    return ;
  
  /* User name of object input management                          */
  /* If successfull the selection is changed and signal emitted... */
  /* so SelectionIntoArgument() is automatically called.           */
  const QString objectUserName = myEditCurrentArgument->text() ;
  QWidget* thisWidget = (QWidget*)this ;
  if( myGeomGUI->SelectionByNameInDialogs( thisWidget, objectUserName, mySelection ) ) {
    myEditCurrentArgument->setText( objectUserName ) ;
  }
  return ;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::ValueChangedInSpinBox( double newValue )
{  
  myAngle = newValue ;  
  if (myOkBase && myOkAxis) {
    MakeRotationSimulationAndDisplay(myBase) ;
  }
  else {
    myGeomGUI->EraseSimulationShape() ; 
    mySimulationTopoDs.Nullify() ;
  }
  return ;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::DeactivateActiveDialog()
{
  if ( GroupConstructors->isEnabled() ) {
    GroupConstructors->setEnabled(false) ;
    GroupC1->setEnabled(false) ;
    GroupButtons->setEnabled(false) ;
    disconnect( mySelection, 0, this, 0 );
    myGeomGUI->EraseSimulationShape() ;
    mySelection->ClearFilters() ;
  }
  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog() ;   
  GroupConstructors->setEnabled(true) ;
  GroupC1->setEnabled(true) ;
  GroupButtons->setEnabled(true) ;
  connect ( mySelection, SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  if( !mySimulationTopoDs.IsNull() )
    myGeomGUI->DisplaySimulationShape( mySimulationTopoDs ) ;
  return ;
}



//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::enterEvent(QEvent* e)
{
  if ( GroupConstructors->isEnabled() )
    return ;  
  ActivateThisDialog() ;
}



//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::closeEvent( QCloseEvent* e )
{
  this->ClickOnCancel() ; /* same than click on cancel button */
}


//=================================================================================
// function : MakeRotationSimulationAndDisplay()
// purpose  :
//=================================================================================
void TransformationGUI_RotationDlg::MakeRotationSimulationAndDisplay( const TopoDS_Shape& S) 
{
  myGeomGUI->EraseSimulationShape() ;
  
  if( S.IsNull() )
    return ;
  
  try {
    gp_Ax1 AX( this->myLoc, this->myDir ) ;
    gp_Trsf theTransformation ;
    theTransformation.SetRotation(AX, this->myAngle*PI180 ) ;
    BRepBuilderAPI_Transform myBRepTransformation(S, theTransformation, Standard_False) ;
    this->mySimulationTopoDs = myBRepTransformation.Shape() ;
    myGeomGUI->DisplaySimulationShape( mySimulationTopoDs ) ;
  }
  catch(Standard_Failure) {
    MESSAGE( "Exception catched in MakeRotationSimulationAndDisplay" ) ;
    return ;
  }
  return ;
}
