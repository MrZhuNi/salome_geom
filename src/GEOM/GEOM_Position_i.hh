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
//  File   : GEOM_Position_i.hh
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef __GEOM_POSITION_I_H__
#define __GEOM_POSITION_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

//=====================================================================
// GEOM_Position_i : class definition
//=====================================================================
class GEOM_Position_i: 
  public POA_GEOM::GEOM_Position
{
public:
  GEOM_Position_i();
  GEOM_Position_i(const GEOM::PointStruct& P0, const GEOM::DirStruct& VX,
		  const GEOM::DirStruct& VY, const GEOM::DirStruct& VZ);

  ~GEOM_Position_i();

private:
  GEOM::PointStruct _P0;
  GEOM::DirStruct _VX;
  GEOM::DirStruct _VY;
  GEOM::DirStruct _VZ;

public:
  void SetOrigin(const GEOM::PointStruct& P0)
    throw (SALOME::SALOME_Exception);

  void SetVX(const GEOM::DirStruct& Vect)
    throw (SALOME::SALOME_Exception);

  void SetVY(const GEOM::DirStruct& Vect)
    throw (SALOME::SALOME_Exception);

  void SetVZ(const GEOM::DirStruct& Vect)
    throw (SALOME::SALOME_Exception);

  GEOM::PointStruct GetOrigin()
    throw (SALOME::SALOME_Exception);

  GEOM::DirStruct GetVX()
    throw (SALOME::SALOME_Exception);

  GEOM::DirStruct GetVY()
    throw (SALOME::SALOME_Exception);

  GEOM::DirStruct GetVZ()
    throw (SALOME::SALOME_Exception);

};

#endif
