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
//  File   : MeasureGUI_PropertiesDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "MeasureGUI_PropertiesDlg.h"

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

#include <TopExp_Explorer.hxx>


//=================================================================================
// class    : MeasureGUI_PropertiesDlg()
// purpose  : Constructs a MeasureGUI_PropertiesDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
MeasureGUI_PropertiesDlg::MeasureGUI_PropertiesDlg( QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_BASICPROPERTIES")));
    QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_SELECT")));

    if ( !name )
	setName( "MeasureGUI_PropertiesDlg" );
    resize( 303, 275 ); 
    setCaption( tr( "GEOM_PROPERTIES_TITLE"  ) );
    setSizeGripEnabled( TRUE );
    MeasureGUI_PropertiesDlgLayout = new QGridLayout( this ); 
    MeasureGUI_PropertiesDlgLayout->setSpacing( 6 );
    MeasureGUI_PropertiesDlgLayout->setMargin( 11 );
    
    /***************************************************************/
    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setTitle( tr( "GEOM_PROPERTIES"  ) );
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
    Constructor1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, Constructor1->sizePolicy().hasHeightForWidth() ) );
    Constructor1->setMinimumSize( QSize( 60, 0 ) );
    GroupConstructorsLayout->addWidget( Constructor1, 0, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupConstructorsLayout->addItem( spacer, 0, 1 );
    MeasureGUI_PropertiesDlgLayout->addWidget( GroupConstructors, 0, 0 );

    /***************************************************************/
    GroupConstructor1 = new QGroupBox( this, "GroupConstructor1" );
    GroupConstructor1->setTitle( tr( "GEOM_PROPERTIES_CONSTR"  ) );
    GroupConstructor1->setColumnLayout(0, Qt::Vertical );
    GroupConstructor1->layout()->setSpacing( 0 );
    GroupConstructor1->layout()->setMargin( 0 );
    GroupConstructor1Layout = new QGridLayout( GroupConstructor1->layout() );
    GroupConstructor1Layout->setAlignment( Qt::AlignTop );
    GroupConstructor1Layout->setSpacing( 6 );
    GroupConstructor1Layout->setMargin( 11 );
    LineEditC1A1 = new QLineEdit( GroupConstructor1, "LineEditC1A1" );
    LineEditC1A1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEditC1A1->sizePolicy().hasHeightForWidth() ) );
    GroupConstructor1Layout->addWidget( LineEditC1A1, 0, 2 );
    SelectButtonC1A1 = new QPushButton( GroupConstructor1, "SelectButtonC1A1" );
    SelectButtonC1A1->setText( tr( ""  ) );
    SelectButtonC1A1->setPixmap( image1 );
    GroupConstructor1Layout->addWidget( SelectButtonC1A1, 0, 1 );
    TextLabelC1A1 = new QLabel( GroupConstructor1, "TextLabelC1A1" );
    TextLabelC1A1->setText( tr( "GEOM_OBJECT"  ) );
    TextLabelC1A1->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A1->setFrameShape( QLabel::NoFrame );
    TextLabelC1A1->setFrameShadow( QLabel::Plain );
    GroupConstructor1Layout->addWidget( TextLabelC1A1, 0, 0 );
    TextLabel_Length = new QLabel( GroupConstructor1, "TextLabel_Length" );
    TextLabel_Length->setText( tr( "GEOM_LENGTH"  ) );
    TextLabel_Length->setMinimumSize( QSize( 50, 0 ) );
    TextLabel_Length->setFrameShape( QLabel::NoFrame );
    TextLabel_Length->setFrameShadow( QLabel::Plain );
    GroupConstructor1Layout->addWidget( TextLabel_Length, 1, 0 );
    LineEdit_Length = new QLineEdit( GroupConstructor1, "LineEdit_Length" );
    LineEdit_Length->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEdit_Length->sizePolicy().hasHeightForWidth() ) );
    //    LineEdit_Length->setEnabled( FALSE );
    LineEdit_Length->setReadOnly( TRUE );
    GroupConstructor1Layout->addWidget( LineEdit_Length, 1, 2 );
    LineEdit_Surface = new QLineEdit( GroupConstructor1, "LineEdit_Surface" );
    LineEdit_Surface->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEdit_Surface->sizePolicy().hasHeightForWidth() ) );
    //LineEdit_Surface->setEnabled( FALSE );
    LineEdit_Surface->setReadOnly( TRUE );
    GroupConstructor1Layout->addWidget( LineEdit_Surface, 2, 2 );
    TextLabel_Surface = new QLabel( GroupConstructor1, "TextLabel_Surface" );
    TextLabel_Surface->setText( tr( "GEOM_PROPERTIES_SURFACE"  ) );
    TextLabel_Surface->setMinimumSize( QSize( 50, 0 ) );
    TextLabel_Surface->setFrameShape( QLabel::NoFrame );
    TextLabel_Surface->setFrameShadow( QLabel::Plain );
    GroupConstructor1Layout->addWidget( TextLabel_Surface, 2, 0 );
    TextLabel_Volume = new QLabel( GroupConstructor1, "TextLabel_Volume" );
    TextLabel_Volume->setText( tr( "GEOM_PROPERTIES_VOLUME"  ) );
    TextLabel_Volume->setMinimumSize( QSize( 50, 0 ) );
    TextLabel_Volume->setFrameShape( QLabel::NoFrame );
    TextLabel_Volume->setFrameShadow( QLabel::Plain );
    GroupConstructor1Layout->addWidget( TextLabel_Volume, 3, 0 );
    LineEdit_Volume = new QLineEdit( GroupConstructor1, "LineEdit_Volume" );
    LineEdit_Volume->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)0, LineEdit_Volume->sizePolicy().hasHeightForWidth() ) );
    //LineEdit_Volume->setEnabled( FALSE );
    LineEdit_Volume->setReadOnly( TRUE );
    GroupConstructor1Layout->addWidget( LineEdit_Volume, 3, 2 );
    MeasureGUI_PropertiesDlgLayout->addWidget( GroupConstructor1, 1, 0 );

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
    GroupButtonsLayout->addWidget( buttonCancel, 0, 1 );
 
    QSpacerItem* spacer_8 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupButtonsLayout->addItem( spacer_8, 0, 0 );
    QSpacerItem* spacer_9 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    GroupButtonsLayout->addItem( spacer_9, 0, 2 );

    MeasureGUI_PropertiesDlgLayout->addWidget( GroupButtons, 2, 0 );
    /***************************************************************/

    Init(Sel) ; /* Initialisations */
}


