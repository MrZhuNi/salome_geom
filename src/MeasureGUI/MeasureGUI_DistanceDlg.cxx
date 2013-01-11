// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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

// GEOM GEOMGUI : GUI for Geometry component
// File   : MeasureGUI_DistanceDlg.cxx
// Author : Nicolas REJNERI, Open CASCADE S.A.S.

#include "MeasureGUI_DistanceDlg.h"
#include "MeasureGUI_Widgets.h"

#include <DlgRef.h>
#include <GEOMBase.h>
#include <GeometryGUI.h>

#include <SUIT_Session.h>
#include <SUIT_Desktop.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>
#include <SUIT_OverrideCursor.h>
#include <SOCC_Prs.h>
#include <SOCC_ViewModel.h>
#include <SalomeApp_Tools.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

// OCCT Includes
#include <Geom_Plane.hxx>
#include <AIS_LengthDimension.hxx>
#include <AIS_Drawer.hxx>
#include <Prs3d_LengthAspect.hxx>
#include <Prs3d_LineAspect.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gce_MakePln.hxx>
#include <Precision.hxx>

//=================================================================================
// class    : MeasureGUI_DistanceDlg()
// purpose  : Constructs a MeasureGUI_DistanceDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            true to construct a modal dialog.
//=================================================================================
MeasureGUI_DistanceDlg::MeasureGUI_DistanceDlg (GeometryGUI* GUI, QWidget* parent)
  : GEOMBase_Skeleton(GUI, parent)
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap image0 (aResMgr->loadPixmap("GEOM", tr("ICON_DLG_MINDIST")));
  QPixmap image1 (aResMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

  setWindowTitle(tr("GEOM_MINDIST_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_DISTANCE"));
  mainFrame()->RadioButton1->setIcon(image0);
  mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();

  myGrp = new MeasureGUI_2Sel4LineEdit (centralWidget());
  myGrp->GroupBox1->setTitle(tr("GEOM_MINDIST_OBJ"));
  myGrp->TextLabel1->setText(tr("GEOM_OBJECT_I").arg("1"));
  myGrp->TextLabel2->setText(tr("GEOM_OBJECT_I").arg("2"));
  myGrp->TextLabel3->setText(tr("GEOM_LENGTH"));
  myGrp->TextLabel4->setText(tr("GEOM_DX"));
  myGrp->TextLabel5->setText(tr("GEOM_DY"));
  myGrp->TextLabel6->setText(tr("GEOM_DZ"));
  myGrp->LineEdit3->setReadOnly(true);
  myGrp->PushButton1->setIcon(image1);
  myGrp->PushButton2->setIcon(image1);
  myGrp->LineEdit1->setReadOnly(true);
  myGrp->LineEdit2->setReadOnly(true);
  myGrp->LineEdit4->setReadOnly(true);
  myGrp->LineEdit5->setReadOnly(true);
  myGrp->LineEdit6->setReadOnly(true);

  QVBoxLayout* layout = new QVBoxLayout (centralWidget());
  layout->setMargin(0); layout->setSpacing(6);
  layout->addWidget(myGrp);
  /***************************************************************/

  myHelpFileName = "min_distance_page.html";

  // Initialisation
  Init();
}

