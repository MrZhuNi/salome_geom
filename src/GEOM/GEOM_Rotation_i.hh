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
//  File   : GEOM_Rotation_i.hh
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef __GEOM_ROTATION_I_H__
#define __GEOM_ROTATION_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

//=====================================================================
// GEOM_Rotation_i : class definition
//=====================================================================
class GEOM_Rotation_i: 
  public POA_GEOM::GEOM_Rotation
{
public:
  GEOM_Rotation_i();
  GEOM_Rotation_i(CORBA::Long Rot1, CORBA::Long Rot2, CORBA::Long Rot3, 
		  CORBA::Double Val1, CORBA::Double Val2, CORBA::Double Val3);

  ~GEOM_Rotation_i();

private:
  CORBA::Long _Rot1;
  CORBA::Long _Rot2;
  CORBA::Long _Rot3;

  CORBA::Double _Val1;
  CORBA::Double _Val2;
  CORBA::Double _Val3;

public:
  void SetRotation(CORBA::Long Rot1, CORBA::Long Rot2, CORBA::Long Rot3, 
		   CORBA::Double Val1, CORBA::Double Val2, CORBA::Double Val3)
    throw (SALOME::SALOME_Exception);

  CORBA::Long GetRot1()
    throw (SALOME::SALOME_Exception);

  CORBA::Long GetRot2()
    throw (SALOME::SALOME_Exception);

  CORBA::Long GetRot3()
    throw (SALOME::SALOME_Exception);

  CORBA::Double GetVal1()
    throw (SALOME::SALOME_Exception);

  CORBA::Double GetVal2()
    throw (SALOME::SALOME_Exception);

  CORBA::Double GetVal3()
    throw (SALOME::SALOME_Exception);

};

#endif
