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

#include <Utils_SALOME_Exception.hxx>

#include "XAO_XaoUtils.hxx"
#include "XAO_Geometry.hxx"
#include "XAO_BrepGeometry.hxx"

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
