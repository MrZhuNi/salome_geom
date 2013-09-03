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

#include <Utils_SALOME_Exception.hxx>

#include "XaoUtils.hxx"
#include "Geometry.hxx"
#include "BrepGeometry.hxx"

using namespace XAO;

Geometry::Geometry(const std::string& name)
    : m_name(name)
{
}

Geometry* Geometry::createGeometry(const XAO::Format& format)
{
    return createGeometry(format, "");
}

Geometry* Geometry::createGeometry(const XAO::Format& format, const std::string& name)
{
    if (format == XAO::BREP)
        return new BrepGeometry(name);

    throw SALOME_Exception(MsgBuilder() << "Geometry format not supported: " << format);
}

Geometry::~Geometry()
{
}

void Geometry::setShape(const TopoDS_Shape& shape)
{
    m_shape = shape;

    // intialization of Ids
    initListIds(TopAbs_VERTEX);
    initListIds(TopAbs_EDGE);
    initListIds(TopAbs_FACE);
    initListIds(TopAbs_SOLID);
}

void Geometry::setBREP(const char* brep)
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
    std::string data = streamShape.str();
    char* res = new char[data.size()];
    strcpy(res, data.c_str());
    return res;
}

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
        return;

    TopTools_IndexedMapOfShape indices;
    TopExp::MapShapes(m_shape, indices);

    std::list<int> indexList;
    TopTools_ListIteratorOfListOfShape itSub(listShape);
    for (int index = 1; itSub.More(); itSub.Next(), ++index)
    {
        TopoDS_Shape value = itSub.Value();
        indexList.push_back(indices.FindIndex(value));
    }

    std::list<int>::iterator it = indexList.begin();
    switch (shapeType)
    {
        case TopAbs_VERTEX:
        {
            m_vertices.setSize(indexList.size());
            for (int i = 0; it != indexList.end(); it++, i++)
                m_vertices.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_EDGE:
        {
            m_edges.setSize(indexList.size());
            for (int i = 0; it != indexList.end(); it++, i++)
                m_edges.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_FACE:
        {
            m_faces.setSize(indexList.size());
            for (int i = 0; it != indexList.end(); it++, i++)
                m_faces.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
        case TopAbs_SOLID:
        {
            m_solids.setSize(indexList.size());
            for (int i = 0; it != indexList.end(); it++, i++)
                m_solids.setReference(i, XaoUtils::intToString((*it)));
            break;
        }
    }
}

const int Geometry::countElements(const XAO::Dimension& dim)
{
    if (dim == XAO::VERTEX)
        return countVertices();
    if (dim == XAO::EDGE)
        return countEdges();
    if (dim == XAO::FACE)
        return countFaces();
    if (dim == XAO::SOLID)
        return countSolids();

    throw SALOME_Exception(MsgBuilder() << "Unknown dimension:" << dim);
}

const std::string Geometry::getElementReference(const XAO::Dimension& dim, const int& index)
{
    if (dim == XAO::VERTEX)
        return getVertexReference(index);
    if (dim == XAO::EDGE)
        return getEdgeReference(index);
    if (dim == XAO::FACE)
        return getFaceReference(index);
    if (dim == XAO::SOLID)
        return getSolidReference(index);

    throw SALOME_Exception(MsgBuilder() << "Unknown dimension:" << dim);
}

const int Geometry::getElementIndexByReference(const XAO::Dimension& dim, const std::string& reference)
{
    if (dim == XAO::VERTEX)
        return getVertexIndexByReference(reference);
    if (dim == XAO::EDGE)
        return getEdgeIndexByReference(reference);
    if (dim == XAO::FACE)
        return getFaceIndexByReference(reference);
    if (dim == XAO::SOLID)
        return getSolidIndexByReference(reference);

    throw SALOME_Exception(MsgBuilder() << "Unknown dimension:" << dim);
}

GeometricElementList::iterator Geometry::begin(const XAO::Dimension& dim)
{
    if (dim == XAO::VERTEX)
        return m_vertices.begin();
    if (dim == XAO::EDGE)
        return m_edges.begin();
    if (dim == XAO::FACE)
        return m_faces.begin();
    if (dim == XAO::SOLID)
        return m_solids.begin();

    throw SALOME_Exception(MsgBuilder() << "Unknown dimension:" << dim);
}

GeometricElementList::iterator Geometry::end(const XAO::Dimension& dim)
{
    if (dim == XAO::VERTEX)
        return m_vertices.end();
    if (dim == XAO::EDGE)
        return m_edges.end();
    if (dim == XAO::FACE)
        return m_faces.end();
    if (dim == XAO::SOLID)
        return m_solids.end();

    throw SALOME_Exception(MsgBuilder() << "Unknown dimension:" << dim);
}
