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
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org
#
#
#
#  File   : geompy.py
#  Author : Paul RASCLE, EDF
#  Module : GEOM
#  $Header$

from salome import *
import GEOM

"""
    \namespace geompy
    \brief Module geompy
"""

g = lcc.FindOrLoadComponent("FactoryServer", "GEOM")
geom = g._narrow( GEOM.GEOM_Gen )
gg = ImportComponentGUI("GEOM") 

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

def SubShapeName(aSubObj, aMainObj):
    """
     *  Get name for sub-shape aSubObj of shape aMainObj

     *  Example: see GEOM_TestAll.py
    """
    aSubId  = orb.object_to_string(aSubObj)
    aMainId = orb.object_to_string(aMainObj)
    index = gg.getIndexTopology(aSubId, aMainId)
    name = gg.getShapeTypeString(aSubId) + "_%d"%(index)
    return name

def addToStudy(aShape, aName):
    """
     *  Publish in study aShape with name aName

     *  Example: see GEOM_TestAll.py
    """
    try:
        aSObject = geom.AddInStudy(myStudy, aShape, aName, None)
    except:
        print "addToStudy() failed"
        return ""
    return aShape.GetStudyEntry()

def addToStudyInFather(aFather, aShape, aName):
    """
     *  Publish in study aShape with name aName as sub-object of previously published aFather

     *  Example: see GEOM_TestAll.py
    """
    try:
        aSObject = geom.AddInStudy(myStudy, aShape, aName, aFather)
    except:
        print "addToStudyInFather() failed"
        return ""
    return aShape.GetStudyEntry()

# -----------------------------------------------------------------------------
# enumeration ShapeType as a dictionary
# -----------------------------------------------------------------------------

ShapeType = {"COMPOUND":0, "COMPSOLID":1, "SOLID":2, "SHELL":3, "FACE":4, "WIRE":5, "EDGE":6, "VERTEX":7, "SHAPE":8}

# -----------------------------------------------------------------------------
# Basic primitives
# -----------------------------------------------------------------------------

def MakeVertex(theX, theY, theZ):
    """
     *  Create point by three coordinates.
     *  \param theX The X coordinate of the point.
     *  \param theY The Y coordinate of the point.
     *  \param theZ The Z coordinate of the point.
     *  \return New GEOM_Object, containing the created point.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePointXYZ(theX, theY, theZ)
    if BasicOp.IsDone() == 0:
      print "MakePointXYZ : ", BasicOp.GetErrorCode()
    return anObj

def MakeVertexWithRef(theReference, theX, theY, theZ):
    """
     *  Create a point, distant from the referenced point
     *  on the given distances along the coordinate axes.
     *  \param theReference The referenced point.
     *  \param theX Displacement from the referenced point along OX axis.
     *  \param theY Displacement from the referenced point along OY axis.
     *  \param theZ Displacement from the referenced point along OZ axis.
     *  \return New GEOM_Object, containing the created point.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePointWithReference(theReference, theX, theY, theZ)
    if BasicOp.IsDone() == 0:
      print "MakePointWithReference : ", BasicOp.GetErrorCode()
    return anObj

def MakeVertexOnCurve(theRefCurve, theParameter):
    """
     *  Create a point, corresponding to the given parameter on the given curve.
     *  \param theRefCurve The referenced curve.
     *  \param theParameter Value of parameter on the referenced curve.
     *  \return New GEOM_Object, containing the created point.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePointOnCurve(theRefCurve, theParameter)
    if BasicOp.IsDone() == 0:
      print "MakePointOnCurve : ", BasicOp.GetErrorCode()
    return anObj

def MakeVectorDXDYDZ(theDX, theDY, theDZ):
    """
     *  Create a vector with the given components.
     *  \param theDX X component of the vector.
     *  \param theDY Y component of the vector.
     *  \param theDZ Z component of the vector.
     *  \return New GEOM_Object, containing the created vector.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakeVectorDXDYDZ(theDX, theDY, theDZ)
    if BasicOp.IsDone() == 0:
      print "MakeVectorDXDYDZ : ", BasicOp.GetErrorCode()
    return anObj

def MakeVector(thePnt1, thePnt2):
    """
     *  Create a vector between two points.
     *  \param thePnt1 Start point for the vector.
     *  \param thePnt2 End point for the vector.
     *  \return New GEOM_Object, containing the created vector.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakeVectorTwoPnt(thePnt1, thePnt2)
    if BasicOp.IsDone() == 0:
      print "MakeVectorTwoPnt : ", BasicOp.GetErrorCode()
    return anObj

def MakeLine(thePnt, theDir):
    """
     *  Create a line, passing through the given point
     *  and parrallel to the given direction
     *  \param thePnt Point. The resulting line will pass through it.
     *  \param theDir Direction. The resulting line will be parallel to it.
     *  \return New GEOM_Object, containing the created line.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakeLine(thePnt, theDir)
    if BasicOp.IsDone() == 0:
      print "MakeLine : ", BasicOp.GetErrorCode()
    return anObj

def MakeLineTwoPnt(thePnt1, thePnt2):
    """
     *  Create a line, passing through the given points
     *  \param thePnt1 First of two points, defining the line.
     *  \param thePnt2 Second of two points, defining the line.
     *  \return New GEOM_Object, containing the created line.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakeLineTwoPnt(thePnt1, thePnt2)
    if BasicOp.IsDone() == 0:
      print "MakeLineTwoPnt : ", BasicOp.GetErrorCode()
    return anObj

def MakePlane(thePnt, theVec, theTrimSize):
    """
     *  Create a plane, passing through the given point
     *  and normal to the given vector.
     *  \param thePnt Point, the plane has to pass through.
     *  \param theVec Vector, defining the plane normal direction.
     *  \param theTrimSize Half size of a side of quadrangle face, representing the plane.
     *  \return New GEOM_Object, containing the created plane.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePlanePntVec(thePnt, theVec, theTrimSize)
    if BasicOp.IsDone() == 0:
      print "MakePlanePntVec : ", BasicOp.GetErrorCode()
    return anObj

def MakePlaneThreePnt(thePnt1, thePnt2, thePnt3, theTrimSize):
    """
     *  Create a plane, passing through the three given points
     *  \param thePnt1 First of three points, defining the plane.
     *  \param thePnt2 Second of three points, defining the plane.
     *  \param thePnt3 Fird of three points, defining the plane.
     *  \param theTrimSize Half size of a side of quadrangle face, representing the plane.
     *  \return New GEOM_Object, containing the created plane.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePlaneThreePnt(thePnt1, thePnt2, thePnt3, theTrimSize)
    if BasicOp.IsDone() == 0:
      print "MakePlaneThreePnt : ", BasicOp.GetErrorCode()
    return anObj

def MakePlaneFace(theFace, theTrimSize):
    """
     *  Create a plane, similar to the existing one, but with another size of representing face.
     *  \param theFace Referenced plane.
     *  \param theTrimSize New half size of a side of quadrangle face, representing the plane.
     *  \return New GEOM_Object, containing the created plane.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakePlaneFace(theFace, theTrimSize)
    if BasicOp.IsDone() == 0:
      print "MakePlaneFace : ", BasicOp.GetErrorCode()
    return anObj

def MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ):
    """
     *  Create a local coordinate system.
     *  \param OX,OY,OZ Three coordinates of coordinate system origin.
     *  \param XDX,XDY,XDZ Three components of OX direction
     *  \param YDX,YDY,YDZ Three components of OY direction
     *  \return New GEOM_Object, containing the created coordinate system.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BasicOp.MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ)
    if BasicOp.IsDone() == 0:
      print "MakeMarker : ", BasicOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Curves
# -----------------------------------------------------------------------------

def MakeArc(thePnt1, thePnt2, thePnt3):
    """
     *  Create an arc of circle, passing through three given points.
     *  \param thePnt1 Start point of the arc.
     *  \param thePnt2 Middle point of the arc.
     *  \param thePnt3 End point of the arc.
     *  \return New GEOM_Object, containing the created arc.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeArc(thePnt1, thePnt2, thePnt3)
    if CurvesOp.IsDone() == 0:
      print "MakeArc : ", CurvesOp.GetErrorCode()
    return anObj

def MakeCircle(thePnt, theVec, theR):
    """
     *  Create a circle with given center, normal vector and radius.
     *  \param thePnt Circle center.
     *  \param theVec Vector, normal to the plane of the circle.
     *  \param theR Circle radius.
     *  \return New GEOM_Object, containing the created circle.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeCirclePntVecR(thePnt, theVec, theR)
    if CurvesOp.IsDone() == 0:
      print "MakeCirclePntVecR : ", CurvesOp.GetErrorCode()
    return anObj

def MakeCircleThreePnt(thePnt1, thePnt2, thePnt3):
    """
     *  Create a circle, passing through three given points
     *  \param thePnt1,thePnt2,thePnt3 Points, defining the circle.
     *  \return New GEOM_Object, containing the created circle.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeCircleThreePnt(thePnt1, thePnt2, thePnt3)
    if CurvesOp.IsDone() == 0:
      print "MakeCircleThreePnt : ", CurvesOp.GetErrorCode()
    return anObj

def MakeEllipse(thePnt, theVec, theRMajor, theRMinor):
    """
     *  Create an ellipse with given center, normal vector and radiuses.
     *  \param thePnt Ellipse center.
     *  \param theVec Vector, normal to the plane of the ellipse.
     *  \param theRMajor Major ellipse radius.
     *  \param theRMinor Minor ellipse radius.
     *  \return New GEOM_Object, containing the created ellipse.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeEllipse(thePnt, theVec, theRMajor, theRMinor)
    if CurvesOp.IsDone() == 0:
      print "MakeEllipse : ", CurvesOp.GetErrorCode()
    return anObj

def MakePolyline(thePoints):
    """
     *  Create a polyline on the set of points.
     *  \param thePoints Sequence of points for the polyline.
     *  \return New GEOM_Object, containing the created polyline.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakePolyline(thePoints)
    if CurvesOp.IsDone() == 0:
      print "MakePolyline : ", CurvesOp.GetErrorCode()
    return anObj

