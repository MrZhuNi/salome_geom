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
    /**
     * @enum Dimension
     */
    enum Dimension
    {
        VERTEX = 0,//!< VERTEX
        EDGE = 1,  //!< EDGE
        FACE = 2,  //!< FACE
        SOLID = 3  //!< SOLID
    };

    class Geometry;
    class Group;
    class IField;

    /**
     * @class Xao
     * The Xao class describes the XAO format.
     */
    class Xao
    {
    public:
        Xao();
        Xao(const char* author, const char* version);
        ~Xao();

        const char* getAuthor()
        {
            return m_author.c_str();
        }
        void setAuthor(const char* author)
        {
            m_author = author;
        }

        const char* getVersion()
        {
            return m_version.c_str();
        }
        void setVersion(const char* version)
        {
            m_version = version;
        }

        //
        // Geometry
        //

        Geometry* getGeometry()
        {
            return m_geometry;
        }
        void setGeometry(Geometry* geometry)
        {
            m_geometry = geometry;
        }

        //
        // Groups
        //

        int countGroups();
        Group* getGroup(const int index);
        void addGroup(Group* group);
        void removeGroup(Group* group);

        //
        // Fields
        //

        int countFields();
        IField* getField(const int index);
        void addField(IField* field);
        void removeField(IField* field);

        //
        // Import / Export
        //
        bool exportXAO(const char* fileName);
        const char* getXML();

        bool importXAO(const char* fileName);
        bool setXML(const char* xml);

    private:
        std::string m_author;
        std::string m_version;
        Geometry* m_geometry;
        std::list<Group*> m_groups;
        std::list<IField*> m_fields;

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
