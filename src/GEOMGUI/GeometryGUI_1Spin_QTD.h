/****************************************************************************
** Form interface generated from reading ui file 'GeometryGUI_1Spin_QTD.ui'
**
** Created: jeu sep 25 12:22:29 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef GEOMETRYGUI_1SPIN_QTD_H
#define GEOMETRYGUI_1SPIN_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QSpinBox;

class GeometryGUI_1Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    GeometryGUI_1Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~GeometryGUI_1Spin_QTD();

    QGroupBox* GroupBox1;
    QSpinBox* SpinBox1;
    QLabel* TextLabel1;


protected:
    QGridLayout* GeometryGUI_1Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
};

#endif // GEOMETRYGUI_1SPIN_QTD_H
