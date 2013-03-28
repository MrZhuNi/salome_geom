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

#include <Standard_Stream.hxx>

#include <GEOMImpl_ExportXAODriver.hxx>
#include <GEOMImpl_IExportXAO.hxx>
#include <GEOMImpl_Types.hxx>
#include <GEOM_Function.hxx>

#include <TFunction_Logbook.hxx>
#include <StdFail_NotDone.hxx>

//@@ include required header files here @@//

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& GEOMImpl_ExportXAODriver::GetID()
{
  static Standard_GUID aGUID("1C3A0F3F-729D-4E83-8232-78E74FC5637C");
  return aGUID;
}

//=======================================================================
//function : GEOMImpl_ExportXAODriver
//purpose  :
//=======================================================================
GEOMImpl_ExportXAODriver::GEOMImpl_ExportXAODriver()
{
}

//=======================================================================
//function : Execute
//purpose  :
//=======================================================================
Standard_Integer GEOMImpl_ExportXAODriver::Execute(TFunction_Logbook& log) const
{
  if (Label().IsNull()) return 0;
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());

  GEOMImpl_IExportXAO aData (aFunction);
  Standard_Integer aType = aFunction->GetType();

  TopoDS_Shape aShape;

  if (aType == EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS) {
    //@@ add implementation of the operation driver here to initialize aShape variable @@//
    //aShape = @...@
  }
  else {
    // other construction modes here
  }

  if (aShape.IsNull()) return 0;

  aFunction->SetValue(aShape);

  log.SetTouched(Label());

  return 1;
}

//=======================================================================
//function :  GEOMImpl_ExportXAODriver_Type_
//purpose  :
//=======================================================================
Standard_EXPORT Handle_Standard_Type& GEOMImpl_ExportXAODriver_Type_()
{
  static Handle_Standard_Type aType1 = STANDARD_TYPE(TFunction_Driver);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TFunction_Driver);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("GEOMImpl_ExportXAODriver",
                                                         sizeof(GEOMImpl_ExportXAODriver),
                                                         1,
                                                         (Standard_Address)_Ancestors,
                                                         (Standard_Address)NULL);
  return _aType;
}

//=======================================================================
//function : DownCast
//purpose  :
//=======================================================================
const Handle(GEOMImpl_ExportXAODriver) Handle(GEOMImpl_ExportXAODriver)::DownCast(const Handle(Standard_Transient)& AnObject)
{
  Handle(GEOMImpl_ExportXAODriver) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(GEOMImpl_ExportXAODriver))) {
       _anOtherObject = Handle(GEOMImpl_ExportXAODriver)((Handle(GEOMImpl_ExportXAODriver)&)AnObject);
     }
  }

  return _anOtherObject;
}
