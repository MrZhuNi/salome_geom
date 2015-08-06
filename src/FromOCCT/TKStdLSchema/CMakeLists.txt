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
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKShapeSchema
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPLCAF
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/PTKernel
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  ${CAS_TKCDF} ${CAS_TKernel} ${CAS_TKLCAF}
  TKShapeSchema
  TKPLCAF
  PTKernel
  )

# --- headers ---

SET(GEOM_HEADERS
  Handle_StdLDrivers_DocumentRetrievalDriver.hxx          
  Handle_StdLDrivers_DocumentStorageDriver.hxx            
  StdLDrivers_DocumentRetrievalDriver.hxx          
  StdLDrivers_DocumentRetrievalDriver.ixx          
  StdLDrivers_DocumentRetrievalDriver.jxx          
  StdLDrivers_DocumentStorageDriver.hxx            
  StdLDrivers_DocumentStorageDriver.ixx            
  StdLDrivers_DocumentStorageDriver.jxx            
  StdLDrivers.hxx                                  
  StdLDrivers.ixx                                  
  StdLDrivers.jxx                                  
  StdLSchema_DBC_VArrayOfCharacter.hxx             
  StdLSchema_DBC_VArrayOfCharacter.ixx             
  StdLSchema_DBC_VArrayOfExtCharacter.hxx          
  StdLSchema_DBC_VArrayOfExtCharacter.ixx          
  StdLSchema.hxx                                   
  StdLSchema_PCDM_Document.hxx                     
  StdLSchema_PCDM_Document.ixx                     
  StdLSchema_PCollection_HAsciiString.hxx          
  StdLSchema_PCollection_HAsciiString.ixx          
  StdLSchema_PCollection_HExtendedString.hxx       
  StdLSchema_PCollection_HExtendedString.ixx       
  StdLSchema_PColStd_FieldOfHArray1OfExtendedString.hxx
  StdLSchema_PColStd_FieldOfHArray1OfExtendedString.ixx
  StdLSchema_PColStd_FieldOfHArray1OfInteger.hxx       
  StdLSchema_PColStd_FieldOfHArray1OfInteger.ixx       
  StdLSchema_PColStd_FieldOfHArray1OfReal.hxx          
  StdLSchema_PColStd_FieldOfHArray1OfReal.ixx          
  StdLSchema_PColStd_FieldOfHArray2OfInteger.hxx       
  StdLSchema_PColStd_FieldOfHArray2OfInteger.ixx       
  StdLSchema_PColStd_HArray1OfExtendedString.hxx       
  StdLSchema_PColStd_HArray1OfExtendedString.ixx       
  StdLSchema_PColStd_HArray1OfInteger.hxx              
  StdLSchema_PColStd_HArray1OfInteger.ixx              
  StdLSchema_PColStd_HArray1OfReal.hxx                 
  StdLSchema_PColStd_HArray1OfReal.ixx                 
  StdLSchema_PColStd_HArray2OfInteger.hxx              
  StdLSchema_PColStd_HArray2OfInteger.ixx              
  StdLSchema_PDataStd_AsciiString.hxx                  
  StdLSchema_PDataStd_AsciiString.ixx                  
  StdLSchema_PDataStd_BooleanArray.hxx                 
  StdLSchema_PDataStd_BooleanArray.ixx                 
  StdLSchema_PDataStd_BooleanList.hxx                  
  StdLSchema_PDataStd_BooleanList.ixx                  
  StdLSchema_PDataStd_ByteArray_1.hxx                  
  StdLSchema_PDataStd_ByteArray_1.ixx                  
  StdLSchema_PDataStd_ByteArray.hxx                    
  StdLSchema_PDataStd_ByteArray.ixx                    
  StdLSchema_PDataStd_Comment.hxx                      
  StdLSchema_PDataStd_Comment.ixx                      
  StdLSchema_PDataStd_Directory.hxx                    
  StdLSchema_PDataStd_Directory.ixx                    
  StdLSchema_PDataStd_Expression.hxx                   
  StdLSchema_PDataStd_Expression.ixx                   
  StdLSchema_PDataStd_ExtStringArray_1.hxx             
  StdLSchema_PDataStd_ExtStringArray_1.ixx             
  StdLSchema_PDataStd_ExtStringArray.hxx               
  StdLSchema_PDataStd_ExtStringArray.ixx               
  StdLSchema_PDataStd_ExtStringList.hxx                
  StdLSchema_PDataStd_ExtStringList.ixx                
  StdLSchema_PDataStd_FieldOfHArray1OfByte.hxx         
  StdLSchema_PDataStd_FieldOfHArray1OfByte.ixx         
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfInteger.hxx
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfInteger.ixx
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfReal.hxx   
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfReal.ixx   
  StdLSchema_PDataStd_FieldOfHArray1OfHAsciiString.hxx    
  StdLSchema_PDataStd_FieldOfHArray1OfHAsciiString.ixx    
  StdLSchema_PDataStd_HArray1OfByte.hxx                   
  StdLSchema_PDataStd_HArray1OfByte.ixx                   
  StdLSchema_PDataStd_HArray1OfHArray1OfInteger.hxx       
  StdLSchema_PDataStd_HArray1OfHArray1OfInteger.ixx       
  StdLSchema_PDataStd_HArray1OfHArray1OfReal.hxx          
  StdLSchema_PDataStd_HArray1OfHArray1OfReal.ixx          
  StdLSchema_PDataStd_HArray1OfHAsciiString.hxx           
  StdLSchema_PDataStd_HArray1OfHAsciiString.ixx           
  StdLSchema_PDataStd_IntegerArray_1.hxx                  
  StdLSchema_PDataStd_IntegerArray_1.ixx                  
  StdLSchema_PDataStd_IntegerArray.hxx                    
  StdLSchema_PDataStd_IntegerArray.ixx                    
  StdLSchema_PDataStd_Integer.hxx                         
  StdLSchema_PDataStd_Integer.ixx                         
  StdLSchema_PDataStd_IntegerList.hxx                     
  StdLSchema_PDataStd_IntegerList.ixx                     
  StdLSchema_PDataStd_IntPackedMap_1.hxx                  
  StdLSchema_PDataStd_IntPackedMap_1.ixx                  
  StdLSchema_PDataStd_IntPackedMap.hxx                    
  StdLSchema_PDataStd_IntPackedMap.ixx                    
  StdLSchema_PDataStd_NamedData.hxx                       
  StdLSchema_PDataStd_NamedData.ixx                       
  StdLSchema_PDataStd_Name.hxx                            
  StdLSchema_PDataStd_Name.ixx                            
  StdLSchema_PDataStd_NoteBook.hxx                        
  StdLSchema_PDataStd_NoteBook.ixx                        
  StdLSchema_PDataStd_RealArray_1.hxx                     
  StdLSchema_PDataStd_RealArray_1.ixx                     
  StdLSchema_PDataStd_RealArray.hxx                       
  StdLSchema_PDataStd_RealArray.ixx                       
  StdLSchema_PDataStd_Real.hxx                            
  StdLSchema_PDataStd_Real.ixx                            
  StdLSchema_PDataStd_RealList.hxx                        
  StdLSchema_PDataStd_RealList.ixx                        
  StdLSchema_PDataStd_ReferenceArray.hxx                  
  StdLSchema_PDataStd_ReferenceArray.ixx                  
  StdLSchema_PDataStd_ReferenceList.hxx
  StdLSchema_PDataStd_ReferenceList.ixx
  StdLSchema_PDataStd_Relation.hxx
  StdLSchema_PDataStd_Relation.ixx
  StdLSchema_PDataStd_Tick.hxx
  StdLSchema_PDataStd_Tick.ixx
  StdLSchema_PDataStd_TreeNode.hxx
  StdLSchema_PDataStd_TreeNode.ixx
  StdLSchema_PDataStd_UAttribute.hxx
  StdLSchema_PDataStd_UAttribute.ixx
  StdLSchema_PDataStd_Variable.hxx
  StdLSchema_PDataStd_Variable.ixx
  StdLSchema_PDF_Attribute.hxx
  StdLSchema_PDF_Attribute.ixx
  StdLSchema_PDF_Data.hxx
  StdLSchema_PDF_Data.ixx
  StdLSchema_PDF_FieldOfHAttributeArray1.hxx
  StdLSchema_PDF_FieldOfHAttributeArray1.ixx
  StdLSchema_PDF_HAttributeArray1.hxx
  StdLSchema_PDF_HAttributeArray1.ixx
  StdLSchema_PDF_Reference.hxx
  StdLSchema_PDF_Reference.ixx
  StdLSchema_PDF_TagSource.hxx
  StdLSchema_PDF_TagSource.ixx
  StdLSchema_PDocStd_Document.hxx
  StdLSchema_PDocStd_Document.ixx
  StdLSchema_PDocStd_XLink.hxx
  StdLSchema_PDocStd_XLink.ixx
  StdLSchema_PFunction_Function.hxx
  StdLSchema_PFunction_Function.ixx
  StdLSchema_Standard_GUID.hxx
  StdLSchema_Standard_GUID.ixx
  StdLSchema_Standard_Persistent.hxx
  StdLSchema_Standard_Persistent.ixx
  )

