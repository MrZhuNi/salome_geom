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
#include <gp_Vec.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax3.hxx>

class Kinematic_Position
{
public:
  Kinematic_Position();
  Kinematic_Position(gp_Pnt Origin, gp_Vec DirX, gp_Vec DirY, gp_Vec DirZ);
  ~Kinematic_Position();

private:
  gp_Pnt myOrigin;
  gp_Vec myDirX;
  gp_Vec myDirY;
  gp_Vec myDirZ;

public:
  void SetOrigin(const gp_Pnt& aPnt);
  void SetDirX(const gp_Vec& aDir);
  void SetDirY(const gp_Vec& aDir);
  void SetDirZ(const gp_Vec& aDir);

  gp_Pnt& GetOrigin(){return myOrigin;};
  gp_Vec& GetDirX(){return myDirX;};
  gp_Vec& GetDirY(){return myDirY;};
  gp_Vec& GetDirZ(){return myDirZ;};

  gp_Ax1 GetAxeX();
  gp_Ax1 GetAxeY();
  gp_Ax1 GetAxeZ();

  gp_Ax3 GetAxe3();

};

#endif
