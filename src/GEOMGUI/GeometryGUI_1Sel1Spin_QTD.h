/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_1Sel1Spin_QTD.ui'
**
** Created: mar sep 23 16:05:08 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_1SEL1SPIN_QTD_H
#define GEOMETRYGUI_1SEL1SPIN_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

class GeometryGUI_1Sel1Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    GeometryGUI_1Sel1Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GeometryGUI_1Sel1Spin_QTD();

    QGroupBox* GroupBox1;
    QPushButton* PushButton1;
    QLabel* TextLabel1;
    QLineEdit* LineEdit1;
    QLabel* TextLabel2;
    QSpinBox* SpinBox1;


protected:
    QGridLayout* GeometryGUI_1Sel1Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
    QGridLayout* Layout2;
};

#endif // GEOMETRYGUI_1SEL1SPIN_QTD_H
