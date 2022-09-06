#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2022  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

import salome

salome.salome_init()

import GEOM
from salome.geom import geomBuilder
import math
import SALOMEDS

geompy = geomBuilder.New()

def GetShapeType(theShape):
    CR = geompy.CanonicalRecognition()
    if CR.isLine(theShape, 0.1)[0]:
        return "Line"
    if CR.isPlane(theShape, 0.1)[0]:
        if CR.isCircle(theShape, 0.1)[0]:
            return ("Plane","Circle")
        if CR.isEllipse(theShape, 0.1)[0]:
            return ("Plane","Ellipse")
        return "Plane"
    if CR.isSphere(theShape, 0.1)[0]:
        return "Sphere"
    if CR.isCone(theShape, 0.1)[0]:
        return "Cone"
    if CR.isCylinder(theShape, 0.1)[0]:
        return "Cylinder"
    return "Not defined"

O = geompy.MakeVertex(0, 0, 0)
OX = geompy.MakeVectorDXDYDZ(1, 0, 0)
OY = geompy.MakeVectorDXDYDZ(0, 1, 0)
OZ = geompy.MakeVectorDXDYDZ(0, 0, 1)
Cylinder_1 = geompy.MakeCylinderRH(100, 300)
[Face_1,Face_2,Face_3] = geompy.ExtractShapes(Cylinder_1, geompy.ShapeType["FACE"], True)
[Edge_1,Edge_2,Edge_3] = geompy.ExtractShapes(Cylinder_1, geompy.ShapeType["EDGE"], True)
Sphere_1 = geompy.MakeSphereR(100)
[Shell_1] = geompy.ExtractShapes(Sphere_1, geompy.ShapeType["SHELL"], True)
Cone_1 = geompy.MakeConeR1R2H(100, 0, 300)
[Shell_2] = geompy.ExtractShapes(Cone_1, geompy.ShapeType["SHELL"], True)
[Face_4,Face_5] = geompy.ExtractShapes(Shell_2, geompy.ShapeType["FACE"], True)

## Create ellips
Cylinder_2 = geompy.MakeCylinderRH(100, 300)
Plane_1 = geompy.MakePlaneLCS(None, 350, 1)
Translation_1 = geompy.MakeTranslation(Plane_1, 0, 0, 150)
Rotation_1 = geompy.MakeRotation(Translation_1, OX, 20*math.pi/180.0)
Partition_1 = geompy.MakePartition([Cylinder_2], [Rotation_1], [], [], geompy.ShapeType["SOLID"], 0, [], 0)
[Edge_4,Edge_5,Edge_6,Edge_7,Edge_8] = geompy.ExtractShapes(Partition_1, geompy.ShapeType["EDGE"], True)

assert GetShapeType(Face_1) == "Plane"
assert GetShapeType(Face_2) == "Cylinder"
assert GetShapeType(Edge_3) == "Line"
assert GetShapeType(Edge_2)[1] == "Circle"
assert GetShapeType(Shell_1) == "Sphere"
assert GetShapeType(Shell_2) == "Cone"
assert GetShapeType(Edge_5)[1] == "Ellipse"
