#  GEOM GEOM_SWIG : binding of C++ omplementaion with Python
#
#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : geompy.py
#  Author : Paul RASCLE, EDF
#  Module : GEOM
#  $Header$

import salome
salome.salome_init()
from salome import *

import GEOM

"""
    \namespace geompy
    \brief Module geompy
"""

g = lcc.FindOrLoadComponent("FactoryServer", "GEOM")
geom = g._narrow( GEOM.GEOM_Gen )
#gg = ImportComponentGUI("GEOM")
#gg.initGeomGen()

#SRN: modified on Mar 18, 2005

myBuilder = None
myStudyId = 0
father    = None

BasicOp  = None
CurvesOp = None
PrimOp   = None
ShapesOp = None
HealOp   = None
InsertOp = None
BoolOp   = None
TrsfOp   = None
LocalOp  = None
MeasuOp  = None
BlocksOp = None
GroupOp  = None

def init_geom(theStudy):

    global myStudy, myBuilder, myStudyId, BasicOp, CurvesOp, PrimOp, ShapesOp, HealOp
    global InsertOp, BoolOp, TrsfOp, LocalOp, MeasuOp, BlocksOp, GroupOp, father

    myStudy = theStudy
    myStudyId = myStudy._get_StudyId()
    myBuilder = myStudy.NewBuilder()
    father = myStudy.FindComponent("GEOM")
    if father is None:
        father = myBuilder.NewComponent("GEOM")
        A1 = myBuilder.FindOrCreateAttribute(father, "AttributeName")
        FName = A1._narrow(SALOMEDS.AttributeName)
        FName.SetValue("Geometry")
      	A2 = myBuilder.FindOrCreateAttribute(father, "AttributePixMap")
      	aPixmap = A2._narrow(SALOMEDS.AttributePixMap)
	aPixmap.SetPixMap("ICON_OBJBROWSER_Geometry")
	myBuilder.DefineComponentInstance(father,geom)
        pass

    # -----------------------------------------------------------------------------
    # Assign Operations Interfaces
    # -----------------------------------------------------------------------------

    BasicOp  = geom.GetIBasicOperations    (myStudyId)
    CurvesOp = geom.GetICurvesOperations   (myStudyId)
    PrimOp   = geom.GetI3DPrimOperations   (myStudyId)
    ShapesOp = geom.GetIShapesOperations   (myStudyId)
    HealOp   = geom.GetIHealingOperations  (myStudyId)
    InsertOp = geom.GetIInsertOperations   (myStudyId)
    BoolOp   = geom.GetIBooleanOperations  (myStudyId)
    TrsfOp   = geom.GetITransformOperations(myStudyId)
    LocalOp  = geom.GetILocalOperations    (myStudyId)
    MeasuOp  = geom.GetIMeasureOperations  (myStudyId)
    BlocksOp = geom.GetIBlocksOperations   (myStudyId)
    GroupOp  = geom.GetIGroupOperations   (myStudyId)
    pass

init_geom(myStudy)

#SRN: end of modifications

## Get name for sub-shape aSubObj of shape aMainObj
#
#  Example: see GEOM_TestAll.py
def SubShapeName(aSubObj, aMainObj):
    #aSubId  = orb.object_to_string(aSubObj)
    #aMainId = orb.object_to_string(aMainObj)
    #index = gg.getIndexTopology(aSubId, aMainId)
    #name = gg.getShapeTypeString(aSubId) + "_%d"%(index)
    index = ShapesOp.GetTopologyIndex(aMainObj, aSubObj)
    name = ShapesOp.GetShapeTypeString(aSubObj) + "_%d"%(index)
    return name

## Publish in study aShape with name aName
#
#  Example: see GEOM_TestAll.py
def addToStudy(aShape, aName):
    try:
        aSObject = geom.AddInStudy(myStudy, aShape, aName, None)
    except:
        print "addToStudy() failed"
        return ""
    return aShape.GetStudyEntry()

## Publish in study aShape with name aName as sub-object of previously published aFather
#
#  Example: see GEOM_TestAll.py
def addToStudyInFather(aFather, aShape, aName):
    try:
        aSObject = geom.AddInStudy(myStudy, aShape, aName, aFather)
    except:
        print "addToStudyInFather() failed"
        return ""
    return aShape.GetStudyEntry()

# -----------------------------------------------------------------------------
# Raise an Error if Operation is Failed
# -----------------------------------------------------------------------------
def RaiseIfFailed (method_name, operation):
    #NPAL18017#if operation.IsDone() == 0:
    if operation.IsDone() == 0 and operation.GetErrorCode() != "NOT_FOUND_ANY":
      raise RuntimeError, method_name + " : " + operation.GetErrorCode()

# -----------------------------------------------------------------------------
# enumeration ShapeType as a dictionary
# -----------------------------------------------------------------------------

ShapeType = {"COMPOUND":0, "COMPSOLID":1, "SOLID":2, "SHELL":3, "FACE":4, "WIRE":5, "EDGE":6, "VERTEX":7, "SHAPE":8}

# -----------------------------------------------------------------------------
# enumeration shape_kind
# -----------------------------------------------------------------------------

kind = GEOM.GEOM_IKindOfShape

class info:
    UNKNOWN  = 0
    CLOSED   = 1
    UNCLOSED = 2

# -----------------------------------------------------------------------------
# Basic primitives
# -----------------------------------------------------------------------------

## Create point by three coordinates.
#  @param theX The X coordinate of the point.
#  @param theY The Y coordinate of the point.
#  @param theZ The Z coordinate of the point.
#  @return New GEOM_Object, containing the created point.
#
#  Example: see GEOM_TestAll.py
def MakeVertex(theX, theY, theZ):
    anObj = BasicOp.MakePointXYZ(theX, theY, theZ)
    RaiseIfFailed("MakePointXYZ", BasicOp)
    return anObj

## Create a point, distant from the referenced point
#  on the given distances along the coordinate axes.
#  @param theReference The referenced point.
#  @param theX Displacement from the referenced point along OX axis.
#  @param theY Displacement from the referenced point along OY axis.
#  @param theZ Displacement from the referenced point along OZ axis.
#  @return New GEOM_Object, containing the created point.
#
#  Example: see GEOM_TestAll.py
def MakeVertexWithRef(theReference, theX, theY, theZ):
    anObj = BasicOp.MakePointWithReference(theReference, theX, theY, theZ)
    RaiseIfFailed("MakePointWithReference", BasicOp)
    return anObj

## Create a point, corresponding to the given parameter on the given curve.
#  @param theRefCurve The referenced curve.
#  @param theParameter Value of parameter on the referenced curve.
#  @return New GEOM_Object, containing the created point.
#
#  Example: see GEOM_TestAll.py
def MakeVertexOnCurve(theRefCurve, theParameter):
    anObj = BasicOp.MakePointOnCurve(theRefCurve, theParameter)
    RaiseIfFailed("MakePointOnCurve", BasicOp)
    return anObj

## Create a point on intersection of two lines.
#  @param theRefLine1, theRefLine2 The referenced lines.
#  @return New GEOM_Object, containing the created point.
#
#  Example: see GEOM_TestAll.py
def MakeVertexOnLinesIntersection(theRefLine1, theRefLine2):
    anObj = BasicOp.MakePointOnCurve(theRefLine1, theRefLine2)
    RaiseIfFailed("MakePointOnLinesIntersection", BasicOp)
    return anObj

## Create a tangent, corresponding to the given parameter on the given curve.
#  @param theRefCurve The referenced curve.
#  @param theParameter Value of parameter on the referenced curve.
#  @return New GEOM_Object, containing the created tangent.
def MakeTangentOnCurve(theRefCurve, theParameter):
    anObj = BasicOp.MakeTangentOnCurve(theRefCurve, theParameter)
    RaiseIfFailed("MakeTangentOnCurve", BasicOp)
    return anObj

## Create a vector with the given components.
#  @param theDX X component of the vector.
#  @param theDY Y component of the vector.
#  @param theDZ Z component of the vector.
#  @return New GEOM_Object, containing the created vector.
#
#  Example: see GEOM_TestAll.py
def MakeVectorDXDYDZ(theDX, theDY, theDZ):
    anObj = BasicOp.MakeVectorDXDYDZ(theDX, theDY, theDZ)
    RaiseIfFailed("MakeVectorDXDYDZ", BasicOp)
    return anObj

## Create a vector between two points.
#  @param thePnt1 Start point for the vector.
#  @param thePnt2 End point for the vector.
#  @return New GEOM_Object, containing the created vector.

#  Example: see GEOM_TestAll.py
def MakeVector(thePnt1, thePnt2):
    anObj = BasicOp.MakeVectorTwoPnt(thePnt1, thePnt2)
    RaiseIfFailed("MakeVectorTwoPnt", BasicOp)
    return anObj

## Create a line, passing through the given point
#  and parrallel to the given direction
#  @param thePnt Point. The resulting line will pass through it.
#  @param theDir Direction. The resulting line will be parallel to it.
#  @return New GEOM_Object, containing the created line.
#
#  Example: see GEOM_TestAll.py
def MakeLine(thePnt, theDir):
    anObj = BasicOp.MakeLine(thePnt, theDir)
    RaiseIfFailed("MakeLine", BasicOp)
    return anObj

## Create a line, passing through the given points
#  @param thePnt1 First of two points, defining the line.
#  @param thePnt2 Second of two points, defining the line.
#  @return New GEOM_Object, containing the created line.
#
#  Example: see GEOM_TestAll.py
def MakeLineTwoPnt(thePnt1, thePnt2):
    anObj = BasicOp.MakeLineTwoPnt(thePnt1, thePnt2)
    RaiseIfFailed("MakeLineTwoPnt", BasicOp)
    return anObj

## Create a line on two faces intersection. 
#  @param theFace1 First of two faces, defining the line.
#  @param theFace2 Second of two faces, defining the line.
#  @return New GEOM_Object, containing the created line.
#
#  Example: see GEOM_TestAll.py
def MakeLineTwoFaces(theFace1, theFace2):
    anObj = BasicOp.MakeLineTwoFaces(theFace1, theFace2)
    RaiseIfFailed("MakeLineTwoFaces", BasicOp)
    return anObj

## Create a plane, passing through the given point
#  and normal to the given vector.
#  @param thePnt Point, the plane has to pass through.
#  @param theVec Vector, defining the plane normal direction.
#  @param theTrimSize Half size of a side of quadrangle face, representing the plane.
#  @return New GEOM_Object, containing the created plane.
#
#  Example: see GEOM_TestAll.py
def MakePlane(thePnt, theVec, theTrimSize):
    anObj = BasicOp.MakePlanePntVec(thePnt, theVec, theTrimSize)
    RaiseIfFailed("MakePlanePntVec", BasicOp)
    return anObj

## Create a plane, passing through the three given points
#  @param thePnt1 First of three points, defining the plane.
#  @param thePnt2 Second of three points, defining the plane.
#  @param thePnt3 Fird of three points, defining the plane.
#  @param theTrimSize Half size of a side of quadrangle face, representing the plane.
#  @return New GEOM_Object, containing the created plane.
#
#  Example: see GEOM_TestAll.py
def MakePlaneThreePnt(thePnt1, thePnt2, thePnt3, theTrimSize):
    anObj = BasicOp.MakePlaneThreePnt(thePnt1, thePnt2, thePnt3, theTrimSize)
    RaiseIfFailed("MakePlaneThreePnt", BasicOp)
    return anObj

## Create a plane, similar to the existing one, but with another size of representing face.
#  @param theFace Referenced plane or LCS(Marker).
#  @param theTrimSize New half size of a side of quadrangle face, representing the plane.
#  @return New GEOM_Object, containing the created plane.
#
#  Example: see GEOM_TestAll.py
def MakePlaneFace(theFace, theTrimSize):
    anObj = BasicOp.MakePlaneFace(theFace, theTrimSize)
    RaiseIfFailed("MakePlaneFace", BasicOp)
    return anObj

## Create a local coordinate system.
#  @param OX,OY,OZ Three coordinates of coordinate system origin.
#  @param XDX,XDY,XDZ Three components of OX direction
#  @param YDX,YDY,YDZ Three components of OY direction
#  @return New GEOM_Object, containing the created coordinate system.
#
#  Example: see GEOM_TestAll.py
def MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ):
    anObj = BasicOp.MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ)
    RaiseIfFailed("MakeMarker", BasicOp)
    return anObj

## Create a local coordinate system.
#  @param theOrigin Point of coordinate system origin.
#  @param theXVec Vector of X direction
#  @param theYVec Vector of Y direction
#  @return New GEOM_Object, containing the created coordinate system.
def MakeMarkerPntTwoVec(theOrigin, theXVec, theYVec):
    O = PointCoordinates( theOrigin )
    OXOY = []
    for vec in [ theXVec, theYVec ]:
        v1, v2 = SubShapeAll( vec, ShapeType["VERTEX"] )
        p1 = PointCoordinates( v1 )
        p2 = PointCoordinates( v2 )
        for i in range( 0, 3 ):
             OXOY.append( p2[i] - p1[i] )
    #
    anObj = BasicOp.MakeMarker( O[0], O[1], O[2],
                                OXOY[0], OXOY[1], OXOY[2],
                                OXOY[3], OXOY[4], OXOY[5], )
    RaiseIfFailed("MakeMarker", BasicOp)
    return anObj

# -----------------------------------------------------------------------------
# Curves
# -----------------------------------------------------------------------------

##  Create an arc of circle, passing through three given points.
#  @param thePnt1 Start point of the arc.
#  @param thePnt2 Middle point of the arc.
#  @param thePnt3 End point of the arc.
#  @return New GEOM_Object, containing the created arc.
#
#  Example: see GEOM_TestAll.py
def MakeArc(thePnt1, thePnt2, thePnt3):
    anObj = CurvesOp.MakeArc(thePnt1, thePnt2, thePnt3)
    RaiseIfFailed("MakeArc", CurvesOp)
    return anObj

