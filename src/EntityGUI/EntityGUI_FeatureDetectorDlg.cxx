// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
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
//

// GEOM GEOMGUI : GUI for Geometry component
// File   : EntityGUI_SketcherDlg.cxx
// Author : Renaud NEDELEC, Open CASCADE S.A.S.

#include "EntityGUI_FeatureDetectorDlg.h"

#include <OCCViewer_ViewPort3d.h>
#include <OCCViewer_ViewWindow.h>
#include <OCCViewer_ViewSketcher.h>
#include <OCCViewer_FeatureDetector.h>
#include <OCCViewer_ViewManager.h>

#include <SOCC_ViewModel.h>

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <EntityGUI.h>
#include <GEOMBase.h>
#include <GEOM_Object.hxx>

#include <SUIT_Desktop.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_ViewWindow.h>
#include <SalomeApp_Application.h>
#include <LightApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <SALOME_ListIteratorOfListIO.hxx>

#include <SalomeApp_Study.h>

#include <utilities.h>

#include <gp_Pnt.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <AIS_TexturedShape.hxx>
#include <TCollection_AsciiString.hxx>
#include <StdSelect_DisplayMode.hxx>

#include <Graphic3d_MaterialAspect.hxx>

#include <Precision.hxx>

#include <set>
#include <utility>

// Constructors
enum{
  CONTOURS,
  CORNERS
};

enum {
  RADIO_BUTTONS,
  MSG,
  PUSH_BUTTON,
  X_COORD,
  Y_COORD,
  Z_COORD
};

// View
enum {
  XY,
  YZ,
  XZ
};

enum {
  LABEL1,
  SPINBOX11,
//   SPINBOX12,
  LABEL2,
  SPINBOX21,
//   SPINBOX22,
  NB_COLS
};
  

