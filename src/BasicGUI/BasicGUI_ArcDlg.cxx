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
//  File   : BasicGUI_ArcDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

#include "BasicGUI_ArcDlg.h"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "SalomeApp_Application.h"
#include "LightApp_SelectionMgr.h"

#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>

#include <qlabel.h>
#include <qcheckbox.h>

#include "GEOMImpl_Types.hxx"

#include "utilities.h"

//=================================================================================
// class    : BasicGUI_ArcDlg()
// purpose  : Constructs a BasicGUI_ArcDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BasicGUI_ArcDlg::BasicGUI_ArcDlg(GeometryGUI* theGeometryGUI, QWidget* parent,
                                 const char* name, bool modal, WFlags fl)
  : GEOMBase_Skeleton(theGeometryGUI, parent, name, modal, WStyle_Customize |
                      WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap image0(aResMgr->loadPixmap("GEOM",tr("ICON_DLG_ARC")));
  QPixmap image1(aResMgr->loadPixmap("GEOM",tr("ICON_DLG_ARC_CENTER")));
  QPixmap image2(aResMgr->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_ARC_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_ARC"));
  RadioButton1->setPixmap(image0);
  RadioButton2->setPixmap(image1);
  RadioButton3->close(TRUE);

  Group3Pnts = new DlgRef_3Sel_QTD(this, "Group3Pnts");
  Group3Pnts->GroupBox1->setTitle(tr("GEOM_POINTS"));
  Group3Pnts->TextLabel1->setText(tr("GEOM_POINT_I").arg("1"));
  Group3Pnts->TextLabel2->setText(tr("GEOM_POINT_I").arg("2"));
  Group3Pnts->TextLabel3->setText(tr("GEOM_POINT_I").arg("3"));

  Group3Pnts->LineEdit1->setReadOnly( true );
  Group3Pnts->LineEdit2->setReadOnly( true );
  Group3Pnts->LineEdit3->setReadOnly( true );

  Group3Pnts->PushButton1->setPixmap(image2);
  Group3Pnts->PushButton2->setPixmap(image2);
  Group3Pnts->PushButton3->setPixmap(image2);


  Group3Pnts2 = new DlgRef_3Sel1Check_QTD(this, "Group3Pnts2");
  Group3Pnts2->GroupBox1->setTitle(tr("GEOM_POINTS"));
  Group3Pnts2->TextLabel1->setText(tr("GEOM_CENTER_POINT"));
  Group3Pnts2->TextLabel2->setText(tr("GEOM_POINT_I").arg("Start"));
  Group3Pnts2->TextLabel3->setText(tr("GEOM_POINT_I").arg("End"));

  Group3Pnts2->LineEdit1->setReadOnly( true );
  Group3Pnts2->LineEdit2->setReadOnly( true );
  Group3Pnts2->LineEdit3->setReadOnly( true );

  Group3Pnts2->PushButton1->setPixmap(image2);
  Group3Pnts2->PushButton2->setPixmap(image2);
  Group3Pnts2->PushButton3->setPixmap(image2);

  Group3Pnts2->CheckButton1->setText(tr("GEOM_REVERSE"));

  Layout1->addWidget( Group3Pnts, 2, 0 );
  Layout1->addWidget( Group3Pnts2, 2, 0 );
  /***************************************************************/

  setHelpFileName("arc.htm");

  Init();
}


//=================================================================================
// function : ~BasicGUI_ArcDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BasicGUI_ArcDlg::~BasicGUI_ArcDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = Group3Pnts->LineEdit1;
  myPoint1 = myPoint2 = myPoint3 = GEOM::GEOM_Object::_nil();
  Group3Pnts2->CheckButton1->setChecked(FALSE);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(Group3Pnts->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group3Pnts->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group3Pnts->PushButton3, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(Group3Pnts->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group3Pnts->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group3Pnts->LineEdit3, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group3Pnts2->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group3Pnts2->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group3Pnts2->PushButton3, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(Group3Pnts2->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group3Pnts2->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group3Pnts2->LineEdit3, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group3Pnts2->CheckButton1, SIGNAL(stateChanged(int)), this, SLOT(ReverseSense(int)));

  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  initName( tr( "GEOM_ARC" ));
  ConstructorsClicked( 0 );
}


//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void BasicGUI_ArcDlg::ConstructorsClicked (int constructorId)
{
  disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);

  switch (constructorId)
  {
  case 0:
    {
      globalSelection(); // close local contexts, if any
      localSelection(GEOM::GEOM_Object::_nil(), TopAbs_VERTEX); //Select Vertex on All Shapes

      Group3Pnts->show();
      resize(0, 0);
      Group3Pnts2->hide();

      Group3Pnts->LineEdit1->setText(Group3Pnts2->LineEdit1->text());
      Group3Pnts->LineEdit2->setText(Group3Pnts2->LineEdit2->text());
      Group3Pnts->LineEdit3->setText(Group3Pnts2->LineEdit3->text());

      myEditCurrentArgument = Group3Pnts->LineEdit1;

      break;
    }
  case 1:
    {
      globalSelection(); // close local contexts, if any
      localSelection(GEOM::GEOM_Object::_nil(), TopAbs_VERTEX); //Select Vertex on All Shapes

      Group3Pnts->hide();
      resize(0, 0);
      Group3Pnts2->show();

      Group3Pnts2->LineEdit1->setText(Group3Pnts->LineEdit1->text());
      Group3Pnts2->LineEdit2->setText(Group3Pnts->LineEdit2->text());
      Group3Pnts2->LineEdit3->setText(Group3Pnts->LineEdit3->text());

      myEditCurrentArgument = Group3Pnts2->LineEdit1;

      break;
    }
  }

  myEditCurrentArgument->setFocus();
  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  if (CORBA::is_nil(myPoint1))
    SelectionIntoArgument();

  displayPreview();
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BasicGUI_ArcDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;

  initName();
  return true;
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void BasicGUI_ArcDlg::SelectionIntoArgument()
{
  if ((getConstructorId() != 0) && (getConstructorId() != 1))
    return;

  myEditCurrentArgument->setText("");

  if (IObjectCount() != 1)
  {
    switch (getConstructorId())
    {
    case 0:
      {
        if      ( myEditCurrentArgument == Group3Pnts->LineEdit1 )   myPoint1 = GEOM::GEOM_Object::_nil();
        else if ( myEditCurrentArgument == Group3Pnts->LineEdit2 )   myPoint2 = GEOM::GEOM_Object::_nil();
        else if ( myEditCurrentArgument == Group3Pnts->LineEdit3 )   myPoint3 = GEOM::GEOM_Object::_nil();
        return;
        break;
      }
    case 1:
      {
        if      ( myEditCurrentArgument == Group3Pnts2->LineEdit1 )   myPoint1 = GEOM::GEOM_Object::_nil();
        else if ( myEditCurrentArgument == Group3Pnts2->LineEdit2 )   myPoint2 = GEOM::GEOM_Object::_nil();
        else if ( myEditCurrentArgument == Group3Pnts2->LineEdit3 )   myPoint3 = GEOM::GEOM_Object::_nil();
        return;
        break;
      }
    }
  }

  // nbSel == 1
  Standard_Boolean aRes = Standard_False;
  GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject(firstIObject(), aRes);
  if (!CORBA::is_nil(aSelectedObject) && aRes)
  {
    QString aName = GEOMBase::GetName(aSelectedObject);

    // Get Selected object if selected subshape
    TopoDS_Shape aShape;
    if (GEOMBase::GetShape(aSelectedObject, aShape, TopAbs_SHAPE) && !aShape.IsNull())
    {
      LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
      TColStd_IndexedMapOfInteger aMap;
      aSelMgr->GetIndexes(firstIObject(), aMap);
      if (aMap.Extent() == 1) // Local Selection
      {
        int anIndex = aMap(1);
        aName += QString(":vertex_%1").arg(anIndex);

	//Find SubShape Object in Father
	GEOM::GEOM_Object_var aFindedObject = GEOMBase_Helper::findObjectInFather(aSelectedObject, aName);

	if ( aFindedObject == GEOM::GEOM_Object::_nil() ) { // Object not found in study
	  GEOM::GEOM_IShapesOperations_var aShapesOp = getGeomEngine()->GetIShapesOperations(getStudyId());
	  aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
	}
	else
	  aSelectedObject = aFindedObject; // get Object from study
      }
      else // Global Selection
      {
        if (aShape.ShapeType() != TopAbs_VERTEX) {
          aSelectedObject = GEOM::GEOM_Object::_nil();
          aName = "";
        }
      }
    }

    myEditCurrentArgument->setText(aName);

    switch (getConstructorId())
    {
      case 0:
      {
        if      ( myEditCurrentArgument == Group3Pnts->LineEdit1 )   myPoint1 = aSelectedObject;
        else if ( myEditCurrentArgument == Group3Pnts->LineEdit2 )   myPoint2 = aSelectedObject;
        else if ( myEditCurrentArgument == Group3Pnts->LineEdit3 )   myPoint3 = aSelectedObject;
        break;
      }
      case 1:
      {
        if      ( myEditCurrentArgument == Group3Pnts2->LineEdit1 )   myPoint1 = aSelectedObject;
        else if ( myEditCurrentArgument == Group3Pnts2->LineEdit2 )   myPoint2 = aSelectedObject;
        else if ( myEditCurrentArgument == Group3Pnts2->LineEdit3 )   myPoint3 = aSelectedObject;
        break;
      }
    }
  }

  displayPreview();
}

//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if (send == Group3Pnts->LineEdit1  || send == Group3Pnts->LineEdit2  || send == Group3Pnts->LineEdit3 ||
      send == Group3Pnts2->LineEdit1 || send == Group3Pnts2->LineEdit2 || send == Group3Pnts2->LineEdit3)
  {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  switch (getConstructorId())
  {
    case 0:
    {
      if      ( send == Group3Pnts->PushButton1 )   myEditCurrentArgument = Group3Pnts->LineEdit1;
      else if ( send == Group3Pnts->PushButton2 )   myEditCurrentArgument = Group3Pnts->LineEdit2;
      else if ( send == Group3Pnts->PushButton3 )   myEditCurrentArgument = Group3Pnts->LineEdit3;
      break;
    }
    case 1:
    {
      if      ( send == Group3Pnts2->PushButton1 )   myEditCurrentArgument = Group3Pnts2->LineEdit1;
      else if ( send == Group3Pnts2->PushButton2 )   myEditCurrentArgument = Group3Pnts2->LineEdit2;
      else if ( send == Group3Pnts2->PushButton3 )   myEditCurrentArgument = Group3Pnts2->LineEdit3;
      break;
    }
  }
  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();

  connect(myGeomGUI->getApp()->selectionMgr(),
          SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  ConstructorsClicked( getConstructorId() );
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void BasicGUI_ArcDlg::enterEvent(QEvent* e)
{
  if (!GroupConstructors->isEnabled())
    ActivateThisDialog();
}


//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BasicGUI_ArcDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetICurvesOperations( getStudyId() );
}

//=================================================================================
// function : isEqual
// purpose  : it may also be needed to check for min distance between gp_Pnt-s...
//=================================================================================
static bool isEqual( const GEOM::GEOM_Object_var& thePnt1, const GEOM::GEOM_Object_var& thePnt2 )
{
  return thePnt1->_is_equivalent( thePnt2 );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BasicGUI_ArcDlg::isValid( QString& msg )
{
  return !myPoint1->_is_nil() && !myPoint2->_is_nil() && !myPoint3->_is_nil() &&
    !isEqual( myPoint1, myPoint2 ) && !isEqual( myPoint1, myPoint3 ) && !isEqual( myPoint2, myPoint3 );
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BasicGUI_ArcDlg::execute( ObjectList& objects )
{
  bool res = false;
  GEOM::GEOM_Object_var anObj;

  switch (getConstructorId())
  {
  case 0:
    {
      if ( !CORBA::is_nil(myPoint1) && !CORBA::is_nil(myPoint2) &&!CORBA::is_nil(myPoint3) )
      {
        anObj = GEOM::GEOM_ICurvesOperations::_narrow(getOperation())->
          MakeArc(myPoint1, myPoint2, myPoint3);
        res = true;
      }
      break;
    }
  case 1:
    {
      bool Sense;
      (Group3Pnts2->CheckButton1->isChecked()) ? Sense = true : Sense = false;
      if ( !CORBA::is_nil(myPoint1) && !CORBA::is_nil(myPoint2) &&!CORBA::is_nil(myPoint3) )
      {
        anObj = GEOM::GEOM_ICurvesOperations::_narrow(getOperation())->
          MakeArcCenter(myPoint1, myPoint2, myPoint3, Sense);
        res = true;
      }
      break;
    }
  }
  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return res;
}

//=================================================================================
// function : ReverseSense()
// purpose  : Orientation of the arc
//=================================================================================
void BasicGUI_ArcDlg::ReverseSense(int sense)
{
  displayPreview();
}

//=================================================================================
// function : addSubshapeToStudy
// purpose  : virtual method to add new SubObjects if local selection
//=================================================================================
void BasicGUI_ArcDlg::addSubshapesToStudy()
{
  QMap<QString, GEOM::GEOM_Object_var> objMap;

switch (getConstructorId())
  {
  case 0:
    objMap[Group3Pnts->LineEdit1->text()] = myPoint1;
    objMap[Group3Pnts->LineEdit2->text()] = myPoint2;
    objMap[Group3Pnts->LineEdit3->text()] = myPoint3;
    break;
  case 1:
    objMap[Group3Pnts2->LineEdit1->text()] = myPoint1;
    objMap[Group3Pnts2->LineEdit2->text()] = myPoint2;
    objMap[Group3Pnts2->LineEdit3->text()] = myPoint3;
    break;
  }
 addSubshapesToFather( objMap );
}
