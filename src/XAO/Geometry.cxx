// Copyright (C) 2013  CEA/DEN, EDF R&D
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

#include "XaoUtils.hxx"
#include "Geometry.hxx"
#include <Standard_TypeMismatch.hxx>

#include <BRepTools.hxx>
#include <BRep_Builder.hxx>
#include <TopAbs.hxx>

//#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iosfwd>
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

using namespace XAO;

Geometry::Geometry()
{
    m_format = "BREP";
}

Geometry::~Geometry()
{
}

void Geometry::setShape(TopoDS_Shape shape)
{
    m_shape = shape;

    // intialization of Ids
    initListIds(TopAbs_VERTEX);
    initListIds(TopAbs_EDGE);
    initListIds(TopAbs_FACE);
    initListIds(TopAbs_SOLID);
}

void Geometry::setShape(const char* brep)
{
    std::istringstream streamBrep(brep);
    BRep_Builder builder;
    BRepTools::Read(m_shape, streamBrep, builder);

    // intialization of Ids
    initListIds(TopAbs_VERTEX);
    initListIds(TopAbs_EDGE);
    initListIds(TopAbs_FACE);
    initListIds(TopAbs_SOLID);
}

const char* Geometry::getBREP()
{
    std::ostringstream streamShape;
    BRepTools::Write(m_shape, streamShape);
    return streamShape.str().c_str();
}

/*
void Geometry::changeVertexName(int id, const char* name)
{
    if (m_nbVertices == 0)
        Standard_TypeMismatch::Raise("Problem with number of vertices");

    if (m_verticesNames == NULL)
        m_verticesNames = new std::string[m_nbVertices];

    int index = findVertex(id);

    if (index == -1)
        Standard_TypeMismatch::Raise("Problem with the id of the vertex");

    m_verticesNames[index] = name;
}

const char* Geometry::findVertexName(int id)
{
    if (m_nbVertices == 0)
    {
        Standard_TypeMismatch::Raise("Problem with number of vertices");
        return "";
    }

    int index = findVertex(id);

    if (index == -1)
    {
        Standard_TypeMismatch::Raise("Problem with the id of the vertex");
        return "";
    }

    return m_verticesNames[index].c_str();
}*/
/*
int Geometry::findVertex(int id)
{
    int index = -1;
    for (int i = 0, n = m_nbVertices; i < n; i++)
    {
        if (m_verticesIds[i] == id)
            return i;
    }
    return index;
}*/

void Geometry::initListIds(const Standard_Integer shapeType)
{
    TopTools_MapOfShape mapShape;
    TopTools_ListOfShape listShape;

    TopExp_Explorer exp(m_shape, TopAbs_ShapeEnum(shapeType));
    for (; exp.More(); exp.Next())
    {
        if (mapShape.Add(exp.Current()))
            listShape.Append(exp.Current());
    }

    if (listShape.IsEmpty())
    {
        return;
    }

    TopTools_IndexedMapOfShape anIndices;
    TopExp::MapShapes(m_shape, anIndices);
    //Handle (TColStd_HArray1OfInteger) anArray;

    std::list<int> aList;
    TopTools_ListIteratorOfListOfShape itSub(listShape);
    for (int index = 1; itSub.More(); itSub.Next(), ++index)
    {
        TopoDS_Shape value = itSub.Value();
        aList.push_back(anIndices.FindIndex(value));
    }

    std::list<int>::iterator it = aList.begin();
    switch (shapeType)
    {
        case TopAbs_VERTEX: /* Fill vertices ids */
        {
            m_vertices.setSize(aList.size());
            for (int i = 0; it != aList.end(); it++, i++)
                m_vertices.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_EDGE: /* Fill edges ids */
        {
            m_edges.setSize(aList.size());
            for (int i = 0; it != aList.end(); it++, i++)
                m_edges.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_FACE: /* Fill faces ids */
        {
            m_faces.setSize(aList.size());
            for (int i = 0; it != aList.end(); it++, i++)
                m_faces.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_SOLID: /* Fill solids ids */
        {
            m_solids.setSize(aList.size());
            for (int i = 0; it != aList.end(); it++, i++)
                m_solids.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
    }

}

