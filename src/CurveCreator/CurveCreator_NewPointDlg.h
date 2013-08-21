#ifndef CURVECREATOR_NEWPOINTDLG_H
#define CURVECREATOR_NEWPOINTDLG_H

#include "CurveCreator.hxx"

#include <QDialog>

class QDoubleSpinBox;
class QDialogButtonBox;
class QAbstractButton;
class QPushButton;
class QLabel;

class CurveCreator_NewPointDlg : public QDialog
{
  Q_OBJECT
public:
  explicit CurveCreator_NewPointDlg(CurveCreator::Dimension theDim, QWidget *parent = 0);
  CurveCreator::Coordinates getCoordinates() const;
  void clear();
  void setSectionName( const QString& theName );
  void setEditMode( bool isEdit );
  void setCoordinates( const CurveCreator::Coordinates& theCoords );
  void setDimension(CurveCreator::Dimension theDim);
signals:
  void addPoint();
public slots:
protected slots:
  void onBtnClicked(QAbstractButton* theBtn );
protected:
  void updateTitle();
  void initSpinBox(QDoubleSpinBox *theSpinBox);
private:
  QDialogButtonBox*       myBtnBox;
  CurveCreator::Dimension myDim;
  QDoubleSpinBox*         myX;
  QDoubleSpinBox*         myY;
  QDoubleSpinBox*         myZ;
  QLabel*                 myZLabel;
  QPushButton*            myContBtn;
  QPushButton*            myAddBtn;
  bool                    myIsEdit;
  QString                 mySectionName;
};

#endif // CURVECREATOR_NEWPOINTDLG_H
