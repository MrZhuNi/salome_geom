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
//  File   : Kinematic_Animation.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "Kinematic_Animation.hxx"

#include <Geom_Transformation.hxx>
#include <AIS_Shape.hxx>
#include <unistd.h>

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Animation::Kinematic_Animation(){}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Animation::Kinematic_Animation(Kinematic_Assembly* Ass, TopoDS_Shape frame,
					 double duration, int nbseq, bool isinloop)
{
  myAss = Ass;
  myFrame = frame;
  myDuration = duration;
  myNbSeq = nbseq;
  myIsInLoop = isinloop;
  IsCreated = false;
  myNbAIS = 1;
  this->SetMap();

  return;
}


//=================================================================================
// function : ~Kinematic_Animation()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Animation::~Kinematic_Animation() {}


//=================================================================================
// function : SetMap()
// purpose  : 
//=================================================================================
void Kinematic_Animation::SetMap()
{
  list <Kinematic_Contact *> ContactList = myAss->GetContactList();
  Kinematic_Contact* aContact;

  list <Kinematic_Contact *>::const_iterator it = ContactList.begin();
  while(it != ContactList.end()) {
    aContact = *it;
    if(!myIndexToShape.Contains(aContact->Shape1()))
      myIndexToShape.Add(aContact->Shape1());

    if(!myIndexToShape.Contains(aContact->Shape2()))
      myIndexToShape.Add(aContact->Shape2());
    
    it++;
  }

  for(int i = 1; i <= myIndexToShape.Extent(); i++) {
    it = ContactList.begin();
    list <Kinematic_Contact *> ContactListOfShape;
    TopoDS_Shape myShape = myIndexToShape.FindKey(i);
    while(it != ContactList.end()) {
      aContact = *it;
      if(myShape == aContact->Shape1() || myShape == aContact->Shape2())
      ContactListOfShape.push_back(aContact);
      it++;
    }
    myStlMapofShapeListOfContact[i] = ContactListOfShape;
  }

  return;
}


//=================================================================================
// function : Animate()
// purpose  : 
//=================================================================================
void Kinematic_Animation::Animate(const Handle(AIS_InteractiveContext)& ic)
{
  bool IsOk = false;
  for(int cpt = 1; cpt <= myIndexToShape.Extent(); cpt++) {
    TopoDS_Shape myShape = myIndexToShape.FindKey(cpt);
    if(myShape == myFrame)
      IsOk = true;
  }
  if(!IsOk)
    return;

  myMovedShape.Clear();
  myMovedShape.Add(myFrame);

  IsCreated = false;
  gp_Trsf aLoc;
  GetNextShape(ic, aLoc, myFrame, 1);
  IsCreated = true;

  Handle(AIS_Shape) mySimulationShape;
  mySimulationShape = new AIS_Shape(TopoDS_Shape());
  mySimulationShape->Set(myFrame);
  mySimulationShape->SetColor(Quantity_NOC_RED);
  ic->Deactivate(mySimulationShape);
  ic->Display(mySimulationShape, Standard_False);
  ic->UpdateCurrentViewer();

  double Step, Val;
  Step = 1.0 / myNbSeq;
  for(int i = 0; i <= myNbSeq; i++) {
    Val = i * Step;

    myMovedShape.Clear();
    myMovedShape.Add(myFrame);

    myNbAIS = 1;
    GetNextShape(ic, aLoc, myFrame, Val);

    ic->UpdateCurrentViewer();
    usleep(myDuration / myNbSeq * 1e6);
  }

  ic->Erase(mySimulationShape, Standard_True, Standard_False);
  ic->ClearPrs(mySimulationShape);

  for(int i = 1; i <= myNbAIS; i++) {
    Handle(AIS_Shape) myShape = Handle(AIS_Shape)::DownCast(ListOfAIS.Value(i));
    ic->Erase(myShape, Standard_True, Standard_False);
    ic->ClearPrs(myShape);
  }

  ic->UpdateCurrentViewer();

  return;
}


//=================================================================================
// function : GetNextShape()
// purpose  : 
//=================================================================================
void Kinematic_Animation::GetNextShape(const Handle(AIS_InteractiveContext)& ic,
				       gp_Trsf& aLoc, TopoDS_Shape Shape1, double Step)
{
  int j = 0;
  list <Kinematic_Contact *> ContactListOfShape;
  Kinematic_Contact* aContact;
  TopoDS_Shape myShape, myShape2, aNewShape;

  for(int i = 1; i <= myIndexToShape.Extent(); i++) {
    myShape = myIndexToShape.FindKey(i);
    if(myShape == Shape1) {
      gp_Trsf aNewLoc = aLoc;
      ContactListOfShape = myStlMapofShapeListOfContact[i];
      list <Kinematic_Contact *>::const_iterator it = ContactListOfShape.begin();
      while(it != ContactListOfShape.end()) {
	aContact = *it;
	if(aContact->Shape1() == Shape1)
	  myShape2 = aContact->Shape2();
	else if(aContact->Shape2() == Shape1)
	  myShape2 = aContact->Shape1();

	if(!myMovedShape.Contains(myShape2)) {
	  myMovedShape.Add(myShape2);

	  if(!IsCreated) {
	    Handle(AIS_Shape) mySimulationShape;
	    mySimulationShape = new AIS_Shape(TopoDS_Shape());
	    mySimulationShape->Set(myShape2);
	    mySimulationShape->SetColor(Quantity_NOC_GREEN);
	    ic->Deactivate(mySimulationShape);
	    ListOfAIS.Append(mySimulationShape);
	  }
	  else
	    MoveShape(ic, aLoc, aContact, Step);

	  GetNextShape(ic, aLoc, myShape2, Step);
	}
	aLoc = aNewLoc;
	it++;
      }
      return;
    }
  }
  return;
}


//=================================================================================
// function : CreateShape()
// purpose  : 
//=================================================================================
void Kinematic_Animation::MoveShape(const Handle(AIS_InteractiveContext)& ic,
				    gp_Trsf& aLoc, Kinematic_Contact* aContact,
				    double Step)
{
  gp_Trsf aTrans = aContact->GetTransformation(Step);
  aLoc = aLoc * aTrans;

  Handle(AIS_Shape) mySimulationShape = Handle(AIS_Shape)::DownCast(ListOfAIS.Value(myNbAIS));
  Handle(Geom_Transformation) aGTrans = new Geom_Transformation();
  aGTrans->SetTrsf(aLoc);
  mySimulationShape->SetTransformation(aGTrans, false, false);

  ic->Display(mySimulationShape, Standard_False);

  myNbAIS++;
  return;
}
