//  GEOM GEOMDS : implementation of Geometry component data structure and Geometry documents management
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
//  File   : GeomDS_Commands.cxx
//  Author : Yves FRICAUD/Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "utilities.h"
#include "GEOMDS_Commands.ixx"

#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_RealArray.hxx>
#include <TDataStd_IntegerArray.hxx>
#include <TDF_Reference.hxx>
#include <TNaming_Tool.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Tool.hxx>

#include "Kinematic_Contact.hxx"
#include "Kinematic_Animation.hxx"


//=======================================================================
//function : GEOMDS_Commands
//purpose  : 
//=======================================================================
GEOMDS_Commands::GEOMDS_Commands(const TDF_Label& Main)
  : myLab(Main)
{
}


//=======================================================================
// function : Generated()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::Generated(const TopoDS_Shape& S,
				     const TCollection_ExtendedString& Name)
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Generated(S);
  TDataStd_Name::Set(NewLab,Name);
  return NewLab;
}



//=======================================================================
// function : Generated()
// purpose  : 
//=======================================================================
TDF_Label GEOMDS_Commands::Generated(const TopoDS_Shape& S1,
				     const TopoDS_Shape& S2,
				     const TCollection_ExtendedString& Name)
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Generated(S1,S2);
  TDataStd_Name::Set(NewLab,Name);
  return NewLab;
}



//=======================================================================
// function : AddShape()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddShape(const TopoDS_Shape& S,
				    const TCollection_ExtendedString& Name)
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Select(S,S);
  TDataStd_Name::Set(NewLab,Name);
  return NewLab;
}


//=======================================================================
// function : AddIndependentShape()
// purpose  : SAME than AddShape() : will be renamed later
//=======================================================================
TDF_Label GEOMDS_Commands::AddIndependentShape(const TopoDS_Shape& S, 
					       const TCollection_AsciiString& nameIOR)
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Select(S,S);
  TDataStd_Name::Set(NewLab, nameIOR);
  return NewLab;
}


//=======================================================================
// function : AddDependentShape()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddDependentShape(const TopoDS_Shape& S,
					     const TCollection_AsciiString& nameIOR,
					     const TDF_Label& mainLab)
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Select(S,S);
  TDataStd_Name::Set(NewLab, nameIOR);
  /* NewLab has a reference attribute to mainLab (the main shape in fact) */
  TDF_Reference::Set(NewLab, mainLab) ;
  return NewLab;
}



//=======================================================================
// function : AddConstructiveElement()
// purpose  : 
//=======================================================================
TDF_Label GEOMDS_Commands::AddConstructiveElement(const TopoDS_Shape& S,
						  const TCollection_ExtendedString& nameIOR,
						  const GEOMDS_ConstructiveType& aType) 
{
  TDF_Label NewLab = myLab.NewChild();
  TNaming_Builder B(NewLab);
  B.Select(S,S);
  TDataStd_Name::Set(NewLab, nameIOR);
  /* Add the Attribute Constructive Element coded with a TDataStd_Integer from an enum */
  TDataStd_Integer::Set(NewLab, Standard_Integer(aType));
  return NewLab;
}


//=======================================================================
// function : AddIORNameAttribute()       
// purpose  : Add attribute TDataStd_Name to a label
//          : this attribute represents the name/IOR of object
//          : Return false if attribute exist before
//=======================================================================
Standard_Boolean GEOMDS_Commands::AddIORNameAttribute(const TDF_Label& aLabel,
						      const TCollection_ExtendedString& nameIOR)
{
  if( this->HasIOR(aLabel) )
    return false ;
  TDataStd_Name::Set(aLabel, nameIOR);
  return true ; 
}



//=======================================================================
// function : IsConstructiveElement() 1/2
// purpose  : Return true if 'aLabel' is a constructive element
//=======================================================================
Standard_Boolean GEOMDS_Commands::IsConstructiveElement(const TDF_Label& aLabel)
{
  Handle(TDataStd_Integer) anAttType ;
  if( aLabel.FindAttribute(TDataStd_Integer::GetID(), anAttType ) )
    return true ;
  return false;
}


