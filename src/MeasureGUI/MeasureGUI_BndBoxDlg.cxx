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
// File   : MeasureGUI_BndBoxDlg.cxx
// Author : Nicolas REJNERI, Open CASCADE S.A.S.
//

#include "MeasureGUI_BndBoxDlg.h"
#include "MeasureGUI_Widgets.h"

#include <GEOMBase.h>

#include <GEOM_Function.hxx>
#include <GEOM_Object.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <BRep_TFace.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Tools.h>

// #include <qlineedit.h>
// #include <qlabel.h>
// #include <qlayout.h>
// #include <qpushbutton.h>
// #include <qradiobutton.h>
// #include <qbuttongroup.h>

//=================================================================================
// class    : MeasureGUI_BndBoxDlg()
// purpose  : Constructs a MeasureGUI_BndBoxDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            true to construct a modal dialog.
//=================================================================================
MeasureGUI_BndBoxDlg::MeasureGUI_BndBoxDlg( GeometryGUI* GUI, QWidget* parent )
  : MeasureGUI_Skeleton( GUI, parent )
{
  QPixmap image0( SUIT_Session::session()->resourceMgr()->loadPixmap(
    "GEOM", tr( "ICON_DLG_BOUNDING_BOX" ) ) );
  QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap(
    "GEOM", tr( "ICON_SELECT" ) ) );

  setWindowTitle( tr( "GEOM_BNDBOX_TITLE" ) );

  /***************************************************************/
  
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_BNDBOX" ) );
  mainFrame()->RadioButton1->setIcon( image0 );

  myGrp = new MeasureGUI_1Sel6LineEdit( centralWidget() );
  myGrp->GroupBox1->setTitle( tr( "GEOM_BNDBOX_OBJDIM" ) );
  myGrp->TextLabel1->setText( tr( "GEOM_OBJECT" ) );
  myGrp->TextLabel2->setText( tr( "GEOM_MIN" ) );
  myGrp->TextLabel3->setText( tr( "GEOM_MAX" ) );
  myGrp->TextLabel4->setText( tr( "GEOM_X" ) );
  myGrp->TextLabel5->setText( tr( "GEOM_Y" ) );
  myGrp->TextLabel6->setText( tr( "GEOM_Z" ) );
  myGrp->LineEdit11->setReadOnly( true );
  myGrp->LineEdit12->setReadOnly( true );
  myGrp->LineEdit21->setReadOnly( true );
  myGrp->LineEdit22->setReadOnly( true );
  myGrp->LineEdit31->setReadOnly( true );
  myGrp->LineEdit32->setReadOnly( true );
  myGrp->PushButton1->setIcon( image1 );
  myGrp->LineEdit1->setReadOnly( true );

  QVBoxLayout* layout = new QVBoxLayout( centralWidget() );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  layout->addWidget( myGrp );
  
  /***************************************************************/

  myHelpFileName = "using_measurement_tools_page.html#bounding_box_anchor";

  /* Initialisation */
  Init();
}


//=================================================================================
// function : ~MeasureGUI_BndBoxDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MeasureGUI_BndBoxDlg::~MeasureGUI_BndBoxDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_BndBoxDlg::Init()
{
  mySelBtn = myGrp->PushButton1;
  mySelEdit = myGrp->LineEdit1;
  MeasureGUI_Skeleton::Init();
}

//=================================================================================
// function : processObject
// purpose  :
//=================================================================================
void MeasureGUI_BndBoxDlg::processObject()
{
  double aXMin, aXMax, aYMin, aYMax, aZMin, aZMax;

  if ( !getParameters( aXMin, aXMax, aYMin, aYMax, aZMin, aZMax ) ) {
    mySelEdit->setText( "" );
    myGrp->LineEdit11->setText( "" );
    myGrp->LineEdit12->setText( "" );
    myGrp->LineEdit21->setText( "" );
    myGrp->LineEdit22->setText( "" );
    myGrp->LineEdit31->setText( "" );
    myGrp->LineEdit32->setText( "" );
  }
  else {
    myGrp->LineEdit11->setText( tr( "%1" ).arg( aXMin, 12, 'f', 6 ) );
    myGrp->LineEdit12->setText( tr( "%1" ).arg( aXMax, 12, 'f', 6 ) );

    myGrp->LineEdit21->setText( tr( "%1" ).arg( aYMin, 12, 'f', 6 ) );
    myGrp->LineEdit22->setText( tr( "%1" ).arg( aYMax, 12, 'f', 6 ) );

    myGrp->LineEdit31->setText( tr( "%1" ).arg( aZMin, 12, 'f', 6 ) );
    myGrp->LineEdit32->setText( tr( "%1" ).arg( aZMax, 12, 'f', 6 ) );            
  }
}

