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
GEOM_IImportExportOperations_i::GEOM_IImportExportOperations_i(PortableServer::POA_ptr thePOA, GEOM::GEOM_Gen_ptr theEngine, ::GEOMImpl_IImportExportOperations* theImpl)
:GEOM_IOperations_i(thePOA, theEngine, theImpl)
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
 *  \param theExportingShape Shape to export
 *  \param theFileName The name of the exported file
 *  \param thelGroups List of groups to export
 *  \param thelFields List of fields to export
 *  \return New GEOM_Object, containing the created shape.
 */
//=============================================================================
CORBA::Boolean GEOM_IImportExportOperations_i::ExportXAO (GEOM::GEOM_Object_ptr theExportingShape, const char* theFileName, const GEOM::ListOfGO& thelGroups, const GEOM::ListOfGO& thelFields)
{
  bool isGood = false;
  //Set a not done flag
  GetOperations()->SetNotDone();
  
  // Get the reference shape
  Handle(GEOM_Object) aReference = GetObjectImpl(theExportingShape);
  
  //Get the reference groups
  int ind = 0;
  int aLenGroup = thelGroups.length();
  std::list<Handle(GEOM_Object)> aGroups;
  for (; ind < aLenGroup; ind++) {
    Handle(GEOM_Object) aGroup = GetObjectImpl(thelGroups[ind]);
    if (aGroup.IsNull()) return false;
    aGroups.push_back(aGroup);
  }
  
  //Get the reference point
  ind = 0;
  int aLenField = thelFields.length();
  std::list<Handle(GEOM_Object)> aFields;
  for (; ind < aLenField; ind++) {
    Handle(GEOM_Object) aField = GetObjectImpl(thelFields[ind]);
    if (aField.IsNull()) return false;
    aFields.push_back(aField);
  }
  
  if (!aReference.IsNull())
  {
    // Export XAO
    isGood = GetOperations()->ExportXAO(aReference, theFileName, aGroups, aFields);
  }

  return isGood;
}

/*@@ insert new functions before this line @@ do not remove this line @@*/