//=======================================================================
// function : IsConstructiveElement() 2/2
// purpose  : Return true if 'aLabel' is a constructive element and return the
//          : topology ' returnTopo' and type 'returnType'
//=======================================================================
Standard_Boolean GEOMDS_Commands::IsConstructiveElement(const TDF_Label& aLabel,
							TopoDS_Shape& returnTopo,
							GEOMDS_ConstructiveType& returnType)
{
  Handle(TDataStd_Integer) anAttType ;
  Handle(TNaming_NamedShape) anAttTopo ;

  if( aLabel.FindAttribute(TDataStd_Integer::GetID(), anAttType) && aLabel.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo)) {

    returnTopo = TNaming_Tool::GetShape(anAttTopo) ;
    returnType = GEOMDS_ConstructiveType( anAttType->Get() ) ;
    return true ;
  }
  return false;
}


//=======================================================================
// function : GetShape()
// purpose  : return true and 'returnTopo' if a topology is found on 'aLabel'
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetShape(const TDF_Label& aLabel,
					   TopoDS_Shape& returnTopo)
{
  Handle(TNaming_NamedShape) anAttTopo ;
  if( aLabel.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo)) {
    returnTopo = TNaming_Tool::GetShape(anAttTopo) ;
    return true ;
  }
  return false;
}


//=======================================================================
// function : IsDependentShape()
// purpose  : return true if the shape in the label is dependant (a sub shape)
//=======================================================================
Standard_Boolean GEOMDS_Commands::IsDependentShape(const TDF_Label& aLabel)
{
  Handle(TDF_Reference) anAttRef ;
  if( aLabel.FindAttribute(TDF_Reference::GetID(), anAttRef))
    return true ;
  return false;
}



//=======================================================================
// function : GetMainShapeLabel()
// purpose  : return true if an attribute Reference is found for 'aLabel'
//          : so 'returnMainLabel' is defined. 'aLabel' is supposed to be
//          : a dependent object, otherwise return false.
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetMainShapeLabel(const TDF_Label& aLabel,
						    TDF_Label& returnMainLabel)
{
  Handle(TDF_Reference) anAttRef ;
  if( aLabel.FindAttribute(TDF_Reference::GetID(), anAttRef)) {
    returnMainLabel = anAttRef->Get() ;
    return true ;
  }
  return false;
}


//=======================================================================
// function : ClearAllIOR()
// purpose  : Clear all IOR from aLabel usually the main label.
//          : Useful before reconstruction after a load of a document.
//          : IOR is the attribute often called 'name' or 'nameIOR'
//=======================================================================
Standard_Boolean GEOMDS_Commands::ClearAllIOR(const TDF_Label& aLabel)
{
  TDF_ChildIterator it;
  Handle(TDataStd_Name) anAttName ;
  bool notTested = false ;
  for( it.Initialize(aLabel, Standard_False); it.More(); it.Next() ) {
    TDF_Label L = it.Value() ;
    if( L.FindAttribute(TDataStd_Name::GetID(), anAttName) ) {
      notTested = L.ForgetAttribute(TDataStd_Name::GetID()) ;
      if(notTested)
	MESSAGE("in GEOMDS_Commands::ClearAllIOR : IOR CLEARED" )
    ClearAllIOR(L);
    }
  }
  return true ;
}


//=======================================================================
// function : HasIOR()
// purpose  : Return true is 'aLabel' has an attribute IOR (nameIOR)
//=======================================================================
Standard_Boolean GEOMDS_Commands::HasIOR(const TDF_Label& aLabel)
{
  Handle(TDataStd_Name) anAttName ;
  if( !aLabel.FindAttribute(TDataStd_Name::GetID(), anAttName) )
    return false ;
  return true ;
}

//=======================================================================
// function : ReturnNameIOR()
// purpose  : Return true is 'aLabel' has an attribute IOR (nameIOR)
//          : and define 'returnNameIOR'
//=======================================================================
Standard_Boolean GEOMDS_Commands::ReturnNameIOR(const TDF_Label& aLabel,
						TCollection_ExtendedString& returnNameIOR)
{
  Handle(TDataStd_Name) anAttName ;
  if( !aLabel.FindAttribute(TDataStd_Name::GetID(), anAttName) )
    return false ;
  else {
    returnNameIOR = anAttName->Get() ;
    return true ;
  }
}


