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
//  File   : GEOMImpl_IImportExportOperations.hxx
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)

#ifndef _GEOMImpl_IImportExportOperations_HXX_
#define _GEOMImpl_IImportExportOperations_HXX_

#include <Utils_SALOME_Exception.hxx>
#include "GEOM_IOperations.hxx"
#include "GEOM_Engine.hxx"
#include "GEOM_Object.hxx"

#include <list>

/*class GEOMImpl_IBasicOperations;
class GEOMImpl_IBooleanOperations;
class GEOMImpl_IShapesOperations;
class GEOMImpl_ITransformOperations;
class GEOMImpl_IBlocksOperations;
class GEOMImpl_I3DPrimOperations;
class GEOMImpl_ILocalOperations;
class GEOMImpl_IHealingOperations;*/
class GEOMImpl_IShapesOperations;
class GEOMImpl_IGroupOperations;

class GEOMImpl_IImportExportOperations: public GEOM_IOperations {

private:
  /*GEOMImpl_IBasicOperations*     myBasicOperations;
  GEOMImpl_IBooleanOperations*   myBooleanOperations;
  GEOMImpl_IShapesOperations*    myShapesOperations;
  GEOMImpl_ITransformOperations* myTransformOperations;
  GEOMImpl_IBlocksOperations*    myBlocksOperations;
  GEOMImpl_I3DPrimOperations*    my3DPrimOperations;
  GEOMImpl_ILocalOperations*     myLocalOperations;
  GEOMImpl_IHealingOperations*   myHealingOperations;*/
  GEOMImpl_IShapesOperations*    myShapesOperations;
  GEOMImpl_IGroupOperations*     myGroupOperations;
  
public:
  Standard_EXPORT GEOMImpl_IImportExportOperations(GEOM_Engine* theEngine, int theDocID);
  Standard_EXPORT ~GEOMImpl_IImportExportOperations();
  
  Standard_EXPORT bool ExportXAO (Handle(GEOM_Object) theExportingShape, const std::string theFileName, std::list<Handle(GEOM_Object)> thelGroups, std::list<Handle(GEOM_Object)> thelFields);
  /*@@ insert new functions before this line @@ do not remove this line @@*/
};
#endif
