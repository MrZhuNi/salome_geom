// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
/****************************************************************************
** Form implementation generated from reading ui file 'DlgRef_Skeleton_QTD.ui'
**
** Created: Tue Mar 13 14:40:34 2007
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "DlgRef_Skeleton_QTD.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a DlgRef_Skeleton_QTD as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DlgRef_Skeleton_QTD::DlgRef_Skeleton_QTD( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "DlgRef_Skeleton_QTD" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)7, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setSizeGripEnabled( TRUE );
    DlgRef_Skeleton_QTDLayout = new QGridLayout( this, 1, 1, 11, 6, "DlgRef_Skeleton_QTDLayout"); 

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 

    GroupButtons = new QGroupBox( this, "GroupButtons" );
    GroupButtons->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, GroupButtons->sizePolicy().hasHeightForWidth() ) );
    GroupButtons->setColumnLayout(0, Qt::Vertical );
    GroupButtons->layout()->setSpacing( 6 );
    GroupButtons->layout()->setMargin( 11 );
    GroupButtonsLayout = new QHBoxLayout( GroupButtons->layout() );
    GroupButtonsLayout->setAlignment( Qt::AlignTop );

    Layout3 = new QHBoxLayout( 0, 0, 6, "Layout3"); 

    buttonOk = new QPushButton( GroupButtons, "buttonOk" );
    Layout3->addWidget( buttonOk );

    buttonApply = new QPushButton( GroupButtons, "buttonApply" );
    Layout3->addWidget( buttonApply );
    Spacer1 = new QSpacerItem( 91, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout3->addItem( Spacer1 );

    buttonCancel = new QPushButton( GroupButtons, "buttonCancel" );
    Layout3->addWidget( buttonCancel );

    buttonHelp = new QPushButton( GroupButtons, "buttonHelp" );
    Layout3->addWidget( buttonHelp );
    GroupButtonsLayout->addLayout( Layout3 );

    Layout1->addWidget( GroupButtons, 3, 0 );

    GroupConstructors = new QButtonGroup( this, "GroupConstructors" );
    GroupConstructors->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, GroupConstructors->sizePolicy().hasHeightForWidth() ) );
    GroupConstructors->setColumnLayout(0, Qt::Vertical );
    GroupConstructors->layout()->setSpacing( 6 );
    GroupConstructors->layout()->setMargin( 11 );
    GroupConstructorsLayout = new QHBoxLayout( GroupConstructors->layout() );
    GroupConstructorsLayout->setAlignment( Qt::AlignTop );

    Layout2 = new QHBoxLayout( 0, 0, 6, "Layout2"); 

    RadioButton1 = new QRadioButton( GroupConstructors, "RadioButton1" );
    Layout2->addWidget( RadioButton1 );

    RadioButton2 = new QRadioButton( GroupConstructors, "RadioButton2" );
    Layout2->addWidget( RadioButton2 );

    RadioButton3 = new QRadioButton( GroupConstructors, "RadioButton3" );
    Layout2->addWidget( RadioButton3 );

    RadioButton4 = new QRadioButton( GroupConstructors, "RadioButton4" );
    Layout2->addWidget( RadioButton4 );
    GroupConstructorsLayout->addLayout( Layout2 );

    Layout1->addWidget( GroupConstructors, 0, 0 );

    GroupBoxName = new QGroupBox( this, "GroupBoxName" );
    GroupBoxName->setColumnLayout(0, Qt::Vertical );
    GroupBoxName->layout()->setSpacing( 6 );
    GroupBoxName->layout()->setMargin( 11 );
    GroupBoxNameLayout = new QGridLayout( GroupBoxName->layout() );
    GroupBoxNameLayout->setAlignment( Qt::AlignTop );

    Layout66 = new QGridLayout( 0, 1, 1, 0, 6, "Layout66"); 

    ResultName = new QLineEdit( GroupBoxName, "ResultName" );

    Layout66->addWidget( ResultName, 0, 1 );

    NameLabel = new QLabel( GroupBoxName, "NameLabel" );

    Layout66->addWidget( NameLabel, 0, 0 );

    GroupBoxNameLayout->addLayout( Layout66, 0, 0 );

    Layout1->addWidget( GroupBoxName, 1, 0 );

    DlgRef_Skeleton_QTDLayout->addLayout( Layout1, 0, 0 );
    languageChange();
    resize( QSize(307, 378).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
}

/*
 *  Destroys the object and frees any allocated resources
 */
DlgRef_Skeleton_QTD::~DlgRef_Skeleton_QTD()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void DlgRef_Skeleton_QTD::languageChange()
{
}

