from batchmode_salome import *
import GEOM
import math

geom = lcc.FindOrLoadComponent("FactoryServer", "GEOM")
myBuilder = myStudy.NewBuilder()

id = myStudy._get_StudyId()

#######################################################
### Basic Operations
#######################################################
###
BasicOp = geom.GetIBasicOperations(id)
print "\n --- Basic Operations --- \n"

#######################################################
point1 = BasicOp.MakePointXYZ(0, 0, 0)
if BasicOp.IsDone() == 0:
    print "MakePointXYZ(0, 0, 0) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point1 is None:
        print "MakePointXYZ(0, 0, 0) returned null : ", BasicOp.GetErrorCode()
    else:
        point1.SetName("Point1: MakePointXYZ(0, 0, 0)")
        print point1.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
point2 = BasicOp.MakePointWithReference(point1, 40, 40, 52)
if BasicOp.IsDone() == 0:
    print "MakePointWithReference(point1, 40, 40, 52) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point2 is None:
        print "MakePointWithReference(point1, 40, 40, 52) returned null : ", BasicOp.GetErrorCode()
    else:
        point2.SetName("Point2: MakePointWithReference(point1, 40, 40, 52)")
        print point2.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
point3 = BasicOp.MakePointWithReference(point2, -20, -20, 0)
if BasicOp.IsDone() == 0:
    print "MakePointWithReference(point2, -20, -20, 0) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point3 is None:
        print "MakePointWithReference(point2, -20, -20, 0) returned null : ", BasicOp.GetErrorCode()
    else:
        point3.SetName("Point3: MakePointWithReference(point2, -20, -20, 0)")
        print point3.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
point5 = BasicOp.MakePointWithReference(point3, 40, 0, -3.5)
if BasicOp.IsDone() == 0:
    print "MakePointWithReference(point3, 40, 0, -3.5) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point5 is None:
        print "MakePointWithReference(point3, 40, 0, -3.5) returned null : ", BasicOp.GetErrorCode()
    else:
        point5.SetName("Point5: MakePointWithReference(point3, 40, 0, -3.5)")
        print point5.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
point6 = BasicOp.MakePointWithReference(point5, 0, 0, -9)
if BasicOp.IsDone() == 0:
    print "MakePointWithReference(point5, 0, 0, -9) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point6 is None:
        print "MakePointWithReference(point5, 0, 0, -9) returned null : ", BasicOp.GetErrorCode()
    else:
        point6.SetName("Point6: MakePointWithReference(point5, 0, 0, -9)")
        print point6.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
point7 = BasicOp.MakePointWithReference(point2, 0, 0, -52)
if BasicOp.IsDone() == 0:
    print "MakePointWithReference(point2, 0, 0, -52) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if point7 is None:
        print "MakePointWithReference(point2, 0, 0, -52) returned null : ", BasicOp.GetErrorCode()
    else:
        point7.SetName("Point7: MakePointWithReference(point2, 0, 0, -52)")
        print point7.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
vector1 = BasicOp.MakeVectorDXDYDZ(0,0,1)
if BasicOp.IsDone() == 0:
    print "MakeVectorDXDYDZ(0,0,1) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if vector1 is None:
        print "MakeVectorDXDYDZ(0,0,1) returned null : ", BasicOp.GetErrorCode()
    else:
        vector1.SetName("Vector1: MakeVectorDXDYDZ(0,0,1)")
        print vector1.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
vector2 = BasicOp.MakeVectorTwoPnt(point2, point3)
if BasicOp.IsDone() == 0:
    print "MakeVectorTwoPnt(point2, point3) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if vector2 is None:
        print "MakeVectorTwoPnt(point2, point3) returned null : ", BasicOp.GetErrorCode()
    else:
        vector2.SetName("Vector2: MakeVectorTwoPnt(point2, point3)")
        print vector2.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
vector3 = BasicOp.MakeVectorTwoPnt(point2, point6)
if BasicOp.IsDone() == 0:
    print "MakeVectorTwoPnt(point2, point6) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if vector3 is None:
        print "MakeVectorTwoPnt(point2, point6) returned null : ", BasicOp.GetErrorCode()
    else:
        vector3.SetName("Vector3: MakeVectorTwoPnt(point2, point6)")
        print vector3.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
