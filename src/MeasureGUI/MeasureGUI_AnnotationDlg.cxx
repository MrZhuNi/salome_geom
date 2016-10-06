// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

//  File   : MeasureGUI_AnnotationDlg.cxx

#include "MeasureGUI.h"
#include "MeasureGUI_AnnotationDlg.h"

#include <DlgRef.h>
#include <GEOMBase.h>
#include <GEOMBase_Skeleton.h>
#include <GEOM_Displayer.h>
#include <GeometryGUI.h>
#include <GEOM_Annotation.hxx>

#include <SOCC_Prs.h>
#include <SOCC_ViewModel.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <SalomeApp_Tools.h>
#include <SalomeApp_IntSpinBox.h>
#include <SalomeApp_DoubleSpinBox.h>

#include <LightApp_SelectionMgr.h>

#include <OCCViewer_ViewModel.h>
#include <OCCViewer_ViewManager.h>
#include <SVTK_ViewModel.h>
#include <SALOME_Prs.h>
#include <SALOME_ListIO.hxx>

#include <SUIT_Desktop.h>
#include <SUIT_MessageBox.h>
#include <SUIT_OverrideCursor.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TColStd_MapOfInteger.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

#include <limits>

#include <SALOMEDS_wrap.hxx>
#include <GEOMImpl_Types.hxx>

#ifdef max
#undef max
#endif

//=======================================================================
//function : MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::MeasureGUI_AnnotationDlg(GeometryGUI* theGeometryGUI,
        const bool theIsCreate, QWidget* parent, bool modal, Qt::WindowFlags fl)
