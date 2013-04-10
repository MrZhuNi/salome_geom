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

# include "GeometricElement.hxx"

namespace XAO
{
    class Geometry
    {
    public:
        Geometry();
        ~Geometry();

        const char* getName()
        {
            return m_name.c_str();
        }
        void setName(const char* name)
        {
            m_name = name;
        }

        const char* getFormat()
        {
            return m_format.c_str();
        }
        void setFormat(const char* format)
        {
            m_format = format;
        }

        TopoDS_Shape getShape()
        {
            return m_shape;
        }
        void setShape(TopoDS_Shape shape);

        const char* getBREP();
        void setShape(const char* brep);

        int countVertices() { return m_vertices.getSize(); }
        int countEdges() { return m_edges.getSize(); }
        int countFaces() { return m_faces.getSize(); }
        int countSolids() { return m_solids.getSize(); }

        void setCountVertices(const int nb) { m_vertices.setSize(nb); }
        void setCountEdges(const int nb) { m_edges.setSize(nb); }
        void setCountFaces(const int nb) { m_faces.setSize(nb); }
        void setCountSolids(const int nb) { m_solids.setSize(nb); }

        void setVertex(const int index, const char* name, const char* reference) { m_vertices.setElement(index, name, reference); }
        void setEdge(const int index, const char* name, const char* reference) { m_edges.setElement(index, name, reference); }
        void setFace(const int index, const char* name, const char* reference) { m_faces.setElement(index, name, reference); }
        void setSolid(const int index, const char* name, const char* reference) { m_solids.setElement(index, name, reference); }

        const char* getVertexName(const int index) { return m_vertices.getName(index); }
        const char* getEdgeName(const int index) { return m_edges.getName(index); }
        const char* getFaceName(const int index) { return m_faces.getName(index); }
        const char* getSolidName(const int index) { return m_solids.getName(index); }

        void setVertexName(const int index, const char* name) { m_vertices.setName(index, name); }
        void setEdgeName(const int index, const char* name) { m_edges.setName(index, name); }
        void setFaceName(const int index, const char* name) { m_faces.setName(index, name); }
        void setSolidName(const int index, const char* name) { m_solids.setName(index, name); }

        const char* getVertexReference(const int index) { return m_vertices.getReference(index); }
        const char* getEdgeReference(const int index) { return m_edges.getReference(index); }
        const char* getFaceReference(const int index) { return m_faces.getReference(index); }
        const char* getSolidReference(const int index) { return m_solids.getReference(index); }

        void setVertexReference(const int index, const char* reference) { m_vertices.setReference(index, reference); }
        void setEdgeReference(const int index, const char* reference) { m_edges.setReference(index, reference); }
        void setFaceReference(const int index, const char* reference) { m_faces.setReference(index, reference); }
        void setSolidReference(const int index, const char* reference) { m_solids.setReference(index, reference); }

        const int getVertexIndexByReference(const char* reference) { return m_vertices.getIndexByReference(reference); }
        const int getEdgeIndexByReference(const char* reference) { return m_edges.getIndexByReference(reference); }
        const int getFaceIndexByReference(const char* reference) { return m_faces.getIndexByReference(reference); }
        const int getSolidIndexByReference(const char* reference) { return m_solids.getIndexByReference(reference); }

    private:
        void initListIds(const Standard_Integer shapeType);

    private:
        TopoDS_Shape m_shape;
        std::string m_name;
        std::string m_format;
        GeometricElementList m_vertices;
        GeometricElementList m_edges;
        GeometricElementList m_faces;
        GeometricElementList m_solids;

    };
}

#endif