//=================================================================================
// function : getParameters
// purpose  :
//=================================================================================
bool MeasureGUI_BndBoxDlg::getParameters( double& theXmin, double& theXmax,
                                          double& theYmin, double& theYmax,
                                          double& theZmin, double& theZmax )
{
  if ( myObj->_is_nil() )
    return false;
  else {
    try {
      Handle(Poly_Triangulation) Trtn = 0; 

      Handle(GEOM_Object) anObject = GEOM_Engine::GetEngine()->GetObject(myObj->GetStudyID(), myObj->GetEntry());
      if (!anObject.IsNull())
      {
	Handle(GEOM_Function) aRefShape = anObject->GetLastFunction();
	if (!aRefShape.IsNull())
	{
	  TopoDS_Shape aShape = aRefShape->GetValue();
	  if (!aShape.IsNull())
	  {
	    TopLoc_Location l;
	    Handle(Poly_Triangulation) T;
	    TopExp_Explorer ex;
	    for (ex.Init(aShape,TopAbs_FACE); ex.More(); ex.Next()) {
	      const TopoDS_Face& F = TopoDS::Face(ex.Current());
	      
	      BRepAdaptor_Surface surf(F);
	      if (surf.GetType() == GeomAbs_Sphere)
	      {
		T = BRep_Tool::Triangulation(F, l);
		if (!T.IsNull()) {
		  Handle(Poly_Triangulation) NullTrtn = 0;
		  (*((Handle(BRep_TFace)*)&F.TShape()))->Triangulation(NullTrtn);
		  Trtn = T;
		  break;
		}
	      }
	      else
		break;
	    }
	  }
	}
      }

      GEOM::GEOM_IMeasureOperations::_narrow( getOperation() )->GetBoundingBox(
        myObj, theXmin, theXmax, theYmin, theYmax, theZmin, theZmax );
      
      if (!Trtn.IsNull())
      {
	Handle(GEOM_Function) aRefShape = anObject->GetLastFunction();
	if (!aRefShape.IsNull())
	{
	  TopoDS_Shape aShape = aRefShape->GetValue();
	  if (!aShape.IsNull())
	  {
	    TopLoc_Location l;
	    Handle(Poly_Triangulation) T;
	    TopExp_Explorer ex;
	    for (ex.Init(aShape,TopAbs_FACE); ex.More(); ex.Next()) {
	      const TopoDS_Face& F = TopoDS::Face(ex.Current());
	      (*((Handle(BRep_TFace)*)&F.TShape()))->Triangulation(Trtn);
	      break;
	    }
	  }
	}
      }

    }
    catch( const SALOME::SALOME_Exception& e ) {
      SalomeApp_Tools::QtCatchCorbaException( e );
      return false;
    }

    return getOperation()->IsDone();
  }
}

//=================================================================================
// function : buildPrs
// purpose  :
//=================================================================================
SALOME_Prs* MeasureGUI_BndBoxDlg::buildPrs()
{
  double aXMin, aYMin, aZMin, aXMax, aYMax, aZMax;

  if ( myObj->_is_nil() || !getParameters( aXMin, aXMax, aYMin, aYMax, aZMin, aZMax ) )
    return 0;

  TopoDS_Shape aShape = BRepPrimAPI_MakeBox( gp_Pnt( aXMin, aYMin, aZMin ),
                                             gp_Pnt( aXMax, aYMax, aZMax ) ).Shape();
       
  return !aShape.IsNull() ? getDisplayer()->BuildPrs( aShape ) : 0;

}
