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
// Author : Frederic Pons (OpenCascade)

#ifndef __XAO_BREPGEOMETRY_HXX__
#define __XAO_BREPGEOMETRY_HXX__

#include <string>
#include <vector>

#include <TopoDS_Shape.hxx>

#include "XAO_Xao.hxx"
#include "XAO_Geometry.hxx"

namespace XAO
{
    class BrepGeometry : public Geometry
    {
    public:
        BrepGeometry();
        BrepGeometry(const std::string& name);

        virtual const XAO::Format getFormat() { return XAO::BREP; }

        virtual const std::string getShape();
        virtual void setShape(const std::string& shape);

        TopoDS_Shape getTopoDS_Shape();
        void setTopoDS_Shape(const TopoDS_Shape& shape);

        /**
         * Gives the two extrimities of an edge.
         * @param edgeIndex the index of the edge.
         * @param vertexA
         * @param vertexB
         */
        void getEdgeVertices(const int& edgeIndex, int& vertexA, int& vertexB);

        /**
         * Gets the number of wires of a face (including holes).
         * @param faceIndex the index of the face.
         * @return the number of wires.
         */
        const int countFaceWires(const int& faceIndex);

        /**
         * Gets the indices of the wires of the face.
         * @param faceIndex the index of the face.
         * @param wireIndex the index of the wire.
         * @return the list of wires for the given face.
         */
        std::vector<int> getFaceEdges(const int& faceIndex, const int& wireIndex);

        /**
         * Gets the number of shells of a solid (including cavities).
         * @param solidIndex the index of the solid.
         * @return the number of shells.
         */
        const int countSolidShells(const int& solidIndex);

        /**
         * Gets the indices of the shells of the solids.
         * @param solidIndex the index of the solid.
         * @param shellIndex the index of the shell (for the given solid).
         * @return the list of shells for the given solid.
         */
        std::vector<int> getSolidFaces(const int& solidIndex, const int& shellIndex);

        /**
         * Gets the coordinates of a vertex.
         * @param vertexIndex the index of the vertex.
         * @param xCoord the X coordinate.
         * @param yCoord the Y coordinate.
         * @param zCoord the Z coordinate.
         */
        void getVertexXYZ(const int& vertexIndex, double& xCoord, double& yCoord, double& zCoord);

        /**
         * Gets the length of an edge.
         * @param edge the index of the edge.
         * @return the length of the edge.
         */
        const double getEdgeLength(const int& index);

        /**
         * Gets the are of a face.
         * @param face the index of a face.
         * @return the area of the face.
         */
        const double getFaceArea(const int& index);

        /**
         * Gets the volume of a solid.
         * @param solid the index of the solid.
         * @return the volume of the solid.
         */
        const double getSolidVolume(const int& index);

        /**
         * Gets the ID of a vertex.
         * @param vertex the index of the vertex.
         * @return the ID of the vertex.
         */
        const int getVertexID(const int& index);

        /**
         * Gets the ID of an edge.
         * @param edge the index of the edge.
         * @return the ID of the edge.
         */
        const int getEdgeID(const int& index);

        /**
         * Gets the ID of a face.
         * @param face the index of the face.
         * @return the ID of the face.
         */
        const int getFaceID(const int& index);

        /**
         * Gets the ID of a solid.
         * @param solid the index of the solid.
         * @return the ID of the solid.
         */
        const int getSolidID(const int& solid);

        void setVertexID(const int& index, const int& id);
        void setEdgeID(const int& index, const int& id);
        void setFaceID(const int& index, const int& id);
        void setSolidID(const int& index, const int& id);

        /**
         * Finds a vertex with its ID.
         * @param id the ID of the vertex.
         * @return the index of the vertex.
         */
        const int findVertex(const int& id);

        /**
         * Finds an edge with its ID.
         * @param id the ID of the edge.
         * @return the index of the edge.
         */
        const int findEdge(const int& id);

        /**
         * Finds a face with its ID.
         * @param id the ID of the face.
         * @return the index of the face.
         */
        const int findFace(const int& id);

        /**
         * Finds a solid with its ID.
         * @param id the ID of the solid.
         * @return th index of the solid.
         */
        const int findSolid(const int& id);

        /**
         * Finds the name of a vertex with its ID.
         * @param id the ID of the vertex.
         * @return the name of the vertex.
         */
        const std::string findVertexName(const int& id);

        /**
         * Finds the name of an edge with its ID.
         * @param id the ID of the edge.
         * @return the name of the edge.
         */
        const std::string findEdgeName(const int& id);

        /**
         * Finds the name of a face with its ID.
         * @param id the ID of the face.
         * @return the name of the face.
         */
        const std::string findFaceName(const int& id);

        /**
         * Finds the name of a solid with its ID.
         * @param id the ID of the solid.
         * @return the name of the solid.
         */
        const std::string findSolidName(const int& id);

        /**
         * Changes the name of a vertex.
         * @param id the ID of the vertex.
         * @param name the name to set.
         */
        void changeVertexName(const int& id, const std::string& name);

        /**
         * Changes the name of an edge.
         * @param id the ID of the edge
         * @param name the name to set.
         */
        void changeEdgeName(const int& id, const std::string& name);

        /**
         * Changes the name of a face.
         * @param id the ID of the face.
         * @param name the name to set.
         */
        void changeFaceName(const int& id, const std::string& name);

        /**
         * Changes the name of a solid.
         * @param id the ID of the solid.
         * @param name the name to set.
         */
        void changeSolidName(const int& id, const std::string& name);

    private:
        void initIds();
        void initListIds(const TopAbs_ShapeEnum& shapeType, GeometricElementList& eltList);
        TopoDS_Shape getSubShape(const TopoDS_Shape& mainShape, const TopAbs_ShapeEnum& shapeType, const int& shapeIndex);
        const int countGeometricalElements(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& shapeType);
        std::vector<int> getGeometricalElements(const TopoDS_Shape& shape, const TopAbs_ShapeEnum& shapeType, const XAO::Dimension& dim);
        const int findElement(const XAO::Dimension& dim, const int& id);

    private:
        TopoDS_Shape m_shape;
    };
}

#endif // __XAO_BREPGEOMETRY_HXX__
