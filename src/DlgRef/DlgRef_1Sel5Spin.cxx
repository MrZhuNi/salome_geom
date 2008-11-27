//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  GEOM GEOMGUI : GUI for Geometry component
//  File   : DlgRef_1Sel5Spin.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 
//
#include "DlgRef_1Sel5Spin.h"

#include <qlayout.h>
#include <qspinbox.h>
#include <qgroupbox.h>

/* 
 *  Constructs a DlgRef_1Sel5Spin which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
DlgRef_1Sel5Spin::DlgRef_1Sel5Spin(QWidget* parent,  const char* name, WFlags fl)
  :DlgRef_1Sel5Spin_QTD(parent, name, fl)
{
  SpinBox1->close(TRUE);
  SpinBox2->close(TRUE);
  SpinBox3->close(TRUE);
  SpinBox4->close(TRUE);
  SpinBox5->close(TRUE);

  SpinBox_1 = new DlgRef_SpinBox(GroupBox1, "SpinBox_1");
  Layout2->addWidget(SpinBox_1, 0, 1);

  SpinBox_2 = new DlgRef_SpinBox(GroupBox1, "SpinBox_2");
  Layout2->addWidget(SpinBox_2, 0, 3);

  SpinBox_3 = new DlgRef_SpinBox(GroupBox1, "SpinBox_3");
  Layout2->addWidget(SpinBox_3, 2, 1);

  SpinBox_4 = new DlgRef_SpinBox(GroupBox1, "SpinBox_4");
  Layout2->addWidget(SpinBox_4, 1, 1);

  SpinBox_5 = new DlgRef_SpinBox(GroupBox1, "SpinBox_5");
  Layout2->addWidget(SpinBox_5, 1, 3);
}


/*  
 *  Destroys the object and frees any allocated resources
 */
DlgRef_1Sel5Spin::~DlgRef_1Sel5Spin()
{
    // no need to delete child widgets, Qt does it all for us
}
