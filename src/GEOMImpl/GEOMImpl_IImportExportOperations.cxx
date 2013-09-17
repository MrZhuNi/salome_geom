// Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include <cmath>
#include <iostream>
#include <sstream>

#include <Standard_Stream.hxx>
#include <Utils_SALOME_Exception.hxx>

#include "GEOMImpl_Types.hxx"
#include "GEOMImpl_IImportExportOperations.hxx"
#include "GEOMImpl_IBasicOperations.hxx"
#include "GEOMImpl_IBooleanOperations.hxx"
#include "GEOMImpl_IShapesOperations.hxx"
#include "GEOMImpl_ITransformOperations.hxx"
#include "GEOMImpl_IBlocksOperations.hxx"
#include "GEOMImpl_I3DPrimOperations.hxx"
#include "GEOMImpl_ILocalOperations.hxx"
#include "GEOMImpl_IHealingOperations.hxx"

#include "GEOMImpl_Gen.hxx"

#include <Basics_OCCTVersion.hxx>

#include <utilities.h>
#include <OpUtil.hxx>
#include <Utils_ExceptHandlers.hxx>

#include "GEOM_Function.hxx"
#include "GEOM_ISubShape.hxx"
#include "GEOM_PythonDump.hxx"

#include <XAO_Xao.hxx>
#include <XAO_Geometry.hxx>
#include <XAO_BrepGeometry.hxx>
#include <XAO_Group.hxx>
#include <XAO_Field.hxx>
#include <XAO_XaoUtils.hxx>

#include <GEOMImpl_XAODriver.hxx>
#include <GEOMImpl_IImportExportXAO.hxx>
/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopAbs.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <TopTools_Array1OfShape.hxx>
#include <TopTools_DataMapIteratorOfDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopTools_MapOfOrientedShape.hxx>
#include <TCollection_AsciiString.hxx>

#include <TColStd_HSequenceOfTransient.hxx>
#include <Standard_Transient.hxx>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax3.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>


#include <TFunction_DriverTable.hxx>
#include <TFunction_Driver.hxx>
#include <TFunction_Logbook.hxx>
#include <TDataStd_Integer.hxx>
#include <TDF_Tool.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx> // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC
//=============================================================================
/*!
 *  Constructor
 */
//=============================================================================
GEOMImpl_IImportExportOperations::GEOMImpl_IImportExportOperations(GEOM_Engine* engine, int docID) :
        GEOM_IOperations(engine, docID)
{
    MESSAGE("GEOMImpl_IImportExportOperations::GEOMImpl_IImportExportOperations");
    m_shapesOperations = new GEOMImpl_IShapesOperations(GetEngine(), GetDocID());
    m_groupOperations = new GEOMImpl_IGroupOperations(GetEngine(), GetDocID());
}

//=============================================================================
/*!
 *  Destructor
 */
//=============================================================================
GEOMImpl_IImportExportOperations::~GEOMImpl_IImportExportOperations()
{
    MESSAGE("GEOMImpl_IImportExportOperations::~GEOMImpl_IImportExportOperations");
    delete m_shapesOperations;
    delete m_groupOperations;
}

TopAbs_ShapeEnum getGroupDimension(XAO::Group* group)
{
    XAO::Dimension dim = group->getDimension();
    if (dim == XAO::VERTEX)
        return TopAbs_VERTEX;
    if (dim == XAO::EDGE)
        return TopAbs_EDGE;
    if (dim == XAO::FACE)
        return TopAbs_FACE;
    if (dim == XAO::SOLID)
        return TopAbs_SOLID;
    return TopAbs_COMPOUND;
}

XAO::Dimension shapeEnumToDimension(const TopAbs_ShapeEnum& shape)
{
    if (shape == TopAbs_VERTEX)
        return XAO::VERTEX;
    if (shape == TopAbs_EDGE)
        return XAO::EDGE;
    if (shape == TopAbs_FACE)
        return XAO::FACE;
    if (shape == TopAbs_SOLID)
        return XAO::SOLID;
    throw SALOME_Exception("Bad type"); // TODO
}

