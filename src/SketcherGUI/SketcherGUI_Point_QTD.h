/****************************************************************************
** Form interface generated from reading ui file 'SketcherGUI_Point_QTD.ui'
**
** Created: lun nov 3 17:27:53 2003
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef SKETCHERGUI_POINT_QTD_H
#define SKETCHERGUI_POINT_QTD_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QRadioButton;

class SketcherGUI_Point_QTD : public QWidget
{ 
    Q_OBJECT

public:
    SketcherGUI_Point_QTD( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SketcherGUI_Point_QTD();

    QButtonGroup* GroupPoint;
    QRadioButton* RB_Point2;
    QRadioButton* RB_Point1;
    QRadioButton* RB_Point3;


protected:
    QGridLayout* SketcherGUI_Point_QTDLayout;
    QGridLayout* GroupPointLayout;
    QGridLayout* Layout1;
};

#endif // SKETCHERGUI_POINT_QTD_H