line1 = BasicOp.MakeLine(point7, point3)
if BasicOp.IsDone() == 0:
    print "MakeLine(point7, point3) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if line1 is None:
        print "MakeLine(point7, point3) returned null : ", BasicOp.GetErrorCode()
    else:
        line1.SetName("Line1: MakeLine(point7, point3)")
        print line1.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
plane1 = BasicOp.MakePlanePntVec(point3, vector1, 20.0 * math.sqrt(2.0))
if BasicOp.IsDone() == 0:
    print "MakePlanePntVec(point3, vector1, 20.0 * math.sqrt(2.0)) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if plane1 is None:
        print "MakePlanePntVec(point3, vector1, 20.0 * math.sqrt(2.0)) returned null : ", BasicOp.GetErrorCode()
    else:
        plane1.SetName("Plane1: MakePlanePntVec(point3, vector1, 20.0 * math.sqrt(2.0))")
        print plane1.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
plane2 = BasicOp.MakePlaneThreePnt(point1, point2, point3, 50)
if BasicOp.IsDone() == 0:
    print "MakePlaneThreePnt(point1, point2, point3, 50) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if plane2 is None:
        print "MakePlaneThreePnt(point1, point2, point3, 50) returned null : ", BasicOp.GetErrorCode()
    else:
        plane2.SetName("Plane2: MakePlaneThreePnt(point1, point2, point3, 50)")
        print plane2.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
plane3 = BasicOp.MakePlaneFace(plane2, 70)
if BasicOp.IsDone() == 0:
    print "MakePlaneFace(plane2, 70) NOT DONE : ", BasicOp.GetErrorCode()
else:
    if plane3 is None:
        print "MakePlaneFace(plane2, 70) returned null : ", BasicOp.GetErrorCode()
    else:
        plane3.SetName("Plane3: MakePlaneFace(plane2, 70)")
        print plane3.GetName(), " : ", BasicOp.GetErrorCode()

#######################################################
### Insert Operations
#######################################################
###
#InsertOp = geom.GetIInsertOperations(id)
#print "\n --- Insert Operations --- \n"
#
#######################################################
#obj3 = InsertOp.MakeCopy(obj2)

#######################################################
### Curves Creation
#######################################################
###
CurvesOp = geom.GetICurvesOperations(id)
print "\n --- Curves Creation --- \n"

#######################################################
circle1 = CurvesOp.MakeCirclePntVecR(point2, vector2, 13)
if CurvesOp.IsDone() == 0:
    print "MakeCirclePntVecR(point2, vector2, 13) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if circle1 is None:
        print "MakeCirclePntVecR(point2, vector2, 13) returned null : ", CurvesOp.GetErrorCode()
    else:
        circle1.SetName("Circle1: MakeCirclePntVecR(point2, vector2, 13)")
        print circle1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
circle2 = CurvesOp.MakeCircleThreePnt(point1, point2, point3)
if CurvesOp.IsDone() == 0:
    print "MakeCircleThreePnt(point1, point2, point3) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if circle2 is None:
        print "MakeCircleThreePnt(point1, point2, point3) returned null : ", CurvesOp.GetErrorCode()
    else:
        circle2.SetName("Circle2: MakeCircleThreePnt(point1, point2, point3)")
        print circle2.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
ellipse1 = CurvesOp.MakeEllipse(point2, vector2, 21, 13)
if CurvesOp.IsDone() == 0:
    print "MakeEllipse(point2, vector2, 21, 13) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if ellipse1 is None:
        print "MakeEllipse(point2, vector2, 21, 13) returned null : ", CurvesOp.GetErrorCode()
    else:
        ellipse1.SetName("Ellipse1: MakeEllipse(point2, vector2, 21, 13)")
        print ellipse1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
arc1 = CurvesOp.MakeArc(point1, point2, point3)
if CurvesOp.IsDone() == 0:
    print "MakeArc(point1, point2, point3) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if arc1 is None:
        print "MakeArc(point1, point2, point3) returned null : ", CurvesOp.GetErrorCode()
    else:
        arc1.SetName("Arc1: MakeArc(point1, point2, point3)")
        print arc1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
