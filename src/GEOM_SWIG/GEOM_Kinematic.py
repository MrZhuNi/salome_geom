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
#  File   : GEOM_Kinematic.py
#  Author : Damien COQUERET, Open CASCADE
#  Module : GEOM
#  $Header: 

import salome
import geompy
import math

geom = salome.lcc.FindOrLoadComponent("FactoryServer", "GEOM")
myBuilder = salome.myStudy.NewBuilder()

#Varaibles
TypeShape = 8 #Explode In Shape
Pivot = 1
Step = 0.
Duration = 5.
NbSeq = 50

#Import Shapes : Robot
Robot = geompy.ImportBREP("/home/dcq/robot.brep")
Id_Robot = geompy.addToStudy(Robot, "Robot")

#Decompose objects
SubShape = []
SubShape = geompy.SubShapeAll(Robot, TypeShape)

for Shape in SubShape :
    name = geompy.SubShapeName(Shape._get_Name(), Robot._get_Name())
#    Id_SubShape = geompy.addToStudy(Shape, "Shape")
    Id_SubShape = geompy.addToStudyInFather(Robot, Shape, name)

#Kinematic
anAss = geompy.InitAssembly()
Id_Ass = geompy.addAssemblyToStudy(anAss, "Robot_Assembly")

aContact1 = geompy.AddContact(anAss, SubShape[0], SubShape[1], Pivot, Step)
aContact1.SetPosition(0., 0., 0., 0., 0., 1., 1., 0., 0., 0., 1., 0.)
geompy.SetPosition(aContact1)
aContact1.SetAngularRange(0., 45., 0., 0., 0., 0.)
geompy.SetAngularRange(aContact1)
Id_Contact1 = geompy.addContactToStudy(anAss, aContact1, "Robot_Contact1")

aContact2 = geompy.AddContact(anAss, SubShape[1], SubShape[2], Pivot, Step)
aContact2.SetPosition(0., 0., 900., 0., 1., 0., 0., 0., 1., 1., 0., 0.)
geompy.SetPosition(aContact2)
aContact2.SetAngularRange(0., 30., 0., 0., 0., 0.)
geompy.SetAngularRange(aContact2)
Id_Contact2 = geompy.addContactToStudy(anAss, aContact2, "Robot_Contact2")

aContact3 = geompy.AddContact(anAss, SubShape[2], SubShape[3], Pivot, Step)
aContact3.SetPosition(565.7, 0., 1465.7, 0., 1., 0., 0., 0., 1., 1., 0., 0.)
geompy.SetPosition(aContact3)
aContact3.SetAngularRange(-180., 0., 0., 0., 0., 0.)
geompy.SetAngularRange(aContact3)
Id_Contact3 = geompy.addContactToStudy(anAss, aContact3, "Robot_Contact3")

aContact4 = geompy.AddContact(anAss, SubShape[3], SubShape[4], Pivot, Step)
aContact4.SetPosition(1132.5, 0., 901.1, 0., 1., 0., 0., 0., 1., 1., 0., 0.)
geompy.SetPosition(aContact4)
aContact4.SetAngularRange(0., 180., 0., 0., 0., 0.)
geompy.SetAngularRange(aContact4)
Id_Contact4 = geompy.addContactToStudy(anAss, aContact4, "Robot_Contact4")

aContact5 = geompy.AddContact(anAss, SubShape[4], SubShape[5], Pivot, Step)
aContact5.SetPosition(0., 0., 900., 1., 0., 0., 0., 1., 0., 0., 0., 1.)
geompy.SetPosition(aContact5)
aContact5.SetAngularRange(0., 180., 0., 0., 0., 0.)
geompy.SetAngularRange(aContact5)
Id_Contact5 = geompy.addContactToStudy(anAss, aContact5, "Robot_Contact5")

anAnim = geompy.AddAnimation(anAss, SubShape[0], Duration, NbSeq)
aList = []
aList.append(0.)
aList.append(-180.)
i = 0
while i <= 9 :
    aList.append(0.)
    i = i + 1
anAnim.SetDisplacement(aContact3, aList)
geompy.SetDisplacement(anAnim, aContact3) 
Id_Anim = geompy.addAnimationToStudy(anAnim, "Robot_Animation")