//=================================================================================
// class    : EntityGUI_FeatureDetectorDlg()
// purpose  : Constructs a EntityGUI_FeatureDetectorDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
EntityGUI_FeatureDetectorDlg::EntityGUI_FeatureDetectorDlg( GeometryGUI* theGeometryGUI, QWidget* parent,
                                              bool modal, Qt::WindowFlags fl)
  : GEOMBase_Skeleton(theGeometryGUI, parent, modal, fl) 
{
  QPixmap image0(SUIT_Session::session()->resourceMgr()->loadPixmap("OCCViewer", tr("ICON_OCCVIEWER_VIEW_DUMP")));
  QPixmap image1(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_SELECT")));
  
  setWindowTitle(tr("GEOM_DETECT_TITLE"));
  
  /***************************************************************/
  
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_FEATURES"));
  mainFrame()->RadioButton1->setText(tr("GEOM_CONTOURS"));
  mainFrame()->RadioButton2->setText(tr("GEOM_CORNERS"));
//   mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
//   mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();
  
  myViewGroup = new DlgRef_3Radio(centralWidget());
  myViewGroup->GroupBox1->setTitle(tr("GEOM_VIEW"));
  myViewGroup->RadioButton1->setText(tr( "GEOM_TOP"  ));
  myViewGroup->RadioButton2->setText(tr( "GEOM_FRONT"));
  myViewGroup->RadioButton3->setText(tr( "GEOM_LEFT" ));
  myViewButtonGroup = new QButtonGroup( this );
  myViewButtonGroup->addButton( myViewGroup->RadioButton1, XY ); // Top view
  myViewButtonGroup->addButton( myViewGroup->RadioButton2, YZ ); // Front View
  myViewButtonGroup->addButton( myViewGroup->RadioButton3, XZ ); // Left View
  
  mySelectionGroup = new QGroupBox(tr("GEOM_DETECT"), centralWidget());
  QGridLayout* mySelectGrpLayout = new QGridLayout(mySelectionGroup);
  myPushButton = new QPushButton(mySelectionGroup);
  myPushButton->setIcon(image0);
  myPushButton->setCheckable(true);
  
  mySelButton = new QPushButton(mySelectionGroup);
  myLineEdit = new QLineEdit(mySelectionGroup);
  
  mySnapshotLabel = new QLabel(mySelectionGroup);
  mySelectGrpLayout->addWidget(myLineEdit,      0, 0);
  mySelectGrpLayout->addWidget(mySelButton,     0, 1);
  mySelectGrpLayout->addWidget(mySnapshotLabel, 1, 0);
  mySelectGrpLayout->addWidget(myPushButton,    1, 1);
  
  myCoordGrp1 = new QGroupBox(tr("GEOM_SCALING"), centralWidget());
  QGridLayout* myCoordGrpLayout = new QGridLayout(myCoordGrp1);
  
  std::string msg = "Please point the top left and the top right corners \non the backgound image \n";
//   in order to determine \n the right scale to use to display the result \n" ;
  
  QLabel* msgLabel = new QLabel(tr(msg.c_str()), myCoordGrp1);
//   msgLabel->setAlignment(Qt::AlignHCenter);
  myCoordGrpLayout->addWidget(msgLabel
  , MSG, 0, 1, NB_COLS);
  
  myPushButton1 = new QPushButton(myCoordGrp1);
  myCoordGrpLayout->addWidget(myPushButton1, PUSH_BUTTON, 0);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_PNT1"), myCoordGrp1), PUSH_BUTTON, 1);
  myPushButton2 = new QPushButton(myCoordGrp1);
  myCoordGrpLayout->addWidget(myPushButton2, PUSH_BUTTON, 2);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_PNT2"), myCoordGrp1), PUSH_BUTTON, 3);
  
  myPushButton1->setIcon(image1);
  myPushButton2->setIcon(image1);
  
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_X"), myCoordGrp1), X_COORD, 0);
  myX = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myX, X_COORD, 1, 1, 1);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_Y"), myCoordGrp1), Y_COORD, 0);
  myY = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myY, Y_COORD, 1, 1, 1);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_Z"), myCoordGrp1), Z_COORD, 0);
  myZ = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myZ, Z_COORD, 1, 1, 1);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_X"), myCoordGrp1), X_COORD, 2);
  myX2 = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myX2, X_COORD, 3, 1, 1);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_Y"), myCoordGrp1), Y_COORD, 2);
  myY2 = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myY2, Y_COORD, 3, 1, 1);
//   myCoordGrpLayout->addWidget(new QLabel(tr("GEOM_Z"), myCoordGrp1), Z_COORD, 2);
  myZ2 = new QLineEdit(myCoordGrp1);
  myCoordGrpLayout->addWidget(myZ2, Z_COORD, 3, 1, 1);
  
  myX->hide();
  myY->hide();
  myZ->hide();
  
  myX2->hide();
  myY2->hide();
  myZ2->hide();
  
  myPushButton1->hide();
  myPushButton2->hide();
  
  msgLabel->hide();
  
  myCoordGrp1->hide();
  
  myCoordGrpLayout->setColumnStretch(SPINBOX11,1);
  myCoordGrpLayout->setColumnStretch(SPINBOX21,1);
  
  QVBoxLayout* layout = new QVBoxLayout(centralWidget());
  layout->setMargin(0); layout->setSpacing(6);
  layout->addWidget( myCoordGrp1);
  layout->addWidget( myViewGroup);
  layout->addWidget( mySelectionGroup);
  
//   mainFrame()->GroupBoxName->hide();
  
  Init();
  
}

