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
    }
    if(notTested)
      MESSAGE("in GEOMDS_Commands::ClearAllIOR : IOR CLEARED" )
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

  TDF_Label LabType = LabContact.NewChild();
  TDataStd_Integer::Set(LabType, KContact.Type());
  
  TDF_Label NewLab1 = LabContact.NewChild();
  TNaming_Builder B1(NewLab1);
  B1.Select(KContact.Shape1(), KContact.Shape1());
//   TDF_Label RefLab1 = TNaming_Tool::Label(myLab, KContact.Shape1());
//   TDF_Reference::Set(NewLab1, RefLab1);

  TDF_Label NewLab2 = LabContact.NewChild();
  TNaming_Builder B2(NewLab2);
  B2.Select(KContact.Shape2(), KContact.Shape2());
//   TDF_Label RefLab2 = TNaming_Tool::Label(myLab, KContact.Shape2());
//   TDF_Reference::Set(NewLab2, RefLab2);

  TDF_Label LabPosition = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayP = TDataStd_RealArray::Set(LabPosition, 1, 12);
  RealArrayP->SetValue(1, KContact.Position().Origin().X());
  RealArrayP->SetValue(2, KContact.Position().Origin().Y());
  RealArrayP->SetValue(3, KContact.Position().Origin().Z());
  RealArrayP->SetValue(4, KContact.Position().DirX().X());
  RealArrayP->SetValue(5, KContact.Position().DirX().Y());
  RealArrayP->SetValue(6, KContact.Position().DirX().Z());
  RealArrayP->SetValue(7, KContact.Position().DirY().X());
  RealArrayP->SetValue(8, KContact.Position().DirY().Y());
  RealArrayP->SetValue(9, KContact.Position().DirY().Z());
  RealArrayP->SetValue(10, KContact.Position().DirZ().X());
  RealArrayP->SetValue(11, KContact.Position().DirZ().Y());
  RealArrayP->SetValue(12, KContact.Position().DirZ().Z());

  TDF_Label LabRotation1 = LabContact.NewChild();
  Handle(TDataStd_IntegerArray) IntegerArrayR = TDataStd_IntegerArray::Set(LabRotation1, 1, 3);
  IntegerArrayR->SetValue(1, KContact.Rotation().Rot1());
  IntegerArrayR->SetValue(2, KContact.Rotation().Rot2());
  IntegerArrayR->SetValue(3, KContact.Rotation().Rot3());

  TDF_Label LabRotation2 = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayR = TDataStd_RealArray::Set(LabRotation2, 1, 3);
  RealArrayR->SetValue(1, KContact.Rotation().ValX());
  RealArrayR->SetValue(2, KContact.Rotation().ValY());
  RealArrayR->SetValue(3, KContact.Rotation().ValZ());

  TDF_Label LabTranslation = LabContact.NewChild();
  Handle(TDataStd_RealArray) RealArrayT = TDataStd_RealArray::Set(LabTranslation, 1, 3);
  RealArrayT->SetValue(1, KContact.Translation().ValX());
  RealArrayT->SetValue(2, KContact.Translation().ValY());
  RealArrayT->SetValue(3, KContact.Translation().ValZ());

  TDF_Label LabStep = LabContact.NewChild();
  TDataStd_Real::Set(LabStep, KContact.Step());

  return LabContact;
}


//=======================================================================
// function : AddAnimation()
// purpose  :
//=======================================================================
TDF_Label GEOMDS_Commands::AddAnimation(Kinematic_Animation& KAnimation,
					const TCollection_ExtendedString& Name)
{
  TDF_Label LabAnimation = myLab.NewChild();
  TDataStd_Name::Set(LabAnimation, Name);

  Standard_Real duration = double(KAnimation.Duration());

  TDF_Label LabDuration = LabAnimation.NewChild();
  TDataStd_Real::Set(LabDuration, duration);

  TDF_Label LabNbSeq = LabAnimation.NewChild();
  TDataStd_Integer::Set(LabNbSeq, KAnimation.NbSeq());

  TDF_Label LabIsInLoop = LabAnimation.NewChild();
  TDataStd_Integer::Set(LabIsInLoop, KAnimation.IsInLoop());

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
  Handle(TDataStd_Name) anAttName;
  if(!aLabel.FindAttribute(TDataStd_Name::GetID(), anAttName))
    return false;
  else {
    TDF_ChildIterator it;
    for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
      TDF_Label L = it.Value();
      Kinematic_Contact* aContact = new Kinematic_Contact();
      Standard_Boolean test = GetContact(L, *aContact);
      Ass->AddContact(aContact);
    }
    returnAss = *Ass;
    return true;
  }
}


