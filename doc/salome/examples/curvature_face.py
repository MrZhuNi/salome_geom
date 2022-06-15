# Curvature of a Face along given direction

import salome
salome.salome_init_without_session()
import GEOM
from salome.geom import geomBuilder
geompy = geomBuilder.New()
import math

O = geompy.MakeVertex(0, 0, 0, 'O')
OX = geompy.MakeVectorDXDYDZ(1, 0, 0, 'OX')
OY = geompy.MakeVectorDXDYDZ(0, 1, 0, 'OY')
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1, 'OZ')

pXYZ = geompy.MakeVertex(105, 105, 105, 'pXYZ')
pY = geompy.MakeVertex(0, 105, 0, 'pY')
pZ = geompy.MakeVertex(0, 0, 105, 'pZ')

vZ_XY = geompy.MakeVectorDXDYDZ(-1, -1, 1, 'vZ-XY')
vZ_XY2 = geompy.MakeVectorDXDYDZ(-1, -1, 10, 'vZ-XY')
vZ_XY3 = geompy.MakeVectorDXDYDZ(-1, -1, 100, 'vZ-XY')

R = 100.0

# I. Curvature of a Sphere
Sphere_1 = geompy.MakeSphereR(R, 'Sphere_1')
[Sph] = geompy.ExtractShapes(Sphere_1, geompy.ShapeType["FACE"], True, "Sph")

curvature_1 = geompy.CurvatureOnFace(Sph, pXYZ, OX,    'curvature_sph_pXYZ_OX')
curvature_2 = geompy.CurvatureOnFace(Sph, pXYZ, vZ_XY, 'curvature_sph_pXYZ_vt')
curvature_3 = geompy.CurvatureOnFace(Sph, pY,   OX,    'curvature_sph_pY_OX')
# Pole
curvature_p = geompy.CurvatureOnFace(Sph, pZ,   OX,    'curvature_sph_pZ_OX')

# All sphere curvature radiuces = R
assert(abs(geompy.BasicProperties(curvature_1)[0] - R) < 1e-07)
assert(abs(geompy.BasicProperties(curvature_2)[0] - R) < 1e-07)
assert(abs(geompy.BasicProperties(curvature_3)[0] - R) < 1e-07)
assert(abs(geompy.BasicProperties(curvature_p)[0] - R) < 1e-07)

# Normal direction
isExcept = False
try:
  geompy.CurvatureOnFace(Sph, pY,   OY)
except:
  isExcept = True
assert(isExcept)

# II. Curvature of a Cylinder
Cylinder_1 = geompy.MakeCylinderRH(R, 300, 'Cylinder_1')
[Face_1,Face_2,Face_3] = geompy.ExtractShapes(Cylinder_1, geompy.ShapeType["FACE"], True, "Face")

# Curvature radius of a cylinder along any direction, orthogonal to its Z axis, equal to R
curvature_4 = geompy.CurvatureOnFace(Face_2, pY, OX, 'curvature_cyl_pY_OX')
assert(abs(geompy.BasicProperties(curvature_4)[0] - R) < 1e-07)

# Curvature radius of a cylinder along its Z direction is infinite
curvature_zero = geompy.CurvatureOnFace(Face_2, pY, OZ)
assert(geompy.MeasuOp.GetErrorCode() == "ZERO_CURVATURE")
assert(not curvature_zero)

# Curvature radius of a cylinder along some direction, different from two above
curvature_5 = geompy.CurvatureOnFace(Face_2, pY, vZ_XY,  'curvature_cyl_pY_vZ_XY')
curvature_6 = geompy.CurvatureOnFace(Face_2, pY, vZ_XY2, 'curvature_cyl_pY_vZ_XY2')
curvature_7 = geompy.CurvatureOnFace(Face_2, pY, vZ_XY3, 'curvature_cyl_pY_vZ_XY3')

# R < r5 < r6 < r7
# r5 = 100.01, r6 = 101.0, r7 = 200
r5 = geompy.BasicProperties(curvature_5)[0]
r6 = geompy.BasicProperties(curvature_6)[0]
r7 = geompy.BasicProperties(curvature_7)[0]

assert(R  + 1e-07 < r5)
assert(r5 + 1e-07 < r6)
assert(r6 + 1e-07 < r7)

# Projection aborted. Point is out of the face boundaries.
isExcept = False
try:
  pXY_Z = geompy.MakeVertex(105, 105, -105, 'pXY_Z')
  geompy.CurvatureOnFace(Face_2, pXY_Z, OX, 'curvature_cyl_pXY_Z')
except:
  isExcept = True
assert(isExcept)

# Projection aborted (point on axis). Equal distances to many points.
isExcept = False
try:
  geompy.CurvatureOnFace(Face_2, O, vZ_XY, 'curvature_cyl_O')
except:
  isExcept = True
assert(isExcept)

# Curvature radius of a planar face is infinite
curvature_zero_2 = geompy.CurvatureOnFace(Face_1, pZ, OX)
assert(geompy.MeasuOp.GetErrorCode() == "ZERO_CURVATURE")
assert(not curvature_zero_2)

# III. Curvature of a "Horse saddle"
[Edge_1,Edge_2,Edge_3] = geompy.ExtractShapes(Sphere_1, geompy.ShapeType["EDGE"], True)
geompy.addToStudyInFather( Sphere_1, Edge_1, 'Edge_1' )
geompy.addToStudyInFather( Sphere_1, Edge_2, 'Edge_2' )
geompy.addToStudyInFather( Sphere_1, Edge_3, 'Edge_3' )

Rotation_1 = geompy.MakeRotation(Edge_3, OX, 90*math.pi/180.0, 'Rotation_1')
Rotation_2 = geompy.MakeRotation(Rotation_1, OY, 180*math.pi/180.0, 'Rotation_2')
Translation_1 = geompy.MakeTranslation(Rotation_2, 200, 0, 0, 'Translation_1')
Translation_2 = geompy.MakeTranslation(Edge_3, 100, 100, 0, 'Translation_2')
Translation_3 = geompy.MakeTranslation(Translation_2, 0, -200, 0, 'Translation_3')
Filling_1 = geompy.MakeFilling([Translation_2, Edge_3, Translation_3])
geompy.addToStudy(Filling_1, 'Filling_1')
Vertex_2 = geompy.MakeVertex(100, 0, 0, 'Vertex_2')

curvature_Y = geompy.CurvatureOnFace(Filling_1, Vertex_2, OY, 'curvature_Y')
curvature_Z = geompy.CurvatureOnFace(Filling_1, Vertex_2, OZ, 'curvature_Z')

cury = geompy.VectorCoordinates(curvature_Y)
curz = geompy.VectorCoordinates(curvature_Z)

# Vectors should be opposite, scalar product should be negative
assert(cury[0]*curz[0] + cury[1]*curz[1] + cury[2]*curz[2] < -1e-07)

# Normal direction
norm_1 = geompy.GetNormal(Filling_1, Vertex_2, "Normal_1")
isExcept = False
try:
  geompy.CurvatureOnFace(Filling_1, Vertex_2, norm_1)
except:
  isExcept = True
assert(isExcept)