//=================================================================================
// function : ~EntityGUI_FeatureDetectorDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
EntityGUI_FeatureDetectorDlg::~EntityGUI_FeatureDetectorDlg()
{
  
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::Init()
{
  connect( myGeomGUI,         SIGNAL( SignalCloseAllDialogs() ), this, SLOT( ClickOnCancel() ) );
  connect( buttonOk(),        SIGNAL( clicked() ),               this, SLOT( ClickOnOk() ) );
  connect( buttonApply(),     SIGNAL( clicked() ),               this, SLOT( ClickOnApply() ) );
  
  connect( this,              SIGNAL(constructorsClicked(int)),  this, SLOT(ConstructorsClicked(int)));
  
  connect( myPushButton,      SIGNAL( toggled( bool ) ),         this, SLOT( onButtonToggled( bool ) ) );
  connect( mySelButton,       SIGNAL( clicked() ),               this, SLOT( SetEditCurrentArgument() ) );
  connect( myPushButton1,     SIGNAL( clicked() ),               this, SLOT( SetEditCurrentArgument() ) );
  connect( myPushButton2,     SIGNAL( clicked() ),               this, SLOT( SetEditCurrentArgument() ) );
  
  connect( myViewButtonGroup, SIGNAL( buttonClicked( int ) ),    this, SLOT( onViewClicked( int ) ) );
  
  connect( myGeomGUI->getApp()->selectionMgr(), SIGNAL( currentSelectionChanged() ),this, SLOT( SelectionIntoArgument() ) );
  
//   /* Get setting of step value from file configuration */
//   SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
//   double step = resMgr->doubleValue("Geometry", "SettingsGeomStep", 100);
// 
//   /* min, max, step and decimals for spin boxes */
//   initSpinBox(myX, COORD_MIN, COORD_MAX, step, "length_precision");
//   initSpinBox(myY, COORD_MIN, COORD_MAX, step, "length_precision");
//   initSpinBox(myZ, COORD_MIN, COORD_MAX, step, "length_precision");
//   myX->setValue(0.0);
//   myY->setValue(0.0);
//   myZ->setValue(0.0);
  
//   initSpinBox(myX2, COORD_MIN, COORD_MAX, step, "length_precision");
//   initSpinBox(myY2, COORD_MIN, COORD_MAX, step, "length_precision");
//   initSpinBox(myZ2, COORD_MIN, COORD_MAX, step, "length_precision");
//   myX2->setValue(0.0);
//   myY2->setValue(0.0);
//   myZ2->setValue(0.0);
  
  myConstructorId = 0;
  myPushButton1->setDown(true);
  
  myX2->setEnabled(false);
  myY2->setEnabled(false);
  myZ2->setEnabled(false);
  
  initName(tr("GEOM_CONTOURS")); 
  resize(100,100);
  
  myViewGroup->RadioButton1->setChecked(true);
  
  gp_Pnt aOrigin = gp_Pnt(0, 0, 0);
  gp_Dir aDirZ = gp_Dir(0, 0, 1);
  gp_Dir aDirX = gp_Dir(1, 0, 0);
  
  aGlobalCS = gp_Ax3(aOrigin, aDirZ, aDirX);
  
  myStartPnt = QPoint(0,0);
  myEndPnt = myStartPnt;
  
  myGeomGUI->SetWorkingPlane( aGlobalCS );
  myGeomGUI->ActiveWorkingPlane();
  
  ConstructorsClicked(myConstructorId);
  
//   SUIT_ViewWindow*       theViewWindow    = getDesktop()->activeWindow();
//   OCCViewer_Viewer*      anOCCViewer      = ( (OCCViewer_ViewManager*)( theViewWindow->getViewManager() ) )->getOCCViewer();
//   OCCViewer_ViewPort3d*  vp               = ((OCCViewer_ViewWindow*)theViewWindow)->getViewPort();
//   QString                theImgFileName   = vp->backgroundImageFilename();
//   Handle(AIS_InteractiveContext) aContext = anOCCViewer->getAISContext(); 
//   
//   gp_Pnt p1(0,0,0);
//   gp_Pnt p2(0,100,0);
//   gp_Pnt p3(100,100,0);
//   gp_Pnt p4(100,0,0);
//   
//   BRepBuilderAPI_MakePolygon Wire(p1,p2,p3,p4, Standard_True); 
//   BRepBuilderAPI_MakeFace Face(Wire,Standard_True);
//   const TopoDS_Shape& S = Face.Shape();
//   Handle(AIS_TexturedShape) anAIS = new AIS_TexturedShape(S);
//   if (!theImgFileName.isEmpty())
//   {
//     MESSAGE("filename is not empty")
//     anAIS->SetTextureFileName(TCollection_AsciiString(theImgFileName.toStdString().c_str()));
//     anAIS->SetTextureMapOn();
//     vp->getView()->SetSurfaceDetail(V3d_TEX_ALL);
// //     anAIS->SetDisplayMode(AIS_Shaded); 
//   }
//   else
//     anAIS->SetTextureMapOff();
//   //creation of the presentable object
//   aContext->SetDisplayMode(anAIS,3);
//   aContext->SetMaterial(anAIS,Graphic3d_NOM_SATIN);
//   aContext->Display(anAIS);
//   aContext->UpdateCurrentViewer();
  //Display the presentable object in the 3d viewer.
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  if ( send == mySelButton ) { 
    myEditCurrentArgument = myLineEdit;
    myLineEdit->setEnabled(true);
  }
  send->setDown(true);
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void EntityGUI_FeatureDetectorDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText( "" );

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);
  SALOME_ListIteratorOfListIO anIt( aSelList );
  for( ; anIt.More(); anIt.Next() )
    if( !anIt.Value().IsNull() )
    {
      myFaceEntry = anIt.Value()->getEntry();
    }

  if (aSelList.Extent() != 1) {
    if (myEditCurrentArgument == myLineEdit) 
      myFace.nullify();
    return;
  }

  TopAbs_ShapeEnum aNeedType = TopAbs_FACE ;
  GEOM::GeomObjPtr aSelectedObject = getSelected( aNeedType );
  TopoDS_Shape aShape;
  if ( aSelectedObject && GEOMBase::GetShape( aSelectedObject.get(), aShape ) && !aShape.IsNull() ) {
    QString aName = GEOMBase::GetName( aSelectedObject.get() );
    myEditCurrentArgument->setText( aName );
    if ( myEditCurrentArgument == myLineEdit ) {
      myFace = aSelectedObject;
    } 
  }
  
}


