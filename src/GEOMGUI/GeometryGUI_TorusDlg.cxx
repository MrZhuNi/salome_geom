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
//  File   : GeometryGUI_TorusDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "GeometryGUI_TorusDlg.h"

#include "GeometryGUI.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"
#include "utilities.h"

#include <BRepAdaptor_Curve.hxx>

#include <qbuttongroup.h>
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
// class    : GeometryGUI_TorusDlg()
// purpose  : Constructs a GeometryGUI_TorusDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GeometryGUI_TorusDlg::GeometryGUI_TorusDlg( QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{

    QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_TORUS_PV")));
    QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_SELECT")));
    QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_TORUS_DXYZ")));
    
    if ( !name )
	setName( "GeometryGUI_TorusDlg" );
    resize( 303, 219 ); 
    setCaption( tr( "GEOM_TORUS_TITLE"  ) );
    setSizeGripEnabled( TRUE );
    GeometryGUI_TorusDlgLayout = new QGridLayout( this ); 
    GeometryGUI_TorusDlgLayout->setSpacing( 6 );
    GeometryGUI_TorusDlgLayout->setMargin( 11 );

    /***************************************************************/
    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setTitle( tr( "GEOM_TORUS"  ) );
    GroupConstructors->setExclusive( TRUE );
    GroupConstructors->setColumnLayout(0, Qt::Vertical );
    GroupConstructors->layout()->setSpacing( 0 );
    GroupConstructors->layout()->setMargin( 0 );
    GroupConstructorsLayout = new QGridLayout( GroupConstructors->layout() );
    GroupConstructorsLayout->setAlignment( Qt::AlignTop );
    GroupConstructorsLayout->setSpacing( 6 );
    GroupConstructorsLayout->setMargin( 11 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupConstructorsLayout->addItem( spacer, 0, 1 );
    Constructor1 = new QRadioButton( GroupConstructors, "Constructor1" );
    Constructor1->setText( tr( ""  ) );
    Constructor1->setPixmap( image0 );
    Constructor1->setChecked( TRUE );
    Constructor1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0,Constructor1->sizePolicy().hasHeightForWidth() ) );
    Constructor1->setMinimumSize( QSize( 50, 0 ) );
    GroupConstructorsLayout->addWidget( Constructor1, 0, 0 );
    Constructor2 = new QRadioButton( GroupConstructors, "Constructor2" );
    Constructor2->setText( tr( ""  ) );
    Constructor2->setMinimumSize( QSize( 50, 0 ) );
    Constructor2->setPixmap( image2 );
    Constructor2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, Constructor1->sizePolicy().hasHeightForWidth() ) );
    GroupConstructorsLayout->addWidget( Constructor2, 0, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupConstructorsLayout->addItem( spacer_2, 0, 2 );
    GeometryGUI_TorusDlgLayout->addWidget( GroupConstructors, 0, 0 );

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
    GeometryGUI_TorusDlgLayout->addWidget( GroupButtons, 2, 0 );

    /***************************************************************/
    GroupC1 = new QGroupBox( this, "GroupC1" );
    GroupC1->setTitle( tr( "GEOM_ARGUMENTS"  ) );
    GroupC1->setColumnLayout(0, Qt::Vertical );
    GroupC1->layout()->setSpacing( 0 );
    GroupC1->layout()->setMargin( 0 );
    GroupC1Layout = new QGridLayout( GroupC1->layout() );
    GroupC1Layout->setAlignment( Qt::AlignTop );
    GroupC1Layout->setSpacing( 6 );
    GroupC1Layout->setMargin( 11 );
    SelectButtonC1A1 = new QPushButton( GroupC1, "SelectButtonC1A1" );
    SelectButtonC1A1->setText( tr( ""  ) );
    SelectButtonC1A1->setPixmap( image1 );
    GroupC1Layout->addWidget( SelectButtonC1A1, 0, 1 );

    LineEditC1A1 = new QLineEdit( GroupC1, "LineEditC1A1" );
    LineEditC1A1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEditC1A1->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( LineEditC1A1, 0, 2 );
    TextLabelC1A1 = new QLabel( GroupC1, "TextLabelC1A1" );
    TextLabelC1A1->setText( tr( "GEOM_BASE_POINT"  ) );
    TextLabelC1A1->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A1->setFrameShape( QLabel::NoFrame );
    TextLabelC1A1->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A1, 0, 0 );
    SelectButtonC1A2 = new QPushButton( GroupC1, "SelectButtonC1A2" );
    SelectButtonC1A2->setText( tr( ""  ) );
    SelectButtonC1A2->setPixmap( image1 );
    GroupC1Layout->addWidget( SelectButtonC1A2, 1, 1 );
    LineEditC1A2 = new QLineEdit( GroupC1, "LineEditC1A2" );
    LineEditC1A2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEditC1A2->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( LineEditC1A2, 1, 2 );
    TextLabelC1A2 = new QLabel( GroupC1, "TextLabelC1A2" );
    TextLabelC1A2->setText( tr( "GEOM_VECTOR"  ) );
    TextLabelC1A2->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A2->setFrameShape( QLabel::NoFrame );
    TextLabelC1A2->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A2, 1, 0 );
    TextLabelC1A3 = new QLabel( GroupC1, "TextLabelC1A3" );
    TextLabelC1A3->setText( tr( "GEOM_RADIUS_I"  ).arg("1") );
    TextLabelC1A3->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A3->setFrameShape( QLabel::NoFrame );
    TextLabelC1A3->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A3, 2, 0 );

    SpinBox_C1A3 = new GeometryGUI_SpinBox( GroupC1, "GeomSpinBox_C1A3" ) ;
    SpinBox_C1A3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, SpinBox_C1A3->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( SpinBox_C1A3, 2, 2 );

    TextLabelC1A4 = new QLabel( GroupC1, "TextLabelC1A4" );
    TextLabelC1A4->setText( tr( "GEOM_RADIUS_I"  ).arg("2") );
    TextLabelC1A4->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A4->setFrameShape( QLabel::NoFrame );
    TextLabelC1A4->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A4, 3, 0 );

    SpinBox_C1A4 = new GeometryGUI_SpinBox( GroupC1, "GeomSpinBox_C1A4" ) ;
    SpinBox_C1A4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, SpinBox_C1A4->sizePolicy().hasHeightForWidth() ) );
    GroupC1Layout->addWidget( SpinBox_C1A4, 3, 2 );

    GeometryGUI_TorusDlgLayout->addWidget( GroupC1, 1, 0 ); 
    /***************************************************************/

    GroupC2 = new QGroupBox( this, "GroupC2" );
    GroupC2->setTitle( tr( "GEOM_BOX_OBJ"  ) );
    GroupC2->setColumnLayout(0, Qt::Vertical );
    GroupC2->layout()->setSpacing( 0 );
    GroupC2->layout()->setMargin( 0 );
    GroupC2Layout = new QGridLayout( GroupC2->layout() );
    GroupC2Layout->setAlignment( Qt::AlignTop );
    GroupC2Layout->setSpacing( 6 );
    GroupC2Layout->setMargin( 11 );
    TextLabel_Radius1 = new QLabel( GroupC2, "TextLabel_Radius1" );
    TextLabel_Radius1->setText( tr( "GEOM_RADIUS_I"  ).arg("1") );
    GroupC2Layout->addWidget( TextLabel_Radius1, 0, 0 );
    TextLabel_Radius2 = new QLabel( GroupC2, "TextLabel_Radius2" );
    TextLabel_Radius2->setText( tr( "GEOM_RADIUS_I"  ).arg("2") );
    GroupC2Layout->addWidget( TextLabel_Radius2, 1, 0 );
    
    SpinBox_Radius1 = new GeometryGUI_SpinBox( GroupC2, "GeomSpinBox_Radius1" ) ;
    GroupC2Layout->addWidget( SpinBox_Radius1, 0, 1 );

    SpinBox_Radius2 = new GeometryGUI_SpinBox( GroupC2, "GeomSpinBox_Radius2" ) ;
    GroupC2Layout->addWidget( SpinBox_Radius2, 1, 1 );   

    QSpacerItem* spacer1 = new QSpacerItem( 20, 62, QSizePolicy::Minimum, QSizePolicy::Fixed );
    GroupC2Layout->addItem( spacer1 );
    GeometryGUI_TorusDlgLayout->addWidget(GroupC2 , 1, 0 );

    /***************************************************************/
    
    /* Initialisations */
    Init(Sel) ;
}


