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
//  File   : GEOM_Animation_i.cc
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

using namespace std;
#include "GEOM_Animation_i.hh"

//=================================================================================
// function : GEOM_Animation_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Animation_i::GEOM_Animation_i()
{
}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Animation_i::GEOM_Animation_i(Kinematic_Animation* Animation,
				   GEOM::GEOM_Assembly_ptr Ass,
				   GEOM::GEOM_Shape_ptr Frame)
{
  _Animation = Animation;
  _Ass = GEOM::GEOM_Assembly::_duplicate(Ass);
  _Frame = GEOM::GEOM_Shape::_duplicate(Frame);

  _shapeid = "";
  _studyshapeid = "";

  _name = "";
}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Animation_i::~GEOM_Animation_i() { delete &_Animation; }


//=================================================================================
// function : GetAssembly()
// purpose  : 
//=================================================================================
GEOM::GEOM_Assembly_ptr GEOM_Animation_i::GetAssembly() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Assembly::_duplicate(_Ass);
}


//=================================================================================
// function : GetFrame()
// purpose  : 
//=================================================================================
GEOM::GEOM_Shape_ptr GEOM_Animation_i::GetFrame() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Shape::_duplicate(_Frame);
}


//=================================================================================
// function : GetDuration()
// purpose  : 
//=================================================================================
CORBA::Double GEOM_Animation_i::GetDuration() throw(SALOME::SALOME_Exception)
{
  return _Animation->Duration();
}


//=================================================================================
// function : GetNbSeq()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Animation_i::GetNbSeq() throw(SALOME::SALOME_Exception)
{
  return _Animation->NbSeq();
}


//=================================================================================
// function : Name (set method)
// purpose  : to set the attribute 'name'.
//          : WARNING : Register to naming service actually removed !
//=================================================================================
void GEOM_Animation_i::Name(const char* name)
{
  _name = strdup(name);
  GEOM::GEOM_Animation_ptr g = GEOM::GEOM_Animation::_narrow(_this());
}


//=================================================================================
// function : Name (get method)
// purpose  : to get the attribute 'name' of this shape
//=================================================================================
char* GEOM_Animation_i::Name() { return strdup(_name); }


//=================================================================================
// function : ShapeId
// purpose  : to get the id of this shape from GEOM (OCAF entry)
//=================================================================================
char* GEOM_Animation_i::ShapeId() { return strdup(_shapeid); }


//=================================================================================
// function : ShapeId (set method) 
// purpose  : to set the id of this shape in GEOM/OCAF doc
//=================================================================================
void GEOM_Animation_i::ShapeId(const char * shapeid) { _shapeid = strdup(shapeid); }


//=================================================================================
// function : StudyShapeId (get method)
// purpose  : to get the id of this shape from the study document (OCAF entry)
//=================================================================================
char* GEOM_Animation_i::StudyShapeId() { return strdup(_studyshapeid) ; }


//=================================================================================
// function : StudyShapeId (set method) 
// purpose  : to set the id of this shape in the Study document (OCAF entry)
//=================================================================================
void GEOM_Animation_i::StudyShapeId(const char * studyshapeid)
{ _studyshapeid = strdup(studyshapeid); }
