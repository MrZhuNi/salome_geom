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
//  File   : GEOM_Contact_i.cc
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

using namespace std;
#include "GEOM_Contact_i.hh"
#include "GEOM_Position_i.hh"
#include "GEOM_Rotation_i.hh"
#include "GEOM_Translation_i.hh"

//=================================================================================
// function : GEOM_Contact_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Contact_i::GEOM_Contact_i()
{
}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Contact_i::GEOM_Contact_i(Kinematic_Contact* Contact,
			       GEOM::GEOM_Shape_ptr Shape1,
			       GEOM::GEOM_Shape_ptr Shape2,
			       CORBA::ORB_ptr orb,
			       GEOM::GEOM_Gen_ptr engine)
{
  _Contact = Contact;
  _Shape1 = GEOM::GEOM_Shape::_duplicate(Shape1);
  _Shape2 = GEOM::GEOM_Shape::_duplicate(Shape2);

  GEOM::PointStruct p0 = engine->MakePointStruct(Contact->Position().Origin().X(),
						 Contact->Position().Origin().Y(),
						 Contact->Position().Origin().Z());
  GEOM::PointStruct pX = engine->MakePointStruct(Contact->Position().DirX().X(),
						 Contact->Position().DirX().Y(),
						 Contact->Position().DirX().Z());
  GEOM::PointStruct pY = engine->MakePointStruct(Contact->Position().DirY().X(),
						 Contact->Position().DirY().Y(),
						 Contact->Position().DirY().Z());
  GEOM::PointStruct pZ = engine->MakePointStruct(Contact->Position().DirZ().X(),
						 Contact->Position().DirZ().Y(),
						 Contact->Position().DirZ().Z());
  GEOM::DirStruct VX = engine->MakeDirection(pX);
  GEOM::DirStruct VY = engine->MakeDirection(pY);
  GEOM::DirStruct VZ = engine->MakeDirection(pZ);
  GEOM_Position_i * Position_servant = new GEOM_Position_i(p0, VX, VY, VZ);
  _Position = GEOM::GEOM_Position::_narrow(Position_servant->_this());

  GEOM_Rotation_i * Rotation_servant = new GEOM_Rotation_i(Contact->Rotation().Rot1(),
							   Contact->Rotation().Rot2(),
							   Contact->Rotation().Rot3(),
							   Contact->Rotation().ValX(),
							   Contact->Rotation().ValY(),
							   Contact->Rotation().ValZ());
  _Rotation = GEOM::GEOM_Rotation::_narrow(Rotation_servant->_this());
  GEOM_Translation_i * Translation_servant = new GEOM_Translation_i(0, 0, 0);
  _Translation = GEOM::GEOM_Translation::_narrow(Translation_servant->_this());

  _orb = orb;
  _engine = engine;

  _shapeid = "";
  _studyshapeid = "";

  _name = "";
}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Contact_i::~GEOM_Contact_i() { delete &_Contact; }


//=================================================================================
// function : GetType()
// purpose  : 
//=================================================================================
CORBA::Long GEOM_Contact_i::GetType() throw(SALOME::SALOME_Exception)
{
  return _Contact->Type();
}


//=================================================================================
// function : GetShape1()
// purpose  : 
//=================================================================================
GEOM::GEOM_Shape_ptr GEOM_Contact_i::GetShape1() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Shape::_duplicate(_Shape1);
}


//=================================================================================
// function : GetShape2()
// purpose  : 
//=================================================================================
GEOM::GEOM_Shape_ptr GEOM_Contact_i::GetShape2() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Shape::_duplicate(_Shape2);
}


//=================================================================================
// function : GetPosition()
// purpose  : 
//=================================================================================
GEOM::GEOM_Position_ptr GEOM_Contact_i::GetPosition() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Position::_duplicate(_Position);
}


//=================================================================================
// function : GetRotation()
// purpose  : 
//=================================================================================
GEOM::GEOM_Rotation_ptr GEOM_Contact_i::GetRotation() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Rotation::_duplicate(_Rotation);
}


//=================================================================================
// function : GetTranslation()
// purpose  : 
//=================================================================================
GEOM::GEOM_Translation_ptr GEOM_Contact_i::GetTranslation() throw(SALOME::SALOME_Exception)
{
  return GEOM::GEOM_Translation::_duplicate(_Translation);
}


//=================================================================================
// function : GetStep()
// purpose  : 
//=================================================================================
CORBA::Double GEOM_Contact_i::GetStep() throw(SALOME::SALOME_Exception)
{
  return _Contact->Step();
}


//=================================================================================
// function : Name (set method)
// purpose  : to set the attribute 'name'.
//          : WARNING : Register to naming service actually removed !
//=================================================================================
void GEOM_Contact_i::Name(const char* name)
{
  _name = strdup(name);
  GEOM::GEOM_Contact_ptr g = GEOM::GEOM_Contact::_narrow(_this());
}


//=================================================================================
// function : Name (get method)
// purpose  : to get the attribute 'name' of this shape
//=================================================================================
char* GEOM_Contact_i::Name() { return strdup(_name); }


//=================================================================================
// function : ShapeId
// purpose  : to get the id of this shape from GEOM (OCAF entry)
//=================================================================================
char* GEOM_Contact_i::ShapeId() { return strdup(_shapeid); }


//=================================================================================
// function : ShapeId (set method) 
// purpose  : to set the id of this shape in GEOM/OCAF doc
//=================================================================================
void GEOM_Contact_i::ShapeId(const char * shapeid) { _shapeid = strdup(shapeid); }


//=================================================================================
// function : StudyShapeId (get method)
// purpose  : to get the id of this shape from the study document (OCAF entry)
//=================================================================================
char* GEOM_Contact_i::StudyShapeId() { return strdup(_studyshapeid) ; }


//=================================================================================
// function : StudyShapeId (set method) 
// purpose  : to set the id of this shape in the Study document (OCAF entry)
//=================================================================================
void GEOM_Contact_i::StudyShapeId(const char * studyshapeid)
{ _studyshapeid = strdup(studyshapeid); }


//=======================================================================
//function : Engine
//purpose  : return generator engine
//=======================================================================
GEOM::GEOM_Gen_ptr GEOM_Contact_i::Engine() { return _engine; }
