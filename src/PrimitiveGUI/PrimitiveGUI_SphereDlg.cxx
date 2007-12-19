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
//  File   : PrimitiveGUI_SphereDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#include "PrimitiveGUI_SphereDlg.h"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "SalomeApp_Application.h"
#include "LightApp_SelectionMgr.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <qlabel.h>

#include "GEOMImpl_Types.hxx"

#include "utilities.h"

using namespace std;

//=================================================================================
// class    : PrimitiveGUI_SphereDlg()
// purpose  : Constructs a PrimitiveGUI_SphereDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
PrimitiveGUI_SphereDlg::PrimitiveGUI_SphereDlg(GeometryGUI* theGeometryGUI, QWidget* parent,
                                               const char* name, bool modal, WFlags fl)
  :GEOMBase_Skeleton(theGeometryGUI, parent, name, modal, WStyle_Customize |
                     WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_DLG_SPHERE_P")));
  QPixmap image1(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_DLG_SPHERE_DXYZ")));
  QPixmap image2(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_SPHERE_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_SPHERE"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  GroupPoints = new DlgRef_1Sel1Spin(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_SPHERE_CR"));
  GroupPoints->TextLabel1->setText(tr("GEOM_CENTER"));
  GroupPoints->TextLabel2->setText(tr("GEOM_RADIUS"));
  GroupPoints->PushButton1->setPixmap(image2);

  GroupDimensions = new DlgRef_1Spin(this, "GroupDimensions");
  GroupDimensions->GroupBox1->setTitle(tr("GEOM_SPHERE_RO"));
  GroupDimensions->TextLabel1->setText(tr("GEOM_RADIUS"));

  Layout1->addWidget(GroupPoints, 2, 0);
  Layout1->addWidget(GroupDimensions, 2, 0);
  /***************************************************************/

  setHelpFileName("sphere.htm");

  Init();
}


//=================================================================================
// function : ~PrimitiveGUI_SphereDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
PrimitiveGUI_SphereDlg::~PrimitiveGUI_SphereDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;
  GroupPoints->LineEdit1->setReadOnly( true );
  
  myPoint = GEOM::GEOM_Object::_nil();
  
  /* Get setting of step value from file configuration */
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue( "Geometry", "SettingsGeomStep", 100);

  /* min, max, step and decimals for spin boxes */
  GroupPoints->SpinBox_DX->RangeStepAndValidator(0.001, COORD_MAX, step, DBL_DIGITS_DISPLAY);
  GroupDimensions->SpinBox_DX->RangeStepAndValidator(0.001, COORD_MAX, step, DBL_DIGITS_DISPLAY);
  GroupPoints->SpinBox_DX->SetValue(100.0);
  GroupDimensions->SpinBox_DX->SetValue(100.0);
  
  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupDimensions->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));

  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupPoints->SpinBox_DX, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), GroupDimensions->SpinBox_DX, SLOT(SetStep(double)));

  connect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(), 
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  initName(tr("GEOM_SPHERE"));
  ConstructorsClicked(0);
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void PrimitiveGUI_SphereDlg::ConstructorsClicked(int constructorId)
{
  disconnect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(), 0, this, 0);
  
  switch (constructorId)
    {
    case 0:
      {
	localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
	GroupDimensions->hide();
	resize(0, 0);
	GroupPoints->show();

	myEditCurrentArgument = GroupPoints->LineEdit1;
	GroupPoints->LineEdit1->setText("");
	myPoint = GEOM::GEOM_Object::_nil();

	connect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(), 
		SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
	break;
      }
    case 1:
      {
	GroupPoints->hide();
	resize(0, 0);
	GroupDimensions->show();
	
	break;
      }
    }
  displayPreview();
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool PrimitiveGUI_SphereDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;
  
  initName();
  ConstructorsClicked( getConstructorId() );
  return true;
}



