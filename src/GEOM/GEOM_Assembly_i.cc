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
//  File   : GEOM_Assembly_i.cc
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#include "GEOM_Assembly_i.hh"

using namespace std;

//=================================================================================
// function : GEOM_Assembly_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Assembly_i::GEOM_Assembly_i() {}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Assembly_i::GEOM_Assembly_i(Kinematic_Assembly* Ass)
{
  _Ass = Ass;

  _aContactList.length(0);
  _NbContact = 0;

  _shapeid = "";
  _studyshapeid = "";

  _name = "";
}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Assembly_i::~GEOM_Assembly_i() { delete &_Ass; }


//=================================================================================
// function : Name (set method)
// purpose  : to set the attribute 'name'.
//          : WARNING : Register to naming service actually removed !
//=================================================================================
void GEOM_Assembly_i::Name(const char* name)
{
  _name = strdup(name);
  GEOM::GEOM_Assembly_ptr g = GEOM::GEOM_Assembly::_narrow(_this());
}


//=================================================================================
// function : ShapeId (set method) 
// purpose  : to set the id of this shape in GEOM/OCAF doc
//=================================================================================
void GEOM_Assembly_i::ShapeId(const char * shapeid)
{ _shapeid = strdup(shapeid); }


//=================================================================================
// function : StudyShapeId (set method) 
// purpose  : to set the id of this shape in the Study document (OCAF entry)
//=================================================================================
void GEOM_Assembly_i::StudyShapeId(const char * studyshapeid)
{ _studyshapeid = strdup(studyshapeid); }


//=================================================================================
// function : AddContact()
// purpose  : 
//=================================================================================
void GEOM_Assembly_i::AddContact(GEOM::GEOM_Contact_ptr aContact) throw(SALOME::SALOME_Exception)
{
  _aContactList.length(_NbContact + 1);
  _aContactList[_NbContact] = GEOM::GEOM_Contact::_duplicate(aContact);
  _NbContact++;

  return;
}

//=================================================================================
// function : GetContactList()
// purpose  : 
//=================================================================================
GEOM::ListOfContact* GEOM_Assembly_i::GetContactList() throw(SALOME::SALOME_Exception)
{
  GEOM::ListOfContact_var aContactList = new GEOM::ListOfContact;
  aContactList->length(_aContactList.length());
  for(int i = 0; i < _NbContact; i++)
    aContactList[i] = GEOM::GEOM_Contact::_duplicate(_aContactList[i]);
  
  return aContactList._retn();
}
