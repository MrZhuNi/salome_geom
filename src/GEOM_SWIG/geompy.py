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

geom = lcc.FindOrLoadComponent("FactoryServer", "GEOM")
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

# -----------------------------------------------------------------------------
# add To Study
# -----------------------------------------------------------------------------

gg = ImportComponentGUI("GEOM")

def SubShapeName(aSubObj, aMainObj):
    aSubId  = orb.object_to_string(aSubObj)
    aMainId = orb.object_to_string(aMainObj)
    index = gg.getIndexTopology(aSubId, aMainId)
    name = gg.getShapeTypeString(aSubId) + "_%d"%(index)
    return name

# -----------------------------------------------------------------------------
# The code below is common with batchmode_geompy.py
# -----------------------------------------------------------------------------

def addToStudy(aShape, aName):
    try:
        aSObject = geom.AddInStudy(myStudy, aShape, aName, None)
    except:
        print "addToStudy() failed"
        return ""
    return aShape.GetStudyEntry()

def addToStudyInFather(aFather, aShape, aName):
    try:
#        myBuilder.NewCommand()
        aSObject = geom.AddInStudy(myStudy, aShape, aName, aFather)
#        myBuilder.CommitCommand()
    except:
        print "addToStudyInFather() failed"
        return ""
    return aShape.GetStudyEntry()

# -----------------------------------------------------------------------------
# enumeration ShapeType as a dictionary
# -----------------------------------------------------------------------------

ShapeType = {"COMPOUND":0, "COMPSOLID":1, "SOLID":2, "SHELL":3, "FACE":4, "WIRE":5, "EDGE":6, "VERTEX":7, "SHAPE":8}

# -----------------------------------------------------------------------------
# Get Operations Interfaces
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

# -----------------------------------------------------------------------------
# Basic primitives
# -----------------------------------------------------------------------------

def MakeVertex(x,y,z):
    anObj = BasicOp.MakePointXYZ(x,y,z)
    if BasicOp.IsDone() == 0:
      print "MakePointXYZ : ", BasicOp.GetErrorCode()
    return anObj

def MakeVertexWithRef(vertex,x,y,z):
    anObj = BasicOp.MakePointWithReference(vertex,x,y,z)
    if BasicOp.IsDone() == 0:
      print "MakePointWithReference : ", BasicOp.GetErrorCode()
    return anObj

def MakeVertexOnCurve(curve,par):
    anObj = BasicOp.MakePointOnCurve(curve,par)
    if BasicOp.IsDone() == 0:
      print "MakePointOnCurve : ", BasicOp.GetErrorCode()
    return anObj

def MakeVectorDXDYDZ(dx,dy,dz):
    anObj = BasicOp.MakeVectorDXDYDZ(dx,dy,dz)
    if BasicOp.IsDone() == 0:
      print "MakeVectorDXDYDZ : ", BasicOp.GetErrorCode()
    return anObj

def MakeVector(p1,p2):
    anObj = BasicOp.MakeVectorTwoPnt(p1, p2)
    if BasicOp.IsDone() == 0:
      print "MakeVectorTwoPnt : ", BasicOp.GetErrorCode()
    return anObj

def MakeLine(p1, p2):
    anObj = BasicOp.MakeLineTwoPnt(p1,p2)
    if BasicOp.IsDone() == 0:
      print "MakeLineTwoPnt : ", BasicOp.GetErrorCode()
    return anObj

def MakePlane(p1,v1,trimsize):
    anObj = BasicOp.MakePlanePntVec(p1,v1,trimsize)
    if BasicOp.IsDone() == 0:
      print "MakePlanePntVec : ", BasicOp.GetErrorCode()
    return anObj

def MakePlaneThreePnt(p1,p2,p3,trimsize):
    anObj = BasicOp.MakePlaneThreePnt(p1,p2,p3,trimsize)
    if BasicOp.IsDone() == 0:
      print "MakePlaneThreePnt : ", BasicOp.GetErrorCode()
    return anObj

def MakePlaneFace(face,trimsize):
    anObj = BasicOp.MakePlaneFace(face,trimsize)
    if BasicOp.IsDone() == 0:
      print "MakePlaneFace : ", BasicOp.GetErrorCode()
    return anObj

def MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ):
    anObj = BasicOp.MakeMarker(OX,OY,OZ, XDX,XDY,XDZ, YDX,YDY,YDZ)
    if BasicOp.IsDone() == 0:
      print "MakeMarker : ", BasicOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Curves
# -----------------------------------------------------------------------------

def MakeArc(p1,p2,p3):
    anObj = CurvesOp.MakeArc(p1,p2,p3)
    if CurvesOp.IsDone() == 0:
      print "MakeArc : ", CurvesOp.GetErrorCode()
    return anObj

def MakeCircle(p1,v1,radius):
    anObj = CurvesOp.MakeCirclePntVecR(p1,v1,radius)
    if CurvesOp.IsDone() == 0:
      print "MakeCirclePntVecR : ", CurvesOp.GetErrorCode()
    return anObj

def MakeCircleThreePnt(p1,p2,p3):
    anObj = CurvesOp.MakeCircleThreePnt(p1,p2,p3)
    if CurvesOp.IsDone() == 0:
      print "MakeCircleThreePnt : ", CurvesOp.GetErrorCode()
    return anObj

def MakeEllipse(p1,v1,radiusMaj,radiusMin):
    anObj = CurvesOp.MakeEllipse(p1,v1,radiusMaj, radiusMin)
    if CurvesOp.IsDone() == 0:
      print "MakeEllipse : ", CurvesOp.GetErrorCode()
    return anObj

def MakePolyline(ListShape):
    anObj = CurvesOp.MakePolyline(ListShape)
    if CurvesOp.IsDone() == 0:
      print "MakePolyline : ", CurvesOp.GetErrorCode()
    return anObj

def MakeBezier(ListShape):
    anObj = CurvesOp.MakeSplineBezier(ListShape)
    if CurvesOp.IsDone() == 0:
      print "MakeSplineBezier : ", CurvesOp.GetErrorCode()
    return anObj

def MakeInterpol(ListShape):
    anObj = CurvesOp.MakeSplineInterpolation(ListShape)
    if CurvesOp.IsDone() == 0:
      print "MakeSplineInterpolation : ", CurvesOp.GetErrorCode()
    return anObj

def MakeSketcher(Cmd, WPL):
    anObj = CurvesOp.MakeSketcher(Cmd, WPL)
    if CurvesOp.IsDone() == 0:
      print "MakeSketcher : ", CurvesOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create 3D Primitives
# -----------------------------------------------------------------------------

def MakeBox(x1,y1,z1,x2,y2,z2):
    pnt1 = MakeVertex(x1,y1,z1)
    pnt2 = MakeVertex(x2,y2,z2)
    return MakeBoxTwoPnt(pnt1,pnt2)

def MakeBoxDXDYDZ(dx,dy,dz):
    anObj = PrimOp.MakeBoxDXDYDZ(dx,dy,dz)
    if PrimOp.IsDone() == 0:
      print "MakeBoxDXDYDZ : ", PrimOp.GetErrorCode()
    return anObj

def MakeBoxTwoPnt(point1, point2):
    anObj = PrimOp.MakeBoxTwoPnt(point1, point2)
    if PrimOp.IsDone() == 0:
      print "MakeBoxTwoPnt : ", PrimOp.GetErrorCode()
    return anObj

def MakeCylinder(p1,v1,radius,height):
    anObj = PrimOp.MakeCylinderPntVecRH(p1,v1,radius,height)
    if PrimOp.IsDone() == 0:
      print "MakeCylinderPntVecRH : ", PrimOp.GetErrorCode()
    return anObj

def MakeCylinderRH(radius,height):
    anObj = PrimOp.MakeCylinderRH(radius,height)
    if PrimOp.IsDone() == 0:
      print "MakeCylinderRH : ", PrimOp.GetErrorCode()
    return anObj

def MakeSphere(point,radius):
    anObj = PrimOp.MakeSpherePntR(point,radius)
    if PrimOp.IsDone() == 0:
      print "MakeSpherePntR : ", PrimOp.GetErrorCode()
    return anObj

