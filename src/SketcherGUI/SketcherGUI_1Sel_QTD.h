/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_1Sel_QTD.ui'
**
** Created: dim nov 2 17:13:01 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_1SEL_QTD_H
#define SKETCHERGUI_1SEL_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

class SketcherGUI_1Sel_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_1Sel_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_1Sel_QTD();

    QGroupBox* GroupBox1;
    QPushButton* buttonUndo;
    QPushButton* buttonApply;
    QLabel* TextLabel1;
    QPushButton* PushButton1;
    QLineEdit* LineEdit1;


protected:
    QGridLayout* SketcherGUI_1Sel_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout3;
    QGridLayout* Layout2;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_1SEL_QTD_H