def MakeBezier(thePoints):
    """
     *  Create bezier curve on the set of points.
     *  \param thePoints Sequence of points for the bezier curve.
     *  \return New GEOM_Object, containing the created bezier curve.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeSplineBezier(thePoints)
    if CurvesOp.IsDone() == 0:
      print "MakeSplineBezier : ", CurvesOp.GetErrorCode()
    return anObj

def MakeInterpol(thePoints):
    """
     *  Create B-Spline curve on the set of points.
     *  \param thePoints Sequence of points for the B-Spline curve.
     *  \return New GEOM_Object, containing the created B-Spline curve.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeSplineInterpolation(thePoints)
    if CurvesOp.IsDone() == 0:
      print "MakeSplineInterpolation : ", CurvesOp.GetErrorCode()
    return anObj

def MakeSketcher(theCommand, theWorkingPlane = [0,0,0, 0,0,1, 1,0,0]):
    """
     *  Create a sketcher (wire or face), following the textual description,
     *  passed through \a theCommand argument. \n
     *  Edges of the resulting wire or face will be arcs of circles and/or linear segments. \n
     *  Format of the description string have to be the following:
     *
     *  "Sketcher[:F x1 y1]:CMD[:CMD[:CMD...]]"
     *
     *  Where:
     *  - x1, y1 are coordinates of the first sketcher point (zero by default),
     *  - CMD is one of
     *     - "R angle" : Set the direction by angle
     *     - "D dx dy" : Set the direction by DX & DY
     *     .
     *       \n
     *     - "TT x y" : Create segment by point at X & Y
     *     - "T dx dy" : Create segment by point with DX & DY
     *     - "L length" : Create segment by direction & Length
     *     - "IX x" : Create segment by direction & Intersect. X
     *     - "IY y" : Create segment by direction & Intersect. Y
     *     .
     *       \n
     *     - "C radius length" : Create arc by direction, radius and length(in degree)
     *     .
     *       \n
     *     - "WW" : Close Wire (to finish)
     *     - "WF" : Close Wire and build face (to finish)
     *
     *  \param theCommand String, defining the sketcher in local
     *                    coordinates of the working plane.
     *  \param theWorkingPlane Nine double values, defining origin,
     *                         OZ and OX directions of the working plane.
     *  \return New GEOM_Object, containing the created wire.

     *  Example: see GEOM_TestAll.py
    """
    anObj = CurvesOp.MakeSketcher(theCommand, theWorkingPlane)
    if CurvesOp.IsDone() == 0:
      print "MakeSketcher : ", CurvesOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create 3D Primitives
# -----------------------------------------------------------------------------

def MakeBox(x1,y1,z1,x2,y2,z2):
    """
     *  Create a box by coordinates of two opposite vertices.

     *  Example: see GEOM_TestAll.py
    """
    pnt1 = MakeVertex(x1,y1,z1)
    pnt2 = MakeVertex(x2,y2,z2)
    return MakeBoxTwoPnt(pnt1,pnt2)

def MakeBoxDXDYDZ(theDX, theDY, theDZ):
    """
     *  Create a box with specified dimensions along the coordinate axes
     *  and with edges, parallel to the coordinate axes.
     *  Center of the box will be at point (DX/2, DY/2, DZ/2).
     *  \param theDX Length of Box edges, parallel to OX axis.
     *  \param theDY Length of Box edges, parallel to OY axis.
     *  \param theDZ Length of Box edges, parallel to OZ axis.
     *  \return New GEOM_Object, containing the created box.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeBoxDXDYDZ(theDX, theDY, theDZ)
    if PrimOp.IsDone() == 0:
      print "MakeBoxDXDYDZ : ", PrimOp.GetErrorCode()
    return anObj

def MakeBoxTwoPnt(thePnt1, thePnt2):
    """
     *  Create a box with two specified opposite vertices,
     *  and with edges, parallel to the coordinate axes
     *  \param thePnt1 First of two opposite vertices.
     *  \param thePnt2 Second of two opposite vertices.
     *  \return New GEOM_Object, containing the created box.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeBoxTwoPnt(thePnt1, thePnt2)
    if PrimOp.IsDone() == 0:
      print "MakeBoxTwoPnt : ", PrimOp.GetErrorCode()
    return anObj

def MakeCylinder(thePnt, theAxis, theR, theH):
    """
     *  Create a cylinder with given base point, axis, radius and height.
     *  \param thePnt Central point of cylinder base.
     *  \param theAxis Cylinder axis.
     *  \param theR Cylinder radius.
     *  \param theH Cylinder height.
     *  \return New GEOM_Object, containing the created cylinder.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeCylinderPntVecRH(thePnt, theAxis, theR, theH)
    if PrimOp.IsDone() == 0:
      print "MakeCylinderPntVecRH : ", PrimOp.GetErrorCode()
    return anObj

def MakeCylinderRH(theR, theH):
    """
     *  Create a cylinder with given radius and height at
     *  the origin of coordinate system. Axis of the cylinder
     *  will be collinear to the OZ axis of the coordinate system.
     *  \param theR Cylinder radius.
     *  \param theH Cylinder height.
     *  \return New GEOM_Object, containing the created cylinder.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeCylinderRH(theR, theH)
    if PrimOp.IsDone() == 0:
      print "MakeCylinderRH : ", PrimOp.GetErrorCode()
    return anObj

def MakeSpherePntR(thePnt, theR):
    """
     *  Create a sphere with given center and radius.
     *  \param thePnt Sphere center.
     *  \param theR Sphere radius.
     *  \return New GEOM_Object, containing the created sphere.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeSpherePntR(thePnt, theR)
    if PrimOp.IsDone() == 0:
      print "MakeSpherePntR : ", PrimOp.GetErrorCode()
    return anObj

def MakeSphere(x, y, z, theR):
    """
     *  Create a sphere with given center and radius.
     *  \param x,y,z Coordinates of sphere center.
     *  \param theR Sphere radius.
     *  \return New GEOM_Object, containing the created sphere.

     *  Example: see GEOM_TestAll.py
    """
    point = MakeVertex(x, y, z)
    anObj = MakeSpherePntR(point, theR)
    return anObj

def MakeSphereR(theR):
    """
     *  Create a sphere with given radius at the origin of coordinate system.
     *  \param theR Sphere radius.
     *  \return New GEOM_Object, containing the created sphere.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeSphereR(theR)
    if PrimOp.IsDone() == 0:
      print "MakeSphereR : ", PrimOp.GetErrorCode()
    return anObj

def MakeCone(thePnt, theAxis, theR1, theR2, theH):
    """
     *  Create a cone with given base point, axis, height and radiuses.
     *  \param thePnt Central point of the first cone base.
     *  \param theAxis Cone axis.
     *  \param theR1 Radius of the first cone base.
     *  \param theR2 Radius of the second cone base.
     *    \note If both radiuses are non-zero, the cone will be truncated.
     *    \note If the radiuses are equal, a cylinder will be created instead.
     *  \param theH Cone height.
     *  \return New GEOM_Object, containing the created cone.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeConePntVecR1R2H(thePnt, theAxis, theR1, theR2, theH)
    if PrimOp.IsDone() == 0:
      print "MakeConePntVecR1R2H : ", PrimOp.GetErrorCode()
    return anObj

def MakeConeR1R2H(theR1, theR2, theH):
    """
     *  Create a cone with given height and radiuses at
     *  the origin of coordinate system. Axis of the cone will
     *  be collinear to the OZ axis of the coordinate system.
     *  \param theR1 Radius of the first cone base.
     *  \param theR2 Radius of the second cone base.
     *    \note If both radiuses are non-zero, the cone will be truncated.
     *    \note If the radiuses are equal, a cylinder will be created instead.
     *  \param theH Cone height.
     *  \return New GEOM_Object, containing the created cone.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeConeR1R2H(theR1, theR2, theH)
    if PrimOp.IsDone() == 0:
      print "MakeConeR1R2H : ", PrimOp.GetErrorCode()
    return anObj

def MakeTorus(thePnt, theVec, theRMajor, theRMinor):
    """
     *  Create a torus with given center, normal vector and radiuses.
     *  \param thePnt Torus central point.
     *  \param theVec Torus axis of symmetry.
     *  \param theRMajor Torus major radius.
     *  \param theRMinor Torus minor radius.
     *  \return New GEOM_Object, containing the created torus.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeTorusPntVecRR(thePnt, theVec, theRMajor, theRMinor)
    if PrimOp.IsDone() == 0:
      print "MakeTorusPntVecRR : ", PrimOp.GetErrorCode()
    return anObj

def MakeTorusRR(theRMajor, theRMinor):
    """
     *  Create a torus with given radiuses at the origin of coordinate system.
     *  \param theRMajor Torus major radius.
     *  \param theRMinor Torus minor radius.
     *  \return New GEOM_Object, containing the created torus.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeTorusRR(theRMajor, theRMinor)
    if PrimOp.IsDone() == 0:
      print "MakeTorusRR : ", PrimOp.GetErrorCode()
    return anObj

def MakePrism(theBase, thePoint1, thePoint2):
    """
     *  Create a shape by extrusion of the base shape along a vector, defined by two points.
     *  \param theBase Base shape to be extruded.
     *  \param thePoint1 First end of extrusion vector.
     *  \param thePoint2 Second end of extrusion vector.
     *  \return New GEOM_Object, containing the created prism.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakePrismTwoPnt(theBase, thePoint1, thePoint2)
    if PrimOp.IsDone() == 0:
      print "MakePrismTwoPnt : ", PrimOp.GetErrorCode()
    return anObj

def MakePrismVecH(theBase, theVec, theH):
    """
     *  Create a shape by extrusion of the base shape along the vector,
     *  i.e. all the space, transfixed by the base shape during its translation
     *  along the vector on the given distance.
     *  \param theBase Base shape to be extruded.
     *  \param theVec Direction of extrusion.
     *  \param theH Prism dimension along theVec.
     *  \return New GEOM_Object, containing the created prism.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakePrismVecH(theBase, theVec, theH)
    if PrimOp.IsDone() == 0:
      print "MakePrismVecH : ", PrimOp.GetErrorCode()
    return anObj

