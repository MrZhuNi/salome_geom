/****************************************************************************
** Form implementation generated from reading ui file 'KinematicGUI_12Spin_QTD.ui'
**
** Created: ven fév 6 12:39:44 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "KinematicGUI_12Spin_QTD.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a KinematicGUI_12Spin_QTD which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
KinematicGUI_12Spin_QTD::KinematicGUI_12Spin_QTD( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KinematicGUI_12Spin_QTD" );
    resize( 396, 139 ); 
    setCaption( trUtf8( "KinematicGUI_12Spin_QTD" ) );
    KinematicGUI_12Spin_QTDLayout = new QGridLayout( this, 1, 1, 0, 6, "KinematicGUI_12Spin_QTDLayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( trUtf8( "" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 

    TextLabel15 = new QLabel( GroupBox1, "TextLabel15" );
    TextLabel15->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel15->sizePolicy().hasHeightForWidth() ) );
    TextLabel15->setText( trUtf8( "TL15" ) );

    Layout1->addWidget( TextLabel15, 3, 3 );
    QSpacerItem* spacer = new QSpacerItem( 0, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer, 4, 4 );

    TextLabel14 = new QLabel( GroupBox1, "TextLabel14" );
    TextLabel14->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel14->sizePolicy().hasHeightForWidth() ) );
    TextLabel14->setText( trUtf8( "TL14" ) );

    Layout1->addWidget( TextLabel14, 3, 1 );

    SpinBox5 = new QSpinBox( GroupBox1, "SpinBox5" );
    SpinBox5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox5->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox5, 1, 4 );

    TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
    TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel3->sizePolicy().hasHeightForWidth() ) );
    TextLabel3->setText( trUtf8( "TL3" ) );

    Layout1->addWidget( TextLabel3, 0, 3 );

    TextLabel11 = new QLabel( GroupBox1, "TextLabel11" );
    TextLabel11->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel11->sizePolicy().hasHeightForWidth() ) );
    TextLabel11->setText( trUtf8( "TL11" ) );

    Layout1->addWidget( TextLabel11, 2, 3 );

    TextLabel8 = new QLabel( GroupBox1, "TextLabel8" );
    TextLabel8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel8->sizePolicy().hasHeightForWidth() ) );
    TextLabel8->setText( trUtf8( "TL8" ) );

    Layout1->addWidget( TextLabel8, 1, 5 );

    TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );
    TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel4->sizePolicy().hasHeightForWidth() ) );
    TextLabel4->setText( trUtf8( "TL4" ) );

    Layout1->addWidget( TextLabel4, 0, 5 );

    TextLabel7 = new QLabel( GroupBox1, "TextLabel7" );
    TextLabel7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel7->sizePolicy().hasHeightForWidth() ) );
    TextLabel7->setText( trUtf8( "TL7" ) );

    Layout1->addWidget( TextLabel7, 1, 3 );

    TextLabel12 = new QLabel( GroupBox1, "TextLabel12" );
    TextLabel12->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel12->sizePolicy().hasHeightForWidth() ) );
    TextLabel12->setText( trUtf8( "TL12" ) );

    Layout1->addWidget( TextLabel12, 2, 5 );

    TextLabel13 = new QLabel( GroupBox1, "TextLabel13" );
    TextLabel13->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel13->sizePolicy().hasHeightForWidth() ) );
    TextLabel13->setText( trUtf8( "TL13" ) );

    Layout1->addWidget( TextLabel13, 3, 0 );

    SpinBox12 = new QSpinBox( GroupBox1, "SpinBox12" );
    SpinBox12->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox12->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox12, 3, 6 );

    SpinBox11 = new QSpinBox( GroupBox1, "SpinBox11" );
    SpinBox11->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox11->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox11, 3, 4 );

    SpinBox9 = new QSpinBox( GroupBox1, "SpinBox9" );
    SpinBox9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox9->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox9, 2, 6 );

    SpinBox8 = new QSpinBox( GroupBox1, "SpinBox8" );
    SpinBox8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox8->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox8, 2, 4 );

    SpinBox7 = new QSpinBox( GroupBox1, "SpinBox7" );
    SpinBox7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox7->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox7, 2, 2 );

    SpinBox6 = new QSpinBox( GroupBox1, "SpinBox6" );
    SpinBox6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox6->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox6, 1, 6 );

    SpinBox3 = new QSpinBox( GroupBox1, "SpinBox3" );
    SpinBox3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox3->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox3, 0, 6 );

    TextLabel16 = new QLabel( GroupBox1, "TextLabel16" );
    TextLabel16->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel16->sizePolicy().hasHeightForWidth() ) );
    TextLabel16->setText( trUtf8( "TL16" ) );

    Layout1->addWidget( TextLabel16, 3, 5 );

    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
    TextLabel1->setText( trUtf8( "TL1" ) );

    Layout1->addWidget( TextLabel1, 0, 0 );

    TextLabel10 = new QLabel( GroupBox1, "TextLabel10" );
    TextLabel10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel10->sizePolicy().hasHeightForWidth() ) );
    TextLabel10->setText( trUtf8( "TL10" ) );

    Layout1->addWidget( TextLabel10, 2, 1 );

    SpinBox4 = new QSpinBox( GroupBox1, "SpinBox4" );
    SpinBox4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox4->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox4, 1, 2 );

    TextLabel6 = new QLabel( GroupBox1, "TextLabel6" );
    TextLabel6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel6->sizePolicy().hasHeightForWidth() ) );
    TextLabel6->setText( trUtf8( "TL6" ) );

    Layout1->addWidget( TextLabel6, 1, 1 );

    SpinBox10 = new QSpinBox( GroupBox1, "SpinBox10" );
    SpinBox10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox10->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox10, 3, 2 );

    SpinBox2 = new QSpinBox( GroupBox1, "SpinBox2" );
    SpinBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox2->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox2, 0, 4 );

    TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2->setText( trUtf8( "TL2" ) );

    Layout1->addWidget( TextLabel2, 0, 1 );

    TextLabel9 = new QLabel( GroupBox1, "TextLabel9" );
    TextLabel9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel9->sizePolicy().hasHeightForWidth() ) );
    TextLabel9->setText( trUtf8( "TL9" ) );

    Layout1->addWidget( TextLabel9, 2, 0 );

    TextLabel5 = new QLabel( GroupBox1, "TextLabel5" );
    TextLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel5->sizePolicy().hasHeightForWidth() ) );
    TextLabel5->setText( trUtf8( "TL5" ) );

    Layout1->addWidget( TextLabel5, 1, 0 );

    SpinBox1 = new QSpinBox( GroupBox1, "SpinBox1" );
    SpinBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox1->sizePolicy().hasHeightForWidth() ) );

    Layout1->addWidget( SpinBox1, 0, 2 );

    GroupBox1Layout->addLayout( Layout1, 0, 0 );

    KinematicGUI_12Spin_QTDLayout->addWidget( GroupBox1, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
KinematicGUI_12Spin_QTD::~KinematicGUI_12Spin_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

