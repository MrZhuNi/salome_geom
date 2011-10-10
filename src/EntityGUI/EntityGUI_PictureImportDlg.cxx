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
// File   : EntityGUI_PictureImportDlg.cxx
// Author : Renaud NEDELEC, Open CASCADE S.A.S.
//
#include "EntityGUI_PictureImportDlg.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <GEOMImpl_Types.hxx>

#include <OCCViewer_FeatureDetector.h>


#include <QFileDialog>

//=================================================================================
// class    : EntityGUI_PictureImportDlg()
// purpose  : Constructs a EntityGUI_PictureImportDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
EntityGUI_PictureImportDlg::EntityGUI_PictureImportDlg( GeometryGUI* theGeometryGUI, QWidget* parent,
                                    bool modal, Qt::WindowFlags fl )
  : GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl )
{
//   QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_LINE_2P" ) ) );
//   QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );
//   QPixmap image2( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_DLG_LINE_2F" ) ) );
// 
  setWindowTitle( tr( "GEOM_IMPORT_PICT_TITLE" ) );
//  
//   /***************************************************************/
//   mainFrame()->GroupConstructors->setTitle( tr( "GEOM_LINE" ) );
//   mainFrame()->RadioButton1->setIcon( image0 );
//   mainFrame()->RadioButton2->setIcon( image2 );

  mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();

  QGroupBox* GroupFileSel = new QGroupBox( centralWidget() );
  QHBoxLayout* myFileSelGrpLayout = new QHBoxLayout(GroupFileSel );
  myPushButton = new QPushButton( GroupFileSel );
  myLineEdit = new QLineEdit( GroupFileSel );
  myPushButton->setText("...");
  
  myFileSelGrpLayout->addWidget( myLineEdit, 1);
  myFileSelGrpLayout->addWidget( myPushButton, 0);
  
  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( GroupFileSel );

//   setHelpFileName( "create_line_page.html" );

  Init();
}


