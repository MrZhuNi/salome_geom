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
//  File   : MeasureGUI_CheckCompoundOfBlocksDlg.cxx
//  Author : VKN
//  Module : GEOM
//  $Header$

#include "MeasureGUI_CheckCompoundOfBlocksDlg.h"
#include "MeasureGUI_1Sel1TextView_QTD.h"
#include "SALOMEGUI_QtCatchCorbaException.hxx"

#include "utilities.h"
#include "QAD_Desktop.h"

#include <qtextedit.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
// QT Includes
#include <qgroupbox.h>
#include <qlabel.h>


//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qlistbox.h>
#endif
//VRV: porting on Qt 3.0.5

#define TEXTEDIT_FONT_FAMILY "Courier"
#define TEXTEDIT_FONT_SIZE 11

//=================================================================================
// class    : MeasureGUI_CheckCompoundOfBlocksDlg()
// purpose  : Constructs a MeasureGUI_CheckCompoundOfBlocksDlg which is a child of 'parent', with the
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
MeasureGUI_CheckCompoundOfBlocksDlg::MeasureGUI_CheckCompoundOfBlocksDlg( QWidget* parent, SALOME_Selection* Sel )
: MeasureGUI_Skeleton( parent, "MeasureGUI_CheckCompoundOfBlocksDlg", Sel )
{
  QPixmap image0( QAD_Desktop::getResourceManager()->loadPixmap(
    "GEOM",tr( "ICON_DLG_CHECK_COMPOUND_OF_BLOCKS" ) ) );
  QPixmap image1( QAD_Desktop::getResourceManager()->loadPixmap(
    "GEOM",tr( "ICON_SELECT" ) ) );

  setCaption( tr( "GEOM_CHECK_BLOCKS_COMPOUND" ) );

  /***************************************************************/

  GroupConstructors->setTitle( tr( "GEOM_CHECK_BLOCKS_COMPOUND" ) );
  RadioButton1->setPixmap( image0 );

  myGrp = new MeasureGUI_1Sel1TextView_QTD( this, "myGrp" );
  myGrp->GroupBox1->setTitle( tr( "GEOM_CHECK_INFOS" ) );
  myGrp->TextLabel1->setText( tr( "GEOM_OBJECT" ) );
  myGrp->TextEdit1->setReadOnly( TRUE );
  
  QFont aFont( TEXTEDIT_FONT_FAMILY, TEXTEDIT_FONT_SIZE );
  aFont.setStyleHint( QFont::TypeWriter, QFont::PreferAntialias );
  myGrp->TextEdit1->setFont( aFont );
  myGrp->PushButton1->setPixmap( image1 );
  myGrp->LineEdit1->setReadOnly( true );


  /***************************************************************/
  QGridLayout* aGBLayout = new QGridLayout( myGrp->GroupBox1->layout() );
  aGBLayout->setAlignment( Qt::AlignTop );
  QGridLayout* Layout2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout2"); 
  myErrorsLbl = new QLabel( tr( "GEOM_CHECK_BLOCKS_COMPOUND_ERRORS" ), myGrp, "Errors" );
  Layout2->addWidget( myErrorsLbl, 0, 0 );

  myErrorsLBox = new QListBox( myGrp, "ListBlockCompoundErrors" );
  myErrorsLBox->setMinimumSize( 100, 100 );
  Layout2->addWidget( myErrorsLBox, 1, 0 );

  mySubShapesLbl = new QLabel( tr( "GEOM_CHECK_BLOCKS_COMPOUND_SUBSHAPES" ), myGrp, "BlockCompoundSubShapes" );
  Layout2->addWidget( mySubShapesLbl, 0, 1 );

  mySubShapesLBox = new QListBox( myGrp, "ListSubShapes" );
  mySubShapesLBox->setMinimumSize( 100, 100 );
  Layout2->addWidget( mySubShapesLBox, 1, 1 );
  aGBLayout->addLayout( Layout2, 1, 0 );

  Layout1->addWidget( myGrp, 1, 0 );
  /* Initialisation */
  Init( Sel );
}


//=================================================================================
// function : ~MeasureGUI_CheckCompoundOfBlocksDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MeasureGUI_CheckCompoundOfBlocksDlg::~MeasureGUI_CheckCompoundOfBlocksDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_CheckCompoundOfBlocksDlg::Init( SALOME_Selection* Sel )
{
  mySelBtn = myGrp->PushButton1;
  mySelEdit = myGrp->LineEdit1;
  MeasureGUI_Skeleton::Init( Sel );
}

//=================================================================================
// function : getBCErrors
// purpose  :
//=================================================================================
bool MeasureGUI_CheckCompoundOfBlocksDlg::getBCErrors( bool& theIsCompoundOfBlocks,
						       GEOM::GEOM_IBlocksOperations::BCErrors& theErrors)
{
  if ( myObj->_is_nil() )
    return false;
  else
  {
    try
    {
      GEOM::GEOM_IBlocksOperations::BCErrors_var aErrs;
      theIsCompoundOfBlocks =
	GEOM::GEOM_IBlocksOperations::_narrow( getOperation() )->CheckCompoundOfBlocks( myObj, aErrs );
      theErrors = aErrs;
    }
    catch( const SALOME::SALOME_Exception& e )
    {
      QtCatchCorbaException( e );
      return false;
    }

    return getOperation()->IsDone();
  }
}

//=================================================================================
// function : processObject
// purpose  :
//=================================================================================
void MeasureGUI_CheckCompoundOfBlocksDlg::processObject()
{
  QString aMsg ("");
  bool isCompoundOfBlocks;
  GEOM::GEOM_IBlocksOperations::BCErrors aErrs;
  if ( !getBCErrors( isCompoundOfBlocks, aErrs ) )
  {
    myGrp->TextEdit1->setText( aMsg );
    return;
  }

  if (isCompoundOfBlocks) {
    aMsg += tr( "GEOM_CHECK_BLOCKS_COMPOUND_HAS_NO_ERRORS" );
  } else {
    aMsg += tr( "GEOM_CHECK_BLOCKS_COMPOUND_HAS_ERRORS" );
  }
  myGrp->TextEdit1->setText(aMsg);
  QStringList aErrList;
  QString aErrStr( "" );
  for ( int i = 0, n = aErrs.length(); i < n; i++ )
  {
    switch ( aErrs[i].error )
    {
      case GEOM::GEOM_IBlocksOperations::NOT_BLOCK :
	aErrStr = "Not a Block";
        break;
      case GEOM::GEOM_IBlocksOperations::EXTRA_EDGE :
	aErrStr = "Extra Edge";
        break;
      case GEOM::GEOM_IBlocksOperations::INVALID_CONNECTION :
	aErrStr = "Invalid Connection";
        break;
      case GEOM::GEOM_IBlocksOperations::NOT_CONNECTED :
	aErrStr = "Not Connected";
        break;
      case GEOM::GEOM_IBlocksOperations::NOT_GLUED :
	aErrStr = "Not Glued";
        break;
      default :
	aErrStr = "";
	break;
    }
    if ( !aErrStr.isEmpty() )
      aErrList.append(aErrStr);
  }
  myErrorsLBox->clear();
  myErrorsLBox->insertStringList(aErrList);
  return;
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr MeasureGUI_CheckCompoundOfBlocksDlg::createOperation()
{
  return getGeomEngine()->GetIBlocksOperations(getStudyId());
}
