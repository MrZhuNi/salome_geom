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
//  File   : Kinematic_Translation.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_TRANSLATION_HXX
#define _KINEMATIC_TRANSLATION_HXX

class Kinematic_Translation
{
public:
  Kinematic_Translation();
  Kinematic_Translation(double ValX, double ValY, double ValZ);
  ~Kinematic_Translation();

private:
  double myValX;
  double myValY;
  double myValZ;

public:
  void ValX(double ValX){myValX = ValX;};
  void ValY(double ValY){myValY = ValY;};
  void ValZ(double ValZ){myValZ = ValZ;};

  double& ValX(){return myValX;};
  double& ValY(){return myValY;};
  double& ValZ(){return myValZ;};

};

#endif
