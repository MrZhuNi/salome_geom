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
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPLCAF
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKShapeSchema
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKStdLSchema
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPShape
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPCAF
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/PTKernel
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  ${CAS_TKCDF} ${CAS_TKernel}
  ${CAS_TKCAF} ${CAS_TKLCAF} ${CAS_TKG3d}
  ${CAS_TKMath} ${CAS_TKG2d}
  TKPLCAF
  TKShapeSchema
  TKStdLSchema
  TKPShape
  TKPCAF
  PTKernel
  )

# --- headers ---

SET(GEOM_HEADERS
  Handle_StdDrivers_DocumentRetrievalDriver.hxx
  Handle_StdDrivers_DocumentStorageDriver.hxx
  StdDrivers_DocumentRetrievalDriver.hxx
  StdDrivers_DocumentRetrievalDriver.ixx
  StdDrivers_DocumentRetrievalDriver.jxx
  StdDrivers_DocumentStorageDriver.hxx
  StdDrivers_DocumentStorageDriver.ixx
  StdDrivers_DocumentStorageDriver.jxx
  StdDrivers.hxx
  StdDrivers.ixx
  StdDrivers.jxx
  StdSchema_DBC_VArrayOfCharacter.hxx
  StdSchema_DBC_VArrayOfCharacter.ixx
  StdSchema_DBC_VArrayOfExtCharacter.hxx
  StdSchema_DBC_VArrayOfExtCharacter.ixx
  StdSchema_gp_Ax1.hxx
  StdSchema_gp_Ax1.ixx
  StdSchema_gp_Ax2d.hxx
  StdSchema_gp_Ax2d.ixx
  StdSchema_gp_Ax2.hxx
  StdSchema_gp_Ax2.ixx
  StdSchema_gp_Ax3.hxx
  StdSchema_gp_Ax3.ixx
  StdSchema_gp_Dir2d.hxx
  StdSchema_gp_Dir2d.ixx
  StdSchema_gp_Dir.hxx
  StdSchema_gp_Dir.ixx
  StdSchema_gp_Mat2d.hxx
  StdSchema_gp_Mat2d.ixx
  StdSchema_gp_Mat.hxx
  StdSchema_gp_Mat.ixx
  StdSchema_gp_Pnt2d.hxx
  StdSchema_gp_Pnt2d.ixx
  StdSchema_gp_Pnt.hxx
  StdSchema_gp_Pnt.ixx
  StdSchema_gp_Trsf2d.hxx
  StdSchema_gp_Trsf2d.ixx
  StdSchema_gp_Trsf.hxx
  StdSchema_gp_Trsf.ixx
  StdSchema_gp_Vec2d.hxx
  StdSchema_gp_Vec2d.ixx
  StdSchema_gp_Vec.hxx
  StdSchema_gp_Vec.ixx
  StdSchema_gp_XY.hxx
  StdSchema_gp_XY.ixx
  StdSchema_gp_XYZ.hxx
  StdSchema_gp_XYZ.ixx
  StdSchema.hxx
  StdSchema_PCollection_HAsciiString.hxx
  StdSchema_PCollection_HAsciiString.ixx
  StdSchema_PCollection_HExtendedString.hxx
  StdSchema_PCollection_HExtendedString.ixx
  StdSchema_PColStd_FieldOfHArray1OfInteger.hxx
  StdSchema_PColStd_FieldOfHArray1OfInteger.ixx
  StdSchema_PColStd_HArray1OfInteger.hxx
  StdSchema_PColStd_HArray1OfInteger.ixx
  StdSchema_PDataStd_Integer.hxx
  StdSchema_PDataStd_Integer.ixx
  StdSchema_PDataStd_Real.hxx
  StdSchema_PDataStd_Real.ixx
  StdSchema_PDataXtd_Axis.hxx
  StdSchema_PDataXtd_Axis.ixx
  StdSchema_PDataXtd_Constraint.hxx
  StdSchema_PDataXtd_Constraint.ixx
  StdSchema_PDataXtd_Geometry.hxx
  StdSchema_PDataXtd_Geometry.ixx
  StdSchema_PDataXtd_PatternStd.hxx
  StdSchema_PDataXtd_PatternStd.ixx
  StdSchema_PDataXtd_Placement.hxx
  StdSchema_PDataXtd_Placement.ixx
  StdSchema_PDataXtd_Plane.hxx
  StdSchema_PDataXtd_Plane.ixx
  StdSchema_PDataXtd_Point.hxx
  StdSchema_PDataXtd_Point.ixx
  StdSchema_PDataXtd_Position.hxx
  StdSchema_PDataXtd_Position.ixx
  StdSchema_PDataXtd_Shape.hxx
  StdSchema_PDataXtd_Shape.ixx
  StdSchema_PDF_Attribute.hxx
  StdSchema_PDF_Attribute.ixx
  StdSchema_PDF_FieldOfHAttributeArray1.hxx
  StdSchema_PDF_FieldOfHAttributeArray1.ixx
  StdSchema_PDF_HAttributeArray1.hxx
  StdSchema_PDF_HAttributeArray1.ixx
  StdSchema_PNaming_FieldOfHArray1OfNamedShape.hxx
  StdSchema_PNaming_FieldOfHArray1OfNamedShape.ixx
  StdSchema_PNaming_HArray1OfNamedShape.hxx
  StdSchema_PNaming_HArray1OfNamedShape.ixx
  StdSchema_PNaming_Name_1.hxx
  StdSchema_PNaming_Name_1.ixx
  StdSchema_PNaming_Name_2.hxx
  StdSchema_PNaming_Name_2.ixx
  StdSchema_PNaming_NamedShape.hxx
  StdSchema_PNaming_NamedShape.ixx
  StdSchema_PNaming_Name.hxx
  StdSchema_PNaming_Name.ixx
  StdSchema_PNaming_Naming_1.hxx
  StdSchema_PNaming_Naming_1.ixx
  StdSchema_PNaming_Naming_2.hxx
  StdSchema_PNaming_Naming_2.ixx
  StdSchema_PNaming_Naming.hxx
  StdSchema_PNaming_Naming.ixx
  StdSchema_PPrsStd_AISPresentation_1.hxx
  StdSchema_PPrsStd_AISPresentation_1.ixx
  StdSchema_PPrsStd_AISPresentation.hxx
  StdSchema_PPrsStd_AISPresentation.ixx
  StdSchema_PTopLoc_Datum3D.hxx
  StdSchema_PTopLoc_Datum3D.ixx
  StdSchema_PTopLoc_ItemLocation.hxx
  StdSchema_PTopLoc_ItemLocation.ixx
  StdSchema_PTopLoc_Location.hxx
  StdSchema_PTopLoc_Location.ixx
  StdSchema_PTopoDS_FieldOfHArray1OfShape1.hxx
  StdSchema_PTopoDS_FieldOfHArray1OfShape1.ixx
  StdSchema_PTopoDS_HArray1OfShape1.hxx
  StdSchema_PTopoDS_HArray1OfShape1.ixx
  StdSchema_PTopoDS_Shape1.hxx
  StdSchema_PTopoDS_Shape1.ixx
  StdSchema_PTopoDS_TShape1.hxx
  StdSchema_PTopoDS_TShape1.ixx
  StdSchema_Standard_Persistent.hxx
  StdSchema_Standard_Persistent.ixx
  )

