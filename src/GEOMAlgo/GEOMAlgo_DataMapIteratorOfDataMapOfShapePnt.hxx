// Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef _GEOMAlgo_DataMapIteratorOfDataMapOfShapePnt_HeaderFile
#define _GEOMAlgo_DataMapIteratorOfDataMapOfShapePnt_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _TCollection_BasicMapIterator_HeaderFile
#include <TCollection_BasicMapIterator.hxx>
#endif
#ifndef _Handle_GEOMAlgo_DataMapNodeOfDataMapOfShapePnt_HeaderFile
#include <Handle_GEOMAlgo_DataMapNodeOfDataMapOfShapePnt.hxx>
#endif
class Standard_NoSuchObject;
class TopoDS_Shape;
class gp_Pnt;
class TopTools_ShapeMapHasher;
class GEOMAlgo_DataMapOfShapePnt;
class GEOMAlgo_DataMapNodeOfDataMapOfShapePnt;



class GEOMAlgo_DataMapIteratorOfDataMapOfShapePnt  : public TCollection_BasicMapIterator {
public:

  void* operator new(size_t,void* anAddress) 
  {
    return anAddress;
  }
  void* operator new(size_t size) 
  {
    return Standard::Allocate(size); 
  }
  void  operator delete(void *anAddress) 
  {
    if (anAddress) Standard::Free((Standard_Address&)anAddress); 
  }

  
  Standard_EXPORT   GEOMAlgo_DataMapIteratorOfDataMapOfShapePnt();
  
  Standard_EXPORT   GEOMAlgo_DataMapIteratorOfDataMapOfShapePnt(const GEOMAlgo_DataMapOfShapePnt& aMap);
  
  Standard_EXPORT     void Initialize(const GEOMAlgo_DataMapOfShapePnt& aMap) ;
  
  Standard_EXPORT    const TopoDS_Shape& Key() const;
  
  Standard_EXPORT    const gp_Pnt& Value() const;





protected:





private:





};





// other Inline functions and methods (like "C++: function call" methods)


#endif