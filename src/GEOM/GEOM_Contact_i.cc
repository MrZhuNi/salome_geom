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

#include "GEOM_Contact_i.hh"

#include "utilities.h"

using namespace std;

//=================================================================================
// function : GEOM_Contact_i() constructor (no arguments)
// purpose  : for what now ?
//=================================================================================
GEOM_Contact_i::GEOM_Contact_i() {}


//=================================================================================
// function : constructor
// purpose  : constructor for servant creation
//=================================================================================
GEOM_Contact_i::GEOM_Contact_i(Kinematic_Contact* Contact,
			       GEOM::GEOM_Shape_ptr Shape1,
			       GEOM::GEOM_Shape_ptr Shape2)
{
  _Contact = Contact;
  _Shape1 = GEOM::GEOM_Shape::_duplicate(Shape1);
  _Shape2 = GEOM::GEOM_Shape::_duplicate(Shape2);

  _shapeid = "";
  _studyshapeid = "";
  _name = "";

  return;
}


//=================================================================================
// function : destructor
// purpose  : 
//=================================================================================
GEOM_Contact_i::~GEOM_Contact_i() { delete &_Contact; }


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
// function : ShapeId (set method) 
// purpose  : to set the id of this shape in GEOM/OCAF doc
//=================================================================================
void GEOM_Contact_i::ShapeId(const char * shapeid)
{ _shapeid = strdup(shapeid); }


//=================================================================================
// function : StudyShapeId (set method) 
// purpose  : to set the id of this shape in the Study document (OCAF entry)
//=================================================================================
void GEOM_Contact_i::StudyShapeId(const char * studyshapeid)
{ _studyshapeid = strdup(studyshapeid); }


//=================================================================================
// function : GetPosition()
// purpose  : 
//=================================================================================
GEOM::ListOfDouble* GEOM_Contact_i::GetPosition() throw(SALOME::SALOME_Exception)
{
  GEOM::ListOfDouble_var aPositionList = new GEOM::ListOfDouble;
  aPositionList->length(12);

  aPositionList[0] = _Contact->GetPosition().GetOrigin().X();
  aPositionList[1] = _Contact->GetPosition().GetOrigin().Y();
  aPositionList[2] = _Contact->GetPosition().GetOrigin().Z();

  aPositionList[3] = _Contact->GetPosition().GetDirX().X();
  aPositionList[4] = _Contact->GetPosition().GetDirX().Y();
  aPositionList[5] = _Contact->GetPosition().GetDirX().Z();

  aPositionList[6] = _Contact->GetPosition().GetDirY().X();
  aPositionList[7] = _Contact->GetPosition().GetDirY().Y();
  aPositionList[8] = _Contact->GetPosition().GetDirY().Z();

  aPositionList[9] = _Contact->GetPosition().GetDirZ().X();
  aPositionList[10] = _Contact->GetPosition().GetDirZ().Y();
  aPositionList[11] = _Contact->GetPosition().GetDirZ().Z();

  return aPositionList._retn();
}


//=================================================================================
// function : GetAngularRange()
// purpose  : 
//=================================================================================
GEOM::ListOfDouble* GEOM_Contact_i::GetAngularRange() throw(SALOME::SALOME_Exception)
{
  GEOM::ListOfDouble_var anAngularRangeList = new GEOM::ListOfDouble;
  anAngularRangeList->length(6);

  anAngularRangeList[0] = _Contact->GetAngularRange().GetMinValX();
  anAngularRangeList[1] = _Contact->GetAngularRange().GetMaxValX();

  anAngularRangeList[2] = _Contact->GetAngularRange().GetMinValY();
  anAngularRangeList[3] = _Contact->GetAngularRange().GetMaxValY();

  anAngularRangeList[4] = _Contact->GetAngularRange().GetMinValZ();
  anAngularRangeList[5] = _Contact->GetAngularRange().GetMaxValZ();

  return anAngularRangeList._retn();
}


