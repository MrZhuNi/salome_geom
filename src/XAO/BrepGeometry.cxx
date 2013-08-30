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

#include "BrepGeometry.hxx"
#include "XaoUtils.hxx"

using namespace XAO;

BrepGeometry::BrepGeometry() : Geometry("")
{
}

BrepGeometry::BrepGeometry(const std::string& name) : Geometry(name)
{
}

void BrepGeometry::getEdgeVertices(const int& edge, int& vertexA, int& vertexB)
{
    throw SALOME_Exception("Not impolemented");
}

const int BrepGeometry::countFacesWires(const int& face)
{
    throw SALOME_Exception("Not impolemented");
}

std::vector<int> BrepGeometry::getFacesWires(const int& face)
{
    throw SALOME_Exception("Not impolemented");
}

const int BrepGeometry::countSolidShells(const int& solid)
{
    throw SALOME_Exception("Not impolemented");
}

std::vector<int> BrepGeometry::getSolidShells(const int& solid)
{
    throw SALOME_Exception("Not impolemented");
}

void BrepGeometry::getVertexXYZ(const int& vertex, int& xCoord, int& yCoord, int& zCoord)
{
    // TODO
    TopTools_MapOfShape mapShape;
    TopTools_ListOfShape listShape;

    TopExp_Explorer exp(m_shape, TopAbs_ShapeEnum(TopAbs_VERTEX));
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
//    switch (shapeType)
//    {
//        case TopAbs_VERTEX:
//        {
//        }
//        case TopAbs_EDGE:
//        {
//            m_edges.setSize(indexList.size());
//            for (int i = 0; it != indexList.end(); it++, i++)
//                m_edges.setReference(i, XaoUtils::intToString((*it)));
//            break;
//        }
//        case TopAbs_FACE:
//        {
//            m_faces.setSize(indexList.size());
//            for (int i = 0; it != indexList.end(); it++, i++)
//                m_faces.setReference(i, XaoUtils::intToString((*it)));
//            break;
//        }
//        case TopAbs_SOLID:
//        {
//            m_solids.setSize(indexList.size());
//            for (int i = 0; it != indexList.end(); it++, i++)
//                m_solids.setReference(i, XaoUtils::intToString((*it)));
//            break;
//        }
//    }
}

const double BrepGeometry::getEdgleLength(const int& edge)
{
    throw SALOME_Exception("Not impolemented");
}

const double BrepGeometry::getFaceArea(const int& face)
{
    throw SALOME_Exception("Not impolemented");
}

const double BrepGeometry::getSolidVolume(const int& solid)
{
    throw SALOME_Exception("Not impolemented");
}

const int BrepGeometry::getVertexID(const int& vertex)
{
    return XaoUtils::stringToInt(getVertexReference(vertex));
}

const int BrepGeometry::getEdgeID(const int& edge)
{
    return XaoUtils::stringToInt(getEdgeReference(edge));
}

const int BrepGeometry::getFaceID(const int& face)
{
    return XaoUtils::stringToInt(getFaceReference(face));
}

const int BrepGeometry::getSolidID(const int& solid)
{
    return XaoUtils::stringToInt(getSolidReference(solid));
}

void BrepGeometry::setVertexID(const int& vertex, const int& id)
{
    throw SALOME_Exception("Not impolemented");
}

void BrepGeometry::setEdgeID(const int& edge, const int& id)
{
    throw SALOME_Exception("Not impolemented");
}

void BrepGeometry::setFaceID(const int& face, const int& id)
{
    throw SALOME_Exception("Not impolemented");
}

void BrepGeometry::setSolidID(const int& solid, const int& id)
{
    throw SALOME_Exception("Not impolemented");
}

const int BrepGeometry::findVertex(const int& id)
{
    return getVertexIndexByReference(XaoUtils::intToString(id));
}

const int BrepGeometry::findEdge(const int& id)
{
    return getEdgeIndexByReference(XaoUtils::intToString(id));
}

const int BrepGeometry::findFace(const int& id)
{
    return getFaceIndexByReference(XaoUtils::intToString(id));
}

const int BrepGeometry::findSolid(const int& id)
{
    return getSolidIndexByReference(XaoUtils::intToString(id));
}

const std::string BrepGeometry::findVertexName(const int& id)
{
    return getVertexName(findVertex(id));
}

const std::string BrepGeometry::findEdgeName(const int& id)
{
    return getEdgeName(findEdge(id));
}

const std::string BrepGeometry::findFaceName(const int& id)
{
    return getFaceName(findFace(id));
}

const std::string BrepGeometry::findSolidName(const int& id)
{
    return getSolidName(findSolid(id));
}

void BrepGeometry::changeVertexName(const int& id, const std::string& name)
{
    setVertexName(findVertex(id), name);
}

void BrepGeometry::changeEdgeName(const int& id, const std::string& name)
{
    setEdgeName(findEdge(id), name);
}

void BrepGeometry::changeFaceName(const int& id, const std::string& name)
{
    setFaceName(findFace(id), name);
}

void BrepGeometry::changeSolidName(const int& id, const std::string& name)
{
    setSolidName(findSolid(id), name);
}