//=======================================================================
// function : GetContact()
// purpose  : 
//=======================================================================
Standard_Boolean GEOMDS_Commands::GetContact(const TDF_Label& aLabel,
					     Kinematic_Contact& returnContact)
{
  Kinematic_Contact* Contact = new Kinematic_Contact();
  Handle(TDataStd_Name) anAttName;
  if(!aLabel.FindAttribute(TDataStd_Name::GetID(), anAttName))
    return false;
  else {
    TDF_ChildIterator it;
    int i = 1;
    for(it.Initialize(aLabel, Standard_False); it.More(); it.Next()) {
      TDF_Label L = it.Value();
      Handle(TNaming_NamedShape) anAttTopo1;
      Handle(TNaming_NamedShape) anAttTopo2;
      Handle(TDataStd_Integer) anAttInteger;
      Handle(TDataStd_Real) anAttReal;
      Handle(TDataStd_IntegerArray) anAttIntegerArrayR;
      Handle(TDataStd_RealArray) anAttRealArrayP;
      Handle(TDataStd_RealArray) anAttRealArrayR;
      Handle(TDataStd_RealArray) anAttRealArrayT;

      if(i == 1 && L.FindAttribute(TDataStd_Integer::GetID(), anAttInteger)) {
	Contact->Type(anAttInteger->Get());
      }
      if(i == 2 && L.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo1)) {
	Contact->Shape1(TNaming_Tool::GetShape(anAttTopo1));
      }
      if(i == 3 && L.FindAttribute(TNaming_NamedShape::GetID(), anAttTopo2)) {
	Contact->Shape2(TNaming_Tool::GetShape(anAttTopo2));
      }
      if(i == 4 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayP)) {
	gp_Pnt Center(anAttRealArrayP->Value(1), anAttRealArrayP->Value(2), anAttRealArrayP->Value(3));
	gp_Dir aDirX(anAttRealArrayP->Value(4), anAttRealArrayP->Value(5), anAttRealArrayP->Value(6));
	gp_Dir aDirY(anAttRealArrayP->Value(7), anAttRealArrayP->Value(8), anAttRealArrayP->Value(9));
	gp_Dir aDirZ(anAttRealArrayP->Value(10), anAttRealArrayP->Value(11), anAttRealArrayP->Value(12));
	Contact->Position().Origin(Center);
	Contact->Position().DirX(aDirX);
	Contact->Position().DirY(aDirY);
	Contact->Position().DirZ(aDirZ);
      }
      if(i == 5 && L.FindAttribute(TDataStd_IntegerArray::GetID(), anAttIntegerArrayR)) {
	Contact->Rotation().Rot1(anAttIntegerArrayR->Value(1));
	Contact->Rotation().Rot2(anAttIntegerArrayR->Value(2));
	Contact->Rotation().Rot3(anAttIntegerArrayR->Value(3));
      }
      if(i == 6 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayR)) {
	Contact->Rotation().ValX(anAttRealArrayR->Value(1));
	Contact->Rotation().ValY(anAttRealArrayR->Value(2));
	Contact->Rotation().ValZ(anAttRealArrayR->Value(3));
      }
      if(i == 7 && L.FindAttribute(TDataStd_RealArray::GetID(), anAttRealArrayT)) {
	Contact->Translation().ValX(anAttRealArrayT->Value(1));
	Contact->Translation().ValY(anAttRealArrayT->Value(2));
	Contact->Translation().ValZ(anAttRealArrayT->Value(3));
      }
      if(i == 8 && L.FindAttribute(TDataStd_Real::GetID(), anAttReal)) {
	Contact->Step(anAttReal->Get());
      }
      i++;
    }
    returnContact = *Contact;
    return true;
  }
}