##  Create an arc of circle from a center and 2 points.
#  @param thePnt1 Center of the arc
#  @param thePnt2 Start point of the arc. (Gives also the radius of the arc)
#  @param thePnt3 End point of the arc (Gives also a direction)
#  @return New GEOM_Object, containing the created arc.
#
#  Example: see GEOM_TestAll.py
def MakeArcCenter(thePnt1, thePnt2, thePnt3,theSense):
    anObj = CurvesOp.MakeArcCenter(thePnt1, thePnt2, thePnt3,theSense)
    RaiseIfFailed("MakeArcCenter", CurvesOp)
    return anObj

## Create a circle with given center, normal vector and radius.
#  @param thePnt Circle center.
#  @param theVec Vector, normal to the plane of the circle.
#  @param theR Circle radius.
#  @return New GEOM_Object, containing the created circle.
#
#  Example: see GEOM_TestAll.py
def MakeCircle(thePnt, theVec, theR):
    anObj = CurvesOp.MakeCirclePntVecR(thePnt, theVec, theR)
    RaiseIfFailed("MakeCirclePntVecR", CurvesOp)
    return anObj

## Create a circle, passing through three given points
#  @param thePnt1,thePnt2,thePnt3 Points, defining the circle.
#  @return New GEOM_Object, containing the created circle.
#
#  Example: see GEOM_TestAll.py
def MakeCircleThreePnt(thePnt1, thePnt2, thePnt3):
    anObj = CurvesOp.MakeCircleThreePnt(thePnt1, thePnt2, thePnt3)
    RaiseIfFailed("MakeCircleThreePnt", CurvesOp)
    return anObj

## Create a circle, with given point1 as center,
#  passing through the point2 as radius and laying in the plane,
#  defined by all three given points.
#  @param thePnt1,thePnt2,thePnt3 Points, defining the circle.
#  @return New GEOM_Object, containing the created circle.
#
#  Example: see GEOM_example6.py
def MakeCircleCenter2Pnt(thePnt1, thePnt2, thePnt3):
    anObj = CurvesOp.MakeCircleCenter2Pnt(thePnt1, thePnt2, thePnt3)
    RaiseIfFailed("MakeCircleCenter2Pnt", CurvesOp)
    return anObj

## Create an ellipse with given center, normal vector and radiuses.
#  @param thePnt Ellipse center.
#  @param theVec Vector, normal to the plane of the ellipse.
#  @param theRMajor Major ellipse radius.
#  @param theRMinor Minor ellipse radius.
#  @return New GEOM_Object, containing the created ellipse.
#
#  Example: see GEOM_TestAll.py
def MakeEllipse(thePnt, theVec, theRMajor, theRMinor):
    anObj = CurvesOp.MakeEllipse(thePnt, theVec, theRMajor, theRMinor)
    RaiseIfFailed("MakeEllipse", CurvesOp)
    return anObj

## Create a polyline on the set of points.
#  @param thePoints Sequence of points for the polyline.
#  @return New GEOM_Object, containing the created polyline.
#
#  Example: see GEOM_TestAll.py
def MakePolyline(thePoints):
    anObj = CurvesOp.MakePolyline(thePoints)
    RaiseIfFailed("MakePolyline", CurvesOp)
    return anObj

## Create bezier curve on the set of points.
#  @param thePoints Sequence of points for the bezier curve.
#  @return New GEOM_Object, containing the created bezier curve.
#
#  Example: see GEOM_TestAll.py
def MakeBezier(thePoints):
    anObj = CurvesOp.MakeSplineBezier(thePoints)
    RaiseIfFailed("MakeSplineBezier", CurvesOp)
    return anObj

## Create B-Spline curve on the set of points.
#  @param thePoints Sequence of points for the B-Spline curve.
#  @return New GEOM_Object, containing the created B-Spline curve.
#
#  Example: see GEOM_TestAll.py
def MakeInterpol(thePoints):
    anObj = CurvesOp.MakeSplineInterpolation(thePoints)
    RaiseIfFailed("MakeSplineInterpolation", CurvesOp)
    return anObj

## Create a sketcher (wire or face), following the textual description,
#  passed through \a theCommand argument. \n
#  Edges of the resulting wire or face will be arcs of circles and/or linear segments. \n
#  Format of the description string have to be the following:
#
#  "Sketcher[:F x1 y1]:CMD[:CMD[:CMD...]]"
#
#  Where:
#  - x1, y1 are coordinates of the first sketcher point (zero by default),
#  - CMD is one of
#     - "R angle" : Set the direction by angle
#     - "D dx dy" : Set the direction by DX & DY
#     .
#       \n
#     - "TT x y" : Create segment by point at X & Y
#     - "T dx dy" : Create segment by point with DX & DY
#     - "L length" : Create segment by direction & Length
#     - "IX x" : Create segment by direction & Intersect. X
#     - "IY y" : Create segment by direction & Intersect. Y
#     .
#       \n
#     - "C radius length" : Create arc by direction, radius and length(in degree)
#     .
#       \n
#     - "WW" : Close Wire (to finish)
#     - "WF" : Close Wire and build face (to finish)
#
#  @param theCommand String, defining the sketcher in local
#                    coordinates of the working plane.
#  @param theWorkingPlane Nine double values, defining origin,
#                         OZ and OX directions of the working plane.
#  @return New GEOM_Object, containing the created wire.
#
#  Example: see GEOM_TestAll.py
def MakeSketcher(theCommand, theWorkingPlane = [0,0,0, 0,0,1, 1,0,0]):
    anObj = CurvesOp.MakeSketcher(theCommand, theWorkingPlane)
    RaiseIfFailed("MakeSketcher", CurvesOp)
    return anObj

## Create a sketcher (wire or face), following the textual description,
#  passed through \a theCommand argument. \n
#  For format of the description string see the previous method.\n
#  @param theCommand String, defining the sketcher in local
#                    coordinates of the working plane.
#  @param theWorkingPlane Planar Face or LCS(Marker) of the working plane.
#  @return New GEOM_Object, containing the created wire.
def MakeSketcherOnPlane(theCommand, theWorkingPlane):
    anObj = CurvesOp.MakeSketcherOnPlane(theCommand, theWorkingPlane)
    RaiseIfFailed("MakeSketcherOnPlane", CurvesOp)
    return anObj

# -----------------------------------------------------------------------------
# Create 3D Primitives
# -----------------------------------------------------------------------------

## Create a box by coordinates of two opposite vertices.
#
#  Example: see GEOM_TestAll.py
def MakeBox(x1,y1,z1,x2,y2,z2):
    pnt1 = MakeVertex(x1,y1,z1)
    pnt2 = MakeVertex(x2,y2,z2)
    return MakeBoxTwoPnt(pnt1,pnt2)

## Create a box with specified dimensions along the coordinate axes
#  and with edges, parallel to the coordinate axes.
#  Center of the box will be at point (DX/2, DY/2, DZ/2).
#  @param theDX Length of Box edges, parallel to OX axis.
#  @param theDY Length of Box edges, parallel to OY axis.
#  @param theDZ Length of Box edges, parallel to OZ axis.
#  @return New GEOM_Object, containing the created box.
#
#  Example: see GEOM_TestAll.py
def MakeBoxDXDYDZ(theDX, theDY, theDZ):
    anObj = PrimOp.MakeBoxDXDYDZ(theDX, theDY, theDZ)
    RaiseIfFailed("MakeBoxDXDYDZ", PrimOp)
    return anObj

## Create a box with two specified opposite vertices,
#  and with edges, parallel to the coordinate axes
#  @param thePnt1 First of two opposite vertices.
#  @param thePnt2 Second of two opposite vertices.
#  @return New GEOM_Object, containing the created box.
#
#  Example: see GEOM_TestAll.py
def MakeBoxTwoPnt(thePnt1, thePnt2):
    anObj = PrimOp.MakeBoxTwoPnt(thePnt1, thePnt2)
    RaiseIfFailed("MakeBoxTwoPnt", PrimOp)
    return anObj

## Create a cylinder with given base point, axis, radius and height.
#  @param thePnt Central point of cylinder base.
#  @param theAxis Cylinder axis.
#  @param theR Cylinder radius.
#  @param theH Cylinder height.
#  @return New GEOM_Object, containing the created cylinder.
#
#  Example: see GEOM_TestAll.py
def MakeCylinder(thePnt, theAxis, theR, theH):
    anObj = PrimOp.MakeCylinderPntVecRH(thePnt, theAxis, theR, theH)
    RaiseIfFailed("MakeCylinderPntVecRH", PrimOp)
    return anObj

## Create a cylinder with given radius and height at
#  the origin of coordinate system. Axis of the cylinder
#  will be collinear to the OZ axis of the coordinate system.
#  @param theR Cylinder radius.
#  @param theH Cylinder height.
#  @return New GEOM_Object, containing the created cylinder.
#
#  Example: see GEOM_TestAll.py
def MakeCylinderRH(theR, theH):
    anObj = PrimOp.MakeCylinderRH(theR, theH)
    RaiseIfFailed("MakeCylinderRH", PrimOp)
    return anObj

## Create a sphere with given center and radius.
#  @param thePnt Sphere center.
#  @param theR Sphere radius.
#  @return New GEOM_Object, containing the created sphere.
#
#  Example: see GEOM_TestAll.py
def MakeSpherePntR(thePnt, theR):
    anObj = PrimOp.MakeSpherePntR(thePnt, theR)
    RaiseIfFailed("MakeSpherePntR", PrimOp)
    return anObj

## Create a sphere with given center and radius.
#  @param x,y,z Coordinates of sphere center.
#  @param theR Sphere radius.
#  @return New GEOM_Object, containing the created sphere.
#
#  Example: see GEOM_TestAll.py
def MakeSphere(x, y, z, theR):
    point = MakeVertex(x, y, z)
    anObj = MakeSpherePntR(point, theR)
    return anObj

## Create a sphere with given radius at the origin of coordinate system.
#  @param theR Sphere radius.
#  @return New GEOM_Object, containing the created sphere.
#
#  Example: see GEOM_TestAll.py
def MakeSphereR(theR):
    anObj = PrimOp.MakeSphereR(theR)
    RaiseIfFailed("MakeSphereR", PrimOp)
    return anObj

## Create a cone with given base point, axis, height and radiuses.
#  @param thePnt Central point of the first cone base.
#  @param theAxis Cone axis.
#  @param theR1 Radius of the first cone base.
#  @param theR2 Radius of the second cone base.
#    \note If both radiuses are non-zero, the cone will be truncated.
#    \note If the radiuses are equal, a cylinder will be created instead.
#  @param theH Cone height.
#  @return New GEOM_Object, containing the created cone.
#
#  Example: see GEOM_TestAll.py
def MakeCone(thePnt, theAxis, theR1, theR2, theH):
    anObj = PrimOp.MakeConePntVecR1R2H(thePnt, theAxis, theR1, theR2, theH)
    RaiseIfFailed("MakeConePntVecR1R2H", PrimOp)
    return anObj

## Create a cone with given height and radiuses at
#  the origin of coordinate system. Axis of the cone will
#  be collinear to the OZ axis of the coordinate system.
#  @param theR1 Radius of the first cone base.
#  @param theR2 Radius of the second cone base.
#    \note If both radiuses are non-zero, the cone will be truncated.
#    \note If the radiuses are equal, a cylinder will be created instead.
#  @param theH Cone height.
#  @return New GEOM_Object, containing the created cone.
#
#  Example: see GEOM_TestAll.py
def MakeConeR1R2H(theR1, theR2, theH):
    anObj = PrimOp.MakeConeR1R2H(theR1, theR2, theH)
    RaiseIfFailed("MakeConeR1R2H", PrimOp)
    return anObj

## Create a torus with given center, normal vector and radiuses.
#  @param thePnt Torus central point.
#  @param theVec Torus axis of symmetry.
#  @param theRMajor Torus major radius.
#  @param theRMinor Torus minor radius.
#  @return New GEOM_Object, containing the created torus.
#
#  Example: see GEOM_TestAll.py
def MakeTorus(thePnt, theVec, theRMajor, theRMinor):
    anObj = PrimOp.MakeTorusPntVecRR(thePnt, theVec, theRMajor, theRMinor)
    RaiseIfFailed("MakeTorusPntVecRR", PrimOp)
    return anObj

## Create a torus with given radiuses at the origin of coordinate system.
#  @param theRMajor Torus major radius.
#  @param theRMinor Torus minor radius.
#  @return New GEOM_Object, containing the created torus.
#
#  Example: see GEOM_TestAll.py
def MakeTorusRR(theRMajor, theRMinor):
    anObj = PrimOp.MakeTorusRR(theRMajor, theRMinor)
    RaiseIfFailed("MakeTorusRR", PrimOp)
    return anObj

## Create a shape by extrusion of the base shape along a vector, defined by two points.
#  @param theBase Base shape to be extruded.
#  @param thePoint1 First end of extrusion vector.
#  @param thePoint2 Second end of extrusion vector.
#  @return New GEOM_Object, containing the created prism.
#
#  Example: see GEOM_TestAll.py
def MakePrism(theBase, thePoint1, thePoint2):
    anObj = PrimOp.MakePrismTwoPnt(theBase, thePoint1, thePoint2)
    RaiseIfFailed("MakePrismTwoPnt", PrimOp)
    return anObj
## The same prism but in two directions forward&backward.
def MakePrism2Ways(theBase, thePoint1, thePoint2):
    anObj = PrimOp.MakePrismTwoPnt2Ways(theBase, thePoint1, thePoint2)
    RaiseIfFailed("MakePrismTwoPnt2Ways", PrimOp)
    return anObj

## Create a shape by extrusion of the base shape along the vector,
#  i.e. all the space, transfixed by the base shape during its translation
#  along the vector on the given distance.
#  @param theBase Base shape to be extruded.
#  @param theVec Direction of extrusion.
#  @param theH Prism dimension along theVec.
#  @return New GEOM_Object, containing the created prism.
#
#  Example: see GEOM_TestAll.py
def MakePrismVecH(theBase, theVec, theH):
    anObj = PrimOp.MakePrismVecH(theBase, theVec, theH)
    RaiseIfFailed("MakePrismVecH", PrimOp)
    return anObj

