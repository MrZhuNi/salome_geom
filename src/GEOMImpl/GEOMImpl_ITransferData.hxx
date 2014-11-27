// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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

//NOTE: This is an intreface to a function for the Transfer Data functionality.
//
#include "GEOM_Function.hxx"

#define TD_ARG_REF1   1
#define TD_ARG_REF2   2
#define TD_ARG_METHOD 3

class GEOMImpl_ITransferData
{
 public:

  GEOMImpl_ITransferData(const Handle(GEOM_Function) &theFunction)
    : _func(theFunction) {}

  void SetRef1(const Handle(GEOM_Function) &theRefPoint1)
  { _func->SetReference(TD_ARG_REF1, theRefPoint1); }

  Handle(GEOM_Function) GetRef1() { return _func->GetReference(TD_ARG_REF1); }

  void SetRef2(const Handle(GEOM_Function) &theRefPoint2)
  { _func->SetReference(TD_ARG_REF2, theRefPoint2); }

  Handle(GEOM_Function) GetRef2() { return _func->GetReference(TD_ARG_REF2); }

  void SetFindMethod(const int theFindMethod)
  { _func->SetInteger(TD_ARG_METHOD, theFindMethod); }

  int GetFindMethod() { return _func->GetInteger(TD_ARG_METHOD); }

 private:

  Handle(GEOM_Function) _func;
};
