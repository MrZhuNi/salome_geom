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
//  File   : Kinematic_Animation.hxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#ifndef _KINEMATIC_ANIMATION_HXX
#define _KINEMATIC_ANIMATION_HXX

#include "Kinematic_Assembly.hxx"

#include <TopoDS_Shape.hxx>
#include <TDF_Label.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_SequenceOfInteractive.hxx>

#include <list>
#include <map>

class Kinematic_Animation
{
public:
  Kinematic_Animation();
  Kinematic_Animation(Kinematic_Assembly* Ass, TopoDS_Shape frame,
		      double duration, int nbseq, bool isinloop);
  ~Kinematic_Animation();

private:
  AIS_SequenceOfInteractive ListOfAIS;
  TopTools_IndexedMapOfShape myIndexToShape;
  TopTools_IndexedMapOfShape myMovedShape;
  map <int, list <Kinematic_Contact *> > myStlMapofShapeListOfContact;
  Kinematic_Assembly* myAss;
  TopoDS_Shape myFrame;
  double myDuration;
  int myNbSeq;
  int myNbAIS;
  bool myIsInLoop;
  bool IsCreated;

public:
  void SetMap();
  void Animate(const Handle(AIS_InteractiveContext)& ic);
  void GetNextShape(const Handle (AIS_InteractiveContext)& ic, 
		    gp_Trsf& aLoc, TopoDS_Shape Shape1, double Step);
  void MoveShape(const Handle(AIS_InteractiveContext)& ic,
		 gp_Trsf& aLoc, Kinematic_Contact* aContact,
		 double Step);

  TopoDS_Shape& Frame(){return myFrame;};
  double& Duration(){return myDuration;};
  int& NbSeq(){return myNbSeq;};
  bool& IsInLoop(){return myIsInLoop;};

};

#endif
