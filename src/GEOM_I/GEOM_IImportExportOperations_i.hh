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

//  File   : GEOM_IImportExportOperations.hh
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
#ifndef _GEOM_IImportExportOperations_i_HeaderFile
#define _GEOM_IImportExportOperations_i_HeaderFile

#include "GEOMImpl_Gen.hxx"

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(GEOM_Gen)
#include "GEOM_IOperations_i.hh"
#include "GEOM_Object_i.hh"

#include "GEOMImpl_IImportExportOperations.hxx"

class GEOM_I_EXPORT GEOM_IImportExportOperations_i :
    public virtual POA_GEOM::GEOM_IImportExportOperations,
    public virtual GEOM_IOperations_i
{
 public:
  GEOM_IImportExportOperations_i (PortableServer::POA_ptr thePOA, GEOM::GEOM_Gen_ptr theEngine,
			    ::GEOMImpl_IImportExportOperations* theImpl);
  ~GEOM_IImportExportOperations_i();

  CORBA::Boolean ExportXAO (GEOM::GEOM_Object_ptr theExportingShape, const char* theFileName, const GEOM::ListOfGO& thelGroups, const GEOM::ListOfGO& thelFields);
  /*@@ insert new functions before this line @@ do not remove this line @@*/

  ::GEOMImpl_IImportExportOperations* GetOperations()
  { return (::GEOMImpl_IImportExportOperations*)GetImpl(); }
};

#endif
