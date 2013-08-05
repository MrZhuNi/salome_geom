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

// File:        CurveCreator_EditPntsDlg.cxx
// Created:     Fri Jul  05 16:29:53 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_EditPntsDlg.h>
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
CurveCreator_EditPntsDlg::CurveCreator_EditPntsDlg
          (QWidget* parent, const CurveCreator::Dimension theDimension)
  : QDialog      (parent),
    myDimension  (theDimension),
    myPntsList   (NULL),
    myXSpn       (NULL),
    myYSpn       (NULL),
    myZSpn       (NULL),
    myAddBtn     (NULL),
    myModifBtn   (NULL),
    myRmBtn      (NULL),
    myClearBtn   (NULL),
    myPntUpBtn   (NULL),
    myPntDownBtn (NULL),
    myOkBtn      (NULL),
    myCancelBtn  (NULL)
{
  setWindowTitle(tr("CC_EDIT_POINTS_TITLE"));

  // Set Add/modify point group
  QGroupBox   *aModifPntGrp =
               new QGroupBox(tr("CC_EDIT_POINTS_ADD_MODIFY"));
  QGridLayout *aModifPntLO  = new QGridLayout(aModifPntGrp);
  QLabel      *aXLbl        =
               new QLabel(tr("CC_EDIT_POINTS_X"), aModifPntGrp);
  QLabel      *aYLbl        =
               new QLabel(tr("CC_EDIT_POINTS_Y"), aModifPntGrp);

  aXLbl->setAlignment(Qt::AlignRight);
  aYLbl->setAlignment(Qt::AlignRight);
  myXSpn     = new QDoubleSpinBox(aModifPntGrp);
  myYSpn     = new QDoubleSpinBox(aModifPntGrp);
  myAddBtn   = new QPushButton(tr("CC_EDIT_POINTS_ADD"), aModifPntGrp);
  myModifBtn = new QPushButton(tr("CC_EDIT_POINTS_MODIFY"), aModifPntGrp);
  myRmBtn    = new QPushButton(tr("CC_EDIT_POINTS_REMOVE"), aModifPntGrp);
  aModifPntLO->setMargin(9);
  aModifPntLO->setSpacing(6);
  aModifPntLO->addWidget(aXLbl,      0, 0);
  aModifPntLO->addWidget(aYLbl,      1, 0);
  aModifPntLO->addWidget(myXSpn,      0, 1);
  aModifPntLO->addWidget(myYSpn,      1, 1);
  aModifPntLO->addWidget(myAddBtn,   0, 2);
  aModifPntLO->addWidget(myModifBtn, 1, 2);
  aModifPntLO->addWidget(myRmBtn,    2, 2);

  if (myDimension == CurveCreator::Dim3d) {
    QLabel   *aZLbl = new QLabel(tr("CC_EDIT_POINTS_Z"), aModifPntGrp);

    aZLbl->setAlignment(Qt::AlignRight);
    myZSpn = new QDoubleSpinBox(aModifPntGrp);
    aModifPntLO->addWidget(aZLbl, 2, 0);
    aModifPntLO->addWidget(myZSpn, 2, 1);
  }

  // Set Buttons group
  QGroupBox   *aPntsGrp     = new QGroupBox();
  QGridLayout *aPntsLO      = new QGridLayout(aPntsGrp);

  myClearBtn   = new QPushButton(tr("CC_EDIT_POINTS_CLEAR"), aModifPntGrp);
  myPntUpBtn   = new QPushButton(tr("CC_EDIT_POINTS_UP"), aPntsGrp);
  myPntDownBtn = new QPushButton(tr("CC_EDIT_POINTS_DOWN"), aPntsGrp);
  myPntsList   = new QListWidget(aPntsGrp);
  aPntsLO->setMargin(9);
  aPntsLO->setSpacing(6);
  aPntsLO->addWidget(myClearBtn,   0, 0);
  aPntsLO->addWidget(myPntUpBtn,   2, 4);
  aPntsLO->addWidget(myPntDownBtn, 3, 4);
  aPntsLO->addWidget(myPntsList,   1, 0, 4, 4);

  // Set OK/Cancel buttons group
  QGroupBox *anOkCancelGrp  = new QGroupBox();
  QGridLayout *anOkCancelLO = new QGridLayout(anOkCancelGrp);

  myOkBtn     = new QPushButton(tr("GEOM_BUT_OK"), anOkCancelGrp);
  myCancelBtn = new QPushButton(tr("GEOM_BUT_CANCEL"), anOkCancelGrp);
  anOkCancelLO->setMargin(9);
  anOkCancelLO->setSpacing(6);
  anOkCancelLO->addWidget(myOkBtn,     0, 3);
  anOkCancelLO->addWidget(myCancelBtn, 0, 4);

  // Set main group
  QGroupBox   *aMainGrp = new QGroupBox;
  QVBoxLayout *aMainLO = new QVBoxLayout(aMainGrp);

  aMainLO->addWidget(aModifPntGrp);
  aMainLO->addWidget(aPntsGrp);
  aMainLO->addWidget(anOkCancelGrp);

  setLayout(aMainLO);

  init();
}

