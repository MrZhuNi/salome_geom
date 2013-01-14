# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
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
#  Modified by : Nathalie Gore (OCN) - autotools usage
#  Module : GEOM
#  $Header: /home/server/cvs/GEOM/GEOM_SRC/src/XAOImport/Makefile.am,v 1.4 2012-08-09 07:43:02 vsr Exp $
#
include $(top_srcdir)/adm_local/unix/make_common_starter.am

# Libraries targets
lib_LTLIBRARIES = libXAOImport.la

# Sources files
dist_libXAOImport_la_SOURCES =	\
	XAOImport.cxx

# additional information to compile and link file

libXAOImport_la_CPPFLAGS =	\
	$(LIBXML_INCLUDES)	\
	$(KERNEL_CXXFLAGS)	\
	$(CAS_CPPFLAGS)

libXAOImport_la_LDFLAGS  =			\
	$(LIBXML_LIBS)				\
	$(KERNEL_LDFLAGS) -lSALOMELocalTrace	\
	$(CAS_LDPATH) -lTKBRep			\
	$(STDLIB)
