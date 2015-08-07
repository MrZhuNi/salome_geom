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
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/PTKernel
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPShape
  ${PROJECT_SOURCE_DIR}/src/FromOCCT/TKPLCAF
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# additional preprocessor / compiler flags
ADD_DEFINITIONS(
  ${CAS_DEFINITIONS}
  )

# libraries to link to
SET(_link_LIBRARIES
  ${CAS_KERNEL}
  ${CAS_TKBRep} ${CAS_TKMath} ${CAS_TKCDF}
  ${CAS_TKG2d} ${CAS_TKV3d} ${CAS_TKService}
  ${CAS_TKG3d} ${CAS_TKCAF} ${CAS_TKLCAF}
  PTKernel
  TKPShape
  TKPLCAF
  )

# --- headers ---

SET(GEOM_HEADERS
  Handle_MDataXtd_AxisRetrievalDriver.hxx
  Handle_MDataXtd_AxisStorageDriver.hxx
  Handle_MDataXtd_ConstraintRetrievalDriver.hxx
  Handle_MDataXtd_ConstraintStorageDriver.hxx
  Handle_MDataXtd_GeometryRetrievalDriver.hxx
  Handle_MDataXtd_GeometryStorageDriver.hxx
  Handle_MDataXtd_PatternStdRetrievalDriver.hxx
  Handle_MDataXtd_PatternStdStorageDriver.hxx
  Handle_MDataXtd_PlacementRetrievalDriver.hxx
  Handle_MDataXtd_PlacementStorageDriver.hxx
  Handle_MDataXtd_PlaneRetrievalDriver.hxx
  Handle_MDataXtd_PlaneStorageDriver.hxx
  Handle_MDataXtd_PointRetrievalDriver.hxx
  Handle_MDataXtd_PointStorageDriver.hxx
  Handle_MDataXtd_ShapeRetrievalDriver.hxx
  Handle_MDataXtd_ShapeStorageDriver.hxx
  Handle_MNaming_NamedShapeRetrievalDriver.hxx
  Handle_MNaming_NamedShapeStorageDriver.hxx
  Handle_MNaming_NamingRetrievalDriver_1.hxx
  Handle_MNaming_NamingRetrievalDriver_2.hxx
  Handle_MNaming_NamingRetrievalDriver.hxx
  Handle_MNaming_NamingStorageDriver.hxx
  Handle_MPrsStd_AISPresentationRetrievalDriver_1.hxx
  Handle_MPrsStd_AISPresentationRetrievalDriver.hxx
  Handle_MPrsStd_AISPresentationStorageDriver.hxx
  Handle_MPrsStd_PositionRetrievalDriver.hxx
  Handle_MPrsStd_PositionStorageDriver.hxx
  Handle_PDataXtd_Axis.hxx
  Handle_PDataXtd_Constraint.hxx
  Handle_PDataXtd_Geometry.hxx
  Handle_PDataXtd_PatternStd.hxx
  Handle_PDataXtd_Placement.hxx
  Handle_PDataXtd_Plane.hxx
  Handle_PDataXtd_Point.hxx
  Handle_PDataXtd_Position.hxx
  Handle_PDataXtd_Shape.hxx
  Handle_PNaming_HArray1OfNamedShape.hxx
  Handle_PNaming_Name_1.hxx
  Handle_PNaming_Name_2.hxx
  Handle_PNaming_NamedShape.hxx
  Handle_PNaming_Name.hxx
  Handle_PNaming_Naming_1.hxx
  Handle_PNaming_Naming_2.hxx
  Handle_PNaming_Naming.hxx
  Handle_PNaming_VArrayNodeOfFieldOfHArray1OfNamedShape.hxx
  Handle_PPrsStd_AISPresentation_1.hxx
  Handle_PPrsStd_AISPresentation.hxx
  MDataXtd_AxisRetrievalDriver.hxx
  MDataXtd_AxisRetrievalDriver.ixx
  MDataXtd_AxisRetrievalDriver.jxx
  MDataXtd_AxisStorageDriver.hxx
  MDataXtd_AxisStorageDriver.ixx
  MDataXtd_AxisStorageDriver.jxx
  MDataXtd_ConstraintRetrievalDriver.hxx
  MDataXtd_ConstraintRetrievalDriver.ixx
  MDataXtd_ConstraintRetrievalDriver.jxx
  MDataXtd_ConstraintStorageDriver.hxx
  MDataXtd_ConstraintStorageDriver.ixx
  MDataXtd_ConstraintStorageDriver.jxx
  MDataXtd_GeometryRetrievalDriver.hxx
  MDataXtd_GeometryRetrievalDriver.ixx
  MDataXtd_GeometryRetrievalDriver.jxx
  MDataXtd_GeometryStorageDriver.hxx
  MDataXtd_GeometryStorageDriver.ixx
  MDataXtd_GeometryStorageDriver.jxx
  MDataXtd.hxx
  MDataXtd.ixx
  MDataXtd.jxx
  MDataXtd_PatternStdRetrievalDriver.hxx
  MDataXtd_PatternStdRetrievalDriver.ixx
  MDataXtd_PatternStdRetrievalDriver.jxx
  MDataXtd_PatternStdStorageDriver.hxx
  MDataXtd_PatternStdStorageDriver.ixx
  MDataXtd_PatternStdStorageDriver.jxx
  MDataXtd_PlacementRetrievalDriver.hxx
  MDataXtd_PlacementRetrievalDriver.ixx
  MDataXtd_PlacementRetrievalDriver.jxx
  MDataXtd_PlacementStorageDriver.hxx
  MDataXtd_PlacementStorageDriver.ixx
  MDataXtd_PlacementStorageDriver.jxx
  MDataXtd_PlaneRetrievalDriver.hxx
  MDataXtd_PlaneRetrievalDriver.ixx
  MDataXtd_PlaneRetrievalDriver.jxx
  MDataXtd_PlaneStorageDriver.hxx
  MDataXtd_PlaneStorageDriver.ixx
  MDataXtd_PlaneStorageDriver.jxx
  MDataXtd_PointRetrievalDriver.hxx
  MDataXtd_PointRetrievalDriver.ixx
  MDataXtd_PointRetrievalDriver.jxx
  MDataXtd_PointStorageDriver.hxx
  MDataXtd_PointStorageDriver.ixx
  MDataXtd_PointStorageDriver.jxx
  MDataXtd_ShapeRetrievalDriver.hxx
  MDataXtd_ShapeRetrievalDriver.ixx
  MDataXtd_ShapeRetrievalDriver.jxx
  MDataXtd_ShapeStorageDriver.hxx
  MDataXtd_ShapeStorageDriver.ixx
  MDataXtd_ShapeStorageDriver.jxx
  MNaming.hxx
  MNaming.ixx
  MNaming.jxx
  MNaming_NamedShapeRetrievalDriver.hxx
  MNaming_NamedShapeRetrievalDriver.ixx
  MNaming_NamedShapeRetrievalDriver.jxx
  MNaming_NamedShapeStorageDriver.hxx
  MNaming_NamedShapeStorageDriver.ixx
  MNaming_NamedShapeStorageDriver.jxx
  MNaming_NamingRetrievalDriver_1.hxx
  MNaming_NamingRetrievalDriver_1.ixx
  MNaming_NamingRetrievalDriver_1.jxx
  MNaming_NamingRetrievalDriver_2.hxx
  MNaming_NamingRetrievalDriver_2.ixx
  MNaming_NamingRetrievalDriver_2.jxx
  MNaming_NamingRetrievalDriver.hxx
  MNaming_NamingRetrievalDriver.ixx
  MNaming_NamingRetrievalDriver.jxx
  MNaming_NamingStorageDriver.hxx
  MNaming_NamingStorageDriver.ixx
  MNaming_NamingStorageDriver.jxx
  MPrsStd_AISPresentationRetrievalDriver_1.hxx
  MPrsStd_AISPresentationRetrievalDriver_1.ixx
  MPrsStd_AISPresentationRetrievalDriver_1.jxx
  MPrsStd_AISPresentationRetrievalDriver.hxx
  MPrsStd_AISPresentationRetrievalDriver.ixx
  MPrsStd_AISPresentationRetrievalDriver.jxx
  MPrsStd_AISPresentationStorageDriver.hxx
  MPrsStd_AISPresentationStorageDriver.ixx
  MPrsStd_AISPresentationStorageDriver.jxx
  MPrsStd.hxx
  MPrsStd.ixx
  MPrsStd.jxx
  MPrsStd_PositionRetrievalDriver.hxx
  MPrsStd_PositionRetrievalDriver.ixx
  MPrsStd_PositionRetrievalDriver.jxx
  MPrsStd_PositionStorageDriver.hxx
  MPrsStd_PositionStorageDriver.ixx
  MPrsStd_PositionStorageDriver.jxx
  PDataXtd_Axis.hxx
  PDataXtd_Axis.ixx
  PDataXtd_Axis.jxx
  PDataXtd_Constraint.hxx
  PDataXtd_Constraint.ixx
  PDataXtd_Constraint.jxx
  PDataXtd_Geometry.hxx
  PDataXtd_Geometry.ixx
  PDataXtd_Geometry.jxx
  PDataXtd_PatternStd.hxx
  PDataXtd_PatternStd.ixx
  PDataXtd_PatternStd.jxx
  PDataXtd_PatternStd.lxx
  PDataXtd_Placement.hxx
  PDataXtd_Placement.ixx
  PDataXtd_Placement.jxx
  PDataXtd_Plane.hxx
  PDataXtd_Plane.ixx
  PDataXtd_Plane.jxx
  PDataXtd_Point.hxx
  PDataXtd_Point.ixx
  PDataXtd_Point.jxx
  PDataXtd_Position.hxx
  PDataXtd_Position.ixx
  PDataXtd_Position.jxx
  PDataXtd_Position.lxx
  PDataXtd_Shape.hxx
  PDataXtd_Shape.ixx
  PDataXtd_Shape.jxx
  PNaming_FieldOfHArray1OfNamedShape.hxx
  PNaming_HArray1OfNamedShape.hxx
  PNaming_Name_1.hxx
  PNaming_Name_1.ixx
  PNaming_Name_1.jxx
  PNaming_Name_1.lxx
  PNaming_Name_2.hxx
  PNaming_Name_2.ixx
  PNaming_Name_2.jxx
  PNaming_Name_2.lxx
  PNaming_NamedShape.hxx
  PNaming_NamedShape.ixx
  PNaming_NamedShape.jxx
  PNaming_Name.hxx
  PNaming_Name.ixx
  PNaming_Name.jxx
  PNaming_Name.lxx
  PNaming_Naming_1.hxx
  PNaming_Naming_1.ixx
  PNaming_Naming_1.jxx
  PNaming_Naming_2.hxx
  PNaming_Naming_2.ixx
  PNaming_Naming_2.jxx
  PNaming_Naming.hxx
  PNaming_Naming.ixx
  PNaming_Naming.jxx
  PNaming_VArrayNodeOfFieldOfHArray1OfNamedShape.hxx
  PNaming_VArrayTNodeOfFieldOfHArray1OfNamedShape.hxx
  PPrsStd_AISPresentation_1.hxx
  PPrsStd_AISPresentation_1.ixx
  PPrsStd_AISPresentation_1.jxx
  PPrsStd_AISPresentation.hxx
  PPrsStd_AISPresentation.ixx
  PPrsStd_AISPresentation.jxx
  )

