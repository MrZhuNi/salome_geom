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

// File:        CurveCreator_Widget.cxx
// Created:     Mon Jul  01 12:49:31 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_Widget.h>
#include <CurveCreator_EditPntsWidget.h>
#include <CurveCreator_UndoOptsDlg.h>
#include <CurveCreator_EditPntDlg.h>
#include <CurveCreator_PointItem.h>
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QListWidget>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QComboBox>
#include <QCheckBox>

#define SECTION_ADD          0
#define SECTION_MODIFY       1
#define POLYLINE_INDEX       0
#define BSPLINE_INDEX        1
#define UNDO_REDO_DISABLED   0
#define UNDO_REDO_UNLIMITED -1

//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_Widget::CurveCreator_Widget(QWidget* parent,
                                         CurveCreator_Curve *theCurve,
                                         Qt::WindowFlags fl)
  : QWidget          (parent, fl),
    myEditor         (theCurve),
    myDimension      (theCurve == NULL ?
                       CurveCreator::Dim2d : theCurve->getDimension()),
    myEnabledUndoLbl (NULL),
    myBufSizeUndoLbl (NULL),
    myUndoBtn        (NULL),
    myRedoBtn        (NULL),
    myUndoOptsBtn    (NULL),
    myUndoOptsDlg    (NULL),
    myAddSecGrp      (NULL),
    mySecBtnGrp      (NULL),
    mySecTypeCmbBox  (NULL),
    mySecCloseChkBox (NULL),
    mySecAddModifBtn (NULL),
    mySecRmBtn       (NULL),
    mySecJoinBtn     (NULL),
    mySecJoinAllBtn  (NULL),
    mySecClearBtn    (NULL),
    mySecUpBtn       (NULL),
    mySecDownBtn     (NULL),
    mySecList        (NULL),
    myEditSecPnts    (NULL),
    myPntsGrp        (NULL),
    myPntsList       (NULL),
    myPntEditDlg     (NULL),
    myEditPnts       (NULL),
    myAddPntsBtn     (NULL),
    myInsertPntsBtn  (NULL),
    myRmPntsBtn      (NULL)
{
  setWindowTitle(tr("CC_TITLE"));

  // Set undo/redo options group
  QGroupBox   *anUndoOptsGrp = new QGroupBox(tr("CC_UNDO_REDO_OPTIONS"));
  QGridLayout *anUndoOptsLO  = new QGridLayout(anUndoOptsGrp);

  myEnabledUndoLbl = new QLabel(tr("CC_UNDO_REDO_ENABLED"), anUndoOptsGrp);
  myBufSizeUndoLbl = new QLabel(tr("CC_UNDO_REDO_BUFFER_SIZE"), anUndoOptsGrp);
  myUndoOptsBtn    = new QPushButton(tr("CC_UNDO_REDO_MODIFY"), anUndoOptsGrp);
  anUndoOptsLO->setMargin(9);
  anUndoOptsLO->setSpacing(6);
  anUndoOptsLO->addWidget(myEnabledUndoLbl, 0, 0);
  anUndoOptsLO->addWidget(myBufSizeUndoLbl, 1, 0);
  anUndoOptsLO->addWidget(myUndoOptsBtn,    0, 1, 2, 1);

  // Set undo/redo group
  QGroupBox   *anUndoRedoGrp     = new QGroupBox(tr("CC_UNDO_REDO_TITLE"));
  QHBoxLayout *anUndoRedoLO      = new QHBoxLayout(anUndoRedoGrp);

  myUndoBtn = new QPushButton(tr("CC_UNDO"), anUndoRedoGrp);
  myRedoBtn = new QPushButton(tr("CC_REDO"), anUndoRedoGrp);
  anUndoRedoLO->setMargin(9);
  anUndoRedoLO->setSpacing(6);
  anUndoRedoLO->addWidget(myUndoBtn,     0, Qt::AlignRight);
  anUndoRedoLO->addWidget(myRedoBtn,     0, Qt::AlignLeft);
  anUndoRedoLO->addWidget(anUndoOptsGrp, 0, Qt::AlignRight);

  // Set section add/modification group.
  myAddSecGrp = new QGroupBox(tr("CC_SECTION_ADD_TITLE"));

  QGridLayout  *anAddSecLO      = new QGridLayout(myAddSecGrp);
  QLabel       *aSectTypeLbl    =
                new QLabel(tr("CC_SECTION_TYPE"), myAddSecGrp);
  QLabel       *aSecPntsLbl     =
                new QLabel(tr("CC_SECTION_POINTS_ADD_LBL"), myAddSecGrp);

  mySecCloseChkBox = new QCheckBox(tr("CC_SECTION_CLOSED"), myAddSecGrp);
  mySecTypeCmbBox  = new QComboBox(myAddSecGrp);
  mySecAddModifBtn = new QPushButton(tr("CC_SECTION_NEW"), myAddSecGrp);
  myEditSecPnts    = new CurveCreator_EditPntsWidget
                (myAddSecGrp, true, myDimension);
  anAddSecLO->setMargin(9);
  anAddSecLO->setSpacing(6);
  anAddSecLO->addWidget(mySecCloseChkBox, 0, 0);
  anAddSecLO->addWidget(aSectTypeLbl,     1, 0);
  anAddSecLO->addWidget(mySecTypeCmbBox,  1, 1);
  anAddSecLO->addWidget(aSecPntsLbl,      2, 0);
  anAddSecLO->addWidget(myEditSecPnts,    2, 1);
  anAddSecLO->addWidget(mySecAddModifBtn, 1, 3);

  // Set section group
  QGroupBox    *aSecGrp         = new QGroupBox(tr("CC_SECTION_TITLE"));
  QGridLayout  *aSecLO          = new QGridLayout(aSecGrp);
  QRadioButton *anAddSecRdBtn   =
               new QRadioButton(tr("CC_SECTION_ADD_TITLE"), aSecGrp);
  QRadioButton *aModifSecRdBtn  =
               new QRadioButton(tr("CC_SECTION_MODIFY_TITLE"), aSecGrp);

  mySecRmBtn      = new QPushButton(tr("CC_SECTION_REMOVE"), aSecGrp);
  mySecJoinBtn    = new QPushButton(tr("CC_SECTION_JOIN"), aSecGrp);
  mySecJoinAllBtn = new QPushButton(tr("CC_SECTION_JOIN_ALL"), aSecGrp);
  mySecClearBtn   = new QPushButton(tr("CC_SECTION_CLEAR"), aSecGrp);
  mySecUpBtn      = new QPushButton(tr("CC_SECTION_UP"), aSecGrp);
  mySecDownBtn    = new QPushButton(tr("CC_SECTION_DOWN"), aSecGrp);
  mySecBtnGrp     = new QButtonGroup(aSecGrp);
  mySecList       = new QListWidget(aSecGrp);
  aSecLO->setMargin(9);
  aSecLO->setSpacing(6);
  aSecLO->addWidget(anAddSecRdBtn,   0, 0);
  aSecLO->addWidget(aModifSecRdBtn,  0, 1);
  aSecLO->addWidget(myAddSecGrp,     1, 0, 1, 5);
  aSecLO->addWidget(mySecRmBtn,      2, 0);
  aSecLO->addWidget(mySecJoinBtn,    2, 1);
  aSecLO->addWidget(mySecJoinAllBtn, 2, 2);
  aSecLO->addWidget(mySecClearBtn,   2, 3);
  aSecLO->addWidget(mySecUpBtn,      4, 4);
  aSecLO->addWidget(mySecDownBtn,    5, 4);
  aSecLO->addWidget(mySecList,       3, 0, 4, 4);
  mySecBtnGrp->addButton(anAddSecRdBtn,  SECTION_ADD);
  mySecBtnGrp->addButton(aModifSecRdBtn, SECTION_MODIFY);

  // Set point add/modification group.
  QGroupBox    *anAddPntsGrp   = new QGroupBox(tr("CC_POINTS_ADD_TITLE"));

  QGridLayout  *anAddPntsLO    = new QGridLayout(anAddPntsGrp);
  QLabel       *aPntsLbl       =
                new QLabel(tr("CC_POINTS_ADD_LBL"), anAddPntsGrp);

  myEditPnts      = new CurveCreator_EditPntsWidget
                            (anAddPntsGrp, false, myDimension);
  myAddPntsBtn    = new QPushButton(tr("CC_POINTS_ADD"), anAddPntsGrp);
  myInsertPntsBtn = new QPushButton(tr("CC_POINTS_INSERT"), anAddPntsGrp);
  anAddPntsLO->setMargin(9);
  anAddPntsLO->setSpacing(6);
  anAddPntsLO->addWidget(aPntsLbl,        0, 0);
  anAddPntsLO->addWidget(myEditPnts,      0, 1);
  anAddPntsLO->addWidget(myAddPntsBtn,    0, 4);
  anAddPntsLO->addWidget(myInsertPntsBtn, 1, 4);

  // Set Points group
  myPntsGrp = new QGroupBox(tr("CC_POINTS_TITLE"));

  QGridLayout *aPntsLO = new QGridLayout(myPntsGrp);

  myPntsList  = new QListWidget(myPntsGrp);
  myRmPntsBtn = new QPushButton(tr("CC_POINTS_REMOVE"), myPntsGrp);
  aPntsLO->setMargin(9);
  aPntsLO->setSpacing(6);
  aPntsLO->addWidget(anAddPntsGrp, 0, 0, 1, 4);
  aPntsLO->addWidget(myRmPntsBtn,  1, 0);
  aPntsLO->addWidget(myPntsList,   2, 0, 4, 4);

  // Set main group
  QGroupBox   *aMainGrp = new QGroupBox(tr("CC_TITLE"));
  QGridLayout *aMainLO = new QGridLayout(aMainGrp);

  aMainLO->setMargin(9);
  aMainLO->setSpacing(6);
  aMainLO->addWidget(anUndoRedoGrp, 0, 0, 1, 2);
  aMainLO->addWidget(aSecGrp,       1, 0);
  aMainLO->addWidget(myPntsGrp,     1, 1);

  setLayout(aMainLO);

  init();
}