//=================================================================================
// function : ~MeasureGUI_DistanceDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MeasureGUI_DistanceDlg::~MeasureGUI_DistanceDlg()
{
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::Init()
{
  myEditCurrentArgument = myGrp->LineEdit1;

  // signals and slots connections
  connect(buttonOk(),         SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(),      SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(myGrp->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(myGrp->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  initName(tr("GEOM_DISTANCE"));
  globalSelection();
  SelectionIntoArgument();
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::ClickOnOk()
{
  if (ClickOnApply())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool MeasureGUI_DistanceDlg::ClickOnApply()
{
  if (!onAccept())
    return false;

  initName();
  return true;
}

//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();

  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  globalSelection();
  redisplayPreview();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::enterEvent(QEvent*)
{
  if (!mainFrame()->GroupConstructors->isEnabled())
    ActivateThisDialog();
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void MeasureGUI_DistanceDlg::SelectionIntoArgument()
{
  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);

  GEOM::GEOM_Object_var aSelectedObject = GEOM::GEOM_Object::_nil();

  if (aSelList.Extent() > 0) {
    aSelectedObject = GEOMBase::ConvertIOinGEOMObject(aSelList.First());
  }

  // clear selection
  disconnect(myGeomGUI->getApp()->selectionMgr(), 0, this, 0);
  myGeomGUI->getApp()->selectionMgr()->clearSelected();
  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  if (myEditCurrentArgument == myGrp->LineEdit1) {
    myObj1 = aSelectedObject;
    if (!myObj1->_is_nil() && myObj2->_is_nil())
      myGrp->PushButton2->click();
  }
  else {
    myObj2 = aSelectedObject;
    if (!myObj2->_is_nil() && myObj1->_is_nil())
      myGrp->PushButton1->click();
  }

  processObject();
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if (send == myGrp->PushButton1) {
    myEditCurrentArgument = myGrp->LineEdit1;

    myGrp->PushButton2->setDown(false);
    myGrp->LineEdit2->setEnabled(false);
  }
  else {
    myEditCurrentArgument = myGrp->LineEdit2;

    myGrp->PushButton1->setDown(false);
    myGrp->LineEdit1->setEnabled(false);
  }

  // enable line edit
  myEditCurrentArgument->setEnabled(true);
  myEditCurrentArgument->setFocus();
  // after setFocus(), because it will be setDown(false) when loses focus
  send->setDown(true);

  // seems we need it only to avoid preview disappearing, caused by selection mode change
  redisplayPreview();
}

//=================================================================================
// function : processObject()
// purpose  : Fill dialogs fields in accordance with myObj1 and myObj2
//=================================================================================
void MeasureGUI_DistanceDlg::processObject()
{
  myGrp->LineEdit1->setText(!myObj1->_is_nil() ? GEOMBase::GetName(myObj1) : "");
  myGrp->LineEdit2->setText(!myObj2->_is_nil() ? GEOMBase::GetName(myObj2) : "");

  gp_Pnt aPnt1, aPnt2;
  double aDist = 0.;
  if (getParameters(aDist, aPnt1, aPnt2)) {
    SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
    int aPrecision = resMgr->integerValue( "Geometry", "length_precision", 6 );

    myGrp->LineEdit3->setText(DlgRef::PrintDoubleValue(aDist, aPrecision));

    gp_XYZ aVec = aPnt2.XYZ() - aPnt1.XYZ();
    myGrp->LineEdit4->setText(DlgRef::PrintDoubleValue(aVec.X(), aPrecision));
    myGrp->LineEdit5->setText(DlgRef::PrintDoubleValue(aVec.Y(), aPrecision));
    myGrp->LineEdit6->setText(DlgRef::PrintDoubleValue(aVec.Z(), aPrecision));

    redisplayPreview();
  }
  else {
    myGrp->LineEdit3->setText("");
    myGrp->LineEdit4->setText("");
    myGrp->LineEdit5->setText("");
    myGrp->LineEdit6->setText("");
    erasePreview();
  }
}

//=================================================================================
// function : getParameters()
// purpose  : Get distance between objects
//=================================================================================
bool MeasureGUI_DistanceDlg::getParameters (double& theDistance, gp_Pnt& thePnt1, gp_Pnt& thePnt2)
{
  QString msg;
  if (isValid(msg)) {
    GEOM::GEOM_IMeasureOperations_var anOper = GEOM::GEOM_IMeasureOperations::_narrow(getOperation());

    try {
      double x1, y1, z1, x2, y2, z2;
      theDistance = anOper->GetMinDistance(myObj1, myObj2, x1, y1, z1, x2, y2, z2);

      thePnt1.SetCoord(x1, y1, z1);
      thePnt2.SetCoord(x2, y2, z2);
    }
    catch (const SALOME::SALOME_Exception& e) {
      SalomeApp_Tools::QtCatchCorbaException(e);
      return false;
    }

    return anOper->IsDone();
  }

  return false;
}

//=================================================================================
// function : buildPrs()
// purpose  :
//=================================================================================
SALOME_Prs* MeasureGUI_DistanceDlg::buildPrs()
{
  double aDist = 0.;
  gp_Pnt aPnt1 (0, 0, 0), aPnt2 (0, 0, 0);

  SUIT_ViewWindow* vw = SUIT_Session::session()->activeApplication()->desktop()->activeWindow();

  if (myObj1->_is_nil() || myObj2->_is_nil() ||
      !getParameters(aDist, aPnt1, aPnt2) ||
      vw->getViewManager()->getType() != OCCViewer_Viewer::Type())
    return 0;

  try
  {
    if (aDist <= 1.e-9) {
      BRepBuilderAPI_MakeVertex aMaker (aPnt1);
      return getDisplayer()->BuildPrs(aMaker.Vertex());
    }
    else {
      BRepBuilderAPI_MakeEdge MakeEdge (aPnt1, aPnt2);
      TopoDS_Vertex aVert1 = BRepBuilderAPI_MakeVertex(aPnt1);
      TopoDS_Vertex aVert2 = BRepBuilderAPI_MakeVertex(aPnt2);

      QString aLabel;
      aLabel.sprintf("%.1f", aDist);

      gp_Pnt aPnt3 ((aPnt1.X() + aPnt2.X()) / 2,
                    (aPnt1.Y() + aPnt2.Y()) / 2,
                    (aPnt1.Z() + aPnt2.Z()) / 2);

      gp_Vec va (aPnt3, aPnt1);
      gp_Vec vb (aPnt3, aPnt2);

      if (va.IsParallel(vb, Precision::Angular())) {
        aPnt3.SetY((aPnt1.Y() + aPnt2.Y()) / 2 + 100);
        aPnt3.SetZ((aPnt1.Z() + aPnt2.Z()) / 2);
      }

      gce_MakePln gce_MP (aPnt1, aPnt2, aPnt3);
      Handle(Geom_Plane) P = new Geom_Plane (gce_MP.Value());

      Handle(AIS_LengthDimension) anIO = new AIS_LengthDimension
        (aVert1, aVert2, P, aDist,
         TCollection_ExtendedString((Standard_CString)aLabel.toLatin1().constData()));
      anIO->SetArrowSize(aDist/20);
      
      SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
      int w = resMgr->integerValue("Geometry", "measures_line_width", 1);
      Handle(Prs3d_LengthAspect) asp = new Prs3d_LengthAspect();
      asp->LineAspect()->SetWidth(w);
      anIO->Attributes()->SetLengthAspect(asp);

      SOCC_Prs* aPrs = dynamic_cast<SOCC_Prs*>(((SOCC_Viewer*)(vw->getViewManager()->getViewModel()))->CreatePrs(0));

      if (aPrs)
        aPrs->AddObject(anIO);

      return aPrs;
    }
  }
  catch (Standard_Failure) {
    return 0;
  }
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr MeasureGUI_DistanceDlg::createOperation()
{
  return getGeomEngine()->GetIMeasureOperations(getStudyId());
}

//=================================================================================
// function : isValid()
// purpose  :
//=================================================================================
bool MeasureGUI_DistanceDlg::isValid (QString& msg)
{
  return !myObj1->_is_nil() && !myObj2->_is_nil();
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool MeasureGUI_DistanceDlg::execute (ObjectList& objects)
{
  GEOM::GEOM_IMeasureOperations_var anOper = GEOM::GEOM_IMeasureOperations::_narrow(getOperation());
  GEOM::GEOM_IBasicOperations_var aBasicOper = getGeomEngine()->GetIBasicOperations(getStudyId());

  double x1, y1, z1, x2, y2, z2;
  double aDist = anOper->GetMinDistance(myObj1, myObj2, x1, y1, z1, x2, y2, z2);

  GEOM::GEOM_Object_var anObj1 = aBasicOper->MakePointXYZ(x1, y1, z1);
  GEOM::GEOM_Object_var anObj2 = aBasicOper->MakePointXYZ(x2, y2, z2);

  if (!anObj1->_is_nil() && !anObj2->_is_nil()) {
    objects.push_back(anObj1._retn());
    objects.push_back(anObj2._retn());
  }

  return true;
}

//=================================================================================
// function : redisplayPreview()
// purpose  :
//=================================================================================
void MeasureGUI_DistanceDlg::redisplayPreview()
{
  QString aMess;
  if (!isValid(aMess)) {
    erasePreview(true);
    return;
  }

  erasePreview(false);

  try {
    SUIT_OverrideCursor();

    getDisplayer()->SetColor(Quantity_NOC_VIOLET);
    getDisplayer()->SetToActivate(false);
    
    if (SALOME_Prs* aPrs = buildPrs())
      displayPreview(aPrs);
  }
  catch (const SALOME::SALOME_Exception& e) {
    SalomeApp_Tools::QtCatchCorbaException(e);
  }
}
