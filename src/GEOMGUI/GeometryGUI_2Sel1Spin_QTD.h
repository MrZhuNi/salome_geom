/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_2Sel1Spin_QTD.ui'
**
** Created: mar sep 23 16:05:09 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_2SEL1SPIN_QTD_H
#define GEOMETRYGUI_2SEL1SPIN_QTD_H

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

class GeometryGUI_2Sel1Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    GeometryGUI_2Sel1Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GeometryGUI_2Sel1Spin_QTD();

    QGroupBox* GroupBox1;
    QLineEdit* LineEdit1;
    QPushButton* PushButton2;
    QLabel* TextLabel1;
    QLineEdit* LineEdit2;
    QPushButton* PushButton1;
    QLabel* TextLabel2;
    QSpinBox* SpinBox1;
    QLabel* TextLabel3;


protected:
    QGridLayout* GeometryGUI_2Sel1Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
    QGridLayout* Layout2;
};

#endif // GEOMETRYGUI_2SEL1SPIN_QTD_H
