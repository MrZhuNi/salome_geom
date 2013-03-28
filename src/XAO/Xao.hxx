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
#include <list>

namespace XAO
{
  enum Kind{
    VERTEX,
    EDGE,
    FACE,
    SOLID
  };
  
  class Geometry;
  class Group;
  class Field;
  
  class Xao
  {
  public:
    Xao();
    Xao(const char *author, const char *version);
    ~Xao();

    void setAuthor(const char *author) { _myAuthor=author; }
    const char *getAuthor() const { return _myAuthor.c_str(); }
    
    void setVersion(const char *version) { _myVersion=version; }
    const char *getVersion() const { return _myVersion.c_str(); }

    void setGeometry(Geometry *myGeometry) { _myGeometry=myGeometry; }
    const Geometry *getGeometry() const { return _myGeometry; }

    int getNbGroups() { return _myGroups.size(); }
    void addGroup(Group *myGroup) { _myGroups.push_back(myGroup); }
    void removeGroup(Group *myGroup) { _myGroups.remove(myGroup); }

    int getNbFields() { return _myNbFields; }
    
    bool Export(const char* fileName);


  private:
    std::string                  _myAuthor;
    std::string                  _myVersion;
    Geometry                    *_myGeometry;
    int                          _myNbGroups;
    std::list<Group *>           _myGroups;
    int                          _myNbFields;
    //std::list<Field *>           _myFields;
  };
}

#endif
