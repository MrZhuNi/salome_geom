// Copyright (C) 2007-2012  CEA/DEN, EDF R&D
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

#ifndef __XAO_GROUP_HXX__
#define __XAO_GROUP_HXX__

#include <string>

namespace XAO
{
  class Group
  {
  public:
    static Group *New();
    void setName(const char *name) { _myName=name; }
    const char *getName() const { return _myName.c_str(); }
    void setType(int type) { _myType=type; }
    int getType() { return _myType; }
    void setCount(int nb) { _myCount=nb; }
    int getCount() { return _myCount; }

  private:
    Group();
    ~Group();

  private:
    std::string _myName;
    int         _myType;
    int         _myCount;
    int        *_myValues;
  };
}

#endif
