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
//  File   : Kinematic_Range.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_RANGE_HXX
#define _KINEMATIC_RANGE_HXX

class Kinematic_Range
{
public:
  Kinematic_Range();
  Kinematic_Range(double MinValX, double MaxValX, double MinValY,
		  double MaxValY, double MinValZ, double MaxValZ);
  ~Kinematic_Range();

private:
  double myMinValX, myMaxValX;
  double myMinValY, myMaxValY;
  double myMinValZ, myMaxValZ;

public:

  void SetRangeX(double MinVal, double MaxVal);
  void SetRangeY(double MinVal, double MaxVal);
  void SetRangeZ(double MinVal, double MaxVal);

  double& GetMinValX(){return myMinValX;};
  double& GetMaxValX(){return myMaxValX;};
  double& GetMinValY(){return myMinValY;};
  double& GetMaxValY(){return myMaxValY;};
  double& GetMinValZ(){return myMinValZ;};
  double& GetMaxValZ(){return myMaxValZ;};

};

#endif