poly1 = CurvesOp.MakePolyline([point1, point3, point2, point7])
if CurvesOp.IsDone() == 0:
    print "MakePolyline([point1, point3, point2, point7]) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if poly1 is None:
        print "MakePolyline([point1, point3, point2, point7]) returned null : ", CurvesOp.GetErrorCode()
    else:
        poly1.SetName("Polyline1: MakePolyline([point1, point3, point2, point7])")
        print poly1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
bezier1 = CurvesOp.MakeSplineBezier([point2, point3, point7, point1])
if CurvesOp.IsDone() == 0:
    print "MakeSplineBezier([point2, point3, point7, point1]) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if bezier1 is None:
        print "MakeSplineBezier([point2, point3, point7, point1]) returned null : ", CurvesOp.GetErrorCode()
    else:
        bezier1.SetName("Bezier1: MakeSplineBezier([point2, point3, point7, point1])")
        print bezier1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
bspline1 = CurvesOp.MakeSplineInterpolation([point2, point7, point1, point3])
if CurvesOp.IsDone() == 0:
    print "MakeSplineInterpolation([point2, point7, point1, point3]) NOT DONE : ", CurvesOp.GetErrorCode()
else:
    if bspline1 is None:
        print "MakeSplineInterpolation([point2, point7, point1, point3]) returned null : ", CurvesOp.GetErrorCode()
    else:
        bspline1.SetName("B-Spline1: MakeSplineInterpolation([point2, point7, point1, point3])")
        print bspline1.GetName(), " : ", CurvesOp.GetErrorCode()

#######################################################
### 3D Primitives Creation
#######################################################
###
PrimOp = geom.GetI3DPrimOperations(id)
print "\n --- 3D Primitives Creation --- \n"

#######################################################
box1 = PrimOp.MakeBoxTwoPnt(point1, point2)
if PrimOp.IsDone() == 0:
    print "MakeBoxTwoPnt(point1, point2) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if box1 is None:
        print "MakeBoxTwoPnt(point1, point2) returned null : ", PrimOp.GetErrorCode()
    else:
        box1.SetName("Box1: MakeBoxTwoPnt(point1, point2)")
        print box1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
box2 = PrimOp.MakeBoxDXDYDZ(10, 20, -30)
if PrimOp.IsDone() == 0:
    print "MakeBoxDXDYDZ(10, 20, -30) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if box2 is None:
        print "MakeBoxDXDYDZ(10, 20, -30) returned null : ", PrimOp.GetErrorCode()
    else:
        box2.SetName("Box2: MakeBoxDXDYDZ(10, 20, -30)")
        print box2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
sphere1 = PrimOp.MakeSphereR(7)
if PrimOp.IsDone() == 0:
    print "MakeSphereR(7) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if sphere1 is None:
        print "MakeSphereR(7) returned null : ", PrimOp.GetErrorCode()
    else:
        sphere1.SetName("Sphere1: MakeSphereR(7)")
        print sphere1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
sphere2 = PrimOp.MakeSpherePntR(point5, 3.5)
if PrimOp.IsDone() == 0:
    print "MakeSpherePntR(point5, 3.5) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if sphere2 is None:
        print "MakeSpherePntR(point5, 3.5) returned null : ", PrimOp.GetErrorCode()
    else:
        sphere2.SetName("Sphere2: MakeSpherePntR(point5, 3.5)")
        print sphere2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
cylinder1 = PrimOp.MakeCylinderRH(3, 33)
if PrimOp.IsDone() == 0:
    print "MakeCylinderRH(3, 33) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if cylinder1 is None:
        print "MakeCylinderRH(3, 33) returned null : ", PrimOp.GetErrorCode()
    else:
        cylinder1.SetName("Cylinder1: MakeCylinderRH(3, 33)")
        print cylinder1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
cylinder2 = PrimOp.MakeCylinderPntVecRH(point3, vector1, 20, 13)
if PrimOp.IsDone() == 0:
    print "MakeCylinderPntVecRH(point3, vector1, 20, 13) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if cylinder2 is None:
        print "MakeCylinderPntVecRH(point3, vector1, 20, 13) returned null : ", PrimOp.GetErrorCode()
    else:
        cylinder2.SetName("Cylinder2: MakeCylinderPntVecRH(point3, vector1, 20, 13)")
        print cylinder2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
