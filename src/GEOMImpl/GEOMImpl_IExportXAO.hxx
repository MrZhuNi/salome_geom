//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#ifndef _GEOMImpl_IExportXAO_HXX_
#define _GEOMImpl_IExportXAO_HXX_

#include "GEOM_Function.hxx"

#define EXPORTXAO_ARG_EXPORTINGSHAPE   1
#define EXPORTXAO_ARG_FILENAME   2
#define EXPORTXAO_ARG_LGROUPS   3
#define EXPORTXAO_ARG_LFIELDS   4

class GEOMImpl_IExportXAO
{
public:
  GEOMImpl_IExportXAO(Handle(GEOM_Function) theFunction): _func(theFunction) {}

  void SetExportingShape(Handle(GEOM_Function) theExportingShape) { _func->SetReference(EXPORTXAO_ARG_EXPORTINGSHAPE, theExportingShape); }
  Handle(GEOM_Function) GetExportingShape() { return _func->GetReference(EXPORTXAO_ARG_EXPORTINGSHAPE); }

  void SetFileName(const TCollection_AsciiString& theFileName) { _func->SetString(EXPORTXAO_ARG_FILENAME, theFileName); }
  const TCollection_AsciiString GetFileName() { return _func->GetString(EXPORTXAO_ARG_FILENAME); }

  void SetGroup(int theId, Handle(GEOM_Function) theGroup) { _func->SetReference(EXPORTXAO_ARG_LGROUPS+theId, theGroup); }
  Handle(GEOM_Function) GetGroup(int theId) { return _func->GetReference(EXPORTXAO_ARG_LGROUPS+theId); }

  void SetField(int theId, Handle(GEOM_Function) theField) { _func->SetReference(EXPORTXAO_ARG_LFIELDS+theId, theField); }
  Handle(GEOM_Function) GetField(int theId) { return _func->GetReference(EXPORTXAO_ARG_LFIELDS+theId); }

private:
  Handle(GEOM_Function) _func;
};

#endif // _GEOMImpl_IExportXAO_HXX_
