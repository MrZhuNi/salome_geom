// GEOM GEOMGUI : GUI for Geometry component
//
// Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// File   : GEOMToolsGUI_TransparencyDlg.h
// Author : Lucien PIGNOLONI
//

#ifndef GEOMTOOLSGUI_TRANSPARENCYDLG_H
#define GEOMTOOLSGUI_TRANSPARENCYDLG_H

#include "GEOM_ToolsGUI.hxx"

#include <QDialog>

class QSlider;

//=================================================================================
// class    : GEOMBase_TransparencyDlg
// purpose  :
//          : WARNING : that is a MODAL dialog.
//=================================================================================
class GEOMTOOLSGUI_EXPORT GEOMToolsGUI_TransparencyDlg : public QDialog
{ 
    Q_OBJECT

public:
  GEOMToolsGUI_TransparencyDlg( QWidget* parent );    
  ~GEOMToolsGUI_TransparencyDlg();

private:
  void      keyPressEvent(QKeyEvent*);

private :
  bool      myFirstInit ;   /* Inform for the first init  */
  QSlider*  mySlider; 
  QString   myHelpFileName;

private slots: 
  void      ClickOnOk();
  void      ClickOnClose();
  void      ClickOnHelp();
  void      ValueHasChanged( int ) ;
    
};

#endif // GEOMTOOLSGUI_TRANSPARENCYDLG_H