//=======================================================================
// function : AddAssembly()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddAssembly(Kinematic_Assembly& KAss,
				       const TCollection_ExtendedString& Name)
{
  TDF_Label LabAssembly = myLab.NewChild();
  TDataStd_Name::Set(LabAssembly, Name);

  TDF_Label LabType = LabAssembly.NewChild();
  TDataStd_Integer::Set(LabType, 1);

  return LabAssembly;
}


//=======================================================================
// function : AddContact()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddContact(Kinematic_Contact& KContact,
				      const TDF_Label& mainRefLab,
				      const TCollection_ExtendedString& Name)
{
  TDF_Label LabContact = mainRefLab.NewChild();
  TDataStd_Name::Set(LabContact, Name);

  TDF_Label LabContactType = LabContact.NewChild();
  TDataStd_Integer::Set(LabContactType, 2);

  TDF_Label LabType = LabContact.NewChild();
  TDataStd_Integer::Set(LabType, KContact.GetType());
  
  int ret;
  TDF_Label NewLab1 = LabContact.NewChild();
  TNaming_Builder B1(NewLab1);
  B1.Select(KContact.GetShape1(), KContact.GetShape1());
  TDF_Label RefLab1 = TNaming_Tool::Label(myLab, KContact.GetShape1(), ret);
  TDF_Reference::Set(NewLab1, RefLab1);

  TDF_Label NewLab2 = LabContact.NewChild();
  TNaming_Builder B2(NewLab2);
  B2.Select(KContact.GetShape2(), KContact.GetShape2());
  TDF_Label RefLab2 = TNaming_Tool::Label(myLab, KContact.GetShape2(), ret);
  TDF_Reference::Set(NewLab2, RefLab2);

  TDF_Label LabPosition = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayP = TDataStd_RealArray::Set(LabPosition, 1, 12);
  RealArrayP->SetValue(1, KContact.GetPosition().GetOrigin().X());
  RealArrayP->SetValue(2, KContact.GetPosition().GetOrigin().Y());
  RealArrayP->SetValue(3, KContact.GetPosition().GetOrigin().Z());
  RealArrayP->SetValue(4, KContact.GetPosition().GetDirX().X());
  RealArrayP->SetValue(5, KContact.GetPosition().GetDirX().Y());
  RealArrayP->SetValue(6, KContact.GetPosition().GetDirX().Z());
  RealArrayP->SetValue(7, KContact.GetPosition().GetDirY().X());
  RealArrayP->SetValue(8, KContact.GetPosition().GetDirY().Y());
  RealArrayP->SetValue(9, KContact.GetPosition().GetDirY().Z());
  RealArrayP->SetValue(10, KContact.GetPosition().GetDirZ().X());
  RealArrayP->SetValue(11, KContact.GetPosition().GetDirZ().Y());
  RealArrayP->SetValue(12, KContact.GetPosition().GetDirZ().Z());

  TDF_Label LabAngularRange = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayR = TDataStd_RealArray::Set(LabAngularRange, 1, 6);
  RealArrayR->SetValue(1, KContact.GetAngularRange().GetMinValX());
  RealArrayR->SetValue(2, KContact.GetAngularRange().GetMaxValX());
  RealArrayR->SetValue(3, KContact.GetAngularRange().GetMinValY());
  RealArrayR->SetValue(4, KContact.GetAngularRange().GetMaxValY());
  RealArrayR->SetValue(5, KContact.GetAngularRange().GetMinValZ());
  RealArrayR->SetValue(6, KContact.GetAngularRange().GetMaxValZ());

  TDF_Label LabLinearRange = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayT = TDataStd_RealArray::Set(LabLinearRange, 1, 6);
  RealArrayT->SetValue(1, KContact.GetLinearRange().GetMinValX());
  RealArrayT->SetValue(2, KContact.GetLinearRange().GetMaxValX());
  RealArrayT->SetValue(3, KContact.GetLinearRange().GetMinValY());
  RealArrayT->SetValue(4, KContact.GetLinearRange().GetMaxValY());
  RealArrayT->SetValue(5, KContact.GetLinearRange().GetMinValZ());
  RealArrayT->SetValue(6, KContact.GetLinearRange().GetMaxValZ());

  TDF_Label LabStep = LabContact.NewChild();
  TDataStd_Real::Set(LabStep, KContact.GetStep());

  return LabContact;
}