# --- sources ---

SET(GEOM_SOURCES
  StdLDrivers.cxx                                       
  StdLDrivers_DocumentRetrievalDriver.cxx               
  StdLDrivers_DocumentStorageDriver.cxx                 
  StdLSchema.cxx                                        
  StdLSchema_DBC_VArrayOfCharacter.cxx                  
  StdLSchema_DBC_VArrayOfExtCharacter.cxx               
  StdLSchema_PCDM_Document.cxx                          
  StdLSchema_PCollection_HAsciiString.cxx               
  StdLSchema_PCollection_HExtendedString.cxx            
  StdLSchema_PColStd_FieldOfHArray1OfExtendedString.cxx 
  StdLSchema_PColStd_FieldOfHArray1OfInteger.cxx        
  StdLSchema_PColStd_FieldOfHArray1OfReal.cxx           
  StdLSchema_PColStd_FieldOfHArray2OfInteger.cxx        
  StdLSchema_PColStd_HArray1OfExtendedString.cxx        
  StdLSchema_PColStd_HArray1OfInteger.cxx               
  StdLSchema_PColStd_HArray1OfReal.cxx                  
  StdLSchema_PColStd_HArray2OfInteger.cxx               
  StdLSchema_PDataStd_AsciiString.cxx                   
  StdLSchema_PDataStd_BooleanArray.cxx                  
  StdLSchema_PDataStd_BooleanList.cxx                   
  StdLSchema_PDataStd_ByteArray_1.cxx                   
  StdLSchema_PDataStd_ByteArray.cxx                     
  StdLSchema_PDataStd_Comment.cxx                       
  StdLSchema_PDataStd_Directory.cxx                     
  StdLSchema_PDataStd_Expression.cxx                    
  StdLSchema_PDataStd_ExtStringArray_1.cxx              
  StdLSchema_PDataStd_ExtStringArray.cxx                
  StdLSchema_PDataStd_ExtStringList.cxx                 
  StdLSchema_PDataStd_FieldOfHArray1OfByte.cxx          
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfInteger.cxx
  StdLSchema_PDataStd_FieldOfHArray1OfHArray1OfReal.cxx   
  StdLSchema_PDataStd_FieldOfHArray1OfHAsciiString.cxx    
  StdLSchema_PDataStd_HArray1OfByte.cxx
  StdLSchema_PDataStd_HArray1OfHArray1OfInteger.cxx
  StdLSchema_PDataStd_HArray1OfHArray1OfReal.cxx
  StdLSchema_PDataStd_HArray1OfHAsciiString.cxx
  StdLSchema_PDataStd_IntegerArray_1.cxx
  StdLSchema_PDataStd_IntegerArray.cxx
  StdLSchema_PDataStd_Integer.cxx
  StdLSchema_PDataStd_IntegerList.cxx
  StdLSchema_PDataStd_IntPackedMap_1.cxx
  StdLSchema_PDataStd_IntPackedMap.cxx
  StdLSchema_PDataStd_Name.cxx
  StdLSchema_PDataStd_NamedData.cxx
  StdLSchema_PDataStd_NoteBook.cxx
  StdLSchema_PDataStd_RealArray_1.cxx
  StdLSchema_PDataStd_RealArray.cxx
  StdLSchema_PDataStd_Real.cxx
  StdLSchema_PDataStd_RealList.cxx
  StdLSchema_PDataStd_ReferenceArray.cxx
  StdLSchema_PDataStd_ReferenceList.cxx
  StdLSchema_PDataStd_Relation.cxx
  StdLSchema_PDataStd_Tick.cxx
  StdLSchema_PDataStd_TreeNode.cxx
  StdLSchema_PDataStd_UAttribute.cxx
  StdLSchema_PDataStd_Variable.cxx
  StdLSchema_PDF_Attribute.cxx
  StdLSchema_PDF_Data.cxx
  StdLSchema_PDF_FieldOfHAttributeArray1.cxx
  StdLSchema_PDF_HAttributeArray1.cxx
  StdLSchema_PDF_Reference.cxx
  StdLSchema_PDF_TagSource.cxx
  StdLSchema_PDocStd_Document.cxx
  StdLSchema_PDocStd_XLink.cxx
  StdLSchema_PFunction_Function.cxx
  StdLSchema_Standard_GUID.cxx
  StdLSchema_Standard_Persistent.cxx
  )

# --- rules ---

ADD_LIBRARY(TKStdLSchema ${GEOM_SOURCES})
TARGET_LINK_LIBRARIES(TKStdLSchema ${_link_LIBRARIES})
INSTALL(TARGETS TKStdLSchema EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${GEOM_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
