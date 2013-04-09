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
// Author : Nathalie Gore (OpenCascade), Frederic Pons (OpenCascade)

#ifndef __XAO_XAO_HXX__
#define __XAO_XAO_HXX__

#include <string>
#include <list>
#include <libxml/parser.h>

namespace XAO
{
  enum Kind
  {
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
    Xao(const char* author, const char* version);
    ~Xao();

    void setAuthor(const char* author) { m_author = author; }
    const char* getAuthor() { return m_author.c_str(); }

    void setVersion(const char* version) { m_version = version; }
    const char* getVersion() { return m_version.c_str(); }

    void setGeometry(Geometry* geometry) { m_geometry = geometry; }
    Geometry* getGeometry() { return m_geometry; }

    int countGroups() { return m_groups.size(); }
    Group* getGroup(const int index);
    void addGroup(Group* group) { m_groups.push_back(group); }
    void removeGroup(Group* group) { m_groups.remove(group); }

    bool exportToFile(const char* fileName);
    const char* getXML();

    bool importFromFile(const char* fileName);
    bool setXML(const char* xml);


  private:
    std::string m_author;
    std::string m_version;
    Geometry* m_geometry;
    int m_nbGroups;
    std::list<Group*> m_groups;
    //int m_nbFields;
    //std::list<Field*> m_fields;

    xmlDocPtr exportXMLDoc();
    void exportGeometry(xmlDocPtr doc, xmlNodePtr xao);
    void exportGroups(xmlNodePtr xao);

    void parseXMLDoc(xmlDocPtr doc);
    void parseXaoNode(xmlDocPtr doc, xmlNodePtr xaoNode);
    void parseGeometryNode(xmlDocPtr doc, xmlNodePtr geometryNode);
    void parseShapeNode(xmlDocPtr doc, xmlNodePtr shapeNode);
    void parseTopologyNode(xmlNodePtr topologyNode);
    void parseVerticesNode(xmlNodePtr verticesNode);
    void parseEdgesNode(xmlNodePtr edgesNode);
    void parseFacesNode(xmlNodePtr facesNode);
    void parseSolidsNode(xmlNodePtr solidsNode);
    void parseGroupsNode(xmlNodePtr groupsNode);
    void parseGroupNode(xmlNodePtr groupNode);

  };

}

#endif
