/****************************************************************************
** Form implementation generated from reading ui file 'KinematicGUI_AnimValues_QTD.ui'
**
** Created: lun mai 3 09:48:44 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "KinematicGUI_AnimValues_QTD.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a KinematicGUI_AnimValues_QTD which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
KinematicGUI_AnimValues_QTD::KinematicGUI_AnimValues_QTD( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KinematicGUI_AnimValues_QTD" );
    resize( 469, 249 ); 
    setCaption( trUtf8( "KinematicGUI_AnimValues_QTD" ) );
    KinematicGUI_AnimValues_QTDLayout = new QGridLayout( this, 1, 1, 0, 6, "KinematicGUI_AnimValues_QTDLayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( trUtf8( "" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    LayoutD = new QGridLayout( 0, 1, 1, 0, 6, "LayoutD"); 
    QSpacerItem* spacer = new QSpacerItem( 0, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LayoutD->addItem( spacer, 2, 1 );

    LayoutC = new QGridLayout( 0, 1, 1, 0, 6, "LayoutC"); 

    TextLabel8 = new QLabel( GroupBox1, "TextLabel8" );
    TextLabel8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel8->sizePolicy().hasHeightForWidth() ) );
    TextLabel8->setText( trUtf8( "TL8" ) );

    LayoutC->addWidget( TextLabel8, 5, 0 );

    SpinBox1 = new QSpinBox( GroupBox1, "SpinBox1" );
    SpinBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox1->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox1, 1, 1, 1, 2 );

    TextLabel9 = new QLabel( GroupBox1, "TextLabel9" );
    TextLabel9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel9->sizePolicy().hasHeightForWidth() ) );
    TextLabel9->setText( trUtf8( "TL9" ) );

    LayoutC->addWidget( TextLabel9, 6, 0 );

    SpinBox6 = new QSpinBox( GroupBox1, "SpinBox6" );
    SpinBox6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox6->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox6, 3, 3 );

    SpinBox2 = new QSpinBox( GroupBox1, "SpinBox2" );
    SpinBox2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox2->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox2, 1, 3 );

    TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );
    TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel4->sizePolicy().hasHeightForWidth() ) );
    TextLabel4->setText( trUtf8( "TL4" ) );

    LayoutC->addWidget( TextLabel4, 1, 0 );

    SpinBox12 = new QSpinBox( GroupBox1, "SpinBox12" );
    SpinBox12->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox12->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox12, 6, 3 );

    SpinBox9 = new QSpinBox( GroupBox1, "SpinBox9" );
    SpinBox9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox9->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox9, 5, 5, 1, 2 );

    SpinBox5 = new QSpinBox( GroupBox1, "SpinBox5" );
    SpinBox5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox5->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox5, 3, 3, 1, 2 );

    TextLabel5 = new QLabel( GroupBox1, "TextLabel5" );
    TextLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel5->sizePolicy().hasHeightForWidth() ) );
    TextLabel5->setText( trUtf8( "TL5" ) );

    LayoutC->addWidget( TextLabel5, 2, 0 );

    TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
    TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, TextLabel3->sizePolicy().hasHeightForWidth() ) );
    TextLabel3->setText( trUtf8( "TL3" ) );
    TextLabel3->setAlignment( int( QLabel::AlignCenter ) );

    LayoutC->addWidget( TextLabel3, 0, 3 );

    SpinBox8 = new QSpinBox( GroupBox1, "SpinBox8" );
    SpinBox8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox8->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox8, 4, 3 );

    TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
    TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, TextLabel2->sizePolicy().hasHeightForWidth() ) );
    TextLabel2->setText( trUtf8( "TL2" ) );
    TextLabel2->setAlignment( int( QLabel::AlignCenter ) );

    LayoutC->addWidget( TextLabel2, 0, 2 );

    SpinBox7 = new QSpinBox( GroupBox1, "SpinBox7" );
    SpinBox7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox7->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox7, 4, 4, 1, 2 );

    TextLabel6 = new QLabel( GroupBox1, "TextLabel6" );
    TextLabel6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel6->sizePolicy().hasHeightForWidth() ) );
    TextLabel6->setText( trUtf8( "TL6" ) );

    LayoutC->addWidget( TextLabel6, 3, 0 );

    TextLabel10 = new QLabel( GroupBox1, "TextLabel10" );
    TextLabel10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel10->sizePolicy().hasHeightForWidth() ) );
    QFont TextLabel10_font(  TextLabel10->font() );
    TextLabel10_font.setBold( TRUE );
    TextLabel10->setFont( TextLabel10_font ); 
    TextLabel10->setText( trUtf8( "TL10" ) );

    LayoutC->addMultiCellWidget( TextLabel10, 0, 0, 0, 1 );

    SpinBox10 = new QSpinBox( GroupBox1, "SpinBox10" );
    SpinBox10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox10->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox10, 5, 3 );

    TextLabel7 = new QLabel( GroupBox1, "TextLabel7" );
    TextLabel7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel7->sizePolicy().hasHeightForWidth() ) );
    TextLabel7->setText( trUtf8( "TL7" ) );

    LayoutC->addWidget( TextLabel7, 4, 0 );

    SpinBox11 = new QSpinBox( GroupBox1, "SpinBox11" );
    SpinBox11->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox11->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox11, 6, 6, 1, 2 );

    SpinBox4 = new QSpinBox( GroupBox1, "SpinBox4" );
    SpinBox4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox4->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addWidget( SpinBox4, 2, 3 );

    SpinBox3 = new QSpinBox( GroupBox1, "SpinBox3" );
    SpinBox3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, SpinBox3->sizePolicy().hasHeightForWidth() ) );

    LayoutC->addMultiCellWidget( SpinBox3, 2, 2, 1, 2 );

    LayoutD->addLayout( LayoutC, 1, 1 );

    ListBox1 = new QListBox( GroupBox1, "ListBox1" );
    ListBox1->insertItem( trUtf8( "New Item" ) );
    ListBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, ListBox1->sizePolicy().hasHeightForWidth() ) );

    LayoutD->addMultiCellWidget( ListBox1, 1, 2, 0, 0 );

    LayoutB = new QGridLayout( 0, 1, 1, 0, 6, "LayoutB"); 

    LineEdit1 = new QLineEdit( GroupBox1, "LineEdit1" );

    LayoutB->addWidget( LineEdit1, 0, 2 );

    PushButton1 = new QPushButton( GroupBox1, "PushButton1" );
    PushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton1->sizePolicy().hasHeightForWidth() ) );
    PushButton1->setText( trUtf8( "" ) );

    LayoutB->addWidget( PushButton1, 0, 1 );

    TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
    TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
    TextLabel1->setText( trUtf8( "TL1" ) );

    LayoutB->addWidget( TextLabel1, 0, 0 );

    LayoutD->addMultiCellLayout( LayoutB, 0, 0, 0, 1 );

    GroupBox1Layout->addLayout( LayoutD, 0, 0 );

    KinematicGUI_AnimValues_QTDLayout->addWidget( GroupBox1, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
KinematicGUI_AnimValues_QTD::~KinematicGUI_AnimValues_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

