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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : GenerationGUI_PrismDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#include "GenerationGUI_PrismDlg.h"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "SalomeApp_Application.h"
#include "LightApp_SelectionMgr.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <qlabel.h>
#include <qcheckbox.h>

#include "GEOMImpl_Types.hxx"

#include "utilities.h"

//=================================================================================
// class    : GenerationGUI_PrismDlg()
// purpose  : Constructs a GenerationGUI_PrismDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GenerationGUI_PrismDlg::GenerationGUI_PrismDlg(GeometryGUI* theGeometryGUI, QWidget* parent,
                                               const char* name, bool modal, WFlags fl)
  : GEOMBase_Skeleton(theGeometryGUI, parent, name, modal, WStyle_Customize |
                      WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap image0(aResMgr->loadPixmap("GEOM", tr("ICON_DLG_PRISM")));
  QPixmap image1(aResMgr->loadPixmap("GEOM", tr("ICON_SELECT")));
  QPixmap image2(aResMgr->loadPixmap("GEOM", tr("ICON_DLG_PRISM_2P")));

  setCaption(tr("GEOM_EXTRUSION_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_EXTRUSION"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image2);
  RadioButton3->close(TRUE);

  RadioButton1->setChecked(true);
  myBothway = myBothway2 = false;

  GroupPoints = new DlgRef_2Sel1Spin2Check(this, "GroupPoints");
  GroupPoints->GroupBox1->setTitle(tr("GEOM_EXTRUSION_BSV"));
  GroupPoints->TextLabel1->setText(tr("GEOM_BASE"));
  GroupPoints->TextLabel2->setText(tr("GEOM_VECTOR"));
  GroupPoints->TextLabel3->setText(tr("GEOM_HEIGHT"));
  GroupPoints->PushButton1->setPixmap(image1);
  GroupPoints->PushButton2->setPixmap(image1);
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );
  GroupPoints->CheckButton1->setText(tr("GEOM_BOTHWAY"));
  GroupPoints->CheckButton1->setChecked(myBothway);
  GroupPoints->CheckButton2->setText(tr("GEOM_REVERSE"));

  GroupPoints2 = new DlgRef_3Sel1Check_QTD(this, "GroupPoints2");
  GroupPoints2->GroupBox1->setTitle(tr("GEOM_EXTRUSION_BSV_2P"));
  GroupPoints2->TextLabel1->setText(tr("GEOM_BASE"));
  GroupPoints2->TextLabel2->setText(tr("GEOM_POINT_I").arg("1"));
  GroupPoints2->TextLabel3->setText(tr("GEOM_POINT_I").arg("2"));
  GroupPoints2->PushButton1->setPixmap(image1);
  GroupPoints2->PushButton2->setPixmap(image1);
  GroupPoints2->PushButton3->setPixmap(image1);
  GroupPoints2->CheckButton1->setText(tr("GEOM_BOTHWAY"));
  GroupPoints2->CheckButton1->setChecked(myBothway2);

  Layout1->addWidget(GroupPoints, 2, 0);
  Layout1->addWidget(GroupPoints2, 2, 0);
  /***************************************************************/

  setHelpFileName("extrusion.htm");

  Init();
}


