//  GEOM KINEMATIC
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
//  File   : Kinematic_Position.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_POSITION_HXX
#define _KINEMATIC_POSITION_HXX

#include <TopoDS_Shape.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax3.hxx>

class Kinematic_Position
{
public:
  Kinematic_Position();
  Kinematic_Position(gp_Pnt Origin, gp_Dir DirX, gp_Dir DirY, gp_Dir DirZ);
  ~Kinematic_Position();

private:
  gp_Pnt myOrigin;
  gp_Dir myDirX;
  gp_Dir myDirY;
  gp_Dir myDirZ;

public:
  void Origin(const gp_Pnt& Origin){myOrigin = Origin;};
  void DirX(const gp_Dir& DirX){myDirX = DirX;};
  void DirY(const gp_Dir& DirY){myDirY = DirY;};
  void DirZ(const gp_Dir& DirZ){myDirZ = DirZ;};

  gp_Pnt& Origin(){return myOrigin;};
  gp_Dir& DirX(){return myDirX;};
  gp_Dir& DirY(){return myDirY;};
  gp_Dir& DirZ(){return myDirZ;};

  gp_Ax1 AxeX();
  gp_Ax1 AxeY();
  gp_Ax1 AxeZ();

  gp_Ax3 Axe3();

};

#endif
