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

#ifndef __XAO_GEOMETRY_HXX__
#define __XAO_GEOMETRY_HXX__

#include <string>

#include <SALOMEconfig.h>
#include <TopoDS_Shape.hxx>

namespace XAO
{ 
  class Geometry
  {
  public:
    Geometry();
    ~Geometry();

    void setName(const char *name) { _myName=name; }
    const char *getName() const { return _myName.c_str(); }

    void setFormat(const char *format) { _myFormat=format; }
    const char *getFormat() const { return _myFormat.c_str(); }
    
    void setShape(TopoDS_Shape Shape);
    TopoDS_Shape getShape() { return _myShape; }
    
    void setShape(const char * brep );
    const char * getBREP();
    
    int countVertices() { return _myNbVertices; }
    int findVertex(int id);
    void setVertexName(int index, const char *myName);
    const char *  getVertexName(int index) { return _myNamesVertices[index].c_str(); } 
    void changeVertexName(int id, const char *myName);
    const char *  findVertexName(int id);

    int countEdges() { return _myNbEdges; }
    int countFaces() { return _myNbFaces; }
    int countSolids() { return _myNbSolids; }
    

  private:
    void initListIds(const Standard_Integer theShapeType);

  private:
    TopoDS_Shape        _myShape;
    std::string         _myName;
    std::string         _myFormat;
    int                 _myNbVertices;
    std::string        *_myNamesVertices;
    int                *_myIdsVertices;
    int                 _myNbEdges;
    std::string        *_myNamesEdges;
    int                *_myIdsEdges;
    int                 _myNbFaces;
    std::string        *_myNamesFaces;
    int                *_myIdsFaces;
    int                 _myNbSolids;
    std::string        *_myNamesSolids;
    int                *_myIdsSolids;
  };
}

#endif