def MakePipe(theBase, thePath):
    """
     *  Create a shape by extrusion of the base shape along
     *  the path shape. The path shape can be a wire or an edge.
     *  \param theBase Base shape to be extruded.
     *  \param thePath Path shape to extrude the base shape along it.
     *  \return New GEOM_Object, containing the created pipe.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakePipe(theBase, thePath)
    if PrimOp.IsDone() == 0:
      print "MakePipe : ", PrimOp.GetErrorCode()
    return anObj

def MakeRevolution(theBase, theAxis, theAngle):
    """
     *  Create a shape by revolution of the base shape around the axis
     *  on the given angle, i.e. all the space, transfixed by the base
     *  shape during its rotation around the axis on the given angle.
     *  \param theBase Base shape to be rotated.
     *  \param theAxis Rotation axis.
     *  \param theAngle Rotation angle in radians.
     *  \return New GEOM_Object, containing the created revolution.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeRevolutionAxisAngle(theBase, theAxis, theAngle)
    if PrimOp.IsDone() == 0:
      print "MakeRevolutionAxisAngle : ", PrimOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create base shapes
# -----------------------------------------------------------------------------

def MakeEdge(thePnt1, thePnt2):
    """
     *  Create a linear edge with specified ends.
     *  \param thePnt1 Point for the first end of edge.
     *  \param thePnt2 Point for the second end of edge.
     *  \return New GEOM_Object, containing the created edge.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeEdge(thePnt1, thePnt2)
    if ShapesOp.IsDone() == 0:
      print "MakeEdge : ", ShapesOp.GetErrorCode()
    return anObj

def MakeWire(theEdgesAndWires):
    """
     *  Create a wire from the set of edges and wires.
     *  \param theEdgesAndWires List of edges and/or wires.
     *  \return New GEOM_Object, containing the created wire.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeWire(theEdgesAndWires)
    if ShapesOp.IsDone() == 0:
      print "MakeWire : ", ShapesOp.GetErrorCode()
    return anObj

def MakeFace(theWire, isPlanarWanted):
    """
     *  Create a face on the given wire.
     *  \param theWire Wire to build the face on.
     *  \param isPlanarWanted If TRUE, only planar face will be built.
     *                        If impossible, NULL object will be returned.
     *  \return New GEOM_Object, containing the created face.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeFace(theWire, isPlanarWanted)
    if ShapesOp.IsDone() == 0:
      print "MakeFace : ", ShapesOp.GetErrorCode()
    return anObj

def MakeFaceWires(theWires, isPlanarWanted):
    """
     *  Create a face on the given wires set.
     *  \param theWires List of wires to build the face on.
     *  \param isPlanarWanted If TRUE, only planar face will be built.
     *                        If impossible, NULL object will be returned.
     *  \return New GEOM_Object, containing the created face.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeFaceWires(theWires, isPlanarWanted)
    if ShapesOp.IsDone() == 0:
      print "MakeFaceWires : ", ShapesOp.GetErrorCode()
    return anObj

def MakeFaces(theWires, isPlanarWanted):
    """
     *  Shortcut to MakeFaceWires()

     *  Example: see GEOM_TestOthers.py
    """
    anObj = MakeFaceWires(theWires, isPlanarWanted)
    return anObj

def MakeShell(theFacesAndShells):
    """
     *  Create a shell from the set of faces and shells.
     *  \param theFacesAndShells List of faces and/or shells.
     *  \return New GEOM_Object, containing the created shell.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeShell(theFacesAndShells)
    if ShapesOp.IsDone() == 0:
	print "MakeShell : ", ShapesOp.GetErrorCode()
    return anObj

def MakeSolid(theShells):
    """
     *  Create a solid, bounded by the given shells.
     *  \param theShells Sequence of bounding shells.
     *  \return New GEOM_Object, containing the created solid.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeSolidShells(theShells)
    if ShapesOp.IsDone() == 0:
	print "MakeSolid : ", ShapesOp.GetErrorCode()
    return anObj

def MakeCompound(theShapes):
    """
     *  Create a compound of the given shapes.
     *  \param theShapes List of shapes to put in compound.
     *  \return New GEOM_Object, containing the created compound.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.MakeCompound(theShapes)
    if ShapesOp.IsDone() == 0:
      print "MakeCompound : ", ShapesOp.GetErrorCode()
    return anObj

def NumberOfFaces(theShape):
    """
     *  Gives quantity of faces in the given shape.
     *  \param theShape Shape to count faces of.
     *  \return Quantity of faces.

     *  Example: see GEOM_TestOthers.py
    """
    nb_faces = ShapesOp.NumberOfFaces(theShape)
    if ShapesOp.IsDone() == 0:
      print "NumberOfFaces : ", ShapesOp.GetErrorCode()
    return nb_faces

def NumberOfEdges(theShape):
    """
     *  Gives quantity of edges in the given shape.
     *  \param theShape Shape to count edges of.
     *  \return Quantity of edges.

     *  Example: see GEOM_TestOthers.py
    """
    nb_edges = ShapesOp.NumberOfEdges(theShape)
    if ShapesOp.IsDone() == 0:
      print "NumberOfEdges : ", ShapesOp.GetErrorCode()
    return nb_edges

def ChangeOrientation(theShape):
    """
     *  Reverses an orientation the given shape.
     *  \param theShape Shape to be reversed.
     *  \return The reversed copy of theShape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = ShapesOp.ChangeOrientation(theShape)
    if ShapesOp.IsDone() == 0:
      print "ChangeOrientation : ", ShapesOp.GetErrorCode()
    return anObj

def OrientationChange(theShape):
    """
     *  Shortcut to ChangeOrientation()

     *  Example: see GEOM_TestOthers.py
    """
    anObj = ChangeOrientation(theShape)
    return anObj

def GetFreeFacesIDs(theShape):
    """
     *  Retrieve all free faces from the given shape.
     *  Free face is a face, which is not shared between two shells of the shape.
     *  \param theShape Shape to find free faces in.
     *  \return List of IDs of all free faces, contained in theShape.

     *  Example: see GEOM_TestOthers.py
    """
    anIDs = ShapesOp.GetFreeFacesIDs(theShape)
    if ShapesOp.IsDone() == 0:
      print "GetFreeFacesIDs : ", ShapesOp.GetErrorCode()
    return anIDs

def GetSharedShapes(theShape1, theShape2, theShapeType):
    """
     *  Get all sub-shapes of theShape1 of the given type, shared with theShape2.
     *  \param theShape1 Shape to find sub-shapes in.
     *  \param theShape2 Shape to find shared sub-shapes with.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \return List of sub-shapes of theShape1, shared with theShape2.

     *  Example: see GEOM_TestOthers.py
    """
    aList = ShapesOp.GetSharedShapes(theShape1, theShape2, theShapeType)
    if ShapesOp.IsDone() == 0:
      print "GetSharedShapes : ", ShapesOp.GetErrorCode()
    return aList

def GetShapesOnPlane(theShape, theShapeType, theAx1, theState):
    """
     *  Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
     *  the specified plane by the certain way, defined through \a theState parameter.
     *  \param theShape Shape to find sub-shapes of.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \param theAx1 Vector (or line, or linear edge), specifying normal
     *                direction and location of the plane to find shapes on.
     *  \param theState The state of the subshapes to find. It can be one of
     *   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
     *  \return List of all found sub-shapes.

     *  Example: see GEOM_TestOthers.py
    """
    aList = ShapesOp.GetShapesOnPlane(theShape, theShapeType, theAx1, theState)
    if ShapesOp.IsDone() == 0:
      print "GetShapesOnPlane : ", ShapesOp.GetErrorCode()
    return aList

def GetShapesOnCylinder(theShape, theShapeType, theAxis, theRadius, theState):
    """
     *  Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
     *  the specified cylinder by the certain way, defined through \a theState parameter.
     *  \param theShape Shape to find sub-shapes of.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \param theAxis Vector (or line, or linear edge), specifying
     *                 axis of the cylinder to find shapes on.
     *  \param theRadius Radius of the cylinder to find shapes on.
     *  \param theState The state of the subshapes to find. It can be one of
     *   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
     *  \return List of all found sub-shapes.

     *  Example: see GEOM_TestOthers.py
    """
    aList = ShapesOp.GetShapesOnCylinder(theShape, theShapeType, theAxis, theRadius, theState)
    if ShapesOp.IsDone() == 0:
      print "GetShapesOnCylinder : ", ShapesOp.GetErrorCode()
    return aList

def GetShapesOnSphere(theShape, theShapeType, theCenter, theRadius, theState):
    """
     *  Find in \a theShape all sub-shapes of type \a theShapeType, situated relatively
     *  the specified sphere by the certain way, defined through \a theState parameter.
     *  \param theShape Shape to find sub-shapes of.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \param theCenter Point, specifying center of the sphere to find shapes on.
     *  \param theRadius Radius of the sphere to find shapes on.
     *  \param theState The state of the subshapes to find. It can be one of
     *   ST_ON, ST_OUT, ST_ONOUT, ST_IN, ST_ONIN.
     *  \return List of all found sub-shapes.

     *  Example: see GEOM_TestOthers.py
    """
    aList = ShapesOp.GetShapesOnSphere(theShape, theShapeType, theCenter, theRadius, theState)
    if ShapesOp.IsDone() == 0:
      print "GetShapesOnSphere : ", ShapesOp.GetErrorCode()
    return aList

