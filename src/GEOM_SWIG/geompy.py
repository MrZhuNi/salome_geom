#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

#  GEOM GEOM_SWIG : binding of C++ omplementaion with Python
#  File   : geompy.py
#  Author : Paul RASCLE, EDF
#  Module : GEOM
#
import salome
import geompyDC
from salome import *

# retrieve GEOM engine in try/except block
# to avoid problems in some cases, e.g. when generating documentation
try:
    # get GEOM engine and initialize GEOM with current study
    engineGeom = lcc.FindOrLoadComponent( "FactoryServer", "GEOM" )
    geom = geompyDC.geomInstance(salome.myStudy, engineGeom)

    # export the methods of geompyDC
    for k in dir( geom ):
	if k[0] == '_': continue
	globals()[k] = getattr( geom, k )
        pass
    del k
    from geompyDC import ShapeType, GEOM, kind, info, PackData, ReadTexture, EnumToLong
    pass
except:
    geom = None
    pass

print """
===============================================================================
WARNING:                                                                      |
Usage of geompy.py is deprecated after SALOME V7.2!                           |
geompy.py will be removed in a future version!                                |
TODO:                                                                         |
The following changes in your scripts are required to avoid this message:     |
                                                                              |
replace                                                                       |
-------                                                                       |
                                                                              |
import geompy                                                                 |
geompy.init_geom(theStudy)                                                    |
                                                                              |
with                                                                          |
----                                                                          |
                                                                              |
import geompyDC                                                               |
geompy = geompyDC.geomInstance(theStudy)                                      |
                                                                              |
===============================================================================
"""
