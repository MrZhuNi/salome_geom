#ifndef CURVECREATOR_NEWSECTION_H
#define CURVECREATOR_NEWSECTION_H

#include "CurveCreator.hxx"

#include <QDialog>

class CurveCreator_Curve;

class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;
class QAbstractButton;
class QDialogButtonBox;

class CurveCreator_NewSectionDlg : public QDialog
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
public slots:
protected slots:
    void onBtnClicked(QAbstractButton* theBtn );
protected:
    void    updateTitle();
private:
    QLineEdit*          myName;
    QComboBox*          myLineType;
    QCheckBox*          myIsClosed;
    bool                myIsEdit;
    QPushButton*        myContBtn;
    QPushButton*        myAddBtn;
    QDialogButtonBox*   myBtnBox;
};

#endif // CURVECREATOR_NEWSECTION_H
