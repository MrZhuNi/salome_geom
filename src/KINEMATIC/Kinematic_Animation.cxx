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

#include "Kinematic_Animation.hxx"

#include <Geom_Transformation.hxx>
#include <AIS_Shape.hxx>
#include <unistd.h>

using namespace std;

//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Animation::Kinematic_Animation()
{
  myIsShading = false;
  IsCreated = false;
  myNbAIS = 0;
  return;
}


//=======================================================================
// profile
// command to build a profile
//=======================================================================
Kinematic_Animation::Kinematic_Animation(Kinematic_Assembly* Ass, TopoDS_Shape frame,
					 double duration, int nbseq)
{
  myIsShading = false;
  IsCreated = false;
  myNbAIS = 0;

  SetAssembly(Ass);
  SetFrame(frame);
  SetDuration(duration);
  SetNbSeq(nbseq);
  this->SetMap();
  this->InitValues();
  return;
}


//=================================================================================
// function : ~Kinematic_Animation()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
Kinematic_Animation::~Kinematic_Animation() {}


//=================================================================================
// function : SetMap()
// purpose  : Create Kinematic Graph
//=================================================================================
void Kinematic_Animation::SetMap()
{
  list <Kinematic_Contact *> aContactList = myAss->GetContactList();
  Kinematic_Contact* aContact;

  list <Kinematic_Contact *>::const_iterator it = aContactList.begin();
  while(it != aContactList.end()) {
    aContact = *it;
    if(!myIndexToShape.Contains(aContact->GetShape1()))
      myIndexToShape.Add(aContact->GetShape1());

    if(!myIndexToShape.Contains(aContact->GetShape2()))
      myIndexToShape.Add(aContact->GetShape2());
    
    it++;
  }

  for(int i = 1; i <= myIndexToShape.Extent(); i++) {
    it = aContactList.begin();
    list <Kinematic_Contact *> aContactListOfShape;
    TopoDS_Shape aShape = myIndexToShape.FindKey(i);
    while(it != aContactList.end()) {
      aContact = *it;
      if(aShape == aContact->GetShape1() || aShape == aContact->GetShape2())
      aContactListOfShape.push_back(aContact);
      it++;
    }
    myStlMapofShapeListOfContact[i] = aContactListOfShape;
  }

  return;
}


//=================================================================================
// function : InitValues()
// purpose  : 
//=================================================================================
void Kinematic_Animation::InitValues()
{
  list <Kinematic_Contact *> aContactList = myAss->GetContactList();
  list <Kinematic_Contact *>::const_iterator it = aContactList.begin();
  while(it != aContactList.end()) {
    InitValuesOnContact(*it);
    it++;
  }

  return;
}


//=================================================================================
// function : InitValuesOnContact()
// purpose  : 
//=================================================================================
void Kinematic_Animation::InitValuesOnContact(Kinematic_Contact* aContact)
{
  cout<<"Kinematic_Animation::InitValuesOnContact"<<endl;
  list <double> aContactListOfValue;

  cout<<"Kinematic_Animation::InitValuesOnContact : Val = "<<aContact->GetAngularRange().GetMaxValX()<<endl;
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMinValX());
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMaxValX());
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMinValY());
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMaxValY());
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMinValZ());
  aContactListOfValue.push_back(aContact->GetAngularRange().GetMaxValZ());

  aContactListOfValue.push_back(aContact->GetLinearRange().GetMinValX());
  aContactListOfValue.push_back(aContact->GetLinearRange().GetMaxValX());
  aContactListOfValue.push_back(aContact->GetLinearRange().GetMinValY());
  aContactListOfValue.push_back(aContact->GetLinearRange().GetMaxValY());
  aContactListOfValue.push_back(aContact->GetLinearRange().GetMinValZ());
  aContactListOfValue.push_back(aContact->GetLinearRange().GetMaxValZ());

  myStlMapofContactListOfValue[aContact] = aContactListOfValue;
  return;
}


//=================================================================================
// function : GetDisplacement()
// purpose  : 
//=================================================================================
list <double> Kinematic_Animation::GetDisplacement(int aContact)
{
  cout<<"Kinematic_Animation::GetDisplacement"<<endl;
  Kinematic_Contact * aKContact;
  list <Kinematic_Contact *> aContactList = myAss->GetContactList();

  list <Kinematic_Contact *>::const_iterator it = aContactList.begin();
  int i = 0;
  while(it != aContactList.end()) {
    if(i == aContact)
      aKContact = *it;
    i++;
    it++;
  }

  if(myStlMapofContactListOfValue.find(aKContact) == myStlMapofContactListOfValue.end()) {
    cout<<"Kinematic_Animation::GetDisplacement : ADD"<<endl;
    InitValuesOnContact(aKContact);
  }
  
  list <double> aContactListOfValue = myStlMapofContactListOfValue[aKContact];
  return aContactListOfValue;
}


