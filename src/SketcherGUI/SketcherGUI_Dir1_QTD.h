/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_Dir1_QTD.ui'
**
** Created: lun nov 3 17:27:54 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_DIR1_QTD_H
#define SKETCHERGUI_DIR1_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QRadioButton;

class SketcherGUI_Dir1_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_Dir1_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_Dir1_QTD();

    QButtonGroup* GroupDir1;
    QRadioButton* RB_Dir12;
    QRadioButton* RB_Dir13;
    QRadioButton* RB_Dir11;
    QRadioButton* RB_Dir14;


protected:
    QGridLayout* SketcherGUI_Dir1_QTDLayout;
    QGridLayout* GroupDir1Layout;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_DIR1_QTD_H