void GEOMImpl_IImportExportOperations::exportGroups(std::list<Handle(GEOM_Object)> groupList, XAO::Xao* xaoObject, XAO::BrepGeometry* geometry)
{
    // add the groups
    std::list<Handle(GEOM_Object)>::iterator groupIterator = groupList.begin();
    while (groupIterator != groupList.end())
    {
        Handle(GEOM_Object) currGroup = (*groupIterator++);
        Handle(TColStd_HArray1OfInteger) groupIds = m_groupOperations->GetObjects(currGroup);

        TopAbs_ShapeEnum shapeGroup = m_groupOperations->GetType(currGroup);
        XAO::Dimension dim = shapeEnumToDimension(shapeGroup);
        XAO::Group* group = xaoObject->addGroup(dim, currGroup->GetName());

        switch (shapeGroup)
        {
            case TopAbs_VERTEX:
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    std::string ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    int index = geometry->getVertexIndexByReference(ref);
                    group->add(index);
                }
                break;
            case TopAbs_EDGE:
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    std::string ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    int index = geometry->getEdgeIndexByReference(ref);
                    group->add(index);
                }
                break;
            case TopAbs_FACE:
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    std::string ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    int index = geometry->getFaceIndexByReference(ref);
                    group->add(index);
                }
                break;
            case TopAbs_SOLID:
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    std::string ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    int index = geometry->getSolidIndexByReference(ref);
                    group->add(index);
                }
                break;
        }
    }
}

void GEOMImpl_IImportExportOperations::exportFields(std::list<Handle(GEOM_Object)> fieldList, XAO::Xao* xaoObject, XAO::BrepGeometry* geometry)
{
    // TODO
}

void GEOMImpl_IImportExportOperations::exportSubshapes(const Handle(GEOM_Object)& shape, XAO::BrepGeometry* geometry)
{
    Handle(TColStd_HSequenceOfTransient) subObjects = m_shapesOperations->GetExistingSubObjects(shape, false);
    int nbSubObjects = subObjects->Length();
    // set the names of the sub shapes
    for (int i = 1; i <= nbSubObjects; i++)
    {
        Handle(Standard_Transient) transientSubObject = subObjects->Value(i);
        if (transientSubObject.IsNull())
            continue;

        Handle(GEOM_Object) subObject = Handle(GEOM_Object)::DownCast(transientSubObject);
        if (subObject->GetType() != GEOM_GROUP)
        {
            int subIndex = m_shapesOperations->GetSubShapeIndex(shape, subObject);
            switch (subObject->GetValue().ShapeType())
            {
                case TopAbs_VERTEX:
                    geometry->changeVertexName(subIndex, subObject->GetName());
                    break;
                case TopAbs_EDGE:
                    geometry->changeEdgeName(subIndex, subObject->GetName());
                    break;
                case TopAbs_FACE:
                    geometry->changeFaceName(subIndex, subObject->GetName());
                    break;
                case TopAbs_SOLID:
                    geometry->changeSolidName(subIndex, subObject->GetName());
                    break;
            }
        }
    }
}

//=============================================================================
/*!
 *  Export a shape to XAO format
 *  \param shape The shape to export
 *  \param groups The list of groups to export
 *  \param fields The list of fields to export
 *  \param fileName The name of the file to exported
 *  \return boolean indicating if export was succeful.
 */