//=======================================================================
// function: init
// purpose:
//=======================================================================
void CurveCreator_Widget::init()
{
  // Initialize Undo/Redo group.
  QString anUnlim = tr("CC_UNDO_REDO_UNLIMITED");

  myEnabledUndoLbl->setText(tr("CC_UNDO_REDO_ENABLED"));
  myBufSizeUndoLbl->setText(tr("CC_UNDO_REDO_BUFFER_SIZE").arg(anUnlim));
  connect(myUndoOptsBtn, SIGNAL(clicked()), this, SLOT(undoOptionsChanged()));
  connect(myUndoBtn, SIGNAL(clicked()), this, SLOT(undo()));
  connect(myRedoBtn, SIGNAL(clicked()), this, SLOT(redo()));
  updateUndoRedoButtons();

  // Initialize sections group.
  mySecBtnGrp->setExclusive(true);
  mySecBtnGrp->button(SECTION_ADD)->setChecked(true);
  connect(mySecBtnGrp, SIGNAL(buttonClicked(int)),
          this, SLOT(addModifChanged(int)));

  // Init section buttons.
  mySecAddModifBtn->setEnabled(true);
  mySecRmBtn->setEnabled(false);
  mySecJoinBtn->setEnabled(false);
  mySecJoinAllBtn->setEnabled(false);
  mySecClearBtn->setEnabled(false);
  mySecUpBtn->setEnabled(false);
  mySecDownBtn->setEnabled(false);

  connect(mySecAddModifBtn, SIGNAL(clicked()),this, SLOT(sectionAddOrModify()));
  connect(mySecRmBtn,       SIGNAL(clicked()),this, SLOT(sectionRemove()));
  connect(mySecJoinBtn,     SIGNAL(clicked()),this, SLOT(sectionJoin()));
  connect(mySecJoinAllBtn,  SIGNAL(clicked()),this, SLOT(sectionJoinAll()));
  connect(mySecClearBtn,    SIGNAL(clicked()),this, SLOT(sectionClear()));
  connect(mySecUpBtn,       SIGNAL(clicked()),this, SLOT(sectionUp()));
  connect(mySecDownBtn,     SIGNAL(clicked()),this, SLOT(sectionDown()));

  // Init section input fields.
  mySecCloseChkBox->setChecked(false);
  mySecTypeCmbBox->addItem(tr("CC_SECTION_TYPE_POLYLINE"));
  mySecTypeCmbBox->addItem(tr("CC_SECTION_TYPE_BSPLINE"));
  mySecTypeCmbBox->setCurrentIndex(POLYLINE_INDEX);
  myEditSecPnts->clear();

  // Init sections list widget.
  mySecList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(this, SIGNAL(numberOfItemsChanged(QListWidget *)),
          this, SLOT(onNumberOfItemsChanged(QListWidget *)));
  initSections();
  connect(mySecList, SIGNAL(itemSelectionChanged()),
          this, SLOT(changeSecSelection()));

  // Init points list widget.
  myPntsGrp->setEnabled(false);
  myPntsList->clear();
  myPntsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(myPntsList, SIGNAL(itemSelectionChanged()),
          this, SLOT(changePntsSelection()));
  connect(myPntsList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          this, SLOT(editPnt(QListWidgetItem *)));

  // Init points buttons.
  connect(myAddPntsBtn,    SIGNAL(clicked()),this, SLOT(pntsAdd()));
  connect(myInsertPntsBtn, SIGNAL(clicked()),this, SLOT(pntsInsert()));
  connect(myRmPntsBtn,     SIGNAL(clicked()),this, SLOT(pntsRemove()));

  // Init edit points widgets.
  myEditSecPnts->setPointsList(myPntsList);
  myEditPnts->setPointsList(myPntsList);

  // Set tab order
  setTabOrder();
}

