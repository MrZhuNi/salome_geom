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
//  File   : Kinematic_Range.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "Kinematic_Range.hxx"

using namespace std;

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Range::Kinematic_Range() {}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Range::Kinematic_Range(double MinValX, double MaxValX,
				 double MinValY, double MaxValY,
				 double MinValZ, double MaxValZ)
{
  SetRangeX(MinValX, MaxValX);
  SetRangeY(MinValY, MaxValY);
  SetRangeZ(MinValZ, MaxValZ);
  return;
}


//=================================================================================
// function : ~Kinematic_Range()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Range::~Kinematic_Range() {}


//=================================================================================
// function : SetRangeX()
// purpose  : 
//=================================================================================
void Kinematic_Range::SetRangeX(double MinVal, double MaxVal)
{
  myMinValX = MinVal;
  myMaxValX = MaxVal;
  return;
}


//=================================================================================
// function : SetRangeY()
// purpose  : 
//=================================================================================
void Kinematic_Range::SetRangeY(double MinVal, double MaxVal)
{
  myMinValY = MinVal;
  myMaxValY = MaxVal;
  return;
}


//=================================================================================
// function : SetRangeZ()
// purpose  : 
//=================================================================================
void Kinematic_Range::SetRangeZ(double MinVal, double MaxVal)
{
  myMinValZ = MinVal;
  myMaxValZ = MaxVal;
  return;
}
