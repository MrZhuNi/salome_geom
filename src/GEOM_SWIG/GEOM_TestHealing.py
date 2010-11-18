#  -*- coding: iso-8859-1 -*-
#  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#  GEOM GEOM_SWIG : binding of C++ implementaion with Python
#  File   : GEOM_TestHealing.py
#  Author : Julia DOROVSKIKH
#  Module : GEOM
#  $Header$
#
def TestProcessShape (geompy):

  ##Load shape from BREP file
  #import os
  #datadir = os.getenv("GEOM_TEST")
  #if datadir != 0:
  #  datadir = datadir + "/Resources/"
  #else:
  #  "Please, define GEOM_TEST variable !"
  #
  #print "Import ", datadir + "aze2.brep"
  #Shape = batchmode_geompy.Import(datadir + "aze2.brep", "BREP")

  p1 = geompy.MakeVertex(0,0,0)
  p2 = geompy.MakeVertex(200,0,0)
  p3 = geompy.MakeVertex(100,150,0)

  edge = geompy.MakeEdge(p1,p2)
  arc  = geompy.MakeArc(p1,p3,p2)
  wire = geompy.MakeWire([edge,arc])
  face = geompy.MakeFace(wire, 1)

  theShape = geompy.MakePrismVecH(face, edge, 130)

  #Check shape
  print "Before ProcessShape:"
  isValid = geompy.CheckShape(theShape)
  if isValid == 0:
    print "The shape is not valid"
  else:
    print "The shape seems to be valid"

  #Process Shape
  Operators = ["FixShape"]
  Parameters = ["FixShape.Tolerance3d"]
  Values = ["1e-7"]

  PS = geompy.ProcessShape(theShape, Operators, Parameters, Values)

  #Check shape
  print "After ProcessShape:"
  isValid = geompy.CheckShape(PS)
  if isValid == 0:
    print "The shape is not valid"
    raise RuntimeError, "It seems, that the ProcessShape() has failed"
  else:
    print "The shape seems to be valid"

  #Add In Study
  Id_Shape = geompy.addToStudy(theShape, "Invalid Shape")
  Id_PS    = geompy.addToStudy(PS, "Processed Shape")

def TestSuppressFaces (geompy):

  #Create base geometry 3D
  Box = geompy.MakeBoxDXDYDZ(200., 200., 200.)

  #IDList for SuppHole
  faces = []
  faces = geompy.SubShapeAllSortedCentres(Box, geompy.ShapeType["FACE"])

  f_glob_id = geompy.GetSubShapeID(Box, faces[5])

  #Transform objects
  SuppFace = geompy.SuppressFaces(Box, [f_glob_id])

  #Add In Study
  Id_SuppFace = geompy.addToStudy(SuppFace, "SuppFace")

