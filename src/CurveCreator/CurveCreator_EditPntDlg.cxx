// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File:        CurveCreator_EditPntDlg.cxx
// Created:     Tue Jul  16 10:58:31 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_EditPntDlg.h>
#include <CurveCreator_PointItem.h>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QListWidget>


//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_EditPntDlg::CurveCreator_EditPntDlg
          (QWidget* parent,
           const CurveCreator::Dimension theDimension)
  : QDialog     (parent),
    myDimension (theDimension),
    myXSpn      (NULL),
    myYSpn      (NULL),
    myZSpn      (NULL),
    myOkBtn     (NULL),
    myCancelBtn (NULL)
{
  setWindowTitle(tr("CC_EDIT_POINT_TITLE"));

  // Set Add/modify point group
  QGroupBox   *aModifPntGrp =
               new QGroupBox(tr("CC_EDIT_POINT_MODIFY"));
  QGridLayout *aModifPntLO  = new QGridLayout(aModifPntGrp);
  QLabel      *aXLbl        =
               new QLabel(tr("CC_EDIT_POINT_X"), aModifPntGrp);
  QLabel      *aYLbl        =
               new QLabel(tr("CC_EDIT_POINT_Y"), aModifPntGrp);

  aXLbl->setAlignment(Qt::AlignRight);
  aYLbl->setAlignment(Qt::AlignRight);
  myXSpn     = new QDoubleSpinBox(aModifPntGrp);
  myYSpn     = new QDoubleSpinBox(aModifPntGrp);
  aModifPntLO->setMargin(9);
  aModifPntLO->setSpacing(6);
  aModifPntLO->addWidget(aXLbl,      0, 0);
  aModifPntLO->addWidget(aYLbl,      1, 0);
  aModifPntLO->addWidget(myXSpn,      0, 1);
  aModifPntLO->addWidget(myYSpn,      1, 1);

  if (myDimension == CurveCreator::Dim3d) {
    QLabel   *aZLbl = new QLabel(tr("CC_EDIT_POINT_Z"), aModifPntGrp);

    aZLbl->setAlignment(Qt::AlignRight);
    myZSpn = new QDoubleSpinBox(aModifPntGrp);
    aModifPntLO->addWidget(aZLbl, 2, 0);
    aModifPntLO->addWidget(myZSpn, 2, 1);
  }

  // Set OK/Cancel buttons group
  QGroupBox   *anOkCancelGrp = new QGroupBox;
  QGridLayout *anOkCancelLO  = new QGridLayout(anOkCancelGrp);

  myOkBtn     = new QPushButton(tr("GEOM_BUT_OK"), anOkCancelGrp);
  myCancelBtn = new QPushButton(tr("GEOM_BUT_CANCEL"), anOkCancelGrp);
  anOkCancelLO->setMargin(9);
  anOkCancelLO->setSpacing(6);
  anOkCancelLO->addWidget(myOkBtn,     0, 0);
  anOkCancelLO->addWidget(myCancelBtn, 0, 1);

  // Set main group
  QGroupBox   *aMainGrp = new QGroupBox;
  QVBoxLayout *aMainLO = new QVBoxLayout(aMainGrp);

  aMainLO->addWidget(aModifPntGrp);
  aMainLO->addWidget(anOkCancelGrp);

  setLayout(aMainLO);

  init();
}

//=======================================================================
// function: Destructor
// purpose:
//=======================================================================
CurveCreator_EditPntDlg::~CurveCreator_EditPntDlg()
{
}

//=======================================================================
// function: setPoint
// purpose:
//=======================================================================
void CurveCreator_EditPntDlg::setPoint
                    (const CurveCreator::Coordinates &thePoint)
{
  myPoint = thePoint;

  if (myPoint.size() == myDimension) {
    myXSpn->setValue(myPoint[0]);
    myYSpn->setValue(myPoint[1]);

    if (myDimension == CurveCreator::Dim3d) {
      myZSpn->setValue(myPoint[2]);
    }
  }
}

//=======================================================================
// function: getPoint
// purpose:
//=======================================================================
const CurveCreator::Coordinates &CurveCreator_EditPntDlg::getPoint() const
{
  return myPoint;
}

//=======================================================================
// function: init
// purpose:
//=======================================================================
void CurveCreator_EditPntDlg::init()
{
  // Init spin boxes.
  initSpinBox(myXSpn);
  initSpinBox(myYSpn);

  if (myDimension == CurveCreator::Dim3d) {
    initSpinBox(myZSpn);
  }

  // Init buttons.
  myOkBtn->setDefault(true);

  connect(myOkBtn,     SIGNAL(clicked()), this, SLOT(accept()));
  connect(myCancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

  setTabOrder();
}

//=======================================================================
// function: initSpinBox
// purpose:
//=======================================================================
void CurveCreator_EditPntDlg::initSpinBox(QDoubleSpinBox *theSpinBox)
{
  const double aCoordMin  = -1.e+15;
  const double aCoordMax  = 1.e+15;
  const double aStep      = 10;
  const int    aPrecision = 6;

  theSpinBox->setDecimals( qAbs( aPrecision ) );
  theSpinBox->setRange(aCoordMin, aCoordMax);
  theSpinBox->setSingleStep(aStep);
  theSpinBox->setValue(0.);
}

//=======================================================================
// function: setTabOrder
// purpose:
//=======================================================================
void CurveCreator_EditPntDlg::setTabOrder()
{
  QWidget::setTabOrder(myXSpn, myYSpn);

  if (myDimension == CurveCreator::Dim3d) {
    QWidget::setTabOrder(myYSpn, myZSpn);
    QWidget::setTabOrder(myZSpn, myOkBtn);
  } else {
    QWidget::setTabOrder(myYSpn, myOkBtn);
  }

  QWidget::setTabOrder(myOkBtn, myCancelBtn);
}

//=======================================================================
// function: accept
// purpose:
//=======================================================================
void CurveCreator_EditPntDlg::accept()
{
  // Copy point
  myPoint.clear();
  myPoint.push_back(myXSpn->value());
  myPoint.push_back(myYSpn->value());

  if (myDimension == CurveCreator::Dim3d) {
    myPoint.push_back(myZSpn->value());
  }

  QDialog::accept();
}
