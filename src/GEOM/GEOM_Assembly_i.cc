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

using namespace std;
#include "GEOM_Assembly_i.hh"

//=================================================================================
// function : GEOM_Assembly_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Assembly_i::GEOM_Assembly_i()
{
}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Assembly_i::GEOM_Assembly_i(Kinematic_Assembly* Ass,
				 CORBA::ORB_ptr orb,
				 GEOM::GEOM_Gen_ptr engine)
{
  _Ass = Ass;
  _orb = orb;
  _engine = engine;

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
  for(int i = 0; i < _NbContact; i++) {
    aContactList[i] = GEOM::GEOM_Contact::_duplicate(_aContactList[i]);
  }
  return aContactList._retn();
}


//=================================================================================
// function : NbContacts()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Assembly_i::NbContacts() throw(SALOME::SALOME_Exception)
{
  return _NbContact;
}


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
// function : Name (get method)
// purpose  : to get the attribute 'name' of this shape
//=================================================================================
char* GEOM_Assembly_i::Name() { return strdup(_name); }


//=================================================================================
// function : ShapeId
// purpose  : to get the id of this shape from GEOM (OCAF entry)
//=================================================================================
char* GEOM_Assembly_i::ShapeId() { return strdup(_shapeid); }


//=================================================================================
// function : ShapeId (set method) 
// purpose  : to set the id of this shape in GEOM/OCAF doc
//=================================================================================
void GEOM_Assembly_i::ShapeId(const char * shapeid) { _shapeid = strdup(shapeid); }


//=================================================================================
// function : StudyShapeId (get method)
// purpose  : to get the id of this shape from the study document (OCAF entry)
//=================================================================================
char* GEOM_Assembly_i::StudyShapeId() { return strdup(_studyshapeid) ; }


//=================================================================================
// function : StudyShapeId (set method) 
// purpose  : to set the id of this shape in the Study document (OCAF entry)
//=================================================================================
void GEOM_Assembly_i::StudyShapeId(const char * studyshapeid)
{ _studyshapeid = strdup(studyshapeid); }


//=======================================================================
//function : Engine
//purpose  : return generator engine
//=======================================================================
GEOM::GEOM_Gen_ptr GEOM_Assembly_i::Engine() { return _engine; }