def GetInPlace(theShapeWhere, theShapeWhat):
    """
     *  Get sub-shape(s) of theShapeWhere, which are
     *  coincident with \a theShapeWhat or could be a part of it.
     *  \param theShapeWhere Shape to find sub-shapes of.
     *  \param theShapeWhat Shape, specifying what to find.
     *  \return Group of all found sub-shapes or a single found sub-shape.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = ShapesOp.GetInPlace(theShapeWhere, theShapeWhat)
    if ShapesOp.IsDone() == 0:
      print "GetInPlace : ", ShapesOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Access to sub-shapes by their unique IDs inside the main shape.
# -----------------------------------------------------------------------------

def GetSubShape(aShape, ListOfID):
    """
     *  Obtain a composite sub-shape of <aShape>, composed from sub-shapes
     *  of <aShape>, selected by their unique IDs inside <aShape>

     *  Example: see GEOM_TestAll.py
    """
    anObj = geom.AddSubShape(aShape,ListOfID)
    return anObj

def GetSubShapeID(aShape, aSubShape):
    """
     *  Obtain unique ID of sub-shape <aSubShape> inside <aShape>

     *  Example: see GEOM_TestAll.py
    """
    anID = LocalOp.GetSubShapeIndex(aShape, aSubShape)
    if LocalOp.IsDone() == 0:
      print "GetSubShapeIndex : ", LocalOp.GetErrorCode()
    return anID

# -----------------------------------------------------------------------------
# Decompose objects
# -----------------------------------------------------------------------------

def SubShapeAll(aShape, aType):
    """
     *  Explode a shape on subshapes of a given type.
     *  \param theShape Shape to be exploded.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \return List of sub-shapes of type theShapeType, contained in theShape.

     *  Example: see GEOM_TestAll.py
    """
    ListObj = ShapesOp.MakeExplode(aShape,aType,0)
    if ShapesOp.IsDone() == 0:
      print "MakeExplode : ", ShapesOp.GetErrorCode()
    return ListObj

def SubShapeAllIDs(aShape, aType):
    """
     *  Explode a shape on subshapes of a given type.
     *  \param theShape Shape to be exploded.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \return List of IDs of sub-shapes.
    """
    ListObj = ShapesOp.SubShapeAllIDs(aShape,aType,0)
    if ShapesOp.IsDone() == 0:
      print "SubShapeAllIDs : ", ShapesOp.GetErrorCode()
    return ListObj

def SubShapeAllSorted(aShape, aType):
    """
     *  Explode a shape on subshapes of a given type.
     *  Sub-shapes will be sorted by coordinates of their gravity centers.
     *  \param theShape Shape to be exploded.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \return List of sub-shapes of type theShapeType, contained in theShape.

     *  Example: see GEOM_TestAll.py
    """
    ListObj = ShapesOp.MakeExplode(aShape,aType,1)
    if ShapesOp.IsDone() == 0:
      print "MakeExplode : ", ShapesOp.GetErrorCode()
    return ListObj

def SubShapeAllSortedIDs(aShape, aType):
    """
     *  Explode a shape on subshapes of a given type.
     *  Sub-shapes will be sorted by coordinates of their gravity centers.
     *  \param theShape Shape to be exploded.
     *  \param theShapeType Type of sub-shapes to be retrieved.
     *  \return List of IDs of sub-shapes.
    """
    ListIDs = ShapesOp.SubShapeAllIDs(aShape,aType,1)
    if ShapesOp.IsDone() == 0:
      print "SubShapeAllSortedIDs : ", ShapesOp.GetErrorCode()
    return ListObj

def SubShape(aShape, aType, ListOfInd):
    """
     *  Obtain a compound of sub-shapes of <aShape>,
     *  selected by they indices in list of all sub-shapes of type <aType>.
     *  Each index is in range [1, Nb_Sub-Shapes_Of_Given_Type]

     *  Example: see GEOM_TestAll.py
    """
    ListOfIDs = []
    AllShapeList = SubShapeAll(aShape, aType)
    for ind in ListOfInd:
        ListOfIDs.append(GetSubShapeID(aShape, AllShapeList[ind - 1]))
    anObj = GetSubShape(aShape, ListOfIDs)
    return anObj

def SubShapeSorted(aShape, aType, ListOfInd):
    """
     *  Obtain a compound of sub-shapes of <aShape>,
     *  selected by they indices in sorted list of all sub-shapes of type <aType>.
     *  Each index is in range [1, Nb_Sub-Shapes_Of_Given_Type]

     *  Example: see GEOM_TestAll.py
    """
    ListOfIDs = []
    AllShapeList = SubShapeAllSorted(aShape, aType)
    for ind in ListOfInd:
        ListOfIDs.append(GetSubShapeID(aShape, AllShapeList[ind - 1]))
    anObj = GetSubShape(aShape, ListOfIDs)
    return anObj

# -----------------------------------------------------------------------------
# Healing operations
# -----------------------------------------------------------------------------

def ProcessShape(theShape, theOperators, theParameters, theValues):
    """
     *  Apply a sequence of Shape Healing operators to the given object.
     *  \param theShape Shape to be processed.
     *  \param theOperators List of names of operators ("FixShape", "SplitClosedFaces", etc.).
     *  \param theParameters List of names of parameters
     *                    ("FixShape.Tolerance3d", "SplitClosedFaces.NbSplitPoints", etc.).
     *  \param theValues List of values of parameters, in the same order
     *                    as parameters are listed in \a theParameters list.
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.ProcessShape(theShape, theOperators, theParameters, theValues)
    if HealOp.IsDone() == 0:
	print "ProcessShape : ", HealOp.GetErrorCode()
    return anObj

def SuppressFaces(theObject, theFaces):
    """
     *  Remove faces from the given object (shape).
     *  \param theObject Shape to be processed.
     *  \param theFaces Indices of faces to be removed, if EMPTY then the method
     *                  removes ALL faces of the given object.
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.SuppressFaces(theObject, theFaces)
    if HealOp.IsDone() == 0:
      print "SuppressFaces : ", HealOp.GetErrorCode()
    return anObj

def MakeSewing(ListShape, theTolerance):
    """
     *  Sewing of some shapes into single shape.

     *  Example: see GEOM_TestHealing.py
    """
    comp = MakeCompound(ListShape)
    anObj = Sew(comp, theTolerance)
    return anObj

def Sew(theObject, theTolerance):
    """
     *  Sewing of the given object.
     *  \param theObject Shape to be processed.
     *  \param theTolerance Required tolerance value.
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see MakeSewing() above
    """
    anObj = HealOp.Sew(theObject, theTolerance)
    if HealOp.IsDone() == 0:
      print "Sew : ", HealOp.GetErrorCode()
    return anObj

def SuppressInternalWires(theObject, theWires):
    """
     *  Remove internal wires and edges from the given object (face).
     *  \param theObject Shape to be processed.
     *  \param theWires Indices of wires to be removed, if EMPTY then the method
     *                  removes ALL internal wires of the given object.
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.RemoveIntWires(theObject, theWires)
    if HealOp.IsDone() == 0:
      print "SuppressInternalWires : ", HealOp.GetErrorCode()
    return anObj

def SuppressHoles(theObject, theWires):
    """
     *  Remove internal closed contours (holes) from the given object.
     *  \param theObject Shape to be processed.
     *  \param theWires Indices of wires to be removed, if EMPTY then the method
     *                  removes ALL internal holes of the given object
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.FillHoles(theObject, theWires)
    if HealOp.IsDone() == 0:
      print "SuppressHoles : ", HealOp.GetErrorCode()
    return anObj

def CloseContour(theObject, theWires, isCommonVertex):
    """
     *  Close an open wire.
     *  \param theObject Shape to be processed.
     *  \param theWires Indexes of edge(s) and wire(s) to be closed within <VAR>theObject</VAR>'s shape,
     *                  if -1, then theObject itself is a wire.
     *  \param isCommonVertex If TRUE : closure by creation of a common vertex,
     *                        If FALS : closure by creation of an edge between ends.
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.CloseContour(theObject, theWires, isCommonVertex)
    if HealOp.IsDone() == 0:
      print "CloseContour : ", HealOp.GetErrorCode()
    return anObj

def DivideEdge(theObject, theEdgeIndex, theValue, isByParameter):
    """
     *  Addition of a point to a given edge object.
     *  \param theObject Shape to be processed.
     *  \param theEdgeIndex Index of edge to be divided within theObject's shape,
     *                      if -1, then theObject itself is the edge.
     *  \param theValue Value of parameter on edge or length parameter,
     *                  depending on \a isByParameter.
     *  \param isByParameter If TRUE : \a theValue is treated as a curve parameter [0..1],
     *                       if FALSE : \a theValue is treated as a length parameter [0..1]
     *  \return New GEOM_Object, containing processed shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.DivideEdge(theObject, theEdgeIndex, theValue, isByParameter)
    if HealOp.IsDone() == 0:
      print "DivideEdge : ", HealOp.GetErrorCode()
    return anObj

def GetFreeBoundary(theObject):
    """
     *  Get a list of wires (wrapped in GEOM_Object-s),
     *  that constitute a free boundary of the given shape.
     *  \param theObject Shape to get free boundary of.
     *  \return [status, theClosedWires, theOpenWires]
     *  status: FALSE, if an error(s) occured during the method execution.
     *  theClosedWires: Closed wires on the free boundary of the given shape.
     *  theOpenWires: Open wires on the free boundary of the given shape.

     *  Example: see GEOM_TestHealing.py
    """
    anObj = HealOp.GetFreeBoundary(theObject)
    if HealOp.IsDone() == 0:
      print "GetFreeBoundaries : ", HealOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create advanced objects
# -----------------------------------------------------------------------------

def MakeCopy(theOriginal):
    """
     *  Create a copy of the given object

     *  Example: see GEOM_TestAll.py
    """
    anObj = InsertOp.MakeCopy(theOriginal)
    if InsertOp.IsDone() == 0:
      print "MakeCopy : ", InsertOp.GetErrorCode()
    return anObj

def MakeFilling(theShape, theMinDeg, theMaxDeg, theTol2D, theTol3D, theNbIter):
    """
     *  Create a filling from the given compound of contours.
     *  \param theShape the compound of contours
     *  \param theMinDeg a minimal degree
     *  \param theMaxDeg a maximal degree
     *  \param theTol2D a 2d tolerance
     *  \param theTol3D a 3d tolerance
     *  \param theNbIter a number of iteration
     *  \return New GEOM_Object, containing the created filling surface.

     *  Example: see GEOM_TestAll.py
    """
    anObj = PrimOp.MakeFilling(theShape, theMinDeg, theMaxDeg, theTol2D, theTol3D, theNbIter)
    if PrimOp.IsDone() == 0:
      print "MakeFilling : ", PrimOp.GetErrorCode()
    return anObj

def MakeGlueFaces(theShape, theTolerance):
    """
     *  Replace coincident faces in theShape by one face.
     *  \param theShape Initial shape.
     *  \param theTolerance Maximum distance between faces, which can be considered as coincident.
     *  \return New GEOM_Object, containing a copy of theShape without coincident faces.

     *  Example: see GEOM_Spanner.py
    """
    anObj = ShapesOp.MakeGlueFaces(theShape, theTolerance)
    if ShapesOp.IsDone() == 0:
      print "MakeGlueFaces : ", ShapesOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Boolean (Common, Cut, Fuse, Section)
# -----------------------------------------------------------------------------

def MakeBoolean(theShape1, theShape2, theOperation):
    """
     *  Perform one of boolean operations on two given shapes.
     *  \param theShape1 First argument for boolean operation.
     *  \param theShape2 Second argument for boolean operation.
     *  \param theOperation Indicates the operation to be done:
     *                      1 - Common, 2 - Cut, 3 - Fuse, 4 - Section.
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BoolOp.MakeBoolean(theShape1, theShape2, theOperation)
    if BoolOp.IsDone() == 0:
      print "MakeBoolean : ", BoolOp.GetErrorCode()
    return anObj

