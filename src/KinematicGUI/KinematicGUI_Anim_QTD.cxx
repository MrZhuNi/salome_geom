/****************************************************************************
** Form implementation generated from reading ui file 'KinematicGUI_Anim_QTD.ui'
**
** Created: jeu mar 4 16:48:10 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "KinematicGUI_Anim_QTD.h"

#include <qvariant.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a KinematicGUI_Anim_QTD which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
KinematicGUI_Anim_QTD::KinematicGUI_Anim_QTD( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KinematicGUI_Anim_QTD" );
    resize( 334, 121 ); 
    setCaption( trUtf8( "KinematicGUI_Anim_QTD" ) );
    KinematicGUI_Anim_QTDLayout = new QGridLayout( this, 1, 1, 0, 6, "KinematicGUI_Anim_QTDLayout"); 

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setTitle( trUtf8( "" ) );
    GroupBox1->setColumnLayout(0, Qt::Vertical );
    GroupBox1->layout()->setSpacing( 6 );
    GroupBox1->layout()->setMargin( 11 );
    GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
    GroupBox1Layout->setAlignment( Qt::AlignTop );

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 
    QSpacerItem* spacer = new QSpacerItem( 0, 156, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer, 3, 0 );

    Layout2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout2"); 

    PushButton3 = new QPushButton( GroupBox1, "PushButton3" );
    PushButton3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton3->sizePolicy().hasHeightForWidth() ) );
    PushButton3->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton3, 0, 2 );

    PushButton5 = new QPushButton( GroupBox1, "PushButton5" );
    PushButton5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton5->sizePolicy().hasHeightForWidth() ) );
    PushButton5->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton5, 0, 4 );

    PushButton2 = new QPushButton( GroupBox1, "PushButton2" );
    PushButton2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton2->sizePolicy().hasHeightForWidth() ) );
    PushButton2->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton2, 0, 1 );

    PushButton4 = new QPushButton( GroupBox1, "PushButton4" );
    PushButton4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton4->sizePolicy().hasHeightForWidth() ) );
    PushButton4->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton4, 0, 3 );

    PushButton6 = new QPushButton( GroupBox1, "PushButton6" );
    PushButton6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton6->sizePolicy().hasHeightForWidth() ) );
    PushButton6->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton6, 0, 5 );

    PushButton1 = new QPushButton( GroupBox1, "PushButton1" );
    PushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton1->sizePolicy().hasHeightForWidth() ) );
    PushButton1->setText( trUtf8( "" ) );

    Layout2->addWidget( PushButton1, 0, 0 );

    Layout1->addLayout( Layout2, 1, 0 );

    Slider1 = new QSlider( GroupBox1, "Slider1" );
    Slider1->setOrientation( QSlider::Horizontal );

    Layout1->addWidget( Slider1, 0, 0 );

    Layout3 = new QGridLayout( 0, 1, 1, 0, 6, "Layout3"); 

    CheckButton1 = new QRadioButton( GroupBox1, "CheckButton1" );
    CheckButton1->setText( trUtf8( "" ) );

    Layout3->addWidget( CheckButton1, 0, 0 );

    PushButton7 = new QPushButton( GroupBox1, "PushButton7" );
    PushButton7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton7->sizePolicy().hasHeightForWidth() ) );
    PushButton7->setText( trUtf8( "" ) );

    Layout3->addWidget( PushButton7, 0, 2 );

    CheckButton2 = new QRadioButton( GroupBox1, "CheckButton2" );
    CheckButton2->setText( trUtf8( "" ) );

    Layout3->addWidget( CheckButton2, 0, 1 );

    Layout1->addLayout( Layout3, 2, 0 );

    GroupBox1Layout->addLayout( Layout1, 0, 0 );

    KinematicGUI_Anim_QTDLayout->addWidget( GroupBox1, 0, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
KinematicGUI_Anim_QTD::~KinematicGUI_Anim_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

