//  GEOM SKETCHER : basic sketcher
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SketcherGUI_Profile.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Shape.hxx>

#include <qstringlist.h>

class SketcherGUI_Profile
{

public:
  SketcherGUI_Profile();
  SketcherGUI_Profile(QStringList aCommand);

private:
  gp_Pnt myLastPoint;
  gp_Dir myLastDir;

  TopoDS_Shape myShape;
  bool myOK;

public:
  gp_Pnt GetLastPoint(){return myLastPoint;};
  gp_Dir GetLastDir(){return myLastDir;};

  const TopoDS_Shape& GetShape(){return myShape;};
  bool IsDone(){return myOK;};

};