//=================================================================================
// function : ~MeasureGUI_PropertiesDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MeasureGUI_PropertiesDlg::~MeasureGUI_PropertiesDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::Init( SALOME_Selection* Sel )
{
  myConstructorId = 0 ;
  Constructor1->setChecked( TRUE );
  myEditCurrentArgument = LineEditC1A1 ;	
  mySelection = Sel;
  myGeomBase = new GEOMBase() ;
  myGeomGUI = GEOMContext::GetGeomGUI() ;
  myGeomGUI->SetActiveDialogBox( (QDialog*)this ) ;
  
  // TODO : previous selection into argument ?

  /* Filter definitions */
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);

  /* signals and slots connections */
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  connect( GroupConstructors, SIGNAL(clicked(int) ), SLOT( ConstructorsClicked(int) ) );
  connect( SelectButtonC1A1, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;
  connect( mySelection, SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  connect( LineEditC1A1, SIGNAL ( returnPressed() ), this, SLOT( LineEditReturnPressed() ) ) ;
  
  connect( myGeomGUI, SIGNAL ( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) ) ;
  /* to close dialog if study change */
  connect( myGeomGUI, SIGNAL ( SignalCloseAllDialogs() ), this, SLOT( ClickOnCancel() ) ) ;

  /* Move widget on the botton right corner of main widget */
  int x, y ;
  myGeomBase->DefineDlgPosition( this, x, y ) ;
  this->move( x, y ) ;
  this->show() ; /* displays Dialog */
  return ;
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void MeasureGUI_PropertiesDlg::ConstructorsClicked(int constructorId)
{
  return ;
}



//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::ClickOnCancel()
{
  myGeomBase->EraseSimulationShape() ;
  disconnect( mySelection, 0, this, 0 );
  myGeomBase->EraseSimulationShape() ; 
  myGeomGUI->ResetState() ;
  reject() ;
  return ;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void MeasureGUI_PropertiesDlg::SelectionIntoArgument()
{
  myGeomBase->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  
  LineEdit_Length->setText("") ;
  LineEdit_Surface->setText("") ;
  LineEdit_Volume->setText("") ;
  myEditCurrentArgument->setText("") ;

  QString aString = ""; /* future the name of selection */

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString) ;
  if ( nbSel != 1 ) {
    return ;
  }

  /*  nbSel == 1  */ 
  TopoDS_Shape S;
  if( !myGeomBase->GetTopoFromSelection(mySelection, S) )
    return ;
  
  if( S.IsNull() || S.ShapeType() == TopAbs_VERTEX ) {
    myEditCurrentArgument->setText( "" );
    return ;
  }
 
  LineEditC1A1->setText(aString) ;

  /* Try to display of a cone simulation shape to show direction of a linear edge only in OCC viewer */
  if( myGeomBase->CreateArrowForLinearEdge( S, mySimulationTopoDs ) ) {
    myGeomBase->DisplaySimulationShape( mySimulationTopoDs ) ;
  }
  this->CalculateAndDisplayProperties(S) ;
  
  return ;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  switch (myConstructorId)
    {
    case 0: /* default constructor */
      {	
	if(send == SelectButtonC1A1) {
	  LineEditC1A1->setFocus() ;
	  myEditCurrentArgument = LineEditC1A1;
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
void MeasureGUI_PropertiesDlg::LineEditReturnPressed()
{  
  QLineEdit* send = (QLineEdit*)sender();  
  if( send == LineEditC1A1 )
    myEditCurrentArgument = LineEditC1A1 ;
  else
    return ;
  
  /* User name of object input management                          */
  /* If successfull the selection is changed and signal emitted... */
  /* so SelectionIntoArgument() is automatically called.           */
  const QString objectUserName = myEditCurrentArgument->text() ;
  QWidget* thisWidget = (QWidget*)this ;
  if( myGeomBase->SelectionByNameInDialogs( thisWidget, objectUserName, mySelection ) ) {
    myEditCurrentArgument->setText( objectUserName ) ;
  }
  return ;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::DeactivateActiveDialog()
{
  if ( GroupConstructors->isEnabled() ) {    
    disconnect( mySelection, 0, this, 0 );
    GroupConstructors->setEnabled(false) ;
    GroupConstructor1->setEnabled(false) ;
    myGeomBase->EraseSimulationShape() ;
    GroupButtons->setEnabled(false) ;
  }
  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog() ;   
  GroupConstructors->setEnabled(true) ;
  GroupConstructor1->setEnabled(true) ;
  GroupButtons->setEnabled(true) ;
  connect( mySelection, SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  
  if( !mySimulationTopoDs.IsNull() )
    myGeomBase->DisplaySimulationShape( mySimulationTopoDs ) ;
  
  return ;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::enterEvent(QEvent* e)
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
void MeasureGUI_PropertiesDlg::closeEvent( QCloseEvent* e )
{
  /* same than click on cancel button */
  this->ClickOnCancel() ;
  return ;
}


//=================================================================================
// function : CalculateAndDisplayProperties()
// purpose  :
//=================================================================================
void MeasureGUI_PropertiesDlg::CalculateAndDisplayProperties(const TopoDS_Shape& S)
{
  LineEdit_Length->setText("") ;
  LineEdit_Surface->setText("") ;
  LineEdit_Volume->setText("") ;
  if( S.IsNull() ) 
    return ;

  Standard_Real result;
  GProp_GProps LProps;
  GProp_GProps SProps;
  QString resString;

  try 
    {
      BRepGProp::LinearProperties(S,LProps);
      result = LProps.Mass();
      if (!IsEqual( result, 0.0))
	{	
	  resString = tr("%1").arg( result, 12, 'f', 6 ) ;
	  LineEdit_Length->setText(resString) ;
	}

      BRepGProp::SurfaceProperties(S, SProps);
      result = SProps.Mass();
      if (!IsEqual( result, 0.0))
	{	
	  resString = tr("%1").arg( result, 12, 'f', 6 ) ;
	  LineEdit_Surface->setText(resString) ;
	}
	
      result = 0.0;
      if (S.ShapeType() < TopAbs_SHELL)
	{
	  for( TopExp_Explorer Exp(S,TopAbs_SOLID); Exp.More(); Exp.Next() ) 
	    { 
	      GProp_GProps VProps;
	      BRepGProp::VolumeProperties(Exp.Current(), VProps);
	      result += VProps.Mass();
	    }
	}
      if (!IsEqual( result, 0.0 ))
	{	
	  resString = tr("%1").arg( result, 12, 'f', 6 ) ;
	  LineEdit_Volume->setText(resString) ;
	}
    }
  catch(Standard_Failure) 
    {
      MESSAGE("Catch intercepted in CalculateAndDisplayProperties()" << endl ) ;
    }
  return ;
}