//=======================================================================
// function: Destructor
// purpose:
//=======================================================================
CurveCreator_EditPntsDlg::~CurveCreator_EditPntsDlg()
{
}

//=======================================================================
// function: setPoints
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::setPoints
                    (const CurveCreator::Coordinates &thePoints)
{
  myPoints = thePoints;
  updateEditList();
}

//=======================================================================
// function: getPoints
// purpose:
//=======================================================================
const CurveCreator::Coordinates &CurveCreator_EditPntsDlg::getPoints() const
{
  return myPoints;
}

//=======================================================================
// function: init
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::init()
{
  // Init spin boxes.
  initSpinBox(myXSpn);
  initSpinBox(myYSpn);

  if (myDimension == CurveCreator::Dim3d) {
    initSpinBox(myZSpn);
  }

  // Init buttons.
  myModifBtn->setEnabled(false);
  myRmBtn->setEnabled(false);
  myClearBtn->setEnabled(false);
  myPntUpBtn->setEnabled(false);
  myPntDownBtn->setEnabled(false);
  myOkBtn->setDefault(true);

  connect(myAddBtn,     SIGNAL(clicked()), this, SLOT(appendPoint()));
  connect(myModifBtn,   SIGNAL(clicked()), this, SLOT(modifyPoint()));
  connect(myRmBtn,      SIGNAL(clicked()), this, SLOT(removePoint()));
  connect(myClearBtn,   SIGNAL(clicked()), this, SLOT(clear()));
  connect(myPntUpBtn,   SIGNAL(clicked()), this, SLOT(upPoint()));
  connect(myPntDownBtn, SIGNAL(clicked()), this, SLOT(downPoint()));
  connect(myOkBtn,      SIGNAL(clicked()), this, SLOT(accept()));
  connect(myCancelBtn,  SIGNAL(clicked()), this, SLOT(reject()));

  // Init list widget.
  myPntsList->clear();
  myPntsList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  myPntsList->setDragEnabled(true);
  myPntsList->setDragDropMode(QAbstractItemView::InternalMove);
  myPntsList->viewport()->setAcceptDrops(true);

  connect(myPntsList, SIGNAL(itemSelectionChanged()),
          this, SLOT(changeSelection()));
  connect(this, SIGNAL(numberOfItemsChanged(int)),
          this, SLOT(onNumberOfItemsChanged(int)));

  // Set tab order.
  setTabOrder();
}

//=======================================================================
// function: initSpinBox
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::initSpinBox(QDoubleSpinBox *theSpinBox)
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

