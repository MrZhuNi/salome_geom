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

//  GEOM OBJECT : interactive object for Geometry entities visualization
//  File   : Handle_GEOM_TopWireframeShape.hxx
//  Module : GEOM
//
#ifndef _Handle_GEOM_TopWireframeShape_HeaderFile
#define _Handle_GEOM_TopWireframeShape_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_SALOME_AISShape_HeaderFile
#include "Handle_SALOME_AISShape.hxx"
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(SALOME_AISShape);
class GEOM_TopWireframeShape;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(GEOM_TopWireframeShape);

class Handle(GEOM_TopWireframeShape) : public Handle(SALOME_AISShape) {
  public:
    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
//    inline void  operator delete(void *anAddress, size_t size) 
//      { 
//        if (anAddress) Standard::Free((Standard_Address&)anAddress,size); 
//      }
    Handle(GEOM_TopWireframeShape)():Handle(SALOME_AISShape)() {} 
    Handle(GEOM_TopWireframeShape)(const Handle(GEOM_TopWireframeShape)& aHandle) : Handle(SALOME_AISShape)(aHandle) 
     {
     }

    Handle(GEOM_TopWireframeShape)(const GEOM_TopWireframeShape* anItem) : Handle(SALOME_AISShape)((SALOME_AISShape *)anItem) 
     {
     }

    Handle(GEOM_TopWireframeShape)& operator=(const Handle(GEOM_TopWireframeShape)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(GEOM_TopWireframeShape)& operator=(const GEOM_TopWireframeShape* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    GEOM_TopWireframeShape* operator->() 
     {
      return (GEOM_TopWireframeShape *)ControlAccess();
     }

    GEOM_TopWireframeShape* operator->() const 
     {
      return (GEOM_TopWireframeShape *)ControlAccess();
     }

   Standard_EXPORT ~Handle(GEOM_TopWireframeShape)();
 
   Standard_EXPORT static const Handle(GEOM_TopWireframeShape) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
