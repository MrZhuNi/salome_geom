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
//  File   : RepairGUI_OrientationDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "RepairGUI_OrientationDlg.h"

#include <Precision.hxx>
#include "QAD_Config.h"

#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <TopExp_Explorer.hxx>

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
#include <qvalidator.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

//=================================================================================
// class    : RepairGUI_OrientationDlg()
// purpose  : Constructs a RepairGUI_OrientationDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
RepairGUI_OrientationDlg::RepairGUI_OrientationDlg( QWidget* parent, const char* name, RepairGUI* theRepairGUI, SALOME_Selection* Sel, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_DLG_ORIENTATION")));
    QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap( "GEOM",tr("ICON_SELECT")));
    if ( !name )
	setName( "RepairGUI_OrientationDlg" );
    resize( 303, 242 ); 
    setCaption( tr( "GEOM_ORIENTATION_TITLE"  ) );
    setSizeGripEnabled( TRUE );
    RepairGUI_OrientationDlgLayout = new QGridLayout( this ); 
    RepairGUI_OrientationDlgLayout->setSpacing( 6 );
    RepairGUI_OrientationDlgLayout->setMargin( 11 );
    
    /***************************************************************/
    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setTitle( tr( "GEOM_ORIENTATION"  ) );
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
    RepairGUI_OrientationDlgLayout->addWidget( GroupConstructors, 0, 0 );    
    
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
    RepairGUI_OrientationDlgLayout->addWidget( GroupButtons, 2, 0 );

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
    LineEditC1A1 = new QLineEdit( GroupC1, "LineEditC1A1" );
    GroupC1Layout->addWidget( LineEditC1A1, 0, 2 );

    /* Spin box construction */
    SpinBox_C1A2 = new DlgRef_SpinBox( GroupC1, "GeomSpinBox_C1A2" ) ;
    GroupC1Layout->addWidget( SpinBox_C1A2, 1, 2 );
    
    SelectButtonC1A1 = new QPushButton( GroupC1, "SelectButtonC1A1" );
    SelectButtonC1A1->setText( tr( ""  ) );
    SelectButtonC1A1->setPixmap( image1 );
    SelectButtonC1A1->setToggleButton( FALSE );
    GroupC1Layout->addWidget( SelectButtonC1A1, 0, 1 );
    TextLabelC1A1 = new QLabel( GroupC1, "TextLabelC1A1" );
    TextLabelC1A1->setText( tr( "GEOM_OBJECT"  ) );
    TextLabelC1A1->setMinimumSize( QSize( 50, 0 ) );
    TextLabelC1A1->setFrameShape( QLabel::NoFrame );
    TextLabelC1A1->setFrameShadow( QLabel::Plain );
    GroupC1Layout->addWidget( TextLabelC1A1, 0, 0 );
    TextLabelC1A2 = new QLabel( GroupC1, "TextLabelC1A2" );
    TextLabelC1A2->setText( tr( "GEOM_VECTOR_LENGTH"  ) );
    TextLabelC1A2->setMinimumSize( QSize( 50, 0 ) );
    GroupC1Layout->addWidget( TextLabelC1A2, 1, 0 );
    CheckBoxReverse = new QCheckBox( GroupC1, "CheckBoxReverse" );
    CheckBoxReverse->setText( tr( "GEOM_ORIENTATION_OPT"  ) );
    GroupC1Layout->addMultiCellWidget( CheckBoxReverse, 2, 2, 0, 2 );
    RepairGUI_OrientationDlgLayout->addWidget( GroupC1, 1, 0 );
    /***************************************************************/
    myRepairGUI = theRepairGUI;
    Init(Sel) ; /* Initialisations */

}