def MakeCommon(s1, s2):
    """
     *  Shortcut to MakeBoolean(s1, s2, 1)

     *  Example: see GEOM_TestOthers.py
    """
    return MakeBoolean(s1, s2, 1)

def MakeCut(s1, s2):
    """
     *  Shortcut to MakeBoolean(s1, s2, 2)

     *  Example: see GEOM_TestOthers.py
    """
    return MakeBoolean(s1, s2, 2)

def MakeFuse(s1, s2):
    """
     *  Shortcut to MakeBoolean(s1, s2, 3)

     *  Example: see GEOM_TestOthers.py
    """
    return MakeBoolean(s1, s2, 3)

def MakeSection(s1, s2):
    """
     *  Shortcut to MakeBoolean(s1, s2, 4)

     *  Example: see GEOM_TestOthers.py
    """
    return MakeBoolean(s1, s2, 4)

def MakePartition(ListShapes, ListTools=[], ListKeepInside=[], ListRemoveInside=[],
                  Limit=ShapeType["SHAPE"], RemoveWebs=0, ListMaterials=[]):
    """
     *  Perform partition operation.
     *  \param ListShapes Shapes to be intersected.
     *  \param ListTools Shapes to intersect theShapes.
     *  \param ListKeepInside Shapes, outside which the results will be deleted.
     *         Each shape from theKeepInside must belong to theShapes also.
     *  \param ListRemoveInside Shapes, inside which the results will be deleted.
     *         Each shape from theRemoveInside must belong to theShapes also.
     *  \param Limit Type of resulting shapes (corresponding to TopAbs_ShapeEnum).
     *  \param RemoveWebs If TRUE, perform Glue 3D algorithm.
     *  \param ListMaterials Material indices for each shape. Make sence, only if theRemoveWebs is TRUE.
     *  \return New GEOM_Object, containing the result shapes.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BoolOp.MakePartition(ListShapes, ListTools,
                                 ListKeepInside, ListRemoveInside,
                                 Limit, RemoveWebs, ListMaterials);
    if BoolOp.IsDone() == 0:
      print "MakePartition : ", BoolOp.GetErrorCode()
    return anObj

def Partition(ListShapes, ListTools=[], ListKeepInside=[], ListRemoveInside=[],
              Limit=ShapeType["SHAPE"], RemoveWebs=0, ListMaterials=[]):
    """
     *  Shortcut to MakePartition()

     *  Example: see GEOM_TestOthers.py
    """
    anObj = MakePartition(ListShapes, ListTools,
                          ListKeepInside, ListRemoveInside,
                          Limit, RemoveWebs, ListMaterials);
    return anObj

def MakeHalfPartition(theShape, thePlane):
    """
     *  Perform partition of the Shape with the Plane
     *  \param theShape Shape to be intersected.
     *  \param thePlane Tool shape, to intersect theShape.
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = BoolOp.MakeHalfPartition(theShape, thePlane)
    if BoolOp.IsDone() == 0:
      print "MakeHalfPartition : ", BoolOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Transform objects
# -----------------------------------------------------------------------------

def MakeTranslationTwoPoints(theObject, thePoint1, thePoint2):
    """
     *  Translate the given object along the vector, specified
     *  by its end points, creating its copy before the translation.
     *  \param theObject The object to be translated.
     *  \param thePoint1 Start point of translation vector.
     *  \param thePoint2 End point of translation vector.
     *  \return New GEOM_Object, containing the translated object.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.TranslateTwoPointsCopy(theObject, thePoint1, thePoint2)
    if TrsfOp.IsDone() == 0:
      print "TranslateTwoPointsCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeTranslation(theObject, theDX, theDY, theDZ):
    """
     *  Translate the given object along the vector, specified
     *  by its components, creating its copy before the translation.
     *  \param theObject The object to be translated.
     *  \param theDX,theDY,theDZ Components of translation vector.
     *  \return New GEOM_Object, containing the translated object.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.TranslateDXDYDZCopy(theObject, theDX, theDY, theDZ)
    if TrsfOp.IsDone() == 0:
      print "TranslateDXDYDZCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeTranslationVector(theObject, theVector):
    """
     *  Translate the given object along the given vector,
     *  creating its copy before the translation.
     *  \param theObject The object to be translated.
     *  \param theVector The translation vector.
     *  \return New GEOM_Object, containing the translated object.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.TranslateVectorCopy(theObject, theVector)
    if TrsfOp.IsDone() == 0:
      print "TranslateVectorCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeRotation(theObject, theAxis, theAngle):
    """
     *  Rotate the given object around the given axis
     *  on the given angle, creating its copy before the rotatation.
     *  \param theObject The object to be rotated.
     *  \param theAxis Rotation axis.
     *  \param theAngle Rotation angle in radians.
     *  \return New GEOM_Object, containing the rotated object.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.RotateCopy(theObject, theAxis, theAngle)
    if TrsfOp.IsDone() == 0:
      print "RotateCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeScaleTransform(theObject, thePoint, theFactor):
    """
     *  Scale the given object by the factor, creating its copy before the scaling.
     *  \param theObject The object to be scaled.
     *  \param thePoint Center point for scaling.
     *  \param theFactor Scaling factor value.
     *  \return New GEOM_Object, containing the scaled shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.ScaleShapeCopy(theObject, thePoint, theFactor)
    if TrsfOp.IsDone() == 0:
      print "ScaleShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMirrorByPlane(theObject, thePlane):
    """
     *  Create an object, symmetrical
     *  to the given one relatively the given plane.
     *  \param theObject The object to be mirrored.
     *  \param thePlane Plane of symmetry.
     *  \return New GEOM_Object, containing the mirrored shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MirrorPlaneCopy(theObject, thePlane)
    if TrsfOp.IsDone() == 0:
      print "MirrorPlaneCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMirrorByAxis(theObject, theAxis):
    """
     *  Create an object, symmetrical
     *  to the given one relatively the given axis.
     *  \param theObject The object to be mirrored.
     *  \param theAxis Axis of symmetry.
     *  \return New GEOM_Object, containing the mirrored shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MirrorAxisCopy(theObject, theAxis)
    if TrsfOp.IsDone() == 0:
      print "MirrorAxisCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMirrorByPoint(theObject, thePoint):
    """
     *  Create an object, symmetrical
     *  to the given one relatively the given point.
     *  \param theObject The object to be mirrored.
     *  \param thePoint Point of symmetry.
     *  \return New GEOM_Object, containing the mirrored shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MirrorPointCopy(theObject, thePoint)
    if TrsfOp.IsDone() == 0:
      print "MirrorPointCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakePosition(theObject, theStartLCS, theEndLCS):
    """
     *  Modify the Location of the given object by LCS
     *  creating its copy before the setting

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.PositionShapeCopy(theObject, theStartLCS, theEndLCS)
    if TrsfOp.IsDone() == 0:
      print "PositionShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeOffset(theObject, theOffset):
    """
     *  Create new object as offset of the given one.
     *  \param theObject The base object for the offset.
     *  \param theOffset Offset value.
     *  \return New GEOM_Object, containing the offset object.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.OffsetShapeCopy(theObject, theOffset)
    if TrsfOp.IsDone() == 0:
      print "OffsetShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Patterns
# -----------------------------------------------------------------------------

def MakeMultiTranslation1D(theObject, theVector, theStep, theNbTimes):
    """
     *  Translate the given object along the given vector a given number times
     *  \param theObject The object to be translated.
     *  \param theVector Direction of the translation.
     *  \param theStep Distance to translate on.
     *  \param theNbTimes Quantity of translations to be done.
     *  \return New GEOM_Object, containing compound of all
     *          the shapes, obtained after each translation.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MultiTranslate1D(theObject, theVector, theStep, theNbTimes)
    if TrsfOp.IsDone() == 0:
      print "MultiTranslate1D : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMultiTranslation2D(theObject, theVector1, theStep1, theNbTimes1,
			              theVector2, theStep2, theNbTimes2):
    """
     *  Conseqently apply two specified translations to theObject specified number of times.
     *  \param theObject The object to be translated.
     *  \param theVector1 Direction of the first translation.
     *  \param theStep1 Step of the first translation.
     *  \param theNbTimes1 Quantity of translations to be done along theVector1.
     *  \param theVector2 Direction of the second translation.
     *  \param theStep2 Step of the second translation.
     *  \param theNbTimes2 Quantity of translations to be done along theVector2.
     *  \return New GEOM_Object, containing compound of all
     *          the shapes, obtained after each translation.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MultiTranslate2D(theObject, theVector1, theStep1, theNbTimes1,
			                       theVector2, theStep2, theNbTimes2)
    if TrsfOp.IsDone() == 0:
      print "MultiTranslate2D : ", TrsfOp.GetErrorCode()
    return anObj

