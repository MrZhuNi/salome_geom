# Copyright (C) 2009-2012  CEA/DEN, EDF R&D
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
#  File   : Makefile.in
#  Author : Nathalie Gore
#
include $(top_srcdir)/adm_local/unix/make_common_starter.am

# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
    XAO_XaoUtils.hxx \
    XAO_GeometricElement.hxx \
    XAO_Xao.hxx \
    XAO_Geometry.hxx \
    XAO_BrepGeometry.hxx \
    XAO_Group.hxx \
    XAO_Field.hxx \
    XAO_BooleanField.hxx \
    XAO_IntegerField.hxx \
    XAO_DoubleField.hxx \
    XAO_StringField.hxx \
    XAO_Step.hxx \
    XAO_BooleanStep.hxx \
    XAO_IntegerStep.hxx \
    XAO_DoubleStep.hxx \
    XAO_StringStep.hxx \
    XAO_XaoExporter.hxx

lib_LTLIBRARIES = libXAO.la
dist_libXAO_la_SOURCES = \
    XAO_XaoUtils.cxx \
    XAO_GeometricElement.cxx \
    XAO_Xao.cxx \
    XAO_Geometry.cxx \
    XAO_BrepGeometry.cxx \
    XAO_Group.cxx \
    XAO_Field.cxx \
    XAO_BooleanField.cxx \
    XAO_IntegerField.cxx \
    XAO_DoubleField.cxx \
    XAO_StringField.cxx \
    XAO_Step.cxx \
    XAO_BooleanStep.cxx \
    XAO_IntegerStep.cxx \
    XAO_DoubleStep.cxx \
    XAO_StringStep.cxx \
    XAO_XaoExporter.cxx

libXAO_la_CPPFLAGS = \
    $(CAS_CPPFLAGS)                     \
    $(LIBXML_INCLUDES)                  \
    -I$(top_builddir)                   \
    -I$(top_builddir)/idl

libXAO_la_LDFLAGS = \
    $(CAS_LDPATH)                       \
    $(LIBXML_LIBS)                      \
    -lTKBRep -lTKTopAlgo -lxml2

