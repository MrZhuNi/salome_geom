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
//  File   : Kinematic_Rotation.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_ROTATION_HXX
#define _KINEMATIC_ROTATION_HXX

class Kinematic_Rotation
{
public:
  Kinematic_Rotation();
  Kinematic_Rotation(int Rot1, int Rot2, int Rot3, double ValX, double ValY, double ValZ);
  ~Kinematic_Rotation();

private:
  int myRot1;
  int myRot2; 
  int myRot3;

  double myValX;
  double myValY;
  double myValZ;

public:
  void Rot1(int Rot1){myRot1 = Rot1;};
  void Rot2(int Rot2){myRot2 = Rot2;};
  void Rot3(int Rot3){myRot3 = Rot3;};
  void ValX(double ValX){myValX = ValX;};
  void ValY(double ValY){myValY = ValY;};
  void ValZ(double ValZ){myValZ = ValZ;};

  int& Rot1(){return myRot1;};
  int& Rot2(){return myRot2;};
  int& Rot3(){return myRot3;};
  double& ValX(){return myValX;};
  double& ValY(){return myValY;};
  double& ValZ(){return myValZ;};

};

#endif