//=================================================================================
// function : ~GenerationGUI_PrismDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GenerationGUI_PrismDlg::~GenerationGUI_PrismDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->LineEdit2->setReadOnly( true );

  GroupPoints2->LineEdit1->setReadOnly( true );
  GroupPoints2->LineEdit2->setReadOnly( true );
  GroupPoints2->LineEdit3->setReadOnly( true );

  myPoint1 = myPoint2 = myBase = myVec = GEOM::GEOM_Object::_nil();
  myOkBase = myOkVec = myOkPnt1 = myOkPnt2 = false;

  /* Get setting of step value from file configuration */
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue( "Geometry", "SettingsGeomStep", 100);

  /* min, max, step and decimals for spin boxes & initial values */
  GroupPoints->SpinBox_DX->RangeStepAndValidator(COORD_MIN, COORD_MAX, step, DBL_DIGITS_DISPLAY);
  GroupPoints->SpinBox_DX->SetValue(100.0);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPoints->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)),
          GroupPoints->SpinBox_DX, SLOT(SetStep(double)));

  connect(GroupPoints->CheckButton1, SIGNAL(toggled(bool)), this, SLOT(onBothway()));
  connect(GroupPoints->CheckButton2, SIGNAL(toggled(bool)), this, SLOT(onReverse()));

  connect(GroupPoints2->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints2->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints2->PushButton3, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints2->CheckButton1, SIGNAL(toggled(bool)), this, SLOT(onBothway2()));

  connect(GroupPoints2->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints2->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints2->LineEdit3, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));


  connect(myGeomGUI->getApp()->selectionMgr(),
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  initName(tr("GEOM_EXTRUSION"));
  ConstructorsClicked(0);
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void GenerationGUI_PrismDlg::ConstructorsClicked(int constructorId)
{
  erasePreview();
  disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);

  switch (constructorId)
  {
  case 0:
    {
      globalSelection( GEOM_ALLSHAPES );

      GroupPoints2->hide();
      resize(0, 0);
      GroupPoints->show();

      myEditCurrentArgument = GroupPoints->LineEdit1;
      GroupPoints->LineEdit1->setText(GroupPoints2->LineEdit1->text()); // keep base
      GroupPoints->LineEdit2->setText("");
      myVec = GEOM::GEOM_Object::_nil();
      myOkVec = false;

      break;
    }
  case 1:
    {
      globalSelection( GEOM_ALLSHAPES );

      GroupPoints->hide();
      resize(0, 0);
      GroupPoints2->show();

      myEditCurrentArgument = GroupPoints2->LineEdit1;
      GroupPoints2->LineEdit1->setText(GroupPoints->LineEdit1->text()); // keep base
      GroupPoints2->LineEdit2->setText("");
      GroupPoints2->LineEdit3->setText("");
      myPoint1 = myPoint2 = GEOM::GEOM_Object::_nil();
      myOkPnt1 = myOkPnt2 = false;

      break;
    }
  }

  myEditCurrentArgument->setFocus();
  connect(myGeomGUI->getApp()->selectionMgr(),
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  if (!myOkBase)
    SelectionIntoArgument();
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::ClickOnApply()
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
void GenerationGUI_PrismDlg::SelectionIntoArgument()
{
  erasePreview();
  myEditCurrentArgument->setText("");

  if (getConstructorId() == 0)
  {
    if (IObjectCount() != 1) {
      if (myEditCurrentArgument == GroupPoints->LineEdit1)
        myOkBase = false;
      else if (myEditCurrentArgument == GroupPoints->LineEdit2)
        myOkVec = false;
      return;
    }

    // nbSel == 1
    Standard_Boolean testResult = Standard_False;
    GEOM::GEOM_Object_ptr aSelectedObject =
      GEOMBase::ConvertIOinGEOMObject( firstIObject(), testResult );

    if (!testResult)
      return;

    bool myOk = true;
    TopoDS_Shape aShape;
    QString aName = GEOMBase::GetName( aSelectedObject );    
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
	    aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
	    aName.append( ":edge_" + QString::number( anIndex ) );
	    aSelMgr->clearSelected();
	  }
	else {
	  if (aShape.ShapeType() != TopAbs_EDGE && myEditCurrentArgument == GroupPoints->LineEdit2) {
	    aSelectedObject = GEOM::GEOM_Object::_nil();
	    aName = "";
	    myOk = false;
	  }
	}
      }
 
    if (myEditCurrentArgument == GroupPoints->LineEdit1) {
      myBase = aSelectedObject;
      myOkBase = true;
    }
    else if (myEditCurrentArgument == GroupPoints->LineEdit2 && myOk) {
      myOkVec = true;      
      myVec = aSelectedObject;
    }
    myEditCurrentArgument->setText( aName );
  }
  else // getConstructorId()==1 - extrusion using 2 points
  {
    if (IObjectCount() != 1) {
      if (myEditCurrentArgument == GroupPoints2->LineEdit1)
        myOkBase = false;
      else if (myEditCurrentArgument == GroupPoints2->LineEdit2) {
	myPoint1 = GEOM::GEOM_Object::_nil();
        myOkPnt1 = false;
      }
      else if (myEditCurrentArgument == GroupPoints2->LineEdit3) {
	myPoint2 = GEOM::GEOM_Object::_nil();
        myOkPnt2 = false;
      }
      return;
    }

    // nbSel == 1
    Standard_Boolean testResult = Standard_False;
    GEOM::GEOM_Object_var aSelectedObject =
      GEOMBase::ConvertIOinGEOMObject(firstIObject(), testResult );

    if (!testResult || CORBA::is_nil( aSelectedObject ))
      return;

    QString aName = GEOMBase::GetName( aSelectedObject );
    TopoDS_Shape aShape;
    bool myOk = true;
    if ( GEOMBase::GetShape( aSelectedObject, aShape, TopAbs_SHAPE ) && !aShape.IsNull() )
    {
	LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
	TColStd_IndexedMapOfInteger aMap;
	aSelMgr->GetIndexes( firstIObject(), aMap );
	if (aMap.Extent() == 1)
        {
          GEOM::GEOM_IShapesOperations_var aShapesOp = getGeomEngine()->GetIShapesOperations(getStudyId());
          int anIndex = aMap(1);
          aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
          aName.append(":vertex_" + QString::number(anIndex));
          aSelMgr->clearSelected();
        }
	else
        {
	  if ((aShape.ShapeType() != TopAbs_VERTEX && myEditCurrentArgument == GroupPoints2->LineEdit2) ||
              (aShape.ShapeType() != TopAbs_VERTEX && myEditCurrentArgument == GroupPoints2->LineEdit3))
          {
	    aSelectedObject = GEOM::GEOM_Object::_nil();
	    aName = "";
	    myOk = false;
	  }
	}
    }

    myEditCurrentArgument->setText(aName);

    if (myEditCurrentArgument == GroupPoints2->LineEdit1) {
      myBase = aSelectedObject;
      myOkBase = true;
    }
    else if (myEditCurrentArgument == GroupPoints2->LineEdit2 && myOk) {
      myOkPnt1 = true;
      myPoint1 = aSelectedObject;
    }
    else if (myEditCurrentArgument == GroupPoints2->LineEdit3 && myOk) {
      myOkPnt2 = true;
      myPoint2 = aSelectedObject;
    }
  }

  displayPreview();
}

