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

#include "Kinematic_Position.hxx"

using namespace std;

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Position::Kinematic_Position() {}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Position::Kinematic_Position(gp_Pnt Origin, gp_Vec DirX, gp_Vec DirY, gp_Vec DirZ)
{
  SetOrigin(Origin);
  SetDirX(DirX);
  SetDirY(DirY);
  SetDirZ(DirZ);

  return;
}


//=================================================================================
// function : ~Kinematic_Position()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Position::~Kinematic_Position() {}


//=================================================================================
// function : SetOrigin()
// purpose  : 
//=================================================================================
void Kinematic_Position::SetOrigin(const gp_Pnt& aPnt)
{
  myOrigin = aPnt;
  return;
}


//=================================================================================
// function : SetDirX()
// purpose  : 
//=================================================================================
void Kinematic_Position::SetDirX(const gp_Vec& aDir)
{
  myDirX = aDir;
  return;
}


//=================================================================================
// function : SetDirY()
// purpose  : 
//=================================================================================
void Kinematic_Position::SetDirY(const gp_Vec& aDir)
{
  myDirY = aDir;
  return;
}


//=================================================================================
// function : SetDirZ()
// purpose  : 
//=================================================================================
void Kinematic_Position::SetDirZ(const gp_Vec& aDir)
{
  myDirZ = aDir;
  return;
}


//=================================================================================
// function : GetAxeX()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::GetAxeX()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirX);
  return anAxe;
}


//=================================================================================
// function : GetAxeY()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::GetAxeY()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirY);
  return anAxe;
}


//=================================================================================
// function : GetAxeZ()
// purpose  : 
//=================================================================================
gp_Ax1 Kinematic_Position::GetAxeZ()
{
  gp_Ax1 anAxe;
  anAxe.SetLocation(myOrigin);
  anAxe.SetDirection(myDirZ);
  return anAxe;
}


//=================================================================================
// function : GetAxe3()
// purpose  : 
//=================================================================================
gp_Ax3 Kinematic_Position::GetAxe3()
{
  gp_Ax3 anAxe3(myOrigin, myDirZ, myDirX);
  return anAxe3;
}
