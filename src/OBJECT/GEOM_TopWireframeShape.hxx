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

//  GEOM OBJECT : interactive object for Geometry entities visualization
//  File   : GEOM_TopWireframeShape.hxx
//  Module : GEOM
//
#ifndef _GEOM_TopWireframeShape_HeaderFile
#define _GEOM_TopWireframeShape_HeaderFile

#include "GEOM_OBJECT_defs.hxx"

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_GEOM_TopWireframeShape_HeaderFile
#include "Handle_GEOM_TopWireframeShape.hxx"
#endif

#ifndef _SALOME_AISShape_HeaderFile
#include "SALOME_AISShape.hxx"
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif

class GEOM_OBJECT_EXPORT GEOM_TopWireframeShape : public SALOME_AISShape {

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

    // Methods PUBLIC
    // 
    GEOM_TopWireframeShape(const TopoDS_Shape& shape);
    virtual  Handle_SALOME_InteractiveObject getIO();
    virtual  Standard_Boolean hasIO();
    virtual  Standard_Boolean isTopLevel();
    virtual  Standard_Boolean switchTopLevel();
    virtual  Standard_Boolean toActivate();
    virtual  void setTopLevel(Standard_Boolean);
    virtual  Standard_CString getName();
    virtual  void setName(const Standard_CString aName);
    virtual  void highlightSubShapes(const NCollection_IndexedMap<Standard_Integer>& aIndexMap, const Standard_Boolean aHighlight );
    ~GEOM_TopWireframeShape();

    // Type management
    //
    friend Handle_Standard_Type& GEOM_TopWireframeShape_Type_();
    const Handle(Standard_Type)& DynamicType() const;
    Standard_Boolean             IsKind(const Handle(Standard_Type)&) const;


        void setIO(const Handle(SALOME_InteractiveObject)& io);

protected:

    // Methods PROTECTED
    // 

    // Fields PROTECTED
    //

private: 

    // Methods PRIVATE
    // 

    // Fields PRIVATE
    //
};

// other inline functions and methods (like "C++: function call" methods)
//

#endif