//=================================================================================
// function : ~RepairGUI_OrientationDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
RepairGUI_OrientationDlg::~RepairGUI_OrientationDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::Init( SALOME_Selection* Sel )
{

  double step ;
  QString St = QAD_CONFIG->getSetting( "Geometry:SettingsGeomStep" ) ;
  step = St.toDouble() ;

  /* min, max, step and decimals for spin boxes */
  SpinBox_C1A2->RangeStepAndValidator( 0.001, 999.999, step, 3 ) ;
  SpinBox_C1A2->SetValue( 25.0 ) ; /* = myLength */

  GroupC1->show();
  myConstructorId = 0 ;
  Constructor1->setChecked( TRUE );
  CheckBoxReverse->setChecked( FALSE );
  myEditCurrentArgument = LineEditC1A1 ;	
  mySelection = Sel;
  myGeomBase = new GEOMBase() ;
  myGeomGUI = GEOMContext::GetGeomGUI() ;

  myLength = 25.0 ;
  myOkShape = false ;
  myOkLength = true;
  mySimulationTopoDs.Nullify() ;
  myShape.Nullify() ;
  myGeomGUI->SetActiveDialogBox( (QDialog*)this ) ;

  // TODO : previous selection into argument ?

  /* Filter definitions */
  Engines::Component_var comp = QAD_Application::getDesktop()->getEngine("FactoryServer", "GEOM");
  myGeom = GEOM::GEOM_Gen::_narrow(comp);

  /* signals and slots connections */
  connect( buttonOk, SIGNAL( clicked() ),     this, SLOT( ClickOnOk() ) );
  connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( ClickOnCancel() ) ) ;
  connect( buttonApply, SIGNAL( clicked() ),  this, SLOT(ClickOnApply() ) );
  connect( GroupConstructors, SIGNAL(clicked(int) ), SLOT( ConstructorsClicked(int) ) );  
  connect( SelectButtonC1A1, SIGNAL (clicked() ), this, SLOT( SetEditCurrentArgument() ) ) ;
  connect( CheckBoxReverse, SIGNAL (stateChanged(int) ), this, SLOT( ReverseOrientation(int) ) ) ;

  connect( LineEditC1A1, SIGNAL ( returnPressed() ),                   this, SLOT( LineEditReturnPressed() ) ) ;
  connect( SpinBox_C1A2, SIGNAL ( valueChanged( double) ), this, SLOT( ValueChangedInSpinBox( double) ) ) ;

  connect( myGeomGUI, SIGNAL ( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) ) ;  
  connect( mySelection, SIGNAL( currentSelectionChanged() ),     this, SLOT( SelectionIntoArgument() ) );
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
void RepairGUI_OrientationDlg::ConstructorsClicked(int constructorId)
{
  return ;
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::ClickOnOk()
{
  this->ClickOnApply() ;
  this->ClickOnCancel() ;

  return ;
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::ClickOnApply()
{
  /* Leave simulation display in this method ! */
  myGeomGUI->GetDesktop()->putInfo( tr("") ) ; 
  
  switch(myConstructorId)
    { 
    case 0 :
      { 
	if(myOkShape && CheckBoxReverse->isChecked() && myOkLength == true ) {	  
	  myRepairGUI->MakeOrientationChangeAndDisplay( myGeomShape ) ;
	  CheckBoxReverse->setChecked( FALSE );
	}
	break ;
      }
    }
  // accept();
  return ;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::ClickOnCancel()
{
  myGeomBase->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  disconnect( mySelection, 0, this, 0 );
  myGeomGUI->ResetState() ;
  reject() ;
  return ;
}

//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::LineEditReturnPressed()
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
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void RepairGUI_OrientationDlg::SelectionIntoArgument()
{
  myGeomBase->EraseSimulationShape() ; 
  this->mySimulationTopoDs.Nullify() ;
  
  /* Name of future selection */
  QString aString = "";

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString) ;
  if ( nbSel != 1 ) {
    if ( myEditCurrentArgument == LineEditC1A1 ) {
      LineEditC1A1->setText("") ;
      this->myOkShape = false ;
    }
    return ;
  }
  
  /* nbSel == 1 ! */
  TopoDS_Shape S;
  Standard_Boolean testResult ;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject() ;
  if( !myGeomBase->GetTopoFromSelection(mySelection, S) )
    return ;  
  
  /* Constructor */
  if ( myEditCurrentArgument == LineEditC1A1 ) { 
    myGeomShape = myGeomBase->ConvertIOinGEOMShape(IO, testResult) ;
    if( !testResult )
      return ;
    LineEditC1A1->setText(aString) ;
    myShape = S ;
    myOkShape = true ;
  }
  
  if( myOkShape && myOkLength ) {
    MakeOrientationSimulationAndDisplay( this->myShape, this->myLength ) ;
  }
  return ;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  switch (myConstructorId)
    {
    case 0: /* default constructor */
      {	
	if(send == SelectButtonC1A1) {
	  LineEditC1A1->setFocus() ;
	  myEditCurrentArgument = LineEditC1A1 ;
	}
	SelectionIntoArgument() ;
	break;
      }
    }
  return ;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::ValueChangedInSpinBox( double newValue )
{ 
  myGeomBase->EraseSimulationShape() ;
  mySimulationTopoDs.Nullify() ;
  
  QObject* send = (QObject*)sender() ; 
  if( send == SpinBox_C1A2 ) {
    this->myLength = newValue ;
    myOkLength = true ;
  } 
  
  if ( myConstructorId == 0 && myOkShape && myOkLength == true ) {
    MakeOrientationSimulationAndDisplay( this->myShape, this->myLength ) ;
  }
  return ;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::DeactivateActiveDialog()
{
  if ( GroupConstructors->isEnabled() ) {
    GroupConstructors->setEnabled(false) ;
    GroupButtons->setEnabled(false) ;
    GroupC1->setEnabled(false) ;
    disconnect( mySelection, 0, this, 0 );
    myGeomBase->EraseSimulationShape() ;
  }
  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog() ;
  GroupConstructors->setEnabled(true) ;
  GroupButtons->setEnabled(true) ;
  GroupC1->setEnabled(true) ;
  
  connect ( mySelection, SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );
  if( !mySimulationTopoDs.IsNull() )
    myGeomBase->DisplaySimulationShape( mySimulationTopoDs ) ;

  return ;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void RepairGUI_OrientationDlg::enterEvent(QEvent* e)
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
void RepairGUI_OrientationDlg::closeEvent( QCloseEvent* e )
{
  /* same than click on cancel button */
  this->ClickOnCancel() ;
  return ;
}



//===========================================================================================
// function : ReverseOrientation()
// purpose  :
//===========================================================================================
void RepairGUI_OrientationDlg::ReverseOrientation(int state)
{
  if( this->myOkShape && myOkLength == true ) {
    MakeOrientationSimulationAndDisplay( this->myShape, this-> myLength ) ;
  }
  return ;
}


//===========================================================================================
// function : MakeOrientationSimulationAndDisplay()
// purpose  : Create 'aCompound1' and 'aCompound2' each contains edges oriented
//	    : respectively FORWARD and REVERSE for all faces of 'aTopoDS'
//	    : These edges represent normal vectors on faces of 'aTopoDS'
//          : For a unique edge an arrow is displayed to show its orientation.
//===========================================================================================
void RepairGUI_OrientationDlg::MakeOrientationSimulationAndDisplay(const TopoDS_Shape& aTopoDS, Standard_Real length )
{
  myGeomBase->EraseSimulationShape() ; 
  mySimulationTopoDs.Nullify() ;
  
  TopoDS_Compound aCompound1, aCompound2 ;
  TopoDS_Compound NullComp ;
  BRep_Builder aBuilder1, aBuilder2;
  aCompound1 = aCompound2 = NullComp ;
  aBuilder1.MakeCompound(aCompound1) ;
  aBuilder2.MakeCompound(aCompound2) ;
  
  if( aTopoDS.IsNull() )
    return ;
  
  /* Case of an edge */
  if( aTopoDS.ShapeType() == TopAbs_EDGE ) {
    /* Try to display a cone simulation shape to show direction of edge */
    TopoDS_Shape tmpShape = aTopoDS ;
    if( this->CheckBoxReverse->isChecked() ) {
      if( aTopoDS.Orientation() == TopAbs_FORWARD)
	tmpShape.Orientation(TopAbs_REVERSED) ;
      else
	tmpShape.Orientation(TopAbs_FORWARD) ;
    }
    if( myGeomBase->CreateArrowForLinearEdge( tmpShape, mySimulationTopoDs ) ) {
      myGeomBase->DisplaySimulationShape( mySimulationTopoDs ) ;
    }
    return ;
  }
  

  gp_Pnt P1, P2 ;
  gp_Vec V, V1, V2 ;
  TopExp_Explorer ex( aTopoDS, TopAbs_FACE );
  int test = 0 ;
  while (ex.More()) {
    
    const TopoDS_Face& F = TopoDS::Face(ex.Current());
    /* find the center of the minmax */
    BRepAdaptor_Surface SF(F);
    Standard_Real u, v, x;
    
    u = SF.FirstUParameter();
    x = SF.LastUParameter();
    if ( Precision::IsInfinite(u) ) {
      if( Precision::IsInfinite(x) ) u = 0.0 ;  else u = x ;
    }
    else if ( !Precision::IsInfinite(x) )
      u = (u+x) / 2.0 ;
    
    v = SF.FirstVParameter();
    x = SF.LastVParameter();
    if ( Precision::IsInfinite(v) ) {
      if ( Precision::IsInfinite(x) ) v = 0.0 ;  else v = x ;
    }
    else if ( !Precision::IsInfinite(x) )
      v = (v+x) / 2.0 ;
    
    SF.D1( u, v, P1, V1, V2 );
    V = V1.Crossed(V2);
    x = V.Magnitude();
    if ( x > 1.e-10 )
      V.Multiply( length/x );
    else {
      V.SetCoord( length/2.0, 0.0, 0.0 ) ;
      MESSAGE ("Null normal in Orientation " << endl ) ;
    }
    
    P2 = P1;
    /* test orientation of each face and displays forward (aCompound1) */
    if( F.Orientation() == TopAbs_FORWARD )
      P2.Translate(V);
    else
      P2.Translate(-V) ;    
    BRepBuilderAPI_MakeEdge anEdge(P1, P2) ;
    aBuilder1.Add( aCompound1, anEdge ) ;
    
    P2 = P1;
    /* test orientation of each face and displays forward (aCompound2) */
    if( F.Orientation() == TopAbs_FORWARD )
      P2.Translate(-V);
    else
      P2.Translate(V) ;    
    anEdge = BRepBuilderAPI_MakeEdge(P1, P2) ;
    aBuilder2.Add( aCompound2, anEdge ) ;
    
    ex.Next();
    test++ ;
  }
  
  /* display simulation compounds */
  if( test > 0 && this->CheckBoxReverse->isChecked() ) {
    mySimulationTopoDs = aCompound1 ;    
  }
  else if ( test > 0 && !CheckBoxReverse->isChecked() ) {
    mySimulationTopoDs = aCompound2 ;
  }
  if(!mySimulationTopoDs.IsNull() )
    myGeomBase->DisplaySimulationShape(mySimulationTopoDs) ;
  
  return ;
}
