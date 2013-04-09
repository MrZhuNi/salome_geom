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

#ifndef __XAO_XAO_HXX__
#define __XAO_XAO_HXX__

#include <string>

namespace XAO
{
  typedef enum
  {
    VERTEX = 0,
    EDGE = 1,
    FACE = 2,
    SOLID = 3
  } FieldDimension;

  typedef enum
  {
    BOOLEAN = 0,
    INTEGER = 1,
    DOUBLE = 2,
    STRING = 3
  } FieldType;

  class Field
  {
  public:
    static Field *New();
    void setName(const char *name) { _myName=name; }
    const char *getName() const { return _myName.c_str(); }
    void setDimension(int nb) { _myDimension=nb; }
    int getDimension() { return _myDimension; }
    void setType(int type) { _myType=type; }
    int getType() { return _myType; }
    void setValuesCount(int nb) { _myValuesCount=nb; }
    int getValuesCount() { return _myValuesCount; }
    void setComponentCount(int nb) { _myComponentCount=nb; }
    int getComponentCount() { return _myComponentCount; }
    void setStepCount(int nb) { _myStepCount=nb; }
    int getStepCount() { return _myStepCount; }

  private:
    Field();
    ~Field();

  private:
    std::string   _myName;
    int           _myDimension;
    int           _myType;
    int           _myValuesCount;
    int           _myComponentCount;
    std::string  *_myComponentNames;
    int           _myStepCount;
    int          *_mySteps;
    double       *_myStamps;
    std::string **_myValues;
  };
}

#endif
