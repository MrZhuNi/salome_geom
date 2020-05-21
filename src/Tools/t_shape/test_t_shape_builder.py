import salome
salome.salome_init()

from salome.geom import geomBuilder
geompy = geomBuilder.New()

from salome.geom.t_shape.t_shape_builder import build_shape
num_errors = 0

for r1 in [1., 100.]:
    for r2 in [0.9*r1, 0.5*r1, 0.1*r1, 0.05*r1]:
        for thickness in [r1/100., r1/10., r1/2.]:
            print(r1, r2, thickness)
            h1 = r1 * 2.0
            h2 = h1
            try:
                res = build_shape(r1, r2, h1, h2, thickness)
                geompy.addToStudy(res, "res_%f_%f_%f"%(r1, r2, thickness))
            except:
                print("problem with res_%f_%f_%f"%(r1, r2, thickness))
                num_errors += 1
if num_errors > 0:
    raise RuntimeError("Error: 1 or more test cases failed!")
