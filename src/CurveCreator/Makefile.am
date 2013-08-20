# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
#  Author : Sergey KHROMOV
#  Modified by :
#  Module : GEOM
#
include $(top_srcdir)/adm_local/unix/make_common_starter.am

# Libraries targets
lib_LTLIBRARIES = libCurveCreator.la

# Sources files
dist_libCurveCreator_la_SOURCES =	\
	CurveCreator_Macro.hxx \
	CurveCreator.hxx \
	CurveCreator_Operation.hxx \
	CurveCreator_Diff.hxx \
	CurveCreator_Section.hxx \
	CurveCreator_Curve.hxx \
	CurveCreator_CurveEditor.hxx \
	CurveCreator_Operation.cxx \
	CurveCreator_Diff.cxx \
	CurveCreator_Curve.cxx \
	CurveCreator_CurveEditor.cxx

# Header files
salomeinclude_HEADERS =	\
	CurveCreator_Macro.hxx \
	CurveCreator.hxx \
	CurveCreator_Operation.hxx \
	CurveCreator_Diff.hxx \
	CurveCreator_Section.hxx \
	CurveCreator_Curve.hxx \
	CurveCreator_CurveEditor.hxx

# Compilation options for GUI mode
if GEOM_ENABLE_GUI
  dist_libCurveCreator_la_SOURCES +=  \
        CurveCreator_NewPointDlg.h \
        CurveCreator_NewSection.h \
        CurveCreator_TreeView.h \
        CurveCreator_Widget.h     \
        CurveCreator_NewPointDlg.cxx \
        CurveCreator_NewSectionDlg.cxx \
        CurveCreator_TreeView.cxx \
        CurveCreator_Widget.cxx

  salomeinclude_HEADERS += \
        CurveCreator_NewPointDlg.h \
        CurveCreator_NewSectionDlg.h \
        CurveCreator_TreeView.h \
        CurveCreator_Widget.h

  MOC_FILES = \
        CurveCreator_NewPointDlg_moc.cxx \
        CurveCreator_NewSectionDlg_moc.cxx \
        CurveCreator_TreeView_moc.cxx \
        CurveCreator_Widget_moc.cxx

  nodist_libCurveCreator_la_SOURCES = $(MOC_FILES)
endif

# additional information to compile and link file

libCurveCreator_la_CPPFLAGS =	\
	$(QT_INCLUDES)				\
	$(CAS_CPPFLAGS)		\
	$(KERNEL_CXXFLAGS) 	\
	$(GUI_CXXFLAGS)

libCurveCreator_la_LDFLAGS  =			\
	$(KERNEL_LDFLAGS)	\
	$(CAS_KERNEL)
