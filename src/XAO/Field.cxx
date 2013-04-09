// Copyright (C) 2013  CEA/DEN, EDF R&D
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author : Nathalie Gore (OpenCascade)

#include "Field.hxx"

using namespace XAO;

Field::Field()
{
  _myName = "";
  _myDimension = -1;
  _myType = -1;
  _myValuesCount = 0;
  _myComponentCount = 0;
  _myComponentNames = NULL;
  _myStepCount = 0;
  _mySteps = NULL;
  _myStamps = NULL;
  _myValues = NULL;
}

Field::~Field()
{
}

Field *Field::New()
{
  return new Field;
}