def MakeSphereR(radius):
    anObj = PrimOp.MakeSphereR(radius)
    if PrimOp.IsDone() == 0:
      print "MakeSphereR : ", PrimOp.GetErrorCode()
    return anObj

def MakeCone(p1,v1,radius1,radius2,height):
    anObj = PrimOp.MakeConePntVecR1R2H(p1,v1,radius1,radius2,height)
    if PrimOp.IsDone() == 0:
      print "MakeConePntVecR1R2H : ", PrimOp.GetErrorCode()
    return anObj

def MakeConeR1R2H(radius1,radius2,height):
    anObj = PrimOp.MakeConeR1R2H(radius1,radius2,height)
    if PrimOp.IsDone() == 0:
      print "MakeConeR1R2H : ", PrimOp.GetErrorCode()
    return anObj

def MakeTorus(p1,v1,major_radius,minor_radius):
    anObj = PrimOp.MakeTorusPntVecRR(p1,v1,major_radius,minor_radius)
    if PrimOp.IsDone() == 0:
      print "MakeTorusPntVecRR : ", PrimOp.GetErrorCode()
    return anObj

def MakeTorusRR(major_radius,minor_radius):
    anObj = PrimOp.MakeTorusRR(major_radius,minor_radius)
    if PrimOp.IsDone() == 0:
      print "MakeTorusRR : ", PrimOp.GetErrorCode()
    return anObj

def MakePrism(baseShape,vector,height):
    anObj = PrimOp.MakePrismVecH(baseShape,vector,height)
    if PrimOp.IsDone() == 0:
      print "MakePrismVecH : ", PrimOp.GetErrorCode()
    return anObj

def MakePrismTwoPnt(baseShape,point1,point2):
    anObj = PrimOp.MakePrismTwoPnt(baseShape,point1,point2)
    if PrimOp.IsDone() == 0:
      print "MakePrismTwoPnt : ", PrimOp.GetErrorCode()
    return anObj

def MakePipe(baseShape,pathShape):
    anObj = PrimOp.MakePipe(baseShape,pathShape)
    if PrimOp.IsDone() == 0:
      print "MakePipe : ", PrimOp.GetErrorCode()
    return anObj

def MakeRevolution(aShape,axis,angle):
    anObj = PrimOp.MakeRevolutionAxisAngle(aShape,axis,angle)
    if PrimOp.IsDone() == 0:
      print "MakeRevolutionAxisAngle : ", PrimOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create base shapes
# -----------------------------------------------------------------------------

def MakeEdge(p1,p2):
    anObj = ShapesOp.MakeEdge(p1,p2)
    if ShapesOp.IsDone() == 0:
      print "MakeEdge : ", ShapesOp.GetErrorCode()
    return anObj

def MakeWire(ListShape):
    anObj = ShapesOp.MakeWire(ListShape)
    if ShapesOp.IsDone() == 0:
      print "MakeWire : ", ShapesOp.GetErrorCode()
    return anObj

def MakeFace(aShapeWire,WantPlanarFace):
    anObj = ShapesOp.MakeFace(aShapeWire,WantPlanarFace)
    if ShapesOp.IsDone() == 0:
      print "MakeFace : ", ShapesOp.GetErrorCode()
    return anObj

def MakeFaceWires(ListWires,WantPlanarFace):
    anObj = ShapesOp.MakeFaceWires(ListWires,WantPlanarFace)
    if ShapesOp.IsDone() == 0:
      print "MakeFaceWires : ", ShapesOp.GetErrorCode()
    return anObj

def MakeShell(ListOfShapes):
    anObj = ShapesOp.MakeShell(ListOfShapes)
    if ShapesOp.IsDone() == 0:
	print "MakeShell : ", ShapesOp.GetErrorCode()
    return anObj

def MakeSolid(ListOfShells):
    anObj = ShapesOp.MakeSolidShells(ListOfShells)
    if ShapesOp.IsDone() == 0:
	print "MakeSolid : ", ShapesOp.GetErrorCode()
    return anObj

def MakeCompound(ListShape):
    anObj = ShapesOp.MakeCompound(ListShape)
    if ShapesOp.IsDone() == 0:
      print "MakeCompound : ", ShapesOp.GetErrorCode()
    return anObj

