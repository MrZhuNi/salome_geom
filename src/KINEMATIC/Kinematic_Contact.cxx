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
//  File   : Kinematic_Contact.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "Kinematic_Contact.hxx"

using namespace std;

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Contact::Kinematic_Contact() {}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Contact::Kinematic_Contact(TopoDS_Shape Shape1, TopoDS_Shape Shape2,
				     int type, double step)
{
  SetType(type);
  SetStep(step);
  SetShape1(Shape1);
  SetShape2(Shape2);

  gp_Pnt Origin(0, 0, 0);
  gp_Vec DirX(1, 0, 0);
  gp_Vec DirY(0, 1, 0);
  gp_Vec DirZ(0, 0, 1);

  Kinematic_Position Pos(Origin, DirX, DirY, DirZ);
  Kinematic_Range AngularRange(0, 0, 0, 0, 0, 0);
  Kinematic_Range LinearRange(0, 0, 0, 0, 0, 0);

  SetPosition(Pos);
  SetAngularRange(AngularRange);
  SetLinearRange(LinearRange);

  return;
}


//=================================================================================
// function : ~Kinematic_Contact()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Contact::~Kinematic_Contact() {}


//=================================================================================
// function : GetLocation()
// purpose  : 
//=================================================================================
TopLoc_Location Kinematic_Contact::GetLocation()
{
  gp_Trsf aTrans;

  aTrans.SetTransformation(myPosition.GetAxe3());
  TopLoc_Location aLoc(aTrans);

  return aLoc;
}


//=================================================================================
// function : GetTransformation()
// purpose  : 
//=================================================================================
gp_Trsf Kinematic_Contact::GetTransformation(double Step)
{
  gp_Vec aVect;
  gp_Trsf aTransformation, aRot, aRot1, aRot2, aRot3;

  double aMinVal = myAngularRange.GetMinValX();
  double aMaxVal = myAngularRange.GetMaxValX();
  aRot1.SetRotation(myPosition.GetAxeX(), (Step * (aMaxVal - aMinVal) + aMinVal) * PI / 180);

  aMinVal = myAngularRange.GetMinValY();
  aMaxVal = myAngularRange.GetMaxValY();
  aRot2.SetRotation(myPosition.GetAxeY(), (Step * (aMaxVal - aMinVal) + aMinVal) * PI / 180);

  aMinVal = myAngularRange.GetMinValZ();
  aMaxVal = myAngularRange.GetMaxValZ();
  aRot3.SetRotation(myPosition.GetAxeZ(), (Step * (aMaxVal - aMinVal) + aMinVal) * PI / 180);

  aVect.SetCoord(Step * (myLinearRange.GetMaxValX() - myLinearRange.GetMinValX()) + myLinearRange.GetMinValX(),
		 Step * (myLinearRange.GetMaxValY() - myLinearRange.GetMinValY()) + myLinearRange.GetMinValY(),
		 Step * (myLinearRange.GetMaxValZ() - myLinearRange.GetMinValZ()) + myLinearRange.GetMinValZ());

  aTransformation.SetTranslation(aVect);
  aRot = aRot1 * aRot2 * aRot3;
  aTransformation = aTransformation * aRot;

  return aTransformation;
}
