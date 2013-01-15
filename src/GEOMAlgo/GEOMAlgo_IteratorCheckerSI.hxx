
// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
// File:        GEOMAlgo_CheckerSI.hxx
// Created:
// Author:      Peter KURNEV
//              <pkv@irinox>
//

#ifndef GEOMAlgo_IteratorCheckerSI_HeaderFile
#define GEOMAlgo_IteratorCheckerSI_HeaderFile

#include <Standard.hxx>
#include <Standard_Macro.hxx>
#include <BOPDS_Iterator.hxx>
#include <NCollection_BaseAllocator.hxx>

//=======================================================================
//class    : GEOMAlgo_CheckerSI
//purpose  : 
//=======================================================================
class GEOMAlgo_IteratorCheckerSI  : public BOPDS_Iterator {
 public:
  Standard_EXPORT
    GEOMAlgo_IteratorCheckerSI();

  Standard_EXPORT
    GEOMAlgo_IteratorCheckerSI(const Handle(NCollection_BaseAllocator)& theAllocator);
  
  Standard_EXPORT
    ~GEOMAlgo_IteratorCheckerSI();

  Standard_EXPORT
    virtual void Intersect();
  
};

#endif