## Create a shape by extrusion of the base shape along the vector,
#  i.e. all the space, transfixed by the base shape during its translation
#  along the vector on the given distance in 2 Ways (forward/backward) .
#  @param theBase Base shape to be extruded.
#  @param theVec Direction of extrusion.
#  @param theH Prism dimension along theVec in forward direction.
#  @return New GEOM_Object, containing the created prism.
#
#  Example: see GEOM_TestAll.py
def MakePrismVecH2Ways(theBase, theVec, theH):
    anObj = PrimOp.MakePrismVecH2Ways(theBase, theVec, theH)
    RaiseIfFailed("MakePrismVecH2Ways", PrimOp)
    return anObj

## Create a shape by extrusion of the base shape along
#  the path shape. The path shape can be a wire or an edge.
#  @param theBase Base shape to be extruded.
#  @param thePath Path shape to extrude the base shape along it.
#  @return New GEOM_Object, containing the created pipe.
#
#  Example: see GEOM_TestAll.py
def MakePipe(theBase, thePath):
    anObj = PrimOp.MakePipe(theBase, thePath)
    RaiseIfFailed("MakePipe", PrimOp)
    return anObj

## Create a shape by revolution of the base shape around the axis
#  on the given angle, i.e. all the space, transfixed by the base
#  shape during its rotation around the axis on the given angle.
#  @param theBase Base shape to be rotated.
#  @param theAxis Rotation axis.
#  @param theAngle Rotation angle in radians.
#  @return New GEOM_Object, containing the created revolution.
#
#  Example: see GEOM_TestAll.py
def MakeRevolution(theBase, theAxis, theAngle):
    anObj = PrimOp.MakeRevolutionAxisAngle(theBase, theAxis, theAngle)
    RaiseIfFailed("MakeRevolutionAxisAngle", PrimOp)
    return anObj
## The Same Revolution but in both ways forward&backward.
def MakeRevolution2Ways(theBase, theAxis, theAngle):
    anObj = PrimOp.MakeRevolutionAxisAngle2Ways(theBase, theAxis, theAngle)
    RaiseIfFailed("MakeRevolutionAxisAngle2Ways", PrimOp)
    return anObj

## Create a shell or solid passing through set of sections.Sections should be wires,edges or vertices.
#  @param theSeqSections - set of specified sections.
#  @param theModeSolid - mode defining building solid or shell
#  @param thePreci - precision 3D used for smoothing by default 1.e-6
#  @param theRuled - mode defining type of the result surfaces (ruled or smoothed).
#  @return New GEOM_Object, containing the created shell or solid.
#
#  Example: see GEOM_TestAll.py
def MakeThruSections(theSeqSections,theModeSolid,thePreci,theRuled):
    anObj = PrimOp.MakeThruSections(theSeqSections,theModeSolid,thePreci,theRuled)
    RaiseIfFailed("MakeThruSections", PrimOp)
    return anObj

## Create a shape by extrusion of the profile shape along
#  the path shape. The path shape can be a wire or an edge.
#  the several profiles can be specified in the several locations of path.	
#  @param theSeqBases - list of  Bases shape to be extruded.
#  @param theLocations - list of locations on the path corresponding
#                        specified list of the Bases shapes. Number of locations
#                        should be equal to number of bases or list of locations can be empty.
#  @param thePath - Path shape to extrude the base shape along it.
#  @param theWithContact - the mode defining that the section is translated to be in
#                          contact with the spine.
#  @param - WithCorrection - defining that the section is rotated to be
#                            orthogonal to the spine tangent in the correspondent point
#  @return New GEOM_Object, containing the created pipe.
#
def MakePipeWithDifferentSections(theSeqBases, theLocations,thePath,theWithContact,theWithCorrection):
    anObj = PrimOp.MakePipeWithDifferentSections(theSeqBases, theLocations,thePath,theWithContact,theWithCorrection)
    RaiseIfFailed("MakePipeWithDifferentSections", PrimOp)
    return anObj

## Create a shape by extrusion of the profile shape along
#  the path shape. The path shape can be a shell or a face.
#  the several profiles can be specified in the several locations of path.	
#  @param theSeqBases - list of  Bases shape to be extruded.
#  @param theSeqSubBases - list of corresponding subshapes of section shapes.
#  @param theLocations - list of locations on the path corresponding
#                        specified list of the Bases shapes. Number of locations
#                        should be equal to number of bases. First and last
#                        locations must be coincided with first and last vertexes
#                        of path correspondingly.
#  @param thePath - Path shape to extrude the base shape along it.
#  @param theWithContact - the mode defining that the section is translated to be in
#                          contact with the spine.
#  @param - WithCorrection - defining that the section is rotated to be
#                            orthogonal to the spine tangent in the correspondent point
#  @return New GEOM_Object, containing the created solids.
#
def MakePipeWithShellSections(theSeqBases, theSeqSubBases,
                              theLocations, thePath,
                              theWithContact, theWithCorrection):
    anObj = PrimOp.MakePipeWithShellSections(theSeqBases, theSeqSubBases,
                                             theLocations, thePath,
                                             theWithContact, theWithCorrection)
    RaiseIfFailed("MakePipeWithShellSections", PrimOp)
    return anObj

def MakePipeWithShellSectionsBySteps(theSeqBases, theSeqSubBases,
                                     theLocations, thePath,
                                     theWithContact, theWithCorrection):
    res = []
    nbsect = len(theSeqBases)
    nbsubsect = len(theSeqSubBases)
    #print "nbsect = ",nbsect
    for i in range(1,nbsect):
        #print "  i = ",i
        tmpSeqBases = [ theSeqBases[i-1], theSeqBases[i] ]
        tmpLocations = [ theLocations[i-1], theLocations[i] ]
        tmpSeqSubBases = []
        if nbsubsect>0: tmpSeqSubBases = [ theSeqSubBases[i-1], theSeqSubBases[i] ]
        anObj = PrimOp.MakePipeWithShellSections(tmpSeqBases, tmpSeqSubBases,
                                                 tmpLocations, thePath,
                                                 theWithContact, theWithCorrection)
        if PrimOp.IsDone() == 0:
            print "Problems with pipe creation between ",i," and ",i+1," sections"
            RaiseIfFailed("MakePipeWithShellSections", PrimOp)
            break
        else:
            print "Pipe between ",i," and ",i+1," sections is OK"
            res.append(anObj)
            pass
        pass

    resc = MakeCompound(res)
    #resc = MakeSewing(res, 0.001)
    #print "resc: ",resc
    return resc


## Create solids between given sections
#  @param theSeqBases - list of sections (shell or face).
#  @param theLocations - list of corresponding vertexes
#  @return New GEOM_Object, containing the created solids.
#
def MakePipeShellsWithoutPath(theSeqBases, theLocations):
    anObj = PrimOp.MakePipeShellsWithoutPath(theSeqBases, theLocations)
    RaiseIfFailed("MakePipeShellsWithoutPath", PrimOp)
    return anObj


# -----------------------------------------------------------------------------
# Create base shapes
# -----------------------------------------------------------------------------

## Create a linear edge with specified ends.
#  @param thePnt1 Point for the first end of edge.
#  @param thePnt2 Point for the second end of edge.
#  @return New GEOM_Object, containing the created edge.
#
#  Example: see GEOM_TestAll.py
def MakeEdge(thePnt1, thePnt2):
    anObj = ShapesOp.MakeEdge(thePnt1, thePnt2)
    RaiseIfFailed("MakeEdge", ShapesOp)
    return anObj

## Create a wire from the set of edges and wires.
#  @param theEdgesAndWires List of edges and/or wires.
#  @return New GEOM_Object, containing the created wire.
#
#  Example: see GEOM_TestAll.py
def MakeWire(theEdgesAndWires):
    anObj = ShapesOp.MakeWire(theEdgesAndWires)
    RaiseIfFailed("MakeWire", ShapesOp)
    return anObj

## Create a face on the given wire.
#  @param theWire closed Wire or Edge to build the face on.
#  @param isPlanarWanted If TRUE, only planar face will be built.
#                        If impossible, NULL object will be returned.
#  @return New GEOM_Object, containing the created face.
#
#  Example: see GEOM_TestAll.py
def MakeFace(theWire, isPlanarWanted):
    anObj = ShapesOp.MakeFace(theWire, isPlanarWanted)
    RaiseIfFailed("MakeFace", ShapesOp)
    return anObj

## Create a face on the given wires set.
#  @param theWires List of closed wires or edges to build the face on.
#  @param isPlanarWanted If TRUE, only planar face will be built.
#                        If impossible, NULL object will be returned.
#  @return New GEOM_Object, containing the created face.
#
#  Example: see GEOM_TestAll.py
def MakeFaceWires(theWires, isPlanarWanted):
    anObj = ShapesOp.MakeFaceWires(theWires, isPlanarWanted)
    RaiseIfFailed("MakeFaceWires", ShapesOp)
    return anObj

## Shortcut to MakeFaceWires()
#
#  Example: see GEOM_TestOthers.py
def MakeFaces(theWires, isPlanarWanted):
    anObj = MakeFaceWires(theWires, isPlanarWanted)
    return anObj

## Create a shell from the set of faces and shells.
#  @param theFacesAndShells List of faces and/or shells.
#  @return New GEOM_Object, containing the created shell.
#
#  Example: see GEOM_TestAll.py
def MakeShell(theFacesAndShells):
    anObj = ShapesOp.MakeShell(theFacesAndShells)
    RaiseIfFailed("MakeShell", ShapesOp)
    return anObj

## Create a solid, bounded by the given shells.
#  @param theShells Sequence of bounding shells.
#  @return New GEOM_Object, containing the created solid.
#
#  Example: see GEOM_TestAll.py
def MakeSolid(theShells):
    anObj = ShapesOp.MakeSolidShells(theShells)
    RaiseIfFailed("MakeSolidShells", ShapesOp)
    return anObj

## Create a compound of the given shapes.
#  @param theShapes List of shapes to put in compound.
#  @return New GEOM_Object, containing the created compound.
#
#  Example: see GEOM_TestAll.py
def MakeCompound(theShapes):
    anObj = ShapesOp.MakeCompound(theShapes)
    RaiseIfFailed("MakeCompound", ShapesOp)
    return anObj

## Gives quantity of faces in the given shape.
#  @param theShape Shape to count faces of.
#  @return Quantity of faces.
#
#  Example: see GEOM_TestOthers.py
def NumberOfFaces(theShape):
    nb_faces = ShapesOp.NumberOfFaces(theShape)
    RaiseIfFailed("NumberOfFaces", ShapesOp)
    return nb_faces

## Gives quantity of edges in the given shape.
#  @param theShape Shape to count edges of.
#  @return Quantity of edges.
#
#  Example: see GEOM_TestOthers.py
def NumberOfEdges(theShape):
    nb_edges = ShapesOp.NumberOfEdges(theShape)
    RaiseIfFailed("NumberOfEdges", ShapesOp)
    return nb_edges

## Reverses an orientation the given shape.
#  @param theShape Shape to be reversed.
#  @return The reversed copy of theShape.
#
#  Example: see GEOM_TestAll.py
def ChangeOrientation(theShape):
    anObj = ShapesOp.ChangeOrientation(theShape)
    RaiseIfFailed("ChangeOrientation", ShapesOp)
    return anObj

## Shortcut to ChangeOrientation()
#
#  Example: see GEOM_TestOthers.py
def OrientationChange(theShape):
    anObj = ChangeOrientation(theShape)
    return anObj

## Retrieve all free faces from the given shape.
#  Free face is a face, which is not shared between two shells of the shape.
#  @param theShape Shape to find free faces in.
#  @return List of IDs of all free faces, contained in theShape.
#
#  Example: see GEOM_TestOthers.py
def GetFreeFacesIDs(theShape):
    anIDs = ShapesOp.GetFreeFacesIDs(theShape)
    RaiseIfFailed("GetFreeFacesIDs", ShapesOp)
    return anIDs