//=============================================================================
bool GEOMImpl_IImportExportOperations::ExportXAO(Handle(GEOM_Object) shape,
        std::list<Handle(GEOM_Object)> groupList,
        std::list<Handle(GEOM_Object)> fieldList,
        const char* author,
        const char* fileName)
{
    SetErrorCode(KO);

    if (shape.IsNull()) return false;

    // add a new shape function with parameters
    Handle(GEOM_Function) lastFunction = shape->GetLastFunction();
    if (lastFunction.IsNull()) return false;

    // add a new result object
    Handle(GEOM_Object) result = GetEngine()->AddObject(GetDocID(), GEOM_IMPORT);

    // add an Export function
    Handle(GEOM_Function) exportFunction = result->AddFunction(GEOMImpl_XAODriver::GetID(), IMPORTEXPORT_EXPORTXAO);
    if (exportFunction.IsNull()) return false;
    if (exportFunction->GetDriverGUID() != GEOMImpl_XAODriver::GetID()) return false;

    // create the XAO object
    XAO::Xao* xaoObject = new XAO::Xao();
    xaoObject->setAuthor(author);

    // add the geometry
    XAO::BrepGeometry* geometry = (XAO::BrepGeometry*)XAO::Geometry::createGeometry(XAO::BREP);
    TopoDS_Shape topoShape = shape->GetValue();
    exportFunction->SetValue(topoShape);
    XAO::BrepGeometry* brep = (XAO::BrepGeometry*)geometry;
    brep->setTopoDS_Shape(topoShape);

    geometry->setName(shape->GetName());
    exportSubshapes(shape, geometry);
    xaoObject->setGeometry(geometry);

    exportGroups(groupList, xaoObject, geometry);
    exportFields(fieldList, xaoObject, geometry);

    // export the XAO to the file
    xaoObject->exportXAO(fileName);

    // make a Python command
    GEOM::TPythonDump pd(exportFunction);
    pd << "exported = geompy.ExportXAO(" << shape;

    // list of groups
    pd << ", [";
    if (groupList.size() > 0)
    {
        std::list<Handle(GEOM_Object)>::iterator itGroup = groupList.begin();
        pd << (*itGroup++);
        while (itGroup != groupList.end())
        {
            pd << ", " << (*itGroup++);
        }
    }

    // list of fields
    pd << "], [";
    if (fieldList.size() > 0)
    {
        std::list<Handle(GEOM_Object)>::iterator itField = fieldList.begin();
        pd << (*itField++);
        while (itField != fieldList.end())
        {
            pd << ", " << (*itField++);
        }
    }
    pd << "], ";
    pd << author << ", \"" << fileName << "\")";

    SetErrorCode(OK);
    delete xaoObject;

    return true;
}

void GEOMImpl_IImportExportOperations::importSubShapes(XAO::Geometry* xaoGeometry,
        Handle(GEOM_Function) function, const int& shapeType, const XAO::Dimension& dim,
        Handle(TColStd_HSequenceOfTransient)& subShapeList)
{
    Handle(GEOM_Object) subShape;
    Handle(GEOM_Function) aFunction;
    Handle(TColStd_HArray1OfInteger) anArray;

    XAO::GeometricElementList::iterator elementIterator = xaoGeometry->begin(dim);
    for (; elementIterator != xaoGeometry->end(dim); elementIterator++)
    {
        XAO::GeometricElement element = elementIterator->second;
        if (!element.hasName())
            continue;

        std::string name = element.getName();
        std::string ref = element.getReference();
        int iref = XAO::XaoUtils::stringToInt(ref);

        anArray = new TColStd_HArray1OfInteger(1, 1);
        anArray->SetValue(1, iref);

        subShape = GetEngine()->AddObject(GetDocID(), GEOM_SUBSHAPE);
        Handle(GEOM_Function) aFunction = subShape->AddFunction(GEOM_Object::GetSubShapeID(), 1);
        if (aFunction.IsNull())
            return;

        subShape->SetName(name.c_str());
        subShape->SetType(shapeType);

        GEOM_ISubShape aSSI(aFunction);
        aSSI.SetMainShape(function);
        aSSI.SetIndices(anArray);

        //aFunction->SetValue(aValue);
        subShapeList->Append(subShape);

        // Put this subshape in the list of sub-shapes of theMainShape
        function->AddSubShapeReference(aFunction);
    }
}

//=============================================================================
/*!
 *  Import a shape from XAO format
 *  \param fileName The name of the file to import
 *  \param shape The imported shape
 *  \param subShapes The list of imported groups
 *  \param groups The list of imported groups
 *  \param fields The list of imported fields
 *  \return boolean indicating if import was succeful.
 */
