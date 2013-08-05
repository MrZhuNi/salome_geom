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

// File:        CurveCreator_EditPntsWidget.cxx
// Created:     Fri Jul  05 16:30:11 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_EditPntsWidget.h>
#include <CurveCreator_EditPntsDlg.h>
#include <CurveCreator_PointItem.h>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>


//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_EditPntsWidget::CurveCreator_EditPntsWidget
  (QWidget* parent,
   const bool IsSection,
   const CurveCreator::Dimension theDimension)
  : QWidget       (parent),
    myDimension   (theDimension),
    myPntsEdit    (NULL),
    myPntsBtn     (NULL),
    myPntsEditDlg (NULL),
    myPntsList    (NULL)
{
  QGroupBox   *aMainGrp = new QGroupBox;
  QHBoxLayout *aMainLO  = new QHBoxLayout(aMainGrp);

  myPntsEdit = new QLineEdit(aMainGrp);
  myPntsBtn  = new QPushButton
    (IsSection? tr("CC_SECTION_POINTS_EDIT") : tr("CC_POINTS_EDIT"), aMainGrp);
  aMainLO->addWidget(myPntsEdit);
  aMainLO->addWidget(myPntsBtn);

  setLayout(aMainLO);

  init();
}

//=======================================================================
// function: Destructor
// purpose:
//=======================================================================
CurveCreator_EditPntsWidget::~CurveCreator_EditPntsWidget()
{
}

//=======================================================================
// function: setPoints
// purpose:
//=======================================================================
void CurveCreator_EditPntsWidget::setPoints
                    (const CurveCreator::Coordinates &thePoints)
{
  myPoints = thePoints;
  updateEditLine();
}

//=======================================================================
// function: getPoints
// purpose:
//=======================================================================
const CurveCreator::Coordinates &CurveCreator_EditPntsWidget::getPoints() const
{
  return myPoints;
}

//=======================================================================
// function : setPointsList
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::setPointsList(QListWidget *thePntsList)
{
  myPntsList = thePntsList;
}

//=======================================================================
// function : clear
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::clear()
{
  myPoints.clear();
  myPntsEdit->setText("");
}

//=======================================================================
// function : getPointsEdit
// purpose  :
//=======================================================================
QLineEdit *CurveCreator_EditPntsWidget::getPointsEdit() const
{
  return myPntsEdit;
}

//=======================================================================
// function : getPointsButton
// purpose  :
//=======================================================================
QPushButton *CurveCreator_EditPntsWidget::getPointsButton() const
{
  return myPntsBtn;
}

//=======================================================================
// function : init
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::init()
{
  connect(myPntsBtn, SIGNAL(clicked()), this, SLOT(editPoints()));

  myPntsEdit->setReadOnly(true);
  updateEditLine();

  // Set tab order.
  QWidget::setTabOrder(myPntsEdit, myPntsBtn);
}

//=======================================================================
// function : editPoints
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::editPoints()
{
  if (myPntsEditDlg == NULL) {
    // Create the dialog.
    myPntsEditDlg = new CurveCreator_EditPntsDlg(this, myDimension);
  }

  // Set points to dialog.
  setPointsToDialog();

  const int aResult = myPntsEditDlg->exec();

  if (aResult == QDialog::Accepted) {
    // Update the list of points and myPntsEdit.
    const CurveCreator::Coordinates &aNewPoints = myPntsEditDlg->getPoints();

    myPoints.clear();
    myPoints.insert(myPoints.end(), aNewPoints.begin(), aNewPoints.end());
    updateEditLine();
  }
}

//=======================================================================
// function : updateEditLine
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::updateEditLine()
{
  const int aNbPnts = myPoints.size();

  if ( aNbPnts == 0 ) {
    myPntsEdit->setText("");
  } else if ( aNbPnts == myDimension ) {
    // One point.
    QString aText;

    if (myDimension == CurveCreator::Dim3d) {
      aText = CurveCreator_PointItem::getText
                  (myPoints[0], myPoints[1], myPoints[2]);
    } else {
      aText = CurveCreator_PointItem::getText(myPoints[0], myPoints[1]);
    }

    myPntsEdit->setText(aText);
  } else if ( aNbPnts > 0 ) {
    myPntsEdit->setText(tr("CC_POINTS_NUMBER").arg(aNbPnts/myDimension));
  }
}

//=======================================================================
// function : setPointsToDialog
// purpose  :
//=======================================================================
void CurveCreator_EditPntsWidget::setPointsToDialog()
{
  bool isPntsFromWidget = false;

  if (myPntsList != NULL) {
    QList<QListWidgetItem *> aListItems = myPntsList->selectedItems();

    if (aListItems.size() > 0) {
      CurveCreator::Coordinates aPoints;

      foreach (QListWidgetItem *anItem, aListItems) {
        const CurveCreator_PointItem *aPntItem =
          (const CurveCreator_PointItem *)anItem;

        aPoints.push_back(aPntItem->getX());
        aPoints.push_back(aPntItem->getY());

        if (myDimension == CurveCreator::Dim3d) {
          aPoints.push_back(aPntItem->getZ());
        }
      }

      myPntsEditDlg->setPoints(aPoints);
      isPntsFromWidget = true;
    }
  }

  if (!isPntsFromWidget) {
    myPntsEditDlg->setPoints(myPoints);
  }
}
