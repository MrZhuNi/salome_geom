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
//  File   : Kinematic_Position.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "Kinematic_Position.hxx"


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Position::Kinematic_Position(){}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Position::Kinematic_Position(gp_Pnt Origin, gp_Dir DirX, gp_Dir DirY, gp_Dir DirZ)
{
  myOrigin = Origin;
  myDirX = DirX;
  myDirY = DirY;
  myDirZ = DirZ;
}


//=================================================================================
// function : ~Kinematic_Position()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Position::~Kinematic_Position() {}


//=================================================================================
// function : AxeX()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::AxeX()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirX);
  return anAxe;
}


//=================================================================================
// function : AxeY()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::AxeY()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirY);
  return anAxe;
}


//=================================================================================
// function : AxeZ()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::AxeZ()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirZ);
  return anAxe;
}


//=================================================================================
// function : Axe3()
// purpose  : 
//=================================================================================
gp_Ax3 Kinematic_Position::Axe3()
{
  gp_Ax3 anAxe3(myOrigin, myDirZ, myDirX);
  return anAxe3;
}
