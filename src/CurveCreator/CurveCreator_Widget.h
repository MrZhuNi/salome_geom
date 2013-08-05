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

// File:        CurveCreator_Widget.h
// Created:     Mon Jul  01 12:49:21 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_Widget_HeaderFile
#define _CurveCreator_Widget_HeaderFile


#include <QWidget>
#include <CurveCreator_CurveEditor.hxx>

class CurveCreator_Curve;
class QGroupBox;
class QButtonGroup;
class QComboBox;
class QCheckBox;
class QPushButton;
class QLabel;
class QListWidget;
class QListWidgetItem;
class CurveCreator_EditPntsWidget;
class CurveCreator_EditPntDlg;
class CurveCreator_UndoOptsDlg;


class CurveCreator_Widget : public QWidget
{
  Q_OBJECT

public:

  CurveCreator_Widget(QWidget* parent,
                      CurveCreator_Curve *theCurve,
                      Qt::WindowFlags fl = 0);

private:

  void init();

  void initSections();

  void addSectionItem(const CurveCreator::Type theType, const bool isClosed);

  void updateSectionItem(const int theRow, const CurveCreator::Type theType,
                         const bool isClosed);

  void sectionMove(const int theShift);

  void initPoints(const int theSectionIndex);

  int getCurrentSectionIndex();

  void updateUndoRedo();

  void updateUndoRedoButtons();

  void setTabOrder();

private slots:

  void undoOptionsChanged();

  void sectionAddOrModify();

  void addModifChanged(int theId);

  void changeSecSelection();

  void sectionRemove();

  void sectionJoin();

  void sectionJoinAll();

  void sectionClear();

  void sectionUp();

  void sectionDown();

  void onNumberOfItemsChanged(QListWidget *theListWidget);

  void changePntsSelection();

  void editPnt(QListWidgetItem *theItem);

  void pntsAdd();

  void pntsInsert();

  void pntsRemove();

  void undo();

  void redo();

signals:

  void numberOfItemsChanged(QListWidget *theListWidget);

protected:

  CurveCreator_CurveEditor     myEditor;
  CurveCreator::Dimension      myDimension;
  // Undo/redo widgets
  QLabel                      *myEnabledUndoLbl;
  QLabel                      *myBufSizeUndoLbl;
  QPushButton                 *myUndoBtn;
  QPushButton                 *myRedoBtn;
  QPushButton                 *myUndoOptsBtn;
  CurveCreator_UndoOptsDlg    *myUndoOptsDlg;
  // Sections widgets
  QGroupBox                   *myAddSecGrp;
  QButtonGroup                *mySecBtnGrp;
  QComboBox                   *mySecTypeCmbBox;
  QCheckBox                   *mySecCloseChkBox;
  QPushButton                 *mySecAddModifBtn;
  QPushButton                 *mySecRmBtn;
  QPushButton                 *mySecJoinBtn;
  QPushButton                 *mySecJoinAllBtn;
  QPushButton                 *mySecClearBtn;
  QPushButton                 *mySecUpBtn;
  QPushButton                 *mySecDownBtn;
  QListWidget                 *mySecList;
  CurveCreator_EditPntsWidget *myEditSecPnts;
  // Points widgets
  QGroupBox                   *myPntsGrp;
  QListWidget                 *myPntsList;
  CurveCreator_EditPntDlg     *myPntEditDlg;
  CurveCreator_EditPntsWidget *myEditPnts;
  QPushButton                 *myAddPntsBtn;
  QPushButton                 *myInsertPntsBtn;
  QPushButton                 *myRmPntsBtn;

};

#endif
