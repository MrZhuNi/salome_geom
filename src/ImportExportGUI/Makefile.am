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

#  File   : Makefile.am
#  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
#
include $(top_srcdir)/adm_local/unix/make_common_starter.am

# header files 
salomeinclude_HEADERS =			\
	ImportExportGUI.h		\
	ImportExportGUI_Widgets.h
	
IMPORTEXPORT_INCLUDES = 
IMPORTEXPORT_INCLUDES += ImportExportGUI_ExportXAODlg.h
##@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@ do not remove this line @@##

salomeinclude_HEADERS += $(IMPORTEXPORT_INCLUDES)

# Libraries targets
lib_LTLIBRARIES = libImportExportGUI.la

dist_libImportExportGUI_la_SOURCES =	\
	ImportExportGUI.cxx		\
	ImportExportGUI_Widgets.cxx
	
IMPORTEXPORT_SOURCES = 
IMPORTEXPORT_SOURCES += ImportExportGUI_ExportXAODlg.h ImportExportGUI_ExportXAODlg.cxx
##@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@ do not remove this line @@##

dist_libImportExportGUI_la_SOURCES += $(IMPORTEXPORT_SOURCES)

MOC_FILES =

IMPORTEXPORT_MOC_FILES = 
IMPORTEXPORT_MOC_FILES += ImportExportGUI_ExportXAODlg_moc.cxx
##@@ insert new functions before this line @@ do not remove this line @@ do not remove this line @@ do not remove this line @@##

MOC_FILES += $(IMPORTEXPORT_MOC_FILES)

nodist_libImportExportGUI_la_SOURCES =	\
	$(MOC_FILES)

UIC_FILES =						\
	ui_ImportExportGUI_1Sel1LineEdit2ListWidget_QTD.h

BUILT_SOURCES = $(UIC_FILES)
# additional information to compile and link file

libImportExportGUI_la_CPPFLAGS =			\
	$(QT_INCLUDES) 				\
	$(VTK_INCLUDES)				\
	$(CAS_CPPFLAGS)				\
	$(PYTHON_INCLUDES)			\
	$(BOOST_CPPFLAGS)			\
	$(KERNEL_CXXFLAGS)			\
	$(GUI_CXXFLAGS)				\
	$(CORBA_CXXFLAGS)			\
	$(CORBA_INCLUDES)			\
	-I$(srcdir)/../GEOMGUI			\
	-I$(srcdir)/../DlgRef			\
	-I$(srcdir)/../GEOMBase			\
	-I$(srcdir)/../OBJECT			\
	-I$(srcdir)/../GEOMClient		\
	-I$(srcdir)/../GEOMImpl			\
	-I$(srcdir)/../GEOMFiltersSelection	\
	-I$(top_builddir)/src/DlgRef		\
	-I$(top_builddir)/idl

libImportExportGUI_la_LDFLAGS = 					\
	../GEOMFiltersSelection/libGEOMFiltersSelection.la	\
	../GEOMBase/libGEOMBase.la
