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

#ifndef _NMTDS_ListIteratorOfListOfPairBoolean_HeaderFile
#define _NMTDS_ListIteratorOfListOfPairBoolean_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Handle_NMTDS_ListNodeOfListOfPairBoolean_HeaderFile
#include <Handle_NMTDS_ListNodeOfListOfPairBoolean.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoMoreObject;
class Standard_NoSuchObject;
class NMTDS_ListOfPairBoolean;
class NMTDS_PairBoolean;
class NMTDS_ListNodeOfListOfPairBoolean;



class NMTDS_ListIteratorOfListOfPairBoolean  {
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

  
  Standard_EXPORT   NMTDS_ListIteratorOfListOfPairBoolean();
  
  Standard_EXPORT   NMTDS_ListIteratorOfListOfPairBoolean(const NMTDS_ListOfPairBoolean& L);
  
  Standard_EXPORT     void Initialize(const NMTDS_ListOfPairBoolean& L) ;
  
        Standard_Boolean More() const;
  
  Standard_EXPORT     void Next() ;
  
  Standard_EXPORT     NMTDS_PairBoolean& Value() const;


friend class NMTDS_ListOfPairBoolean;



protected:





private:



Standard_Address current;
Standard_Address previous;


};

#define Item NMTDS_PairBoolean
#define Item_hxx <NMTDS_PairBoolean.hxx>
#define TCollection_ListNode NMTDS_ListNodeOfListOfPairBoolean
#define TCollection_ListNode_hxx <NMTDS_ListNodeOfListOfPairBoolean.hxx>
#define TCollection_ListIterator NMTDS_ListIteratorOfListOfPairBoolean
#define TCollection_ListIterator_hxx <NMTDS_ListIteratorOfListOfPairBoolean.hxx>
#define Handle_TCollection_ListNode Handle_NMTDS_ListNodeOfListOfPairBoolean
#define TCollection_ListNode_Type_() NMTDS_ListNodeOfListOfPairBoolean_Type_()
#define TCollection_List NMTDS_ListOfPairBoolean
#define TCollection_List_hxx <NMTDS_ListOfPairBoolean.hxx>

#include <TCollection_ListIterator.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx


// other Inline functions and methods (like "C++: function call" methods)


#endif