: GEOMBase_Skeleton(theGeometryGUI, parent, modal, fl),
  myIsCreation(theIsCreate)
{
  myEditCurrentArgument = 0;

  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  QPixmap iconSelect(resMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

  setWindowTitle(myIsCreation ?
                 tr("CREATE_ANNOTATION_TITLE") :
                 tr("EDIT_ANNOTATION_TITLE"));

  // Shape type button group
  mainFrame()->GroupConstructors->hide();

  // Field name
  mainFrame()->GroupBoxName->setTitle(tr("ANNOTATION_NAME"));

  // Field properties
  QGroupBox* propGroup = new QGroupBox(tr("ANNOTATION_PROPERTIES"), centralWidget());
  QGridLayout* propLayout = new QGridLayout(propGroup);
  propLayout->setMargin(9);
  propLayout->setSpacing(6);

  QLabel* textLabel = new QLabel(tr("ANNOTATION_TEXT"), propGroup);
  myTextEdit = new QLineEdit(propGroup);
  propLayout->addWidget(textLabel, 0, 0);
  propLayout->addWidget(myTextEdit, 0, 1, 1, 2);

  // shape
  QLabel* shapeLabel = new QLabel(tr("ANNOTATION_SHAPE"), propGroup);
  myShapeSelBtn = new QPushButton(propGroup);
  myShapeSelBtn->setIcon(iconSelect);
  myShapeSelBtn->setEnabled(myIsCreation);
  myShapeName = new QLineEdit(propGroup);
  myShapeName->setReadOnly(true);
  myShapeName->setEnabled(myIsCreation);

  // data type
  QLabel* typeLabel = new QLabel(tr("ANNOTATION_TYPE"), propGroup);
  myTypeCombo = new QComboBox(propGroup);
  myTypeCombo->insertItem(0, tr("3D"));
  myTypeCombo->insertItem(1, tr("2D"));
  myTypeCombo->setCurrentIndex(0); // double

  propLayout->addWidget(shapeLabel, 1, 0);
  propLayout->addWidget(myShapeSelBtn, 1, 1);
  propLayout->addWidget(myShapeName, 1, 2);
  propLayout->addWidget(typeLabel,          2, 0, 1, 2);
  propLayout->addWidget(myTypeCombo,        2, 2);
  propLayout->setColumnStretch(2, 5);

  QLabel* shapeTypeLabel = new QLabel(tr("ANNOTATION_SUB_SHAPE"), propGroup);
  mySubShapeTypeCombo = new QComboBox(propGroup);
  mySubShapeTypeCombo->addItem(tr("WHOLE_SHAPE"), TopAbs_SHAPE);
  mySubShapeTypeCombo->addItem(tr("GEOM_VERTEX"), TopAbs_VERTEX);
  mySubShapeTypeCombo->addItem(tr("GEOM_EDGE"), TopAbs_EDGE);
  mySubShapeTypeCombo->addItem(tr("GEOM_FACE"), TopAbs_FACE);
  mySubShapeTypeCombo->addItem(tr("GEOM_SOLID"), TopAbs_SOLID);
  mySubShapeTypeCombo->setCurrentIndex(0); // VERTEX

  propLayout->addWidget(shapeTypeLabel, 3, 0);
  propLayout->addWidget(mySubShapeTypeCombo, 3, 1, 1, 2);

  // sub-shape
  mySubShapeSelBtn = new QPushButton(propGroup);
  mySubShapeSelBtn->setIcon(iconSelect);
  mySubShapeSelBtn->setEnabled(myIsCreation);
  mySubShapeName = new QLineEdit(propGroup);
  mySubShapeName->setReadOnly(true);
  mySubShapeName->setEnabled(myIsCreation);

  propLayout->addWidget(mySubShapeSelBtn, 4, 1);
  propLayout->addWidget(mySubShapeName, 4, 2);

  // Field properties
  QGroupBox* styleGroup = new QGroupBox(tr("ANNOTATION_STYLE"), centralWidget());
  QGridLayout* styleLayout = new QGridLayout(styleGroup);
  styleLayout->setMargin(9);
  styleLayout->setSpacing(6);

  QLabel* fontLabel = new QLabel(tr("ANNOTATION_FONT"), styleGroup);
  styleLayout->addWidget(fontLabel, 0, 0);

  QVBoxLayout* layout = new QVBoxLayout(centralWidget());
  layout->setMargin(0);
  layout->setSpacing(6);
  layout->addWidget(propGroup);
  layout->addWidget(styleGroup);

  setHelpFileName("annotation_page.html");

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  connect(aSelMgr, SIGNAL(currentSelectionChanged()), this,
          SLOT(SelectionIntoArgument()));
  connect(buttonOk(), SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  Init();
  //updateState();
}

//=======================================================================
//function : ~MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::~MeasureGUI_AnnotationDlg()
{
}

//=================================================================================
// function : Init()
// purpose  : fills annotation properties with default values(in create mode) or
// the values of modified object
//=================================================================================
void MeasureGUI_AnnotationDlg::Init()
{
  if (myIsCreation) {
    initName(tr("ANNOTATION_PREFIX"));

    myEditCurrentArgument = myShapeName;

    // default presentation values
    myAnnotationProperties.Position = gp_Pnt(50, 50, 50);
    myAnnotationProperties.Text = tr("ANNOTATION_PREFIX");
    myAnnotationProperties.IsVisible = true;
    myAnnotationProperties.IsScreenFixed = false;
    myAnnotationProperties.Attach = gp_Pnt(0, 0, 0);
    myAnnotationProperties.ShapeIndex = -1;
    myAnnotationProperties.ShapeType = (int)TopAbs_SHAPE;

    SelectionIntoArgument();

    mySelectionMode = TopAbs_SHAPE;

    connect(myShapeSelBtn, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
    connect(mySubShapeSelBtn, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

    connect(myTextEdit,   SIGNAL(textChanged(const QString&)), this, SLOT(onTextChange()));
    connect(myTypeCombo,   SIGNAL(currentIndexChanged(int)),  this, SLOT(onTypeChange()));
    connect(mySubShapeTypeCombo,   SIGNAL(currentIndexChanged(int)),
            this, SLOT(onSubShapeTypeChange()));

    SelectionIntoArgument();
  }
  else { // edition

  }
  redisplayPreview();
}

//=================================================================================
// function : activateSelection
// purpose  : Activate local selection
//=================================================================================
void MeasureGUI_AnnotationDlg::activateSelection()
{
  globalSelection(GEOM_ALLSHAPES);
  if (!myShape->_is_nil() && mySelectionMode != TopAbs_SHAPE) {
    localSelection(myShape.get(), mySelectionMode);
  }
}

//=================================================================================
// function : getShapeType()
// purpose  :
//=================================================================================
TopAbs_ShapeEnum MeasureGUI_AnnotationDlg::getShapeType() const
{
  return (TopAbs_ShapeEnum)mySubShapeTypeCombo->itemData(mySubShapeTypeCombo->currentIndex()).toInt();
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::ClickOnOk()
{
    setIsApplyAndClose(true);
    if (ClickOnApply())
        ClickOnCancel();
    setIsApplyAndClose(false);
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::ClickOnApply()
{
    if(!isApplyAndClose()) {
        setIsDisableBrowsing(true);
        setIsDisplayResult(false);
    }

    QString msg;
    if (!isValid(msg)) {
        showError(msg);
        return false;
    }

    SUIT_OverrideCursor wc;
    SUIT_Session::session()->activeApplication()->putInfo("");

    try
    {
        if (openCommand())
            if (!execute (/*isApplyAndClose()*/))
            {
                abortCommand();
                showError();
                return false;
            }
    }
    catch(const SALOME::SALOME_Exception& e) {
        SalomeApp_Tools::QtCatchCorbaException(e);
        abortCommand();
        return false;
    }
    commitCommand();

    if(!isApplyAndClose()) {
        setIsDisableBrowsing(false);
        setIsDisplayResult(true);
    }

    /*if (myIsCreation)
    {
        myAnnotation = GEOM::GEOM_Field::_nil();
        if (!isApplyAndClose())
            Init();
    }*/
    return true;
 }

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  : process click on shape/sub-shape button. It stores as current edit argument
// the corresponded line edit, set focus in it and unpress other button if it was pressed
//=================================================================================
void MeasureGUI_AnnotationDlg::SetEditCurrentArgument()
{
  QPushButton* aSelectButton = (QPushButton*)sender();
  QPushButton* anOtherButton = 0;
  if (aSelectButton == myShapeSelBtn) {
    myEditCurrentArgument = myShapeName;
    anOtherButton = mySubShapeSelBtn;
  }
  else if (aSelectButton == mySubShapeSelBtn) {
    myEditCurrentArgument = mySubShapeName;
    anOtherButton = myShapeSelBtn;
  }
  else
    myEditCurrentArgument = 0;

  if (myEditCurrentArgument)
    myEditCurrentArgument->setFocus();

  aSelectButton->setDown(true);

  if (aSelectButton->isChecked())
    anOtherButton->setDown(false);

  SelectionIntoArgument();
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed. Sets the current selection in the
// annotation property and redisplays presentation
//=================================================================================
void MeasureGUI_AnnotationDlg::SelectionIntoArgument()
{
  if (myIsCreation && myEditCurrentArgument) {
    GEOM::GeomObjPtr anObj = getSelected(mySelectionMode);

    QString aName = GEOMBase::GetName(anObj.get());

    gp_Pnt anAttachPoint(0, 0, 0);
    int aSubShapeIndex = -1;
    if (myEditCurrentArgument == myShapeName) { // Selection of a shape is active
      if (mySelectionMode == TopAbs_SHAPE) {
        myEditCurrentArgument->setText(aName);

        if (anObj->_is_nil())
          myShape = GEOM::GEOM_Object::_nil();
        else
          myShape = anObj;

        bool aNullShape = myShape->_is_nil();
        mySubShapeTypeCombo->setEnabled(!aNullShape);
        mySubShapeSelBtn->setEnabled(!aNullShape);
        mySubShapeName->setEnabled(!aNullShape);
        activateSelection();


        if (!myShape->_is_nil()) {
          TopoDS_Shape aShape;
          GEOMBase::GetShape(myShape.get(), aShape);
          anAttachPoint = getAttachPoint(aShape);
        }
      }
    }
    else if (myEditCurrentArgument == mySubShapeName) {
      if (!myShape->_is_nil()) {
        myEditCurrentArgument->setText(aName);

        TopTools_IndexedMapOfShape aMainMap;
        TopoDS_Shape aMainShape;
        GEOMBase::GetShape(myShape.get(), aMainShape);
        TopExp::MapShapes(aMainShape, aMainMap);

        TopExp_Explorer anExp (aMainShape, getShapeType());
        bool isShowWarning = true;

        TopoDS_Shape aSubShape;
        for (; anExp.More() && aSubShapeIndex < 0; anExp.Next()) {
          TopoDS_Shape aCurrentSubShape = anExp.Current();
          int anIndex = aMainMap.FindIndex(aCurrentSubShape);
          if (anIndex >= 0) {
            aSubShapeIndex = anIndex;
            aSubShape = aCurrentSubShape;
          }
        }
        anAttachPoint = getAttachPoint(aSubShape);
      }
    }
    myAnnotationProperties.Attach = anAttachPoint;
  }
}

//=======================================================================
//function : onTextChange
//purpose  : change annotation text
//=======================================================================
void MeasureGUI_AnnotationDlg::onTextChange()
{
  myAnnotationProperties.Text = myTextEdit->text();
  redisplayPreview();
}

//=======================================================================
//function : onTypeChange
//purpose  : change annotation type: 2D or 3D
//=======================================================================
void MeasureGUI_AnnotationDlg::onTypeChange()
{
  myAnnotationProperties.IsScreenFixed = myTypeCombo->currentIndex() == 1;
  redisplayPreview();
}

//=======================================================================
//function : onSubShapeTypeChange
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::onSubShapeTypeChange()
{
  TopAbs_ShapeEnum aShapeType = getShapeType();
  myAnnotationProperties.ShapeType = aShapeType;

  mySelectionMode = getShapeType();
  activateSelection();
  redisplayPreview();
}

#define RETURN_WITH_MSG(a, b) \
  if ((a)) { \
    theMessage += (b); \
    return false; \
  }

//=================================================================================
// function : isValid()
// purpose  : Verify validity of input data
//=================================================================================
bool MeasureGUI_AnnotationDlg::isValid(QString& theMessage)
{
  SalomeApp_Study* study = getStudy();
  RETURN_WITH_MSG(!study, tr("GEOM_NO_STUDY"))
  RETURN_WITH_MSG(study->studyDS()->GetProperties()->IsLocked(),
                  tr("GEOM_STUDY_LOCKED"))

  if (myIsCreation) {
    RETURN_WITH_MSG(myShape->_is_nil(), tr("NO_SHAPE"))
  }
  else {
    //RETURN_WITH_MSG(CORBA::is_nil(myShape), tr("NO_FIELD"))
  }

  if (getShapeType() != TopAbs_SHAPE) {
    if (myIsCreation) {
      RETURN_WITH_MSG(myAnnotationProperties.ShapeIndex < 0, tr("NO_SUB_SHAPE"))
    }
    else {
      //RETURN_WITH_MSG(CORBA::is_nil(myShape), tr("NO_FIELD"))
    }
  }
  return true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::execute()
{
  QString anError;
  if (!isValid(anError))
    return false;

  if (myIsCreation) {

  }
  return true;
}

//=================================================================================
// function : buildPrs
// purpose  : creates annotation presentation object and corresponded SALOME presentation
//=================================================================================
SALOME_Prs* MeasureGUI_AnnotationDlg::buildPrs()
{
  Handle (GEOM_Annotation) aPresentation = new GEOM_Annotation();

  TopoDS_Shape aShape;
  GEOMBase::GetShape(myShape.get(), aShape);
  gp_Ax3 aShapeLCS = gp_Ax3().Transformed(aShape.Location().Transformation());
  myAnnotationProperties.ToPresentation(aPresentation, aShapeLCS);

  // add Prs to preview
  SUIT_ViewWindow* vw =
          SUIT_Session::session()->activeApplication()->desktop()->activeWindow();
  SOCC_Prs* aPrs =
          dynamic_cast<SOCC_Prs*>(((SOCC_Viewer*) (vw->getViewManager()->getViewModel()))->CreatePrs(
                  0));

  if (aPrs)
    aPrs->AddObject(aPresentation);

  return aPrs;
}


//=================================================================================
// function : buildPrs
// purpose  : creates annotation presentation object and corresponded SALOME presentation
//=================================================================================
void MeasureGUI_AnnotationDlg::redisplayPreview()
{
  QString aMess;
  if (!isValid(aMess)) {
    erasePreview(true);
    return;
  }

  erasePreview(false);

  try {
    //SUIT_OverrideCursor wc;
    //getDisplayer()->SetColor(Quantity_NOC_VIOLET);
    //getDisplayer()->SetToActivate(false);

    if (SALOME_Prs* aPrs = buildPrs())
      displayPreview(aPrs);
  }
  catch (const SALOME::SALOME_Exception& e) {
    SalomeApp_Tools::QtCatchCorbaException(e);
  }
  catch (...) {
  }
}

//=================================================================================
// function : getAttachPoint
// purpose  : finds a point on shape to attach annotation object
//=================================================================================
gp_Pnt MeasureGUI_AnnotationDlg::getAttachPoint(const TopoDS_Shape& theShape)
{
  gp_Pnt aPoint = gp_Pnt(300, 300, 300);

  return aPoint;
}