//=======================================================================
// function: updateEditList
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::updateEditList()
{
  myPntsList->clear();

  const int aNbCoords = myPoints.size();

  if (aNbCoords % myDimension == 0) {
    int i = 0;

    while (i < aNbCoords) {
      const CurveCreator::TypeCoord aX = myPoints[i++];
      const CurveCreator::TypeCoord aY = myPoints[i++];

      if (myDimension == CurveCreator::Dim3d) {
        const CurveCreator::TypeCoord aZ = myPoints[i++];

        new CurveCreator_PointItem(aX, aY, aZ, myPntsList);
      } else {
        new CurveCreator_PointItem(aX, aY, myPntsList);
      }
    }
  }

  emit numberOfItemsChanged(myPntsList->count());
}

//=======================================================================
// function: movePoints
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::movePoints(const int theShift)
{
  // Sort list items in ascending or descending order depending on
  // the sign of theShift.
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();

  if (!aListItems.empty() && theShift != 0) {
    QMap<int, QListWidgetItem *> aMapItems;

    foreach(QListWidgetItem *anItem, aListItems) {
      int aRow = myPntsList->row(anItem);

      if (theShift > 0) {
        aRow = -aRow;
      }

      aMapItems.insert(aRow, anItem);
    }

    // Compute new rows
    QList<int> aListRows = aMapItems.keys();
    QList<int> aListNewRows;
    int i;
    const int aSize = aListRows.size();


    if (theShift < 0) {
      // Check each row to be positive.
      int aMinIndex = 0;

      for (i = 0; i < aSize; i++) {
        int aRow = aListRows[i] + theShift;

        if (aRow < aMinIndex) {
          aRow = aMinIndex++;
        }

        aListNewRows.append(aRow);
      }
    } else {
      // Check each row to be not greater then a myPntsList's size.
      int aMaxIndex = myPntsList->count() - 1;

      for (i = 0; i < aSize; i++) {
        int aRow = -aListRows[i] + theShift;

        if (aRow > aMaxIndex) {
          aRow = aMaxIndex--;
        }

        aListRows[i] = -aListRows[i];
        aListNewRows.append(aRow);
      }
    }

    // Move each item to another position.
    for (i = 0; i < aSize; i++) {
      if (aListRows[i] != aListNewRows[i]) {
        QListWidgetItem *anItem = myPntsList->takeItem(aListRows[i]);

        myPntsList->insertItem(aListNewRows[i], anItem);
      }
    }

    // Select added items.
    foreach (int anIndex, aListNewRows) {
      myPntsList->item(anIndex)->setSelected(true);
    }
  }
}

//=======================================================================
// function: setTabOrder
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::setTabOrder()
{
  QWidget::setTabOrder(myXSpn, myYSpn);

  if (myDimension == CurveCreator::Dim3d) {
    QWidget::setTabOrder(myYSpn, myZSpn);
    QWidget::setTabOrder(myZSpn, myAddBtn);
  } else {
    QWidget::setTabOrder(myYSpn, myAddBtn);
  }

  QWidget::setTabOrder(myAddBtn, myModifBtn);
  QWidget::setTabOrder(myModifBtn, myRmBtn);
  QWidget::setTabOrder(myRmBtn, myClearBtn);
  QWidget::setTabOrder(myClearBtn, myPntsList);
  QWidget::setTabOrder(myPntsList, myPntUpBtn);
  QWidget::setTabOrder(myPntUpBtn, myPntDownBtn);
  QWidget::setTabOrder(myPntDownBtn, myOkBtn);
  QWidget::setTabOrder(myOkBtn, myCancelBtn);
}

//=======================================================================
// function: appendPoint
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::appendPoint()
{
  if (myDimension == CurveCreator::Dim3d) {
    new CurveCreator_PointItem(myXSpn->value(), myYSpn->value(),
                               myZSpn->value(), myPntsList);
  } else {
    new CurveCreator_PointItem(myXSpn->value(), myYSpn->value(), myPntsList);
  }

  emit numberOfItemsChanged(myPntsList->count());
}

