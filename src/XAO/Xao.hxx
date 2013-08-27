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
        SOLID = 3, //!< SOLID
        WHOLE = -1 //!< WHOLE
    };

    enum Type
    {
        BOOLEAN = 0,
        INTEGER = 1,
        DOUBLE = 2,
        STRING = 3
    };

    class Geometry;
    class Group;
    class Field;

    /**
     * @class Xao
     * The Xao class describes the XAO format.
     */
    class Xao
    {
    public:
        /**
         * Default constructor.
         */
        Xao();
        /**
         * Constructor with author and version.
         * \param author the author of the file.
         * \param version the version of the XAO format.
         */
        Xao(const char* author, const char* version);
        /**
         * Destructor.
         */
        virtual ~Xao();

        /**
         * Gets the author of the file.
         * \return the author of the file.
         */
        const char* getAuthor()
        {
            return m_author.c_str();
        }
        /**
         * Sets the author of the file.
         * \param author the author to set.
         */
        void setAuthor(const char* author)
        {
            m_author = author;
        }

        /**
         * Gets the version of the file.
         * \return the version of the file.
         */
        const char* getVersion()
        {
            return m_version.c_str();
        }
        /**
         * Sets the version of the file.
         * \param version the version to set.
         */
        void setVersion(const char* version)
        {
            m_version = version;
        }

        //
        // Geometry
        //

        /**
         * Gets the geometry.
         * \return the geometry.
         */
        Geometry* getGeometry()
        {
            return m_geometry;
        }
        /**
         * Sets the geometry.
         * \param geometry the geometry to set.
         */
        void setGeometry(Geometry* geometry)
        {
            m_geometry = geometry;
        }

        //
        // Groups
        //

        /**
         * Gets the number of groups.
         * \return the number of groups.
         */
        int countGroups();
        /**
         * Gets a group.
         * \param index the index of the wanted group.
         * \return the group or NULL if index is bigger than the number of groups.
         */
        Group* getGroup(const int index);
        /**
         * Adds a group.
         * \param group the group to add.
         */
        void addGroup(Group* group);
        /**
         * Removes a group.
         * \param group the group to remove.
         */
        void removeGroup(Group* group);

        //
        // Fields
        //

        /**
         * Gets the number of fields.
         * \return the number of fields.
         */
        int countFields();
        /**
         * Gets a field.
         * \param index the index of the wanted field.
         * \return the field or NULL if the index is bigger than the number of fields.
         */
        Field* getField(const int index);
        /**
         * Adds a field.
         * \param field the field to add.
         */
        void addField(Field* field);
        /**
         * Removes a field.
         * \param field the field to remove.
         */
        void removeField(Field* field);

        //
        // Import / Export
        //
        /**
         * Exports this XAO object to a file.
         * \param fileName the name of the file to create.
         * \return true is the export is successful.
         */
        bool exportXAO(const char* fileName);
        /**
         * Gets the XML corresponding to this XAO.
         * \return the XML as a string.
         */
        const char* getXML();

        /**
         * Imports an XAO file into this object.
         * \param fileName the name of the file to import.
         * \return true if the import is successful.
         */
        bool importXAO(const char* fileName);
        /**
         * Sets an XML describing an XAO format to this object.
         * \param xml the XML to set.
         * \return true if the import is successful.
         */
        bool setXML(const char* xml);

    private:
        /** The author of the file. */
        std::string m_author;
        /** The version of the file. */
        std::string m_version;
        /** The geometry. */
        Geometry* m_geometry;
        /** The list of groups. */
        std::list<Group*> m_groups;
        /** The list of fields. */
        std::list<Field*> m_fields;
    };

}

#endif
