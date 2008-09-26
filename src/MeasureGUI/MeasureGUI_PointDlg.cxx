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
// File   : MeasureGUI_PointDlg.cxx
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//

#include "MeasureGUI_PointDlg.h"
#include "MeasureGUI_Widgets.h"

#include <GEOMBase.h>
#include <DlgRef.h>

#include <GeometryGUI.h>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <gp_Pnt.hxx>

//=================================================================================
// class    : MeasureGUI_PointDlg()
// purpose  : Constructs a MeasureGUI_PointDlg which is a child of 'parent'
//            
//=================================================================================
MeasureGUI_PointDlg::MeasureGUI_PointDlg( GeometryGUI* GUI, QWidget* parent )
  : MeasureGUI_Skeleton( GUI, parent )
{
  QPixmap iconPnt( SUIT_Session::session()->resourceMgr()->loadPixmap(
    "GEOM", tr( "ICON_DLG_POINT" ) ) );
  
  QPixmap iconSelect( SUIT_Session::session()->resourceMgr()->loadPixmap(
    "GEOM", tr( "ICON_SELECT" ) ) );

  setWindowTitle( tr( "CAPTION" ) );

  mainFrame()->GroupConstructors->setTitle( tr( "CAPTION" ) );
  mainFrame()->RadioButton1->setIcon( iconPnt );

  myGrp = new MeasureGUI_1Sel3LineEdit( centralWidget() );
  myGrp->TextLabel1->setText( tr( "POINT" ) );
  myGrp->PushButton1->setIcon( iconSelect );
  myGrp->LineEdit1->setReadOnly( true );
  myGrp->TextLabel2->setText( tr( "X" ) );
  myGrp->TextLabel3->setText( tr( "Y" ) );
  myGrp->TextLabel4->setText( tr( "Z" ) );
  myGrp->LineEdit2->setReadOnly( true );
  myGrp->LineEdit3->setReadOnly( true );
  myGrp->LineEdit4->setReadOnly( true );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( myGrp );

  myHelpFileName = "using_measurement_tools_page.html#point_coord_anchor";

  Init();
}


//=================================================================================
// function : ~MeasureGUI_PointDlg()
// purpose  : Destroys the object and frees any allocated resources
//======================myGrp->LineEdit2->setReadOnly( true );===========================================================
MeasureGUI_PointDlg::~MeasureGUI_PointDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_PointDlg::Init()
{
  QSize aSize( size() );
  resize( (int)(aSize.width() *0.75 ), aSize.height() );
  MeasureGUI_Skeleton::Init();
}

//=================================================================================
// function : activateSelection
// purpose  :
//=================================================================================
void MeasureGUI_PointDlg::activateSelection()
{
  localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
}

//=================================================================================
// function : SelectionIntoArgument
// purpose  :
//=================================================================================
void MeasureGUI_PointDlg::SelectionIntoArgument()
{
  try {
    myObj = GEOM::GEOM_Object::_nil();
    myGrp->LineEdit1->setText( "" );
    myGrp->LineEdit2->setText( "" );
    myGrp->LineEdit3->setText( "" );
    myGrp->LineEdit4->setText( "" );

    LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
    SALOME_ListIO aSelList;
    aSelMgr->selectedObjects(aSelList);

    if (aSelList.Extent() < 1)
      return;

    Standard_Boolean testResult = Standard_False;
    GEOM::GEOM_Object_var aSelectedObject =
      GEOMBase::ConvertIOinGEOMObject(aSelList.First(), testResult);

    if (!testResult || aSelectedObject->_is_nil())
      return;

    myObj = aSelectedObject;

    TColStd_IndexedMapOfInteger anIndexes;
    aSelMgr->GetIndexes(aSelList.First(), anIndexes);

    TopoDS_Shape aShape;
    if ( anIndexes.Extent() > 1 || !GEOMBase::GetShape( myObj, aShape ) || aShape.IsNull() )
      return;

    TopoDS_Vertex aPoint;
    QString aName = GEOMBase::GetName( myObj );
    if ( anIndexes.Extent() == 0 ) {
      if ( aShape.ShapeType() == TopAbs_VERTEX )
        aPoint = TopoDS::Vertex( aShape );
    }
    else {
      int i = anIndexes( 1 );

      TopTools_IndexedMapOfShape aShapes;
      TopExp::MapShapes( aShape, aShapes );
      
      if ( aShape != aShapes( i ) )
        aName += QString( ":%1" ).arg( i );

      aPoint = TopoDS::Vertex( aShapes( i ) );
    }

    if ( !aPoint.IsNull() ) {
      gp_Pnt aPnt = BRep_Tool::Pnt( aPoint );
      myGrp->LineEdit1->setText( aName );
      myGrp->LineEdit2->setText( DlgRef::PrintDoubleValue( aPnt.X() ) );
      myGrp->LineEdit3->setText( DlgRef::PrintDoubleValue( aPnt.Y() ) );
      myGrp->LineEdit4->setText( DlgRef::PrintDoubleValue( aPnt.Z() ) );
    }
  }
  catch( ... )
  {
    myObj = GEOM::GEOM_Object::_nil();
    myGrp->LineEdit1->setText( "" );
    myGrp->LineEdit2->setText( "" );
    myGrp->LineEdit3->setText( "" );
    myGrp->LineEdit4->setText( "" );
  }
}
