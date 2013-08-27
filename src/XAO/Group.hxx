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

#ifndef __XAO_GROUP_HXX__
#define __XAO_GROUP_HXX__

# include <iostream>
#include <string>
#include <vector>

#include "Xao.hxx"

namespace XAO
{
    /**
     * \class Group
     * Class to represent a Geometrical Group.
     */
    class Group
    {
    public:
        /**
         * Default constructor.
         */
        Group();

        /**
         * Destructor.
         */
        virtual ~Group();

        /**
         * Sets the name of the group.
         * \param name the name to set.
         */
        void setName(const char* name)
        {
            m_name = name;
        }
        /**
         * Gets the name of the group.
         * \return the name of the group.
         */
        const char *getName() const
        {
            return m_name.c_str();
        }

        /**
         * Gets the dimension of the group.
         * \return the dimension of the group.
         */
        const Dimension getDimension();

        /**
         * Sets the dimension of the group.
         * \param dim the dimension to set.
         */
        void setDimension(const Dimension dim);

        /**
         * Gets the number of elements in the group.
         * \return the number of elements.
         */
        int getCount()
        {
            return m_elements.size();
        }

        /**
         * Adds an element to the group.
         * \param value the index of the element to add in the geometric element list (vertex, face...).
         */
        void addElement(const int value)
        {
            m_elements.push_back(value);
        }

        /**
         * Gets the reference of an element.
         * \param index the index of the element.
         * \return the reference of the element.
         */
        const int getElement(const int index)
        {
            return m_elements[index];
        }

    private:
        /** The name of the group. */
        std::string m_name;
        /** The dimension of the group. */
        Dimension m_dimension;
        /** The number of elements in the group. */
        int m_count;
        /** The elements of the group. */
        std::vector<int> m_elements;
    };
}

#endif