## Get all sub-shapes of theShape1 of the given type, shared with theShape2.
#  @param theShape1 Shape to find sub-shapes in.
#  @param theShape2 Shape to find shared sub-shapes with.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @return List of sub-shapes of theShape1, shared with theShape2.
#
#  Example: see GEOM_TestOthers.py
def GetSharedShapes(theShape1, theShape2, theShapeType):
    aList = ShapesOp.GetSharedShapes(theShape1, theShape2, theShapeType)
    RaiseIfFailed("GetSharedShapes", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified plane by the certain way, defined through \a theState parameter.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theAx1 Vector (or line, or linear edge), specifying normal
#                direction and location of the plane to find shapes on.
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnPlane(theShape, theShapeType, theAx1, theState):
    aList = ShapesOp.GetShapesOnPlane(theShape, theShapeType, theAx1, theState)
    RaiseIfFailed("GetShapesOnPlane", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnPlaneIDs(theShape, theShapeType, theAx1, theState):
    aList = ShapesOp.GetShapesOnPlaneIDs(theShape, theShapeType, theAx1, theState)
    RaiseIfFailed("GetShapesOnPlaneIDs", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified plane by the certain way, defined through \a theState parameter.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theAx1 Vector (or line, or linear edge), specifying normal
#                direction of the plane to find shapes on.
#  @param thePnt Point specifying location of the plane to find shapes on.
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnPlaneWithLocation(theShape, theShapeType, theAx1, thePnt, theState):
    aList = ShapesOp.GetShapesOnPlaneWithLocation(theShape, theShapeType, theAx1, thePnt, theState)
    RaiseIfFailed("GetShapesOnPlaneWithLocation", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnPlaneWithLocationIDs(theShape, theShapeType, theAx1, thePnt, theState):
    aList = ShapesOp.GetShapesOnPlaneWithLocationIDs(theShape, theShapeType, theAx1, thePnt, theState)
    RaiseIfFailed("GetShapesOnPlaneWithLocationIDs", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified cylinder by the certain way, defined through \a theState parameter.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theAxis Vector (or line, or linear edge), specifying
#                 axis of the cylinder to find shapes on.
#  @param theRadius Radius of the cylinder to find shapes on.
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnCylinder(theShape, theShapeType, theAxis, theRadius, theState):
    aList = ShapesOp.GetShapesOnCylinder(theShape, theShapeType, theAxis, theRadius, theState)
    RaiseIfFailed("GetShapesOnCylinder", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnCylinderIDs(theShape, theShapeType, theAxis, theRadius, theState):
    aList = ShapesOp.GetShapesOnCylinderIDs(theShape, theShapeType, theAxis, theRadius, theState)
    RaiseIfFailed("GetShapesOnCylinderIDs", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified sphere by the certain way, defined through \a theState parameter.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theCenter Point, specifying center of the sphere to find shapes on.
#  @param theRadius Radius of the sphere to find shapes on.
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnSphere(theShape, theShapeType, theCenter, theRadius, theState):
    aList = ShapesOp.GetShapesOnSphere(theShape, theShapeType, theCenter, theRadius, theState)
    RaiseIfFailed("GetShapesOnSphere", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnSphereIDs(theShape, theShapeType, theCenter, theRadius, theState):
    aList = ShapesOp.GetShapesOnSphereIDs(theShape, theShapeType, theCenter, theRadius, theState)
    RaiseIfFailed("GetShapesOnSphereIDs", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified quadrangle by the certain way, defined through \a theState parameter.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theTopLeftPoint Point, specifying top left corner of a quadrangle
#  @param theTopRigthPoint Point, specifying top right corner of a quadrangle
#  @param theBottomLeftPoint Point, specifying bottom left corner of a quadrangle
#  @param theBottomRigthPoint Point, specifying bottom right corner of a quadrangle
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnQuadrangle(theShape, theShapeType, theTopLeftPoint, theTopRigthPoint, theBottomLeftPoint, theBottomRigthPoint, theState):
    aList = ShapesOp.GetShapesOnQuadrangle(theShape, theShapeType, theTopLeftPoint, theTopRigthPoint, theBottomLeftPoint, theBottomRigthPoint, theState)
    RaiseIfFailed("GetShapesOnQuadrangle", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def GetShapesOnQuadrangleIDs(theShape, theShapeType, theTopLeftPoint, theTopRigthPoint, theBottomLeftPoint, theBottomRigthPoint, theState):
    aList = ShapesOp.GetShapesOnQuadrangleIDs(theShape, theShapeType, theTopLeftPoint, theTopRigthPoint, theBottomLeftPoint, theBottomRigthPoint, theState)
    RaiseIfFailed("GetShapesOnQuadrangleIDs", ShapesOp)
    return aList

## Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
#  the specified \a theBox by the certain way, defined through \a theState parameter.
#  @param theBox Shape for relative comparing.
#  @param theShape Shape to find sub-shapes of.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @param theState The state of the subshapes to find. It can be one of
#   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
#  @return List of all found sub-shapes.
#
def GetShapesOnBox(theBox, theShape, theShapeType, theState):
    aList = ShapesOp.GetShapesOnBox(theBox, theShape, theShapeType, theState)
    RaiseIfFailed("GetShapesOnBox", ShapesOp)
    return aList

## Works like the above method, but returns list of sub-shapes indices
#
def GetShapesOnBoxIDs(theBox, theShape, theShapeType, theState):
    aList = ShapesOp.GetShapesOnBoxIDs(theBox, theShape, theShapeType, theState)
    RaiseIfFailed("GetShapesOnBoxIDs", ShapesOp)
    return aList

## Get sub-shape(s) of theShapeWhere, which are
#  coincident with \a theShapeWhat or could be a part of it.
#  @param theShapeWhere Shape to find sub-shapes of.
#  @param theShapeWhat Shape, specifying what to find (must be in the
#                      building history of the ShapeWhere).
#  @return Group of all found sub-shapes or a single found sub-shape.
#
#  Example: see GEOM_TestOthers.py
def GetInPlace(theShapeWhere, theShapeWhat):
    anObj = ShapesOp.GetInPlace(theShapeWhere, theShapeWhat)
    RaiseIfFailed("GetInPlace", ShapesOp)
    return anObj

## Get sub-shape(s) of \a theShapeWhere, which are
#  coincident with \a theShapeWhat or could be a part of it.
#
#  Implementation of this method is based on a saved history of an operation,
#  produced \a theShapeWhere. The \a theShapeWhat must be among this operation's
#  arguments (an argument shape or a sub-shape of an argument shape).
#  The operation could be the Partition or one of boolean operations,
#  performed on simple shapes (not on compounds).
#
#  @param theShapeWhere Shape to find sub-shapes of.
#  @param theShapeWhat Shape, specifying what to find (must be in the
#                      building history of the ShapeWhere).
#  @return Group of all found sub-shapes or a single found sub-shape.
#
#  Example: see GEOM_TestOthers.py
def GetInPlaceByHistory(theShapeWhere, theShapeWhat):
    anObj = ShapesOp.GetInPlaceByHistory(theShapeWhere, theShapeWhat)
    RaiseIfFailed("GetInPlaceByHistory", ShapesOp)
    return anObj

## Get sub-shape of theShapeWhere, which is
#  equal to \a theShapeWhat.
#  @param theShapeWhere Shape to find sub-shape of.
#  @param theShapeWhat Shape, specifying what to find
#                      (must be usual shape).
#  @return New GEOM_Object for found sub-shape.
#
def GetSame(theShapeWhere, theShapeWhat):
    anObj = ShapesOp.GetSame(theShapeWhere, theShapeWhat)
    RaiseIfFailed("GetSame", ShapesOp)
    return anObj

# -----------------------------------------------------------------------------
# Access to sub-shapes by their unique IDs inside the main shape.
# -----------------------------------------------------------------------------

## Obtain a composite sub-shape of <aShape>, composed from sub-shapes
#  of <aShape>, selected by their unique IDs inside <aShape>
#
#  Example: see GEOM_TestAll.py
def GetSubShape(aShape, ListOfID):
    anObj = geom.AddSubShape(aShape,ListOfID)
    return anObj

## Obtain unique ID of sub-shape <aSubShape> inside <aShape>
#
#  Example: see GEOM_TestAll.py
def GetSubShapeID(aShape, aSubShape):
    anID = LocalOp.GetSubShapeIndex(aShape, aSubShape)
    RaiseIfFailed("GetSubShapeIndex", LocalOp)
    return anID

# -----------------------------------------------------------------------------
# Decompose objects
# -----------------------------------------------------------------------------

## Explode a shape on subshapes of a given type.
#  @param theShape Shape to be exploded.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @return List of sub-shapes of type theShapeType, contained in theShape.
#
#  Example: see GEOM_TestAll.py
def SubShapeAll(aShape, aType):
    ListObj = ShapesOp.MakeExplode(aShape,aType,0)
    RaiseIfFailed("MakeExplode", ShapesOp)
    return ListObj

## Explode a shape on subshapes of a given type.
#  @param theShape Shape to be exploded.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @return List of IDs of sub-shapes.
def SubShapeAllIDs(aShape, aType):
    ListObj = ShapesOp.SubShapeAllIDs(aShape,aType,0)
    RaiseIfFailed("SubShapeAllIDs", ShapesOp)
    return ListObj

## Explode a shape on subshapes of a given type.
#  Sub-shapes will be sorted by coordinates of their gravity centers.
#  @param theShape Shape to be exploded.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @return List of sub-shapes of type theShapeType, contained in theShape.
#
#  Example: see GEOM_TestAll.py
def SubShapeAllSorted(aShape, aType):
    ListObj = ShapesOp.MakeExplode(aShape,aType,1)
    RaiseIfFailed("MakeExplode", ShapesOp)
    return ListObj

## Explode a shape on subshapes of a given type.
#  Sub-shapes will be sorted by coordinates of their gravity centers.
#  @param theShape Shape to be exploded.
#  @param theShapeType Type of sub-shapes to be retrieved.
#  @return List of IDs of sub-shapes.
def SubShapeAllSortedIDs(aShape, aType):
    ListIDs = ShapesOp.SubShapeAllIDs(aShape,aType,1)
    RaiseIfFailed("SubShapeAllIDs", ShapesOp)
    return ListIDs

## Obtain a compound of sub-shapes of <aShape>,
#  selected by they indices in list of all sub-shapes of type <aType>.
#  Each index is in range [1, Nb_Sub-Shapes_Of_Given_Type]
#
#  Example: see GEOM_TestAll.py
def SubShape(aShape, aType, ListOfInd):
    ListOfIDs = []
    AllShapeList = SubShapeAll(aShape, aType)
    for ind in ListOfInd:
        ListOfIDs.append(GetSubShapeID(aShape, AllShapeList[ind - 1]))
    anObj = GetSubShape(aShape, ListOfIDs)
    return anObj

## Obtain a compound of sub-shapes of <aShape>,
#  selected by they indices in sorted list of all sub-shapes of type <aType>.
#  Each index is in range [1, Nb_Sub-Shapes_Of_Given_Type]
#
#  Example: see GEOM_TestAll.py
def SubShapeSorted(aShape, aType, ListOfInd):
    ListOfIDs = []
    AllShapeList = SubShapeAllSorted(aShape, aType)
    for ind in ListOfInd:
        ListOfIDs.append(GetSubShapeID(aShape, AllShapeList[ind - 1]))
    anObj = GetSubShape(aShape, ListOfIDs)
    return anObj

# -----------------------------------------------------------------------------
# Healing operations
# -----------------------------------------------------------------------------

## Apply a sequence of Shape Healing operators to the given object.
#  @param theShape Shape to be processed.
#  @param theOperators List of names of operators ("FixShape", "SplitClosedFaces", etc.).
#  @param theParameters List of names of parameters
#                    ("FixShape.Tolerance3d", "SplitClosedFaces.NbSplitPoints", etc.).
#  @param theValues List of values of parameters, in the same order
#                    as parameters are listed in \a theParameters list.
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def ProcessShape(theShape, theOperators, theParameters, theValues):
    anObj = HealOp.ProcessShape(theShape, theOperators, theParameters, theValues)
    RaiseIfFailed("ProcessShape", HealOp)
    return anObj

## Remove faces from the given object (shape).
#  @param theObject Shape to be processed.
#  @param theFaces Indices of faces to be removed, if EMPTY then the method
#                  removes ALL faces of the given object.
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def SuppressFaces(theObject, theFaces):
    anObj = HealOp.SuppressFaces(theObject, theFaces)
    RaiseIfFailed("SuppressFaces", HealOp)
    return anObj

## Sewing of some shapes into single shape.
#
#  Example: see GEOM_TestHealing.py
def MakeSewing(ListShape, theTolerance):
    comp = MakeCompound(ListShape)
    anObj = Sew(comp, theTolerance)
    return anObj

## Sewing of the given object.
#  @param theObject Shape to be processed.
#  @param theTolerance Required tolerance value.
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see MakeSewing() above
def Sew(theObject, theTolerance):
    anObj = HealOp.Sew(theObject, theTolerance)
    RaiseIfFailed("Sew", HealOp)
    return anObj

## Remove internal wires and edges from the given object (face).
#  @param theObject Shape to be processed.
#  @param theWires Indices of wires to be removed, if EMPTY then the method
#                  removes ALL internal wires of the given object.
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def SuppressInternalWires(theObject, theWires):
    anObj = HealOp.RemoveIntWires(theObject, theWires)
    RaiseIfFailed("RemoveIntWires", HealOp)
    return anObj

## Remove internal closed contours (holes) from the given object.
#  @param theObject Shape to be processed.
#  @param theWires Indices of wires to be removed, if EMPTY then the method
#                  removes ALL internal holes of the given object
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def SuppressHoles(theObject, theWires):
    anObj = HealOp.FillHoles(theObject, theWires)
    RaiseIfFailed("FillHoles", HealOp)
    return anObj

## Close an open wire.
#  @param theObject Shape to be processed.
#  @param theWires Indexes of edge(s) and wire(s) to be closed within <VAR>theObject</VAR>'s shape,
#                  if -1, then theObject itself is a wire.
#  @param isCommonVertex If TRUE : closure by creation of a common vertex,
#                        If FALS : closure by creation of an edge between ends.
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def CloseContour(theObject, theWires, isCommonVertex):
    anObj = HealOp.CloseContour(theObject, theWires, isCommonVertex)
    RaiseIfFailed("CloseContour", HealOp)
    return anObj

## Addition of a point to a given edge object.
#  @param theObject Shape to be processed.
#  @param theEdgeIndex Index of edge to be divided within theObject's shape,
#                      if -1, then theObject itself is the edge.
#  @param theValue Value of parameter on edge or length parameter,
#                  depending on \a isByParameter.
#  @param isByParameter If TRUE : \a theValue is treated as a curve parameter [0..1],
#                       if FALSE : \a theValue is treated as a length parameter [0..1]
#  @return New GEOM_Object, containing processed shape.
#
#  Example: see GEOM_TestHealing.py
def DivideEdge(theObject, theEdgeIndex, theValue, isByParameter):
    anObj = HealOp.DivideEdge(theObject, theEdgeIndex, theValue, isByParameter)
    RaiseIfFailed("DivideEdge", HealOp)
    return anObj

## Change orientation of the given object.
#  @param theObject Shape to be processed.
#  @update given shape
def ChangeOrientationShell(theObject):
    theObject = HealOp.ChangeOrientation(theObject)
    RaiseIfFailed("ChangeOrientation", HealOp)

## Change orientation of the given object.
#  @param theObject Shape to be processed.
#  @return New GEOM_Object, containing processed shape.
def ChangeOrientationShellCopy(theObject):
    anObj = HealOp.ChangeOrientationCopy(theObject)
    RaiseIfFailed("ChangeOrientationCopy", HealOp)
    return anObj

## Get a list of wires (wrapped in GEOM_Object-s),
#  that constitute a free boundary of the given shape.
#  @param theObject Shape to get free boundary of.
#  @return [status, theClosedWires, theOpenWires]
#  status: FALSE, if an error(s) occured during the method execution.
#  theClosedWires: Closed wires on the free boundary of the given shape.
#  theOpenWires: Open wires on the free boundary of the given shape.
#
#  Example: see GEOM_TestHealing.py
def GetFreeBoundary(theObject):
    anObj = HealOp.GetFreeBoundary(theObject)
    RaiseIfFailed("GetFreeBoundary", HealOp)
    return anObj

# -----------------------------------------------------------------------------
# Create advanced objects
# -----------------------------------------------------------------------------

## Create a copy of the given object
#
#  Example: see GEOM_TestAll.py
def MakeCopy(theOriginal):
    anObj = InsertOp.MakeCopy(theOriginal)
    RaiseIfFailed("MakeCopy", InsertOp)
    return anObj

## Create a filling from the given compound of contours.
#  @param theShape the compound of contours
#  @param theMinDeg a minimal degree
#  @param theMaxDeg a maximal degree
#  @param theTol2D a 2d tolerance
#  @param theTol3D a 3d tolerance
#  @param theNbIter a number of iteration
#  @return New GEOM_Object, containing the created filling surface.
#
#  Example: see GEOM_TestAll.py
def MakeFilling(theShape, theMinDeg, theMaxDeg, theTol2D, theTol3D, theNbIter):
    anObj = PrimOp.MakeFilling(theShape, theMinDeg, theMaxDeg, theTol2D, theTol3D, theNbIter)
    RaiseIfFailed("MakeFilling", PrimOp)
    return anObj

## Replace coincident faces in theShape by one face.
#  @param theShape Initial shape.
#  @param theTolerance Maximum distance between faces, which can be considered as coincident.
#  @param doKeepNonSolids If FALSE, only solids will present in the result, otherwise all initial shapes.
#  @return New GEOM_Object, containing a copy of theShape without coincident faces.
#
#  Example: see GEOM_Spanner.py
def MakeGlueFaces(theShape, theTolerance, doKeepNonSolids=True):
    anObj = ShapesOp.MakeGlueFaces(theShape, theTolerance, doKeepNonSolids)
    if anObj is None:
      raise RuntimeError, "MakeGlueFaces : " + ShapesOp.GetErrorCode()
    return anObj


## Find coincident faces in theShape for possible gluing.
#  @param theShape Initial shape.
#  @param theTolerance Maximum distance between faces,
#                      which can be considered as coincident.
#  @return ListOfGO.
#
#  Example: see GEOM_Spanner.py
def GetGlueFaces(theShape, theTolerance):
    anObj = ShapesOp.GetGlueFaces(theShape, theTolerance)
    RaiseIfFailed("GetGlueFaces", ShapesOp)
    return anObj


## Replace coincident faces in theShape by one face
#  in compliance with given list of faces
#  @param theShape Initial shape.
#  @param theTolerance Maximum distance between faces,
#                      which can be considered as coincident.
#  @param theFaces List of faces for gluing.
#  @param doKeepNonSolids If FALSE, only solids will present in the result, otherwise all initial shapes.
#  @return New GEOM_Object, containing a copy of theShape
#          without some faces.
#
#  Example: see GEOM_Spanner.py
def MakeGlueFacesByList(theShape, theTolerance, theFaces, doKeepNonSolids=True):
    anObj = ShapesOp.MakeGlueFacesByList(theShape, theTolerance, theFaces, doKeepNonSolids)
    if anObj is None:
      raise RuntimeError, "MakeGlueFacesByList : " + ShapesOp.GetErrorCode()
    return anObj


# -----------------------------------------------------------------------------
# Boolean (Common, Cut, Fuse, Section)
# -----------------------------------------------------------------------------

## Perform one of boolean operations on two given shapes.
#  @param theShape1 First argument for boolean operation.
#  @param theShape2 Second argument for boolean operation.
#  @param theOperation Indicates the operation to be done:
#                      1 - Common, 2 - Cut, 3 - Fuse, 4 - Section.
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestAll.py
def MakeBoolean(theShape1, theShape2, theOperation):
    anObj = BoolOp.MakeBoolean(theShape1, theShape2, theOperation)
    RaiseIfFailed("MakeBoolean", BoolOp)
    return anObj

## Shortcut to MakeBoolean(s1, s2, 1)
#
#  Example: see GEOM_TestOthers.py
def MakeCommon(s1, s2):
    return MakeBoolean(s1, s2, 1)

## Shortcut to MakeBoolean(s1, s2, 2)
#
#  Example: see GEOM_TestOthers.py
def MakeCut(s1, s2):
    return MakeBoolean(s1, s2, 2)

## Shortcut to MakeBoolean(s1, s2, 3)
#
#  Example: see GEOM_TestOthers.py
def MakeFuse(s1, s2):
    return MakeBoolean(s1, s2, 3)

## Shortcut to MakeBoolean(s1, s2, 4)
#
#  Example: see GEOM_TestOthers.py
def MakeSection(s1, s2):
    return MakeBoolean(s1, s2, 4)

## Perform partition operation.
#  @param ListShapes Shapes to be intersected.
#  @param ListTools Shapes to intersect theShapes.
#  !!!NOTE: Each compound from ListShapes and ListTools will be exploded
#           in order to avoid possible intersection between shapes from
#           this compound.
#  @param Limit Type of resulting shapes (corresponding to TopAbs_ShapeEnum).
#  @param KeepNonlimitShapes: if this parameter == 0 - only shapes with
#                             type <= Limit are kept in the result,
#                             else - shapes with type > Limit are kept
#                             also (if they exist)
#
#  After implementation new version of PartitionAlgo (October 2006)
#  other parameters are ignored by current functionality. They are kept
#  in this function only for support old versions.
#  Ignored parameters:
#      @param ListKeepInside Shapes, outside which the results will be deleted.
#         Each shape from theKeepInside must belong to theShapes also.
#      @param ListRemoveInside Shapes, inside which the results will be deleted.
#         Each shape from theRemoveInside must belong to theShapes also.
#      @param RemoveWebs If TRUE, perform Glue 3D algorithm.
#      @param ListMaterials Material indices for each shape. Make sence,
#         only if theRemoveWebs is TRUE.
#
#  @return New GEOM_Object, containing the result shapes.
#
#  Example: see GEOM_TestAll.py
def MakePartition(ListShapes, ListTools=[], ListKeepInside=[], ListRemoveInside=[],
                  Limit=ShapeType["SHAPE"], RemoveWebs=0, ListMaterials=[],
                  KeepNonlimitShapes=0):
    anObj = BoolOp.MakePartition(ListShapes, ListTools,
                                 ListKeepInside, ListRemoveInside,
                                 Limit, RemoveWebs, ListMaterials,
                                 KeepNonlimitShapes);
    RaiseIfFailed("MakePartition", BoolOp)
    return anObj

## Perform partition operation.
#  This method may be useful if it is needed to make a partition for
#  compound contains nonintersected shapes. Performance will be better
#  since intersection between shapes from compound is not performed.
#
#  Description of all parameters as in previous method MakePartition()
#
#  !!!NOTE: Passed compounds (via ListShapes or via ListTools)
#           have to consist of nonintersecting shapes.
#
#  @return New GEOM_Object, containing the result shapes.
#
def MakePartitionNonSelfIntersectedShape(ListShapes, ListTools=[],
                                         ListKeepInside=[], ListRemoveInside=[],
                                         Limit=ShapeType["SHAPE"], RemoveWebs=0,
                                         ListMaterials=[], KeepNonlimitShapes=0):
    anObj = BoolOp.MakePartitionNonSelfIntersectedShape(ListShapes, ListTools,
                                                        ListKeepInside, ListRemoveInside,
                                                        Limit, RemoveWebs, ListMaterials,
                                                        KeepNonlimitShapes);
    RaiseIfFailed("MakePartitionNonSelfIntersectedShape", BoolOp)
    return anObj

## Shortcut to MakePartition()
#
#  Example: see GEOM_TestOthers.py
def Partition(ListShapes, ListTools=[], ListKeepInside=[], ListRemoveInside=[],
              Limit=ShapeType["SHAPE"], RemoveWebs=0, ListMaterials=[],
              KeepNonlimitShapes=0):
    anObj = MakePartition(ListShapes, ListTools,
                          ListKeepInside, ListRemoveInside,
                          Limit, RemoveWebs, ListMaterials,
                          KeepNonlimitShapes);
    return anObj

## Perform partition of the Shape with the Plane
#  @param theShape Shape to be intersected.
#  @param thePlane Tool shape, to intersect theShape.
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestAll.py
def MakeHalfPartition(theShape, thePlane):
    anObj = BoolOp.MakeHalfPartition(theShape, thePlane)
    RaiseIfFailed("MakeHalfPartition", BoolOp)
    return anObj

# -----------------------------------------------------------------------------
# Transform objects
# -----------------------------------------------------------------------------

## Translate the given object along the vector, specified
#  by its end points, creating its copy before the translation.
#  @param theObject The object to be translated.
#  @param thePoint1 Start point of translation vector.
#  @param thePoint2 End point of translation vector.
#  @return New GEOM_Object, containing the translated object.
#
#  Example: see GEOM_TestAll.py
def MakeTranslationTwoPoints(theObject, thePoint1, thePoint2):
    anObj = TrsfOp.TranslateTwoPointsCopy(theObject, thePoint1, thePoint2)
    RaiseIfFailed("TranslateTwoPointsCopy", TrsfOp)
    return anObj

## Translate the given object along the vector, specified
#  by its components, creating its copy before the translation.
#  @param theObject The object to be translated.
#  @param theDX,theDY,theDZ Components of translation vector.
#  @return New GEOM_Object, containing the translated object.
#
#  Example: see GEOM_TestAll.py
def MakeTranslation(theObject, theDX, theDY, theDZ):
    anObj = TrsfOp.TranslateDXDYDZCopy(theObject, theDX, theDY, theDZ)
    RaiseIfFailed("TranslateDXDYDZCopy", TrsfOp)
    return anObj

## Translate the given object along the given vector,
#  creating its copy before the translation.
#  @param theObject The object to be translated.
#  @param theVector The translation vector.
#  @return New GEOM_Object, containing the translated object.
#
#  Example: see GEOM_TestAll.py
def MakeTranslationVector(theObject, theVector):
    anObj = TrsfOp.TranslateVectorCopy(theObject, theVector)
    RaiseIfFailed("TranslateVectorCopy", TrsfOp)
    return anObj

## Rotate the given object around the given axis
#  on the given angle, creating its copy before the rotatation.
#  @param theObject The object to be rotated.
#  @param theAxis Rotation axis.
#  @param theAngle Rotation angle in radians.
#  @return New GEOM_Object, containing the rotated object.
#
#  Example: see GEOM_TestAll.py
def MakeRotation(theObject, theAxis, theAngle):
    anObj = TrsfOp.RotateCopy(theObject, theAxis, theAngle)
    RaiseIfFailed("RotateCopy", TrsfOp)
    return anObj

## Rotate given object around vector perpendicular to plane
#  containing three points, creating its copy before the rotatation.
#  @param theObject The object to be rotated.
#  @param theCentPoint central point - the axis is the vector perpendicular to the plane
#  containing the three points.
#  @param thePoint1 and thePoint2 - in a perpendicular plan of the axis.
#  @return New GEOM_Object, containing the rotated object.
#
#  Example: see GEOM_TestAll.py
def MakeRotationThreePoints(theObject, theCentPoint, thePoint1, thePoint2):
    anObj = TrsfOp.RotateThreePointsCopy(theObject, theCentPoint, thePoint1, thePoint2)
    RaiseIfFailed("RotateThreePointsCopy", TrsfOp)
    return anObj

## Scale the given object by the factor, creating its copy before the scaling.
#  @param theObject The object to be scaled.
#  @param thePoint Center point for scaling.
#  @param theFactor Scaling factor value.
#  @return New GEOM_Object, containing the scaled shape.
#
#  Example: see GEOM_TestAll.py
def MakeScaleTransform(theObject, thePoint, theFactor):
    anObj = TrsfOp.ScaleShapeCopy(theObject, thePoint, theFactor)
    RaiseIfFailed("ScaleShapeCopy", TrsfOp)
    return anObj

## Create an object, symmetrical
#  to the given one relatively the given plane.
#  @param theObject The object to be mirrored.
#  @param thePlane Plane of symmetry.
#  @return New GEOM_Object, containing the mirrored shape.
#
#  Example: see GEOM_TestAll.py
def MakeMirrorByPlane(theObject, thePlane):
    anObj = TrsfOp.MirrorPlaneCopy(theObject, thePlane)
    RaiseIfFailed("MirrorPlaneCopy", TrsfOp)
    return anObj

## Create an object, symmetrical
#  to the given one relatively the given axis.
#  @param theObject The object to be mirrored.
#  @param theAxis Axis of symmetry.
#  @return New GEOM_Object, containing the mirrored shape.
#
#  Example: see GEOM_TestAll.py
def MakeMirrorByAxis(theObject, theAxis):
    anObj = TrsfOp.MirrorAxisCopy(theObject, theAxis)
    RaiseIfFailed("MirrorAxisCopy", TrsfOp)
    return anObj

## Create an object, symmetrical
#  to the given one relatively the given point.
#  @param theObject The object to be mirrored.
#  @param thePoint Point of symmetry.
#  @return New GEOM_Object, containing the mirrored shape.
#
#  Example: see GEOM_TestAll.py
def MakeMirrorByPoint(theObject, thePoint):
    anObj = TrsfOp.MirrorPointCopy(theObject, thePoint)
    RaiseIfFailed("MirrorPointCopy", TrsfOp)
    return anObj

## Modify the Location of the given object by LCS,
#  creating its copy before the setting.
#  @param theObject The object to be displaced.
#  @param theStartLCS Coordinate system to perform displacement from it.
#                     If \a theStartLCS is NULL, displacement
#                     will be performed from global CS.
#                     If \a theObject itself is used as \a theStartLCS,
#                     its location will be changed to \a theEndLCS.
#  @param theEndLCS Coordinate system to perform displacement to it.
#  @return New GEOM_Object, containing the displaced shape.
#
#  Example: see GEOM_TestAll.py
def MakePosition(theObject, theStartLCS, theEndLCS):
    anObj = TrsfOp.PositionShapeCopy(theObject, theStartLCS, theEndLCS)
    RaiseIfFailed("PositionShapeCopy", TrsfOp)
    return anObj

## Create new object as offset of the given one.
#  @param theObject The base object for the offset.
#  @param theOffset Offset value.
#  @return New GEOM_Object, containing the offset object.
#
#  Example: see GEOM_TestAll.py
def MakeOffset(theObject, theOffset):
    anObj = TrsfOp.OffsetShapeCopy(theObject, theOffset)
    RaiseIfFailed("OffsetShapeCopy", TrsfOp)
    return anObj

# -----------------------------------------------------------------------------
# Patterns
# -----------------------------------------------------------------------------

## Translate the given object along the given vector a given number times
#  @param theObject The object to be translated.
#  @param theVector Direction of the translation.
#  @param theStep Distance to translate on.
#  @param theNbTimes Quantity of translations to be done.
#  @return New GEOM_Object, containing compound of all
#          the shapes, obtained after each translation.
#
#  Example: see GEOM_TestAll.py
def MakeMultiTranslation1D(theObject, theVector, theStep, theNbTimes):
    anObj = TrsfOp.MultiTranslate1D(theObject, theVector, theStep, theNbTimes)
    RaiseIfFailed("MultiTranslate1D", TrsfOp)
    return anObj

## Conseqently apply two specified translations to theObject specified number of times.
#  @param theObject The object to be translated.
#  @param theVector1 Direction of the first translation.
#  @param theStep1 Step of the first translation.
#  @param theNbTimes1 Quantity of translations to be done along theVector1.
#  @param theVector2 Direction of the second translation.
#  @param theStep2 Step of the second translation.
#  @param theNbTimes2 Quantity of translations to be done along theVector2.
#  @return New GEOM_Object, containing compound of all
#          the shapes, obtained after each translation.
#
#  Example: see GEOM_TestAll.py
def MakeMultiTranslation2D(theObject, theVector1, theStep1, theNbTimes1,
			              theVector2, theStep2, theNbTimes2):
    anObj = TrsfOp.MultiTranslate2D(theObject, theVector1, theStep1, theNbTimes1,
			                       theVector2, theStep2, theNbTimes2)
    RaiseIfFailed("MultiTranslate2D", TrsfOp)
    return anObj

## Rotate the given object around the given axis a given number times.
#  Rotation angle will be 2*PI/theNbTimes.
#  @param theObject The object to be rotated.
#  @param theAxis The rotation axis.
#  @param theNbTimes Quantity of rotations to be done.
#  @return New GEOM_Object, containing compound of all the
#          shapes, obtained after each rotation.
#
#  Example: see GEOM_TestAll.py
def MultiRotate1D(theObject, theAxis, theNbTimes):
    anObj = TrsfOp.MultiRotate1D(theObject, theAxis, theNbTimes)
    RaiseIfFailed("MultiRotate1D", TrsfOp)
    return anObj

## Rotate the given object around the
#  given axis on the given angle a given number
#  times and multi-translate each rotation result.
#  Translation direction passes through center of gravity
#  of rotated shape and its projection on the rotation axis.
#  @param theObject The object to be rotated.
#  @param theAxis Rotation axis.
#  @param theAngle Rotation angle in graduces.
#  @param theNbTimes1 Quantity of rotations to be done.
#  @param theStep Translation distance.
#  @param theNbTimes2 Quantity of translations to be done.
#  @return New GEOM_Object, containing compound of all the
#          shapes, obtained after each transformation.
#
#  Example: see GEOM_TestAll.py
def MultiRotate2D(theObject, theAxis, theAngle, theNbTimes1, theStep, theNbTimes2):
    anObj = TrsfOp.MultiRotate2D(theObject, theAxis, theAngle, theNbTimes1, theStep, theNbTimes2)
    RaiseIfFailed("MultiRotate2D", TrsfOp)
    return anObj

## The same, as MultiRotate1D(), but axis is given by direction and point
#
#  Example: see GEOM_TestOthers.py
def MakeMultiRotation1D(aShape,aDir,aPoint,aNbTimes):
    aVec = MakeLine(aPoint,aDir)
    anObj = MultiRotate1D(aShape,aVec,aNbTimes)
    return anObj

## The same, as MultiRotate2D(), but axis is given by direction and point
#
#  Example: see GEOM_TestOthers.py
def MakeMultiRotation2D(aShape,aDir,aPoint,anAngle,nbtimes1,aStep,nbtimes2):
    aVec = MakeLine(aPoint,aDir)
    anObj = MultiRotate2D(aShape,aVec,anAngle,nbtimes1,aStep,nbtimes2)
    return anObj

# -----------------------------------------------------------------------------
# Local operations
# -----------------------------------------------------------------------------

## Perform a fillet on all edges of the given shape.
#  @param theShape Shape, to perform fillet on.
#  @param theR Fillet radius.
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestOthers.py
def MakeFilletAll(theShape, theR):
    anObj = LocalOp.MakeFilletAll(theShape, theR)
    RaiseIfFailed("MakeFilletAll", LocalOp)
    return anObj

## Perform a fillet on the specified edges/faces of the given shape
#  @param theShape Shape, to perform fillet on.
#  @param theR Fillet radius.
#  @param theShapeType Type of shapes in <theListShapes>.
#  @param theListShapes Global indices of edges/faces to perform fillet on.
#    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestAll.py
def MakeFillet(theShape, theR, theShapeType, theListShapes):
    anObj = None
    if theShapeType == ShapeType["EDGE"]:
        anObj = LocalOp.MakeFilletEdges(theShape, theR, theListShapes)
        RaiseIfFailed("MakeFilletEdges", LocalOp)
    else:
        anObj = LocalOp.MakeFilletFaces(theShape, theR, theListShapes)
        RaiseIfFailed("MakeFilletFaces", LocalOp)
    return anObj

## The same but with two Fillet Radius R1 and R2
def MakeFilletR1R2(theShape, theR1, theR2, theShapeType, theListShapes):
    anObj = None
    if theShapeType == ShapeType["EDGE"]:
        anObj = LocalOp.MakeFilletEdgesR1R2(theShape, theR1, theR2, theListShapes)
        RaiseIfFailed("MakeFilletEdgesR1R2", LocalOp)
    else:
        anObj = LocalOp.MakeFilletFacesR1R2(theShape, theR1, theR2, theListShapes)
        RaiseIfFailed("MakeFilletFacesR1R2", LocalOp)
    return anObj

## Perform a symmetric chamfer on all edges of the given shape.
#  @param theShape Shape, to perform chamfer on.
#  @param theD Chamfer size along each face.
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestOthers.py
def MakeChamferAll(theShape, theD):
    anObj = LocalOp.MakeChamferAll(theShape, theD)
    RaiseIfFailed("MakeChamferAll", LocalOp)
    return anObj

## Perform a chamfer on edges, common to the specified faces,
#  with distance D1 on the Face1
#  @param theShape Shape, to perform chamfer on.
#  @param theD1 Chamfer size along \a theFace1.
#  @param theD2 Chamfer size along \a theFace2.
#  @param theFace1,theFace2 Global indices of two faces of \a theShape.
#    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestAll.py
def MakeChamferEdge(theShape, theD1, theD2, theFace1, theFace2):
    anObj = LocalOp.MakeChamferEdge(theShape, theD1, theD2, theFace1, theFace2)
    RaiseIfFailed("MakeChamferEdge", LocalOp)
    return anObj

## The Same chamfer but with params theD is chamfer lenght and
#  theAngle is Angle of chamfer (angle in radians)
def MakeChamferEdgeAD(theShape, theD, theAngle, theFace1, theFace2):
    anObj = LocalOp.MakeChamferEdgeAD(theShape, theD, theAngle, theFace1, theFace2)
    RaiseIfFailed("MakeChamferEdgeAD", LocalOp)
    return anObj

## Perform a chamfer on all edges of the specified faces,
#  with distance D1 on the first specified face (if several for one edge)
#  @param theShape Shape, to perform chamfer on.
#  @param theD1 Chamfer size along face from \a theFaces. If both faces,
#               connected to the edge, are in \a theFaces, \a theD1
#               will be get along face, which is nearer to \a theFaces beginning.
#  @param theD2 Chamfer size along another of two faces, connected to the edge.
#  @param theFaces Sequence of global indices of faces of \a theShape.
#    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_TestAll.py
def MakeChamferFaces(theShape, theD1, theD2, theFaces):
    anObj = LocalOp.MakeChamferFaces(theShape, theD1, theD2, theFaces)
    RaiseIfFailed("MakeChamferFaces", LocalOp)
    return anObj

## The Same chamfer but with params theD is chamfer lenght and
#  theAngle is Angle of chamfer (angle in radians)
def MakeChamferFacesAD(theShape, theD, theAngle, theFaces):
    anObj = LocalOp.MakeChamferFacesAD(theShape, theD, theAngle, theFaces)
    RaiseIfFailed("MakeChamferFacesAD", LocalOp)
    return anObj

## Perform a chamfer on edges,
#  with distance D1 on the first specified face (if several for one edge)
#  @param theShape Shape, to perform chamfer on.
#  @param theD1 and theD2 Chamfer size 
#  @param theEdges Sequence of edges of \a theShape.
#  @return New GEOM_Object, containing the result shape.
#
#  Example:
def MakeChamferEdges(theShape, theD1, theD2, theEdges):
    anObj = LocalOp.MakeChamferEdges(theShape, theD1, theD2, theEdges)
    RaiseIfFailed("MakeChamferEdges", LocalOp)
    return anObj

## The Same chamfer but with params theD is chamfer lenght and
#  theAngle is Angle of chamfer (angle in radians)
def MakeChamferEdgesAD(theShape, theD, theAngle, theEdges):
    anObj = LocalOp.MakeChamferEdgesAD(theShape, theD, theAngle, theEdges)
    RaiseIfFailed("MakeChamferEdgesAD", LocalOp)
    return anObj

## Shortcut to MakeChamferEdge() and MakeChamferFaces()
#
#  Example: see GEOM_TestOthers.py
def MakeChamfer(aShape,d1,d2,aShapeType,ListShape):
    anObj = None
    if aShapeType == ShapeType["EDGE"]:
        anObj = MakeChamferEdge(aShape,d1,d2,ListShape[0],ListShape[1])
    else:
        anObj = MakeChamferFaces(aShape,d1,d2,ListShape)
    return anObj

## Perform an Archimde operation on the given shape with given parameters.
#  The object presenting the resulting face is returned.
#  @param theShape Shape to be put in water.
#  @param theWeight Weight og the shape.
#  @param theWaterDensity Density of the water.
#  @param theMeshDeflection Deflection of the mesh, using to compute the section.
#  @return New GEOM_Object, containing a section of \a theShape
#          by a plane, corresponding to water level.
#
#  Example: see GEOM_TestAll.py
def Archimede(theShape, theWeight, theWaterDensity, theMeshDeflection):
    anObj = LocalOp.MakeArchimede(theShape, theWeight, theWaterDensity, theMeshDeflection)
    RaiseIfFailed("MakeArchimede", LocalOp)
    return anObj

# -----------------------------------------------------------------------------
# Information objects
# -----------------------------------------------------------------------------

## Get point coordinates
#  @return [x, y, z]
#
#  Example: see GEOM_TestMeasures.py
def PointCoordinates(Point):
    aTuple = MeasuOp.PointCoordinates(Point)
    RaiseIfFailed("PointCoordinates", MeasuOp)
    return aTuple

## Get summarized length of all wires,
#  area of surface and volume of the given shape.
#  @param theShape Shape to define properties of.
#  @return [theLength, theSurfArea, theVolume]
#  theLength:   Summarized length of all wires of the given shape.
#  theSurfArea: Area of surface of the given shape.
#  theVolume:   Volume of the given shape.
#
#  Example: see GEOM_TestMeasures.py
def BasicProperties(theShape):
    aTuple = MeasuOp.GetBasicProperties(theShape)
    RaiseIfFailed("GetBasicProperties", MeasuOp)
    return aTuple

## Get parameters of bounding box of the given shape
#  @param theShape Shape to obtain bounding box of.
#  @return [Xmin,Xmax, Ymin,Ymax, Zmin,Zmax]
#  Xmin,Xmax: Limits of shape along OX axis.
#  Ymin,Ymax: Limits of shape along OY axis.
#  Zmin,Zmax: Limits of shape along OZ axis.
#
#  Example: see GEOM_TestMeasures.py
def BoundingBox(theShape):
    aTuple = MeasuOp.GetBoundingBox(theShape)
    RaiseIfFailed("GetBoundingBox", MeasuOp)
    return aTuple

## Get inertia matrix and moments of inertia of theShape.
#  @param theShape Shape to calculate inertia of.
#  @return [I11,I12,I13, I21,I22,I23, I31,I32,I33, Ix,Iy,Iz]
#  I(1-3)(1-3): Components of the inertia matrix of the given shape.
#  Ix,Iy,Iz:    Moments of inertia of the given shape.
#
#  Example: see GEOM_TestMeasures.py
def Inertia(theShape):
    aTuple = MeasuOp.GetInertia(theShape)
    RaiseIfFailed("GetInertia", MeasuOp)
    return aTuple

## Get minimal distance between the given shapes.
#  @param theShape1,theShape2 Shapes to find minimal distance between.
#  @return Value of the minimal distance between the given shapes.
#
#  Example: see GEOM_TestMeasures.py
def MinDistance(theShape1, theShape2):
    aTuple = MeasuOp.GetMinDistance(theShape1, theShape2)
    RaiseIfFailed("GetMinDistance", MeasuOp)
    return aTuple[0]

## Get minimal distance between the given shapes.
#  @param theShape1,theShape2 Shapes to find minimal distance between.
#  @return Value of the minimal distance between the given shapes.
#
#  Example: see GEOM_TestMeasures.py
def MinDistanceComponents(theShape1, theShape2):
    aTuple = MeasuOp.GetMinDistance(theShape1, theShape2)
    RaiseIfFailed("GetMinDistance", MeasuOp)
    aRes = [aTuple[0], aTuple[4] - aTuple[1], aTuple[5] - aTuple[2], aTuple[6] - aTuple[3]]
    return aRes

## Get angle between the given shapes.
#  @param theShape1,theShape2 Lines or linear edges to find angle between.
#  @return Value of the angle between the given shapes.
#
#  Example: see GEOM_TestMeasures.py
def GetAngle(theShape1, theShape2):
    anAngle = MeasuOp.GetAngle(theShape1, theShape2)
    RaiseIfFailed("GetAngle", MeasuOp)
    return anAngle

## Get min and max tolerances of sub-shapes of theShape
#  @param theShape Shape, to get tolerances of.
#  @return [FaceMin,FaceMax, EdgeMin,EdgeMax, VertMin,VertMax]
#  FaceMin,FaceMax: Min and max tolerances of the faces.
#  EdgeMin,EdgeMax: Min and max tolerances of the edges.
#  VertMin,VertMax: Min and max tolerances of the vertices.
#
#  Example: see GEOM_TestMeasures.py
def Tolerance(theShape):
    aTuple = MeasuOp.GetTolerance(theShape)
    RaiseIfFailed("GetTolerance", MeasuOp)
    return aTuple

## Obtain description of the given shape (number of sub-shapes of each type)
#  @param theShape Shape to be described.
#  @return Description of the given shape.
#
#  Example: see GEOM_TestMeasures.py
def WhatIs(theShape):
    aDescr = MeasuOp.WhatIs(theShape)
    RaiseIfFailed("WhatIs", MeasuOp)
    return aDescr

## Get a point, situated at the centre of mass of theShape.
#  @param theShape Shape to define centre of mass of.
#  @return New GEOM_Object, containing the created point.
#
#  Example: see GEOM_TestMeasures.py
def MakeCDG(theShape):
    anObj = MeasuOp.GetCentreOfMass(theShape)
    RaiseIfFailed("GetCentreOfMass", MeasuOp)
    return anObj

## Get a normale to the given face. If the point is not given,
#  the normale is calculated at the center of mass.
#  @param theFace Face to define normale of.
#  @param theOptionalPoint Point to compute the normale at.
#  @return New GEOM_Object, containing the created vector.
#
#  Example: see GEOM_TestMeasures.py
def GetNormal(theFace, theOptionalPoint = None):
    anObj = MeasuOp.GetNormal(theFace, theOptionalPoint)
    RaiseIfFailed("GetNormal", MeasuOp)
    return anObj

## Check a topology of the given shape.
#  @param theShape Shape to check validity of.
#  @param theIsCheckGeom If FALSE, only the shape's topology will be checked,
#                        if TRUE, the shape's geometry will be checked also.
#  @return TRUE, if the shape "seems to be valid".
#  If theShape is invalid, prints a description of problem.
#
#  Example: see GEOM_TestMeasures.py
def CheckShape(theShape, theIsCheckGeom = 0):
    if theIsCheckGeom:
        (IsValid, Status) = MeasuOp.CheckShapeWithGeometry(theShape)
        RaiseIfFailed("CheckShapeWithGeometry", MeasuOp)
    else:
        (IsValid, Status) = MeasuOp.CheckShape(theShape)
        RaiseIfFailed("CheckShape", MeasuOp)
    if IsValid == 0:
        print Status
    return IsValid

## Get position (LCS) of theShape.
#
#  Origin of the LCS is situated at the shape's center of mass.
#  Axes of the LCS are obtained from shape's location or,
#  if the shape is a planar face, from position of its plane.
#
#  @param theShape Shape to calculate position of.
#  @return [Ox,Oy,Oz, Zx,Zy,Zz, Xx,Xy,Xz].
#          Ox,Oy,Oz: Coordinates of shape's LCS origin.
#          Zx,Zy,Zz: Coordinates of shape's LCS normal(main) direction.
#          Xx,Xy,Xz: Coordinates of shape's LCS X direction.
#
#  Example: see GEOM_TestMeasures.py
def GetPosition(theShape):
    aTuple = MeasuOp.GetPosition(theShape)
    RaiseIfFailed("GetPosition", MeasuOp)
    return aTuple

## Get kind of theShape.
#
#  @param theShape Shape to get a kind of.
#  @return Returns a kind of shape in terms of <VAR>GEOM_IKindOfShape.shape_kind</VAR> enumeration
#          and a list of parameters, describing the shape.
#  @note  Concrete meaning of each value, returned via \a theIntegers
#         or \a theDoubles list depends on the kind of the shape.
#         The full list of possible outputs is:
#
#  geompy.kind.COMPOUND              nb_solids  nb_faces  nb_edges  nb_vertices
#  geompy.kind.COMPSOLID             nb_solids  nb_faces  nb_edges  nb_vertices
#
#  geompy.kind.SHELL       geompy.info.CLOSED   nb_faces  nb_edges  nb_vertices
#  geompy.kind.SHELL       geompy.info.UNCLOSED nb_faces  nb_edges  nb_vertices
#
#  geompy.kind.WIRE        geompy.info.CLOSED             nb_edges  nb_vertices
#  geompy.kind.WIRE        geompy.info.UNCLOSED           nb_edges  nb_vertices
#
#  geompy.kind.SPHERE       xc yc zc            R
#  geompy.kind.CYLINDER     xb yb zb  dx dy dz  R         H
#  geompy.kind.BOX          xc yc zc                      ax ay az
#  geompy.kind.ROTATED_BOX  xc yc zc  zx zy zz  xx xy xz  ax ay az
#  geompy.kind.TORUS        xc yc zc  dx dy dz  R_1  R_2
#  geompy.kind.CONE         xb yb zb  dx dy dz  R_1  R_2  H
#  geompy.kind.POLYHEDRON                       nb_faces  nb_edges  nb_vertices
#  geompy.kind.SOLID                            nb_faces  nb_edges  nb_vertices
#
#  geompy.kind.SPHERE2D     xc yc zc            R
#  geompy.kind.CYLINDER2D   xb yb zb  dx dy dz  R         H
#  geompy.kind.TORUS2D      xc yc zc  dx dy dz  R_1  R_2
#  geompy.kind.CONE2D       xc yc zc  dx dy dz  R_1  R_2  H
#  geompy.kind.DISK_CIRCLE  xc yc zc  dx dy dz  R
#  geompy.kind.DISK_ELLIPSE xc yc zc  dx dy dz  R_1  R_2
#  geompy.kind.POLYGON      xo yo zo  dx dy dz            nb_edges  nb_vertices
#  geompy.kind.PLANE        xo yo zo  dx dy dz
#  geompy.kind.PLANAR       xo yo zo  dx dy dz            nb_edges  nb_vertices
#  geompy.kind.FACE                                       nb_edges  nb_vertices
#
#  geompy.kind.CIRCLE       xc yc zc  dx dy dz  R
#  geompy.kind.ARC_CIRCLE   xc yc zc  dx dy dz  R         x1 y1 z1  x2 y2 z2
#  geompy.kind.ELLIPSE      xc yc zc  dx dy dz  R_1  R_2
#  geompy.kind.ARC_ELLIPSE  xc yc zc  dx dy dz  R_1  R_2  x1 y1 z1  x2 y2 z2
#  geompy.kind.LINE         xo yo zo  dx dy dz
#  geompy.kind.SEGMENT      x1 y1 z1  x2 y2 z2
#  geompy.kind.EDGE                                                 nb_vertices
#
#  geompy.kind.VERTEX       x  y  z
#
#  Example: see GEOM_TestMeasures.py
def KindOfShape(theShape):
    aRoughTuple = MeasuOp.KindOfShape(theShape)
    RaiseIfFailed("KindOfShape", MeasuOp)

    aKind  = aRoughTuple[0]
    anInts = aRoughTuple[1]
    aDbls  = aRoughTuple[2]

    # Now there is no exception from this rule:
    aKindTuple = [aKind] + aDbls + anInts

    # If they are we will regroup parameters for such kind of shape.
    # For example:
    #if aKind == kind.SOME_KIND:
    #    #  SOME_KIND     int int double int double double
    #    aKindTuple = [aKind, anInts[0], anInts[1], aDbls[0], anInts[2], aDbls[1], aDbls[2]]

    return aKindTuple

# -----------------------------------------------------------------------------
# Import/Export objects
# -----------------------------------------------------------------------------

## Import a shape from the BREP or IGES or STEP file
#  (depends on given format) with given name.
#  @param theFileName The file, containing the shape.
#  @param theFormatName Specify format for the file reading.
#         Available formats can be obtained with InsertOp.ImportTranslators() method.
#  @return New GEOM_Object, containing the imported shape.
#
#  Example: see GEOM_TestOthers.py
def Import(theFileName, theFormatName):
    anObj = InsertOp.Import(theFileName, theFormatName)
    RaiseIfFailed("Import", InsertOp)
    return anObj

## Shortcut to Import() for BREP format
#
#  Example: see GEOM_TestOthers.py
def ImportBREP(theFileName):
    return Import(theFileName, "BREP")

## Shortcut to Import() for IGES format
#
#  Example: see GEOM_TestOthers.py
def ImportIGES(theFileName):
    return Import(theFileName, "IGES")

## Shortcut to Import() for STEP format
#
#  Example: see GEOM_TestOthers.py
def ImportSTEP(theFileName):
    return Import(theFileName, "STEP")

## Export the given shape into a file with given name.
#  @param theObject Shape to be stored in the file.
#  @param theFileName Name of the file to store the given shape in.
#  @param theFormatName Specify format for the shape storage.
#         Available formats can be obtained with InsertOp.ImportTranslators() method.
#
#  Example: see GEOM_TestOthers.py
def Export(theObject, theFileName, theFormatName):
    InsertOp.Export(theObject, theFileName, theFormatName)
    RaiseIfFailed("Export", InsertOp)

## Shortcut to Export() for BREP format
#
#  Example: see GEOM_TestOthers.py
def ExportBREP(theObject, theFileName):
    return Export(theObject, theFileName, "BREP")

## Shortcut to Export() for IGES format
#
#  Example: see GEOM_TestOthers.py
def ExportIGES(theObject, theFileName):
    return Export(theObject, theFileName, "IGES")

## Shortcut to Export() for STEP format
#
#  Example: see GEOM_TestOthers.py
def ExportSTEP(theObject, theFileName):
    return Export(theObject, theFileName, "STEP")

# -----------------------------------------------------------------------------
# Block operations
# -----------------------------------------------------------------------------

## Create a quadrangle face from four edges. Order of Edges is not
#  important. It is  not necessary that edges share the same vertex.
#  @param E1,E2,E3,E4 Edges for the face bound.
#  @return New GEOM_Object, containing the created face.
#
#  Example: see GEOM_Spanner.py
def MakeQuad(E1, E2, E3, E4):
    anObj = BlocksOp.MakeQuad(E1, E2, E3, E4)
    RaiseIfFailed("MakeQuad", BlocksOp)
    return anObj

## Create a quadrangle face on two edges.
#  The missing edges will be built by creating the shortest ones.
#  @param E1,E2 Two opposite edges for the face.
#  @return New GEOM_Object, containing the created face.
#
#  Example: see GEOM_Spanner.py
def MakeQuad2Edges(E1, E2):
    anObj = BlocksOp.MakeQuad2Edges(E1, E2)
    RaiseIfFailed("MakeQuad2Edges", BlocksOp)
    return anObj

## Create a quadrangle face with specified corners.
#  The missing edges will be built by creating the shortest ones.
#  @param V1,V2,V3,V4 Corner vertices for the face.
#  @return New GEOM_Object, containing the created face.
#
#  Example: see GEOM_Spanner.py
def MakeQuad4Vertices(V1, V2, V3, V4):
    anObj = BlocksOp.MakeQuad4Vertices(V1, V2, V3, V4)
    RaiseIfFailed("MakeQuad4Vertices", BlocksOp)
    return anObj

## Create a hexahedral solid, bounded by the six given faces. Order of
#  faces is not important. It is  not necessary that Faces share the same edge.
#  @param F1,F2,F3,F4,F5,F6 Faces for the hexahedral solid.
#  @return New GEOM_Object, containing the created solid.
#
#  Example: see GEOM_Spanner.py
def MakeHexa(F1, F2, F3, F4, F5, F6):
    anObj = BlocksOp.MakeHexa(F1, F2, F3, F4, F5, F6)
    RaiseIfFailed("MakeHexa", BlocksOp)
    return anObj

## Create a hexahedral solid between two given faces.
#  The missing faces will be built by creating the smallest ones.
#  @param F1,F2 Two opposite faces for the hexahedral solid.
#  @return New GEOM_Object, containing the created solid.
#
#  Example: see GEOM_Spanner.py
def MakeHexa2Faces(F1, F2):
    anObj = BlocksOp.MakeHexa2Faces(F1, F2)
    RaiseIfFailed("MakeHexa2Faces", BlocksOp)
    return anObj

## Get a vertex, found in the given shape by its coordinates.
#  @param theShape Block or a compound of blocks.
#  @param theX,theY,theZ Coordinates of the sought vertex.
#  @param theEpsilon Maximum allowed distance between the resulting
#                    vertex and point with the given coordinates.
#  @return New GEOM_Object, containing the found vertex.
#
#  Example: see GEOM_TestOthers.py
def GetPoint(theShape, theX, theY, theZ, theEpsilon):
    anObj = BlocksOp.GetPoint(theShape, theX, theY, theZ, theEpsilon)
    RaiseIfFailed("GetPoint", BlocksOp)
    return anObj

## Get an edge, found in the given shape by two given vertices.
#  @param theShape Block or a compound of blocks.
#  @param thePoint1,thePoint2 Points, close to the ends of the desired edge.
#  @return New GEOM_Object, containing the found edge.
#
#  Example: see GEOM_Spanner.py
def GetEdge(theShape, thePoint1, thePoint2):
    anObj = BlocksOp.GetEdge(theShape, thePoint1, thePoint2)
    RaiseIfFailed("GetEdge", BlocksOp)
    return anObj

## Find an edge of the given shape, which has minimal distance to the given point.
#  @param theShape Block or a compound of blocks.
#  @param thePoint Point, close to the desired edge.
#  @return New GEOM_Object, containing the found edge.
#
#  Example: see GEOM_TestOthers.py
def GetEdgeNearPoint(theShape, thePoint):
    anObj = BlocksOp.GetEdgeNearPoint(theShape, thePoint)
    RaiseIfFailed("GetEdgeNearPoint", BlocksOp)
    return anObj

## Returns a face, found in the given shape by four given corner vertices.
#  @param theShape Block or a compound of blocks.
#  @param thePoint1-thePoint4 Points, close to the corners of the desired face.
#  @return New GEOM_Object, containing the found face.
#
#  Example: see GEOM_Spanner.py
def GetFaceByPoints(theShape, thePoint1, thePoint2, thePoint3, thePoint4):
    anObj = BlocksOp.GetFaceByPoints(theShape, thePoint1, thePoint2, thePoint3, thePoint4)
    RaiseIfFailed("GetFaceByPoints", BlocksOp)
    return anObj

## Get a face of block, found in the given shape by two given edges.
#  @param theShape Block or a compound of blocks.
#  @param theEdge1,theEdge2 Edges, close to the edges of the desired face.
#  @return New GEOM_Object, containing the found face.
#
#  Example: see GEOM_Spanner.py
def GetFaceByEdges(theShape, theEdge1, theEdge2):
    anObj = BlocksOp.GetFaceByEdges(theShape, theEdge1, theEdge2)
    RaiseIfFailed("GetFaceByEdges", BlocksOp)
    return anObj

## Find a face, opposite to the given one in the given block.
#  @param theBlock Must be a hexahedral solid.
#  @param theFace Face of \a theBlock, opposite to the desired face.
#  @return New GEOM_Object, containing the found face.
#
#  Example: see GEOM_Spanner.py
def GetOppositeFace(theBlock, theFace):
    anObj = BlocksOp.GetOppositeFace(theBlock, theFace)
    RaiseIfFailed("GetOppositeFace", BlocksOp)
    return anObj

## Find a face of the given shape, which has minimal distance to the given point.
#  @param theShape Block or a compound of blocks.
#  @param thePoint Point, close to the desired face.
#  @return New GEOM_Object, containing the found face.
#
#  Example: see GEOM_Spanner.py
def GetFaceNearPoint(theShape, thePoint):
    anObj = BlocksOp.GetFaceNearPoint(theShape, thePoint)
    RaiseIfFailed("GetFaceNearPoint", BlocksOp)
    return anObj

## Find a face of block, whose outside normale has minimal angle with the given vector.
#  @param theShape Block or a compound of blocks.
#  @param theVector Vector, close to the normale of the desired face.
#  @return New GEOM_Object, containing the found face.
#
#  Example: see GEOM_Spanner.py
def GetFaceByNormale(theBlock, theVector):
    anObj = BlocksOp.GetFaceByNormale(theBlock, theVector)
    RaiseIfFailed("GetFaceByNormale", BlocksOp)
    return anObj

## Check, if the compound of blocks is given.
#  To be considered as a compound of blocks, the
#  given shape must satisfy the following conditions:
#  - Each element of the compound should be a Block (6 faces and 12 edges).
#  - A connection between two Blocks should be an entire quadrangle face or an entire edge.
#  - The compound should be connexe.
#  - The glue between two quadrangle faces should be applied.
#  @param theCompound The compound to check.
#  @return TRUE, if the given shape is a compound of blocks.
#  If theCompound is not valid, prints all discovered errors.
#
#  Example: see GEOM_Spanner.py
def CheckCompoundOfBlocks(theCompound):
    (IsValid, BCErrors) = BlocksOp.CheckCompoundOfBlocks(theCompound)
    RaiseIfFailed("CheckCompoundOfBlocks", BlocksOp)
    if IsValid == 0:
        Descr = BlocksOp.PrintBCErrors(theCompound, BCErrors)
        print Descr
    return IsValid

## Remove all seam and degenerated edges from \a theShape.
#  Unite faces and edges, sharing one surface. It means that
#  this faces must have references to one C++ surface object (handle).
#  @param theShape The compound or single solid to remove irregular edges from.
#  @return Improved shape.
#
#  Example: see GEOM_TestOthers.py
def RemoveExtraEdges(theShape):
    anObj = BlocksOp.RemoveExtraEdges(theShape)
    RaiseIfFailed("RemoveExtraEdges", BlocksOp)
    return anObj

## Check, if the given shape is a blocks compound.
#  Fix all detected errors.
#    \note Single block can be also fixed by this method.
#  @param theCompound The compound to check and improve.
#  @return Improved compound.
#
#  Example: see GEOM_TestOthers.py
def CheckAndImprove(theShape):
    anObj = BlocksOp.CheckAndImprove(theShape)
    RaiseIfFailed("CheckAndImprove", BlocksOp)
    return anObj

## Get all the blocks, contained in the given compound.
#  @param theCompound The compound to explode.
#  @param theMinNbFaces If solid has lower number of faces, it is not a block.
#  @param theMaxNbFaces If solid has higher number of faces, it is not a block.
#    \note If theMaxNbFaces = 0, the maximum number of faces is not restricted.
#  @return List of GEOM_Objects, containing the retrieved blocks.
#
#  Example: see GEOM_TestOthers.py
def MakeBlockExplode(theCompound, theMinNbFaces, theMaxNbFaces):
    aList = BlocksOp.ExplodeCompoundOfBlocks(theCompound, theMinNbFaces, theMaxNbFaces)
    RaiseIfFailed("ExplodeCompoundOfBlocks", BlocksOp)
    return aList

## Find block, containing the given point inside its volume or on boundary.
#  @param theCompound Compound, to find block in.
#  @param thePoint Point, close to the desired block. If the point lays on
#         boundary between some blocks, we return block with nearest center.
#  @return New GEOM_Object, containing the found block.
#
#  Example: see GEOM_Spanner.py
def GetBlockNearPoint(theCompound, thePoint):
    anObj = BlocksOp.GetBlockNearPoint(theCompound, thePoint)
    RaiseIfFailed("GetBlockNearPoint", BlocksOp)
    return anObj

## Find block, containing all the elements, passed as the parts, or maximum quantity of them.
#  @param theCompound Compound, to find block in.
#  @param theParts List of faces and/or edges and/or vertices to be parts of the found block.
#  @return New GEOM_Object, containing the found block.
#
#  Example: see GEOM_TestOthers.py
def GetBlockByParts(theCompound, theParts):
    anObj = BlocksOp.GetBlockByParts(theCompound, theParts)
    RaiseIfFailed("GetBlockByParts", BlocksOp)
    return anObj

## Return all blocks, containing all the elements, passed as the parts.
#  @param theCompound Compound, to find blocks in.
#  @param theParts List of faces and/or edges and/or vertices to be parts of the found blocks.
#  @return List of GEOM_Objects, containing the found blocks.
#
#  Example: see GEOM_Spanner.py
def GetBlocksByParts(theCompound, theParts):
    aList = BlocksOp.GetBlocksByParts(theCompound, theParts)
    RaiseIfFailed("GetBlocksByParts", BlocksOp)
    return aList

## Multi-transformate block and glue the result.
#  Transformation is defined so, as to superpose direction faces.
#  @param Block Hexahedral solid to be multi-transformed.
#  @param DirFace1 ID of First direction face.
#  @param DirFace2 ID of Second direction face.
#  @param NbTimes Quantity of transformations to be done.
#    \note Unique ID of sub-shape can be obtained, using method GetSubShapeID().
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_Spanner.py
def MakeMultiTransformation1D(Block, DirFace1, DirFace2, NbTimes):
    anObj = BlocksOp.MakeMultiTransformation1D(Block, DirFace1, DirFace2, NbTimes)
    RaiseIfFailed("MakeMultiTransformation1D", BlocksOp)
    return anObj

## Multi-transformate block and glue the result.
#  @param Block Hexahedral solid to be multi-transformed.
#  @param DirFace1U,DirFace2U IDs of Direction faces for the first transformation.
#  @param DirFace1V,DirFace2V IDs of Direction faces for the second transformation.
#  @param NbTimesU,NbTimesV Quantity of transformations to be done.
#  @return New GEOM_Object, containing the result shape.
#
#  Example: see GEOM_Spanner.py
def MakeMultiTransformation2D(Block, DirFace1U, DirFace2U, NbTimesU,
			             DirFace1V, DirFace2V, NbTimesV):
    anObj = BlocksOp.MakeMultiTransformation2D(Block, DirFace1U, DirFace2U, NbTimesU,
					              DirFace1V, DirFace2V, NbTimesV)
    RaiseIfFailed("MakeMultiTransformation2D", BlocksOp)
    return anObj

## Build all possible propagation groups.
#  Propagation group is a set of all edges, opposite to one (main)
#  edge of this group directly or through other opposite edges.
#  Notion of Opposite Edge make sence only on quadrangle face.
#  @param theShape Shape to build propagation groups on.
#  @return List of GEOM_Objects, each of them is a propagation group.
#
#  Example: see GEOM_TestOthers.py
def Propagate(theShape):
    listChains = BlocksOp.Propagate(theShape)
    RaiseIfFailed("Propagate", BlocksOp)
    return listChains

# -----------------------------------------------------------------------------
# Group operations
# -----------------------------------------------------------------------------

## Creates a new group which will store sub shapes of theMainShape
#  @param theMainShape is a GEOM object on which the group is selected
#  @param theShapeType defines a shape type of the group
#  @return a newly created GEOM group
#
#  Example: see GEOM_TestOthers.py
def CreateGroup(theMainShape, theShapeType):
    anObj = GroupOp.CreateGroup(theMainShape, theShapeType)
    RaiseIfFailed("CreateGroup", GroupOp)
    return anObj

## Adds a sub object with ID theSubShapeId to the group
#  @param theGroup is a GEOM group to which the new sub shape is added
#  @param theSubShapeID is a sub shape ID in the main object.
#  \note Use method GetSubShapeID() to get an unique ID of the sub shape
#
#  Example: see GEOM_TestOthers.py
def AddObject(theGroup, theSubShapeID):
    GroupOp.AddObject(theGroup, theSubShapeID)
    RaiseIfFailed("AddObject", GroupOp)

## Removes a sub object with ID \a theSubShapeId from the group
#  @param theGroup is a GEOM group from which the new sub shape is removed
#  @param theSubShapeID is a sub shape ID in the main object.
#  \note Use method GetSubShapeID() to get an unique ID of the sub shape
#
#  Example: see GEOM_TestOthers.py
def RemoveObject(theGroup, theSubShapeID):
    GroupOp.RemoveObject(theGroup, theSubShapeID)
    RaiseIfFailed("RemoveObject", GroupOp)

## Adds to the group all the given shapes. No errors, if some shapes are alredy included.
#  @param theGroup is a GEOM group to which the new sub shapes are added.
#  @param theSubShapes is a list of sub shapes to be added.
#
#  Example: see GEOM_TestOthers.py
def UnionList (theGroup, theSubShapes):
    GroupOp.UnionList(theGroup, theSubShapes)
    RaiseIfFailed("UnionList", GroupOp)

## Works like the above method, but argument
#  theSubShapes here is a list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def UnionIDs(theGroup, theSubShapes):
    GroupOp.UnionIDs(theGroup, theSubShapes)
    RaiseIfFailed("UnionIDs", GroupOp)

## Removes from the group all the given shapes. No errors, if some shapes are not included.
#  @param theGroup is a GEOM group from which the sub-shapes are removed.
#  @param theSubShapes is a list of sub-shapes to be removed.
#
#  Example: see GEOM_TestOthers.py
def DifferenceList (theGroup, theSubShapes):
    GroupOp.DifferenceList(theGroup, theSubShapes)
    RaiseIfFailed("DifferenceList", GroupOp)

## Works like the above method, but argument
#  theSubShapes here is a list of sub-shapes indices
#
#  Example: see GEOM_TestOthers.py
def DifferenceIDs(theGroup, theSubShapes):
    GroupOp.DifferenceIDs(theGroup, theSubShapes)
    RaiseIfFailed("DifferenceIDs", GroupOp)

## Returns a list of sub objects ID stored in the group
#  @param theGroup is a GEOM group for which a list of IDs is requested
#
#  Example: see GEOM_TestOthers.py
def GetObjectIDs(theGroup):
    ListIDs = GroupOp.GetObjects(theGroup)
    RaiseIfFailed("GetObjects", GroupOp)
    return ListIDs

## Returns a type of sub objects stored in the group
#  @param theGroup is a GEOM group which type is returned.
#
#  Example: see GEOM_TestOthers.py
def GetType(theGroup):
    aType = GroupOp.GetType(theGroup)
    RaiseIfFailed("GetType", GroupOp)
    return aType

## Returns a main shape associated with the group
#  @param theGroup is a GEOM group for which a main shape object is requested
#  @return a GEOM object which is a main shape for theGroup
#
#  Example: see GEOM_TestOthers.py
def GetMainShape(theGroup):
    anObj = GroupOp.GetMainShape(theGroup)
    RaiseIfFailed("GetMainShape", GroupOp)
    return anObj

## Create group of edges of theShape, whose length is in range [min_length, max_length].
#  If include_min/max == 0, edges with length == min/max_length will not be included in result.
def GetEdgesByLength (theShape, min_length, max_length, include_min = 1, include_max = 1):
    edges = SubShapeAll(theShape, ShapeType["EDGE"])
    edges_in_range = []
    for edge in edges:
        Props = BasicProperties(edge)
	if min_length <= Props[0] and Props[0] <= max_length:
	    if (not include_min) and (min_length == Props[0]):
	        skip = 1
            else:
	        if (not include_max) and (Props[0] == max_length):
	            skip = 1
                else:
	            edges_in_range.append(edge)

    if len(edges_in_range) <= 0:
        print "No edges found by given criteria"
	return 0

    group_edges = CreateGroup(theShape, ShapeType["EDGE"])
    UnionList(group_edges, edges_in_range)

    return group_edges

## Create group of edges of selected shape, whose length is in range [min_length, max_length].
#  If include_min/max == 0, edges with length == min/max_length will not be included in result.
def SelectEdges (min_length, max_length, include_min = 1, include_max = 1):
    nb_selected = sg.SelectedCount()
    if nb_selected < 1:
        print "Select a shape before calling this function, please."
	return 0
    if nb_selected > 1:
        print "Only one shape must be selected"
	return 0

    id_shape = sg.getSelected(0)
    shape = IDToObject( id_shape )

    group_edges = GetEdgesByLength(shape, min_length, max_length, include_min, include_max)

    left_str  = " < "
    right_str = " < "
    if include_min: left_str  = " <= "
    if include_max: right_str  = " <= "

    addToStudyInFather(shape, group_edges, "Group of edges with " + `min_length`
		       + left_str + "length" + right_str + `max_length`)

    sg.updateObjBrowser(1)

    return group_edges

## Add Path to load python scripts from
def addPath(Path):
    if (sys.path.count(Path) < 1):
	sys.path.append(Path)
