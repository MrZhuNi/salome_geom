/****************************************************************************
** Form interface generated from reading ui file 'KinematicGUI_Skeleton_QTD.ui'
**
** Created: mer mar 3 16:54:07 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef KINEMATICGUI_SKELETON_QTD_H
#define KINEMATICGUI_SKELETON_QTD_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QPushButton;
class QRadioButton;

class KinematicGUI_Skeleton_QTD : public QDialog
{ 
    Q_OBJECT

public:
    KinematicGUI_Skeleton_QTD( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~KinematicGUI_Skeleton_QTD();

    QGroupBox* GroupButtons;
    QPushButton* buttonClose;
    QButtonGroup* GroupConstructors;
    QRadioButton* RadioButton1;
    QGroupBox* GroupMedium;


protected:
    QGridLayout* KinematicGUI_Skeleton_QTDLayout;
    QGridLayout* Layout1;
    QGridLayout* GroupButtonsLayout;
    QGridLayout* Layout3;
    QGridLayout* GroupConstructorsLayout;
    QHBoxLayout* Layout2;
};

#endif // KINEMATICGUI_SKELETON_QTD_H
