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
// See http://www.salome-platform.org/
//
/****************************************************************************
** Form interface generated from reading ui file 'EntityGUI_Skeleton_QTD.ui'
**
** Created: ven déc 12 11:17:07 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef ENTITYGUI_SKELETON_QTD_H
#define ENTITYGUI_SKELETON_QTD_H

#include "GEOM_EntityGUI.hxx"

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QPushButton;
class QRadioButton;

class GEOM_ENTITYGUI_EXPORT EntityGUI_Skeleton_QTD : public QDialog
{ 
    Q_OBJECT

public:
    EntityGUI_Skeleton_QTD( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~EntityGUI_Skeleton_QTD();

    QGroupBox* GroupVal;
    QButtonGroup* GroupConstructors;
    QRadioButton* RadioButton1;
    QRadioButton* RadioButton2;
    QGroupBox* GroupDest;
    QButtonGroup* GroupDest1;
    QRadioButton* RB_Dest2;
    QRadioButton* RB_Dest1;
    QButtonGroup* GroupDest2;
    QButtonGroup* GroupDest3;
    QGroupBox* GroupButtons;
    QPushButton* buttonEnd;
    QPushButton* buttonClose;
    QPushButton* buttonCancel;


protected:
    QGridLayout* EntityGUI_Skeleton_QTDLayout;
    QGridLayout* Layout1;
    QGridLayout* GroupConstructorsLayout;
    QHBoxLayout* Layout2;
    QGridLayout* GroupDestLayout;
    QGridLayout* Layout5;
    QGridLayout* GroupDest1Layout;
    QGridLayout* Layout4;
    QGridLayout* GroupDest2Layout;
    QGridLayout* GroupDest3Layout;
    QHBoxLayout* GroupButtonsLayout;
    QHBoxLayout* Layout3;
};

#endif // ENTITYGUI_SKELETON_QTD_H
