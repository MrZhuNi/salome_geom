/****************************************************************************
** Form implementation generated from reading ui file 'SketcherGUI_Dir1_QTD.ui'
**
** Created: lun nov 3 17:27:54 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "SketcherGUI_Dir1_QTD.h"

#include <qvariant.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a SketcherGUI_Dir1_QTD which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
SketcherGUI_Dir1_QTD::SketcherGUI_Dir1_QTD( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "SketcherGUI_Dir1_QTD" );
    resize( 131, 123 ); 
    setCaption( trUtf8( "SketcherGUI_Dir1_QTD" ) );
    SketcherGUI_Dir1_QTDLayout = new QGridLayout( this, 1, 1, 0, 6, "SketcherGUI_Dir1_QTDLayout"); 

    GroupDir1 = new QButtonGroup( this, "GroupDir1" );
    GroupDir1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, GroupDir1->sizePolicy().hasHeightForWidth() ) );
    GroupDir1->setTitle( trUtf8( "Direction" ) );
    GroupDir1->setColumnLayout(0, Qt::Vertical );
    GroupDir1->layout()->setSpacing( 6 );
    GroupDir1->layout()->setMargin( 11 );
    GroupDir1Layout = new QGridLayout( GroupDir1->layout() );
    GroupDir1Layout->setAlignment( Qt::AlignTop );

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 

    RB_Dir12 = new QRadioButton( GroupDir1, "RB_Dir12" );
    RB_Dir12->setText( trUtf8( "Perpendicular" ) );

    Layout1->addWidget( RB_Dir12, 1, 0 );

    RB_Dir13 = new QRadioButton( GroupDir1, "RB_Dir13" );
    RB_Dir13->setText( trUtf8( "Tangent" ) );

    Layout1->addWidget( RB_Dir13, 2, 0 );

    RB_Dir11 = new QRadioButton( GroupDir1, "RB_Dir11" );
    RB_Dir11->setText( trUtf8( "Angle" ) );

    Layout1->addWidget( RB_Dir11, 0, 0 );

    RB_Dir14 = new QRadioButton( GroupDir1, "RB_Dir14" );
    RB_Dir14->setText( trUtf8( "VX-VY" ) );

    Layout1->addWidget( RB_Dir14, 3, 0 );

    GroupDir1Layout->addLayout( Layout1, 0, 0 );

    SketcherGUI_Dir1_QTDLayout->addWidget( GroupDir1, 0, 0 );

    // tab order
    setTabOrder( RB_Dir11, RB_Dir12 );
    setTabOrder( RB_Dir12, RB_Dir13 );
    setTabOrder( RB_Dir13, RB_Dir14 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SketcherGUI_Dir1_QTD::~SketcherGUI_Dir1_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

