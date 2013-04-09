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
    /*myBasicOperations     = new GEOMImpl_IBasicOperations(GetEngine(), GetDocID());
     myBooleanOperations   = new GEOMImpl_IBooleanOperations(GetEngine(), GetDocID());
     myShapesOperations    = new GEOMImpl_IShapesOperations(GetEngine(), GetDocID());
     myTransformOperations = new GEOMImpl_ITransformOperations(GetEngine(), GetDocID());
     myBlocksOperations    = new GEOMImpl_IBlocksOperations(GetEngine(), GetDocID());
     my3DPrimOperations    = new GEOMImpl_I3DPrimOperations(GetEngine(), GetDocID());
     myLocalOperations     = new GEOMImpl_ILocalOperations(GetEngine(), GetDocID());
     myHealingOperations   = new GEOMImpl_IHealingOperations(GetEngine(), GetDocID());*/
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
    /*delete myBasicOperations;
     delete myBooleanOperations;
     delete myShapesOperations;
     delete myTransformOperations;
     delete myBlocksOperations;
     delete my3DPrimOperations;
     delete myLocalOperations;
     delete myHealingOperations;*/
    delete m_shapesOperations;
    delete m_groupOperations;
}

//=============================================================================
/*!
 *  Export a shape to XAO Format
 *  \param shape The shape to export
 *  \param fileName The name of the exported file
 *  \param groupList The list of groups to export
 *  \param fieldList The list of fields to export
 *  \return New GEOM_Object, containing the created shape.
 */