cone1 = PrimOp.MakeConeR1R2H(10, 3, 2)
if PrimOp.IsDone() == 0:
    print "MakeConeR1R2H(10, 3, 2) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if cone1 is None:
        print "MakeConeR1R2H(10, 3, 2) returned null : ", PrimOp.GetErrorCode()
    else:
        cone1.SetName("Cone1: MakeConeR1R2H(10, 3, 2)")
        print cone1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
cone2 = PrimOp.MakeConePntVecR1R2H(point6, vector1, 3.5, 0, 9)
if PrimOp.IsDone() == 0:
    print "MakeConePntVecR1R2H(point6, vector1, 3.5, 0, 9) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if cone2 is None:
        print "MakeConePntVecR1R2H(point6, vector1, 3.5, 0, 9) returned null : ", PrimOp.GetErrorCode()
    else:
        cone2.SetName("Cone2: MakeConePntVecR1R2H(point6, vector1, 3.5, 0, 9)")
        print cone2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
cone3 = PrimOp.MakeConePntVecR1R2H(point6, vector1, 3.5, 0, -9)
if PrimOp.IsDone() == 0:
    print "MakeConePntVecR1R2H(point6, vector1, 3.5, 0, -9) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if cone3 is None:
        print "MakeConePntVecR1R2H(point6, vector1, 3.5, 0, -9) returned null : ", PrimOp.GetErrorCode()
    else:
        cone3.SetName("Cone3: MakeConePntVecR1R2H(point6, vector1, 3.5, 0, -9)")
        print cone3.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
torus1 = PrimOp.MakeTorusPntVecRR(point7, vector2, 50, 49)
if PrimOp.IsDone() == 0:
    print "MakeTorusPntVecRR(point7, vector2, 50, 49) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if torus1 is None:
        print "MakeTorusPntVecRR(point7, vector2, 50, 49) returned null : ", PrimOp.GetErrorCode()
    else:
        torus1.SetName("Torus1: MakeTorusPntVecRR(point7, vector2, 50, 49)")
        print torus1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
torus2 = PrimOp.MakeTorusRR(50, 49)
if PrimOp.IsDone() == 0:
    print "MakeTorusRR(50, 49) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if torus2 is None:
        print "MakeTorusRR(50, 49) returned null : ", PrimOp.GetErrorCode()
    else:
        torus2.SetName("Torus2: MakeTorusRR(50, 49)")
        print torus2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
prism1 = PrimOp.MakePrismVecH(poly1, vector3, 10)
if PrimOp.IsDone() == 0:
    print "MakePrismVecH(poly1, vector3, 10) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if prism1 is None:
        print "MakePrismVecH(poly1, vector3, 10) returned null : ", PrimOp.GetErrorCode()
    else:
        prism1.SetName("Prism1: MakePrismVecH(poly1, vector3, 10)")
        print prism1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
pipe1 = PrimOp.MakePipe(circle1, bspline1)
if PrimOp.IsDone() == 0:
    print "MakePipe(circle1, bspline1) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if pipe1 is None:
        print "MakePipe(circle1, bspline1) returned null : ", PrimOp.GetErrorCode()
    else:
        pipe1.SetName("Pipe1: MakePipe(circle1, bspline1)")
        print pipe1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
pipe2 = PrimOp.MakePipe(circle1, box1)
if PrimOp.IsDone() == 0:
    print "MakePipe(circle1, box1) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if pipe2 is None:
        print "MakePipe(circle1, box1) returned null : ", PrimOp.GetErrorCode()
    else:
        pipe2.SetName("Pipe2: MakePipe(circle1, box1)")
        print pipe2.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
revol1 = PrimOp.MakeRevolutionAxisAngle(bezier1, vector3, 10*math.pi/180.0)
if PrimOp.IsDone() == 0:
    print "MakeRevolutionAxisAngle(bezier1, vector3, 10*math.pi/180.0) NOT DONE : ", PrimOp.GetErrorCode()
else:
    if revol1 is None:
        print "MakeRevolutionAxisAngle(bezier1, vector3, 10*math.pi/180.0) returned null : ", PrimOp.GetErrorCode()
    else:
        revol1.SetName("Revolution1: MakeRevolutionAxisAngle(bezier1, vector3, 10*math.pi/180.0)")
        print revol1.GetName(), " : ", PrimOp.GetErrorCode()

