/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_3Spin_QTD.ui'
**
** Created: jeu oct 30 18:28:54 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_3SPIN_QTD_H
#define SKETCHERGUI_3SPIN_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QLabel;
class QPushButton;
class QSpinBox;

class SketcherGUI_3Spin_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_3Spin_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_3Spin_QTD();

    QGroupBox* GroupBox1;
    QPushButton* buttonUndo;
    QPushButton* buttonApply;
    QLabel* TextLabel2;
    QLabel* TextLabel1;
    QSpinBox* SpinBox1;
    QSpinBox* SpinBox2;
    QLabel* TextLabel3;
    QSpinBox* SpinBox3;


protected:
    QGridLayout* SketcherGUI_3Spin_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout3;
    QGridLayout* Layout2;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_3SPIN_QTD_H
