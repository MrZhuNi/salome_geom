#include "CurveCreator_NewSectionDlg.h"
#include "CurveCreator_Curve.hxx"

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>

CurveCreator_NewSectionDlg::CurveCreator_NewSectionDlg( QWidget *parent ) :
  QDialog(parent)
{
  std::string aNameStr;
  QGridLayout* aLay = new QGridLayout();
  QLabel* aLbl = new QLabel(tr("NAME"), this);
  myName = new QLineEdit(this);
  aLay->addWidget(aLbl, 0, 0);
  aLay->addWidget(myName, 0 , 1);

  aLbl = new QLabel(tr("LINE_TYPE"));
  myLineType = new QComboBox(this);

  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap aPolylinePixmap(aResMgr->loadPixmap("GEOM", tr("ICON_CC_POLYLINE")));
  QPixmap aSplinePixmap(aResMgr->loadPixmap("GEOM", tr("ICON_CC_SPLINE")));

//  QPixmap aPolylinePixmap = QPixmap(tr(":images/ICON_POLYLINE"));
//  QPixmap aSplinePixmap = QPixmap(tr(":images/ICON_SPLINE"));
  myLineType->addItem(aPolylinePixmap, tr("POLYLINE_TYPE"));
  myLineType->addItem(aSplinePixmap, tr("SPLINE_TYPE"));
  myLineType->setCurrentIndex(0);
  aLay->addWidget(aLbl, 1, 0);
  aLay->addWidget(myLineType, 1 , 1);

  aLbl = new QLabel(tr("LINE_CLOSED"));
  myIsClosed = new QCheckBox(this);
  aLay->addWidget(aLbl, 2, 0);
  aLay->addWidget(myIsClosed, 2, 1);

  myBtnBox = new QDialogButtonBox(this);
  myAddBtn = myBtnBox->addButton(tr("ADD_BTN"), QDialogButtonBox::AcceptRole );
  myContBtn = myBtnBox->addButton(tr("ADD_CONTINUE_BTN"), QDialogButtonBox::ResetRole );
  myBtnBox->addButton(tr("CANCEL"), QDialogButtonBox::RejectRole );

  connect( myBtnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect( myBtnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect( myBtnBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onBtnClicked(QAbstractButton*) ));

  QVBoxLayout* aMainLay = new QVBoxLayout();
  aMainLay->addLayout(aLay);
  aMainLay->addWidget(myBtnBox);
  setLayout(aMainLay);
}

void CurveCreator_NewSectionDlg::setSectionParameters( const QString& theName, bool isClosed, CurveCreator::Type theType )
{
  myName->setText(theName);
  myIsClosed->setChecked(isClosed);
  if( theType == CurveCreator::Polyline )
    myLineType->setCurrentIndex(0);
  else
    myLineType->setCurrentIndex(1);
}

void CurveCreator_NewSectionDlg::clear()
{
  myName->setText("");
  myIsClosed->setChecked(true);
  myLineType->setCurrentIndex(0);
}

void CurveCreator_NewSectionDlg::setEditMode( bool isEdit )
{
  myIsEdit = isEdit;
  if( myIsEdit ){
    myContBtn->hide();
    myAddBtn->setText(tr("OK"));
  }
  else{
    myContBtn->show();
    myAddBtn->setText(tr("ADD_BTN"));
  }
  updateTitle();
}

QString CurveCreator_NewSectionDlg::getName() const
{
  return myName->text();
}

bool CurveCreator_NewSectionDlg::isClosed() const
{
  return myIsClosed->isChecked();
}

CurveCreator::Type CurveCreator_NewSectionDlg::getSectionType() const
{
  if( myLineType->currentIndex() == 0 )
    return CurveCreator::Polyline;
  else
    return CurveCreator::BSpline;
}

void CurveCreator_NewSectionDlg::updateTitle()
{
  QString aTitle;
  if( !myIsEdit )
    aTitle = tr("ADD_NEW_SECTION");
  else
    aTitle = QString(tr("SET_SECTION_PARAMETERS"));
  setWindowTitle(aTitle);
}

void CurveCreator_NewSectionDlg::setSectionName( const QString& theName )
{
  myName->setText(theName);
}

void CurveCreator_NewSectionDlg::onBtnClicked(QAbstractButton* theBtn )
{
  if( myBtnBox->buttonRole(theBtn) == QDialogButtonBox::ResetRole ){
    emit addSection();
  }
}
