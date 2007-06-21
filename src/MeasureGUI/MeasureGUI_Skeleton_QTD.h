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
** Form interface generated from reading ui file 'MeasureGUI_Skeleton_QTD.ui'
**
** Created: Tue Mar 13 14:20:20 2007
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MEASUREGUI_SKELETON_QTD_H
#define MEASUREGUI_SKELETON_QTD_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QPushButton;
class QButtonGroup;
class QRadioButton;

class MeasureGUI_Skeleton_QTD : public QDialog
{
    Q_OBJECT

public:
    MeasureGUI_Skeleton_QTD( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MeasureGUI_Skeleton_QTD();

    QGroupBox* GroupButtons;
    QPushButton* buttonClose;
    QPushButton* buttonHelp;
    QButtonGroup* GroupConstructors;
    QRadioButton* RadioButton1;

protected:
    QGridLayout* MeasureGUI_Skeleton_QTDLayout;
    QGridLayout* Layout1;
    QGridLayout* GroupButtonsLayout;
    QGridLayout* Layout3;
    QSpacerItem* Spacer1;
    QGridLayout* GroupConstructorsLayout;
    QHBoxLayout* Layout2;

protected slots:
    virtual void languageChange();

};

#endif // MEASUREGUI_SKELETON_QTD_H