def ChangeOrientation(Shape):
    anObj = ShapesOp.ChangeOrientation(Shape)
    if ShapesOp.IsDone() == 0:
      print "ChangeOrientation : ", ShapesOp.GetErrorCode()
    return anObj 

# -----------------------------------------------------------------------------
# Decompose objects
# -----------------------------------------------------------------------------

def SubShape(aShape,ListOfId):
    anObj = geom.AddSubShape(aShape,ListOfId)
    return anObj

def SubShapeAll(aShape,type):
    ListObj = ShapesOp.MakeExplode(aShape,type,0)
    if ShapesOp.IsDone() == 0:
      print "MakeExplode : ", ShapesOp.GetErrorCode()
    return ListObj

def SubShapeAllSorted(aShape,type):
    ListObj = ShapesOp.MakeExplode(aShape,type,1)
    if ShapesOp.IsDone() == 0:
      print "MakeExplode : ", ShapesOp.GetErrorCode()
    return ListObj

def GetSubShapeID(aShape, aSubShape):
    return LocalOp.GetSubShapeIndex(aShape, aSubShape)

# -----------------------------------------------------------------------------
# Healing operations
# -----------------------------------------------------------------------------

def ProcessShape(Shape, Operators, Parameters, Values):
    anObj = HealOp.ProcessShape(Shape, Operators, Parameters, Values)
    if HealOp.IsDone() == 0:
	print "ProcessShape : ", HealOp.GetErrorCode()
    return anObj

def SuppressFaces(aShape,ListOfId):
    anObj = HealOp.SuppressFaces(aShape,ListOfId)
    if HealOp.IsDone() == 0:
      print "SuppressFaces : ", HealOp.GetErrorCode()
    return anObj

def MakeSewing(ListShape,precision):
    comp = MakeCompound(ListShape)
    anObj = Sew(comp,precision)
    return anObj

def Sew(aShape,precision):
    anObj = HealOp.Sew(aShape,precision)
    if HealOp.IsDone() == 0:
      print "Sew : ", HealOp.GetErrorCode()
    return anObj

def SuppressInternalWires(aShape, Wires):
    anObj = HealOp.RemoveIntWires(aShape, Wires)
    if HealOp.IsDone() == 0:
      print "SuppressInternalWires : ", HealOp.GetErrorCode()
    return anObj

def SuppressHoles(aShape, ListOfId):
    anObj = HealOp.FillHoles(aShape,ListOfId)
    if HealOp.IsDone() == 0:
      print "SuppressHoles : ", HealOp.GetErrorCode()
    return anObj

def CloseContour(aShape, Wires, IsCommonVertex):
    anObj = HealOp.CloseContour(aShape, Wires, IsCommonVertex)
    if HealOp.IsDone() == 0:
      print "CloseContour : ", HealOp.GetErrorCode()
    return anObj

def DivideEdge(aShape, EdgeID, Value, IsByParameter):
    anObj = HealOp.DivideEdge(aShape, EdgeID, Value, IsByParameter)
    if HealOp.IsDone() == 0:
      print "DivideEdge : ", HealOp.GetErrorCode()
    return anObj

def GetFreeBoundary(Shape):
    anObj = HealOp.GetFreeBoundary(Shape)
    if HealOp.IsDone() == 0:
      print "GetFreeBoundaries : ", HealOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Create advanced objects
# -----------------------------------------------------------------------------

def MakeCopy(aShape):
    anObj = InsertOp.MakeCopy(aShape)
    if InsertOp.IsDone() == 0:
      print "MakeCopy : ", InsertOp.GetErrorCode()
    return anObj

def MakeFilling(aShape,mindeg,maxdeg,tol2d,tol3d,nbiter):
    anObj = PrimOp.MakeFilling(aShape,mindeg,maxdeg,tol2d,tol3d,nbiter)
    if PrimOp.IsDone() == 0:
      print "MakeFilling : ", PrimOp.GetErrorCode()
    return anObj

