// Copyright (C) 2007-2012  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Nathalie Gore (OpenCascade)

#include "Geometry.hxx"
#include <Standard_TypeMismatch.hxx>

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <TopAbs.hxx>

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <list>

#include <TopTools_MapOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
 
using namespace std;

using namespace XAO;

Geometry::Geometry()
{
  _myFormat = "BREP";
   
  _myNbVertices = 0;
  _myNamesVertices = NULL;
  _myIdsVertices = NULL;
  
  _myNbEdges = 0;
  _myNamesEdges = NULL;
  _myIdsEdges = NULL;
  
  _myNbFaces = 0;
  _myNamesFaces = NULL;
  _myIdsFaces = NULL;
  
  _myNbSolids = 0;
  _myNamesSolids = NULL;
  _myIdsSolids = NULL;
}

Geometry::~Geometry()
{
  if (_myNamesVertices != NULL)
    delete _myNamesVertices;
  if (_myIdsVertices != NULL)
    delete _myIdsVertices;
    
  if (_myNamesEdges != NULL)
    delete _myNamesEdges;
  if (_myIdsEdges != NULL)
    delete _myIdsEdges;
    
  if (_myNamesFaces != NULL)
    delete _myNamesFaces;
  if (_myIdsFaces != NULL)
    delete _myIdsFaces;
    
  if (_myNamesSolids != NULL)
    delete _myNamesSolids;
  if (_myIdsSolids != NULL)
    delete _myIdsSolids;
}

void Geometry::setShape(TopoDS_Shape Shape)
{
  _myShape = Shape;
  
  // intialization of Ids
  initListIds(TopAbs_VERTEX);
  initListIds(TopAbs_EDGE);
  initListIds(TopAbs_FACE);
  initListIds(TopAbs_SOLID);
}

void Geometry::setShape(const char * brep )
{
  istringstream streamBrep(brep);
  BRep_Builder aBuilder;
  BRepTools::Read(_myShape, streamBrep, aBuilder);
  
  // intialization of Ids
  initListIds(TopAbs_VERTEX);
  initListIds(TopAbs_EDGE);
  initListIds(TopAbs_FACE);
  initListIds(TopAbs_SOLID);
}

const char * Geometry::getBREP()
{
  ostringstream streamShape;
  BRepTools::Write(_myShape, streamShape);
  return streamShape.str().c_str();
}

void Geometry::setVertexName(int index, const char *myName)
{
  if (_myNbVertices == 0 || index > _myNbVertices)
    Standard_TypeMismatch::Raise("Problem with number of vertices");

  if (_myNamesVertices == NULL)
    _myNamesVertices = new std::string[_myNbVertices];
    
  _myNamesVertices[index] = myName;
}

void Geometry::changeVertexName(int id, const char *myName)
{
  if (_myNbVertices == 0)
    Standard_TypeMismatch::Raise("Problem with number of vertices");

  if (_myNamesVertices == NULL)
    _myNamesVertices = new std::string[_myNbVertices];
    
  int index = findVertex(id);
  
  if (index == -1)
    Standard_TypeMismatch::Raise("Problem with the id of the vertex");
    
  _myNamesVertices[index] = myName;
}

const char * Geometry::findVertexName(int id)
{
  if (_myNbVertices == 0) {
    Standard_TypeMismatch::Raise("Problem with number of vertices");
    return "";
  }

  int index = findVertex(id);
  
  if (index == -1) {
    Standard_TypeMismatch::Raise("Problem with the id of the vertex");
    return "";
  }
    
  return _myNamesVertices[index].c_str();
}

int Geometry::findVertex(int id)
{
  int index = -1;
  for (int i = 0, n = _myNbVertices; i < n; i++)
  {
    if (_myIdsVertices[i] == id)
      return i;
  }
  return index;
}

void Geometry::initListIds(const Standard_Integer theShapeType)
{
  std::list<int> aList;
  
  TopTools_MapOfShape mapShape;
  TopTools_ListOfShape listShape;
  
  TopExp_Explorer exp (_myShape, TopAbs_ShapeEnum(theShapeType));
  for (; exp.More(); exp.Next())
    if (mapShape.Add(exp.Current()))
      listShape.Append(exp.Current());

  if (listShape.IsEmpty()) {
    return ;
  }

  TopTools_IndexedMapOfShape anIndices;
  TopExp::MapShapes(_myShape, anIndices);
  Handle(TColStd_HArray1OfInteger) anArray;

  TopTools_ListIteratorOfListOfShape itSub (listShape);
  for (int index = 1; itSub.More(); itSub.Next(), ++index) {
    TopoDS_Shape aValue = itSub.Value();
    aList.push_back(anIndices.FindIndex(aValue));
  }
  
  std::list<int>::iterator it = aList.begin();
  switch ( theShapeType ) {
    case TopAbs_VERTEX: /* Fill vertices ids */ 
    {
      _myNbVertices = aList.size();
      if (_myIdsVertices != NULL)
        delete _myIdsVertices;
      _myIdsVertices = new int[_myNbVertices];
      for (int i = 0; it != aList.end(); it++, i++)
        _myIdsVertices[i] = (*it);
      break;
    }
    case TopAbs_EDGE: /* Fill edges ids */
    {
      _myNbEdges = aList.size();
      if (_myIdsEdges != NULL)
        delete _myIdsEdges;
      _myIdsEdges = new int[_myNbEdges];
      for (int i = 0; it != aList.end(); it++, i++)
        _myIdsEdges[i] = (*it);
      break;
    }
    case TopAbs_FACE: /* Fill faces ids */
    {
      _myNbFaces = aList.size();
      if (_myIdsFaces != NULL)
        delete _myIdsFaces;
      _myIdsFaces = new int[_myNbFaces];
      for (int i = 0; it != aList.end(); it++, i++)
        _myIdsFaces[i] = (*it);
      break;
    }
    case TopAbs_SOLID: /* Fill solids ids */
    {
      _myNbSolids = aList.size();
      if (_myIdsSolids != NULL)
        delete _myIdsSolids;
      _myIdsSolids = new int[_myNbSolids];
      for (int i = 0; it != aList.end(); it++, i++)
        _myIdsSolids[i] = (*it);
      break;
    }
  }
  
}

