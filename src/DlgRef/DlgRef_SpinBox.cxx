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
//  File   : DlgRef_SpinBox.cxx
//  Author : Lucien PIGNOLONI
//  Module : GEOM
//  $Header$

using namespace std;
#include "DlgRef_SpinBox.h"
//#include "DlgRef.h"
#include <qvalidator.h>
//#include <qpushbutton.h>
//#include "utilities.h"

//=================================================================================
// class    : DlgRef_SpinBox()
// purpose  : constructor of specific widget accepting floats in double precision.
//=================================================================================
DlgRef_SpinBox::DlgRef_SpinBox( QWidget* parent, const char* name )
: QAD_SpinBoxDbl( parent, name)
{
  /* when step value is changed in myGeomGUI */
  //connect( DlgRef::GetDlgRef(), SIGNAL( SignalDefaultStepValueChanged( double )), this, SLOT( SetStep( double ) ) );
}


//=================================================================================
// function : SetStep()  [SLOT]
// purpose  :
//=================================================================================
void DlgRef_SpinBox::SetStep( double newStep )
{
  setLineStep( newStep );
}

//=================================================================================
// function : ~DlgRef_SpinBox()
// purpose  : destructor
//=================================================================================
DlgRef_SpinBox::~DlgRef_SpinBox()
{
}

//=================================================================================
// function : SetValue()
// purpose  :
//=================================================================================
void DlgRef_SpinBox::SetValue( double v )
{
  setValue( v );
}

//=================================================================================
// function : GetValue()
// purpose  : returns a double
//=================================================================================
double DlgRef_SpinBox::GetValue( )
{
  return value();
}

//=================================================================================
// function : GetString()
// purpose  : returns a QString
//=================================================================================
QString DlgRef_SpinBox::GetString( )
{
  return cleanText();
}

//=================================================================================
// function : RangeStepAndValidator()
// purpose  :
//=================================================================================
void DlgRef_SpinBox::RangeStepAndValidator( double min, double max, double step, unsigned short decimals )
{
  setRange( min, max );
  setLineStep( step );
  ( ( QDoubleValidator* )validator() )->setRange( min, max, decimals ) ;
}