//=======================================================================
// function : AddAnimation()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddAnimation(Kinematic_Animation& KAnimation,
					const TDF_Label& AssLab,
					const TCollection_ExtendedString& Name)
{
  cout<<"GEOMDS_Commands::AddAnimation"<<endl;
  TDF_Label LabAnimation = myLab.NewChild();
  TDataStd_Name::Set(LabAnimation, Name);

  TDF_Label LabType = LabAnimation.NewChild();
  TDataStd_Integer::Set(LabType, 3);

  TDF_Label LabAssembly = LabAnimation.NewChild();
  TDF_Reference::Set(LabAssembly, AssLab);

  int ret;
  TDF_Label LabFrame = LabAnimation.NewChild();
  TNaming_Builder B1(LabFrame);
  B1.Select(KAnimation.GetFrame(), KAnimation.GetFrame());
  TDF_Label RefLab = TNaming_Tool::Label(myLab, KAnimation.GetFrame(), ret);
  TDF_Reference::Set(LabFrame, RefLab);

  TDF_Label LabDuration = LabAnimation.NewChild();
  Standard_Real duration = double(KAnimation.GetDuration());
  TDataStd_Real::Set(LabDuration, duration);

  TDF_Label LabNbSeq = LabAnimation.NewChild();
  TDataStd_Integer::Set(LabNbSeq, KAnimation.GetNbSeq());

  Kinematic_Assembly* returnAss = new Kinematic_Assembly();
  if(GetAssembly(AssLab, *returnAss)) {
    list <Kinematic_Contact *> aContactList = returnAss->GetContactList();
    list <Kinematic_Contact *>::const_iterator it = aContactList.begin();
    int i = 0;
    while(it != aContactList.end()) {
      TDF_Label LabDisplacement = LabAnimation.NewChild();
      Handle(TDataStd_RealArray) RealArrayT = TDataStd_RealArray::Set(LabDisplacement, 1, 12);

      list <double> aList = KAnimation.GetDisplacement(i);
      list <double>::const_iterator it1 = aList.begin();
      int k = 1;
      while(it1 != aList.end()) {
	RealArrayT->SetValue(k, *it1);
	k++;
	it1++;
      }
      i++;
      it++;
    }
  }

  return LabAnimation;
}


//=======================================================================
// function : GetAssembly()
// purpose  : 
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetAssembly(const TDF_Label& aLabel,
					      Kinematic_Assembly& returnAss)
{
  Kinematic_Assembly* Ass = new Kinematic_Assembly();

  TDF_ChildIterator it;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TDataStd_Integer) anAttInteger;
    if(L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger)) {
      if(anAttInteger->Get() != 1) {
	returnAss = *Ass;
	return false;
      }
    }
    else {
      Kinematic_Contact* aContact = new Kinematic_Contact();
      if(GetContact(L, *aContact))
	Ass->AddContact(aContact);
    }
  }
  returnAss = *Ass;
  return true;
}


