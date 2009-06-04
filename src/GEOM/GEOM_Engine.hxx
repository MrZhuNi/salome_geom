//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#ifndef _GEOM_Engine_HXX_
#define _GEOM_Engine_HXX_

#include "GEOM_Application.hxx"
#include "GEOM_Object.hxx"
#include "GEOM_DataMapOfAsciiStringTransient.hxx"

#include <Interface_DataMapOfIntegerTransient.hxx> 
#include <Resource_DataMapOfAsciiStringAsciiString.hxx>
#include <TDocStd_Document.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TDF_Label.hxx>

#include <map>
#include <list>
#include <vector>

struct TVariable{
  TCollection_AsciiString myVariable;
  bool isVariable;

  TVariable(const TCollection_AsciiString& theVariable, bool theFlag = true):
    myVariable(theVariable),
    isVariable(theFlag){}
};

typedef std::vector<TVariable> TState;
typedef std::vector<TState>    TAllStates;

class ObjectStates
{
public:
  Standard_EXPORT ObjectStates();
  ~ObjectStates();

  TAllStates GetAllStates() const { return _states; }

  TState GetCurrectState() const;
  Standard_EXPORT void AddState(const TState &theState);
  void IncrementState();

private:
  TAllStates              _states;
  int                     _dumpstate;
};

typedef std::map<TCollection_AsciiString, ObjectStates* > TVariablesList;

typedef std::map<int, std::list<TDF_Label> > TFreeLabelsList;

class GEOM_Engine
{
 public:
  Standard_EXPORT GEOM_Engine();
  Standard_EXPORT virtual ~GEOM_Engine();

  //Retuns the engine    
  Standard_EXPORT static GEOM_Engine* GetEngine();   

  //Returns the OCAF document by its ID, if document doesn't exists it will be created
  Standard_EXPORT Handle(TDocStd_Document) GetDocument(int theDocID);

  //Returns the ID of the given OCAF document
  Standard_EXPORT int GetDocID(Handle(TDocStd_Document) theDocument);
  
  //Returns the OCAF appliaction
  Standard_EXPORT Handle(TDocStd_Application) GetApplication() { return _OCAFApp; }

  //Returns a pointer to GEOM_Object defined by a document and the entry
  Standard_EXPORT Handle(GEOM_Object) GetObject(int theDocID, char* theEntry);
  
  //Adds a new object of the type theType in the OCAF document
  Standard_EXPORT Handle(GEOM_Object) AddObject(int theDocID, int theType);

  //Removes the object from the OCAF document
  Standard_EXPORT bool RemoveObject(Handle(GEOM_Object) theObject);  

  //Saves the OCAF document with ID = theDocID with file with name theFileName
  Standard_EXPORT bool Save(int theDocID, char* theFileName);
  
  //Loads the OCAF document into the application and assigns to it an ID = theDocID
  Standard_EXPORT bool Load(int theDocID, char* theFileName);

  //Closes the document with ID =  theDocID
  Standard_EXPORT void Close(int theDocID);

  //Sets the number of Undos (default value = 10)
  Standard_EXPORT void SetUndoLimit(int theLimit) { _UndoLimit = theLimit; }

  //Applies an Undo to document with ID = theDocID
  Standard_EXPORT void Undo(int theDocID);

  //Applies an Redo to document with ID = theDocID
  Standard_EXPORT void Redo(int theDocID);

  //Adds a new sub shape object of the MainShape object
  Standard_EXPORT Handle(GEOM_Object) AddSubShape(Handle(GEOM_Object) theMainShape, 
				  Handle(TColStd_HArray1OfInteger) theIndices,
				  bool isStandaloneOperation = false);

  Standard_EXPORT TCollection_AsciiString DumpPython(int theDocID, 
				     Resource_DataMapOfAsciiStringAsciiString& theObjectNames,
                                     TVariablesList theVariables,
				     bool isPublished, 
				     bool& aValidScript);

  Standard_EXPORT const char* GetDumpName (const char* theStudyEntry) const;

  Standard_EXPORT Handle(TColStd_HSequenceOfAsciiString) GetAllDumpNames() const;

 protected:
  Standard_EXPORT static void SetEngine(GEOM_Engine* theEngine);       
  
 private:

  Handle(GEOM_Application)  _OCAFApp;
  Interface_DataMapOfIntegerTransient _mapIDDocument;
  int _UndoLimit;
  GEOM_DataMapOfAsciiStringTransient _objects;

  Resource_DataMapOfAsciiStringAsciiString _studyEntry2NameMap;

  TFreeLabelsList _freeLabels;
};

#endif
