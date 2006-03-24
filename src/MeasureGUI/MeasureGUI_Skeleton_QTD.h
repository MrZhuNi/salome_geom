/****************************************************************************
** Form interface generated from reading ui file 'MeasureGUI_Skeleton_QTD.ui'
**
** Created: Tue Mar 21 10:10:08 2006
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
    QGroupBox* GroupMedium;

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