# --- sources ---

SET(GEOM_SOURCES
  StdDrivers.cxx
  StdDrivers_DocumentRetrievalDriver.cxx
  StdDrivers_DocumentStorageDriver.cxx
  StdSchema.cxx
  StdSchema_DBC_VArrayOfCharacter.cxx
  StdSchema_DBC_VArrayOfExtCharacter.cxx
  StdSchema_gp_Ax1.cxx
  StdSchema_gp_Ax2.cxx
  StdSchema_gp_Ax2d.cxx
  StdSchema_gp_Ax3.cxx
  StdSchema_gp_Dir2d.cxx
  StdSchema_gp_Dir.cxx
  StdSchema_gp_Mat2d.cxx
  StdSchema_gp_Mat.cxx
  StdSchema_gp_Pnt2d.cxx
  StdSchema_gp_Pnt.cxx
  StdSchema_gp_Trsf2d.cxx
  StdSchema_gp_Trsf.cxx
  StdSchema_gp_Vec2d.cxx
  StdSchema_gp_Vec.cxx
  StdSchema_gp_XY.cxx
  StdSchema_gp_XYZ.cxx
  StdSchema_PCollection_HAsciiString.cxx
  StdSchema_PCollection_HExtendedString.cxx
  StdSchema_PColStd_FieldOfHArray1OfInteger.cxx
  StdSchema_PColStd_HArray1OfInteger.cxx
  StdSchema_PDataStd_Integer.cxx
  StdSchema_PDataStd_Real.cxx
  StdSchema_PDataXtd_Axis.cxx
  StdSchema_PDataXtd_Constraint.cxx
  StdSchema_PDataXtd_Geometry.cxx
  StdSchema_PDataXtd_PatternStd.cxx
  StdSchema_PDataXtd_Placement.cxx
  StdSchema_PDataXtd_Plane.cxx
  StdSchema_PDataXtd_Point.cxx
  StdSchema_PDataXtd_Position.cxx
  StdSchema_PDataXtd_Shape.cxx
  StdSchema_PDF_Attribute.cxx
  StdSchema_PDF_FieldOfHAttributeArray1.cxx
  StdSchema_PDF_HAttributeArray1.cxx
  StdSchema_PNaming_FieldOfHArray1OfNamedShape.cxx
  StdSchema_PNaming_HArray1OfNamedShape.cxx
  StdSchema_PNaming_Name_1.cxx
  StdSchema_PNaming_Name_2.cxx
  StdSchema_PNaming_Name.cxx
  StdSchema_PNaming_NamedShape.cxx
  StdSchema_PNaming_Naming_1.cxx
  StdSchema_PNaming_Naming_2.cxx
  StdSchema_PNaming_Naming.cxx
  StdSchema_PPrsStd_AISPresentation_1.cxx
  StdSchema_PPrsStd_AISPresentation.cxx
  StdSchema_PTopLoc_Datum3D.cxx
  StdSchema_PTopLoc_ItemLocation.cxx
  StdSchema_PTopLoc_Location.cxx
  StdSchema_PTopoDS_FieldOfHArray1OfShape1.cxx
  StdSchema_PTopoDS_HArray1OfShape1.cxx
  StdSchema_PTopoDS_Shape1.cxx
  StdSchema_PTopoDS_TShape1.cxx
  StdSchema_Standard_Persistent.cxx
  )

# --- rules ---

ADD_LIBRARY(TKStdSchema ${GEOM_SOURCES})
TARGET_LINK_LIBRARIES(TKStdSchema ${_link_LIBRARIES})
INSTALL(TARGETS TKStdSchema EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${GEOM_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})

SET(CAS_STDPLUGIN TKStdSchema)
