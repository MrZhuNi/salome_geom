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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : MeasureGUI_InertiaDlg.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM

#include "MeasureGUI_InertiaDlg.h"
#include "MeasureGUI_1Sel12LineEdit_QTD.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "GEOMBase.h"

#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <GProp_PrincipalProps.hxx>
#include <gp_XYZ.hxx>
#include <TopoDS_Shape.hxx>

#include "utilities.h"
#include "QAD_Desktop.h"

#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlabel.h>

//=================================================================================
// class    : MeasureGUI_InertiaDlg()
// purpose  : Constructs a MeasureGUI_InertiaDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================

MeasureGUI_InertiaDlg::MeasureGUI_InertiaDlg( QWidget* parent, SALOME_Selection* Sel )
: MeasureGUI_Skeleton( parent, "MeasureGUI_InertiaDlg", Sel )
{
  QPixmap image0( QAD_Desktop::getResourceManager()->loadPixmap(
    "GEOM",tr( "ICON_DLG_INERTIA" ) ) );
  QPixmap image1( QAD_Desktop::getResourceManager()->loadPixmap(
    "GEOM",tr( "ICON_SELECT" ) ) );

  setCaption( tr( "GEOM_INERTIA_TITLE" ) );

  /***************************************************************/
  
  GroupConstructors->setTitle( tr( "GEOM_INERTIA_CONSTR" ) );
  RadioButton1->setPixmap( image0 );

  myGrp = new MeasureGUI_1Sel12LineEdit_QTD( this, "myGrp" );
  myGrp->GroupBox1->setTitle( tr( "GEOM_MATRIX" ) );
  myGrp->TextLabel1->setText( tr( "GEOM_OBJECT" ) );
  myGrp->TextLabel2->setText( tr( "GEOM_INERTIA_I" ).arg( "1" ) );
  myGrp->TextLabel3->setText( tr( "GEOM_INERTIA_I" ).arg( "2" ) );
  myGrp->TextLabel4->setText( tr( "GEOM_INERTIA_I" ).arg( "3" ) );
  myGrp->TextLabel5->setText( tr( "GEOM_INERTIA_IXYZ" ) );
  myGrp->LineEdit11->setReadOnly( TRUE );
  myGrp->LineEdit12->setReadOnly( TRUE );
  myGrp->LineEdit13->setReadOnly( TRUE );
  myGrp->LineEdit21->setReadOnly( TRUE );
  myGrp->LineEdit22->setReadOnly( TRUE );
  myGrp->LineEdit23->setReadOnly( TRUE );
  myGrp->LineEdit31->setReadOnly( TRUE );
  myGrp->LineEdit32->setReadOnly( TRUE );
  myGrp->LineEdit33->setReadOnly( TRUE );
  myGrp->LineEdit41->setReadOnly( TRUE );
  myGrp->LineEdit42->setReadOnly( TRUE );
  myGrp->LineEdit43->setReadOnly( TRUE );
  myGrp->PushButton1->setPixmap( image1 );
  myGrp->LineEdit1->setReadOnly( true );

  Layout1->addWidget( myGrp, 1, 0 );
  
  /***************************************************************/

  /* Initialisation */
  Init( Sel );
}


//=================================================================================
// function : ~MeasureGUI_InertiaDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MeasureGUI_InertiaDlg::~MeasureGUI_InertiaDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_InertiaDlg::Init( SALOME_Selection* Sel )
{
  mySelBtn = myGrp->PushButton1;
  mySelEdit = myGrp->LineEdit1;
  MeasureGUI_Skeleton::Init( Sel );
}

//=================================================================================
// function : processObject
// purpose  :
//=================================================================================
void MeasureGUI_InertiaDlg::processObject()
{
  gp_XYZ anIXYZ;
  gp_Mat aMat;

  if ( !getParameters( aMat, anIXYZ ) )
  {
    mySelEdit->setText( "" );
    
    myGrp->LineEdit11->setText( "" );
    myGrp->LineEdit12->setText( "" );
    myGrp->LineEdit13->setText( "" );
    
    myGrp->LineEdit21->setText( "" );
    myGrp->LineEdit22->setText( "" );
    myGrp->LineEdit23->setText( "" );
    
    myGrp->LineEdit31->setText( "" );
    myGrp->LineEdit32->setText( "" );
    myGrp->LineEdit33->setText( "" );

    myGrp->LineEdit41->setText( "" );
    myGrp->LineEdit42->setText( "" );
    myGrp->LineEdit43->setText( "" );
  }
  else
  {
    myGrp->LineEdit11->setText( QString( "%1" ).arg( aMat( 1, 1 ), 12, 'e', 4 ) );
    myGrp->LineEdit12->setText( QString( "%1" ).arg( aMat( 1, 2 ), 12, 'e', 4 ) );
    myGrp->LineEdit13->setText( QString( "%1" ).arg( aMat( 1, 3 ), 12, 'e', 4 ) );
    
    myGrp->LineEdit21->setText( QString( "%1" ).arg( aMat( 2, 1 ), 12, 'e', 4 ) );
    myGrp->LineEdit22->setText( QString( "%1" ).arg( aMat( 2, 2 ), 12, 'e', 4 ) );
    myGrp->LineEdit23->setText( QString( "%1" ).arg( aMat( 2, 3 ), 12, 'e', 4 ) );
    
    myGrp->LineEdit31->setText( QString( "%1" ).arg( aMat( 3, 1 ), 12, 'e', 4 ) );
    myGrp->LineEdit32->setText( QString( "%1" ).arg( aMat( 3, 2 ), 12, 'e', 4 ) );
    myGrp->LineEdit33->setText( QString( "%1" ).arg( aMat( 3, 3 ), 12, 'e', 4 ) );
    
    myGrp->LineEdit41->setText( QString( "%1" ).arg( anIXYZ.X(), 12, 'e', 4 ) );
    myGrp->LineEdit42->setText( QString( "%1" ).arg( anIXYZ.Y(), 12, 'e', 4 ) );
    myGrp->LineEdit43->setText( QString( "%1" ).arg( anIXYZ.Z(), 12, 'e', 4 ) );
  }
}

//=================================================================================
// function : getParameters
// purpose  :
//=================================================================================
bool MeasureGUI_InertiaDlg::getParameters( gp_Mat& I,
                                           gp_XYZ& theIXYZ )
{
  if ( myObj->_is_nil() )
    return false;
  else
  {
    try
    {
      double x, y, z;
      GEOM::GEOM_IMeasureOperations::_narrow( getOperation() )->GetInertia( myObj,
        I( 1, 1 ), I( 1, 2 ), I( 1, 3 ),
        I( 2, 1 ), I( 2, 2 ), I( 2, 3 ),
        I( 3, 1 ), I( 3, 2 ), I( 3, 3 ),
        x, y, z );

        theIXYZ.SetCoord( x, y, z );
    }
    catch( const SALOME::SALOME_Exception& e )
    {
      QtCatchCorbaException( e );
      return false;
    }

    return getOperation()->IsDone();
  }
}