def MultiRotate1D(theObject, theAxis, theNbTimes):
    """
     *  Rotate the given object around the given axis a given number times.
     *  Rotation angle will be 2*PI/theNbTimes.
     *  \param theObject The object to be rotated.
     *  \param theAxis The rotation axis.
     *  \param theNbTimes Quantity of rotations to be done.
     *  \return New GEOM_Object, containing compound of all the
     *          shapes, obtained after each rotation.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MultiRotate1D(theObject, theAxis, theNbTimes)
    if TrsfOp.IsDone() == 0:
      print "MultiRotate1D : ", TrsfOp.GetErrorCode()
    return anObj

def MultiRotate2D(theObject, theAxis, theAngle, theNbTimes1, theStep, theNbTimes2):
    """
     *  Rotate the given object around the
     *  given axis on the given angle a given number
     *  times and multi-translate each rotation result.
     *  Translation direction passes through center of gravity
     *  of rotated shape and its projection on the rotation axis.
     *  \param theObject The object to be rotated.
     *  \param theAxis Rotation axis.
     *  \param theAngle Rotation angle in graduces.
     *  \param theNbTimes1 Quantity of rotations to be done.
     *  \param theStep Translation distance.
     *  \param theNbTimes2 Quantity of translations to be done.
     *  \return New GEOM_Object, containing compound of all the
     *          shapes, obtained after each transformation.

     *  Example: see GEOM_TestAll.py
    """
    anObj = TrsfOp.MultiRotate2D(theObject, theAxis, theAngle, theNbTimes1, theStep, theNbTimes2)
    if TrsfOp.IsDone() == 0:
      print "MultiRotate2D : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMultiRotation1D(aShape,aDir,aPoint,aNbTimes):
    """
     *  The same, as MultiRotate1D(), but axis is given by direction and point

     *  Example: see GEOM_TestOthers.py
    """
    aVec = MakeLine(aPoint,aDir)
    anObj = MultiRotate1D(aShape,aVec,aNbTimes)
    return anObj

def MakeMultiRotation2D(aShape,aDir,aPoint,anAngle,nbtimes1,aStep,nbtimes2):
    """
     *  The same, as MultiRotate2D(), but axis is given by direction and point

     *  Example: see GEOM_TestOthers.py
    """
    aVec = MakeLine(aPoint,aDir)
    anObj = MultiRotate2D(aShape,aVec,anAngle,nbtimes1,aStep,nbtimes2)
    return anObj

# -----------------------------------------------------------------------------
# Local operations
# -----------------------------------------------------------------------------

def MakeFilletAll(theShape, theR):
    """
     *  Perform a fillet on all edges of the given shape.
     *  \param theShape Shape, to perform fillet on.
     *  \param theR Fillet radius.
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = LocalOp.MakeFilletAll(theShape, theR)
    if LocalOp.IsDone() == 0:
      print "MakeFilletAll : ", LocalOp.GetErrorCode()
    return anObj

def MakeFillet(theShape, theR, theShapeType, theListShapes):
    """
     *  Perform a fillet on the specified edges/faces of the given shape
     *  \param theShape Shape, to perform fillet on.
     *  \param theR Fillet radius.
     *  \param theShapeType Type of shapes in <theListShapes>.
     *  \param theListShapes Global indices of edges/faces to perform fillet on.
     *    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = None
    if theShapeType == ShapeType["EDGE"]:
        anObj = LocalOp.MakeFilletEdges(theShape, theR, theListShapes)
    else:
        anObj = LocalOp.MakeFilletFaces(theShape, theR, theListShapes)
    if LocalOp.IsDone() == 0:
      print "MakeFillet : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferAll(theShape, theD):
    """
     *  Perform a symmetric chamfer on all edges of the given shape.
     *  \param theShape Shape, to perform chamfer on.
     *  \param theD Chamfer size along each face.
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = LocalOp.MakeChamferAll(theShape, theD)
    if LocalOp.IsDone() == 0:
      print "MakeChamferAll : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferEdge(theShape, theD1, theD2, theFace1, theFace2):
    """
     *  Perform a chamfer on edges, common to the specified faces,
     *  with distance D1 on the Face1
     *  \param theShape Shape, to perform chamfer on.
     *  \param theD1 Chamfer size along \a theFace1.
     *  \param theD2 Chamfer size along \a theFace2.
     *  \param theFace1,theFace2 Global indices of two faces of \a theShape.
     *    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = LocalOp.MakeChamferEdge(theShape, theD1, theD2, theFace1, theFace2)
    if LocalOp.IsDone() == 0:
      print "MakeChamferEdge : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferFaces(theShape, theD1, theD2, theFaces):
    """
     *  Perform a chamfer on all edges of the specified faces,
     *  with distance D1 on the first specified face (if several for one edge)
     *  \param theShape Shape, to perform chamfer on.
     *  \param theD1 Chamfer size along face from \a theFaces. If both faces,
     *               connected to the edge, are in \a theFaces, \a theD1
     *               will be get along face, which is nearer to \a theFaces beginning.
     *  \param theD2 Chamfer size along another of two faces, connected to the edge.
     *  \param theFaces Sequence of global indices of faces of \a theShape.
     *    \note Global index of sub-shape can be obtained, using method geompy.GetSubShapeID().
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_TestAll.py
    """
    anObj = LocalOp.MakeChamferFaces(theShape, theD1, theD2, theFaces)
    if LocalOp.IsDone() == 0:
      print "MakeChamferFaces : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamfer(aShape,d1,d2,aShapeType,ListShape):
    """
     *  Shortcut to MakeChamferEdge() and MakeChamferFaces()

     *  Example: see GEOM_TestOthers.py
    """
    anObj = None
    if aShapeType == ShapeType["EDGE"]:
        anObj = MakeChamferEdge(aShape,d1,d2,ListShape[0],ListShape[1])
    else:
        anObj = MakeChamferFaces(aShape,d1,d2,ListShape)
    return anObj

def Archimede(theShape, theWeight, theWaterDensity, theMeshDeflection):
    """
     *  Perform an Archimde operation on the given shape with given parameters.
     *                    The object presenting the resulting face is returned
     *  \param theShape Shape to be put in water.
     *  \param theWeight Weight og the shape.
     *  \param theWaterDensity Density of the water.
     *  \param theMeshDeflection Deflection of the mesh, using to compute the section.
     *  \return New GEOM_Object, containing a section of \a theShape
     *          by a plane, corresponding to water level.

     *  Example: see GEOM_TestAll.py
    """
    anObj = LocalOp.MakeArchimede(theShape, theWeight, theWaterDensity, theMeshDeflection)
    if LocalOp.IsDone() == 0:
      print "MakeArchimede : ", LocalOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Information objects
# -----------------------------------------------------------------------------

def PointCoordinates(Point):
    """
     *  Get point coordinates
     *  \return [x, y, z]

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.PointCoordinates(Point)
    if MeasuOp.IsDone() == 0:
      print "PointCoordinates : ", MeasuOp.GetErrorCode()
    return aTuple

def BasicProperties(theShape):
    """
     *  Get summarized length of all wires,
     *  area of surface and volume of the given shape.
     *  \param theShape Shape to define properties of.
     *  \return [theLength, theSurfArea, theVolume]
     *  theLength:   Summarized length of all wires of the given shape.
     *  theSurfArea: Area of surface of the given shape.
     *  theVolume:   Volume of the given shape.

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.GetBasicProperties(theShape)
    if MeasuOp.IsDone() == 0:
      print "BasicProperties : ", MeasuOp.GetErrorCode()
    return aTuple

def BoundingBox(theShape):
    """
     *  Get parameters of bounding box of the given shape
     *  \param theShape Shape to obtain bounding box of.
     *  \return [Xmin,Xmax, Ymin,Ymax, Zmin,Zmax]
     *  Xmin,Xmax: Limits of shape along OX axis.
     *  Ymin,Ymax: Limits of shape along OY axis.
     *  Zmin,Zmax: Limits of shape along OZ axis.

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.GetBoundingBox(theShape)
    if MeasuOp.IsDone() == 0:
      print "BoundingBox : ", MeasuOp.GetErrorCode()
    return aTuple

def Inertia(theShape):
    """
     *  Get inertia matrix and moments of inertia of theShape.
     *  \param theShape Shape to calculate inertia of.
     *  \return [I11,I12,I13, I21,I22,I23, I31,I32,I33, Ix,Iy,Iz]
     *  I(1-3)(1-3): Components of the inertia matrix of the given shape.
     *  Ix,Iy,Iz:    Moments of inertia of the given shape.

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.GetInertia(theShape)
    if MeasuOp.IsDone() == 0:
      print "Inertia : ", MeasuOp.GetErrorCode()
    return aTuple

def MinDistance(theShape1, theShape2):
    """
     *  Get minimal distance between the given shapes.
     *  \param theShape1,theShape2 Shapes to find minimal distance between.
     *  \return Value of the minimal distance between the given shapes.

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.GetMinDistance(theShape1, theShape2)
    if MeasuOp.IsDone() == 0:
      print "MinDistance : ", MeasuOp.GetErrorCode()
    return aTuple[0]

def Tolerance(theShape):
    """
     *  Get min and max tolerances of sub-shapes of theShape
     *  \param theShape Shape, to get tolerances of.
     *  \return [FaceMin,FaceMax, EdgeMin,EdgeMax, VertMin,VertMax]
     *  FaceMin,FaceMax: Min and max tolerances of the faces.
     *  EdgeMin,EdgeMax: Min and max tolerances of the edges.
     *  VertMin,VertMax: Min and max tolerances of the vertices.

     *  Example: see GEOM_TestMeasures.py
    """
    aTuple = MeasuOp.GetTolerance(theShape)
    if MeasuOp.IsDone() == 0:
      print "Tolerance : ", MeasuOp.GetErrorCode()
    return aTuple

def WhatIs(theShape):
    """
     *  Obtain description of the given shape (number of sub-shapes of each type)
     *  \param theShape Shape to be described.
     *  \return Description of the given shape.

     *  Example: see GEOM_TestMeasures.py
    """
    aDescr = MeasuOp.WhatIs(theShape)
    if MeasuOp.IsDone() == 0:
      print "WhatIs : ", MeasuOp.GetErrorCode()
    return aDescr

def MakeCDG(theShape):
    """
     *  Get a point, situated at the centre of mass of theShape.
     *  \param theShape Shape to define centre of mass of.
     *  \return New GEOM_Object, containing the created point.

     *  Example: see GEOM_TestMeasures.py
    """
    anObj = MeasuOp.GetCentreOfMass(theShape)
    if MeasuOp.IsDone() == 0:
      print "GetCentreOfMass : ", MeasuOp.GetErrorCode()
    return anObj

