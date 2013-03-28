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
	Xao.hxx \
	Geometry.hxx \
	Group.hxx \
	Field.hxx

lib_LTLIBRARIES = libXAO.la
dist_libXAO_la_SOURCES = \
	Xao.cxx \
	Geometry.cxx \
	Group.cxx \
	Field.cxx



libXAO_la_CPPFLAGS = \
	$(GEOM_CXXFLAGS)                        \
	$(CAS_CPPFLAGS)                         \
	$(LIBXML_INCLUDES)	                \
	-I$(top_builddir)                       \
	-I$(top_builddir)/idl

libXAO_la_LDFLAGS = \
	$(CAS_LDPATH)                           \
	$(LIBXML_LIBS)				\
        $(GEOM_LDFLAGS)

