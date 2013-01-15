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
// File:        GEOMAlgo_CheckerSI.cxx
// Created:
// Author:      Peter KURNEV
//              <pkv@irinox>
//

#include <GEOMAlgo_CheckerSI.hxx>
#include <BOPCol_BaseAllocator.hxx>
#include <BOPAlgo_PaveFiller.hxx>
#include <BOPDS_DS.hxx>
#include <GEOMAlgo_IteratorCheckerSI.hxx>
#include <BOPInt_Context.hxx>

//=======================================================================
//function : GEOMAlgo_CheckerSI
//purpose  : 
//=======================================================================
GEOMAlgo_CheckerSI::GEOMAlgo_CheckerSI()
:
  BOPAlgo_PaveFiller()
{
}
//=======================================================================
//function : GEOMAlgo_CheckerSI
//purpose  : 
//=======================================================================
GEOMAlgo_CheckerSI::GEOMAlgo_CheckerSI(const BOPCol_BaseAllocator& theAllocator)
:
  BOPAlgo_PaveFiller(theAllocator)
{
}
//=======================================================================
//function : ~
//purpose  : 
//=======================================================================
GEOMAlgo_CheckerSI::~GEOMAlgo_CheckerSI()
{
}
//=======================================================================
//function : Init
//purpose  : 
//=======================================================================
void GEOMAlgo_CheckerSI::Init()
{
  Standard_Integer aNb;
  //
  myErrorStatus=0;
  //
  aNb=myArguments.Extent();
  if (!aNb) {
    myErrorStatus=10;
    return;
  }
  if (aNb>1) {
    myErrorStatus=11;
    return;
  }
  //
  // 0 Clear
  Clear();
  //
  // 1.myDS 
  myDS=new BOPDS_DS(myAllocator);
  myDS->SetArguments(myArguments);
  myDS->Init();
  //
  // 2.myIterator 
  myIterator=new GEOMAlgo_IteratorCheckerSI(myAllocator);
  myIterator->SetDS(myDS);
  myIterator->Prepare();
  //
  // 3 myContext
  myContext=new BOPInt_Context;
  //
  myErrorStatus=0;
}
