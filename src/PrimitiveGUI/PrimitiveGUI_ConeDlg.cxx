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
// File   : PrimitiveGUI_ConeDlg.cxx
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//

#include "PrimitiveGUI_ConeDlg.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

// OCCT Includes
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <GEOMImpl_Types.hxx>

//=================================================================================
// class    : PrimitiveGUI_ConeDlg()
// purpose  : Constructs a PrimitiveGUI_ConeDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
PrimitiveGUI_ConeDlg::PrimitiveGUI_ConeDlg (GeometryGUI* theGeometryGUI, QWidget* parent,
                                            bool modal, Qt::WindowFlags fl)
  : GEOMBase_Skeleton(theGeometryGUI, parent, modal, fl),
    myInitial(true)
{
  QPixmap image0 (SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_DLG_CONE_PV")));
  QPixmap image1 (SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_DLG_CONE_DXYZ")));
  QPixmap image2 (SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_SELECT")));

  setWindowTitle(tr("GEOM_CONE_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_CONE"));
  mainFrame()->RadioButton1->setIcon(image0);
  mainFrame()->RadioButton2->setIcon(image1);
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();

  GroupPoints = new DlgRef_2Sel3Spin(centralWidget());
  GroupPoints->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  GroupPoints->TextLabel1->setText(tr("GEOM_BASE_POINT"));
  GroupPoints->TextLabel2->setText(tr("GEOM_VECTOR"));
  GroupPoints->TextLabel3->setText(tr("GEOM_RADIUS_I").arg(1));
  GroupPoints->TextLabel4->setText(tr("GEOM_RADIUS_I").arg(2));
  GroupPoints->TextLabel5->setText(tr("GEOM_HEIGHT"));
  GroupPoints->PushButton1->setIcon(image2);
  GroupPoints->PushButton2->setIcon(image2);

  GroupDimensions = new DlgRef_3Spin(centralWidget());
  GroupDimensions->GroupBox1->setTitle(tr("GEOM_BOX_OBJ"));
  GroupDimensions->TextLabel1->setText(tr("GEOM_RADIUS_I").arg(1));
  GroupDimensions->TextLabel2->setText(tr("GEOM_RADIUS_I").arg(2));
  GroupDimensions->TextLabel3->setText(tr("GEOM_HEIGHT"));

  QVBoxLayout* layout = new QVBoxLayout(centralWidget());
  layout->setMargin(0); layout->setSpacing(6);
  layout->addWidget(GroupPoints);
  layout->addWidget(GroupDimensions);
  /***************************************************************/

  setHelpFileName("create_cone_page.html");

  Init();
}

//=================================================================================
// function : ~PrimitiveGUI_ConeDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
PrimitiveGUI_ConeDlg::~PrimitiveGUI_ConeDlg()
{
  // no need to delete child widgets, Qt does it all for us
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::Init()
{
  // Get setting of step value from file configuration
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue("Geometry", "SettingsGeomStep", 100);

  // min, max, step and decimals for spin boxes & initial values
  initSpinBox(GroupPoints->SpinBox_DX, 0.000, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox(GroupPoints->SpinBox_DY, 0.000, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox(GroupPoints->SpinBox_DZ, COORD_MIN, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox(GroupDimensions->SpinBox_DX, 0.000, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox(GroupDimensions->SpinBox_DY, 0.000, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY
  initSpinBox(GroupDimensions->SpinBox_DZ, COORD_MIN, COORD_MAX, step, 3); // VSR: TODO: DBL_DIGITS_DISPLAY

  // init variables
  GroupPoints->LineEdit1->setReadOnly(true);
  GroupPoints->LineEdit2->setReadOnly(true);

  GroupPoints->LineEdit1->setText("");
  GroupPoints->LineEdit2->setText("");
  myPoint = myDir = GEOM::GEOM_Object::_nil();

  double aRadius1(100.0), aRadius2(0.0), aHeight(300.0);
  GroupPoints->SpinBox_DX->setValue(aRadius1);
  GroupPoints->SpinBox_DY->setValue(aRadius2);
  GroupPoints->SpinBox_DZ->setValue(aHeight);
  GroupDimensions->SpinBox_DX->setValue(aRadius1);
  GroupDimensions->SpinBox_DY->setValue(aRadius2);
  GroupDimensions->SpinBox_DZ->setValue(aHeight);

  // signals and slots connections
  connect(buttonOk(),    SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(this,          SIGNAL(constructorsClicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(GroupPoints->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(GroupPoints->SpinBox_DX,     SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupPoints->SpinBox_DY,     SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupPoints->SpinBox_DZ,     SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupDimensions->SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupDimensions->SpinBox_DY, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));
  connect(GroupDimensions->SpinBox_DZ, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox()));

  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), this, SLOT(SetDoubleSpinBoxStep(double)));

  initName(tr("GEOM_CONE"));

  setConstructorId(1); // simplest constructor
  ConstructorsClicked(1);
}

//=================================================================================
// function : SetDoubleSpinBoxStep()
// purpose  : Double spin box management
//=================================================================================
void PrimitiveGUI_ConeDlg::SetDoubleSpinBoxStep (double step)
{
  GroupPoints->SpinBox_DX->setSingleStep(step);
  GroupPoints->SpinBox_DY->setSingleStep(step);
  GroupPoints->SpinBox_DZ->setSingleStep(step);
  GroupDimensions->SpinBox_DX->setSingleStep(step);
  GroupDimensions->SpinBox_DY->setSingleStep(step);
  GroupDimensions->SpinBox_DZ->setSingleStep(step);
}

//=================================================================================
// function : ConstructorsClicked()
// purpose  : Radio button management
//=================================================================================
void PrimitiveGUI_ConeDlg::ConstructorsClicked (int constructorId)
{
  switch (constructorId) {
  case 0:
    {
      GroupDimensions->hide();
      GroupPoints->show();

      GroupPoints->PushButton1->click();
      break;
    }
  case 1:
    {
      GroupPoints->hide();
      GroupDimensions->show();
      disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
      globalSelection(); // close local contexts, if any

      displayPreview();
      break;
    }
  }

  qApp->processEvents();
  updateGeometry();
  resize(minimumSize());

  if (myInitial) {
    // on dialog initialization we init the first field with a selected object (if any)
    SelectionIntoArgument();
    myInitial = false;
  }
  else {
    displayPreview();
  }
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::ClickOnOk()
{
  if (ClickOnApply())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool PrimitiveGUI_ConeDlg::ClickOnApply()
{
  if (!onAccept())
    return false;

  initName();
  // activate selection and connect selection manager
  ConstructorsClicked(getConstructorId());
  return true;
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection is changed or on dialog initialization or activation
//=================================================================================
void PrimitiveGUI_ConeDlg::SelectionIntoArgument()
{
  if (getConstructorId() != 0)
    return;

  erasePreview();
  myEditCurrentArgument->setText("");

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);

  if (aSelList.Extent() != 1) {
    if      (myEditCurrentArgument == GroupPoints->LineEdit1) myPoint = GEOM::GEOM_Object::_nil();
    else if (myEditCurrentArgument == GroupPoints->LineEdit2) myDir   = GEOM::GEOM_Object::_nil();
    return;
  }

  // nbSel == 1
  Standard_Boolean testResult = Standard_False;
  GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject(aSelList.First(), testResult);

  if (!testResult || CORBA::is_nil(aSelectedObject))
    return;

  QString aName = GEOMBase::GetName(aSelectedObject);

  // Get Selected object if selected subshape
  TopoDS_Shape aShape;
  if (GEOMBase::GetShape(aSelectedObject, aShape, TopAbs_SHAPE) && !aShape.IsNull())
  {
    TopAbs_ShapeEnum aNeedType = TopAbs_VERTEX;
    if (myEditCurrentArgument == GroupPoints->LineEdit2)
      aNeedType = TopAbs_EDGE;

    TColStd_IndexedMapOfInteger aMap;
    aSelMgr->GetIndexes(aSelList.First(), aMap);
    if (aMap.Extent() == 1) // Local Selection
    {
      int anIndex = aMap(1);
      if (aNeedType == TopAbs_EDGE)
        aName.append(":edge_" + QString::number(anIndex));
      else
        aName.append(":vertex_" + QString::number(anIndex));

      //Find SubShape Object in Father
      GEOM::GEOM_Object_var aFindedObject = GEOMBase_Helper::findObjectInFather(aSelectedObject, aName);

      if (aFindedObject == GEOM::GEOM_Object::_nil()) { // Object not found in study
        GEOM::GEOM_IShapesOperations_var aShapesOp =
          getGeomEngine()->GetIShapesOperations(getStudyId());
        aSelectedObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
      }
      else {
        aSelectedObject = aFindedObject; // get Object from study
      }
    }
    else // Global Selection
    {
      if (aShape.ShapeType() != aNeedType) {
        aSelectedObject = GEOM::GEOM_Object::_nil();
        aName = "";
      }
    }
  }

  myEditCurrentArgument->setText(aName);

  // clear selection
  disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
  myGeomGUI->getApp()->selectionMgr()->clearSelected();
  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  if (myEditCurrentArgument == GroupPoints->LineEdit1) {
    myPoint = aSelectedObject;
    if (!myPoint->_is_nil() && myDir->_is_nil())
      GroupPoints->PushButton2->click();
  }
  else if (myEditCurrentArgument == GroupPoints->LineEdit2) {
    myDir = aSelectedObject;
    if (!myDir->_is_nil() && myPoint->_is_nil())
      GroupPoints->PushButton1->click();
  }

  displayPreview();
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
  if (send == GroupPoints->PushButton1) {
    myEditCurrentArgument = GroupPoints->LineEdit1;

    GroupPoints->PushButton2->setDown(false);
    GroupPoints->LineEdit2->setEnabled(false);

    globalSelection(GEOM_POINT); // to break previous local selection
    localSelection(GEOM::GEOM_Object::_nil(), TopAbs_VERTEX);
  }
  else if (send == GroupPoints->PushButton2) {
    myEditCurrentArgument = GroupPoints->LineEdit2;

    GroupPoints->PushButton1->setDown(false);
    GroupPoints->LineEdit1->setEnabled(false);

    globalSelection(GEOM_LINE);// to break previous local selection
    localSelection(GEOM::GEOM_Object::_nil(), TopAbs_EDGE);
  }
  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  // enable line edit
  myEditCurrentArgument->setEnabled(true);
  myEditCurrentArgument->setFocus();
  // after setFocus(), because it will be setDown(false) then loses focus
  send->setDown(true);
}

//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if (send == GroupPoints->LineEdit1 ||
      send == GroupPoints->LineEdit2) {
    myEditCurrentArgument = send;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}

//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();

  // reinit, because some selected objects could be removed
  Init();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::enterEvent (QEvent*)
{
  if (!mainFrame()->GroupConstructors->isEnabled())
    ActivateThisDialog();
}

//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void PrimitiveGUI_ConeDlg::ValueChangedInSpinBox()
{
  displayPreview();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr  PrimitiveGUI_ConeDlg::createOperation()
{
  return getGeomEngine()->GetI3DPrimOperations(getStudyId());
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool  PrimitiveGUI_ConeDlg::isValid (QString&)
{
  return getConstructorId() == 0 ? !(myPoint->_is_nil() || myDir->_is_nil()) : true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool PrimitiveGUI_ConeDlg::execute (ObjectList& objects)
{
  bool res = false;

  GEOM::GEOM_Object_var anObj;

  switch (getConstructorId()) {
  case 0:
    if (!CORBA::is_nil(myPoint) && !CORBA::is_nil(myDir)) {
      anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
        MakeConePntVecR1R2H(myPoint, myDir, getRadius1(), getRadius2(), getHeight());
      res = true;
    }
    break;
  case 1:
    anObj = GEOM::GEOM_I3DPrimOperations::_narrow(getOperation())->
      MakeConeR1R2H(getRadius1(), getRadius2(), getHeight());
    res = true;
    break;
  }

  if (!anObj->_is_nil())
    objects.push_back(anObj._retn());

  return res;
}

//=================================================================================
// function : getRadius1()
// purpose  :
//=================================================================================
double PrimitiveGUI_ConeDlg::getRadius1() const
{
  int aConstructorId = getConstructorId();
  if (aConstructorId == 0)
    return GroupPoints->SpinBox_DX->value();
  else if (aConstructorId == 1)
    return GroupDimensions->SpinBox_DX->value();
  return 0;
}

//=================================================================================
// function : getRadius2()
// purpose  :
//=================================================================================
double PrimitiveGUI_ConeDlg::getRadius2() const
{
  int aConstructorId = getConstructorId();
  if (aConstructorId == 0)
    return GroupPoints->SpinBox_DY->value();
  else if (aConstructorId == 1)
    return GroupDimensions->SpinBox_DY->value();
  return 0;
}

//=================================================================================
// function : getHeight()
// purpose  :
//=================================================================================
double PrimitiveGUI_ConeDlg::getHeight() const
{
  int aConstructorId = getConstructorId();
  if (aConstructorId == 0)
    return GroupPoints->SpinBox_DZ->value();
  else if (aConstructorId == 1)
    return GroupDimensions->SpinBox_DZ->value();
  return 0;
}

//=================================================================================
// function : addSubshapeToStudy
// purpose  : virtual method to add new SubObjects if local selection
//=================================================================================
void PrimitiveGUI_ConeDlg::addSubshapesToStudy()
{
  QMap<QString, GEOM::GEOM_Object_var> objMap;

  switch (getConstructorId()) {
  case 0:
    objMap[GroupPoints->LineEdit1->text()] = myPoint;
    objMap[GroupPoints->LineEdit2->text()] = myDir;
    break;
  case 1:
    return;
  }
  addSubshapesToFather(objMap);
}