//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if (send == GroupPoints->LineEdit1 ||
      send == GroupPoints->LineEdit2 ||
      send == GroupPoints2->LineEdit1 ||
      send == GroupPoints2->LineEdit2 ||
      send == GroupPoints2->LineEdit3 )
  {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  globalSelection( GEOM_ALLSHAPES );

  if (send == GroupPoints->PushButton1) {
    GroupPoints->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit1;
  }
  else if (send == GroupPoints->PushButton2) {
    GroupPoints->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints->LineEdit2;
    localSelection( GEOM::GEOM_Object::_nil(), TopAbs_EDGE );
  }
  else if (send == GroupPoints2->PushButton1) {
    GroupPoints2->LineEdit1->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit1;
  }
  else if (send == GroupPoints2->PushButton2) {
    GroupPoints2->LineEdit2->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit2;
    localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
  }
  else if (send == GroupPoints2->PushButton3) {
    GroupPoints2->LineEdit3->setFocus();
    myEditCurrentArgument = GroupPoints2->LineEdit3;
    localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
  }

  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();

  connect(myGeomGUI->getApp()->selectionMgr(),
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  ConstructorsClicked( getConstructorId() );
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void GenerationGUI_PrismDlg::enterEvent(QEvent * e)
{
  if (!GroupConstructors->isEnabled())
    ActivateThisDialog();
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::ValueChangedInSpinBox()
{
  displayPreview();
}


//=================================================================================
// function : getHeight()
// purpose  :
//=================================================================================
double GenerationGUI_PrismDlg::getHeight() const
{
    return GroupPoints->SpinBox_DX->GetValue();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr GenerationGUI_PrismDlg::createOperation()
{
  return getGeomEngine()->GetI3DPrimOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::isValid( QString& )
{
  if (getConstructorId() == 0)
    return (myOkBase && myOkVec);     // by vector and height
  else
    return (myOkBase && myOkPnt1 && myOkPnt2);   // by two points
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool GenerationGUI_PrismDlg::execute( ObjectList& objects )
{
  GEOM::GEOM_Object_var anObj;

  switch ( getConstructorId() )
  {
  case 0:
    {
      if (!myBothway)
	anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
	  MakePrismVecH(myBase, myVec, getHeight());
      else
      	anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
	MakePrismVecH2Ways(myBase, myVec, getHeight() );
      break;
    }
  case 1:
    {
      if (!myBothway2)
	anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
        MakePrismTwoPnt(myBase, myPoint1, myPoint2);
      else
	anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
        MakePrismTwoPnt2Ways(myBase, myPoint1, myPoint2);
      break;
    }
  }
  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return true;
}


//=================================================================================
// function :  onReverse()
// purpose  :
//=================================================================================
void GenerationGUI_PrismDlg::onReverse()
{
  double anOldValue = GroupPoints->SpinBox_DX->GetValue();
  GroupPoints->SpinBox_DX->SetValue( -anOldValue );
}

//=================================================================================
// function :  onBothway()
// purpose  :  
//=================================================================================
void GenerationGUI_PrismDlg::onBothway()
{
  bool anOldValue = myBothway;
  myBothway = !anOldValue;
  GroupPoints->CheckButton2->setEnabled(!myBothway);
  displayPreview();
}

void GenerationGUI_PrismDlg::onBothway2()
{
  bool anOldValue = myBothway2;
  myBothway2 = !anOldValue;
  displayPreview();
}