#######################################################
### Shapes Creation
#######################################################
###
ShapesOp = geom.GetIShapesOperations(id)
print "\n --- Shapes Creation --- \n"

#######################################################
edge1 = ShapesOp.MakeEdge(point1, point2)
if ShapesOp.IsDone() == 0:
    print "MakeEdge(point1, point2) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if edge1 is None:
        print "MakeEdge(point1, point2) returned null : ", ShapesOp.GetErrorCode()
    else:
        edge1.SetName("Edge1: MakeEdge(point1, point2)")
        print edge1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
edge2 = ShapesOp.MakeEdge(point2, point3)
if ShapesOp.IsDone() == 0:
    print "MakeEdge(point2, point3) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if edge2 is None:
        print "MakeEdge(point2, point3) returned null : ", ShapesOp.GetErrorCode()
    else:
        edge2.SetName("Edge2: MakeEdge(point2, point3)")
        print edge2.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
edge3 = ShapesOp.MakeEdge(point3, point1)
if ShapesOp.IsDone() == 0:
    print "MakeEdge(point3, point1) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if edge3 is None:
        print "MakeEdge(point3, point1) returned null : ", ShapesOp.GetErrorCode()
    else:
        edge3.SetName("Edge3: MakeEdge(point3, point1)")
        print edge3.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
wire1 = ShapesOp.MakeWire([edge1, edge2, edge3])
if ShapesOp.IsDone() == 0:
    print "MakeWire([edge1, edge2, edge3]) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if wire1 is None:
        print "MakeWire([edge1, edge2, edge3]) returned null : ", ShapesOp.GetErrorCode()
    else:
        wire1.SetName("Wire1: MakeWire([edge1, edge2, edge3])")
        print wire1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
face1 = ShapesOp.MakeFace(wire1, 1)
if ShapesOp.IsDone() == 0:
    print "MakeFace(wire1, 1) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if face1 is None:
        print "MakeFace(wire1, 1) returned null : ", ShapesOp.GetErrorCode()
    else:
        face1.SetName("Face1: MakeFace(wire1, 1)")
        print face1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
shell1 = ShapesOp.MakeShell([face1])
if ShapesOp.IsDone() == 0:
    print "MakeShell([face1]) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if shell1 is None:
        print "MakeShell([face1]) returned null : ", ShapesOp.GetErrorCode()
    else:
        shell1.SetName("Shell1: MakeShell([face1])")
        print shell1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
solid1 = ShapesOp.MakeSolidShells([shell1])
if ShapesOp.IsDone() == 0:
    print "MakeSolidShells([shell1]) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if solid1 is None:
        print "MakeSolidShells([shell1]) returned null : ", ShapesOp.GetErrorCode()
    else:
        solid1.SetName("Solid1: MakeSolidShells([shell1])")
        print solid1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
solid2 = ShapesOp.MakeSolidShell(shell1)
if ShapesOp.IsDone() == 0:
    print "MakeSolidShell(shell1) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if solid2 is None:
        print "MakeSolidShell(shell1) returned null : ", ShapesOp.GetErrorCode()
    else:
        solid2.SetName("Solid2: MakeSolidShell(shell1)")
        print solid2.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
comp1 = ShapesOp.MakeCompound([box1, sphere2, cone2])
if ShapesOp.IsDone() == 0:
    print "MakeCompound([box1, sphere2, cone2]) NOT DONE : ", ShapesOp.GetErrorCode()
else:
    if comp1 is None:
        print "MakeCompound([box1, sphere2, cone2]) returned null : ", ShapesOp.GetErrorCode()
    else:
        comp1.SetName("Compound1: MakeCompound([box1, sphere2, cone2])")
        print comp1.GetName(), " : ", ShapesOp.GetErrorCode()

#######################################################
### Transformations
#######################################################
###
TrsfOp = geom.GetITransformOperations(id)
print "\n --- Transformations --- \n"

#######################################################
TrsfOp.TranslateTwoPoints(cone1, point1, point2)
if TrsfOp.IsDone() == 0:
    print "TranslateTwoPoints(cone1, point1, point2) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "TranslateTwoPoints(cone1, point1, point2) DONE : ", TrsfOp.GetErrorCode()

