/****************************************************************************
** Form implementation generated from reading ui file 'GeometryGUI_1Spin_QTD.ui'
**
** Created: jeu sep 25 12:22:29 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "GeometryGUI_1Spin_QTD.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a GeometryGUI_1Spin_QTD which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
GeometryGUI_1Spin_QTD::GeometryGUI_1Spin_QTD( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "GeometryGUI_1Spin_QTD" );
    resize( 124, 55 ); 
    setCaption( trUtf8( "GeometryGUI_1Spin_QTD" ) );
    GeometryGUI_1Spin_QTDLayout = new QGridLayout( this, 1, 1, 0, 6, "GeometryGUI_1Spin_QTDLayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( trUtf8( "" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 0, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer, 3, 1 );

    SpinBox1 = new QSpinBox( GroupBox1, "SpinBox1" );
    SpinBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox1->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox1, 0, 1 );

    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
    TextLabel1->setText( trUtf8( "TL1" ) );

    Layout1->addWidget( TextLabel1, 0, 0 );

    GroupBox1Layout->addLayout( Layout1, 0, 0 );

    GeometryGUI_1Spin_QTDLayout->addWidget( GroupBox1, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
GeometryGUI_1Spin_QTD::~GeometryGUI_1Spin_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