def MakeGlueFaces(aShape,aTolerance):
    anObj = ShapesOp.MakeGlueFaces(aShape,aTolerance)
    if ShapesOp.IsDone() == 0:
      print "MakeGlueFaces : ", ShapesOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Boolean (Common, Cut, Fuse, Section)
# -----------------------------------------------------------------------------

def MakeBoolean(shape1,shape2,operation):
    anObj = BoolOp.MakeBoolean(shape1,shape2,operation)
    if BoolOp.IsDone() == 0:
      print "MakeBoolean : ", BoolOp.GetErrorCode()
    return anObj

def MakePartition(ListShapes, ListTools=[], ListKeepInside=[], ListRemoveInside=[],
                  Limit=ShapeType["SHAPE"], RemoveWebs=0, ListMaterials=[]):
    anObj = BoolOp.MakePartition(ListShapes, ListTools,
                                 ListKeepInside, ListRemoveInside,
                                 Limit, RemoveWebs, ListMaterials);
    if BoolOp.IsDone() == 0:
      print "MakePartition : ", BoolOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Transform objects
# -----------------------------------------------------------------------------

def MakeTranslationTwoPoints(aShape,point1,point2):
    anObj = TrsfOp.TranslateTwoPointsCopy(aShape,point1,point2)
    if TrsfOp.IsDone() == 0:
      print "TranslateTwoPointsCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeTranslation(aShape,dx,dy,dz):
    anObj = TrsfOp.TranslateDXDYDZCopy(aShape,dx,dy,dz)
    if TrsfOp.IsDone() == 0:
      print "TranslateDXDYDZCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeRotation(aShape,axis,angle):
    anObj = TrsfOp.RotateCopy(aShape,axis,angle)
    if TrsfOp.IsDone() == 0:
      print "RotateCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeScaleTransform(aShape,theCenterofScale,factor):
    anObj = TrsfOp.ScaleShapeCopy(aShape,theCenterofScale,factor)
    if TrsfOp.IsDone() == 0:
      print "ScaleShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMirrorByPlane(aShape,aPlane):
    anObj = TrsfOp.MirrorPlaneCopy(aShape,aPlane)
    if TrsfOp.IsDone() == 0:
      print "MirrorPlaneCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakePosition(aShape,theStartLCS,theEndLCS):
    anObj = TrsfOp.PositionShapeCopy(aShape,theStartLCS,theEndLCS)
    if TrsfOp.IsDone() == 0:
      print "PositionShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

def MakeOffset(aShape, anOffset):
    anObj = TrsfOp.OffsetShapeCopy(aShape, anOffset)
    if TrsfOp.IsDone() == 0:
      print "OffsetShapeCopy : ", TrsfOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Patterns
# -----------------------------------------------------------------------------

def MakeMultiTranslation1D(aShape,aDir,aStep,aNbTimes):
    anObj = TrsfOp.MultiTranslate1D(aShape,aDir,aStep,aNbTimes)
    if TrsfOp.IsDone() == 0:
      print "MultiTranslate1D : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMultiTranslation2D(aShape,d1,step1,nbtimes1,d2,step2,nbtimes2):
    anObj = TrsfOp.MultiTranslate2D(aShape,d1,step1,nbtimes1,d2,step2,nbtimes2)
    if TrsfOp.IsDone() == 0:
      print "MultiTranslate2D : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMultiRotation1D(aShape,aVec,aNbTimes):
    anObj = TrsfOp.MultiRotate1D(aShape,aVec,aNbTimes)
    if TrsfOp.IsDone() == 0:
      print "MultiRotate1D : ", TrsfOp.GetErrorCode()
    return anObj

def MakeMultiRotation2D(aShape,aVec,anAngle,nbtimes1,aStep,nbtimes2):
    anObj = TrsfOp.MultiRotate2D(aShape,aVec,anAngle,nbtimes1,aStep,nbtimes2)
    if TrsfOp.IsDone() == 0:
      print "MultiRotate2D : ", TrsfOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Local operations
# -----------------------------------------------------------------------------

def MakeFilletAll(aShape,radius):
    anObj = LocalOp.MakeFilletAll(aShape,radius)
    if LocalOp.IsDone() == 0:
      print "MakeFilletAll : ", LocalOp.GetErrorCode()
    return anObj

