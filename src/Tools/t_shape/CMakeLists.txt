# Copyright (C) 2014-2020  EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

IF(SALOME_BUILD_GUI)
  INCLUDE(UsePyQt)
ENDIF()

# --- scripts ---

# base scripts
SET(_base_SCRIPTS
  __init__.py
  t_shape_builder.py
  )

IF(SALOME_BUILD_GUI)
   # gui scripts 
   SET(_gui_SCRIPTS
     t_shape_dialog.py
     t_shape_dialog.ui
     t_shape_progress.py
     )

  # uic files / to be processed by pyuic
  SET(_pyuic_FILES
    t_shape_dialog.ui
    )

  # scripts / pyuic wrappings
  PYQT_WRAP_UIC(_pyuic_SCRIPTS ${_pyuic_FILES} TARGET_NAME _target_name_pyuic)
ENDIF()

# --- rules ---

SALOME_INSTALL_SCRIPTS("${_base_SCRIPTS}" ${SALOME_INSTALL_PYTHON}/salome/geom/t_shape)

IF(SALOME_BUILD_GUI)
  SALOME_INSTALL_SCRIPTS("${_gui_SCRIPTS}" ${SALOME_INSTALL_PYTHON}/salome/geom/t_shape)
  SALOME_INSTALL_SCRIPTS("${_pyuic_SCRIPTS}" ${SALOME_INSTALL_PYTHON}/salome/geom/t_shape TARGET_NAME _target_name_pyuic_py)
  # add dependency of compiled py files on uic files in order
  # to avoid races problems when compiling in parallel
  ADD_DEPENDENCIES(${_target_name_pyuic_py} ${_target_name_pyuic})
ENDIF()

IF(SALOME_BUILD_TESTS)
  SET(TEST_HELPER ${KERNEL_ROOT_DIR}/bin/salome/test_helper.py)
  SALOME_GENERATE_TESTS_ENVIRONMENT(tests_env)
  ADD_TEST(NAME t_shape_builder
           COMMAND ${PYTHON_EXECUTABLE} -B ${TEST_HELPER} ${CMAKE_CURRENT_SOURCE_DIR}/test_t_shape_builder.py)

  SET(TEST_INSTALL_DIRECTORY ${SALOME_INSTALL_SCRIPT_SCRIPTS}/test/t_shape)
  INSTALL(FILES test_t_shape_builder.py DESTINATION ${TEST_INSTALL_DIRECTORY})

  INSTALL(FILES CTestTestfileInstall.cmake
          DESTINATION ${TEST_INSTALL_DIRECTORY}
          RENAME CTestTestfile.cmake)
ENDIF()
