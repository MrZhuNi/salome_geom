/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_Skeleton_QTD.ui'
**
** Created: mar sep 23 15:06:57 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_SKELETON_QTD_H
#define GEOMETRYGUI_SKELETON_QTD_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QPushButton;
class QRadioButton;

class GeometryGUI_Skeleton_QTD : public QDialog
{ 
    Q_OBJECT

public:
    GeometryGUI_Skeleton_QTD( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~GeometryGUI_Skeleton_QTD();

    QGroupBox* GroupButtons;
    QPushButton* buttonOk;
    QPushButton* buttonApply;
    QPushButton* buttonCancel;
    QButtonGroup* GroupConstructors;
    QRadioButton* RadioButton1;
    QRadioButton* RadioButton2;
    QRadioButton* RadioButton3;
    QGroupBox* GroupMedium;


protected:
    QGridLayout* GeometryGUI_Skeleton_QTDLayout;
    QGridLayout* Layout1;
    QHBoxLayout* GroupButtonsLayout;
    QHBoxLayout* Layout3;
    QGridLayout* GroupConstructorsLayout;
    QHBoxLayout* Layout2;
};

#endif // GEOMETRYGUI_SKELETON_QTD_H