//=============================================================================
bool GEOMImpl_IImportExportOperations::ExportXAO(Handle(GEOM_Object) shape,
        const std::string fileName,
        std::list<Handle(GEOM_Object)> groupList,
        std::list<Handle(GEOM_Object)> fieldList)
{
    std::cout << ">> ExportXAO" << std::endl;
    SetErrorCode(KO);
    bool isGood = false;

    //

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

    /*GEOMImpl_IExportXAO aData (aRefFunction);

     TCollection_AsciiString FileName = theFileName.c_str();
     aData.SetExportingShape(aRefFunction);
     aData.SetFileName(FileName);
     aData.SetlGroups(thelGroups);
     aData.SetlFields(thelFields);

     //Compute the resulting value
     try {
     #if (OCC_VERSION_MAJOR << 16 | OCC_VERSION_MINOR << 8 | OCC_VERSION_MAINTENANCE) > 0x060100
     OCC_CATCH_SIGNALS;
     #endif
     if (!GetSolver()->ComputeFunction(aFunction)) {
     SetErrorCode("ExportXAO driver failed : Not enough space on disk, or you haven't permissions to write this directory");
     return false;
     }
     }
     catch (Standard_Failure) {
     Handle(Standard_Failure) aFail = Standard_Failure::Caught();
     SetErrorCode(aFail->GetMessageString());
     return false;
     }*/
    XAO::Xao* xaoObject = new XAO::Xao();

    XAO::Geometry* geometry = new XAO::Geometry();
    TopoDS_Shape topoShape = shape->GetValue();
    std::ostringstream dumpStream;
    BRepTools::Dump(topoShape, dumpStream);
    geometry->setShape(dumpStream.str().c_str());
    geometry->setName(shape->GetName());

    Handle(TColStd_HSequenceOfTransient) subObjects = m_shapesOperations->GetExistingSubObjects(shape, false);
    int nbSubObjects = subObjects->Length();
    std::cout << "nb sub obj = " << nbSubObjects << std::endl;
    int nbVertices = 0, nbEdges = 0, nbFaces = 0, nbSolids = 0, nbGroups = 0;
    for (int i = 1; i <= nbSubObjects; i++)
    {
        Handle(Standard_Transient) transientSubObject = subObjects->Value(i);
        if (transientSubObject.IsNull())
            continue;

        Handle(GEOM_Object) subObject = Handle(GEOM_Object)::DownCast(transientSubObject);
        if (subObject->GetType() == GEOM_GROUP)
        {
            nbGroups++;
        }
        else
        {
            switch (subObject->GetValue().ShapeType())
            {
                case TopAbs_VERTEX: nbVertices++; break;
                case TopAbs_EDGE: nbEdges++; break;
                case TopAbs_FACE: nbFaces++; break;
                case TopAbs_SOLID: nbSolids++; break;
            }
        }
    }

    std::cout << "Nb = " << nbVertices << ", " << nbEdges << ", " << nbFaces << ", " << nbSolids << ", " << nbGroups << std::endl;
    if (nbVertices > 0) geometry->setCountVertices(nbVertices);
    if (nbEdges > 0) geometry->setCountEdges(nbEdges);
    if (nbFaces > 0) geometry->setCountFaces(nbFaces);
    if (nbSolids > 0) geometry->setCountSolids(nbSolids);

    int vInd = 0, eInd = 0, fInd = 0, sInd = 0;
    for (int i = 1; i <= nbSubObjects; i++)
    {
        Handle(Standard_Transient) transientSubObject = subObjects->Value(i);
        if (transientSubObject.IsNull())
            continue;

        Handle(GEOM_Object) subObject = Handle(GEOM_Object)::DownCast(transientSubObject);
        std::cout << " sub[" << i << "] N=" << subObject->GetName()
                //<< " E=" << subObject->GetEntry()
                //<< " D=" << subObject->GetDocID()
                << " T=" << subObject->GetType() << std::endl;
        if (subObject->GetType() != GEOM_GROUP)
        {
            int index = m_shapesOperations->GetSubShapeIndex(shape, subObject);
            const char* strIndex = XAO::XaoUtils::intToString(index);
            std::cout << "    index= " << index << std::endl;
            switch (subObject->GetValue().ShapeType())
            {
                case TopAbs_VERTEX:
                    geometry->setVertex(vInd++, subObject->GetName(), strIndex);
                    break;
                case TopAbs_EDGE:
                    geometry->setEdge(eInd++, subObject->GetName(), strIndex);
                    break;
                case TopAbs_FACE:
                    geometry->setFace(fInd++, subObject->GetName(), strIndex);
                    break;
                case TopAbs_SOLID:
                    geometry->setSolid(sInd++, subObject->GetName(), strIndex);
                    break;
            }
        }
    }
    xaoObject->setGeometry(geometry);

    xaoObject->exportToFile(fileName.c_str());

    /*
    // adding groups
    std::list<Handle(GEOM_Object)>::iterator itG1 = groupList.begin();
    int gInd = 0;
    while (itG1 != groupList.end())
    {
        std::cout << "Group = " << gInd++ << std::endl;
        Handle(GEOM_Object) itGroup = (*itG1++);
        XAO::Group* group = new XAO::Group();
        group->setName(itGroup->GetName());
        Handle(TColStd_HArray1OfInteger) groupIds = m_groupOperations->GetObjects(itGroup);
        TopAbs_ShapeEnum shapeGroup = m_groupOperations->GetType(itGroup);
        std::cout << "GroupShape = " << shapeGroup << std::endl;
        if (shapeGroup == TopAbs_VERTEX)
        {
            group->setDimension(0);
            for (int i = 1; i <= groupIds->Length(); i++)
            {
                const char* index = geometry->getVertexReference(groupIds->Value(i));
                group->addElement(index);
            }
        }
        xaoObject->addGroup(group);
        /*{
         case TopAbs_SOLID:
         Group->setType(3);
         break;
         case TopAbs_FACE:
         Group->setType(2);
         break;
         case TopAbs_EDGE:
         Group->setType(1);
         break;
         case TopAbs_VERTEX:
         Group->setType(0);
         break;
         }  *

    }*/

    /*
    // make a Python command
    TCollection_AsciiString fileNameStr = fileName.c_str();
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

    return isGood;
}
/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/
