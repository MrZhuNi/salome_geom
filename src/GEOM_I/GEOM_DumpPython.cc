using namespace std;

#include "GEOM_Gen_i.hh"
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <Resource_DataMapOfAsciiStringAsciiString.hxx>

Engines::TMPFile* GEOM_Gen_i::DumpPython(CORBA::Object_ptr theStudy, 
					 CORBA::Boolean isPublished, 
					 CORBA::Boolean& isValidScript)
{
  SALOMEDS::Study_var aStudy = SALOMEDS::Study::_narrow(theStudy);
  if(CORBA::is_nil(aStudy))
    return new Engines::TMPFile(0);   

  SALOMEDS::SObject_var aSO = aStudy->FindComponent(ComponentDataType());
  if(CORBA::is_nil(aSO))
    return new Engines::TMPFile(0);  

  Resource_DataMapOfAsciiStringAsciiString aMap;
  TCollection_AsciiString s("qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM0987654321_");

  SALOMEDS::ChildIterator_var Itr = aStudy->NewChildIterator(aSO);
  for(Itr->InitEx(true); Itr->More(); Itr->Next()) {
    SALOMEDS::SObject_var aValue = Itr->Value();
    char* IOR = aValue->GetIOR();
    if(strlen(IOR) > 0) {
      CORBA::Object_var obj = _orb->string_to_object(IOR);
      GEOM::GEOM_Object_var GO = GEOM::GEOM_Object::_narrow(obj);
      if(!CORBA::is_nil(GO)) {
	TCollection_AsciiString aName(aValue->GetName());
        int p, p2=1, e = aName.Length();
	while ((p = aName.FirstLocationNotInSet(s, p2, e))) {
	   aName.SetValue(p, '_');
	   p2=p;
	}  
	aMap.Bind(TCollection_AsciiString(GO->GetEntry()), aName);
      }
    }
  }

  bool aValidScript;
  TCollection_AsciiString aScript = _impl->DumpPython(aStudy->StudyId(), aMap, isPublished, aValidScript);

  int aLen = aScript.Length(); 
  unsigned char* aBuffer = new unsigned char[aLen+1];
  strcpy((char*)aBuffer, aScript.ToCString());

  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;
  Engines::TMPFile_var aStreamFile = new Engines::TMPFile(aLen+1, aLen+1, anOctetBuf, 1); 
  isValidScript = aValidScript;

  return aStreamFile._retn(); 
}
