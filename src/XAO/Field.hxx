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
// Author : Frederic Pons (OpenCascade)

#ifndef __XAO_FIELD_HXX__
#define __XAO_FIELD_HXX__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Xao.hxx"
#include "Step.hxx"

namespace XAO
{
    /**
     * @class Field
     * A geometrical Field.
     */
    class Field
    {
    protected:
        Field();

    public:
        static Field* createField(const XAO::Type type, const XAO::Dimension dimension, const int nbComponents);
        static Field* createField(const XAO::Type type, const std::string name, const XAO::Dimension dimension, const int nbComponents);

        virtual ~Field();

        /**
         * Gets the Type of the field.
         * @return the Type of the field.
         */
        virtual const XAO::Type getType() = 0;

        /**
         * Gets the name of the Field.
         * @return the name of the Field.
         */
        const std::string getName()
        {
            return m_name;
        }

        /**
         * Sets the name of the Field.
         * @param name the name to set.
         */
        void setName(std::string name)
        {
            m_name = name;
        }

        /**
         * Gets the Dimension of the Field.
         * @return the Dimension of the Field.
         */
        const XAO::Dimension getDimension()
        {
            return m_dimension;
        }

        int countElements();

        /**
         * Gets the number of components.
         * @return the number of components.
         */
        const int countComponents()
        {
            return m_components.size();
        }

        const int countValues();

        /**
         * Gets the number of the steps.
         * @return the number of steps.
         */
        const int countSteps()
        {
            return m_steps.size();
        }

        /**
         * Gets the name of a component.
         * @param index the index of the component to get.
         * @return the name of the component for the given index.
         */
        const std::string getComponentName(const int index);

        /**
         * Sets the name of a component.
         * @param componentIndex the index of the component to set.
         * @param name the name to set.
         */
        void setComponentName(const int componentIndex, const std::string name);

        /**
         * Remove a step.
         * @param step the step to remove.
         * @return
         */
        bool removeStep(Step* step);

    protected:
        /** The name of the Field. */
        std::string m_name;
        /** The dimension of the Field. */
        XAO::Dimension m_dimension;

        /** The number of components. */
        int m_nbComponents;
        /** The components of the field. */
        std::vector<std::string> m_components;
        /** The steps. */
        std::map<int, Step* > m_steps;

        int m_nbElements;
    };
}

#endif