//=================================================================================
// function : OnPointSelected
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::OnPointSelected(const gp_Pnt& thePnt)
{
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  int aPrecision = resMgr->integerValue("Geometry", "length_precision", 6);
  if (myPushButton1->isDown())
  {
//     myX->setValue(thePnt.X());
//     myY->setValue(thePnt.Y());
//     myZ->setValue(thePnt.Z());
    myX->setText(DlgRef::PrintDoubleValue(thePnt.X(), aPrecision));
    myY->setText(DlgRef::PrintDoubleValue(thePnt.Y(), aPrecision));
    myZ->setText(DlgRef::PrintDoubleValue(thePnt.Z(), aPrecision));
    x1 = thePnt.X(); 
    y1 = thePnt.Y();
    z1 = thePnt.Z();
  
    myPushButton2->click();
  }
  else
  {
//     myX2->setValue(thePnt.X());
//     myY2->setValue(thePnt.Y());
//     myZ2->setValue(thePnt.Z());
    myX2->setText(DlgRef::PrintDoubleValue(thePnt.X(), aPrecision));
    myY2->setText(DlgRef::PrintDoubleValue(thePnt.Y(), aPrecision));
    myZ2->setText(DlgRef::PrintDoubleValue(thePnt.Z(), aPrecision));
    x2 = thePnt.X(); 
    y2 = thePnt.Y();
    z2 = thePnt.Z();
   
  }
}

//=================================================================================
// function : acceptMouseEvent()
// purpose  :
//=================================================================================
bool EntityGUI_FeatureDetectorDlg::acceptMouseEvent() const
{ 
  return myPushButton->isChecked();  
}

