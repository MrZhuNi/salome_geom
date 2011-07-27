//  Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// GEOM GEOMGUI : GUI for Geometry component
// File   : GenerationGUI_PipeDlg.h
// Author : Lucien PIGNOLONI, Open CASCADE S.A.S.
//
#ifndef GENERATIONGUI_PIPEDLG_H
#define GENERATIONGUI_PIPEDLG_H

#include <GEOMBase_Skeleton.h>

class DlgRef_3Sel1Check;

//=================================================================================
// class    : GenerationGUI_PipeDlg
// purpose  :
//=================================================================================
class GenerationGUI_PipeDlg : public GEOMBase_Skeleton
{ 
  Q_OBJECT

public:
  GenerationGUI_PipeDlg( GeometryGUI*, QWidget* = 0, bool = false, Qt::WindowFlags = 0 );
  ~GenerationGUI_PipeDlg();
  
protected:
  // redefined from GEOMBase_Helper
  virtual GEOM::GEOM_IOperations_ptr createOperation();
  virtual bool                       isValid( QString& );
  virtual bool                       execute( ObjectList& );    
  virtual void                       addSubshapesToStudy();
  virtual bool                       extractPrefix() const;

private:
  void                               Init();
  void                               enterEvent( QEvent* );
  
private:
  GEOM::ListOfGO                     myBaseObjects;  /* Base shapes */
  GEOM::GEOM_Object_var              myPath; /* Shape, defining the path */
  GEOM::GEOM_Object_var              myVec;  /* Vector, defining the constant binormal direction */
  bool                               myOkBase;
  bool                               myOkPath;
  bool                               myOkVec; /* to check when arguments are defined */
  
  DlgRef_3Sel1Check*                       GroupPoints;

private slots:
  void                               ClickOnOk();
  bool                               ClickOnApply();
  void                               ActivateThisDialog();
  void                               LineEditReturnPressed();
  void                               SelectionIntoArgument();
  void                               SetEditCurrentArgument();
  void                               ConstructorsClicked( int );
  void                               SelectionTypeButtonClicked();
};

#endif // GENERATIONGUI_PIPEDLG_H