//=======================================================================
// function: initSections
// purpose:
//=======================================================================
void CurveCreator_Widget::initSections()
{
  CurveCreator_Curve *aCurve = myEditor.getCurve();

  const int aNbSections = aCurve->getNbSections();
  int i;

  mySecList->clear();

  for (i = 0; i < aNbSections; i++) {
    addSectionItem(aCurve->getType(i), aCurve->isClosed(i));
  }

  emit numberOfItemsChanged(mySecList);
}

//=======================================================================
// function: addSectionItem
// purpose:
//=======================================================================
void CurveCreator_Widget::addSectionItem(const CurveCreator::Type theType,
                                         const bool isClosed)
{
  const int aRow = mySecList->count();

  new QListWidgetItem(mySecList);
  updateSectionItem(aRow, theType, isClosed);
  emit numberOfItemsChanged(mySecList);
}

//=======================================================================
// function: updateSectionItem
// purpose:
//=======================================================================
void CurveCreator_Widget::updateSectionItem(const int theRow,
                                            const CurveCreator::Type theType,
                                            const bool isClosed)
{
  QListWidgetItem *anItem = mySecList->item(theRow);
  QString          aStrType;

  if (theType == CurveCreator::Polyline) {
    aStrType = tr("CC_SECTION_ITEM_POLYLINE");
  } else {
    aStrType = tr("CC_SECTION_ITEM_BSPLINE");
  }

  const QString aTrCode =
    (isClosed ? "CC_SECTION_ITEM_CLOSED" : "CC_SECTION_ITEM");

  anItem->setText(tr(aTrCode.toLatin1().constData()).arg(aStrType));
}

