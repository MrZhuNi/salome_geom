#ifndef CURVECREATOR_NEWSECTION_H
#define CURVECREATOR_NEWSECTION_H

#include "CurveCreator.hxx"

#include <QDockWidget>

class CurveCreator_Curve;

class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;
class QAbstractButton;
class QDialogButtonBox;
class QFrame;

class CurveCreator_NewSectionDlg : public QWidget
{
  Q_OBJECT
public:
  explicit CurveCreator_NewSectionDlg(QWidget *parent = 0);

  QString getName() const;
  bool    isClosed() const;
  CurveCreator::Type getSectionType() const;

  void    setSectionParameters( const QString& theName, bool isClosed, CurveCreator::Type theType );
  void    setSectionName(const QString& theName );
  void    clear();
  void    setEditMode( bool isEdit );

signals:
  void    addSection();
  void    modifySection();
  void    cancelSection();
public slots:
protected slots:
protected:
  void    updateTitle();
private:
  QFrame*             myBtnFrame;
  QLineEdit*          myName;
  QComboBox*          myLineType;
  QCheckBox*          myIsClosed;
  bool                myIsEdit;
  QPushButton*        myAddBtn;
  QPushButton*        myCancelBtn;
};

#endif // CURVECREATOR_NEWSECTION_H
