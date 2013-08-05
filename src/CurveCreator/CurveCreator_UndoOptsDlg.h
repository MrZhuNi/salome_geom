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

// File:        CurveCreator_UndoOptsDlg.h
// Created:     Wed Jul  17 12:50:42 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_UndoOptsDlg_HeaderFile
#define _CurveCreator_UndoOptsDlg_HeaderFile


#include <QDialog>

class QButtonGroup;
class QLineEdit;
class QPushButton;


class CurveCreator_UndoOptsDlg : public QDialog
{
  Q_OBJECT

public:

  CurveCreator_UndoOptsDlg(QWidget* parent);

  ~CurveCreator_UndoOptsDlg();

  void setUndoDepth(const int theDepth);

  int getUndoDepth() const;

  bool isEnabled() const;

  bool isUnlimited() const;

private:

  void init();

  void setTabOrder();

private slots:

  void optionChanged(int theId);

  void accept();

protected:

  int           myUndoDepth;
  QButtonGroup *myOptsBtnGrp;
  QLineEdit    *myBufferSizeEdit;
  QPushButton  *myOkBtn;
  QPushButton  *myCancelBtn;

};

#endif
