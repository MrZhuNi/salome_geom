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

#ifndef ENTITYGUI_POLYLINEDLG_H
#define ENTITYGUI_POLYLINEDLG_H


#include <GEOMBase_Skeleton.h>


class CurveCreator_Curve;
class CurveCreator_Widget;
class QGroupBox;


//=================================================================================
// class    : EntityGUI_PolylineDlg
// purpose  :
//=================================================================================
class EntityGUI_PolylineDlg : public GEOMBase_Skeleton
{
  Q_OBJECT

public:

  EntityGUI_PolylineDlg (GeometryGUI*, QWidget* = 0,
                         bool = false, Qt::WindowFlags = 0);

  virtual ~EntityGUI_PolylineDlg();

  void  deleteSelected();
  bool  deleteEnabled();

protected:

  // redefined from GEOMBase_Helper
  virtual GEOM::GEOM_IOperations_ptr createOperation();
  virtual bool                       isValid( QString& );
  virtual bool                       execute( ObjectList& );
	
private:

  void                               Init();

protected slots:

  void processStartedSubOperation( QWidget*, bool );
  void processFinishedSubOperation( QWidget* );

private:

  CurveCreator_Curve  *myCurve;
  CurveCreator_Widget *myEditorWidget;
  QGroupBox           *myAddElementBox;

};

#endif // ENTITYGUI_POLYLINEDLG_H