def TestSuppressInternalWires (geompy):

  #Create Face with hole
  p11 = geompy.MakeVertex( 0,  0, 0)
  p12 = geompy.MakeVertex(30,  0, 0)
  p13 = geompy.MakeVertex(30, 30, 0)
  p14 = geompy.MakeVertex( 0, 30, 0)

  p21 = geompy.MakeVertex(10, 10, 0)
  p22 = geompy.MakeVertex(20, 10, 0)
  p23 = geompy.MakeVertex(20, 20, 0)
  p24 = geompy.MakeVertex(10, 20, 0)

  e11 = geompy.MakeEdge(p11, p12)
  e12 = geompy.MakeEdge(p12, p13)
  e13 = geompy.MakeEdge(p13, p14)
  e14 = geompy.MakeEdge(p14, p11)

  e21 = geompy.MakeEdge(p21, p22)
  e22 = geompy.MakeEdge(p22, p23)
  e23 = geompy.MakeEdge(p23, p24)
  e24 = geompy.MakeEdge(p24, p21)

  w1 = geompy.MakeWire([e11, e12, e13, e14])
  w2 = geompy.MakeWire([e21, e22, e23, e24])

  id_w1 = geompy.addToStudy(w1, "Outside Wire")
  id_w2 = geompy.addToStudy(w2, "Inside Wire")

  f12 = geompy.MakeFaceWires([w1, w2], 0)
  id_f12 = geompy.addToStudy(f12, "Face WO + WI")

  #Get Free Boundary
  Res1 = geompy.GetFreeBoundary(f12)
  isSuccess1   = Res1[0]
  ClosedWires1 = Res1[1]
  OpenWires1   = Res1[2]
  nbw1 = 0

  for wire in ClosedWires1:
    nbw1 = nbw1 + 1

  if nbw1 != 2:
    raise RuntimeError, "GetFreeBoundary(f12) must return 2 closed wires, but returned ", nbw1

  #SuppressInternalWires
  face = geompy.SuppressInternalWires(f12, [])

  #Get Free Boundary
  Res2 = geompy.GetFreeBoundary(face)
  isSuccess2   = Res2[0]
  ClosedWires2 = Res2[1]
  OpenWires2   = Res2[2]
  nbw2 = 0

  for wire in ClosedWires2:
    nbw2 = nbw2 + 1

  if nbw2 != 1:
    print "GetFreeBoundary(face) must return 1 closed wires, but returned ", nbw2
    raise RuntimeError, "SuppressInternalWires() works not correctly"

  #Add In Study
  Id_face = geompy.addToStudy(face, "Face without internal wires")

def TestCloseContour (geompy):

  ##Load shape from BREP file
  #import os
  #datadir = os.getenv("GEOM_TEST")
  #if datadir != 0:
  #  datadir = datadir + "/Resources/"
  #else:
  #  "Please, define GEOM_TEST variable !"
  #
  #print "Import ", datadir + "open_cont.brep"
  #Shape = geompy.Import(datadir + "open_cont.brep", "BREP")

  p0   = geompy.MakeVertex(0.  , 0.  , 0.  )
  py   = geompy.MakeVertex(0.  , 100., 0.  )
  pz   = geompy.MakeVertex(0.  , 0.  , 100.)
  p200 = geompy.MakeVertex(200., 200., 200.)

  Shape = geompy.MakePolyline([p0, pz, py, p200])

  #Check shape
  print "Before closing contour:"
  isValid = geompy.CheckShape(Shape)
  if isValid == 0:
    print "The shape is not valid"
  else:
    print "The shape seems to be valid"

  #Close Contour
  IsCommonVertex = 0 # false

  shape_wires = geompy.SubShapeAll(Shape, geompy.ShapeType["WIRE"])
  Wires = []
  wi = 0

  for wire in shape_wires:
    Wires.append(geompy.GetSubShapeID(Shape, shape_wires[wi]))
    wi = wi + 1

  CC = geompy.CloseContour(Shape, Wires, IsCommonVertex)

  #Check shape
  print "After closing contour:"
  isValid = geompy.CheckShape(CC)
  if isValid == 0:
    print "The shape is not valid"
    raise RuntimeError, "It seems, that the contour was not closed"
  else:
    print "The shape seems to be valid"

  #Add In Study
  Id_Shape = geompy.addToStudy(Shape, "Shape with open wire")
  Id_CC    = geompy.addToStudy(CC, "Shape with closed wire")