//=======================================================================
// function : GetContact()
// purpose  : 
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetContact(const TDF_Label& aLabel,
					     Kinematic_Contact& returnContact)
{
  Kinematic_Contact* Contact = new Kinematic_Contact();

  TDF_ChildIterator it;
  int i = 1;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TNaming_NamedShape) anAttTopo1;
    Handle(TNaming_NamedShape) anAttTopo2;
    Handle(TDataStd_Integer) anAttInteger1;
    Handle(TDataStd_Integer) anAttInteger2;
    Handle(TDataStd_Real) anAttReal;
    Handle(TDataStd_RealArray) anAttRealArrayP;
    Handle(TDataStd_RealArray) anAttRealArrayR;
    Handle(TDataStd_RealArray) anAttRealArrayT;

    if(i == 1 && L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger1)) {
      if(anAttInteger1->Get() != 2) {
	returnContact = *Contact;
	return false;
      }
    }
    if(i == 2 && L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger2)) {
      Contact->SetType(anAttInteger2->Get());
    }
    if(i == 3 && L.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo1)) {
      Contact->SetShape1(TNaming_Tool::GetShape(anAttTopo1));
    }
    if(i == 4 && L.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo2)) {
      Contact->SetShape2(TNaming_Tool::GetShape(anAttTopo2));
    }
    if(i == 5 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayP)) {
      gp_Pnt Center(anAttRealArrayP->Value(1), anAttRealArrayP->Value(2), anAttRealArrayP->Value(3));
      gp_Dir aDirX(anAttRealArrayP->Value(4), anAttRealArrayP->Value(5), anAttRealArrayP->Value(6));
      gp_Dir aDirY(anAttRealArrayP->Value(7), anAttRealArrayP->Value(8), anAttRealArrayP->Value(9));
      gp_Dir aDirZ(anAttRealArrayP->Value(10), anAttRealArrayP->Value(11), anAttRealArrayP->Value(12));
      Contact->GetPosition().SetOrigin(Center);
      Contact->GetPosition().SetDirX(aDirX);
      Contact->GetPosition().SetDirY(aDirY);
      Contact->GetPosition().SetDirZ(aDirZ);
    }
    if(i == 6 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayR)) {
      Contact->GetAngularRange().SetRangeX(anAttRealArrayR->Value(1), anAttRealArrayR->Value(2));
      Contact->GetAngularRange().SetRangeY(anAttRealArrayR->Value(3), anAttRealArrayR->Value(4));
      Contact->GetAngularRange().SetRangeZ(anAttRealArrayR->Value(5), anAttRealArrayR->Value(6));
    }
    if(i == 7 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayT)) {
      Contact->GetLinearRange().SetRangeX(anAttRealArrayT->Value(1), anAttRealArrayT->Value(2));
      Contact->GetLinearRange().SetRangeY(anAttRealArrayT->Value(3), anAttRealArrayT->Value(4));
      Contact->GetLinearRange().SetRangeZ(anAttRealArrayT->Value(5), anAttRealArrayT->Value(6));
    }
    if(i == 8 && L.FindAttribute(TDataStd_Real::GetID(), anAttReal)) {
      Contact->SetStep(anAttReal->Get());
    }
    i++;
  }
  returnContact = *Contact;
  return true;
}


//=======================================================================
// function : GetAnimation()
// purpose  : 
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetAnimation(const TDF_Label& aLabel,
					       Kinematic_Animation& returnAnim)
{
  cout<<"GEOMDS_Commands::GetAnimation"<<endl;
  Kinematic_Animation* Anim = new Kinematic_Animation();

  TDF_ChildIterator it;
  int i = 1;
  int k = 0;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TNaming_NamedShape) anAttTopo;
    Handle(TDataStd_Real) anAttReal;
    Handle(TDataStd_Integer) anAttInteger;
    Handle(TDataStd_Integer) anAttInteger1;
    Handle(TDataStd_RealArray) anAttRealArrayP;

    if(i == 1 && L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger)) {
      if(anAttInteger->Get() != 3) {
	returnAnim = *Anim;
	return false;
      }
    }
    if(i == 2) {
      Kinematic_Assembly* aAss =  new Kinematic_Assembly();
      Standard_Boolean test = GetAssembly(L, *aAss);
      Anim->SetAssembly(aAss);
    }
    if(i == 3 && L.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo)) {
      Anim->SetFrame(TNaming_Tool::GetShape(anAttTopo));
    }
    if(i == 4 && L.FindAttribute(TDataStd_Real::GetID(), anAttReal)) {
      Anim->SetDuration(anAttReal->Get());
    }
    if(i == 5 && L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger1)) {
      Anim->SetNbSeq(anAttInteger1->Get());
    }
    if(i > 5 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayP)) {
      list <double> aList;
      for(int j = 1; j <= 12; j++)
	aList.push_back(anAttRealArrayP->Value(j));
      Anim->SetDisplacement(k, aList);
      k++;
    }
    i++;
  }
  Anim->SetMap();
  returnAnim = *Anim;
  return true;
}