//=================================================================================
// function : ~GeometryGUI_TorusDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GeometryGUI_TorusDlg::~GeometryGUI_TorusDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::Init( SALOME_Selection* Sel )
{

  /* Get setting of step value from file configuration */
  double step ;
  QString St = QAD_CONFIG->getSetting( "Geometry:SettingsGeomStep" ) ;
  step = St.toDouble() ;

  /* min, max, step and decimals for spin boxes */
  SpinBox_C1A3->RangeStepAndValidator( 0.001, 999.999, step, 3 ) ;
  SpinBox_C1A4->RangeStepAndValidator( 0.001, 999.999, step, 3 ) ;
  SpinBox_Radius1->RangeStepAndValidator( 0.001, 999.999, step, 3 ) ;
  SpinBox_Radius2->RangeStepAndValidator( 0.001, 999.999, step, 3 ) ;

  GroupC1->show();
  GroupC2->hide();
  myConstructorId = 0 ;
  Constructor1->setChecked( TRUE );
  myEditCurrentArgument = LineEditC1A1 ;	
  mySelection = Sel;
  myGeomGUI = GeometryGUI::GetGeometryGUI() ;

  SpinBox_C1A3->SetValue( 300.0 ) ;
  SpinBox_C1A4->SetValue( 100.0 ) ;
  SpinBox_Radius1->SetValue( 300.0 ) ;
  SpinBox_Radius2->SetValue( 100.0 ) ;
  myRadius1 = 300.0 ;
  myRadius2 = 100.0 ;
  
  myOkRadius1 = true ;
  myOkRadius2 = true ;
  myOkPoint1 = false ;
  myOkDir = false ;

  mySimulationTopoDs.Nullify() ;
  myGeomGUI->SetActiveDialogBox( (QDialog*)this ) ;

  // TODO : previous selection into argument ?

  /* Filter definitions */
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);  
  myEdgeFilter   = new GEOM_EdgeFilter( StdSelect_Line, myGeom );
  myVertexFilter = new GEOM_ShapeTypeFilter( TopAbs_VERTEX, myGeom );
  /* first filter used */
  mySelection->AddFilter(myVertexFilter) ;

  /* signals and slots connections */
  connect( buttonOk, SIGNAL( clicked() ),     this, SLOT( ClickOnOk() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  connect( buttonApply, SIGNAL( clicked() ), this, SLOT(ClickOnApply() ) );
  connect( GroupConstructors, SIGNAL(clicked(int) ), SLOT( ConstructorsClicked(int) ) );  
  connect( SelectButtonC1A1, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;
  connect( SelectButtonC1A2, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;

  connect( SpinBox_C1A3, SIGNAL ( valueChanged( double)  ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;
  connect( SpinBox_C1A4, SIGNAL ( valueChanged( double)  ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;
  connect( SpinBox_Radius1, SIGNAL ( valueChanged( double)  ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;
  connect( SpinBox_Radius2, SIGNAL ( valueChanged( double)  ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;

  connect( LineEditC1A1, SIGNAL ( returnPressed() ), this, SLOT( LineEditReturnPressed() ) ) ;
  connect( LineEditC1A2, SIGNAL ( returnPressed() ), this, SLOT( LineEditReturnPressed() ) ) ;

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
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void GeometryGUI_TorusDlg::ConstructorsClicked(int constructorId)
{
  mySelection->ClearFilters() ;
  myGeomGUI->EraseSimulationShape() ;

  switch(myConstructorId)
    { 
    case 0 :
      { 
	GroupC1->hide();
	GroupC2->show();
	myConstructorId = constructorId ;
        myOkRadius1 = myOkPoint1 = myOkRadius2 = myOkDir = true ;

	SpinBox_C1A3->SetValue( 300.0 ) ;   /* radius 1 */
	SpinBox_C1A4->SetValue( 100.0 ) ;   /* radius 2 */
	myRadius1 = 300.0 ;
	myRadius2 = 100.0 ;
	
	disconnect( mySelection, 0, this, 0 );

	myPoint1.SetCoord( 0.0, 0.0, 0.0 ) ;
	myDir.SetCoord( 0.0, 0.0, 1.0 ) ;

	if( myOkPoint1 && myOkDir && myOkRadius1 && myOkRadius2 ) {
	  gp_Dir aDir = myDir ;
	  
	  MakeTorusSimulationAndDisplay() ;
	} 
	break ;
      }
    case 1 :
      { 
	GroupC2->hide();
	GroupC1->show();
	myConstructorId = constructorId ;
	myEditCurrentArgument = LineEditC1A1 ;
        myOkRadius1 =  myOkRadius2 = true ;
	myOkPoint1 = myOkDir = false ;

	SpinBox_Radius1->SetValue( 300.0 ) ;
	SpinBox_Radius2->SetValue( 100.0 ) ;
	myRadius1 = 300.0 ;
	myRadius2 = 100.0 ;

	LineEditC1A1->setText( tr("") );
	disconnect( mySelection, 0, this, 0 );
	break ;
      }
    }
  return ;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo( tr("") ) ; 
  myGeomGUI->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  switch(myConstructorId)
    { 
    case 0 :
      { 
	if(myOkPoint1 && myOkDir && myOkRadius1 && myOkRadius2 ) {	  
	  myGeomGUI->MakeTorusAndDisplay( myPoint1, myDir, myRadius1, myRadius2 ) ; 
	}
	break ;
      }
    case 1 :
      { 
	if(myOkPoint1 && myOkDir && myOkRadius1 && myOkRadius2 ) {
	  myGeomGUI->MakeTorusAndDisplay( myPoint1, myDir, myRadius1, myRadius2 ) ; 
	}
	break ;
      }
    }
  // accept();
  return ;
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::ClickOnOk()
{
  this->ClickOnApply() ;
  this->ClickOnCancel() ;

  return ;
}

//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::ClickOnCancel()
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
void GeometryGUI_TorusDlg::SelectionIntoArgument()
{
  myGeomGUI->EraseSimulationShape() ; 
  mySimulationTopoDs.Nullify() ;
  
  QString aString = ""; /* Name of future selection */
  
  int nbSel = myGeomGUI->GetNameOfSelectedIObjects(mySelection, aString) ;
  if ( nbSel != 1 ) {
    if ( myEditCurrentArgument == LineEditC1A1 ) {
      LineEditC1A1->setText("") ;
      myOkPoint1 = false ;
    }
    else if ( myEditCurrentArgument == LineEditC1A2 ) { 
      LineEditC1A2->setText("") ;
      myOkDir = false ;
    }
    return ;
  }

  /* nbSel == 1 */
  TopoDS_Shape S;  
  if( !myGeomGUI->GetTopoFromSelection(mySelection, S) )
    return ;  
 
  /*  gp_Pnt : not used */
  if ( myEditCurrentArgument == LineEditC1A1 && myGeomGUI->VertexToPoint(S, myPoint1) ) {
    LineEditC1A1->setText(aString) ;
    myOkPoint1 = true ;
  }    
  else if ( myEditCurrentArgument == LineEditC1A2 /*&& myGeomGUI->LinearLocationAndDirection(S, notUsed, myDir) */) {
    BRepAdaptor_Curve curv(TopoDS::Edge(S));
    myDir = curv.Line().Direction();
    LineEditC1A2->setText(aString) ;
    myOkDir = true ;
  }

  if( myConstructorId == 0 && myOkPoint1 && myOkDir && myOkRadius1 && myOkRadius2 ) {
    MakeTorusSimulationAndDisplay() ;
  }
  return ;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  mySelection->ClearFilters() ;
  switch (myConstructorId)
    {
    case 0: /* default constructor */
      {	
	if(send == SelectButtonC1A1) {
	  LineEditC1A1->setFocus() ;
	  myEditCurrentArgument = LineEditC1A1;
	  mySelection->AddFilter(myVertexFilter) ;
	}
	else if(send == SelectButtonC1A2) {
	  LineEditC1A2->setFocus() ;	  
	  myEditCurrentArgument = LineEditC1A2;
	  mySelection->AddFilter(myEdgeFilter) ;
	}	
	SelectionIntoArgument() ;
	break;
      }
     case 1:
      {	
	break ;
      }
    }

  return ;
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();
  if( send == LineEditC1A1 )
    myEditCurrentArgument = LineEditC1A1 ;
  else if ( send == LineEditC1A2)
    myEditCurrentArgument = LineEditC1A2; 
  else
    return ;
  
  /* User name of object input management                          */
  /* If successfull the selection is changed and signal emitted... */
  /* so SelectionIntoArgument() is automatically called.           */
  QLineEdit* LE = (QLineEdit*)myEditCurrentArgument ;
  const QString objectUserName = LE->text() ;
  QWidget* thisWidget = (QWidget*)this ;
  if( myGeomGUI->SelectionByNameInDialogs( thisWidget, objectUserName, mySelection ) ) {
    LE->setText( objectUserName ) ;
  }
  
  return ;
}


//=================================================================================
// function : ValueChangedInSpinBox
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::ValueChangedInSpinBox( double newValue )
{  
  QObject* send = (QObject*)sender();

  if(send == SpinBox_C1A3 ) {
    myRadius1 = newValue ;
    myOkRadius1 = true ;
  }
  else if(send == SpinBox_C1A4 ) {   
    myRadius2 = newValue ;
    myOkRadius2 = true ;
  }
  else if(send == SpinBox_Radius1 ) {  
    myRadius1 = newValue ;
    myOkRadius1 = true ;
  }
  else if(send == SpinBox_Radius2 ) {
    myRadius2 = newValue ;
    myOkRadius2 = true ;
  }
  
  if (myOkPoint1 && myOkDir && myOkRadius1 && myOkRadius2 ) {
    MakeTorusSimulationAndDisplay() ;
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
void GeometryGUI_TorusDlg::DeactivateActiveDialog()
{
  if ( GroupConstructors->isEnabled() ) {
    GroupConstructors->setEnabled(false) ;
    GroupC1->setEnabled(false) ;
    GroupC2->setEnabled(false) ;
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
void GeometryGUI_TorusDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog() ;   
  GroupConstructors->setEnabled(true) ;
  GroupC1->setEnabled(true) ;
  GroupC2->setEnabled(true) ;
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
void GeometryGUI_TorusDlg::enterEvent(QEvent* e)
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
void GeometryGUI_TorusDlg::closeEvent( QCloseEvent* e )
{
  /* same than click on cancel button */
  this->ClickOnCancel() ;
  return ;
}


//=================================================================================
// function : MakeTorusSimulationAndDisplay()
// purpose  :
//=================================================================================
void GeometryGUI_TorusDlg::MakeTorusSimulationAndDisplay() 
{
  myGeomGUI->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  
  try {
    gp_Ax2 anAxis(this->myPoint1, this->myDir) ;
    mySimulationTopoDs = BRepPrimAPI_MakeTorus( anAxis, this->myRadius1, this->myRadius2 ).Shape() ;
    myGeomGUI->DisplaySimulationShape( mySimulationTopoDs ) ;
  }
  catch(Standard_Failure) {
    MESSAGE( "Exception catched in MakeTorusSimulationAndDisplay" ) ;
  }
  return ;
}

