/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_Dir2_QTD.ui'
**
** Created: lun nov 3 17:27:54 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_DIR2_QTD_H
#define SKETCHERGUI_DIR2_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QRadioButton;

class SketcherGUI_Dir2_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_Dir2_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_Dir2_QTD();

    QButtonGroup* GroupDir2;
    QRadioButton* RB_Dir22;
    QRadioButton* RB_Dir23;
    QRadioButton* RB_Dir21;


protected:
    QGridLayout* SketcherGUI_Dir2_QTDLayout;
    QGridLayout* GroupDir2Layout;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_DIR2_QTD_H
