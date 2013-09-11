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

#include "XAO_Xao.hxx"
#include "XAO_GeometricElement.hxx"

namespace XAO
{
    /**
     * @class Geometry
     * Base class for geometries.
     */
    class Geometry
    {
    protected:
        /**
         * Constructor.
         * @param name the name of the Geometry.
         */
        Geometry(const std::string& name);

    public:
        /**
         * Creates a geometry.
         * @param format the format of the geometry.
         * @return the created geometry.
         */
        static Geometry* createGeometry(const XAO::Format& format);

        /**
         * Constructor.
         * Creates a geometry.
         * @name name the name of the geometry.
         * @return the created geometry.
         */
        static Geometry* createGeometry(const XAO::Format& format, const std::string& name);

        /** Destructor. */
        ~Geometry();

        /**
         * Gets the name of the geometry.
         * @return the name of the geometry.
         */
        const std::string getName()
        {
            return m_name;
        }
        /**
         * Sets the name of the geometry.
         * @param name the name to set.
         */
        void setName(const std::string& name)
        {
            m_name = name;
        }

        /**
         * Gets the format of the geometry.
         * @return the format of the geometry.
         */
        virtual const XAO::Format getFormat() = 0;

        virtual const std::string getShape() = 0;
        virtual void setShape(const std::string& shape) = 0;

        const int countElements(const XAO::Dimension& dim);
        const int countVertices() { return m_vertices.getSize(); }
        const int countEdges() { return m_edges.getSize(); }
        const int countFaces() { return m_faces.getSize(); }
        const int countSolids() { return m_solids.getSize(); }

        void setCountVertices(const int& nb) { m_vertices.setSize(nb); }
        void setCountEdges(const int& nb) { m_edges.setSize(nb); }
        void setCountFaces(const int& nb) { m_faces.setSize(nb); }
        void setCountSolids(const int& nb) { m_solids.setSize(nb); }

        const std::string getVertexName(const int& index) { return m_vertices.getName(index); }
        const std::string getEdgeName(const int& index) { return m_edges.getName(index); }
        const std::string getFaceName(const int& index) { return m_faces.getName(index); }
        const std::string getSolidName(const int& index) { return m_solids.getName(index); }

        void setVertexName(const int& index, const std::string& name) { m_vertices.setName(index, name); }
        void setEdgeName(const int& index, const std::string& name) { m_edges.setName(index, name); }
        void setFaceName(const int& index, const std::string& name) { m_faces.setName(index, name); }
        void setSolidName(const int& index, const std::string& name) { m_solids.setName(index, name); }

        const bool hasVertexName(const int& index) { return m_vertices.hasName(index); }
        const bool hasEdgeName(const int& index) { return m_edges.hasName(index); }
        const bool hasFaceName(const int& index) { return m_faces.hasName(index); }
        const bool hasSolidName(const int& index) { return m_solids.hasName(index); }

        const std::string getVertexReference(const int& index) { return m_vertices.getReference(index); }
        const std::string getEdgeReference(const int& index) { return m_edges.getReference(index); }
        const std::string getFaceReference(const int& index) { return m_faces.getReference(index); }
        const std::string getSolidReference(const int& index) { return m_solids.getReference(index); }
        const std::string getElementReference(const XAO::Dimension& dim, const int& index);

        void setVertexReference(const int& index, const std::string& reference) { m_vertices.setReference(index, reference); }
        void setEdgeReference(const int& index, const std::string& reference) { m_edges.setReference(index, reference); }
        void setFaceReference(const int& index, const std::string& reference) { m_faces.setReference(index, reference); }
        void setSolidReference(const int& index, const std::string& reference) { m_solids.setReference(index, reference); }

        void setVertex(const int& index, const std::string& name, const std::string& reference) { m_vertices.setElement(index, name, reference); }
        void setEdge(const int& index, const std::string& name, const std::string& reference) { m_edges.setElement(index, name, reference); }
        void setFace(const int& index, const std::string& name, const std::string& reference) { m_faces.setElement(index, name, reference); }
        void setSolid(const int& index, const std::string& name, const std::string& reference) { m_solids.setElement(index, name, reference); }

        const int getVertexIndexByReference(const std::string& reference) { return m_vertices.getIndexByReference(reference); }
        const int getEdgeIndexByReference(const std::string& reference) { return m_edges.getIndexByReference(reference); }
        const int getFaceIndexByReference(const std::string& reference) { return m_faces.getIndexByReference(reference); }
        const int getSolidIndexByReference(const std::string& reference) { return m_solids.getIndexByReference(reference); }
        const int getElementIndexByReference(const XAO::Dimension& dim, const std::string& reference);

        GeometricElementList::iterator begin(const XAO::Dimension& dim);
        GeometricElementList::iterator end(const XAO::Dimension& dim);

    protected:
        std::string m_name;
        GeometricElementList m_vertices;
        GeometricElementList m_edges;
        GeometricElementList m_faces;
        GeometricElementList m_solids;

    };
}

#endif
