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
//  File   : GEOM_Rotation_i.cc
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

using namespace std;
#include "GEOM_Rotation_i.hh"

//=================================================================================
// function : GEOM_Rotation_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Rotation_i::GEOM_Rotation_i()
{
}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Rotation_i::GEOM_Rotation_i(CORBA::Long Rot1, CORBA::Long Rot2, CORBA::Long Rot3, 
				 CORBA::Double Val1, CORBA::Double Val2, CORBA::Double Val3)
{

  SetRotation(Rot1, Rot2, Rot3, Val1, Val2, Val3);
  return;

}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Rotation_i::~GEOM_Rotation_i() {}


//=================================================================================
// function : SetRotation()
// purpose  : 
//=================================================================================
void GEOM_Rotation_i::SetRotation(CORBA::Long Rot1, CORBA::Long Rot2, CORBA::Long Rot3, 
				  CORBA::Double Val1, CORBA::Double Val2, CORBA::Double Val3)
  throw(SALOME::SALOME_Exception)
{
  
  _Rot1 = Rot1;
  _Rot2 = Rot2;
  _Rot3 = Rot3;

  _Val1 = Val1;
  _Val2 = Val2;
  _Val3 = Val3;

  return;

}

//=================================================================================
// function : GetRot1()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Rotation_i::GetRot1() throw(SALOME::SALOME_Exception)
{
  return _Rot1;
}

//=================================================================================
// function : GetRot2()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Rotation_i::GetRot2() throw(SALOME::SALOME_Exception)
{
  return _Rot2;
}

//=================================================================================
// function : GetRot3()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Rotation_i::GetRot3() throw(SALOME::SALOME_Exception)
{
  return _Rot3;
}

//=================================================================================
// function : GetVal1()
// purpose  : 
//=================================================================================
CORBA::Double GEOM_Rotation_i::GetVal1() throw(SALOME::SALOME_Exception)
{
  return _Val1;
}

//=================================================================================
// function : GetVal2()
// purpose  : 
//=================================================================================
CORBA::Double GEOM_Rotation_i::GetVal2() throw(SALOME::SALOME_Exception)
{
  return _Val2;
}

//=================================================================================
// function : GetVal3()
// purpose  : 
//=================================================================================
CORBA::Double GEOM_Rotation_i::GetVal3() throw(SALOME::SALOME_Exception)
{
  return _Val3;
}