#######################################################
transl_cone1 = TrsfOp.TranslateTwoPointsCopy(cone1, point1, point2)
if TrsfOp.IsDone() == 0:
    print "TranslateTwoPointsCopy(cone1, point1, point2) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if transl_cone1 is None:
        print "TranslateTwoPointsCopy(cone1, point1, point2) returned null : ", TrsfOp.GetErrorCode()
    else:
        transl_cone1.SetName("Copy1a: TranslateTwoPointsCopy(cone1, point1, point2)")
        print transl_cone1.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.TranslateVector(cone1, vector2)
if TrsfOp.IsDone() == 0:
    print "TranslateVector(cone1, vector2) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "TranslateVector(cone1, vector2) DONE : ", TrsfOp.GetErrorCode()

#######################################################
transl_cone2 = TrsfOp.TranslateVectorCopy(cone1, vector2)
if TrsfOp.IsDone() == 0:
    print "TranslateVectorCopy(cone1, vector2) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if transl_cone2 is None:
        print "TranslateVectorCopy(cone1, vector2) returned null : ", TrsfOp.GetErrorCode()
    else:
        transl_cone2.SetName("Copy1b: TranslateVectorCopy(cone1, vector2)")
        print transl_cone2.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
#TrsfOp.Rotate(cone1, vector2, math.pi/2.0)
#if TrsfOp.IsDone() == 0:
#    print "Rotate(cone1, vector2, ) NOT DONE : ", TrsfOp.GetErrorCode()
#else:
#    print "Rotate(cone1, vector2, ) DONE : ", TrsfOp.GetErrorCode()

#######################################################
#rotate_cone2 = TrsfOp.RotateCopy(cone1, vector2, math.pi/4.0)
#if TrsfOp.IsDone() == 0:
#    print "RotateCopy(cone1, vector2, ) NOT DONE : ", TrsfOp.GetErrorCode()
#else:
#    if rotate_cone2 is None:
#        print "RotateCopy(cone1, vector2, ) returned null : ", TrsfOp.GetErrorCode()
#    else:
#        rotate_cone2.SetName("Copy1c: RotateCopy(cone1, vector2, )")
#        print rotate_cone2.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.MirrorPlane(arc1, plane1)
if TrsfOp.IsDone() == 0:
    print "MirrorPlane(arc1, plane1) NOT DONE: ", TrsfOp.GetErrorCode()
else:
    print "MirrorPlane(arc1, plane1) DONE: ", TrsfOp.GetErrorCode()

#######################################################
mirror_ell1 = TrsfOp.MirrorPlaneCopy(ellipse1, plane3)
if TrsfOp.IsDone() == 0:
    print "MirrorPlaneCopy(ellipse1, plane3) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if mirror_ell1 is None:
        print "MirrorPlaneCopy(ellipse1, plane3) returned null : ", TrsfOp.GetErrorCode()
    else:
        mirror_ell1.SetName("Copy4: MirrorPlaneCopy(ellipse1, plane3)")
        print mirror_ell1.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.MirrorAxis(arc1, line1)
if TrsfOp.IsDone() == 0:
    print "MirrorAxis(arc1, line1) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "MirrorAxis(arc1, line1) DONE : ", TrsfOp.GetErrorCode()

#######################################################
mirror_ell1_vec3 = TrsfOp.MirrorAxisCopy(ellipse1, vector3)
if TrsfOp.IsDone() == 0:
    print "MirrorAxisCopy(ellipse1, vector3) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if mirror_ell1_vec3 is None:
        print "MirrorAxisCopy(ellipse1, vector3) returned null : ", TrsfOp.GetErrorCode()
    else:
        mirror_ell1_vec3.SetName("Copy5: MirrorAxisCopy(ellipse1, vector3)")
        print mirror_ell1_vec3.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.MirrorPoint(arc1, point7)
if TrsfOp.IsDone() == 0:
    print "MirrorPoint(arc1, point7) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "MirrorPoint(arc1, point7) DONE : ", TrsfOp.GetErrorCode()

