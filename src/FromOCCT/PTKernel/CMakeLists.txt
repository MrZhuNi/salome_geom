# Copyright (C) 2012-2015  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

# --- options ---

# additional include directories
INCLUDE_DIRECTORIES(
  ${CAS_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  ${CAS_KERNEL}
  )

# --- headers ---

SET(GEOM_HEADERS
  DBC_BaseArray.hxx                                                                                                        
  DBC_BaseArray.ixx                                                                                                        
  DBC_BaseArray.jxx                                                                                                        
  DBC_BaseArray.lxx                                                                                                        
  DBC_BaseArray_objs.hxx                                                                                                   
  DBC_DBVArray.hxx                                                                                                         
  DBC_VArray.gxx                                                                                                           
  DBC_VArrayNode.gxx                                                                                                       
  DBC_VArrayNodeOfVArrayOfCharacter.hxx                                                                                    
  DBC_VArrayNodeOfVArrayOfExtCharacter.hxx                                                                                 
  DBC_VArrayNodeOfVArrayOfInteger.hxx                                                                                      
  DBC_VArrayNodeOfVArrayOfReal.hxx                                                                                         
  DBC_VArrayOfCharacter.hxx                                                                                                
  DBC_VArrayOfExtCharacter.hxx                                                                                             
  DBC_VArrayOfInteger.hxx                                                                                                  
  DBC_VArrayOfReal.hxx                                                                                                     
  DBC_VArrayTNode.gxx                                                                                                      
  DBC_VArrayTNode.lxx                                                                                                      
  DBC_VArrayTNodeOfVArrayOfCharacter.hxx                                                                                   
  DBC_VArrayTNodeOfVArrayOfExtCharacter.hxx                                                                                
  DBC_VArrayTNodeOfVArrayOfInteger.hxx                                                                                     
  DBC_VArrayTNodeOfVArrayOfReal.hxx                                                                                        
  Handle_DBC_VArrayNodeOfVArrayOfCharacter.hxx                                                                             
  Handle_DBC_VArrayNodeOfVArrayOfExtCharacter.hxx                                                                          
  Handle_DBC_VArrayNodeOfVArrayOfInteger.hxx                                                                               
  Handle_DBC_VArrayNodeOfVArrayOfReal.hxx                                                                                  
  Handle_ObjMgt_ExternRef.hxx                                                                                              
  Handle_ObjMgt_ExternShareable.hxx                                                                                        
  Handle_ObjMgt_PSeqOfExtRef.hxx                                                                                           
  Handle_ObjMgt_SeqNodeOfPSeqOfExtRef.hxx                                                                                  
  Handle_PCollection_HAsciiString.hxx                                                                                      
  Handle_PCollection_HExtendedString.hxx                                                                                   
  Handle_PColStd_HArray1OfExtendedString.hxx                                                                               
  Handle_PColStd_HArray1OfInteger.hxx                                                                                      
  Handle_PColStd_HArray1OfPersistent.hxx                                                                                   
  Handle_PColStd_HArray1OfReal.hxx                                                                                         
  Handle_PColStd_HArray2OfInteger.hxx                                                                                      
  Handle_PColStd_HArray2OfPersistent.hxx                                                                                   
  Handle_PColStd_HArray2OfReal.hxx                                                                                         
  Handle_PColStd_VArrayNodeOfFieldOfHArray1OfExtendedString.hxx                                                            
  Handle_PColStd_VArrayNodeOfFieldOfHArray1OfInteger.hxx                                                                   
  Handle_PColStd_VArrayNodeOfFieldOfHArray1OfPersistent.hxx                                                                
  Handle_PColStd_VArrayNodeOfFieldOfHArray1OfReal.hxx                                                                      
  Handle_PColStd_VArrayNodeOfFieldOfHArray2OfInteger.hxx                                                                   
  Handle_PColStd_VArrayNodeOfFieldOfHArray2OfPersistent.hxx                                                                
  Handle_PColStd_VArrayNodeOfFieldOfHArray2OfReal.hxx                                                                      
  Handle_PMMgt_PManaged.hxx                                                                                                
  Handle_PShort_HArray1OfShortReal.hxx                                                                                     
  Handle_PShort_HArray2OfShortReal.hxx                                                                                     
  Handle_PShort_HSequenceOfShortReal.hxx                                                                                   
  Handle_PShort_SeqNodeOfHSequenceOfShortReal.hxx                                                                          
  Handle_PShort_VArrayNodeOfFieldOfHArray1OfShortReal.hxx                                                                  
  Handle_PShort_VArrayNodeOfFieldOfHArray2OfShortReal.hxx                                                                  
  Handle_PStandard_ArrayNode.hxx                                                                                           
  Handle_PTColStd_DataMapNodeOfPersistentTransientMap.hxx                                                                  
  Handle_PTColStd_DataMapNodeOfTransientPersistentMap.hxx                                                                  
  Handle_PTColStd_DoubleMapNodeOfDoubleMapOfTransientPersistent.hxx                                                        
  ObjMgt_ExternRef.hxx                                                                                                     
  ObjMgt_ExternRef.ixx                                                                                                     
  ObjMgt_ExternRef.jxx                                                                                                     
  ObjMgt_ExternShareable.hxx                                                                                               
  ObjMgt_ExternShareable.ixx                                                                                               
  ObjMgt_ExternShareable.jxx                                                                                               
  ObjMgt_PSeqOfExtRef.hxx                                                                                                  
  ObjMgt_SeqExplorerOfPSeqOfExtRef.hxx                                                                                     
  ObjMgt_SeqNodeOfPSeqOfExtRef.hxx                                                                                         
  PCollection_AccessMode.hxx                                                                                               
  PCollection_HArray1.gxx                                                                                                  
  PCollection_HArray1.lxx                                                                                                  
  PCollection_HArray2.gxx                                                                                                  
  PCollection_HArray2.lxx                                                                                                  
  PCollection_HAsciiString.hxx                                                                                             
  PCollection_HAsciiString.ixx                                                                                             
  PCollection_HAsciiString.jxx                                                                                             
  PCollection_HExtendedString.hxx                                                                                          
  PCollection_HExtendedString.ixx                                                                                          
  PCollection_HExtendedString.jxx                                                                                          
  PCollection_HSequence.gxx                                                                                                
  PCollection_SeqExplorer.gxx                                                                                              
  PCollection_SeqNode.gxx                                                                                                  
  PColStd_FieldOfHArray1OfExtendedString.hxx                                                                               
  PColStd_FieldOfHArray1OfInteger.hxx                                                                                      
  PColStd_FieldOfHArray1OfPersistent.hxx                                                                                   
  PColStd_FieldOfHArray1OfReal.hxx                                                                                         
  PColStd_FieldOfHArray2OfInteger.hxx                                                                                      
  PColStd_FieldOfHArray2OfPersistent.hxx                                                                                   
  PColStd_FieldOfHArray2OfReal.hxx                                                                                         
  PColStd_HArray1OfExtendedString.hxx                                                                                      
  PColStd_HArray1OfInteger.hxx                                                                                             
  PColStd_HArray1OfPersistent.hxx                                                                                          
  PColStd_HArray1OfReal.hxx                                                                                                
  PColStd_HArray2OfInteger.hxx                                                                                             
  PColStd_HArray2OfPersistent.hxx                                                                                          
  PColStd_HArray2OfReal.hxx                                                                                                
  PColStd_VArrayNodeOfFieldOfHArray1OfExtendedString.hxx                                                                   
  PColStd_VArrayNodeOfFieldOfHArray1OfInteger.hxx                                                                          
  PColStd_VArrayNodeOfFieldOfHArray1OfPersistent.hxx                                                                       
  PColStd_VArrayNodeOfFieldOfHArray1OfReal.hxx                                                                             
  PColStd_VArrayNodeOfFieldOfHArray2OfInteger.hxx                                                                          
  PColStd_VArrayNodeOfFieldOfHArray2OfPersistent.hxx
  PColStd_VArrayNodeOfFieldOfHArray2OfReal.hxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfExtendedString.hxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfInteger.hxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfPersistent.hxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfReal.hxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfInteger.hxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfPersistent.hxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfReal.hxx
  PMMgt_PManaged.hxx
  PMMgt_PManaged.ixx
  PMMgt_PManaged.jxx
  PShort_FieldOfHArray1OfShortReal.hxx
  PShort_FieldOfHArray2OfShortReal.hxx
  PShort_HArray1OfShortReal.hxx
  PShort_HArray2OfShortReal.hxx
  PShort_HSequenceOfShortReal.hxx
  PShort_SeqExplorerOfHSequenceOfShortReal.hxx
  PShort_SeqNodeOfHSequenceOfShortReal.hxx
  PShort_VArrayNodeOfFieldOfHArray1OfShortReal.hxx
  PShort_VArrayNodeOfFieldOfHArray2OfShortReal.hxx
  PShort_VArrayTNodeOfFieldOfHArray1OfShortReal.hxx
  PShort_VArrayTNodeOfFieldOfHArray2OfShortReal.hxx
  PStandard_ArrayNode.hxx
  PStandard_ArrayNode.ixx
  PStandard_ArrayNode.jxx
  PTColStd_DataMapIteratorOfPersistentTransientMap.hxx
  PTColStd_DataMapIteratorOfTransientPersistentMap.hxx
  PTColStd_DataMapNodeOfPersistentTransientMap.hxx
  PTColStd_DataMapNodeOfTransientPersistentMap.hxx
  PTColStd_DoubleMapIteratorOfDoubleMapOfTransientPersistent.hxx
  PTColStd_DoubleMapNodeOfDoubleMapOfTransientPersistent.hxx
  PTColStd_DoubleMapOfTransientPersistent.hxx
  PTColStd_MapPersistentHasher.hxx
  PTColStd_PersistentTransientMap.hxx
  PTColStd_TransientPersistentMap.hxx
  )

# --- sources ---

SET(GEOM_SOURCES
  DBC_BaseArray.cxx                                  
  DBC_DBVArray.cxx                                   
  DBC_VArrayNodeOfVArrayOfCharacter_0.cxx            
  DBC_VArrayNodeOfVArrayOfExtCharacter_0.cxx         
  DBC_VArrayNodeOfVArrayOfInteger_0.cxx              
  DBC_VArrayNodeOfVArrayOfReal_0.cxx                 
  DBC_VArrayOfCharacter_0.cxx                        
  DBC_VArrayOfExtCharacter_0.cxx                     
  DBC_VArrayOfInteger_0.cxx                          
  DBC_VArrayOfReal_0.cxx                             
  DBC_VArrayTNodeOfVArrayOfCharacter_0.cxx           
  DBC_VArrayTNodeOfVArrayOfExtCharacter_0.cxx        
  DBC_VArrayTNodeOfVArrayOfInteger_0.cxx             
  DBC_VArrayTNodeOfVArrayOfReal_0.cxx                
  ObjMgt_ExternRef.cxx                               
  ObjMgt_ExternShareable.cxx                         
  ObjMgt_PSeqOfExtRef_0.cxx                          
  ObjMgt_SeqExplorerOfPSeqOfExtRef_0.cxx             
  ObjMgt_SeqNodeOfPSeqOfExtRef_0.cxx                 
  PCollection_HAsciiString.cxx                       
  PCollection_HExtendedString.cxx                    
  PColStd_FieldOfHArray1OfExtendedString_0.cxx       
  PColStd_FieldOfHArray1OfInteger_0.cxx              
  PColStd_FieldOfHArray1OfPersistent_0.cxx           
  PColStd_FieldOfHArray1OfReal_0.cxx                 
  PColStd_FieldOfHArray2OfInteger_0.cxx              
  PColStd_FieldOfHArray2OfPersistent_0.cxx           
  PColStd_FieldOfHArray2OfReal_0.cxx                 
  PColStd_HArray1OfExtendedString_0.cxx              
  PColStd_HArray1OfInteger_0.cxx                     
  PColStd_HArray1OfPersistent_0.cxx                  
  PColStd_HArray1OfReal_0.cxx                        
  PColStd_HArray2OfInteger_0.cxx                     
  PColStd_HArray2OfPersistent_0.cxx                  
  PColStd_HArray2OfReal_0.cxx                        
  PColStd_VArrayNodeOfFieldOfHArray1OfExtendedString_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray1OfInteger_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray1OfPersistent_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray1OfReal_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray2OfInteger_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray2OfPersistent_0.cxx
  PColStd_VArrayNodeOfFieldOfHArray2OfReal_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfExtendedString_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfInteger_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfPersistent_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray1OfReal_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfInteger_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfPersistent_0.cxx
  PColStd_VArrayTNodeOfFieldOfHArray2OfReal_0.cxx
  PMMgt_PManaged.cxx
  PShort_FieldOfHArray1OfShortReal_0.cxx
  PShort_FieldOfHArray2OfShortReal_0.cxx
  PShort_HArray1OfShortReal_0.cxx
  PShort_HArray2OfShortReal_0.cxx
  PShort_HSequenceOfShortReal_0.cxx
  PShort_SeqExplorerOfHSequenceOfShortReal_0.cxx
  PShort_SeqNodeOfHSequenceOfShortReal_0.cxx
  PShort_VArrayNodeOfFieldOfHArray1OfShortReal_0.cxx
  PShort_VArrayNodeOfFieldOfHArray2OfShortReal_0.cxx
  PShort_VArrayTNodeOfFieldOfHArray1OfShortReal_0.cxx
  PShort_VArrayTNodeOfFieldOfHArray2OfShortReal_0.cxx
  PStandard_ArrayNode.cxx
  PTColStd_DataMapIteratorOfPersistentTransientMap_0.cxx
  PTColStd_DataMapIteratorOfTransientPersistentMap_0.cxx
  PTColStd_DataMapNodeOfPersistentTransientMap_0.cxx
  PTColStd_DataMapNodeOfTransientPersistentMap_0.cxx
  PTColStd_DoubleMapIteratorOfDoubleMapOfTransientPersistent_0.cxx
  PTColStd_DoubleMapNodeOfDoubleMapOfTransientPersistent_0.cxx
  PTColStd_DoubleMapOfTransientPersistent_0.cxx
  PTColStd_MapPersistentHasher_0.cxx
  PTColStd_PersistentTransientMap_0.cxx
  PTColStd_TransientPersistentMap_0.cxx
  )

# --- rules ---

ADD_LIBRARY(PTKernel ${GEOM_SOURCES})
TARGET_LINK_LIBRARIES(PTKernel ${_link_LIBRARIES})
INSTALL(TARGETS PTKernel EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${GEOM_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
