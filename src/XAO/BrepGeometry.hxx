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

#include <SALOMEconfig.h>
#include <TopoDS_Shape.hxx>

#include "Xao.hxx"
#include "Geometry.hxx"

namespace XAO
{
    class BrepGeometry : public Geometry
    {
    public:
        BrepGeometry();
        BrepGeometry(const std::string& name);

        virtual const XAO::Format getFormat() { return XAO::BREP; }

        /**
         * Gives the two extrimities of an edge.
         * @param edge
         * @param vertexA
         * @param vertexB
         */
        void getEdgeVertices(const int& edge, int& vertexA, int& vertexB);

        const int countFacesWires(const int& face);
        std::vector<int> getFacesWires(const int& face);

        const int countSolidShells(const int& solid);
        std::vector<int> getSolidShells(const int& solid);

        void getVertexXYZ(const int& vertex, int& xCoord, int& yCoord, int& zCoord);
        const double getEdgleLength(const int& edge);
        const double getFaceArea(const int& face);
        const double getSolidVolume(const int& solid);

        const int getVertexID(const int& vertex);
        const int getEdgeID(const int& edge);
        const int getFaceID(const int& face);
        const int getSolidID(const int& solid);

        void setVertexID(const int& vertex, const int& id);
        void setEdgeID(const int& edge, const int& id);
        void setFaceID(const int& face, const int& id);
        void setSolidID(const int& solid, const int& id);

        const int findVertex(const int& id);
        const int findEdge(const int& id);
        const int findFace(const int& id);
        const int findSolid(const int& id);

        const std::string findVertexName(const int& id);
        const std::string findEdgeName(const int& id);
        const std::string findFaceName(const int& id);
        const std::string findSolidName(const int& id);

        void changeVertexName(const int& id, const std::string& name);
        void changeEdgeName(const int& id, const std::string& name);
        void changeFaceName(const int& id, const std::string& name);
        void changeSolidName(const int& id, const std::string& name);
    };
}

#endif // __XAO_BREPGEOMETRY_HXX__