//=============================================================================
bool GEOMImpl_IImportExportOperations::ImportXAO(const char* fileName,
        Handle(GEOM_Object)& shape,
        Handle(TColStd_HSequenceOfTransient)& subShapes,
        Handle(TColStd_HSequenceOfTransient)& groups,
        Handle(TColStd_HSequenceOfTransient)& fields)
{
    SetErrorCode(KO);

    if (fileName == NULL || groups.IsNull() || fields.IsNull())
        return false;

    // Read the XAO
    XAO::Xao* xaoObject = new XAO::Xao();
    try
    {
        xaoObject->importXAO(fileName);
    }
    catch (XAO::XAO_Exception& exc)
    {
        delete xaoObject;
        SetErrorCode(exc.what());
        return false;
    }

    XAO::Geometry* xaoGeometry = xaoObject->getGeometry();
    if (xaoGeometry == NULL)
    {
        delete xaoObject;
        SetErrorCode("Cannot import XAO: geometry format not supported.");
        return false;
    }

    // create the shape
    shape = GetEngine()->AddObject(GetDocID(), GEOM_IMPORT);
    Handle(GEOM_Function) function = shape->AddFunction(GEOMImpl_XAODriver::GetID(), IMPORTEXPORT_EXPORTXAO);
    if (function.IsNull()) return false;
    if (function->GetDriverGUID() != GEOMImpl_XAODriver::GetID()) return false;

    // set the geometry
    if (xaoGeometry->getFormat() == XAO::BREP)
    {
        XAO::BrepGeometry* brep = (XAO::BrepGeometry*)xaoGeometry;
        TopoDS_Shape geomShape = brep->getTopoDS_Shape();
        function->SetValue(geomShape);
        shape->SetName(xaoGeometry->getName().c_str());
    }
    else
    {
        delete xaoObject;
        SetErrorCode("Cannot import XAO: geometry format not supported.");
        return false;
    }

    // create sub shapes with names
    importSubShapes(xaoGeometry, function, GEOM_POINT, XAO::VERTEX, subShapes);
    importSubShapes(xaoGeometry, function, GEOM_EDGE, XAO::EDGE, subShapes);
    importSubShapes(xaoGeometry, function, GEOM_FACE, XAO::FACE, subShapes);
    importSubShapes(xaoGeometry, function, GEOM_SOLID, XAO::SOLID, subShapes);

    // create groups
    int nbGroups = xaoObject->countGroups();
    for (int i = 0; i < nbGroups; ++i)
    {
        XAO::Group* xaoGroup = xaoObject->getGroup(i);

        // build an array with the indexes of the sub shapes
        int nbElt = xaoGroup->count();
        Handle(TColStd_HArray1OfInteger) array = new TColStd_HArray1OfInteger(1, nbElt);
        int j = 0;
        for (std::set<int>::iterator it = xaoGroup->begin(); it != xaoGroup->end(); ++it)
        {
            int index = (*it);
            std::string ref = xaoGeometry->getElementReference(xaoGroup->getDimension(), index);
            array->SetValue(++j, XAO::XaoUtils::stringToInt(ref));
        }

        // create the group with the array of sub shapes indexes
        Handle(GEOM_Object) group = GetEngine()->AddSubShape(shape, array);
        group->SetType(GEOM_GROUP);
        group->SetName(xaoGroup->getName().c_str());

        // Set a sub-shape type
        TDF_Label freeLabel = group->GetFreeLabel();
        TDataStd_Integer::Set(freeLabel, (Standard_Integer) getGroupDimension(xaoGroup));
        groups->Append(group);

        function = group->GetLastFunction();
    }

    // TODO: create the fields

    // make a Python command
    GEOM::TPythonDump pd(function);
    pd << "(imported, " << shape << ", ";

    // list of sub shapes
    pd << "[";
    int nbSubshapes = subShapes->Length();
    std::cout << "Nb SubShapes = " << nbSubshapes << std::endl;
    if (nbSubshapes > 0)
    {
        for (int i = 1; i <= nbSubshapes; i++)
        {
            Handle(GEOM_Object) obj = Handle(GEOM_Object)::DownCast(subShapes->Value(i));
            pd << obj << ((i < nbSubshapes) ? ", " : "");
        }
    }
    pd << "], [";

    // list of groups
    if (nbGroups > 0)
    {
        for (int i = 1; i <= nbGroups; i++)
        {
            Handle(GEOM_Object) obj = Handle(GEOM_Object)::DownCast(groups->Value(i));
            pd << obj << ((i < nbGroups) ? ", " : "");
        }
    }

    pd << "], [";

    // list of fields
    pd << "]";
    pd << ") = geompy.ImportXAO(\"" << fileName << "\")";

    delete xaoObject;
    SetErrorCode(OK);

    return true;
}


/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/
