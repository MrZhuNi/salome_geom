# Copyright (C) 2013  CEA/DEN, EDF R&D
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
# Author : Nathalie Gore (OpenCascade)

include $(top_srcdir)/adm_local/unix/make_common_starter.am

bin_PROGRAMS= TestXAO

TestXAO_CPPFLAGS= \
    @CPPUNIT_INCLUDES@ @PTHREAD_CFLAGS@  \
    $(GEOM_CXXFLAGS)                     \
    $(CAS_CPPFLAGS)                      \
    $(LIBXML_INCLUDES)                   \
    -I$(srcdir)

TestXAO_LDFLAGS = \
    @CPPUNIT_LIBS@  \
    $(CAS_LDPATH)   \
    $(LIBXML_LIBS)  \
    $(GEOM_LDFLAGS) \
    ../libXAO.la

dist_TestXAO_SOURCES = \
    XaoUtilsTest.cxx \
    GroupTest.cxx \
    FieldTest.cxx \
    GeometryTest.cxx \
    ImportExportTest.cxx \
    BrepGeometryTest.cxx \
    XaoTest.cxx \
    XAOTests.cxx

UNIT_TEST_PROG = TestXAO

check : tests

CLEANFILES = \
        UnitTestsResult

