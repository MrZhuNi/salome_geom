/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_Skeleton_QTD.ui'
**
** Created: lun nov 3 17:27:53 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_SKELETON_QTD_H
#define SKETCHERGUI_SKELETON_QTD_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QPushButton;
class QRadioButton;

class SketcherGUI_Skeleton_QTD : public QDialog
{ 
    Q_OBJECT

public:
    SketcherGUI_Skeleton_QTD( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SketcherGUI_Skeleton_QTD();

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
    QGridLayout* SketcherGUI_Skeleton_QTDLayout;
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

#endif // SKETCHERGUI_SKELETON_QTD_H