# --- sources ---

SET(GEOM_SOURCES
  MDataXtd_AxisRetrievalDriver.cxx
  MDataXtd_AxisStorageDriver.cxx
  MDataXtd_ConstraintRetrievalDriver.cxx
  MDataXtd_ConstraintStorageDriver.cxx
  MDataXtd.cxx
  MDataXtd_GeometryRetrievalDriver.cxx
  MDataXtd_GeometryStorageDriver.cxx
  MDataXtd_PatternStdRetrievalDriver.cxx
  MDataXtd_PatternStdStorageDriver.cxx
  MDataXtd_PlacementRetrievalDriver.cxx
  MDataXtd_PlacementStorageDriver.cxx
  MDataXtd_PlaneRetrievalDriver.cxx
  MDataXtd_PlaneStorageDriver.cxx
  MDataXtd_PointRetrievalDriver.cxx
  MDataXtd_PointStorageDriver.cxx
  MDataXtd_ShapeRetrievalDriver.cxx
  MDataXtd_ShapeStorageDriver.cxx
  MNaming.cxx
  MNaming_NamedShapeRetrievalDriver.cxx
  MNaming_NamedShapeStorageDriver.cxx
  MNaming_NamingRetrievalDriver_1.cxx
  MNaming_NamingRetrievalDriver_2.cxx
  MNaming_NamingRetrievalDriver.cxx
  MNaming_NamingStorageDriver.cxx
  MPrsStd_AISPresentationRetrievalDriver_1.cxx
  MPrsStd_AISPresentationRetrievalDriver.cxx
  MPrsStd_AISPresentationStorageDriver.cxx
  MPrsStd.cxx
  MPrsStd_PositionRetrievalDriver.cxx
  MPrsStd_PositionStorageDriver.cxx
  PDataXtd_Axis.cxx
  PDataXtd_Constraint.cxx
  PDataXtd_Geometry.cxx
  PDataXtd_PatternStd.cxx
  PDataXtd_PatternStd.lxx
  PDataXtd_Placement.cxx
  PDataXtd_Plane.cxx
  PDataXtd_Point.cxx
  PDataXtd_Position.cxx
  PDataXtd_Position.lxx
  PDataXtd_Shape.cxx
  PNaming_FieldOfHArray1OfNamedShape_0.cxx
  PNaming_HArray1OfNamedShape_0.cxx
  PNaming_Name_1.cxx
  PNaming_Name_1.lxx
  PNaming_Name_2.cxx
  PNaming_Name_2.lxx
  PNaming_Name.cxx
  PNaming_NamedShape.cxx
  PNaming_Name.lxx
  PNaming_Naming_1.cxx
  PNaming_Naming_2.cxx
  PNaming_Naming.cxx
  PNaming_VArrayNodeOfFieldOfHArray1OfNamedShape_0.cxx
  PNaming_VArrayTNodeOfFieldOfHArray1OfNamedShape_0.cxx
  PPrsStd_AISPresentation_1.cxx
  PPrsStd_AISPresentation.cxx
  )

# --- rules ---

ADD_LIBRARY(TKPCAF ${GEOM_SOURCES})
TARGET_LINK_LIBRARIES(TKPCAF ${_link_LIBRARIES})
INSTALL(TARGETS TKPCAF EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

INSTALL(FILES ${GEOM_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
