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
		 GEOM::GEOM_Shape_ptr Shape2,
		 CORBA::ORB_ptr orb,
		 GEOM::GEOM_Gen_ptr engine);

  ~GEOM_Contact_i();

private:
  Kinematic_Contact* _Contact;
  GEOM::GEOM_Shape_ptr _Shape1;
  GEOM::GEOM_Shape_ptr _Shape2;
  GEOM::GEOM_Position_ptr _Position;
  GEOM::GEOM_Rotation_ptr _Rotation;
  GEOM::GEOM_Translation_ptr _Translation;
  CORBA::ORB_ptr _orb;
  GEOM::GEOM_Gen_ptr _engine;
  
  char* _name;
  char* _shapeid;
  char* _studyshapeid; // exists only if added in the study document

public:
  CORBA::Long GetType()
    throw (SALOME::SALOME_Exception);

  GEOM::GEOM_Shape_ptr GetShape1()
    throw (SALOME::SALOME_Exception);

  GEOM::GEOM_Shape_ptr GetShape2()
    throw (SALOME::SALOME_Exception);

  GEOM::GEOM_Position_ptr GetPosition()
    throw (SALOME::SALOME_Exception);

  GEOM::GEOM_Rotation_ptr GetRotation()
    throw (SALOME::SALOME_Exception);

  GEOM::GEOM_Translation_ptr GetTranslation()
    throw (SALOME::SALOME_Exception);

  CORBA::Double GetStep()
    throw (SALOME::SALOME_Exception);

  char* Name();
  void  Name(const char* name);

  char* ShapeId(); 
  void  ShapeId(const char* shapeid);

  char* StudyShapeId();
  void  StudyShapeId(const char* studyshapeid);

  GEOM::GEOM_Gen_ptr Engine();

};

#endif
