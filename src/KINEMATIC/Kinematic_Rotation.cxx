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
//  File   : Kinematic_Rotation.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "Kinematic_Rotation.hxx"


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Rotation::Kinematic_Rotation(){}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Rotation::Kinematic_Rotation(int Rot1, int Rot2, int Rot3, double ValX, double ValY, double ValZ)
{
  myRot1 = Rot1;
  myRot2 = Rot2;
  myRot3 = Rot3;

  myValX = ValX;
  myValY = ValY;
  myValZ = ValZ;
}


//=================================================================================
// function : ~Kinematic_Rotation()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Rotation::~Kinematic_Rotation() {}
