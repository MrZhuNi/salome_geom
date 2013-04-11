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

class GEOMImpl_IShapesOperations;
class GEOMImpl_IGroupOperations;

class GEOMImpl_IImportExportOperations: public GEOM_IOperations
{

private:

    GEOMImpl_IShapesOperations* m_shapesOperations;
    GEOMImpl_IGroupOperations* m_groupOperations;

public:
    Standard_EXPORT GEOMImpl_IImportExportOperations(GEOM_Engine* engine, int docID);
    Standard_EXPORT ~GEOMImpl_IImportExportOperations();

    Standard_EXPORT bool ExportXAO (const std::string fileName,
            Handle(GEOM_Object) shape,
            std::list<Handle(GEOM_Object)> groupList,
            std::list<Handle(GEOM_Object)> fieldList);
//    Standard_EXPORT bool ImportXAO (const std::string fileName,
//            Handle(GEOM_Object) shape,
//            std::list<Handle(GEOM_Object)> groupList,
//            std::list<Handle(GEOM_Object)> fieldList);
    /*@@ insert new functions before this line @@ do not remove this line @@*/
};
#endif
