# Suppress Holes

import geompy
import salome
gg = salome.ImportComponentGUI("GEOM")

# create a vertex and a vector
p1 = geompy.MakeVertex(35, 35, 0)
p2 = geompy.MakeVertex(35, 35, 50)
v = geompy.MakeVector(p1, p2)

# create a cylinder
height = 20
radius1 = 20
cylinder = geompy.MakeCylinder(p1, v, radius1, height)

# create a cone
cone = geompy.MakeCone(p1, v, 70, 0, 80)

# make a cut
cut = geompy.MakeCut(cone, cylinder)

# get faces as sub-shapes
faces = []
faces = geompy.SubShapeAllSortedCentres(cut, geompy.ShapeType["FACE"])
f_2 = geompy.GetSubShapeID(cut, faces[2])

# remove one face from the shape
cut_without_f_2 = geompy.SuppressFaces(cut, [f_2])

# get wires as sub-shapes
wires = []
wires = geompy.SubShapeAllSortedCentres(cut_without_f_2, geompy.ShapeType["WIRE"])
w_0 = geompy.GetSubShapeID(cut_without_f_2, wires[0])

# suppress the selected wire
result = geompy.SuppressHoles(cut_without_f_2, [w_0])

# add objects in the study
id_cut = geompy.addToStudy(cut, "Cut")
id_cut_without_f_2 = geompy.addToStudy(cut_without_f_2, "Cut without f_2")
id_result = geompy.addToStudy(result, "Result")

# display the results
gg.createAndDisplayGO(id_cut)
gg.setDisplayMode(id_cut,1)
gg.createAndDisplayGO(id_cut_without_f_2)
gg.setDisplayMode(id_cut_without_f_2,1)
gg.createAndDisplayGO(id_result)
gg.setDisplayMode(id_result,1) 