def CheckShape(theShape):
    """
     *  Check a topology of the given shape.
     *  \param theShape Shape to check validity of.
     *  \return TRUE, if the shape "seems to be valid" from the topological point of view.
     *  If theShape is invalid, prints a description of problem.

     *  Example: see GEOM_TestMeasures.py
    """
    (IsValid, Status) = MeasuOp.CheckShape(theShape)
    if MeasuOp.IsDone() == 0:
      print "CheckShape : ", MeasuOp.GetErrorCode()
    else:
      if IsValid == 0:
        print Status
    return IsValid

# -----------------------------------------------------------------------------
# Import/Export objects
# -----------------------------------------------------------------------------

def Import(theFileName, theFormatName):
    """
     *  Import a shape from the BREP or IGES or STEP file
     *  (depends on given format) with given name.
     *  \param theFileName The file, containing the shape.
     *  \param theFormatName Specify format for the file reading.
     *         Available formats can be obtained with InsertOp.ImportTranslators() method.
     *  \return New GEOM_Object, containing the imported shape.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = InsertOp.Import(theFileName, theFormatName)
    if InsertOp.IsDone() == 0:
      print "Import : ", InsertOp.GetErrorCode()
    return anObj

def ImportBREP(theFileName):
    """
     *  Shortcut to Import() for BREP format

     *  Example: see GEOM_TestOthers.py
    """
    return Import(theFileName, "BREP")

def ImportIGES(theFileName):
    """
     *  Shortcut to Import() for IGES format

     *  Example: see GEOM_TestOthers.py
    """
    return Import(theFileName, "IGES")

def ImportSTEP(theFileName):
    """
     *  Shortcut to Import() for STEP format

     *  Example: see GEOM_TestOthers.py
    """
    return Import(theFileName, "STEP")

def Export(theObject, theFileName, theFormatName):
    """
     *  Export the given shape into a file with given name.
     *  \param theObject Shape to be stored in the file.
     *  \param theFileName Name of the file to store the given shape in.
     *  \param theFormatName Specify format for the shape storage.
     *         Available formats can be obtained with InsertOp.ImportTranslators() method.

     *  Example: see GEOM_TestOthers.py
    """
    InsertOp.Export(theObject, theFileName, theFormatName)
    if InsertOp.IsDone() == 0:
      print "Export : ", InsertOp.GetErrorCode()

def ExportBREP(theObject, theFileName):
    """
     *  Shortcut to Export() for BREP format

     *  Example: see GEOM_TestOthers.py
    """
    return Export(theObject, theFileName, "BREP")

def ExportIGES(theObject, theFileName):
    """
     *  Shortcut to Export() for IGES format

     *  Example: see GEOM_TestOthers.py
    """
    return Export(theObject, theFileName, "IGES")

def ExportSTEP(theObject, theFileName):
    """
     *  Shortcut to Export() for STEP format

     *  Example: see GEOM_TestOthers.py
    """
    return Export(theObject, theFileName, "STEP")

# -----------------------------------------------------------------------------
# Block operations
# -----------------------------------------------------------------------------

def MakeQuad(E1, E2, E3, E4):
    """
     *  Create a quadrangle face from four edges. Order of Edges is not
     *  important. It is  not necessary that edges share the same vertex.
     *  \param E1,E2,E3,E4 Edges for the face bound.
     *  \return New GEOM_Object, containing the created face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeQuad(E1, E2, E3, E4)
    if BlocksOp.IsDone() == 0:
      print "MakeQuad : ", BlocksOp.GetErrorCode()
    return anObj

def MakeQuad2Edges(E1, E2):
    """
     *  Create a quadrangle face on two edges.
     *  The missing edges will be built by creating the shortest ones.
     *  \param E1,E2 Two opposite edges for the face.
     *  \return New GEOM_Object, containing the created face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeQuad2Edges(E1, E2)
    if BlocksOp.IsDone() == 0:
      print "MakeQuad2Edges : ", BlocksOp.GetErrorCode()
    return anObj

def MakeQuad4Vertices(V1, V2, V3, V4):
    """
     *  Create a quadrangle face with specified corners.
     *  The missing edges will be built by creating the shortest ones.
     *  \param V1,V2,V3,V4 Corner vertices for the face.
     *  \return New GEOM_Object, containing the created face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeQuad4Vertices(V1, V2, V3, V4)
    if BlocksOp.IsDone() == 0:
      print "MakeQuad4Vertices : ", BlocksOp.GetErrorCode()
    return anObj

def MakeHexa(F1, F2, F3, F4, F5, F6):
    """
     *  Create a hexahedral solid, bounded by the six given faces. Order of
     *  faces is not important. It is  not necessary that Faces share the same edge.
     *  \param F1,F2,F3,F4,F5,F6 Faces for the hexahedral solid.
     *  \return New GEOM_Object, containing the created solid.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeHexa(F1, F2, F3, F4, F5, F6)
    if BlocksOp.IsDone() == 0:
      print "MakeHexa : ", BlocksOp.GetErrorCode()
    return anObj

def MakeHexa2Faces(F1, F2):
    """
     *  Create a hexahedral solid between two given faces.
     *  The missing faces will be built by creating the smallest ones.
     *  \param F1,F2 Two opposite faces for the hexahedral solid.
     *  \return New GEOM_Object, containing the created solid.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeHexa2Faces(F1, F2)
    if BlocksOp.IsDone() == 0:
      print "MakeHexa2Faces : ", BlocksOp.GetErrorCode()
    return anObj

def GetPoint(theShape, theX, theY, theZ, theEpsilon):
    """
     *  Get a vertex, found in the given shape by its coordinates.
     *  \param theShape Block or a compound of blocks.
     *  \param theX,theY,theZ Coordinates of the sought vertex.
     *  \param theEpsilon Maximum allowed distance between the resulting
     *                    vertex and point with the given coordinates.
     *  \return New GEOM_Object, containing the found vertex.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = BlocksOp.GetPoint(theShape, theX, theY, theZ, theEpsilon)
    if BlocksOp.IsDone() == 0:
      print "GetPoint : ", BlocksOp.GetErrorCode()
    return anObj

def GetEdge(theShape, thePoint1, thePoint2):
    """
     *  Get an edge, found in the given shape by two given vertices.
     *  \param theShape Block or a compound of blocks.
     *  \param thePoint1,thePoint2 Points, close to the ends of the desired edge.
     *  \return New GEOM_Object, containing the found edge.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetEdge(theShape, thePoint1, thePoint2)
    if BlocksOp.IsDone() == 0:
      print "GetEdge : ", BlocksOp.GetErrorCode()
    return anObj

def GetEdgeNearPoint(theShape, thePoint):
    """
     *  Find an edge of the given shape, which has minimal distance to the given point.
     *  \param theShape Block or a compound of blocks.
     *  \param thePoint Point, close to the desired edge.
     *  \return New GEOM_Object, containing the found edge.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = BlocksOp.GetEdgeNearPoint(theShape, thePoint)
    if BlocksOp.IsDone() == 0:
      print "GetEdgeNearPoint : ", BlocksOp.GetErrorCode()
    return anObj

def GetFaceByPoints(theShape, thePoint1, thePoint2, thePoint3, thePoint4):
    """
     *  Returns a face, found in the given shape by four given corner vertices.
     *  \param theShape Block or a compound of blocks.
     *  \param thePoint1-thePoint4 Points, close to the corners of the desired face.
     *  \return New GEOM_Object, containing the found face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetFaceByPoints(theShape, thePoint1, thePoint2, thePoint3, thePoint4)
    if BlocksOp.IsDone() == 0:
      print "GetFaceByPoints : ", BlocksOp.GetErrorCode()
    return anObj

def GetFaceByEdges(theShape, theEdge1, theEdge2):
    """
     *  Get a face of block, found in the given shape by two given edges.
     *  \param theShape Block or a compound of blocks.
     *  \param theEdge1,theEdge2 Edges, close to the edges of the desired face.
     *  \return New GEOM_Object, containing the found face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetFaceByEdges(theShape, theEdge1, theEdge2)
    if BlocksOp.IsDone() == 0:
      print "GetFaceByEdges : ", BlocksOp.GetErrorCode()
    return anObj

def GetOppositeFace(theBlock, theFace):
    """
     *  Find a face, opposite to the given one in the given block.
     *  \param theBlock Must be a hexahedral solid.
     *  \param theFace Face of \a theBlock, opposite to the desired face.
     *  \return New GEOM_Object, containing the found face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetOppositeFace(theBlock, theFace)
    if BlocksOp.IsDone() == 0:
      print "GetOppositeFace : ", BlocksOp.GetErrorCode()
    return anObj

def GetFaceNearPoint(theShape, thePoint):
    """
     *  Find a face of the given shape, which has minimal distance to the given point.
     *  \param theShape Block or a compound of blocks.
     *  \param thePoint Point, close to the desired face.
     *  \return New GEOM_Object, containing the found face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetFaceNearPoint(theShape, thePoint)
    if BlocksOp.IsDone() == 0:
      print "GetFaceNearPoint : ", BlocksOp.GetErrorCode()
    return anObj

def GetFaceByNormale(theBlock, theVector):
    """
     *  Find a face of block, whose outside normale has minimal angle with the given vector.
     *  \param theShape Block or a compound of blocks.
     *  \param theVector Vector, close to the normale of the desired face.
     *  \return New GEOM_Object, containing the found face.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetFaceByNormale(theBlock, theVector)
    if BlocksOp.IsDone() == 0:
      print "GetFaceByNormale : ", BlocksOp.GetErrorCode()
    return anObj

def CheckCompoundOfBlocks(theCompound):
    """
     *  Check, if the compound of blocks is given.
     *  To be considered as a compound of blocks, the
     *  given shape must satisfy the following conditions:
     *  - Each element of the compound should be a Block (6 faces and 12 edges).
     *  - A connection between two Blocks should be an entire quadrangle face or an entire edge.
     *  - The compound should be connexe.
     *  - The glue between two quadrangle faces should be applied.
     *  \param theCompound The compound to check.
     *  \return TRUE, if the given shape is a compound of blocks.
     *  If theCompound is not valid, prints all discovered errors.

     *  Example: see GEOM_Spanner.py
    """
    (IsValid, BCErrors) = BlocksOp.CheckCompoundOfBlocks(theCompound)
    if BlocksOp.IsDone() == 0:
      print "CheckCompoundOfBlocks : ", BlocksOp.GetErrorCode()
    else:
      if IsValid == 0:
        Descr = BlocksOp.PrintBCErrors(theCompound, BCErrors)
        print Descr
    return IsValid

