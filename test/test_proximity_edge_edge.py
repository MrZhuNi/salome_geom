# Shape Proximity between edges

import math
import salome
salome.salome_init_without_session()
import GEOM
from salome.geom import geomBuilder
geompy = geomBuilder.New()

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)

# create arc and segment
Vertex_1 = geompy.MakeVertex(0, 0, -1)
Vertex_2 = geompy.MakeVertex(1, 0, 0)
Vertex_3 = geompy.MakeVertex(0, 0, 1)
Arc_1 = geompy.MakeArc(Vertex_1, Vertex_2, Vertex_3)
Arc_1_vertex_2 = geompy.GetSubShape(Arc_1, [2])
Edge_1 = geompy.MakeEdgeOnCurveByLength(Arc_1, 3, Arc_1_vertex_2)
Edge_2 = geompy.MakeEdge(Vertex_1, Vertex_3)

shape1 = Edge_1
shape2 = Edge_2

# perform proximity calculation with the default parameters
p1 = geompy.ShapeProximity()
proximity1 = p1.proximity(shape1, shape2)

# perform proximity calculation with custom parameters
p2 = geompy.ShapeProximity()
p2.setShapes(shape1, shape2)
p2.setSampling(shape1, 100) # number of sample points for the first shape
p2.setSampling(shape2, 40)  # number of sample points for the second shape
proximity2_coarse = p2.coarseProximity()
proximity2_fine = p2.preciseProximity()

assert(math.fabs(proximity1 - proximity2_fine) < 1.e-7)
assert(math.fabs(proximity2_coarse - 0.9974949866) < 1.e-7)
assert(math.fabs(proximity2_fine - 1) < 1.e-7)

# move second edge and check proximity
Translation_1 = geompy.MakeTranslation(Edge_2, 0.3, 0, 0)
shape2 = Translation_1

# perform proximity calculation with the default parameters
p1 = geompy.ShapeProximity()
proximity1 = p1.proximity(shape1, shape2)

# perform proximity calculation with custom parameters
p2 = geompy.ShapeProximity()
p2.setShapes(shape1, shape2)
p2.setSampling(shape1, 100) # number of sample points for the first shape
p2.setSampling(shape2, 40)  # number of sample points for the second shape
proximity2_coarse = p2.coarseProximity()
proximity2_fine = p2.preciseProximity()

assert(math.fabs(proximity1 - 0.7) < 1.e-7)
assert(math.fabs(proximity2_fine - 0.7) < 1.e-7)
