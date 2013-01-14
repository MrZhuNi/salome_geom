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

#ifndef __XAO_XAO_HXX__
#define __XAO_XAO_HXX__

#include <string>

namespace XAO
{
  class Geometry;
  class Group;
  class Field;
  
  class Xao
  {
  public:
    static Xao *New();
    void setAuthor(const char *author) { _myAuthor=author; }
    const char *getAuthor() const { return _myAuthor.c_str(); }
    void setVersion(const char *version) { _myVersion=version; }
    const char *getVersion() const { return _myVersion.c_str(); }
    void setNbGroups(int nb) { _myNbGroups=nb; }
    int getNbGroups() { return _myNbGroups; }
    void setNbFields(int nb) { _myNbFields=nb; }
    int getNbFields() { return _myNbFields; }

  private:
    Xao();
    ~Xao();

  private:
    Geometry    *_myGeom;
    int          _myNbGroups;
    Group       *_myGroups;
    int          _myNbFields;
    Field       *_myFields;
    std::string  _myAuthor;
    std::string  _myVersion;
  };
}

#endif
