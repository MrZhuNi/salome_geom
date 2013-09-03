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

#include <cassert>

#include <TopTools_MapOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <TColStd_HArray1OfInteger.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>

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

TopoDS_Shape BrepGeometry::getGeometricalElement(TopAbs_ShapeEnum shapeType, const int& shapeIndex)
{
    TopTools_MapOfShape mapShape;
    TopTools_ListOfShape listShape;

    TopExp_Explorer exp(m_shape, shapeType);
    for (; exp.More(); exp.Next())
    {
        if (mapShape.Add(exp.Current()))
            listShape.Append(exp.Current());
    }

    if (!listShape.IsEmpty())
    {
        TopTools_IndexedMapOfShape indices;
        TopExp::MapShapes(m_shape, indices);

        TopTools_ListIteratorOfListOfShape itSub(listShape);
        for (int index = 1; itSub.More(); itSub.Next(), ++index)
        {
            TopoDS_Shape value = itSub.Value();
            if (shapeIndex == (int)indices.FindIndex(value))
                return value;
        }
    }

    throw SALOME_Exception(MsgBuilder() << "Shape not found: " << shapeIndex);
}

const int BrepGeometry::countGeometricalElements(TopoDS_Shape shape, TopAbs_ShapeEnum shapeType)
{
    int res = 0;
    TopTools_MapOfShape mapShape;
    TopExp_Explorer exp(shape, shapeType);
    for (; exp.More(); exp.Next())
    {
//        if (mapShape.Add(exp.Current()))
            res++;
    }

    return res;
}

std::vector<int> BrepGeometry::getGeometricalElements(TopoDS_Shape shape, TopAbs_ShapeEnum shapeType)
{
    std::vector<int> indexList;

    TopTools_MapOfShape mapShape;
    TopTools_ListOfShape listShape;

    TopExp_Explorer exp(shape, shapeType);
    for (; exp.More(); exp.Next())
    {
        if (mapShape.Add(exp.Current()))
            listShape.Append(exp.Current());
    }

    if (!listShape.IsEmpty())
    {
        TopTools_IndexedMapOfShape indices;
        TopExp::MapShapes(shape, indices);

        TopTools_ListIteratorOfListOfShape itSub(listShape);
        for (int index = 1; itSub.More(); itSub.Next(), ++index)
        {
            TopoDS_Shape value = itSub.Value();
            indexList.push_back(indices.FindIndex(value));
        }
    }

    return indexList;
}

void BrepGeometry::getEdgeVertices(const int& edge, int& vertexA, int& vertexB)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_EDGE, edge);
    std::vector<int> vertices = getGeometricalElements(shape, TopAbs_VERTEX);
    assert(vertices.size() == 2);

    vertexA = vertices[0];
    vertexB = vertices[1];
}

const int BrepGeometry::countFaceWires(const int& face)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_FACE, face);
    return countGeometricalElements(shape, TopAbs_WIRE);
}

std::vector<int> BrepGeometry::getFaceWires(const int& face)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_FACE, face);
    return getGeometricalElements(shape, TopAbs_WIRE);
}

const int BrepGeometry::countSolidShells(const int& solid)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_SOLID, solid);
    return countGeometricalElements(shape, TopAbs_SHELL);
}

std::vector<int> BrepGeometry::getSolidShells(const int& solid)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_SOLID, solid);
    return getGeometricalElements(shape, TopAbs_SHELL);
}

void BrepGeometry::getVertexXYZ(const int& vertex, double& xCoord, double& yCoord, double& zCoord)
{
    xCoord = 0.;
    yCoord = 0.;
    zCoord = 0.;

    TopoDS_Shape shape = getGeometricalElement(TopAbs_VERTEX, vertex);
    if (shape.ShapeType() != TopAbs_VERTEX)
        throw SALOME_Exception(MsgBuilder() << "Shape " << vertex << " is not a point.");

    TopoDS_Vertex point = TopoDS::Vertex(shape);
    if (!point.IsNull())
    {
        gp_Pnt aPnt = BRep_Tool::Pnt(point);
        xCoord = aPnt.X();
        yCoord = aPnt.Y();
        zCoord = aPnt.Z();
    }
}

const double BrepGeometry::getEdgeLength(const int& edge)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_EDGE, edge);
    GProp_GProps system;
    BRepGProp::LinearProperties(shape, system);
    return system.Mass();
}

const double BrepGeometry::getFaceArea(const int& face)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_FACE, face);
    GProp_GProps system;
    BRepGProp::SurfaceProperties(shape, system);
    return system.Mass();
}

const double BrepGeometry::getSolidVolume(const int& solid)
{
    TopoDS_Shape shape = getGeometricalElement(TopAbs_SOLID, solid);
    GProp_GProps system;
    BRepGProp::VolumeProperties(shape, system);
    return system.Mass();
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
    setVertexReference(vertex, XaoUtils::intToString(id));
}

void BrepGeometry::setEdgeID(const int& edge, const int& id)
{
    setEdgeReference(edge, XaoUtils::intToString(id));
}

void BrepGeometry::setFaceID(const int& face, const int& id)
{
    setEdgeReference(face, XaoUtils::intToString(id));
}

void BrepGeometry::setSolidID(const int& solid, const int& id)
{
    setEdgeReference(solid, XaoUtils::intToString(id));
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