def RemoveExtraEdges(theShape):
    """
     *  Remove all seam and degenerated edges from \a theShape.
     *  Unite faces and edges, sharing one surface.
     *  \param theShape The compound or single solid to remove irregular edges from.
     *  \return Improved shape.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = BlocksOp.RemoveExtraEdges(theShape)
    if BlocksOp.IsDone() == 0:
      print "RemoveExtraEdges : ", BlocksOp.GetErrorCode()
    return anObj

def CheckAndImprove(theShape):
    """
     *  Check, if the given shape is a blocks compound.
     *  Fix all detected errors.
     *    \note Single block can be also fixed by this method.
     *  \param theCompound The compound to check and improve.
     *  \return Improved compound.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = BlocksOp.CheckAndImprove(theShape)
    if BlocksOp.IsDone() == 0:
      print "CheckAndImprove : ", BlocksOp.GetErrorCode()
    return anObj

def MakeBlockExplode(theCompound, theMinNbFaces, theMaxNbFaces):
    """
     *  Get all the blocks, contained in the given compound.
     *  \param theCompound The compound to explode.
     *  \param theMinNbFaces If solid has lower number of faces, it is not a block.
     *  \param theMaxNbFaces If solid has higher number of faces, it is not a block.
     *    \note If theMaxNbFaces = 0, the maximum number of faces is not restricted.
     *  \return List of GEOM_Objects, containing the retrieved blocks.

     *  Example: see GEOM_TestOthers.py
    """
    aList = BlocksOp.ExplodeCompoundOfBlocks(theCompound, theMinNbFaces, theMaxNbFaces)
    if BlocksOp.IsDone() == 0:
      print "MakeBlockExplode : ", BlocksOp.GetErrorCode()
    return aList

def GetBlockNearPoint(theCompound, thePoint):
    """
     *  Find block, containing the given point inside its volume or on boundary.
     *  \param theCompound Compound, to find block in.
     *  \param thePoint Point, close to the desired block. If the point lays on
     *         boundary between some blocks, we return block with nearest center.
     *  \return New GEOM_Object, containing the found block.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.GetBlockNearPoint(theCompound, thePoint)
    if BlocksOp.IsDone() == 0:
      print "GetBlockNearPoint : ", BlocksOp.GetErrorCode()
    return anObj

def GetBlockByParts(theCompound, theParts):
    """
     *  Find block, containing all the elements, passed as the parts, or maximum quantity of them.
     *  \param theCompound Compound, to find block in.
     *  \param theParts List of faces and/or edges and/or vertices to be parts of the found block.
     *  \return New GEOM_Object, containing the found block.

     *  Example: see GEOM_TestOthers.py
    """
    anObj = BlocksOp.GetBlockByParts(theCompound, theParts)
    if BlocksOp.IsDone() == 0:
      print "GetBlockByParts : ", BlocksOp.GetErrorCode()
    return anObj

def GetBlocksByParts(theCompound, theParts):
    """
     *  Return all blocks, containing all the elements, passed as the parts.
     *  \param theCompound Compound, to find blocks in.
     *  \param theParts List of faces and/or edges and/or vertices to be parts of the found blocks.
     *  \return List of GEOM_Objects, containing the found blocks.

     *  Example: see GEOM_Spanner.py
    """
    aList = BlocksOp.GetBlocksByParts(theCompound, theParts)
    if BlocksOp.IsDone() == 0:
      print "GetBlocksByParts : ", BlocksOp.GetErrorCode()
    return aList

def MakeMultiTransformation1D(Block, DirFace1, DirFace2, NbTimes):
    """
     *  Multi-transformate block and glue the result.
     *  Transformation is defined so, as to superpose direction faces.
     *  \param Block Hexahedral solid to be multi-transformed.
     *  \param DirFace1 ID of First direction face.
     *  \param DirFace2 ID of Second direction face.
     *  \param NbTimes Quantity of transformations to be done.
     *    \note Unique ID of sub-shape can be obtained, using method GetSubShapeID().
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeMultiTransformation1D(Block, DirFace1, DirFace2, NbTimes)
    if BlocksOp.IsDone() == 0:
      print "MakeMultiTransformation1D : ", BlocksOp.GetErrorCode()
    return anObj

def MakeMultiTransformation2D(Block, DirFace1U, DirFace2U, NbTimesU,
			             DirFace1V, DirFace2V, NbTimesV):
    """
     *  Multi-transformate block and glue the result.
     *  \param Block Hexahedral solid to be multi-transformed.
     *  \param DirFace1U,DirFace2U IDs of Direction faces for the first transformation.
     *  \param DirFace1V,DirFace2V IDs of Direction faces for the second transformation.
     *  \param NbTimesU,NbTimesV Quantity of transformations to be done.
     *  \return New GEOM_Object, containing the result shape.

     *  Example: see GEOM_Spanner.py
    """
    anObj = BlocksOp.MakeMultiTransformation2D(Block, DirFace1U, DirFace2U, NbTimesU,
					              DirFace1V, DirFace2V, NbTimesV)
    if BlocksOp.IsDone() == 0:
      print "MakeMultiTransformation2D : ", BlocksOp.GetErrorCode()
    return anObj

def Propagate(theShape):
    """
     *  Build all possible propagation groups.
     *  Propagation group is a set of all edges, opposite to one (main)
     *  edge of this group directly or through other opposite edges.
     *  Notion of Opposite Edge make sence only on quadrangle face.
     *  \param theShape Shape to build propagation groups on.
     *  \return List of GEOM_Objects, each of them is a propagation group.

     *  Example: see GEOM_TestOthers.py
    """
    listChains = BlocksOp.Propagate(theShape)
    if BlocksOp.IsDone() == 0:
      print "Propagate : ", BlocksOp.GetErrorCode()
    return listChains

# -----------------------------------------------------------------------------
# Group operations
# -----------------------------------------------------------------------------

def CreateGroup(theMainShape, theShapeType):
    """
     *  Creates a new group which will store sub shapes of theMainShape
     *  \param theMainShape is a GEOM object on which the group is selected
     *  \param theShapeType defines a shape type of the group
     *  \return a newly created GEOM group

     *  Example: see GEOM_TestOthers.py
    """
    anObj = GroupOp.CreateGroup(theMainShape, theShapeType)
    if GroupOp.IsDone() == 0:
       print "CreateGroup : ", GroupOp.GetErrorCode()
    return anObj

def AddObject(theGroup, theSubShapeID):
    """
     *  Adds a sub object with ID theSubShapeId to the group
     *  \param theGroup is a GEOM group to which the new sub shape is added
     *  \param theSubShapeID is a sub shape ID in the main object.
     *  \note Use method GetSubShapeID() to get an unique ID of the sub shape

     *  Example: see GEOM_TestOthers.py
    """
    GroupOp.AddObject(theGroup, theSubShapeID)
    if GroupOp.IsDone() == 0:
      print "AddObject : ", GroupOp.GetErrorCode()

def RemoveObject(theGroup, theSubShapeID):
    """
     *  Removes a sub object with ID \a theSubShapeId from the group
     *  \param theGroup is a GEOM group from which the new sub shape is removed
     *  \param theSubShapeID is a sub shape ID in the main object.
     *  \note Use method GetSubShapeID() to get an unique ID of the sub shape

     *  Example: see GEOM_TestOthers.py
    """
    GroupOp.RemoveObject(theGroup, theSubShapeID)
    if GroupOp.IsDone() == 0:
      print "RemoveObject : ", GroupOp.GetErrorCode()

def UnionList (theGroup, theSubShapes):
    """
     *  Adds to the group all the given shapes. No errors, if some shapes are alredy included.
     *  \param theGroup is a GEOM group to which the new sub shapes are added.
     *  \param theSubShapes is a list of sub shapes to be added.

     *  Example: see GEOM_TestOthers.py
    """
    GroupOp.UnionList(theGroup, theSubShapes)
    if GroupOp.IsDone() == 0:
      print "UnionList : ", GroupOp.GetErrorCode()

def DifferenceList (theGroup, theSubShapes):
    """
     *  Removes from the group all the given shapes. No errors, if some shapes are not included.
     *  \param theGroup is a GEOM group from which the sub-shapes are removed.
     *  \param theSubShapes is a list of sub-shapes to be removed.

     *  Example: see GEOM_TestOthers.py
    """
    GroupOp.DifferenceList(theGroup, theSubShapes)
    if GroupOp.IsDone() == 0:
      print "DifferenceList : ", GroupOp.GetErrorCode()

def GetObjectIDs(theGroup):
    """
     *  Returns a list of sub objects ID stored in the group
     *  \param theGroup is a GEOM group for which a list of IDs is requested

     *  Example: see GEOM_TestOthers.py
    """
    ListIDs = GroupOp.GetObjects(theGroup)
    if GroupOp.IsDone() == 0:
      print "GetObjectIDs : ", GroupOp.GetErrorCode()
    return ListIDs

def GetType(theGroup):
    """
     *  Returns a type of sub objects stored in the group
     *  \param theGroup is a GEOM group which type is returned.

     *  Example: see GEOM_TestOthers.py
    """
    aType = GroupOp.GetType(theGroup)
    if GroupOp.IsDone() == 0:
      print "GetType : ", GroupOp.GetErrorCode()
    return aType

def GetMainShape(theGroup):
    """
     *  Returns a main shape associated with the group
     *  \param theGroup is a GEOM group for which a main shape object is requested
     *  \return a GEOM object which is a main shape for theGroup

     *  Example: see GEOM_TestOthers.py
    """
    anObj = GroupOp.GetMainShape(theGroup)
    if GroupOp.IsDone() == 0:
      print "GetMainShape : ", GroupOp.GetErrorCode()
    return anObj

def addPath(Path):
    """
     * Add Path to load python scripts from
    """
    if (sys.path.count(Path) < 1):
	sys.path.append(Path)
