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

namespace XAO
{
    /**
     * @enum CAD
     */
    enum Format
    {
        BREP,
        STEP
    };

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

    /**
     * @enum Type
     */
    enum Type
    {
        BOOLEAN = 0,//!< BOOLEAN
        INTEGER = 1,//!< INTEGER
        DOUBLE = 2, //!< DOUBLE
        STRING = 3  //!< STRING
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
        Xao(const std::string& author, const std::string& version);
        /**
         * Destructor.
         */
        virtual ~Xao();

        /**
         * Gets the author of the file.
         * \return the author of the file.
         */
        const std::string getAuthor() const
        {
            return m_author;
        }
        /**
         * Sets the author of the file.
         * \param author the author to set.
         */
        void setAuthor(const std::string& author)
        {
            m_author = author;
        }

        /**
         * Gets the version of the file.
         * \return the version of the file.
         */
        const std::string getVersion() const
        {
            return m_version;
        }
        /**
         * Sets the version of the file.
         * \param version the version to set.
         */
        void setVersion(const std::string& version)
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
        Geometry* getGeometry() const
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
        const int countGroups() const;
        /**
         * Gets a group.
         * \param index the index of the wanted group.
         * \return the group or NULL if index is bigger than the number of groups.
         */
        Group* getGroup(const int& index);
        /**
         * Adds a group.
         * \param dim the dimension of the group.
         * \return the created group.
         */
        Group* addGroup(const XAO::Dimension& dim);
        /**
         * Adds a group.
         * \param name the name of the group.
         * \param dim the dimension of the group.
         * \return the created group.
         */
        Group* addGroup(const std::string& name, const XAO::Dimension& dim);
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
        const int countFields() const;
        /**
         * Gets a field.
         * \param index the index of the wanted field.
         * \return the field or NULL if the index is bigger than the number of fields.
         */
        Field* getField(const int& index);
        /**
         * Adds a field.
         * \param type the type of the field.
         * \param dim the dimension of the field.
         * \param nbComponents the number of components in the field.
         * \return the created field.
         */
        Field* addField(const XAO::Type& type, const XAO::Dimension& dim, const int& nbComponents);
        /**
         * Adds a field.
         * \param type the type of the field.
         * \param the name of the field.
         * \param dim the dimension of the field.
         * \param nbComponents the number of components in the field.
         * \return the created field.
         */
        Field* addField(const XAO::Type& type, const std::string& name, const XAO::Dimension& dim, const int& nbComponents);
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
        const bool exportXAO(const std::string& fileName);
        /**
         * Gets the XML corresponding to this XAO.
         * \return the XML as a string.
         */
        const std::string getXML();

        /**
         * Imports an XAO file into this object.
         * \param fileName the name of the file to import.
         * \return true if the import is successful.
         */
        const bool importXAO(const std::string& fileName);
        /**
         * Sets an XML describing an XAO format to this object.
         * \param xml the XML to set.
         * \return true if the import is successful.
         */
        const bool setXML(const std::string& xml);

    private:
        void checkGeometry() const;

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