def MakeFillet(aShape,radius,aShapeType,ListShape):
    anObj = None
    if aShapeType == ShapeType["EDGE"]:
        anObj = LocalOp.MakeFilletEdges(aShape,radius,ListShape)
    else:
        anObj = LocalOp.MakeFilletFaces(aShape,radius,ListShape)
    if LocalOp.IsDone() == 0:
      print "MakeFillet : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferAll(aShape,d):
    anObj = LocalOp.MakeChamferAll(aShape,d)
    if LocalOp.IsDone() == 0:
      print "MakeChamferAll : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferEdge(aShape,d1,d2,face1,face2):
    anObj = LocalOp.MakeChamferEdge(aShape,d1,d2,face1,face2)
    if LocalOp.IsDone() == 0:
      print "MakeChamferEdge : ", LocalOp.GetErrorCode()
    return anObj

def MakeChamferFaces(aShape,d1,d2,ListShape):
    anObj = LocalOp.MakeChamferFaces(aShape,d1,d2,ListShape)
    if LocalOp.IsDone() == 0:
      print "MakeChamferFaces : ", LocalOp.GetErrorCode()
    return anObj

def Archimede(aShape,weight,WaterDensity,MeshingDeflection):
    anObj = LocalOp.MakeArchimede(aShape,weight,WaterDensity,MeshingDeflection)
    if LocalOp.IsDone() == 0:
      print "MakeArchimede : ", LocalOp.GetErrorCode()
    return anObj

def MakeMultiTransformation1D(Block, DirFaceID1, DirFaceID2, NbTimes):
    anObj = BlocksOp.MakeMultiTransformation1D(Block, DirFaceID1, DirFaceID2, NbTimes)
    if BlocksOp.IsDone() == 0:
      print "MakeMultiTransformation1D : ", BlocksOp.GetErrorCode()
    return anObj

def MakeMultiTransformation2D(Block, DirFaceID1U, DirFaceID2U, NbTimesU, DirFaceID1V, DirFaceID2V, NbTimesV):
    anObj = BlocksOp.MakeMultiTransformation2D(Block, DirFaceID1U, DirFaceID2U, NbTimesU, DirFaceID1V, DirFaceID2V, NbTimesV)
    if BlocksOp.IsDone() == 0:
      print "MakeMultiTransformation2D : ", BlocksOp.GetErrorCode()
    return anObj

def MakeBlockExplode(Compound, MinNbFaces, MaxNbFaces):
    aList = BlocksOp.ExplodeCompoundOfBlocks(Compound, MinNbFaces, MaxNbFaces)
    if BlocksOp.IsDone() == 0:
      print "MakeBlockExplode : ", BlocksOp.GetErrorCode()
    return aList

# -----------------------------------------------------------------------------
# Information objects
# -----------------------------------------------------------------------------

def PointCoordinates(Point):
    aTuple = MeasuOp.PointCoordinates(Point)
    if MeasuOp.IsDone() == 0:
      print "PointCoordinates : ", MeasuOp.GetErrorCode()
    return aTuple

def BasicProperties(Shape):
    aTuple = MeasuOp.GetBasicProperties(Shape)
    if MeasuOp.IsDone() == 0:
      print "BasicProperties : ", MeasuOp.GetErrorCode()
    return aTuple

def BoundingBox(Shape):
    aTuple = MeasuOp.GetBoundingBox(Shape)
    if MeasuOp.IsDone() == 0:
      print "BoundingBox : ", MeasuOp.GetErrorCode()
    return aTuple

def Inertia(Shape):
    aTuple = MeasuOp.GetInertia(Shape)
    if MeasuOp.IsDone() == 0:
      print "Inertia : ", MeasuOp.GetErrorCode()
    return aTuple

def MinDistance(Shape1, Shape2):
    aTuple = MeasuOp.GetMinDistance(Shape1, Shape2)
    if MeasuOp.IsDone() == 0:
      print "MinDistance : ", MeasuOp.GetErrorCode()
    return aTuple[0]

def Tolerance(Shape):
    aTuple = MeasuOp.GetTolerance(Shape)
    if MeasuOp.IsDone() == 0:
      print "Tolerance : ", MeasuOp.GetErrorCode()
    return aTuple

