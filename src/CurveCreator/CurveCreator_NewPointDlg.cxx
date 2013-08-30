#include "CurveCreator_NewPointDlg.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QDoubleValidator>
#include <QRegExpValidator>
#include <QAbstractButton>
#include <QPushButton>
#include <QLocale>

CurveCreator_NewPointDlg::CurveCreator_NewPointDlg(CurveCreator::Dimension theDim, QWidget *parent) :
  QDialog(parent), myX(NULL), myY(NULL), myZ(NULL), myIsEdit(false), myDim(theDim)
{
  QGridLayout* aCoordLay = new QGridLayout();

  QString aTitle = QString(tr("ADD_NEW_POINT"));
  setWindowTitle(aTitle);

  QLabel* aLbl = new QLabel( tr("X_COORD"), this);
  myX = new QDoubleSpinBox(this);
  aCoordLay->addWidget(aLbl, 0, 0);
  aCoordLay->addWidget(myX, 0, 1 );

  aLbl = new QLabel( tr("Y_COORD"), this);
  myY = new QDoubleSpinBox(this);
  aCoordLay->addWidget(aLbl, 1, 0 );
  aCoordLay->addWidget(myY, 1, 1 );

  myZLabel = new QLabel( tr("Z_COORD"), this);
  myZ = new QDoubleSpinBox(this);
  aCoordLay->addWidget(myZLabel, 2,0 );
  aCoordLay->addWidget(myZ, 2,1 );

  if( theDim != CurveCreator::Dim3d ){
    myZ->hide();
    myZLabel->hide();
  }

  myBtnBox = new QDialogButtonBox(this);
  myAddBtn = myBtnBox->addButton(tr("ADD_BTN"), QDialogButtonBox::AcceptRole );
  myContBtn = myBtnBox->addButton(tr("ADD_CONTINUE_BTN"), QDialogButtonBox::ResetRole );
  myBtnBox->addButton(tr("CANCEL"), QDialogButtonBox::RejectRole );

  connect( myBtnBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect( myBtnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect( myBtnBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onBtnClicked(QAbstractButton*) ));
  QVBoxLayout* aMainLay = new QVBoxLayout();
  aMainLay->addLayout(aCoordLay);
  aMainLay->addWidget(myBtnBox);
  setLayout(aMainLay);
  clear();
  updateTitle();
}

void CurveCreator_NewPointDlg::setSectionName( const QString& theName )
{
  mySectionName = theName;
  updateTitle();
}

void CurveCreator_NewPointDlg::setEditMode( bool isEdit )
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

void CurveCreator_NewPointDlg::updateTitle()
{
  QString aTitle;
  if( !myIsEdit ){
    if( mySectionName.isEmpty() ){
      aTitle = tr("ADD_NEW_POINT");
    }
    else{
      aTitle = QString(tr("ADD_NEW_POINT_TO_%1")).arg(mySectionName);
    }
  }
  else{
    aTitle = tr("SET_POINT_COORDINATES");
  }
  setWindowTitle(aTitle);
}

CurveCreator::Coordinates CurveCreator_NewPointDlg::getCoordinates() const
{
  CurveCreator::Coordinates aCoords;
  double anX = myX->value();
  aCoords.push_back(anX);
  double anY = myY->value();
  aCoords.push_back(anY);
  if( myDim == CurveCreator::Dim3d ){
    double aZ = myZ->value();
    aCoords.push_back(aZ);
  }
  return aCoords;
}

void CurveCreator_NewPointDlg::onBtnClicked(QAbstractButton* theBtn )
{
  if( myBtnBox->buttonRole(theBtn) == QDialogButtonBox::ResetRole ){
    emit addPoint();
  }
}

void CurveCreator_NewPointDlg::clear()
{
  initSpinBox(myX);
  initSpinBox(myY);
  initSpinBox(myZ);
}

void CurveCreator_NewPointDlg::setDimension(CurveCreator::Dimension theDim)
{
  if( theDim == CurveCreator::Dim2d ){
    myZ->hide();
    myZLabel->hide();
  }
  else{
    myZ->show();
    myZLabel->show();
  }
}

void CurveCreator_NewPointDlg::setCoordinates( const CurveCreator::Coordinates& theCoords )
{
  double anX = theCoords[0];
  myX->setValue(anX);
  double anY = theCoords[1];
  myY->setValue(anY);
  if( theCoords.size() == 3 ){
    double aZ = theCoords[2];
    myZ->setValue(aZ);
  }
}

//=======================================================================
// function: initSpinBox
// purpose:
//=======================================================================
void CurveCreator_NewPointDlg::initSpinBox(QDoubleSpinBox *theSpinBox)
{
  const double aCoordMin  = -1.e+15;
  const double aCoordMax  = 1.e+15;
  const double aStep      = 10;
  const int    aPrecision = 6;

  theSpinBox->setDecimals( qAbs( aPrecision ) );
  theSpinBox->setRange(aCoordMin, aCoordMax);
  theSpinBox->setSingleStep(aStep);
  theSpinBox->setValue(0.0);
}