//=======================================================================
// function: sectionMove
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionMove(const int theShift)
{
  // Sort list items in ascending or descending order depending on
  // the sign of theShift.
  QList<QListWidgetItem *> aListItems = mySecList->selectedItems();

  if (!aListItems.empty() && theShift != 0) {
    QMap<int, QListWidgetItem *> aMapItems;

    foreach(QListWidgetItem *anItem, aListItems) {
      int aRow = mySecList->row(anItem);

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
      // Check each row to be not greater then a mySecList's size.
      int aMaxIndex = mySecList->count() - 1;

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
        QListWidgetItem *anItem = mySecList->takeItem(aListRows[i]);

        // Move section.
        mySecList->insertItem(aListNewRows[i], anItem);
        myEditor.moveSection(aListRows[i], aListNewRows[i]);
      }
    }

    // Select added items.
    foreach (int anIndex, aListNewRows) {
      mySecList->item(anIndex)->setSelected(true);
    }

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: initPoints
// purpose:
//=======================================================================
void CurveCreator_Widget::initPoints(const int theSectionIndex)
{
  // Clear the points list
  const int aNbPnts = myPntsList->count();

  myPntsList->clear();

  if (theSectionIndex < 0) {
    if (aNbPnts > 0) {
      emit numberOfItemsChanged(myPntsList);
    }
  } else {
    // Fill the points list with the points of the section.
    const CurveCreator::Coordinates &aPoints =
        myEditor.getCurve()->getPoints(theSectionIndex);
    const int aNbCoords = aPoints.size();

    if (aNbCoords % myDimension == 0) {
      int i = 0;

      while (i < aNbCoords) {
        const CurveCreator::TypeCoord aX = aPoints[i++];
        const CurveCreator::TypeCoord aY = aPoints[i++];

        if (myDimension == CurveCreator::Dim3d) {
          const CurveCreator::TypeCoord aZ = aPoints[i++];

          new CurveCreator_PointItem(aX, aY, aZ, myPntsList);
        } else {
          new CurveCreator_PointItem(aX, aY, myPntsList);
        }
      }
    }

    if (aNbPnts != aNbCoords/myDimension) {
      emit numberOfItemsChanged(myPntsList);
    }
  }
}

//=======================================================================
// function: getCurrentSectionIndex
// purpose:
//=======================================================================
int CurveCreator_Widget::getCurrentSectionIndex()
{
  int anIndex = -1;
  QList<QListWidgetItem *> aListSec = mySecList->selectedItems();

  if (aListSec.size() == 1) {
    anIndex = mySecList->row(aListSec.first());
  }

  return anIndex;
}

//=======================================================================
// function: updateUndoRedo
// purpose:
//=======================================================================
void CurveCreator_Widget::updateUndoRedo()
{
  // Update undo/redo buttons.
  updateUndoRedoButtons();

  // Update sections.
  CurveCreator_Curve *aCurve = myEditor.getCurve();
  const bool isKeepSelection = (mySecList->count() == aCurve->getNbSections());
  QList<int> aSelectedRows;
  QList<QListWidgetItem *> aListItems = mySecList->selectedItems();

  if (isKeepSelection) {
    foreach (QListWidgetItem *anItem, aListItems) {
      aSelectedRows.append(mySecList->row(anItem));
    }
  }

  initSections();

  // Set selection.
  if (isKeepSelection) {
    foreach (int aRow, aSelectedRows) {
      mySecList->item(aRow)->setSelected(true);
    }
  }

  changeSecSelection();
}

//=======================================================================
// function: updateUndoRedoButtons
// purpose:
//=======================================================================
void CurveCreator_Widget::updateUndoRedoButtons()
{
  // Update undo/redo buttons.
  const int anUndoDepth = myEditor.getUndoDepth();

  if (anUndoDepth == UNDO_REDO_DISABLED) {
    myUndoBtn->setEnabled(false);
    myRedoBtn->setEnabled(false);
  } else {
    myUndoBtn->setEnabled(myEditor.getNbUndo() > 0);
    myRedoBtn->setEnabled(myEditor.getNbRedo() > 0);
  }
}


//=======================================================================
// function: setTabOrder
// purpose:
//=======================================================================
void CurveCreator_Widget::setTabOrder()
{
  QWidget::setTabOrder(myUndoBtn, myRedoBtn);
  QWidget::setTabOrder(myRedoBtn, myUndoOptsBtn);
  QWidget::setTabOrder(myUndoOptsBtn, mySecBtnGrp->button(SECTION_ADD));
  QWidget::setTabOrder(mySecBtnGrp->button(SECTION_ADD),
                       mySecBtnGrp->button(SECTION_MODIFY));
  QWidget::setTabOrder(mySecBtnGrp->button(SECTION_MODIFY), mySecCloseChkBox);
  QWidget::setTabOrder(mySecCloseChkBox, mySecTypeCmbBox);
  QWidget::setTabOrder(mySecTypeCmbBox, myEditSecPnts->getPointsEdit());
  QWidget::setTabOrder(myEditSecPnts->getPointsEdit(),
                       myEditSecPnts->getPointsButton());
  QWidget::setTabOrder(myEditSecPnts->getPointsButton(), mySecAddModifBtn);
  QWidget::setTabOrder(mySecAddModifBtn, mySecRmBtn);
  QWidget::setTabOrder(mySecRmBtn, mySecJoinBtn);
  QWidget::setTabOrder(mySecJoinBtn, mySecJoinAllBtn);
  QWidget::setTabOrder(mySecJoinAllBtn, mySecClearBtn);
  QWidget::setTabOrder(mySecClearBtn, mySecList);
  QWidget::setTabOrder(mySecList, mySecUpBtn);
  QWidget::setTabOrder(mySecUpBtn, mySecDownBtn);
  QWidget::setTabOrder(mySecDownBtn, myEditPnts->getPointsEdit());
  QWidget::setTabOrder(myEditPnts->getPointsEdit(),
                       myEditPnts->getPointsButton());
  QWidget::setTabOrder(myEditPnts->getPointsButton(), myAddPntsBtn);
  QWidget::setTabOrder(myAddPntsBtn, myInsertPntsBtn);
  QWidget::setTabOrder(myInsertPntsBtn, myRmPntsBtn);
  QWidget::setTabOrder(myRmPntsBtn, myPntsList);
}

//=======================================================================
// function: undoOptionsChanged
// purpose:
//=======================================================================
void CurveCreator_Widget::undoOptionsChanged()
{
  if (myUndoOptsDlg == NULL) {
    // Create the dialog.
    myUndoOptsDlg = new CurveCreator_UndoOptsDlg(this);
  }

  // set undo depth.
  myUndoOptsDlg->setUndoDepth(myEditor.getUndoDepth());

  const int aResult = myUndoOptsDlg->exec();

  if (aResult == QDialog::Accepted) {
    // Get undo depth.
    const int  anUndoDepth = myUndoOptsDlg->getUndoDepth();
    const bool isEnabled   = myUndoOptsDlg->isEnabled();

    myEditor.setUndoDepth(anUndoDepth);

    // Update options labels.
    myBufSizeUndoLbl->setVisible(isEnabled);

    if (isEnabled) {
      QString aSize;

      if (myUndoOptsDlg->isUnlimited()) {
        aSize = tr("CC_UNDO_REDO_UNLIMITED");
      } else {
        aSize = QString::number(anUndoDepth);
      }

      myEnabledUndoLbl->setText(tr("CC_UNDO_REDO_ENABLED"));
      myBufSizeUndoLbl->setText(tr("CC_UNDO_REDO_BUFFER_SIZE").arg(aSize));
    } else {
      myEnabledUndoLbl->setText(tr("CC_UNDO_REDO_DISABLED"));
    }
  }

  // Update Undo Redo buttons.
  updateUndoRedoButtons();
}

//=======================================================================
// function: sectionAddOrModify
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionAddOrModify()
{
  const bool         isClosed = mySecCloseChkBox->isChecked();
  CurveCreator::Type aType;

  if (mySecTypeCmbBox->currentIndex() == POLYLINE_INDEX) {
    aType = CurveCreator::Polyline;
  } else {
    aType = CurveCreator::BSpline;
  }

  if (mySecBtnGrp->checkedId() == SECTION_MODIFY) {
    // Modify section mode.
    QList<QListWidgetItem *> aListItems = mySecList->selectedItems();

    if (aListItems.size() == 1) {
      const int           aRow       = mySecList->row(aListItems.first());
      bool                isModified = false;
      CurveCreator_Curve *aCurve     = myEditor.getCurve();


      if (aCurve->getType(aRow) != aType) {
        // Set type.
        isModified = true;
        myEditor.setType(aType, aRow);
      }

      if (aCurve->isClosed(aRow) != isClosed) {
        // Set closed flag.
        isModified = true;
        myEditor.setClosed(isClosed, aRow);
      }

      if (isModified) {
        updateSectionItem(aRow, aType, isClosed);

        // Update undo/redo buttons.
        updateUndoRedoButtons();
      }
    }
  } else {
    // Add section mode. Add section.
    myEditor.addSection(aType, isClosed, myEditSecPnts->getPoints());
    addSectionItem(aType, isClosed);

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: secButtonClicked
// purpose:
//=======================================================================
void CurveCreator_Widget::addModifChanged(int theId)
{
  if (theId == SECTION_ADD) {
    // Add section
    myAddSecGrp->setTitle(tr("CC_SECTION_ADD_TITLE"));
    mySecAddModifBtn->setText(tr("CC_SECTION_NEW"));
    myEditSecPnts->setEnabled(true);
    mySecAddModifBtn->setEnabled(true);
  } else {
    // Modify section
    myAddSecGrp->setTitle(tr("CC_SECTION_MODIFICATION_TITLE"));
    mySecAddModifBtn->setText(tr("CC_SECTION_MODIFY"));
    myEditSecPnts->setEnabled(false);
    changeSecSelection();
  }
}

//=======================================================================
// function: changeSecSelection
// purpose:
//=======================================================================
void CurveCreator_Widget::changeSecSelection()
{
  // Update modify button and section data.
  QList<QListWidgetItem *> aListItems = mySecList->selectedItems();
  const int aNbItems = aListItems.size();

  if (mySecBtnGrp->checkedId() == SECTION_MODIFY) {
    // Modify section mode.
    if (aNbItems == 1) {
      // Initialize type and closed flags.
      CurveCreator_Curve *aCurve  = myEditor.getCurve();
      const int           anIndex = mySecList->row(aListItems.first());

      mySecAddModifBtn->setEnabled(true);
      mySecCloseChkBox->setChecked(aCurve->isClosed(anIndex));

      if (aCurve->getType(anIndex) == CurveCreator::Polyline) {
        mySecTypeCmbBox->setCurrentIndex(POLYLINE_INDEX);
      } else {
        mySecTypeCmbBox->setCurrentIndex(BSPLINE_INDEX);
      }
    } else if (mySecAddModifBtn->isEnabled()) {
      mySecAddModifBtn->setEnabled(false);
    }
  } else {
    // Add section mode.
    mySecAddModifBtn->setEnabled(true);
  }

  // Set enabled remove, up and down points.
  bool isEnabled = (aNbItems > 0);
  const int aCount = mySecList->count();

  mySecRmBtn->setEnabled(isEnabled);
  isEnabled &= (aNbItems < aCount);
  mySecUpBtn->setEnabled(isEnabled);
  mySecDownBtn->setEnabled(isEnabled);
  mySecJoinBtn->setEnabled(aNbItems == 2);

  // Init points.
  myPntsGrp->setEnabled(aNbItems == 1);

  if (aNbItems == 1) {
    const int aSecIndex = mySecList->row(aListItems.first());

    initPoints(aSecIndex);
  } else {
    initPoints(-1);
  }
}

//=======================================================================
// function: sectionRemove
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionRemove()
{
  QList<QListWidgetItem *> aListItems = mySecList->selectedItems();

  if (!aListItems.empty()) {
    QList<int> aListRows;

    // Get items rows.
    foreach(QListWidgetItem *anItem, aListItems) {
      aListRows.append(mySecList->row(anItem));
    }

    // Delete items.
    foreach(QListWidgetItem *anItem, aListItems) {
      delete anItem;
    }

    qSort(aListRows);

    // Delete points from curve.
    const int aNbPnts = aListRows.size();
    int       i;

    for (i = aNbPnts - 1; i >= 0; i--) {
      myEditor.removeSection(aListRows[i]);
    }

    // Set the new selection.
    int aRow = aListRows[0];

    if (aListRows[0] >= mySecList->count()) {
      aRow = mySecList->count() - 1;
    }

    if (aRow >= 0) {
      mySecList->item(aRow)->setSelected(true);
    }

    emit numberOfItemsChanged(mySecList);

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: sectionJoin
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionJoin()
{
  QList<QListWidgetItem *> aListItems = mySecList->selectedItems();

  if (aListItems.size() == 2) {
    int aRowTo   = mySecList->row(aListItems.first());
    int aRowFrom = mySecList->row(aListItems.last());

    if (aRowTo > aRowFrom) {
      // Swap rows.
      int aTmp = aRowTo;

      aRowTo   = aRowFrom;
      aRowFrom = aTmp;
    }

    // Join two sections.
    myEditor.join(aRowTo, aRowFrom);
    delete mySecList->takeItem(aRowFrom);
    emit numberOfItemsChanged(mySecList);
    // Select the joined item.
    mySecList->item(aRowTo)->setSelected(true);
    changeSecSelection();

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: sectionJoinAll
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionJoinAll()
{
  const int aNbItems = mySecList->count();

  if (aNbItems > 1) {
    // Join two sections.
    myEditor.join();

    // Update mySecList.
    for (int i = 1; i < aNbItems; i++) {
      delete mySecList->takeItem(1);
    }

    emit numberOfItemsChanged(mySecList);
    // Select the joined item.
    mySecList->item(0)->setSelected(true);
    changeSecSelection();

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: sectionClear
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionClear()
{
  bool isEmpty = (mySecList->count() == 0);

  if (!isEmpty) {
    mySecList->clear();
    myEditor.clear();
    emit numberOfItemsChanged(mySecList);

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: sectionUp
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionUp()
{
  sectionMove(-1);
}

//=======================================================================
// function: sectionDown
// purpose:
//=======================================================================
void CurveCreator_Widget::sectionDown()
{
  sectionMove(1);
}

//=======================================================================
// function: onNumberOfItemsChanged
// purpose:
//=======================================================================
void CurveCreator_Widget::onNumberOfItemsChanged(QListWidget *theListWidget)
{
  if (theListWidget == mySecList) {
    // List of sections is modified.
    mySecJoinAllBtn->setEnabled(mySecList->count() > 1);
    mySecClearBtn->setEnabled(mySecList->count() > 0);
  } else {
    // List of points is modified.
    QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();
    const int aNbItems = aListItems.size();

    myRmPntsBtn->setEnabled(aNbItems > 0);
  }
}

//=======================================================================
// function: changePntsSelection
// purpose:
//=======================================================================
void CurveCreator_Widget::changePntsSelection()
{
  // Update modify buttons and section data.
  QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();
  const int aNbItems = aListItems.size();
  const int aCount   = myPntsList->count();

  myRmPntsBtn->setEnabled(aNbItems > 0);
  myInsertPntsBtn->setEnabled(aCount == 0 || aNbItems == 1);
}

//=======================================================================
// function: editPnt
// purpose:
//=======================================================================
void CurveCreator_Widget::editPnt(QListWidgetItem *theItem)
{
  if (theItem != NULL) {
    if (myPntEditDlg == NULL) {
      // Create the dialog.
      myPntEditDlg = new CurveCreator_EditPntDlg(this, myDimension);
    }

    // Get coordinates.
    CurveCreator_PointItem    *aPntItem = (CurveCreator_PointItem *)theItem;
    CurveCreator::Coordinates  aPoint;

    aPoint.push_back(aPntItem->getX());
    aPoint.push_back(aPntItem->getY());

    if (myDimension == CurveCreator::Dim3d) {
      aPoint.push_back(aPntItem->getZ());
    }

    myPntEditDlg->setPoint(aPoint);

    const int aResult = myPntEditDlg->exec();

    if (aResult == QDialog::Accepted) {
      // Get the section index.
      const int aSectionIndex = getCurrentSectionIndex();

      if (aSectionIndex >= 0) {
        // Update the point.
        const CurveCreator::Coordinates &aNewPoint = myPntEditDlg->getPoint();

        if (myDimension == CurveCreator::Dim3d) {
          aPntItem->setCoord(aNewPoint[0], aNewPoint[1], aNewPoint[2]);
        } else {
          aPntItem->setCoord(aNewPoint[0], aNewPoint[1]);
        }

        // Update curve
        const int aPntIndex = myPntsList->row(theItem);

        myEditor.setCoordinates(aNewPoint, aSectionIndex, aPntIndex);

        // Update undo/redo buttons.
        updateUndoRedoButtons();
      }
    }
  }
}

//=======================================================================
// function: pntsAdd
// purpose:
//=======================================================================
void CurveCreator_Widget::pntsAdd()
{
  const CurveCreator::Coordinates &aNewPoints = myEditPnts->getPoints();
  const int                        aNbCoords  = aNewPoints.size();

  if (aNbCoords > 0 && aNbCoords % myDimension == 0) {
    // Get the section index.
    const int aSectionIndex = getCurrentSectionIndex();

    if (aSectionIndex >= 0) {
      // Update list.
      int i = 0;

      while (i < aNbCoords) {
        const CurveCreator::TypeCoord aX = aNewPoints[i++];
        const CurveCreator::TypeCoord aY = aNewPoints[i++];

        if (myDimension == CurveCreator::Dim3d) {
          const CurveCreator::TypeCoord aZ = aNewPoints[i++];

          new CurveCreator_PointItem(aX, aY, aZ, myPntsList);
        } else {
          new CurveCreator_PointItem(aX, aY, myPntsList);
        }
      }

      // Update curve
      myEditor.addPoints(aNewPoints, aSectionIndex);
      emit numberOfItemsChanged(myPntsList);

      // Update undo/redo buttons.
      updateUndoRedoButtons();
    }
  }
}

//=======================================================================
// function: pntsInsert
// purpose:
//=======================================================================
void CurveCreator_Widget::pntsInsert()
{
  const CurveCreator::Coordinates &aNewPoints = myEditPnts->getPoints();
  const int                        aNbCoords  = aNewPoints.size();

  if (aNbCoords > 0 && aNbCoords % myDimension == 0) {
    // Get the section index.
    const int aSectionIndex = getCurrentSectionIndex();

    if (aSectionIndex >= 0) {
      // Get the selected point index.
      int aPntIndex = -1;
      QList<QListWidgetItem *> aListPnts = myPntsList->selectedItems();

      if (aListPnts.size() == 1) {
        aPntIndex = myPntsList->row(aListPnts.first());
      }

      // Update list.
      int i = 0;
      int aRow = aPntIndex;

      while (i < aNbCoords) {
        const CurveCreator::TypeCoord  aX       = aNewPoints[i++];
        const CurveCreator::TypeCoord  aY       = aNewPoints[i++];
        CurveCreator_PointItem        *aNewItem = NULL;

        if (myDimension == CurveCreator::Dim3d) {
          const CurveCreator::TypeCoord aZ = aNewPoints[i++];

          aNewItem = new CurveCreator_PointItem(aX, aY, aZ);
        } else {
          aNewItem = new CurveCreator_PointItem(aX, aY);
        }


        if (aPntIndex < 0) {
          // Append point.
          myPntsList->addItem(aNewItem);
        } else {
          // Insert point.
          myPntsList->insertItem(aRow++, aNewItem);
        }
      }

      // Update curve
      myEditor.insertPoints(aNewPoints, aSectionIndex, aPntIndex);
      emit numberOfItemsChanged(myPntsList);

      // Update undo/redo buttons.
      updateUndoRedoButtons();
    }
  }
}

//=======================================================================
// function: pntsRemove
// purpose:
//=======================================================================
void CurveCreator_Widget::pntsRemove()
{
  QList<QListWidgetItem *> aListItems    = myPntsList->selectedItems();
  const int                aSectionIndex = getCurrentSectionIndex();

  if (aSectionIndex >= 0 && aListItems.size() > 0) {
    QList<int> aListRows;

    // Get items rows.
    foreach(QListWidgetItem *anItem, aListItems) {
      aListRows.append(myPntsList->row(anItem));
    }

    // Delete items.
    foreach(QListWidgetItem *anItem, aListItems) {
      delete anItem;
    }

    qSort(aListRows);

    // Delete points from curve.
    const int aNbPnts   = aListRows.size();
    int       aNbPntsRm = 1;
    int       aCurRow   = aListRows.last();
    int       i;

    for (i = aNbPnts - 2; i >= 0; i--) {
      if (aListRows[i] == aCurRow - 1) {
        // This is the previous index.
        aCurRow--;
        aNbPntsRm++;
      } else {
        // This is not the previous index. Remove the current chain of points.
        myEditor.removePoints(aSectionIndex, aCurRow, aNbPntsRm);
        aCurRow   = aListRows[i];
        aNbPntsRm = 1;
      }
    }

    // Remove last points.
    myEditor.removePoints(aSectionIndex, aCurRow, aNbPntsRm);

    emit numberOfItemsChanged(myPntsList);

    // Set the new selection.
    int aRow = aListRows.first();

    if (aRow >= myPntsList->count()) {
      aRow = myPntsList->count() - 1;
    }

    if (aRow >= 0) {
      myPntsList->item(aRow)->setSelected(true);
    }

    // Update undo/redo buttons.
    updateUndoRedoButtons();
  }
}

//=======================================================================
// function: undo
// purpose:
//=======================================================================
void CurveCreator_Widget::undo()
{
  if (myEditor.getNbUndo() > 0) {
    // Perform undo
    myEditor.undo();

    // Reinitialize required widgets.
    updateUndoRedo();
  }
}

//=======================================================================
// function: redo
// purpose:
//=======================================================================
void CurveCreator_Widget::redo()
{
  if (myEditor.getNbRedo() > 0) {
    // Perform undo
    myEditor.redo();

    // Reinitialize required widgets.
    updateUndoRedo();
  }
}