//=================================================================================
// function : SetDisplacement()
// purpose  : 
//=================================================================================
void Kinematic_Animation::SetDisplacement(int aContact, list <double> aList)
{
  cout<<"Kinematic_Animation::SetDisplacement"<<endl;
  Kinematic_Contact * aKContact;
  list <Kinematic_Contact *> aContactList = myAss->GetContactList();

  list <Kinematic_Contact *>::const_iterator it = aContactList.begin();
  int i = 0;
  while(it != aContactList.end()) {
    if(i == aContact)
      aKContact = *it;
    i++;
    it++;
  }

  myStlMapofContactListOfValue[aKContact] = aList;
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
  list <Kinematic_Contact *> aContactListOfShape;
  Kinematic_Contact* aContact;
  TopoDS_Shape aShape, aShape2, aNewShape;

  for(int i = 1; i <= myIndexToShape.Extent(); i++) {
    aShape = myIndexToShape.FindKey(i);
    if(aShape == Shape1) {
      gp_Trsf aNewLoc = aLoc;
      aContactListOfShape = myStlMapofShapeListOfContact[i];
      list <Kinematic_Contact *>::const_iterator it = aContactListOfShape.begin();
      while(it != aContactListOfShape.end()) {
	aContact = *it;
	if(aContact->GetShape1() == Shape1)
	  aShape2 = aContact->GetShape2();
	else if(aContact->GetShape2() == Shape1)
	  aShape2 = aContact->GetShape1();

	if(!myMovedShape.Contains(aShape2)) {
	  myMovedShape.Add(aShape2);

	  if(!IsCreated) {
	    Handle(AIS_Shape) mySimulationShape;
	    mySimulationShape = new AIS_Shape(TopoDS_Shape());
	    mySimulationShape->Set(aShape2);
	    myListOfAIS.Append(mySimulationShape);
	  }
	  else
	    MoveShape(ic, aLoc, aContact, Step);

	  GetNextShape(ic, aLoc, aShape2, Step);
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
// function : MoveShape()
// purpose  : 
//=================================================================================
void Kinematic_Animation::MoveShape(const Handle(AIS_InteractiveContext)& ic,
				    gp_Trsf& aLoc, Kinematic_Contact* aContact,
				    double Step)
{
  gp_Trsf aTrans = GetTransformation(aContact, Step);
  aLoc = aLoc * aTrans;

  myNbAIS++;
  Handle(AIS_Shape) mySimulationShape = Handle(AIS_Shape)::DownCast(myListOfAIS.Value(myNbAIS));
  Handle(Geom_Transformation) aGTrans = new Geom_Transformation();
  aGTrans->SetTrsf(aLoc);
  mySimulationShape->SetTransformation(aGTrans, false, false);

  if(myIsShading) {
    mySimulationShape->SetColor(Quantity_NOC_GOLDENROD);
    ic->SetDisplayMode(mySimulationShape, 1, Standard_False);
  }
  else {
    mySimulationShape->SetColor(Quantity_NOC_GREEN);
    ic->SetDisplayMode(mySimulationShape, 0, Standard_False);
  }
  ic->Display(mySimulationShape, Standard_False);
  return;
}


//=================================================================================
// function : GetTransformation()
// purpose  : 
//=================================================================================
gp_Trsf Kinematic_Animation::GetTransformation(Kinematic_Contact* aContact, double Step)
{
  gp_Vec aVect;
  gp_Trsf aTransformation, aRot, aRot1, aRot2, aRot3;

  list <double> aValuesList = myStlMapofContactListOfValue[aContact];
  list <double>::const_iterator it = aValuesList.begin();
  double aL[12];

  int i = 0;
  while(it != aValuesList.end()) {
    aL[i] = *it;
    i++;
    it++;
  }

  aRot1.SetRotation(aContact->GetPosition().GetAxeX(), (Step * (aL[1] - aL[0]) + aL[0]) * PI / 180);
  aRot2.SetRotation(aContact->GetPosition().GetAxeY(), (Step * (aL[3] - aL[2]) + aL[2]) * PI / 180);
  aRot3.SetRotation(aContact->GetPosition().GetAxeZ(), (Step * (aL[5] - aL[4]) + aL[4]) * PI / 180);
  aRot = aRot1 * aRot2 * aRot3;

  aVect.SetCoord(Step * (aL[7] - aL[6]) + aL[6],
		 Step * (aL[9] - aL[8]) + aL[8],
		 Step * (aL[11] - aL[10]) + aL[10]);

  aTransformation.SetTranslation(aVect);
  aTransformation = aTransformation * aRot;

  return aTransformation;
}
