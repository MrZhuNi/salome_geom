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

// File:        CurveCreator_EditPntsWidget.h
// Created:     Fri Jul  05 16:30:17 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_EditPntsWidget_HeaderFile
#define _CurveCreator_EditPntsWidget_HeaderFile


#include <QWidget>
#include <CurveCreator.hxx>

class QLineEdit;
class QPushButton;
class QListWidget;
class CurveCreator_EditPntsDlg;


class CurveCreator_EditPntsWidget : public QWidget
{
  Q_OBJECT

public:

  CurveCreator_EditPntsWidget(QWidget* parent,
                              const bool IsSection,
                              const CurveCreator::Dimension theDimension);

  ~CurveCreator_EditPntsWidget();

  void setPoints(const CurveCreator::Coordinates &thePoints);

  const CurveCreator::Coordinates &getPoints() const;

  void setPointsList(QListWidget *thePntsList);

  void clear();

  QLineEdit *getPointsEdit() const;

  QPushButton *getPointsButton() const;

private slots:

  void editPoints();

private:

  void init();

  void updateEditLine();

  void setPointsToDialog();

protected:

  CurveCreator::Coordinates  myPoints;
  CurveCreator::Dimension    myDimension;
  QPushButton               *myPntsBtn;
  QLineEdit                 *myPntsEdit;
  CurveCreator_EditPntsDlg  *myPntsEditDlg;
  QListWidget               *myPntsList;

};

#endif