def WhatIs(Shape):
    aDescr = MeasuOp.WhatIs(Shape)
    if MeasuOp.IsDone() == 0:
      print "WhatIs : ", MeasuOp.GetErrorCode()
    return aDescr

def MakeCDG(aShape):
    anObj = MeasuOp.GetCentreOfMass(aShape)
    if MeasuOp.IsDone() == 0:
      print "GetCentreOfMass : ", MeasuOp.GetErrorCode()
    return anObj

def CheckShape(aShape):
    (IsValid, Status) = MeasuOp.CheckShape(aShape)
    if MeasuOp.IsDone() == 0:
      print "CheckShape : ", MeasuOp.GetErrorCode()
    else:
      if IsValid == 0:
        print Status
    return IsValid

# -----------------------------------------------------------------------------
# Import/Export objects
# -----------------------------------------------------------------------------

def Import(filename, formatname):
    anObj = InsertOp.Import(filename, formatname)
    if InsertOp.IsDone() == 0:
      print "Import : ", InsertOp.GetErrorCode()
    return anObj

def Export(aShape, filename, formatname):
    InsertOp.Export(aShape, filename, formatname)
    if InsertOp.IsDone() == 0:
      print "Export : ", InsertOp.GetErrorCode()

# -----------------------------------------------------------------------------
# Block operations
# -----------------------------------------------------------------------------

def MakeQuadFace(E1, E2, E3, E4):
    anObj = BlocksOp.MakeQuadFaceFourEdges(E1, E2, E3, E4)
    if BlocksOp.IsDone() == 0:
      print "MakeQuadFace : ", BlocksOp.GetErrorCode()
    return anObj

def MakeQuadFaceTwoEdges(E1, E2):
    anObj = BlocksOp.MakeQuadFaceTwoEdges(E1, E2)
    if BlocksOp.IsDone() == 0:
      print "MakeQuadFaceEdge : ", BlocksOp.GetErrorCode()
    return anObj

def MakeQuadFaceVertices(V1, V2, V3, V4):
    anObj = BlocksOp.MakeQuadFaceFourVertices(V1, V2, V3, V4)
    if BlocksOp.IsDone() == 0:
      print "MakeQuadFaceVertex : ", BlocksOp.GetErrorCode()
    return anObj

def MakeHexaSolid(F1, F2, F3, F4, F5, F6):
    anObj = BlocksOp.MakeHexaSolidSixFaces(F1, F2, F3, F4, F5, F6)
    if BlocksOp.IsDone() == 0:
      print "MakeHexaSolid : ", BlocksOp.GetErrorCode()
    return anObj

def MakeHexaSolidTwoFaces(F1, F2):
    anObj = BlocksOp.MakeHexaSolidTwoFaces(F1, F2)
    if BlocksOp.IsDone() == 0:
      print "MakeHexaSolidTwoFace : ", BlocksOp.GetErrorCode()
    return anObj

# -----------------------------------------------------------------------------
# Group operations
# -----------------------------------------------------------------------------

def CreateGroup(MainShape, ShapeType):
    anObj = GroupOp.CreateGroup(MainShape, ShapeType)
    if GroupOp.IsDone() == 0:
       print "CreateGroup : ", GroupOp.GetErrorCode()
    return anObj

def AddObject(Group, SubShapeID):
    anObj = GroupOp.AddObject(Group, SubShapeID)
    if GroupOp.IsDone() == 0:
      print "AddObject : ", GroupOp.GetErrorCode()
    return anObj

def RemoveObject(Group, SubShapeID):
    anObj = GroupOp.RemoveObject(Group, SubShapeID)
    if GroupOp.IsDone() == 0:
      print "RemoveObject : ", GroupOp.GetErrorCode()
    return anObj

def GetObjectIDs(Group):
    anObj = GroupOp.GetObjects(Group)
    if GroupOp.IsDone() == 0:
      print "GetObjectIDs : ", GroupOp.GetErrorCode()
    return anObj

def addPath(Path):
    if (sys.path.count(Path) < 1):
	sys.path.append(Path)