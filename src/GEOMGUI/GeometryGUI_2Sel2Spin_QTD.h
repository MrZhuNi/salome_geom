/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_2Sel2Spin_QTD.ui'
**
** Created: jeu sep 25 12:10:29 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_2SEL2SPIN_QTD_H
#define GEOMETRYGUI_2SEL2SPIN_QTD_H

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

class GeometryGUI_2Sel2Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    GeometryGUI_2Sel2Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GeometryGUI_2Sel2Spin_QTD();

    QGroupBox* GroupBox1;
    QLineEdit* LineEdit1;
    QSpinBox* SpinBox2;
    QLabel* TextLabel4;
    QLabel* TextLabel3;
    QSpinBox* SpinBox1;
    QPushButton* PushButton2;
    QLabel* TextLabel2;
    QLabel* TextLabel1;
    QPushButton* PushButton1;
    QLineEdit* LineEdit2;


protected:
    QGridLayout* GeometryGUI_2Sel2Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
    QGridLayout* Layout2;
};

#endif // GEOMETRYGUI_2SEL2SPIN_QTD_H