//=================================================================================
// function : ~EntityGUI_PictureImportDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
EntityGUI_PictureImportDlg::~EntityGUI_PictureImportDlg()
{  
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void EntityGUI_PictureImportDlg::Init()
{
//   /* init variables */
//   myEditCurrentArgument = GroupPoints->LineEdit1;
//   myPoint1.nullify();
//   myPoint2.nullify();
//   myFace1.nullify();
//   myFace2.nullify();
// 
//   GroupPoints->PushButton1->setDown(true);
        
  /* signals and slots connections */
//   connect( myGeomGUI, SIGNAL( SignalDeactivateActiveDialog() ), this, SLOT( DeactivateActiveDialog() ) );
//   connect( myGeomGUI, SIGNAL( SignalCloseAllDialogs() ),        this, SLOT( ClickOnCancel() ) );
// 
//   connect( this,           SIGNAL( constructorsClicked( int ) ), this, SLOT( ConstructorsClicked( int ) ) );
// 
  connect( buttonOk(),     SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(),  SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );
// 
  connect( myPushButton, SIGNAL( clicked() ),       this, SLOT( FileSelectionClicked() ) );
//   connect( GroupPoints->PushButton2, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
//   connect( GroupFaces->PushButton1,  SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
//   connect( GroupFaces->PushButton2,  SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
// 
//   connect( myGeomGUI->getApp()->selectionMgr(), SIGNAL( currentSelectionChanged() ),
//            this, SLOT( SelectionIntoArgument() ) );
  
  resize(100,100);
  
  gp_Pnt aOrigin = gp_Pnt(0, 0, 0);
  gp_Dir aDirZ = gp_Dir(0, 0, 1);
  gp_Dir aDirX = gp_Dir(1, 0, 0);
  
  gp_Ax3 aGlobalCS = gp_Ax3(aOrigin, aDirZ, aDirX);
  
  myGeomGUI->SetWorkingPlane( aGlobalCS );
  myGeomGUI->ActiveWorkingPlane();
  
  initName( tr( "GEOM_PICTURE" ) );

}


// =================================================================================
// function : FileSelectionClicked()
// purpose  : 
// =================================================================================
void EntityGUI_PictureImportDlg::FileSelectionClicked()
{
  QString selFile = QFileDialog::getOpenFileName(this,tr( "SELECT_IMAGE"),"/home", tr("OCC_IMAGE_FILES"));
  if ( ! selFile.isEmpty() )
    myLineEdit->setText(selFile);
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void EntityGUI_PictureImportDlg::ClickOnOk()
{
  MESSAGE("EntityGUI_PictureImportDlg::ClickOnOk()")
  setIsApplyAndClose( true );
  if ( ClickOnApply() )
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool EntityGUI_PictureImportDlg::ClickOnApply()
{
  MESSAGE("EntityGUI_PictureImportDlg::ClickOnApply()")
  if ( !onAccept() )
    return false;
  
  initName();

  myLineEdit->setText( "" );
//   ConstructorsClicked( getConstructorId() );

  return true;
}
// 
// //=================================================================================
// // function : ConstructorsClicked()
// // purpose  : Radio button management
// //=================================================================================
// void EntityGUI_PictureImportDlg::ConstructorsClicked( int constructorId )
// {  
//   switch ( constructorId ) {
//   case 0:
//     {
//       myEditCurrentArgument = GroupPoints->LineEdit1;
//       myEditCurrentArgument->setText( "" );
//       GroupPoints->LineEdit2->setText( "" );
//       myPoint1.nullify();
//       myPoint2.nullify();
//       GroupPoints->PushButton1->setDown(true);
//       GroupPoints->PushButton2->setDown(false);
//       GroupPoints->LineEdit1->setEnabled(true);
//       GroupPoints->LineEdit2->setEnabled(false);
//       GroupPoints->show();
//       GroupFaces->hide();
//       break;
//     }
//   case 1:
//     {
//       myEditCurrentArgument = GroupFaces->LineEdit1;
//       myEditCurrentArgument->setText("");
//       myFace1.nullify();
//       myFace2.nullify();
//       GroupFaces->PushButton1->setDown(true);
//       GroupFaces->PushButton2->setDown(false);
//       GroupFaces->LineEdit1->setEnabled(true);
//       GroupFaces->LineEdit2->setEnabled(false);
//       GroupPoints->hide();
//       GroupFaces->show();
//       break;
//     }
//   }
// 
//   TopAbs_ShapeEnum aNeedType = ( myEditCurrentArgument == GroupFaces->LineEdit1 ||
// 				 myEditCurrentArgument == GroupFaces->LineEdit2 ) ?
//     TopAbs_FACE : TopAbs_VERTEX;
//   globalSelection(); // close local selection to clear it
//   localSelection( GEOM::GEOM_Object::_nil(), aNeedType );
// 
//   qApp->processEvents();
//   updateGeometry();
//   resize( minimumSizeHint() );
//   SelectionIntoArgument();
// }
// 
// //=================================================================================
// // function : SelectionIntoArgument()
// // purpose  : Called when selection as changed or other case
// //=================================================================================
// void EntityGUI_PictureImportDlg::SelectionIntoArgument()
// {
//   myEditCurrentArgument->setText( "" );
// 
//   LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
//   SALOME_ListIO aSelList;
//   aSelMgr->selectedObjects(aSelList);
// 
//   if (aSelList.Extent() != 1) {
//     if      (myEditCurrentArgument == GroupPoints->LineEdit1) myPoint1.nullify();
//     else if (myEditCurrentArgument == GroupPoints->LineEdit2) myPoint2.nullify();
//     else if (myEditCurrentArgument == GroupFaces->LineEdit1)  myFace1.nullify();
//     else if (myEditCurrentArgument == GroupFaces->LineEdit2)  myFace2.nullify();
//     displayPreview(true);
//     return;
//   }
// 
//   TopAbs_ShapeEnum aNeedType = ( myEditCurrentArgument == GroupFaces->LineEdit1 ||
// 				 myEditCurrentArgument == GroupFaces->LineEdit2 ) ?
//     TopAbs_FACE : TopAbs_VERTEX;
//   GEOM::GeomObjPtr aSelectedObject = getSelected( aNeedType );
//   TopoDS_Shape aShape;
//   if ( aSelectedObject && GEOMBase::GetShape( aSelectedObject.get(), aShape ) && !aShape.IsNull() ) {
//     QString aName = GEOMBase::GetName( aSelectedObject.get() );
//     myEditCurrentArgument->setText( aName );
//     if ( myEditCurrentArgument == GroupPoints->LineEdit1 ) {
//       myPoint1 = aSelectedObject;
//       if ( myPoint1 && !myPoint2 )
//         GroupPoints->PushButton2->click();
//     }
//     else if ( myEditCurrentArgument == GroupPoints->LineEdit2 ) {
//       myPoint2 = aSelectedObject;
//       if ( myPoint2 && !myPoint1 )
//         GroupPoints->PushButton1->click();
//     }
//     else if ( myEditCurrentArgument == GroupFaces->LineEdit1 ) {
//       myFace1 = aSelectedObject;
//       if ( myFace1 && !myFace2 )
//         GroupFaces->PushButton2->click();
//     }
//     else if ( myEditCurrentArgument == GroupFaces->LineEdit2 ) {
//       myFace2 = aSelectedObject;
//       if ( myFace2 && !myFace1 )
//         GroupFaces->PushButton1->click();      
//     }
//   }
//   
//   displayPreview(true);
// }

// //=================================================================================
// // function : SetEditCurrentArgument()
// // purpose  :
// //=================================================================================
// void EntityGUI_PictureImportDlg::SetEditCurrentArgument()
// {
//   QPushButton* send = (QPushButton*)sender();
//   if ( send == GroupPoints->PushButton1 ) {
//     myEditCurrentArgument = GroupPoints->LineEdit1;
//     GroupPoints->PushButton2->setDown(false);
//     GroupPoints->LineEdit1->setEnabled(true);
//     GroupPoints->LineEdit2->setEnabled(false);
//   }
//   else if ( send == GroupPoints->PushButton2 ) {
//     myEditCurrentArgument = GroupPoints->LineEdit2;
//     GroupPoints->PushButton1->setDown(false);
//     GroupPoints->LineEdit1->setEnabled(false);
//     GroupPoints->LineEdit2->setEnabled(true);
//   }
//   else if ( send == GroupFaces->PushButton1 ) {
//     myEditCurrentArgument = GroupFaces->LineEdit1;
//     GroupFaces->PushButton2->setDown(false);
//     GroupFaces->LineEdit1->setEnabled(true);
//     GroupFaces->LineEdit2->setEnabled(false);
//   }
//   else if ( send == GroupFaces->PushButton2 ) {
//     myEditCurrentArgument = GroupFaces->LineEdit2;
//     GroupFaces->PushButton1->setDown(false);
//     GroupFaces->LineEdit1->setEnabled(false);
//     GroupFaces->LineEdit2->setEnabled(true);
//   }
// 
//   TopAbs_ShapeEnum aNeedType = ( myEditCurrentArgument == GroupFaces->LineEdit1 ||
// 				 myEditCurrentArgument == GroupFaces->LineEdit2 ) ?
//     TopAbs_FACE : TopAbs_VERTEX;
//   globalSelection(); // close local selection to clear it
//   localSelection( GEOM::GEOM_Object::_nil(), aNeedType );
// 
//   myEditCurrentArgument->setFocus();
//   //  SelectionIntoArgument();
//   send->setDown(true);
//   displayPreview(true);
// }
// 
// //=================================================================================
// // function : ActivateThisDialog()
// // purpose  :
// //=================================================================================
// void EntityGUI_PictureImportDlg::ActivateThisDialog()
// {
//   GEOMBase_Skeleton::ActivateThisDialog();
//   connect( myGeomGUI->getApp()->selectionMgr(), SIGNAL( currentSelectionChanged() ),
//            this, SLOT( SelectionIntoArgument() ) );
// 
//   ConstructorsClicked( getConstructorId() );
// }
// 
// //=================================================================================
// // function : DeactivateActiveDialog()
// // purpose  : public slot to deactivate if active
// //=================================================================================
// void EntityGUI_PictureImportDlg::DeactivateActiveDialog()
// {
//   GEOMBase_Skeleton::DeactivateActiveDialog();
// }
// 
// //=================================================================================
// // function : enterEvent()
// // purpose  :
// //=================================================================================
// void EntityGUI_PictureImportDlg::enterEvent( QEvent* )
// {
//   if ( !mainFrame()->GroupConstructors->isEnabled() )
//     ActivateThisDialog();
// }

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr EntityGUI_PictureImportDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetIBlocksOperations( getStudyId() );
}
// 
// //=================================================================================
// // function : isValid
// // purpose  :
// //=================================================================================
// bool EntityGUI_PictureImportDlg::isValid( QString& msg )
// {
//   bool ok = false;
//   switch ( getConstructorId() ) {
//   case 0 :
//     ok = myPoint1 && myPoint2;
//     break;
//   case 1:
//     ok = myFace1 && myFace2;
//     break;
//   default:
//     break;
//   }
//   return ok;
// }

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool EntityGUI_PictureImportDlg::execute( ObjectList& objects )
{
  MESSAGE("EntityGUI_PictureImportDlg::execute()")
  bool res = false;
  GEOM::GEOM_Object_var anObj;
  GEOM::GEOM_IBasicOperations_var aBasicOperations = myGeomGUI->GetGeomGen()->GetIBasicOperations( getStudyId() );
  GEOM::GEOM_IBlocksOperations_var aBlocksOperations = GEOM::GEOM_IBlocksOperations::_narrow( getOperation() );
  QString theImgFileName = myLineEdit->text();
  if ( theImgFileName.isEmpty() )
    return res;
  
  OCCViewer_FeatureDetector* aDetector = new OCCViewer_FeatureDetector( theImgFileName );
    
  int height            =  aDetector->GetImgHeight();
  int width             =  aDetector->GetImgWidth();
  
  
  GEOM::GEOM_Object_var P1 = aBasicOperations->MakePointXYZ( -0.5*width, -0.5*height,0 );
  GEOM::GEOM_Object_var P2 = aBasicOperations->MakePointXYZ( -0.5*width,  0.5*height,0 );
  GEOM::GEOM_Object_var P3 = aBasicOperations->MakePointXYZ(  0.5*width,  0.5*height,0 );
  GEOM::GEOM_Object_var P4 = aBasicOperations->MakePointXYZ(  0.5*width, -0.5*height,0 );
  
  GEOM::GEOM_Object_var aFace = aBlocksOperations->MakeQuad4Vertices(P1,P2,P3,P4);
  getDisplayer()->SetTexture(theImgFileName.toStdString());
  getDisplayer()->SetDisplayMode( 3 );
//     OCCViewer_Viewer*              anOCCViewer =((OCCViewer_ViewWindow*)theViewWindow)->getViewManager())->getOCCViewer();
//     Handle(AIS_InteractiveContext) aContext = anOCCViewer->getAISContext(); 
  
  MESSAGE("EntityGUI_FeatureDetectorDlg::execute() theImgFileName = "<<theImgFileName.toStdString());
  if ( !aFace->_is_nil() )
  {
    objects.push_back( aFace._retn() );
  }
  
  res=true;
 

  return res;
}

// //=================================================================================
// // function : addSubshapeToStudy
// // purpose  : virtual method to add new SubObjects if local selection
// //=================================================================================
// void EntityGUI_PictureImportDlg::addSubshapesToStudy()
// {
//   switch ( getConstructorId() ) {
//   case 0 :
//     GEOMBase::PublishSubObject( myPoint1.get() );
//     GEOMBase::PublishSubObject( myPoint2.get() );
//     break;
//   case 1 :
//     GEOMBase::PublishSubObject( myFace1.get() );
//     GEOMBase::PublishSubObject( myFace2.get() );
//     break;
//   default:
//     break;
//   }
// }