//=======================================================================
// function : ClickOnOk()
// purpose  :
//=======================================================================
void EntityGUI_FeatureDetectorDlg::ClickOnOk()
{
  setIsApplyAndClose( true );
  if (onAccept())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool EntityGUI_FeatureDetectorDlg::ClickOnApply()
{
  if (!onAccept())
    return false;

//   initName();
//   ConstructorsClicked(getConstructorId());
  return true;
}

//=================================================================================
// function : onConstructorClicked(int)
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::ConstructorsClicked(int id)
{
  MESSAGE("Constructor id ="<<id)
  myConstructorId = id;
  switch (id)
  {
    case CORNERS:
//       myViewGroup->show();
//       mySelectionGroup->show();
      mySnapshotLabel->setText(tr("GEOM_DETECT_ZONE"));
      initName(tr("GEOM_CORNERS"));
      break;
    case CONTOURS:
//       myViewGroup->hide();
//       mySelectionGroup->hide();
//       mySelectionGroup->show();
      mySnapshotLabel->setText(tr("GEOM_COLOR_FILTER"));
      initName(tr("GEOM_CONTOURS"));
      break;
  }
}

//=================================================================================
// function : onViewClicked()
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::onViewClicked(int id)
{
  gp_Pnt aOrigin = gp_Pnt(0, 0, 0);
  gp_Dir aDirZ;
  gp_Dir aDirX;
  
  switch(id)
  {
    case XY:
        aDirZ = gp_Dir(0, 0, 1);
        aDirX = gp_Dir(1, 0, 0);
      break;
    case YZ:
        aDirZ = gp_Dir(1, 0, 0);
        aDirX = gp_Dir(0, 1, 0);
      break;
    case XZ:
        aDirZ = gp_Dir(0, -1, 0);
        aDirX = gp_Dir(1, 0 , 0);
      break;
  }
  
  myWPlane = gp_Ax3(aOrigin, aDirZ, aDirX);
  myGeomGUI->SetWorkingPlane( myWPlane );
  myGeomGUI->ActiveWorkingPlane();
  
}

//=================================================================================
// function : onButtonToggled()
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::onButtonToggled( bool checked)
{
  if (!checked)
  {
    myStartPnt = QPoint(0,0);
    myEndPnt = myStartPnt;
  }
}

//=================================================================================
// function : setStartPnt( const QPoint& )
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::setStartPnt(const QPoint& theStartPnt)
{
  myStartPnt = theStartPnt;
}

//=================================================================================
// function : setEndPnt( const QPoint& )
// purpose  :
//=================================================================================
void EntityGUI_FeatureDetectorDlg::setEndPnt(const QPoint& theEndPnt)
{
  myEndPnt = theEndPnt;
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr EntityGUI_FeatureDetectorDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetIShapesOperations( getStudyId() );
}

//=================================================================================
// function : execute()
// purpose  :
//=================================================================================
bool EntityGUI_FeatureDetectorDlg::execute( ObjectList& objects )
{
  MESSAGE("EntityGUI_FeatureDetectorDlg::execute( ObjectList& objects )")
  
  bool res = false;
  SUIT_ViewWindow*       theViewWindow  = getDesktop()->activeWindow();
  OCCViewer_ViewPort3d*  vp             = ((OCCViewer_ViewWindow*)theViewWindow)->getViewPort();
//   QString                theImgFileName = vp->backgroundImageFilename();
  
  MESSAGE("myFaceEntry = "<< myFaceEntry.toStdString());
  std::map< std::string , std::vector<Handle(AIS_InteractiveObject)> >::iterator AISit;
  SOCC_Viewer* soccViewer = (SOCC_Viewer*)(theViewWindow->getViewManager()->getViewModel());
  
  MESSAGE("repere1")
  AISit = soccViewer->entry2aisobjects.find(myFaceEntry.toStdString());
  if (AISit == soccViewer->entry2aisobjects.end())
    return res;
  
  Handle(AIS_InteractiveObject) myAIS = (*AISit).second[0];
  Handle(GEOM_AISShape) myAISShape;
  if( myAIS->IsInstance( STANDARD_TYPE(GEOM_AISShape) ) ) {
    myAISShape = Handle(GEOM_AISShape)::DownCast( myAIS );
  }
  else
    return res;
  
  QString theImgFileName = QString::fromStdString( myAISShape->TextureFile() );
   
  
  if ( theImgFileName.isEmpty() )
    return res;
  
  // Build an instance of detection used to perform image processing operations
  OCCViewer_FeatureDetector* aDetector = new OCCViewer_FeatureDetector( theImgFileName );
    
  int height            =  aDetector->GetImgHeight();
  int width             =  aDetector->GetImgWidth();
  int winHeight         =  vp->height();
  int winWidth          =  vp->width();
  double x_offset, y_offset;
  int i;

  // Recompute of the values computed in OCC OpenGl_view.c 
  // while waiting for a function to retrieve parameters of the displayed backgroun image 
  double hratio = winHeight * 1.0 / height;
  double wratio = winWidth  * 1.0 / width ;
  double imgZoomRatio = 1.0;
  
  if (hratio < wratio){ 
    x_offset = 0.5 * width * hratio;
    y_offset = 0.5 * winHeight;
    imgZoomRatio = hratio;
  }
  else {
    y_offset = 0.5 * height * wratio;
    x_offset = 0.5 * winWidth;
    imgZoomRatio = wratio;
  }
  
  // Selection rectangle coordinates in the view
  double rectLeft  = myStartPnt.x(); 
  double rectTop   = myStartPnt.y(); 
  
  // Operations to display the corners properly in the 3D scene
  double viewLeft  = 0.5 * winWidth  - x_offset;  // X coordinate of the top left  corner of the background image in the view
  double viewTop   = 0.5 * winHeight - y_offset;  // Y coordinate of both top corners
  
  // Set detection rectangle in the background image coordinates system and detect the corners
  myStartPnt.setX( (myStartPnt.x() -  (0.5 * winWidth  - x_offset)) * 1.0 / imgZoomRatio );
  myStartPnt.setY( (myStartPnt.y() -  (0.5 * winHeight - y_offset)) * 1.0 / imgZoomRatio );
  myEndPnt.setX(   (myEndPnt.x()   -  (0.5 * winWidth  - x_offset)) * 1.0 / imgZoomRatio );
  myEndPnt.setY(   (myEndPnt.y()   -  (0.5 * winHeight - y_offset)) * 1.0 / imgZoomRatio );
  
  QRect aRect = QRect(myStartPnt, myEndPnt);
  
  
  GEOM::GEOM_IBasicOperations_var  aBasicOperations  = myGeomGUI->GetGeomGen()->GetIBasicOperations( getStudyId() );
  GEOM::GEOM_IShapesOperations_var aShapesOperations = GEOM::GEOM_IShapesOperations::_narrow( getOperation() );
  if (myConstructorId == CORNERS)
  {
    if( !aRect.isEmpty() )
    {
      aDetector->SetROI( aRect );
      viewLeft  =  rectLeft;                
      viewTop   =  rectTop;
    }
    aDetector->ComputeCorners();
    CvPoint2D32f* corners     = aDetector->GetCorners();
    int cornerCount           = aDetector->GetCornerCount();
    
    // Build the geom objects associated to the detected corners and returned by execute   
    if( !aBasicOperations->_is_nil() && !aShapesOperations->_is_nil() ) 
    {
      GEOM::GEOM_Object_var  aGeomCorner;
      GEOM::ListOfGO_var     geomCorners = new GEOM::ListOfGO();
      geomCorners->length( cornerCount );
      for (i = 0; i < cornerCount; i++)
      {
//         gp_Pnt aCornerPnt = EntityGUI::ConvertClickToPoint( viewLeft + corners[i].x*imgZoomRatio,
//                                                             viewTop  + corners[i].y*imgZoomRatio, vp->getView() );
//         
//         double x = aCornerPnt.X();
//         double y = aCornerPnt.Y();
//         double z = aCornerPnt.Z();

        // When using the new way with textures on shapes we just have to do the following
        double x = -0.5*width  + corners[i].x;
        double y =  0.5*height - corners[i].y;
        double z =  0;
        
        aGeomCorner = aBasicOperations->MakePointXYZ( x,y,z );
        
        geomCorners[i] = aGeomCorner;  
      } 
      GEOM::GEOM_Object_var aCompound = aShapesOperations->MakeCompound(geomCorners);    
      if ( !aCompound->_is_nil() )
      {
        // Dark blue color
        SALOMEDS::Color aColor;
        aColor.R = 0;
        aColor.G = 0;
        aColor.B = 0.8;
        
        aCompound->SetColor(aColor);
        aCompound->SetMarkerStd(GEOM::MT_POINT,GEOM::MS_30);
        objects.push_back( aCompound._retn() );
        res = true;
      }
    }
  }
  else if (myConstructorId == CONTOURS)
  {
    if( !aRect.isEmpty() )
    {
      aDetector->SetROI( aRect );
    }
    
    GEOM::GEOM_ICurvesOperations_var aCurveOperations = myGeomGUI->GetGeomGen()->GetICurvesOperations( getStudyId() );
//     GEOM::GEOM_ICurvesOperations::_narrow( getOperation() );
    
    aDetector->ComputeContours();
    std::vector< std::vector<cv::Point> >   contours  = aDetector->GetContours();
    std::vector<cv::Vec4i>                  hierarchy = aDetector->GetContoursHierarchy();
    
    std::vector< cv::Point >                contour;
    int idx = 0;
    
    GEOM::ListOfGO_var                      geomContours = new GEOM::ListOfGO();
    int contourCount = 0;
    
    MESSAGE("hierarchy.size() =" << hierarchy.size()) 
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
      contour = contours[idx];
      std::vector< cv::Point >::iterator it;
      GEOM::GEOM_Object_var  aGeomContourPnt;
      GEOM::ListOfGO_var     geomContourPnts = new GEOM::ListOfGO();
      geomContourPnts->length( contour.size() );

      int j = 0;
      std::set< std::vector<int> > existing_points;
      std::pair< std::set< std::vector<int> >::iterator,bool > pnt_it;
      for ( it=contour.begin() ; it < contour.end(); it++ )
      {
//         gp_Pnt  aContourPnt = EntityGUI::ConvertClickToPoint(viewLeft + it->x*imgZoomRatio, viewTop + it->y*imgZoomRatio, vp->getView());
//         double x = aContourPnt.X();
//         double y = aContourPnt.Y();
//         double z = aContourPnt.Z();
        
         // When using the new way with textures on shapes we just have to do the following
//         double pnt_array[] = {it->x,it->y}; 
//         std::vector<int> pnt (pnt_array, pnt_array + sizeof(pnt_array) / sizeof(double) );
// 
//         pnt_it=existing_points.insert(pnt);
//         if (pnt_it.second == true)
//         {
//           MESSAGE("point absent du contour insere")
        double x = -0.5*width  + it->x;
        double y = 0.5 *height - it->y;
        double z = 0;
        aGeomContourPnt    = aBasicOperations->MakePointXYZ( x,y,z );
        geomContourPnts[j] = aGeomContourPnt;
        j++;
//         }
      }
      GEOM::GEOM_Object_var aWire = aCurveOperations->MakePolyline(geomContourPnts.in(), false);
//       GEOM::GEOM_Object_var aContourCompound = aShapesOperations->MakeCompound(geomContourPnts);
//       GEOM::GEOM_Object_var aWire = aCurveOperations->MakeSplineInterpolation(geomContourPnts.in(), false, true);
      if ( !aWire->_is_nil() )
      {
        geomContours->length(contourCount + 1);
        geomContours[contourCount] = aWire;
        contourCount++;
//         objects.push_back( aWire._retn() );
      }
//        if ( !aContourCompound->_is_nil() )
//       {
//         geomContours->length(contourCount + 1);
//         geomContours[contourCount] = aContourCompound;
//         contourCount++;
// //         objects.push_back( aWire._retn() );
//       }
    }
    GEOM::GEOM_Object_var aContoursCompound = aShapesOperations->MakeCompound(geomContours);
    if ( !aContoursCompound->_is_nil() )
    {
      objects.push_back( aContoursCompound._retn() );
    }

    res=true;
  }
  
  return res;
}