//=======================================================================
// function : ClickOnCancel()
// purpose  :
//=======================================================================
void PrimitiveGUI_SphereDlg::ClickOnCancel()
{
  GEOMBase_Skeleton::ClickOnCancel();
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void PrimitiveGUI_SphereDlg::SelectionIntoArgument()
{
  if ( getConstructorId() != 0 )
    return;
  
  myEditCurrentArgument->setText("");
  
  if(IObjectCount() != 1) 
    {
      myPoint = GEOM::GEOM_Object::_nil();
      return;
    }
  
  /* nbSel == 1 ! */
  Standard_Boolean testResult = Standard_False;
  GEOM::GEOM_Object_ptr aSelectedObject = GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );
  
  if (!testResult || CORBA::is_nil( aSelectedObject ))
    return;

  QString aName = GEOMBase::GetName( aSelectedObject );
  TopoDS_Shape aShape;
  if ( GEOMBase::GetShape( aSelectedObject, aShape, TopAbs_SHAPE ) && !aShape.IsNull() )
    {
      LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
      TColStd_IndexedMapOfInteger aMap;
      aSelMgr->GetIndexes( firstIObject(), aMap );
      if ( aMap.Extent() == 1 )
	{
	  GEOM::GEOM_IShapesOperations_var aShapesOp =
	    getGeomEngine()->GetIShapesOperations( getStudyId() );
	  int anIndex = aMap( 1 );
	  aName.append( ":vertex_" + QString::number( anIndex ) );
	  aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
	  aSelMgr->clearSelected();
	}
    }

  myEditCurrentArgument->setText( aName );
  myPoint = aSelectedObject;
 
  displayPreview();
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if(send == GroupPoints->LineEdit1)
    {
      myEditCurrentArgument = send;
      GEOMBase_Skeleton::LineEditReturnPressed();
    }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  
  if(send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
    localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
    SelectionIntoArgument();
  }
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(((SalomeApp_Application*)(SUIT_Session::session()->activeApplication()))->selectionMgr(), 
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  
  ConstructorsClicked( getConstructorId() );
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  : public slot to deactivate if active
//=================================================================================
void PrimitiveGUI_SphereDlg::DeactivateActiveDialog()
{
  GEOMBase_Skeleton::DeactivateActiveDialog();
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::enterEvent(QEvent* e)
{
  if ( !GroupConstructors->isEnabled() )
    ActivateThisDialog();
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void PrimitiveGUI_SphereDlg::ValueChangedInSpinBox()
{
  displayPreview();
}


//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr PrimitiveGUI_SphereDlg::createOperation()
{
  return getGeomEngine()->GetI3DPrimOperations( getStudyId() );
}


//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool PrimitiveGUI_SphereDlg::isValid( QString& msg  )
{
  return getConstructorId() == 0 ? !myPoint->_is_nil() : true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool  PrimitiveGUI_SphereDlg::execute( ObjectList& objects )
{
  bool res = false;
  
  GEOM::GEOM_Object_var anObj;

  switch ( getConstructorId() ) 
  {
  case 0 :
    {
      if ( !CORBA::is_nil( myPoint ) ) {
	anObj = GEOM::GEOM_I3DPrimOperations::_narrow( getOperation() )->MakeSpherePntR( myPoint, getRadius() );
	res = true;
      }
      break;
    }
  case 1 :
    {
      anObj = GEOM::GEOM_I3DPrimOperations::_narrow( getOperation() )->MakeSphereR( getRadius() );
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
void PrimitiveGUI_SphereDlg::closeEvent( QCloseEvent* e )
{
  //myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::closeEvent( e );
}



//=================================================================================
// function : getRadius()
// purpose  :
//=================================================================================
double PrimitiveGUI_SphereDlg::getRadius() const
{
  int aConstructorId = getConstructorId();
  if (aConstructorId == 0)
    return GroupPoints->SpinBox_DX->GetValue();
  else if (aConstructorId == 1)
    return GroupDimensions->SpinBox_DX->GetValue();
  return 0;
}
