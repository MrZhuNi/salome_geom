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

#ifndef _NMTDS_ListOfPairBoolean_HeaderFile
#define _NMTDS_ListOfPairBoolean_HeaderFile

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
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoSuchObject;
class NMTDS_ListIteratorOfListOfPairBoolean;
class NMTDS_PairBoolean;
class NMTDS_ListNodeOfListOfPairBoolean;



class NMTDS_ListOfPairBoolean  {
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

  
  Standard_EXPORT   NMTDS_ListOfPairBoolean();
  
  Standard_EXPORT     void Assign(const NMTDS_ListOfPairBoolean& Other) ;
    void operator=(const NMTDS_ListOfPairBoolean& Other) 
{
  Assign(Other);
}
  
  Standard_EXPORT     Standard_Integer Extent() const;
  
  Standard_EXPORT     void Clear() ;
~NMTDS_ListOfPairBoolean()
{
  Clear();
}
  
        Standard_Boolean IsEmpty() const;
  
  Standard_EXPORT     void Prepend(const NMTDS_PairBoolean& I) ;
  
  Standard_EXPORT     void Prepend(const NMTDS_PairBoolean& I,NMTDS_ListIteratorOfListOfPairBoolean& theIt) ;
  
  Standard_EXPORT     void Prepend(NMTDS_ListOfPairBoolean& Other) ;
  
  Standard_EXPORT     void Append(const NMTDS_PairBoolean& I) ;
  
  Standard_EXPORT     void Append(const NMTDS_PairBoolean& I,NMTDS_ListIteratorOfListOfPairBoolean& theIt) ;
  
  Standard_EXPORT     void Append(NMTDS_ListOfPairBoolean& Other) ;
  
  Standard_EXPORT     NMTDS_PairBoolean& First() const;
  
  Standard_EXPORT     NMTDS_PairBoolean& Last() const;
  
  Standard_EXPORT     void RemoveFirst() ;
  
  Standard_EXPORT     void Remove(NMTDS_ListIteratorOfListOfPairBoolean& It) ;
  
  Standard_EXPORT     void InsertBefore(const NMTDS_PairBoolean& I,NMTDS_ListIteratorOfListOfPairBoolean& It) ;
  
  Standard_EXPORT     void InsertBefore(NMTDS_ListOfPairBoolean& Other,NMTDS_ListIteratorOfListOfPairBoolean& It) ;
  
  Standard_EXPORT     void InsertAfter(const NMTDS_PairBoolean& I,NMTDS_ListIteratorOfListOfPairBoolean& It) ;
  
  Standard_EXPORT     void InsertAfter(NMTDS_ListOfPairBoolean& Other,NMTDS_ListIteratorOfListOfPairBoolean& It) ;


friend class NMTDS_ListIteratorOfListOfPairBoolean;



protected:





private:

  
  Standard_EXPORT   NMTDS_ListOfPairBoolean(const NMTDS_ListOfPairBoolean& Other);


Standard_Address myFirst;
Standard_Address myLast;


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

#include <TCollection_List.lxx>

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