//  GEOM KINEMATIC
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
//  File   : Kinematic_Contact.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_CONTACT_HXX
#define _KINEMATIC_CONTACT_HXX

#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>
#include <gp_Trsf.hxx>
#include <TopLoc_Location.hxx>

#include "Kinematic_Position.hxx"
#include "Kinematic_Rotation.hxx"
#include "Kinematic_Translation.hxx"

class Kinematic_Contact
{
public:
  Kinematic_Contact();
  Kinematic_Contact(TopoDS_Shape Shape1, TopoDS_Shape Shape2,
		    int type, double step);
  ~Kinematic_Contact();

private:
  int myType;
  TopoDS_Shape myShape1;
  TopoDS_Shape myShape2;
  double myStep;
  Kinematic_Position myPosition;
  Kinematic_Rotation myRotation;
  Kinematic_Translation myTranslation;

public:
  gp_Trsf GetTransformation(double Step = 1);
  TopLoc_Location GetLocation();

  void Type(int Type){myType = Type;};
  void Shape1(TopoDS_Shape Shape1){myShape1 = Shape1;};
  void Shape2(TopoDS_Shape Shape2){myShape2 = Shape2;};
  void Step(double Step){myStep = Step;};
  void Position(Kinematic_Position& Position){myPosition = Position;};
  void Rotation(Kinematic_Rotation& Rotation){myRotation = Rotation;};
  void Translation(Kinematic_Translation& Translation){myTranslation = Translation;};

  int& Type(){return myType;};
  TopoDS_Shape& Shape1(){return myShape1;};
  TopoDS_Shape& Shape2(){return myShape2;};
  double& Step(){return myStep;};
  Kinematic_Position& Position(){return myPosition;};
  Kinematic_Rotation& Rotation(){return myRotation;};
  Kinematic_Translation& Translation(){return myTranslation;};

};

#endif
