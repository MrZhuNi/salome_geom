//  GEOM GEOM : 
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
//  File   : GEOM_Position_i.cc
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

using namespace std;
#include "GEOM_Position_i.hh"

//=================================================================================
// function : GEOM_Position_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Position_i::GEOM_Position_i()
{
}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Position_i::GEOM_Position_i(const GEOM::PointStruct& P0, const GEOM::DirStruct& VX,
				 const GEOM::DirStruct& VY, const GEOM::DirStruct& VZ)
{

  SetOrigin(P0);
  SetVX(VX);
  SetVY(VY);
  SetVZ(VZ);
  return;

}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Position_i::~GEOM_Position_i() {}


//=================================================================================
// function : SetOrigin()
// purpose  : 
//=================================================================================
void GEOM_Position_i::SetOrigin(const GEOM::PointStruct& P0)
  throw(SALOME::SALOME_Exception)
{
  _P0 = P0;
  return;
}

//=================================================================================
// function : SetVX()
// purpose  : 
//=================================================================================
void GEOM_Position_i::SetVX(const GEOM::DirStruct& Vect)
  throw(SALOME::SALOME_Exception)
{
  _VX = Vect;
  return;
}

//=================================================================================
// function : SetVY()
// purpose  : 
//=================================================================================
void GEOM_Position_i::SetVY(const GEOM::DirStruct& Vect)
  throw(SALOME::SALOME_Exception)
{
  _VY = Vect;
  return;
}

//=================================================================================
// function : SetVZ()
// purpose  : 
//=================================================================================
void GEOM_Position_i::SetVZ(const GEOM::DirStruct& Vect)
  throw(SALOME::SALOME_Exception)
{
  _VZ = Vect;
  return;
}

//=================================================================================
// function : GetOrigin()
// purpose  : 
//=================================================================================
GEOM::PointStruct GEOM_Position_i::GetOrigin()
  throw(SALOME::SALOME_Exception)
{
  return _P0;
}

//=================================================================================
// function : GetVX()
// purpose  : 
//=================================================================================
GEOM::DirStruct GEOM_Position_i::GetVX()
  throw(SALOME::SALOME_Exception)
{
  return _VX;
}

//=================================================================================
// function : GetVY()
// purpose  : 
//=================================================================================
GEOM::DirStruct GEOM_Position_i::GetVY()
  throw(SALOME::SALOME_Exception)
{
  return _VY;
}

//=================================================================================
// function : GetVZ()
// purpose  : 
//=================================================================================
GEOM::DirStruct GEOM_Position_i::GetVZ()
  throw(SALOME::SALOME_Exception)
{
  return _VZ;
}
