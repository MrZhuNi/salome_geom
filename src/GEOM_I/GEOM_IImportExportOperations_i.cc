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

//  File   : GEOM_IImportExportOperations.cc
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
#include <Standard_Stream.hxx>

#include "GEOM_IImportExportOperations_i.hh"

#include <utilities.h>
#include <OpUtil.hxx>
#include <Utils_ExceptHandlers.hxx>

#include "GEOM_Engine.hxx"
#include "GEOM_Object.hxx"

//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================
GEOM_IImportExportOperations_i::GEOM_IImportExportOperations_i(PortableServer::POA_ptr thePOA,
        GEOM::GEOM_Gen_ptr theEngine, ::GEOMImpl_IImportExportOperations* theImpl)
:
        GEOM_IOperations_i(thePOA, theEngine, theImpl)
{
    MESSAGE("GEOM_IImportExportOperations_i::GEOM_IImportExportOperations_i");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================
GEOM_IImportExportOperations_i::~GEOM_IImportExportOperations_i()
{
    MESSAGE("GEOM_IImportExportOperations_i::~GEOM_IImportExportOperations_i");
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
CORBA::Boolean GEOM_IImportExportOperations_i::ExportXAO(GEOM::GEOM_Object_ptr shape,
        const GEOM::ListOfGO& groups, const GEOM::ListOfGO& fields, CORBA::String_out xao)
{
    bool isGood = false;
    // Set a not done flag
    GetOperations()->SetNotDone();

    // Get the reference shape
    Handle(GEOM_Object) reference = GetObjectImpl(shape);

    // Get the reference groups
    int ind = 0;
    std::list<Handle(GEOM_Object)> groupsObj;
    for (; ind < groups.length(); ind++)
    {
        Handle(GEOM_Object) gobj = GetObjectImpl(groups[ind]);
        if (gobj.IsNull()) return false;
        groupsObj.push_back(gobj);
    }

    // Get the reference fields
    ind = 0;
    std::list<Handle(GEOM_Object)> fieldsObj;
    for (; ind < fields.length(); ind++)
    {
        Handle(GEOM_Object) fobj = GetObjectImpl(fields[ind]);
        if (fobj.IsNull()) return false;
        fieldsObj.push_back(fobj);
    }

    if (!reference.IsNull())
    {
        // Export XAO
        char* data;
        isGood = GetOperations()->ExportXAO(reference, groupsObj, fieldsObj, data);
        xao = CORBA::string_dup(data);
        delete data;
    }

    return isGood;
}

//=============================================================================
/*!
 *  Import a shape from XAO Format
 *  \param xao The XAO data to import
 *  \param shape The imported shape
 *  \param groups The list of imported groups
 *  \param fields The list of imported fields
 *  \return boolean indicating if import was succeful.
 */
//=============================================================================
//bool GEOMImpl_IImportExportOperations::ImportXAO(const std::xao fileName,
//        Handle(GEOM_Object),
//        std::list<Handle_GEOM_Object, std::allocator<Handle_GEOM_Object> > groups,
//        std::list<Handle_GEOM_Object, std::allocator<Handle_GEOM_Object> > fields)
//{
//    return true;
//}

/*@@ insert new functions before this line @@ do not remove this line @@*/
