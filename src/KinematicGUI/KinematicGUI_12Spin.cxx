//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : KinematicGUI_12Spin.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "KinematicGUI_12Spin.h"

#include <qlayout.h>
#include <qspinbox.h>
#include <qgroupbox.h>

/* 
 *  Constructs a KinematicGUI_12Spin which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
KinematicGUI_12Spin::KinematicGUI_12Spin(QWidget* parent,  const char* name, WFlags fl)
  :KinematicGUI_12Spin_QTD(parent, name, fl)
{
  SpinBox1->close(TRUE);
  SpinBox2->close(TRUE);
  SpinBox3->close(TRUE);
  SpinBox4->close(TRUE);
  SpinBox5->close(TRUE);
  SpinBox6->close(TRUE);
  SpinBox7->close(TRUE);
  SpinBox8->close(TRUE);
  SpinBox9->close(TRUE);
  SpinBox10->close(TRUE);
  SpinBox11->close(TRUE);
  SpinBox12->close(TRUE);
  SpinBox_11 = new DlgRef_SpinBox(GroupBox1, "SpinBox_11");
  SpinBox_12 = new DlgRef_SpinBox(GroupBox1, "SpinBox_12");
  SpinBox_13 = new DlgRef_SpinBox(GroupBox1, "SpinBox_13");
  SpinBox_21 = new DlgRef_SpinBox(GroupBox1, "SpinBox_21");
  SpinBox_22 = new DlgRef_SpinBox(GroupBox1, "SpinBox_22");
  SpinBox_23 = new DlgRef_SpinBox(GroupBox1, "SpinBox_23");
  SpinBox_31 = new DlgRef_SpinBox(GroupBox1, "SpinBox_31");
  SpinBox_32 = new DlgRef_SpinBox(GroupBox1, "SpinBox_32");
  SpinBox_33 = new DlgRef_SpinBox(GroupBox1, "SpinBox_33");
  SpinBox_41 = new DlgRef_SpinBox(GroupBox1, "SpinBox_41");
  SpinBox_42 = new DlgRef_SpinBox(GroupBox1, "SpinBox_42");
  SpinBox_43 = new DlgRef_SpinBox(GroupBox1, "SpinBox_43");
  Layout1->addWidget(SpinBox_11, 0, 2);
  Layout1->addWidget(SpinBox_12, 0, 4);
  Layout1->addWidget(SpinBox_13, 0, 6);
  Layout1->addWidget(SpinBox_21, 1, 2);
  Layout1->addWidget(SpinBox_22, 1, 4);
  Layout1->addWidget(SpinBox_23, 1, 6);
  Layout1->addWidget(SpinBox_31, 2, 2);
  Layout1->addWidget(SpinBox_32, 2, 4);
  Layout1->addWidget(SpinBox_33, 2, 6);
  Layout1->addWidget(SpinBox_41, 3, 2);
  Layout1->addWidget(SpinBox_42, 3, 4);
  Layout1->addWidget(SpinBox_43, 3, 6);
}


/*  
 *  Destroys the object and frees any allocated resources
 */
KinematicGUI_12Spin::~KinematicGUI_12Spin()
{
    // no need to delete child widgets, Qt does it all for us
}