//=======================================================================
// function : SetPosition()
// purpose  : 
//=======================================================================
void GEOMDS_Commands::SetPosition(const TDF_Label& aLabel,
				  double P0x, double P0y, double P0z,
				  double VXx, double VXy, double VXz,
				  double VYx, double VYy, double VYz,
				  double VZx, double VZy, double VZz)
{
  TDF_ChildIterator it;
  int i = 1;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TDataStd_RealArray) anAttRealArrayP;

    if(i == 5 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayP)) {
      anAttRealArrayP->SetValue(1, P0x);
      anAttRealArrayP->SetValue(2, P0y);
      anAttRealArrayP->SetValue(3, P0z);
      anAttRealArrayP->SetValue(4, VXx);
      anAttRealArrayP->SetValue(5, VXy);
      anAttRealArrayP->SetValue(6, VXz);
      anAttRealArrayP->SetValue(7, VYx);
      anAttRealArrayP->SetValue(8, VYy);
      anAttRealArrayP->SetValue(9, VYz);
      anAttRealArrayP->SetValue(10, VZx);
      anAttRealArrayP->SetValue(11, VZy);
      anAttRealArrayP->SetValue(12, VZz);
      return;
    }
    i++;
  }
  return;
}


//=======================================================================
// function : SetAngularRange()
// purpose  : 
//=======================================================================
void GEOMDS_Commands::SetAngularRange(const TDF_Label& aLabel,
				      double MinValX, double MaxValX,
				      double MinValY, double MaxValY,
				      double MinValZ, double MaxValZ)
{
  TDF_ChildIterator it;
  int i = 1;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TDataStd_RealArray) anAttRealArrayR;

    if(i == 6 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayR)) {
      anAttRealArrayR->SetValue(1, MinValX);
      anAttRealArrayR->SetValue(2, MaxValX);
      anAttRealArrayR->SetValue(3, MinValY);
      anAttRealArrayR->SetValue(4, MaxValY);
      anAttRealArrayR->SetValue(5, MinValZ);
      anAttRealArrayR->SetValue(6, MaxValZ);
      return;
    }
    i++;
  }
  return;
}


//=======================================================================
// function : SetLinearRange()
// purpose  : 
//=======================================================================
void GEOMDS_Commands::SetLinearRange(const TDF_Label& aLabel, 
				     double MinValX, double MaxValX,
				     double MinValY, double MaxValY,
				     double MinValZ, double MaxValZ)
{
  TDF_ChildIterator it;
  int i = 1;
  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();
    Handle(TDataStd_RealArray) anAttRealArrayT;

    if(i == 7 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayT)) {
      anAttRealArrayT->SetValue(1, MinValX);
      anAttRealArrayT->SetValue(2, MaxValX);
      anAttRealArrayT->SetValue(3, MinValY);
      anAttRealArrayT->SetValue(4, MaxValY);
      anAttRealArrayT->SetValue(5, MinValZ);
      anAttRealArrayT->SetValue(6, MaxValZ);
      return;
    }
    i++;
  }
  return;
}


//=======================================================================
// function : SetDisplacement()
// purpose  : 
//=======================================================================
void GEOMDS_Commands::SetDisplacement(const TDF_Label& aLabel, int aContact,
				      double MinValRX, double MaxValRX,
				      double MinValRY, double MaxValRY,
				      double MinValRZ, double MaxValRZ,
				      double MinValTX, double MaxValTX,
				      double MinValTY, double MaxValTY,
				      double MinValTZ, double MaxValTZ)
{
  cout<<"GEOMDS_Commands::SetDisplacement"<<endl;
  int i = 1;
  TDF_ChildIterator it;
  Handle(TDataStd_RealArray) anAttRealArrayT;

  for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
    TDF_Label L = it.Value();

    if(i == (aContact + 6) && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayT)) {
      anAttRealArrayT->SetValue(1, MinValRX);
      anAttRealArrayT->SetValue(2, MaxValRX);
      anAttRealArrayT->SetValue(3, MinValRY);
      anAttRealArrayT->SetValue(4, MaxValRY);
      anAttRealArrayT->SetValue(5, MinValRZ);
      anAttRealArrayT->SetValue(6, MaxValRZ);
      anAttRealArrayT->SetValue(7, MinValTX);
      anAttRealArrayT->SetValue(8, MaxValTX);
      anAttRealArrayT->SetValue(9, MinValTY);
      anAttRealArrayT->SetValue(10, MaxValTY);
      anAttRealArrayT->SetValue(11, MinValTZ);
      anAttRealArrayT->SetValue(12, MaxValTZ);
    }
    i++;
  }
  return;
}
