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
  setWindowTitle( tr( "GEOM_IMPORT_PICT_TITLE" ) );
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_PICTURE" ) );
//   mainFrame()->RadioButton1->setIcon( image0 );
//   mainFrame()->RadioButton2->setIcon( image2 );

  mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();

  QGroupBox* GroupFileSel = new QGroupBox( centralWidget() );
  GroupFileSel->setTitle( tr( "GEOM_FILE" ) );
  QHBoxLayout* myFileSelGrpLayout = new QHBoxLayout(GroupFileSel );
  myPushButton = new QPushButton( GroupFileSel );
  myLineEdit = new QLineEdit( GroupFileSel );
  myPushButton->setText("...");
  
  myFileSelGrpLayout->addWidget( myLineEdit, 1);
  myFileSelGrpLayout->addWidget( myPushButton, 0);
  
  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( GroupFileSel );

//   setHelpFileName( "import_picture_page.html" );

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
  connect( buttonOk(),     SIGNAL( clicked() ), this, SLOT( ClickOnOk() ) );
  connect( buttonApply(),  SIGNAL( clicked() ), this, SLOT( ClickOnApply() ) );
  connect( myPushButton,   SIGNAL( clicked() ), this, SLOT( FileSelectionClicked() ) );
  
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

  return true;
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr EntityGUI_PictureImportDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetIBlocksOperations( getStudyId() );
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool EntityGUI_PictureImportDlg::execute( ObjectList& objects )
{
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
  
  
  GEOM::GEOM_Object_var P1 = aBasicOperations->MakePointXYZ( -0.5*width, -0.5*height, 0 );
  GEOM::GEOM_Object_var P2 = aBasicOperations->MakePointXYZ( -0.5*width,  0.5*height, 0 );
  GEOM::GEOM_Object_var P3 = aBasicOperations->MakePointXYZ(  0.5*width,  0.5*height, 0 );
  GEOM::GEOM_Object_var P4 = aBasicOperations->MakePointXYZ(  0.5*width, -0.5*height, 0 );
  
  GEOM::GEOM_Object_var aFace = aBlocksOperations->MakeQuad4Vertices(P1,P2,P3,P4);
  getDisplayer()->SetTexture(theImgFileName.toStdString());
  
  if ( !aFace->_is_nil() )
  {
    objects.push_back( aFace._retn() );
  }
  
  res=true;
 

  return res;
}

