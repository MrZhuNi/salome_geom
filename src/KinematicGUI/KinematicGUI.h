//  GEOM GEOMGUI : GUI for Geometry component
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
//  File   : KinematicGUI.h
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef KINEMATICGUI_H
#define KINEMATICGUI_H

#include "GEOMBase.h"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)
#include CORBA_SERVER_HEADER(GEOM_Shape)
#include CORBA_SERVER_HEADER(GEOM_Kinematic)

#include "Kinematic_Animation.hxx"
#include "Kinematic_Contact.hxx"

//=================================================================================
// class    : KinematicGUI
// purpose  :
//=================================================================================
class KinematicGUI : public QObject
{
  Q_OBJECT /* for QT compatibility */

public :
  KinematicGUI();
  ~KinematicGUI();

  static bool OnGUIEvent(int theCommandID, QAD_Desktop* parent);

  void AddNewAssembly();
  void AddContact(GEOM::GEOM_Assembly_ptr Ass, GEOM::GEOM_Shape_ptr Shape1,
		  GEOM::GEOM_Shape_ptr Shape2, int type, double step = 0);
  void AddAnimation(GEOM::GEOM_Assembly_ptr Ass, GEOM::GEOM_Shape_ptr Shape1,
		    double Duration, int NbSeq);

  bool AddContactInStudy(GEOM::GEOM_Assembly_ptr Ass, 
			 GEOM::GEOM_Contact_ptr aContact);
  bool AddAssemblyInStudy(GEOM::GEOM_Assembly_ptr aAssembly);
  bool AddAnimationInStudy(GEOM::GEOM_Animation_ptr aAnimation);
  TCollection_AsciiString GetNameFromType(int type);

  void SetPosition(GEOM::GEOM_Contact_ptr aContact, GEOM::PointStruct P0,
			       GEOM::DirStruct VX, GEOM::DirStruct VY, GEOM::DirStruct VZ);
  void SetRotation(GEOM::GEOM_Contact_ptr aContact,
		   int Rot1, int Rot2, int Rot3,
		   double Val1, double Val2, double Val3);
  void SetTranslation(GEOM::GEOM_Contact_ptr aContact,
		      double Val1, double Val2, double Val3);

  Kinematic_Contact* CreateContact(GEOM::GEOM_Contact_ptr aContact);
  Kinematic_Assembly* CreateAssembly(GEOM::GEOM_Assembly_ptr aAssembly);
  Kinematic_Animation* CreateAnimation(GEOM::GEOM_Animation_ptr aAnimation);

  GEOMBase* myGeomBase;
  GEOMContext* myGeomGUI;
  GEOM::GEOM_Gen_var myGeom;   /* Current Geom Component */

};

#endif
