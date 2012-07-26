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

//  File   : GEOM_IAdvancedOperations.hh
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
#ifndef _GEOM_IAdvancedOperations_i_HeaderFile
#define _GEOM_IAdvancedOperations_i_HeaderFile

#include "GEOMImpl_Gen.hxx"

#include <SALOMEconfig.h>

#include CORBA_SERVER_HEADER(GEOM_Gen)
#include "GEOM_IOperations_i.hh"
#include "GEOM_Object_i.hh"

#include "GEOMImpl_IAdvancedOperations.hxx"

class GEOM_I_EXPORT GEOM_IAdvancedOperations_i :
    public virtual POA_GEOM::GEOM_IAdvancedOperations,
    public virtual GEOM_IOperations_i
{
 public:
  GEOM_IAdvancedOperations_i (PortableServer::POA_ptr thePOA, GEOM::GEOM_Gen_ptr theEngine,
			    ::GEOMImpl_IAdvancedOperations* theImpl);
  ~GEOM_IAdvancedOperations_i();

  GEOM::ListOfGO* MakePipeTShape (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
				  CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2,
				  CORBA::Boolean theHexMesh);
  GEOM::ListOfGO* MakePipeTShapeWithPosition (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
					      CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2,
					      CORBA::Boolean theHexMesh,
					      GEOM::GEOM_Object_ptr theP1, GEOM::GEOM_Object_ptr theP2, GEOM::GEOM_Object_ptr theP3);
  GEOM::ListOfGO* MakePipeTShapeChamfer (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
					 CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2,
					 CORBA::Double theH, CORBA::Double theW, CORBA::Boolean theHexMesh);
  GEOM::ListOfGO* MakePipeTShapeChamferWithPosition (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
						     CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2,
						     CORBA::Double theH, CORBA::Double theW, CORBA::Boolean theHexMesh,
						     GEOM::GEOM_Object_ptr theP1, GEOM::GEOM_Object_ptr theP2, GEOM::GEOM_Object_ptr theP3);
  GEOM::ListOfGO* MakePipeTShapeFillet (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
				        CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2, 
				        CORBA::Double theRF, CORBA::Boolean theHexMesh);
  GEOM::ListOfGO* MakePipeTShapeFilletWithPosition (CORBA::Double theR1, CORBA::Double theW1, CORBA::Double theL1, 
						    CORBA::Double theR2, CORBA::Double theW2, CORBA::Double theL2, 
						    CORBA::Double theRF, CORBA::Boolean theHexMesh,
						    GEOM::GEOM_Object_ptr theP1, GEOM::GEOM_Object_ptr theP2, GEOM::GEOM_Object_ptr theP3);
  
  GEOM::GEOM_Object_ptr MakeDividedDisk        (CORBA::Double theR, 
                                                CORBA::Double theRatio, 
                                                CORBA::Short theOrientation,
                                                GEOM::pattern thePattern);
  
  GEOM::GEOM_Object_ptr MakeDividedDiskPntVecR (GEOM::GEOM_Object_ptr theCenter,
                                                GEOM::GEOM_Object_ptr theVector,
                                                CORBA::Double theR,
                                                CORBA::Double theRatio,
                                                GEOM::pattern thePattern);
  
  GEOM::GEOM_Object_ptr MakeDividedCylinder (CORBA::Double theR, CORBA::Double theH);
  /*@@ insert new functions before this line @@ do not remove this line @@*/

  ::GEOMImpl_IAdvancedOperations* GetOperations()
  { return (::GEOMImpl_IAdvancedOperations*)GetImpl(); }
};

#endif
