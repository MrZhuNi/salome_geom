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
//  File   : GEOM_Assembly_i.hh
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef __GEOM_ASSEMBLY_I_H__
#define __GEOM_ASSEMBLY_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

#include "Kinematic_Assembly.hxx"

//=====================================================================
// GEOM_Assembly_i : class definition
//=====================================================================
class GEOM_Assembly_i: 
  public POA_GEOM::GEOM_Assembly
{
public:
  GEOM_Assembly_i();
  GEOM_Assembly_i(Kinematic_Assembly* Ass);

  ~GEOM_Assembly_i();

private:
  Kinematic_Assembly* _Ass;

  GEOM::ListOfContact _aContactList;
  CORBA::Long _NbContact;
  
  char* _name;
  char* _shapeid;
  char* _studyshapeid; // exists only if added in the study document

public:
  void AddContact(GEOM::GEOM_Contact_ptr aContact)
    throw (SALOME::SALOME_Exception);

  GEOM::ListOfContact* GetContactList()
    throw (SALOME::SALOME_Exception);

  CORBA::Long NbContacts() throw (SALOME::SALOME_Exception)
    {return _NbContact;};

  char* Name() {return strdup(_name);};
  void  Name(const char* name);

  char* ShapeId() {return strdup(_shapeid);}; 
  void  ShapeId(const char* shapeid);

  char* StudyShapeId() {return strdup(_studyshapeid);};
  void  StudyShapeId(const char* studyshapeid);

};

#endif