def TestSuppressHoles (geompy):

  #Create base Variables
  radius = 50.
  height = 300.

  #Create base points
  p1 = geompy.MakeVertex(100., 100., 50.)

  #Create base directions
  vz = geompy.MakeVectorDXDYDZ(0., 0., 100.)

  #Create base geometry 3D
  Box      = geompy.MakeBoxDXDYDZ(200., 200., 200.)
  Cylinder = geompy.MakeCylinder(p1, vz, radius, height)

  #Boolean (Cut)
  Cut = geompy.MakeBoolean(Box, Cylinder, 2)
  idCut = geompy.addToStudy(Cut, "CUT")

  #IDList for SuppressFaces
  faces = []
  faces = geompy.SubShapeAllSortedCentres(Cut, geompy.ShapeType["FACE"])
  ind = 0
  for face in faces:
      f_name = "FACE %d"%(ind)
      f_id = geompy.addToStudyInFather(Cut, face, f_name)

      f_glob_id = geompy.GetSubShapeID(Cut, face)
      print "face ", ind, " global index = ", f_glob_id
      ind = ind + 1

  f_glob_id_0 = geompy.GetSubShapeID(Cut, faces[0])
  cut_without_f_0 = geompy.SuppressFaces(Cut, [f_glob_id_0])
  geompy.addToStudy(cut_without_f_0, "Cut without face 0")

  faces1 = []
  faces1 = geompy.SubShapeAllSortedCentres(cut_without_f_0, geompy.ShapeType["FACE"])
  ind = 0
  for face in faces1:
      f_name = "FACE %d"%(ind)
      f_id = geompy.addToStudyInFather(cut_without_f_0, face, f_name)

      f_glob_id = geompy.GetSubShapeID(cut_without_f_0, face)
      print "face ", ind, " global index = ", f_glob_id
      ind = ind + 1

  f_glob_id_3 = geompy.GetSubShapeID(cut_without_f_0, faces1[3])
  cut_without_f_0_3 = geompy.SuppressFaces(cut_without_f_0, [f_glob_id_3])
  cut_without_f_0_3_id = geompy.addToStudy(cut_without_f_0_3, "Cut without faces 0 and 3")

  #IDList for SuppHole
  wires = []
  wires = geompy.SubShapeAllSortedCentres(cut_without_f_0_3, geompy.ShapeType["WIRE"])
  ind = 0
  for wire in wires:
      w_name = "WIRE %d"%(ind)
      w_id = geompy.addToStudyInFather(cut_without_f_0_3, wire, w_name)

      w_glob_id = geompy.GetSubShapeID(cut_without_f_0_3, wire)
      print "wire ", ind, " global index = ", w_glob_id
      ind = ind + 1

  w_3 = geompy.GetSubShapeID(cut_without_f_0_3, wires[3])

  SuppHole3 = geompy.SuppressHoles(cut_without_f_0_3, [w_3])
  SuppHole3_id = geompy.addToStudy(SuppHole3, "Supp Hole 3")

def TestMakeSewing (geompy, math):

  #Create base points
  px = geompy.MakeVertex(100., 0., 0.)
  py = geompy.MakeVertex(0., 100., 0.)
  pz = geompy.MakeVertex(0., 0., 100.)

  #Create base geometry 2D & 3D
  Vector = geompy.MakeVector(px, py)
  Arc    = geompy.MakeArc(py, pz, px)

  #Create base objects
  angle     = 45. * math.pi / 180
  WantPlanarFace = 1 #True

  Wire = geompy.MakeWire([Vector, Arc])
  Face = geompy.MakeFace(Wire, WantPlanarFace)
  S    = geompy.MakeRotation(Face, Vector, angle)

  #Make Sewing
  precision = 0.00001
  Sewing = geompy.MakeSewing([Face, S], precision)

  #Add In Study
  id_Sewing = geompy.addToStudy(Sewing, "Sewing")

def TestDivideEdge (geompy):

  #Create Box
  Box = geompy.MakeBoxDXDYDZ(200., 200., 200.)

  #Divide Edge
  box_edges = geompy.SubShapeAllSortedCentres(Box, geompy.ShapeType["EDGE"])
  edge_ind = geompy.GetSubShapeID(Box, box_edges[1])

  Divide = geompy.DivideEdge(Box, edge_ind, 0.5, 1) # Obj, ind, param, is_curve_param

  #Add In Study
  Id_Box    = geompy.addToStudy(Box, "Box")
  Id_Divide = geompy.addToStudy(Divide, "Box with Divided Edge")

def TestHealingOperations (geompy, math):

  TestMakeSewing(geompy, math)
  TestDivideEdge(geompy)
  TestSuppressHoles(geompy)
  TestSuppressInternalWires(geompy)
  TestCloseContour(geompy)
  TestSuppressFaces(geompy)
  TestProcessShape(geompy)
