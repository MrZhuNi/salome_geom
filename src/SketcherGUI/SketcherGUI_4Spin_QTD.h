/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_4Spin_QTD.ui'
**
** Created: lun nov 3 17:39:03 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_4SPIN_QTD_H
#define SKETCHERGUI_4SPIN_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QPushButton;
class QSpinBox;

class SketcherGUI_4Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_4Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_4Spin_QTD();

    QGroupBox* GroupBox1;
    QPushButton* buttonUndo;
    QPushButton* buttonApply;
    QSpinBox* SpinBox3;
    QSpinBox* SpinBox2;
    QLabel* TextLabel2;
    QSpinBox* SpinBox4;
    QLabel* TextLabel3;
    QSpinBox* SpinBox1;
    QLabel* TextLabel1;
    QLabel* TextLabel4;


protected:
    QGridLayout* SketcherGUI_4Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout3;
    QGridLayout* Layout2;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_4SPIN_QTD_H
