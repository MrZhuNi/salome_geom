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
  QWidget(parent)
{
  QFrame* aFrame = new QFrame( this );
  QVBoxLayout* aLayout = new QVBoxLayout( aFrame );

  QFrame* aCoordFrame = new QFrame( aFrame );
  QGridLayout* aCoordLayout = new QGridLayout( aCoordFrame );

  QLabel* aLbl = new QLabel(tr("NAME"), this);
  myName = new QLineEdit(this);
  aCoordLayout->addWidget(aLbl, 0, 0);
  aCoordLayout->addWidget(myName, 0 , 1);

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
  aCoordLayout->addWidget(aLbl, 1, 0);
  aCoordLayout->addWidget(myLineType, 1 , 1);

  aLbl = new QLabel(tr("LINE_CLOSED"));
  myIsClosed = new QCheckBox(this);
  aCoordLayout->addWidget(aLbl, 2, 0);
  aCoordLayout->addWidget(myIsClosed, 2, 1);

  myBtnFrame = new QFrame( aFrame );
  QHBoxLayout* aBtnsLayout = new QHBoxLayout( myBtnFrame );

  myAddBtn = new QPushButton( tr( "ADD_BTN" ), myBtnFrame );
  myCancelBtn = new QPushButton( tr( "CANCEL" ), myBtnFrame );

  connect( myAddBtn,  SIGNAL( clicked() ), this, SIGNAL( addSection() ) );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SIGNAL( cancelSection() ) );

  aBtnsLayout->addWidget( myAddBtn );
  aBtnsLayout->addStretch( 1 );
  aBtnsLayout->addWidget( myCancelBtn );

  aLayout->addWidget( aCoordFrame, 0 );
  aLayout->addWidget( myBtnFrame, 1 );
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
    myAddBtn->setText(tr("OK"));
    myAddBtn->disconnect( SIGNAL( clicked() ) );
    connect( myAddBtn, SIGNAL( clicked() ), this, SIGNAL( modifySection() ) );
  }
  else{
    myAddBtn->setText(tr("ADD_BTN"));
    myAddBtn->disconnect( SIGNAL( clicked() ) );
    connect( myAddBtn, SIGNAL( clicked() ), this, SIGNAL( addSection() ) );
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
