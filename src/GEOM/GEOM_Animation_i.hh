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
//  File   : GEOM_Animation_i.hh
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header$

#ifndef __GEOM_ANIMATION_I_H__
#define __GEOM_ANIMATION_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

#include "GEOM_Shape_i.hh"
#include "Kinematic_Animation.hxx"

//=====================================================================
// GEOM_Animation_i : class definition
//=====================================================================
class GEOM_Animation_i: 
  public POA_GEOM::GEOM_Animation
{
public:
  GEOM_Animation_i();
  GEOM_Animation_i(Kinematic_Animation* Animation,
		   GEOM::GEOM_Assembly_ptr Ass,
		   GEOM::GEOM_Shape_ptr Frame);

  ~GEOM_Animation_i();

private:
  Kinematic_Animation* _Animation;
  GEOM::GEOM_Assembly_ptr _Ass;
  GEOM::GEOM_Shape_ptr _Frame;
  
  char* _name;
  char* _shapeid;
  char* _studyshapeid; // exists only if added in the study document

public:
  GEOM::GEOM_Assembly_ptr GetAssembly() throw (SALOME::SALOME_Exception)
    {return GEOM::GEOM_Assembly::_duplicate(_Ass);};

  GEOM::GEOM_Shape_ptr GetFrame() throw (SALOME::SALOME_Exception)
    {return GEOM::GEOM_Shape::_duplicate(_Frame);};

  CORBA::Double GetDuration() throw (SALOME::SALOME_Exception)
    {return _Animation->GetDuration();};

  CORBA::Long GetNbSeq() throw (SALOME::SALOME_Exception)
    {return _Animation->GetNbSeq();};

  GEOM::ListOfDouble* GetDisplacement(GEOM::GEOM_Contact_ptr aContact)
    throw (SALOME::SALOME_Exception);

  void SetDisplacement(GEOM::GEOM_Contact_ptr aContact,
		       const GEOM::ListOfDouble& aList)
    throw (SALOME::SALOME_Exception);

  char* Name() {return strdup(_name);};
  void  Name(const char* name);

  char* ShapeId() {return strdup(_shapeid);}; 
  void  ShapeId(const char* shapeid);

  char* StudyShapeId() {return strdup(_studyshapeid);};
  void  StudyShapeId(const char* studyshapeid);

};

#endif
