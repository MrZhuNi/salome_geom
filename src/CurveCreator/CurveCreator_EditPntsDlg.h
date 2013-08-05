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

// File:        CurveCreator_EditPntsDlg.h
// Created:     Fri Jul  05 16:29:48 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_EditPntsDlg_HeaderFile
#define _CurveCreator_EditPntsDlg_HeaderFile


#include <QDialog>
#include <CurveCreator.hxx>

class QListWidget;
class QDoubleSpinBox;
class QPushButton;


class CurveCreator_EditPntsDlg : public QDialog
{
  Q_OBJECT

public:

  CurveCreator_EditPntsDlg(QWidget* parent,
                           const CurveCreator::Dimension theDimension);

  ~CurveCreator_EditPntsDlg();

  void setPoints(const CurveCreator::Coordinates &thePoints);

  const CurveCreator::Coordinates &getPoints() const;

private:

  void init();

  void initSpinBox(QDoubleSpinBox *theSpinBox);

  void updateEditList();

  void movePoints(const int theShift);

  void setTabOrder();

private slots:

  void appendPoint();

  void modifyPoint();

  void removePoint();

  void upPoint();

  void downPoint();

  void changeSelection();

  void accept();

  void clear();

  void onNumberOfItemsChanged(int theNewValue);

signals:

  void numberOfItemsChanged(int theNewValue);

protected:

  CurveCreator::Dimension    myDimension;
  CurveCreator::Coordinates  myPoints;
  QListWidget               *myPntsList;
  QDoubleSpinBox            *myXSpn;
  QDoubleSpinBox            *myYSpn;
  QDoubleSpinBox            *myZSpn;
  QPushButton               *myAddBtn;
  QPushButton               *myModifBtn;
  QPushButton               *myRmBtn;
  QPushButton               *myClearBtn;
  QPushButton               *myPntUpBtn;
  QPushButton               *myPntDownBtn;
  QPushButton               *myOkBtn;
  QPushButton               *myCancelBtn;

};

#endif
