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

#include <GEOMImpl_ExportXAODriver.hxx>
#include <GEOMImpl_IExportXAO.hxx>
/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <TCollection_AsciiString.hxx>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax3.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRep_Tool.hxx>
#include <cmath>

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
GEOMImpl_IImportExportOperations::GEOMImpl_IImportExportOperations(GEOM_Engine* theEngine, int theDocID) :
  GEOM_IOperations(theEngine, theDocID)
{
  MESSAGE("GEOMImpl_IImportExportOperations::GEOMImpl_IImportExportOperations");
  myBasicOperations     = new GEOMImpl_IBasicOperations(GetEngine(), GetDocID());
  myBooleanOperations   = new GEOMImpl_IBooleanOperations(GetEngine(), GetDocID());
  myShapesOperations    = new GEOMImpl_IShapesOperations(GetEngine(), GetDocID());
  myTransformOperations = new GEOMImpl_ITransformOperations(GetEngine(), GetDocID());
  myBlocksOperations    = new GEOMImpl_IBlocksOperations(GetEngine(), GetDocID());
  my3DPrimOperations    = new GEOMImpl_I3DPrimOperations(GetEngine(), GetDocID());
  myLocalOperations     = new GEOMImpl_ILocalOperations(GetEngine(), GetDocID());
  myHealingOperations   = new GEOMImpl_IHealingOperations(GetEngine(), GetDocID());
}

//=============================================================================
/*!
 *  Destructor
 */
//=============================================================================
GEOMImpl_IImportExportOperations::~GEOMImpl_IImportExportOperations()
{
  MESSAGE("GEOMImpl_IImportExportOperations::~GEOMImpl_IImportExportOperations");
  delete myBasicOperations;
  delete myBooleanOperations;
  delete myShapesOperations;
  delete myTransformOperations;
  delete myBlocksOperations;
  delete my3DPrimOperations;
  delete myLocalOperations;
  delete myHealingOperations;
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
bool GEOMImpl_IImportExportOperations::ExportXAO (Handle(GEOM_Object) theExportingShape, const std::string theFileName, std::list<Handle(GEOM_Object)> thelGroups, std::list<Handle(GEOM_Object)> thelFields)
{
  SetErrorCode(KO);
  bool isGood = false;

  //Add a new shape function with parameters
  Handle(GEOM_Function) aRefFunction = theExportingShape->GetLastFunction();
  if (aRefFunction.IsNull()) return false;

  //Add a new result object
  Handle(GEOM_Object) result = GetEngine()->AddObject(GetDocID(), GEOM_IMPORT);
  
   //Add an Export function
  Handle(GEOM_Function) aFunction = result->AddFunction(GEOMImpl_ExportXAODriver::GetID(), EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS);
  if (aFunction.IsNull()) return false;
  
 //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_ExportXAODriver::GetID()) return false;

  GEOMImpl_IExportXAO aData (aRefFunction);

  TCollection_AsciiString FileName = theFileName.c_str();
  aData.SetExportingShape(aRefFunction);
  aData.SetFileName(FileName);
  /* A MODIFIER aData.SetlGroups(thelGroups);
  aData.SetlFields(thelFields);*/

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
  }

  //Make a Python command
  GEOM::TPythonDump pd (aFunction);
  std::list<Handle(GEOM_Object)>::iterator itG = thelGroups.begin();
  std::list<Handle(GEOM_Object)>::iterator itF = thelFields.begin();
  pd << /*isGood <<*/ " = geompy.ExportXAO(" << theExportingShape << ", " << FileName.ToCString() << ", [";
  //itG = thePoints.begin();
  pd << (*itG++);
  while (itG != thelGroups.end()) {
    pd << ", " << (*itG++);
  }
  pd << "], [";
  pd << (*itF++);
  while (itF != thelFields.end()) {
    pd << ", " << (*itF++);
  }
  pd << "])";

  SetErrorCode(OK);

  return isGood;
}
/*@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@*/
