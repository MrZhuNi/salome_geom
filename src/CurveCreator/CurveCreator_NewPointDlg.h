#ifndef CURVECREATOR_NEWPOINTDLG_H
#define CURVECREATOR_NEWPOINTDLG_H

#include "CurveCreator.hxx"

#include <QDockWidget>

class QDoubleSpinBox;
class QDialogButtonBox;
class QAbstractButton;
class QPushButton;
class QLabel;
class QFrame;

class CurveCreator_NewPointDlg : public QWidget
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
  bool isInstantSketchingEnabled() const;
  void setInstantSketchingEnabled( const bool theState );
signals:
  void addPoint();
  void modifyPoint();
  void cancelPoint();
public slots:
protected slots:
protected:
  void updateTitle();
  void initSpinBox(QDoubleSpinBox *theSpinBox);
private:
  QFrame*                 myBtnFrame;
  CurveCreator::Dimension myDim;
  QDoubleSpinBox*         myX;
  QDoubleSpinBox*         myY;
  QDoubleSpinBox*         myZ;
  QLabel*                 myZLabel;
  QPushButton*            myAddBtn;
  QPushButton*            myCancelBtn;
  bool                    myIsEdit;
  QString                 mySectionName;
  bool                    myIsInstantSketchingEnabled;
};

#endif // CURVECREATOR_NEWPOINTDLG_H