//=======================================================================
// function: modifyPoint
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::modifyPoint()
{
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();

  if (aListItems.size() == 1) {
    CurveCreator_PointItem *aPntItem =
      (CurveCreator_PointItem *)aListItems.first();

    if (myDimension == CurveCreator::Dim3d) {
      aPntItem->setCoord(myXSpn->value(), myYSpn->value(), myZSpn->value());
    } else {
      aPntItem->setCoord(myXSpn->value(), myYSpn->value());
    }
  }
}

//=======================================================================
// function: removePoint
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::removePoint()
{
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();
  int aRow = -1;

  foreach(QListWidgetItem *anItem, aListItems) {
    if (aRow < 0) {
      aRow = myPntsList->row(anItem);
    }

    delete anItem;
  }

  if (aRow >= 0) {
    emit numberOfItemsChanged(myPntsList->count());
  }

  // Set the new selection.
  if (aRow >= myPntsList->count()) {
    aRow = myPntsList->count() - 1;
  }

  if (aRow >= 0) {
    myPntsList->item(aRow)->setSelected(true);
  }
}

//=======================================================================
// function: upPoint
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::upPoint()
{
  movePoints(-1);
}

//=======================================================================
// function: downPoint
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::downPoint()
{
  movePoints(1);
}

//=======================================================================
// function: changeSelection
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::changeSelection()
{
  // Update modify button and spin boxes.
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();
  const int aNbItems = aListItems.size();

  if (aNbItems == 1) {
    const CurveCreator_PointItem *aPntItem =
      (const CurveCreator_PointItem *)aListItems.first();

    myModifBtn->setEnabled(true);
    myXSpn->setValue(aPntItem->getX());
    myYSpn->setValue(aPntItem->getY());

    if (myDimension == CurveCreator::Dim3d) {
      myZSpn->setValue(aPntItem->getZ());
    }
  } else if (myModifBtn->isEnabled()) {
    myModifBtn->setEnabled(false);
    myXSpn->setValue(0.0);
    myYSpn->setValue(0.0);

    if (myDimension == CurveCreator::Dim3d) {
      myZSpn->setValue(0.0);
    }
  }

  // Set enabled remove, up and down points.
  bool isEnabled = (aNbItems > 0);

  myRmBtn->setEnabled(isEnabled);
  isEnabled &= (aNbItems < myPntsList->count());
  myPntUpBtn->setEnabled(isEnabled);
  myPntDownBtn->setEnabled(isEnabled);
}

//=======================================================================
// function: accept
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::accept()
{
  // Copy points
  myPoints.clear();

  const int aNbPoints = myPntsList->count();
  int i;

  for (i = 0; i < aNbPoints; i++) {
    const CurveCreator_PointItem *aPntItem =
      (const CurveCreator_PointItem *)myPntsList->item(i);

    myPoints.push_back(aPntItem->getX());
    myPoints.push_back(aPntItem->getY());

    if (myDimension == CurveCreator::Dim3d) {
      myPoints.push_back(aPntItem->getZ());
    }
  }

  QDialog::accept();
}

//=======================================================================
// function: clear
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::clear()
{
  bool isEmpty = (myPntsList->count() == 0);

  myPntsList->clear();

  if (!isEmpty) {
    emit onNumberOfItemsChanged(0);
  }
}

//=======================================================================
// function: onNumberOfItemsChanged
// purpose:
//=======================================================================
void CurveCreator_EditPntsDlg::onNumberOfItemsChanged(int theNewValue)
{
  myClearBtn->setEnabled(theNewValue > 0);

  // Update Up and down buttons
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();
  const int aNbItems = aListItems.size();
  const bool isEnabled = (aNbItems > 0 && aNbItems < theNewValue);

  myPntUpBtn->setEnabled(isEnabled);
  myPntDownBtn->setEnabled(isEnabled);
}