//=================================================================================
// function : GetLinearRange()
// purpose  : 
//=================================================================================
GEOM::ListOfDouble* GEOM_Contact_i::GetLinearRange() throw(SALOME::SALOME_Exception)
{
  GEOM::ListOfDouble_var aLinearRangeList = new GEOM::ListOfDouble;
  aLinearRangeList->length(6);

  aLinearRangeList[0] = _Contact->GetLinearRange().GetMinValX();
  aLinearRangeList[1] = _Contact->GetLinearRange().GetMaxValX();

  aLinearRangeList[2] = _Contact->GetLinearRange().GetMinValY();
  aLinearRangeList[3] = _Contact->GetLinearRange().GetMaxValY();

  aLinearRangeList[4] = _Contact->GetLinearRange().GetMinValZ();
  aLinearRangeList[5] = _Contact->GetLinearRange().GetMaxValZ();

  return aLinearRangeList._retn();
}


//=================================================================================
// function : SetPosition()
// purpose  : 
//=================================================================================
void GEOM_Contact_i::SetPosition(double P0x, double P0y, double P0z,
				 double VXx, double VXy, double VXz,
				 double VYx, double VYy, double VYz,
				 double VZx, double VZy, double VZz)
  throw(SALOME::SALOME_Exception)
{
  gp_Pnt Center(P0x, P0y, P0z);
  _Contact->GetPosition().SetOrigin(Center);
  
  if(VXx == 0 && VXy == 0 && VXz == 0) {
    MESSAGE("Null Vector in Contact Position (SetVX)."<<endl);
  } else {
    gp_Vec aDirX(VXx, VXy, VXz);
    _Contact->GetPosition().SetDirX(aDirX);
  }

  if(VYx == 0 && VYy == 0 && VYz == 0) {
    MESSAGE("Null Vector in Contact Position (SetVY)."<<endl);
  } else {
    gp_Vec aDirY(VYx, VYy, VYz);
    _Contact->GetPosition().SetDirY(aDirY);
  }

  if(VZx == 0 && VZy == 0 && VZz == 0) {
    MESSAGE("Null Vector in Contact Position (SetVZ)."<<endl);
  } else {
    gp_Vec aDirZ(VZx, VZy, VZz);
    _Contact->GetPosition().SetDirZ(aDirZ);
  }

  return;
}


//=================================================================================
// function : SetAngularRange()
// purpose  : 
//=================================================================================
void GEOM_Contact_i::SetAngularRange(double MinValX, double MaxValX,
				     double MinValY, double MaxValY,
				     double MinValZ, double MaxValZ)
  throw(SALOME::SALOME_Exception)
{
  _Contact->GetAngularRange().SetRangeX(MinValX, MaxValX);
  _Contact->GetAngularRange().SetRangeY(MinValY, MaxValY);
  _Contact->GetAngularRange().SetRangeZ(MinValZ, MaxValZ);

  if(_Contact->GetType() == 9) { //HELICOIDAL
    double p = _Contact->GetStep() / 360;
    _Contact->GetLinearRange().SetRangeX(p * MinValX, p * MaxValX);
    _Contact->GetLinearRange().SetRangeY(p * MinValY, p * MaxValY);
    _Contact->GetLinearRange().SetRangeZ(p * MinValZ, p * MaxValZ);
  }

  return;
}


//=================================================================================
// function : SetLinearRange()
// purpose  : 
//=================================================================================
void GEOM_Contact_i::SetLinearRange(double MinValX, double MaxValX,
				    double MinValY, double MaxValY,
				    double MinValZ, double MaxValZ)
  throw(SALOME::SALOME_Exception)
{
  _Contact->GetLinearRange().SetRangeX(MinValX, MaxValX);
  _Contact->GetLinearRange().SetRangeY(MinValY, MaxValY);
  _Contact->GetLinearRange().SetRangeZ(MinValZ, MaxValZ);

  if(_Contact->GetType() == 9) { //HELICOIDAL
    double p = 360 / _Contact->GetStep();
    _Contact->GetAngularRange().SetRangeX(p * MinValX, p * MaxValX);
    _Contact->GetAngularRange().SetRangeY(p * MinValY, p * MaxValY);
    _Contact->GetAngularRange().SetRangeZ(p * MinValZ, p * MaxValZ);
  }

  return;
}
