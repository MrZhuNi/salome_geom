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

using namespace std;
#include "Kinematic_Contact.hxx"

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Contact::Kinematic_Contact(){}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Contact::Kinematic_Contact(TopoDS_Shape Shape1, TopoDS_Shape Shape2,
				     int type, double step)
{
  myType = type;
  myShape1 = Shape1;
  myShape2 = Shape2;
  myStep = step;

  gp_Pnt Origin(0, 0, 0);
  gp_Dir DirX(1, 0, 0);
  gp_Dir DirY(0, 1, 0);
  gp_Dir DirZ(0, 0, 1);

  Kinematic_Position Pos(Origin, DirX, DirY, DirZ);
  Kinematic_Rotation Rot(1, 2, 3, 0, 0, 0);
  Kinematic_Translation Trans(0, 0, 0);

  myPosition = Pos;
  myRotation = Rot;
  myTranslation = Trans;
}


//=================================================================================
// function : ~Kinematic_Contact()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Contact::~Kinematic_Contact() {}


//=================================================================================
// function : GetTransformation()
// purpose  : 
//=================================================================================
gp_Trsf Kinematic_Contact::GetTransformation(double Step)
{
  gp_Vec aVect;
  gp_Trsf aTransformation, aRot, aRot1, aRot2, aRot3;

  aVect.SetCoord(Step * myTranslation.ValX(), Step * myTranslation.ValY(), Step * myTranslation.ValZ());
  aTransformation.SetTranslation(aVect);

  if(myRotation.Rot1() == 1) { //Axe X
    aRot1.SetRotation(myPosition.AxeX(), Step * myRotation.ValX() * PI / 180);
    if(myRotation.Rot2() == 2) {
      aRot2.SetRotation(myPosition.AxeY(), Step * myRotation.ValY() * PI / 180);
      aRot3.SetRotation(myPosition.AxeZ(), Step * myRotation.ValZ() * PI / 180);
    } else {
      aRot2.SetRotation(myPosition.AxeZ(), Step * myRotation.ValZ() * PI / 180);
      aRot3.SetRotation(myPosition.AxeY(), Step * myRotation.ValY() * PI / 180);
    }
  } else if(myRotation.Rot1() == 2) { //Axe Y
    aRot1.SetRotation(myPosition.AxeY(), Step * myRotation.ValY() * PI / 180);
    if(myRotation.Rot2() == 1) {
      aRot2.SetRotation(myPosition.AxeX(), Step * myRotation.ValX() * PI / 180);
      aRot3.SetRotation(myPosition.AxeZ(), Step * myRotation.ValZ() * PI / 180);
    } else {
      aRot2.SetRotation(myPosition.AxeZ(), Step * myRotation.ValZ() * PI / 180);
      aRot3.SetRotation(myPosition.AxeX(), Step * myRotation.ValX() * PI / 180);
    }
  } else if(myRotation.Rot1() == 3) { //Axe Z
    aRot1.SetRotation(myPosition.AxeZ(), Step * myRotation.ValZ() * PI / 180);
    if(myRotation.Rot2() == 1) {
      aRot2.SetRotation(myPosition.AxeX(), Step * myRotation.ValX() * PI / 180);
      aRot3.SetRotation(myPosition.AxeY(), Step * myRotation.ValY() * PI / 180);
    } else {
      aRot2.SetRotation(myPosition.AxeY(), Step * myRotation.ValY() * PI / 180);
      aRot3.SetRotation(myPosition.AxeX(), Step * myRotation.ValX() * PI / 180);
    }
  }

  aRot = aRot1 * aRot2 * aRot3;
  aTransformation = aTransformation * aRot;

  return aTransformation;
}


//=================================================================================
// function : GetLocation()
// purpose  : 
//=================================================================================
TopLoc_Location Kinematic_Contact::GetLocation()
{
  gp_Trsf aTrans;

  aTrans.SetTransformation(myPosition.Axe3());
  TopLoc_Location aLoc(aTrans);

  return aLoc;
}
