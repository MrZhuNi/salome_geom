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
#include <TopLoc_Location.hxx>
#include <gp_Trsf.hxx>

#include "Kinematic_Position.hxx"
#include "Kinematic_Range.hxx"

class Kinematic_Contact
{
public:
  Kinematic_Contact();
  Kinematic_Contact(TopoDS_Shape Shape1, TopoDS_Shape Shape2,
		    int type, double step);
  ~Kinematic_Contact();

private:
  TopoDS_Shape myShape1;
  TopoDS_Shape myShape2;
  int myType;
  double myStep;
  Kinematic_Position myPosition;
  Kinematic_Range myAngularRange;
  Kinematic_Range myLinearRange;

public:
  TopLoc_Location GetLocation();
  gp_Trsf GetTransformation(double Step);

  void SetType(int aValue){myType = aValue;};
  void SetShape1(TopoDS_Shape aShape){myShape1 = aShape;};
  void SetShape2(TopoDS_Shape aShape){myShape2 = aShape;};
  void SetStep(double aValue){myStep = aValue;};
  void SetPosition(Kinematic_Position& aPosition){myPosition = aPosition;};
  void SetAngularRange(Kinematic_Range& anAngularRange){myAngularRange = anAngularRange;};
  void SetLinearRange(Kinematic_Range& aLinearRange){myLinearRange = aLinearRange;};

  int& GetType(){return myType;};
  TopoDS_Shape& GetShape1(){return myShape1;};
  TopoDS_Shape& GetShape2(){return myShape2;};
  double& GetStep(){return myStep;};
  Kinematic_Position& GetPosition(){return myPosition;};
  Kinematic_Range& GetAngularRange(){return myAngularRange;};
  Kinematic_Range& GetLinearRange(){return myLinearRange;};

};

#endif
