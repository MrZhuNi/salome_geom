/****************************************************************************
** Form interface generated from reading ui file 'KinematicGUI_Anim_QTD.ui'
**
** Created: jeu mar 4 16:48:10 2004
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef KINEMATICGUI_ANIM_QTD_H
#define KINEMATICGUI_ANIM_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QGroupBox;
class QPushButton;
class QRadioButton;
class QSlider;

class KinematicGUI_Anim_QTD : public QWidget
{ 
    Q_OBJECT

public:
    KinematicGUI_Anim_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KinematicGUI_Anim_QTD();

    QGroupBox* GroupBox1;
    QPushButton* PushButton3;
    QPushButton* PushButton5;
    QPushButton* PushButton2;
    QPushButton* PushButton4;
    QPushButton* PushButton6;
    QPushButton* PushButton1;
    QSlider* Slider1;
    QRadioButton* CheckButton1;
    QPushButton* PushButton7;
    QRadioButton* CheckButton2;


protected:
    QGridLayout* KinematicGUI_Anim_QTDLayout;
    QGridLayout* GroupBox1Layout;
    QGridLayout* Layout1;
    QGridLayout* Layout2;
    QGridLayout* Layout3;
};

#endif // KINEMATICGUI_ANIM_QTD_H
