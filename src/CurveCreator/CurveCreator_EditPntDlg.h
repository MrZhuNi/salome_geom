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

// File:        CurveCreator_EditPntDlg.h
// Created:     Tue Jul  16 10:58:22 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_EditPntDlg_HeaderFile
#define _CurveCreator_EditPntDlg_HeaderFile


#include <QDialog>
#include <CurveCreator.hxx>

class QListWidget;
class QDoubleSpinBox;
class QPushButton;


class CurveCreator_EditPntDlg : public QDialog
{
  Q_OBJECT

public:

  CurveCreator_EditPntDlg(QWidget* parent,
                          const CurveCreator::Dimension theDimension);

  ~CurveCreator_EditPntDlg();

  void setPoint(const CurveCreator::Coordinates &thePoint);

  const CurveCreator::Coordinates &getPoint() const;

private:

  void init();

  void initSpinBox(QDoubleSpinBox *theSpinBox);

  void setTabOrder();

private slots:

  void accept();

protected:

  CurveCreator::Dimension    myDimension;
  CurveCreator::Coordinates  myPoint;
  QDoubleSpinBox            *myXSpn;
  QDoubleSpinBox            *myYSpn;
  QDoubleSpinBox            *myZSpn;
  QPushButton               *myOkBtn;
  QPushButton               *myCancelBtn;

};

#endif
