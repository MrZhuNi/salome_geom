# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

include $(top_srcdir)/adm_local/unix/make_common_starter.am

# this option puts it to dist
#BUILT_SOURCES = swig_wrap.cpp

SWIG_FLAGS    = @SWIG_FLAGS@ -I$(srcdir)/../XAO
SWIG_SOURCES  = xao_swig.i

# Libraries targets
lib_LTLIBRARIES = _xao_swig.la
_xao_swig_la_SOURCES = $(SWIG_SOURCES)
nodist__xao_swig_la_SOURCES = swig_wrap.cpp
nodist_salomescript_DATA = xao_swig.py

dist_salomescript_PYTHON = xao.py

EXTRA_DIST += $(SWIG_SOURCES)

xao_swig.py: swig_wrap.cpp

_xao_swig_la_CPPFLAGS = \
	$(PYTHON_INCLUDES) \
	$(CAS_CPPFLAGS) \
	-I$(srcdir)/../XAO \
	-I$(top_builddir)/idl

_xao_swig_la_LDFLAGS = -module $(PYTHON_LIBS)
_xao_swig_la_LIBADD  = \
	../XAO/libXAO.la

swig_wrap.cpp : $(SWIG_SOURCES)
	$(SWIG) $(SWIG_FLAGS) -o $@ $<

CLEANFILES = swig_wrap.cpp xao_swig.py