#######################################################
mirror_ell1_pnt7 = TrsfOp.MirrorPointCopy(ellipse1, point7)
if TrsfOp.IsDone() == 0:
    print "MirrorPointCopy(ellipse1, point7) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if mirror_ell1_pnt7 is None:
        print "MirrorPointCopy(ellipse1, point7) returned null : ", TrsfOp.GetErrorCode()
    else:
        mirror_ell1_pnt7.SetName("Copy6: MirrorPointCopy(ellipse1, point7)")
        print mirror_ell1_pnt7.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.OffsetShape(cone1, 11)
if TrsfOp.IsDone() == 0:
    print "OffsetShape(cone1, 11) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "OffsetShape(cone1, 11) DONE : ", TrsfOp.GetErrorCode()

#######################################################
offset_cone2 = TrsfOp.OffsetShapeCopy(cone1, 11)
if TrsfOp.IsDone() == 0:
    print "OffsetShapeCopy(cone1, 11) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if offset_cone2 is None:
        print "OffsetShapeCopy(cone1, 11) returned null : ", TrsfOp.GetErrorCode()
    else:
        offset_cone2.SetName("Copy2: OffsetShapeCopy(cone1, 11)")
        print offset_cone2.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
TrsfOp.ScaleShape(cylinder1, point1, 3)
if TrsfOp.IsDone() == 0:
    print "ScaleShape(cylinder1, point1, 3) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    print "ScaleShape(cylinder1, point1, 3) DONE : ", TrsfOp.GetErrorCode()

#######################################################
copy_scale1 = TrsfOp.ScaleShapeCopy(cylinder1, point1, 3)
if TrsfOp.IsDone() == 0:
    print "ScaleShapeCopy(cylinder1, point1, 3) NOT DONE : ", TrsfOp.GetErrorCode()
else:
    if copy_scale1 is None:
        print "ScaleShapeCopy(cylinder1, point1, 3) returned null : ", TrsfOp.GetErrorCode()
    else:
        copy_scale1.SetName("Copy3: ScaleShapeCopy(cylinder1, point1, 3)")
        print copy_scale1.GetName(), " : ", TrsfOp.GetErrorCode()

#######################################################
### Boolean Operations
#######################################################
###
BoolOp = geom.GetIBooleanOperations(id)
print "\n --- Boolean Operations --- \n"

#######################################################
common1 = BoolOp.MakeBoolean(box1, sphere1, 1)
#print common1
#print dir(common1)
if BoolOp.IsDone() == 0:
    print "MakeBoolean(box1, sphere1, 1) NOT DONE : ", BoolOp.GetErrorCode()
else:
    if common1 is None:
        print "MakeBoolean(box1, sphere1, 1) returned null : ", BoolOp.GetErrorCode()
    else:
#        print common1.GetEntry()
        common1.SetName("Common1: MakeBoolean(box1, sphere1, 1)")
        print common1.GetName(), " : ", BoolOp.GetErrorCode()
#        geom.PublishInStudy(myStudy, None, common1, "common_box1_sphere1")

#######################################################
#part_limit = batchmode_geompy.ShapeType["SHAPE"]

partition1 = BoolOp.MakePartition([box1, sphere1], [], [], [], 8, 1, [7, 7])
if BoolOp.IsDone() == 0:
    print "MakePartition([box1, sphere1], [], [], [], SHAPE, 1, [7, 7]) NOT DONE : ", BoolOp.GetErrorCode()
else:
    if partition1 is None:
        print "MakePartition([box1, sphere1], [], [], [], SHAPE, 1, [7, 7]) returned null : ", BoolOp.GetErrorCode()
    else:
        partition1.SetName("Partition1: MakePartition([box1, sphere1], [], [], [], SHAPE, 1, [7, 7])")
        print partition1.GetName(), " : ", BoolOp.GetErrorCode()

#######################################################
### Local Operations
#######################################################
###
LocalOp = geom.GetILocalOperations(id)
print "\n --- Local Operations --- \n"

#######################################################
fillet1 = LocalOp.MakeFilletAll(box1, 7)
if LocalOp.IsDone() == 0:
    print "MakeFilletAll(box1, 7) NOT DONE : ", LocalOp.GetErrorCode()
else:
    if fillet1 is None:
        print "MakeFilletAll(box1, 7) returned null : ", LocalOp.GetErrorCode()
    else:
        fillet1.SetName("Fillet1: MakeFilletAll(box1, 7)")
        print fillet1.GetName(), " : ", LocalOp.GetErrorCode()
