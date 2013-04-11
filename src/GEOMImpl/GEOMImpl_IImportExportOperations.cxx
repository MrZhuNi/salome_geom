// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  File   : GEOMImpl_IImportExportOperations.cxx
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)

#include <cmath>
#include <iostream>
#include <sstream>

#include <Standard_Stream.hxx>

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
#include "GEOM_PythonDump.hxx"

#include "Xao.hxx"
#include "Geometry.hxx"
#include "Group.hxx"
#include "XaoUtils.hxx"

#include <GEOMImpl_ExportXAODriver.hxx>
#include <GEOMImpl_IExportXAO.hxx>
/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopAbs.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

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

//=============================================================================
/*!
 *  Export a shape to XAO Format
 *  \param fileName The name of the exported file
 *  \param shape The shape to export
 *  \param groups The list of groups to export
 *  \param fields The list of fields to export
 *  \return boolean indicating if export was succeful.
 */
//=============================================================================
bool GEOMImpl_IImportExportOperations::ExportXAO(const std::string fileName,
        Handle(GEOM_Object) shape,
        std::list<Handle(GEOM_Object)> groupList,
        std::list<Handle(GEOM_Object)> fieldList)
{
    SetErrorCode(KO);

    // add a new shape function with parameters
    Handle(GEOM_Function) lastFunction = shape->GetLastFunction();
    if (lastFunction.IsNull())
        return false;

    // add a new result object
    Handle(GEOM_Object) result = GetEngine()->AddObject(GetDocID(), GEOM_IMPORT);

    // add an Export function
    Handle(GEOM_Function) exportFunction = result->AddFunction(GEOMImpl_ExportXAODriver::GetID(),
            EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS);
    if (exportFunction.IsNull())
        return false;

    // check if the function is set correctly
    if (exportFunction->GetDriverGUID() != GEOMImpl_ExportXAODriver::GetID())
        return false;

    XAO::Xao* xaoObject = new XAO::Xao();

    XAO::Geometry* geometry = new XAO::Geometry();
    TopoDS_Shape topoShape = shape->GetValue();
    geometry->setShape(topoShape);
    geometry->setName(shape->GetName());

    Handle(TColStd_HSequenceOfTransient) subObjects = m_shapesOperations->GetExistingSubObjects(shape, false);
    int nbSubObjects = subObjects->Length();

    int tmpIndex;
    for (int i = 1; i <= nbSubObjects; i++)
    {
        Handle(Standard_Transient) transientSubObject = subObjects->Value(i);
        if (transientSubObject.IsNull())
            continue;

        Handle(GEOM_Object) subObject = Handle(GEOM_Object)::DownCast(transientSubObject);
        if (subObject->GetType() != GEOM_GROUP)
        {
            int subIndex = m_shapesOperations->GetSubShapeIndex(shape, subObject);
            // convert index to a string
            std::stringstream str;
            str << subIndex;
            std::string strIndex = str.str();

            switch (subObject->GetValue().ShapeType())
            {
                case TopAbs_VERTEX:
                    tmpIndex = geometry->getVertexIndexByReference(strIndex.c_str());
                    geometry->setVertexName(tmpIndex, subObject->GetName());
                    break;
                case TopAbs_EDGE:
                    tmpIndex = geometry->getEdgeIndexByReference(strIndex.c_str());
                    geometry->setEdgeName(tmpIndex, subObject->GetName());
                    break;
                case TopAbs_FACE:
                    tmpIndex = geometry->getFaceIndexByReference(strIndex.c_str());
                    geometry->setFaceName(tmpIndex, subObject->GetName());
                    break;
                case TopAbs_SOLID:
                    tmpIndex = geometry->getSolidIndexByReference(strIndex.c_str());
                    geometry->setSolidName(tmpIndex, subObject->GetName());
                    break;
            }
        }
    }
    xaoObject->setGeometry(geometry);

    // adding groups
    std::list<Handle(GEOM_Object)>::iterator itG1 = groupList.begin();
    while (itG1 != groupList.end())
    {
        Handle(GEOM_Object) itGroup = (*itG1++);
        Handle(TColStd_HArray1OfInteger) groupIds = m_groupOperations->GetObjects(itGroup);
        TopAbs_ShapeEnum shapeGroup = m_groupOperations->GetType(itGroup);

        XAO::Group* group = new XAO::Group();
        group->setName(itGroup->GetName());
        switch (shapeGroup)
        {
            case TopAbs_VERTEX:
                group->setDimension(0);
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    const char* ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    const int index = geometry->getVertexIndexByReference(ref);
                    group->addElement(index);
                }
                break;
            case TopAbs_EDGE:
                group->setDimension(1);
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    const char* ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    const int index = geometry->getEdgeIndexByReference(ref);
                    group->addElement(index);
                }
                break;
            case TopAbs_FACE:
                group->setDimension(2);
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    const char* ref = XAO::XaoUtils::intToString((int)(groupIds->Value(i)));
                    const int index = geometry->getFaceIndexByReference(ref);
                    group->addElement(index);
                }
                break;
            case TopAbs_SOLID:
                group->setDimension(3);
                for (int i = 1; i <= groupIds->Length(); i++)
                {
                    const char* ref = XAO::XaoUtils::intToString(groupIds->Value(i));
                    const int index = geometry->getSolidIndexByReference(ref);
                    group->addElement(index);
                }
                break;
        }

        xaoObject->addGroup(group);
    }

    xaoObject->exportToFile(fileName.c_str());

    // make a Python command
    /*TCollection_AsciiString fileNameStr = fileName.c_str();
    GEOM::TPythonDump pd(exportFunction);
    std::list<Handle(GEOM_Object)>::iterator itG = groupList.begin();
    std::list<Handle(GEOM_Object)>::iterator itF = fieldList.begin();
    //pd << /*isGood <<**" = geompy.ExportXAO(" << shape << ", " << fileNameStr.ToCString() << ", [";

    pd << (*itG++);
    while (itG != groupList.end())
    {
        pd << ", " << (*itG++);
    }
    pd << "], [";
    pd << (*itF++);
    while (itF != fieldList.end())
    {
        pd << ", " << (*itF++);
    }
    pd << "])";*/

    SetErrorCode(OK);

    return true;
}

//=============================================================================
/*!
 *  Import a shape from XAO Format
 *  \param fileName The name of the imported file
 *  \param shape The imported shape
 *  \param groups The list of imported groups
 *  \param fields The list of imported fields
 *  \return boolean indicating if import was succeful.
 */
//=============================================================================
//bool GEOMImpl_IImportExportOperations::ImportXAO(const std::string fileName,
//        Handle(GEOM_Object) shape,
//        std::list<Handle(GEOM_Object)> groupList,
//        std::list<Handle(GEOM_Object)> fieldList)
//{
//    SetErrorCode(KO);
//    return true;
//}


/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/
