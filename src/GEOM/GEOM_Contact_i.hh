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
//  File   : GEOM_Contact_i.hh
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef __GEOM_CONTACT_I_H__
#define __GEOM_CONTACT_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

#include "GEOM_Shape_i.hh"
#include "Kinematic_Contact.hxx"

//=====================================================================
// GEOM_Contact_i : class definition
//=====================================================================
class GEOM_Contact_i: 
  public POA_GEOM::GEOM_Contact
{
public:
  GEOM_Contact_i();
  GEOM_Contact_i(Kinematic_Contact* Contact,
		 GEOM::GEOM_Shape_ptr Shape1,
		 GEOM::GEOM_Shape_ptr Shape2);

  ~GEOM_Contact_i();

private:
  Kinematic_Contact* _Contact;
  GEOM::GEOM_Shape_ptr _Shape1;
  GEOM::GEOM_Shape_ptr _Shape2;
  
  char* _name;
  char* _shapeid;
  char* _studyshapeid; // exists only if added in the study document

public:
  CORBA::Long GetType() throw (SALOME::SALOME_Exception)
    {return _Contact->GetType();};
  CORBA::Double GetStep() throw (SALOME::SALOME_Exception)
    {return _Contact->GetStep();};
  GEOM::GEOM_Shape_ptr GetShape1() throw (SALOME::SALOME_Exception)
    {return GEOM::GEOM_Shape::_duplicate(_Shape1);};
  GEOM::GEOM_Shape_ptr GetShape2() throw (SALOME::SALOME_Exception)
    {return GEOM::GEOM_Shape::_duplicate(_Shape2);};

  GEOM::ListOfDouble* GetPosition()
    throw (SALOME::SALOME_Exception);
  GEOM::ListOfDouble* GetAngularRange()
    throw (SALOME::SALOME_Exception);
  GEOM::ListOfDouble* GetLinearRange()
    throw (SALOME::SALOME_Exception);

  void SetPosition(double P0x, double P0y, double P0z,
		   double VXx, double VXy, double VXz,
		   double VYx, double VYy, double VYz,
		   double VZx, double VZy, double VZz)
    throw (SALOME::SALOME_Exception);

  void SetAngularRange(double MinValX, double MaxValX,
		       double MinValY, double MaxValY,
		       double MinValZ, double MaxValZ)
    throw (SALOME::SALOME_Exception);

  void SetLinearRange(double MinValX, double MaxValX,
		      double MinValY, double MaxValY,
		      double MinValZ, double MaxValZ)
    throw (SALOME::SALOME_Exception);

  char* Name() {return strdup(_name);};
  void  Name(const char* name);

  char* ShapeId() {return strdup(_shapeid);}; 
  void  ShapeId(const char* shapeid);

  char* StudyShapeId() {return strdup(_studyshapeid) ;};
  void  StudyShapeId(const char* studyshapeid);

};

#endif
