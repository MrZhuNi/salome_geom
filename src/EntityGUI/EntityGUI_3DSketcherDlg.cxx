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
// File   : EntityGUI_3DSketcherDlg.cxx
// Author : DMV, OCN
//

#include "EntityGUI_3DSketcherDlg.h"
#include "EntityGUI_Widgets.h"
#include <QtxDoubleSpinBox.h>

#include <GEOMBase.h>
#include <GeometryGUI.h>
#include <GEOMImpl_Types.hxx>
#include <Precision.hxx>  

#include <DlgRef.h>
#include <SUIT_Desktop.h>
#include <SUIT_Session.h>
#include <SUIT_MessageBox.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
#include <LightApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <BRep_Tool.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

class Locker
{
public:
  Locker( bool& l ) : myLock( l ) { myLock = true;  }
  ~Locker()                       { myLock = false; }
private:
  bool& myLock;
};

//=================================================================================
// class    : EntityGUI_3DSketcherDlg()
// purpose  : Constructs a EntityGUI_3DSketcherDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
EntityGUI_3DSketcherDlg::EntityGUI_3DSketcherDlg( GeometryGUI* theGeometryGUI, QWidget* parent,
					          bool modal, Qt::WindowFlags fl )
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );
  QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_UNDO" ) ) );
  QPixmap image2( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_REDO" ) ) );
  QPixmap image3( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICO_3DSKETCH" ) ) );

  setWindowTitle( tr( "GEOM_3DSKETCHER_TITLE" ) );

  /***************************************************************/

  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_3DSKETCHER" ) );
  mainFrame()->RadioButton1->setIcon( image3 );;
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->close();

  GroupType = new DlgRef_3Radio(centralWidget());
  GroupType->GroupBox1->setTitle(tr("GEOM_COORDINATES_TYPE"));
  GroupType->RadioButton1->setText(tr("GEOM_SKETCHER_ABS"));
  GroupType->RadioButton2->setText(tr("GEOM_SKETCHER_REL"));
  GroupType->RadioButton3->close();

  Group3Spin = new EntityGUI_3Spin( centralWidget() );
  Group3Spin->GroupBox1->setTitle( tr( "GEOM_SKETCHER_VALUES" ) );
  Group3Spin->buttonApply->setText( tr( "GEOM_SKETCHER_APPLY" ) );
  Group3Spin->buttonUndo->setIcon( image1 );
  Group3Spin->buttonRedo->setIcon( image2 );
  Group3Spin->TextLabel1->setText( tr( "GEOM_SKETCHER_X2" ) );
  Group3Spin->TextLabel2->setText( tr( "GEOM_SKETCHER_Y2" ) );
  Group3Spin->TextLabel3->setText( tr( "GEOM_SKETCHER_Z2" ) );

  buttonOk()->setText( tr( "GEOM_BUT_END_SKETCH" ) );
  buttonApply()->setText( tr( "GEOM_BUT_CLOSE_SKETCH" ) );
  
  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( GroupType );
  layout->addWidget( Group3Spin );

  setHelpFileName( "create_3dsketcher_page.html" );

  resize(0,0);
  Init();
}  



//=================================================================================
// function : ~EntityGUI_3DSketcherDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
EntityGUI_3DSketcherDlg::~EntityGUI_3DSketcherDlg()
{
  myGeomGUI->SetActiveDialogBox( 0 );
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::Init()
{
  myOK = false;

  localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );

  /* Get setting of step value from file configuration */
  double step = SUIT_Session::session()->resourceMgr()->doubleValue( "Geometry", "SettingsGeomStep", 100.0 );

  /* min, max, step and decimals for spin boxes */
  initSpinBox( Group3Spin->SpinBox_DX, COORD_MIN, COORD_MAX, step, 3 ); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox( Group3Spin->SpinBox_DY, COORD_MIN, COORD_MAX, step, 3 ); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox( Group3Spin->SpinBox_DZ, COORD_MIN, COORD_MAX, step, 3 ); // VSR: TODO: DBL_DIGITS_DISPLAY

  /* signals and slots connections */
  connect( this,           SIGNAL( constructorsClicked( int ) ), this, SLOT( PointClicked( int ) ) );

  connect( buttonOk(),     SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(),  SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );

  connect( myGeomGUI->getApp()->selectionMgr(), SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  connect( Group3Spin->buttonApply,  SIGNAL( clicked() ), this, SLOT( ClickOnAddPoint() ) );
  connect( Group3Spin->buttonUndo,   SIGNAL( clicked() ), this, SLOT( ClickOnUndo() ) );
  connect( Group3Spin->buttonRedo,   SIGNAL( clicked() ), this, SLOT( ClickOnRedo() ) ) ;

  connect( GroupType->RadioButton1,  SIGNAL( clicked() ), this, SLOT( TypeClicked() ) );
  connect( GroupType->RadioButton2,  SIGNAL( clicked() ), this, SLOT( TypeClicked() ) );

  connect( Group3Spin->SpinBox_DX, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );
  connect( Group3Spin->SpinBox_DY, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );
  connect( Group3Spin->SpinBox_DZ, SIGNAL( valueChanged( double ) ), this, SLOT( ValueChangedInSpinBox( double ) ) );

  connect( myGeomGUI, SIGNAL( SignalDefaultStepValueChanged( double ) ), this, SLOT( SetDoubleSpinBoxStep( double ) ) );

  connect( myGeomGUI, SIGNAL( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) );
  connect( myGeomGUI, SIGNAL( SignalCloseAllDialogs() ),        this, SLOT( ClickOnCancel() ) );

  initName( tr( "GEOM_3DSKETCHER" ) );

  UpdateButtonsState();
}

//=================================================================================
// function : TypeClicked()
// purpose  : Radio button management
//=================================================================================
void EntityGUI_3DSketcherDlg::TypeClicked()
{
  bool blocked = Group3Spin->SpinBox_DX->signalsBlocked();
  Group3Spin->SpinBox_DX->blockSignals(true);
  Group3Spin->SpinBox_DY->blockSignals(true);
  Group3Spin->SpinBox_DZ->blockSignals(true);
  // Get setting of step value from file configuration
  double x, y, z;
  GetLastPoints(x, y, z);
  if ( GroupType->RadioButton1->isChecked() ) {  // XY
    Group3Spin->TextLabel1->setText( tr( "GEOM_SKETCHER_X2" ) );
    Group3Spin->TextLabel2->setText( tr( "GEOM_SKETCHER_Y2" ) );
    Group3Spin->TextLabel3->setText( tr( "GEOM_SKETCHER_Z2" ) );
    Group3Spin->SpinBox_DX->setValue( x + Group3Spin->SpinBox_DX->value() );
    Group3Spin->SpinBox_DY->setValue( y + Group3Spin->SpinBox_DY->value() );
    Group3Spin->SpinBox_DZ->setValue( z + Group3Spin->SpinBox_DZ->value() );
    Group3Spin->buttonApply->setFocus();
  } else if ( GroupType->RadioButton2->isChecked() ) {  // DXDY
    Group3Spin->TextLabel1->setText( tr( "GEOM_SKETCHER_DX2" ) );
    Group3Spin->TextLabel2->setText( tr( "GEOM_SKETCHER_DY2" ) );
    Group3Spin->TextLabel3->setText( tr( "GEOM_SKETCHER_DZ2" ) );
    Group3Spin->SpinBox_DX->setValue( Group3Spin->SpinBox_DX->value() - x );
    Group3Spin->SpinBox_DY->setValue( Group3Spin->SpinBox_DY->value() - y );
    Group3Spin->SpinBox_DZ->setValue( Group3Spin->SpinBox_DZ->value() - z );
    Group3Spin->buttonApply->setFocus();
  }
  Group3Spin->SpinBox_DX->blockSignals(blocked);
  Group3Spin->SpinBox_DY->blockSignals(blocked);
  Group3Spin->SpinBox_DZ->blockSignals(blocked);
}

//=================================================================================
// function : ClickOnAddPoint()
// purpose  : called when the point coordinates is Applyed
//=================================================================================
void EntityGUI_3DSketcherDlg::ClickOnAddPoint()
{
  double x, y, z;
  GetCurrentPoints(x, y, z);
  myPointsList.append(x);
  myPointsList.append(y);
  myPointsList.append(z);

  /*if (myRedoList.size() >= 2) { // Remove this positions from Redo List
    for (int i = 0; i <= 2; i++)
      myRedoList.removeFirst();
  }*/
  myRedoList.clear();

  if ( GroupType->RadioButton2->isChecked() ) {
    Group3Spin->SpinBox_DX->setValue( 0.0 );
    Group3Spin->SpinBox_DY->setValue( 0.0 );
    Group3Spin->SpinBox_DZ->setValue( 0.0 );
  }
  UpdateButtonsState();
  displayPreview();
}

//=================================================================================
// function : UpdateButtonsState()
// purpose  : 
//=================================================================================
void EntityGUI_3DSketcherDlg::UpdateButtonsState()
{
  if ( !myPointsList.count() )
    GroupType->RadioButton1->setChecked( true );
  GroupType->RadioButton2->setEnabled( myPointsList.count() > 2 );
  Group3Spin->buttonUndo->setEnabled( myPointsList.count() > 2 );
  Group3Spin->buttonRedo->setEnabled( myRedoList.count() > 2 );
}

//=================================================================================
// function : ClickOnUndo()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::ClickOnUndo()
{
  if (myPointsList.count() > 2) {
    double x, y, z;
    GetLastPoints(x, y, z);
    myRedoList.append(x);
    myRedoList.append(y);
    myRedoList.append(z);
    myPointsList.removeLast();
    myPointsList.removeLast();
    myPointsList.removeLast();

    UpdateButtonsState();
    displayPreview();
  }
}

//=================================================================================
// function : ClickOnRedo()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::ClickOnRedo()
{
  int count = myRedoList.count();
  if ( count > 2 ) {
    myPointsList.append( myRedoList[count-3] );
    myPointsList.append( myRedoList[count-2] );
    myPointsList.append( myRedoList[count-1] );
    myRedoList.removeLast();
    myRedoList.removeLast();
    myRedoList.removeLast();

    UpdateButtonsState();
    displayPreview();
  }
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed
//=================================================================================
void EntityGUI_3DSketcherDlg::SelectionIntoArgument()
{
  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);

  int nbSel = aSelList.Extent();
  if ( nbSel == 1 ) {
    Standard_Boolean aRes = Standard_False;
    GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject(aSelList.First(), aRes);
    if (!CORBA::is_nil(aSelectedObject) && aRes) {
      TopoDS_Shape aShape;
      if (GEOMBase::GetShape(aSelectedObject, aShape, TopAbs_SHAPE)) { // Explore the shape if its a local selection
	TColStd_IndexedMapOfInteger aMap;
	aSelMgr->GetIndexes(aSelList.First(), aMap);
	if (aMap.Extent() == 1)
	  {
	    int anIndex = aMap(1);
	    GEOM::GEOM_IShapesOperations_var aShapesOp = getGeomEngine()->GetIShapesOperations(getStudyId());
	    aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
	  }
      }
      bool isOk = true;
      if ( aShape.ShapeType() != TopAbs_VERTEX )
	isOk = GEOMBase::GetShape(aSelectedObject, aShape, TopAbs_VERTEX);
      if (isOk) {
	gp_Pnt aPnt;
	if ( GEOMBase::VertexToPoint( aShape, aPnt ) ) {
	  // set coordinates to the Spin Boxes
	  double aX, aY, aZ;
	  aX = aPnt.X();
	  aY = aPnt.Y();
	  aZ = aPnt.Z();
	  bool blocked = Group3Spin->SpinBox_DX->signalsBlocked();
	  Group3Spin->SpinBox_DX->blockSignals(true);
	  Group3Spin->SpinBox_DY->blockSignals(true);
	  Group3Spin->SpinBox_DZ->blockSignals(true);
	  if ( GroupType->RadioButton1->isChecked() ) {
	    Group3Spin->SpinBox_DX->setValue( aX );
	    Group3Spin->SpinBox_DY->setValue( aY );
	    Group3Spin->SpinBox_DZ->setValue( aZ );
	  } else if ( GroupType->RadioButton2->isChecked() ) {
	    double x, y, z;
	    GetLastPoints(x, y, z);
	    Group3Spin->SpinBox_DX->setValue( aX - x );
	    Group3Spin->SpinBox_DY->setValue( aY - y );
	    Group3Spin->SpinBox_DZ->setValue( aZ - z );
	  }
	  Group3Spin->SpinBox_DX->blockSignals(blocked);
	  Group3Spin->SpinBox_DY->blockSignals(blocked);
	  Group3Spin->SpinBox_DZ->blockSignals(blocked);
	}
      }
    }
  }
  displayPreview();
}

//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::DeactivateActiveDialog()
{
  setEnabled( false );
  globalSelection();
  disconnect( myGeomGUI->getApp()->selectionMgr(), 0, this, 0 );
  myGeomGUI->SetActiveDialogBox( 0 );
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::ActivateThisDialog()
{
  myGeomGUI->EmitSignalDeactivateDialog();
  setEnabled( true );
  myGeomGUI->SetActiveDialogBox( this );

  connect( myGeomGUI->getApp()->selectionMgr(),
	  SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
  displayPreview();
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::ValueChangedInSpinBox( double newValue )
{
  displayPreview();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::enterEvent( QEvent* )
{
  if ( !mainFrame()->GroupConstructors->isEnabled() )
    ActivateThisDialog();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr EntityGUI_3DSketcherDlg::createOperation()
{
  return getGeomEngine()->GetICurvesOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool EntityGUI_3DSketcherDlg::isValid( QString& msg )
{
  int nbPoints = myPointsList.count()/3;
  if ( myOK )
    return nbPoints >= 2;
  else 
    return nbPoints >= 1;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool EntityGUI_3DSketcherDlg::execute( ObjectList& objects )
{
  GEOM::ListOfDouble_var aCoordsArray = new GEOM::ListOfDouble;
  if (myOK)
    aCoordsArray->length(myPointsList.size());
  else
    aCoordsArray->length(myPointsList.size()+3);

  int i = 0;
  QList<double>::const_iterator it;
  for(it = myPointsList.begin(); it != myPointsList.end(); ++it ) {
    aCoordsArray[i] = *it;
    i++;
  }

  if (!myOK) {
    double x, y, z;
    GetCurrentPoints(x, y, z);
    aCoordsArray[i] = x;
    aCoordsArray[i+1] = y;
    aCoordsArray[i+2] = z;
  }

  GEOM::GEOM_Object_var anObj =
    GEOM::GEOM_ICurvesOperations::_narrow( getOperation() )->Make3DSketcher( aCoordsArray );

  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return true;
}

//=================================================================================
// function : initSpinBox()
// purpose  :
//=================================================================================

void EntityGUI_3DSketcherDlg::initSpinBox( QDoubleSpinBox* spinBox,
					 double min,  double max,
					 double step, int decimals )
{
  spinBox->setDecimals( decimals );
  spinBox->setRange( min, max );
  spinBox->setSingleStep( step );
}

//=================================================================================
// function : SetDoubleSpinBoxStep()
// purpose  : Double spin box management
//=================================================================================
void EntityGUI_3DSketcherDlg::SetDoubleSpinBoxStep( double step )
{
  Group3Spin->SpinBox_DX->setSingleStep(step);
  Group3Spin->SpinBox_DY->setSingleStep(step);
  Group3Spin->SpinBox_DZ->setSingleStep(step);
}

//=================================================================================
// function : isSameAsPrevious()
// purpose  : Compare Last point in the list with current selected point
//=================================================================================
/*bool EntityGUI_3DSketcherDlg::isSameAsPrevious() // Not used this time
{
  double curX, curY, curZ;
  double lastX, lastY, lastZ;
  GetCurrentPoints(curX, curY, curZ);
  GetLastPoints(lastX, lastY, lastZ);
  if ( abs(lastX - curX) < Precision::Confusion() &&
       abs(lastY - curY) < Precision::Confusion() &&
       abs(lastZ - curZ) < Precision::Confusion() )
    return true;

    return false;
}*/

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void EntityGUI_3DSketcherDlg::ClickOnOk()
{
  Locker lock( myOK );

  if ( !onAccept() )
    return;

  ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool EntityGUI_3DSketcherDlg::ClickOnApply()
{
  if (myPointsList.count() > 3) {
    myPointsList.append(myPointsList[0]);
    myPointsList.append(myPointsList[1]);
    myPointsList.append(myPointsList[2]);
  }

  Locker lock( myOK );

  if ( !onAccept() )
    return false;

  ClickOnCancel();
  return true;
}

//=================================================================================
// function : GetLastPoints()
// purpose  : return last points from list
//=================================================================================
void EntityGUI_3DSketcherDlg::GetLastPoints(double& x, double& y, double& z)
{
  int count = myPointsList.count();
  x = count > 2 ? myPointsList[count-3] : 0.0;
  y = count > 2 ? myPointsList[count-2] : 0.0;
  z = count > 2 ? myPointsList[count-1] : 0.0;
}

//=================================================================================
// function : GetCurrentPoints()
// purpose  : returns current points
//=================================================================================
void EntityGUI_3DSketcherDlg::GetCurrentPoints(double& x, double& y, double& z)
{
  if ( GroupType->RadioButton1->isChecked() ) {
    x = Group3Spin->SpinBox_DX->value();
    y = Group3Spin->SpinBox_DY->value();
    z = Group3Spin->SpinBox_DZ->value();
  } else { // if (GroupType->RadioButton2->isChecked())
    GetLastPoints(x, y, z);
    x += Group3Spin->SpinBox_DX->value();
    y += Group3Spin->SpinBox_DY->value();
    z += Group3Spin->SpinBox_DZ->value();
  }
}
