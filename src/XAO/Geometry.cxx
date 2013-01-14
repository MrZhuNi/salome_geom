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

Geometry *Geometry::New()
{
  return new Geometry;
}

void Geometry::initShapeFromBREP(const char * brep )
{
  istringstream streamBrep(brep);
  BRep_Builder aBuilder;
  BRepTools::Read(_myShape, streamBrep, aBuilder);
}

const char * Geometry::convertShapeToBREP()
{
  ostringstream streamShape;
  BRepTools::Write(_myShape, streamShape);
  return streamShape.str().c_str();
}

void Geometry::setNameVerticesAt(int i, const char *myName)
{
  if (_myNbVertices == 0 || i > _myNbVertices)
    Standard_TypeMismatch::Raise("Problem with number of vertices");

  if (_myNamesVertices == NULL)
    _myNamesVertices = new std::string[_myNbVertices];
    
  _myNamesVertices[i] = myName;
}

void Geometry::setNamesVertices(const char **myNames)
{
  if (_myNamesVertices == NULL)
    _myNamesVertices = new std::string[_myNbVertices];
    
  for (int i = 0;i < _myNbVertices; i++)
    _myNamesVertices[i] = * myNames[i];
}

void Geometry::initIdsVertices()
{
  //TopTools_ListOfShape listShape;
}

