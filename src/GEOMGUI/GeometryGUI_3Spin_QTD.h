/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_3Spin_QTD.ui'
**
** Created: mar sep 23 16:05:09 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_3SPIN_QTD_H
#define GEOMETRYGUI_3SPIN_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QSpinBox;

class GeometryGUI_3Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    GeometryGUI_3Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GeometryGUI_3Spin_QTD();

    QGroupBox* GroupBox1;
    QSpinBox* SpinBox2;
    QLabel* TextLabel3;
    QSpinBox* SpinBox1;
    QSpinBox* SpinBox3;
    QLabel* TextLabel1;
    QLabel* TextLabel2;


protected:
    QGridLayout* GeometryGUI_3Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
};

#endif // GEOMETRYGUI_3SPIN_QTD_H
