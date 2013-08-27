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


#ifndef __XAO_BOOLEANSTEP_HXX__
#define __XAO_BOOLEANSTEP_HXX__

#include <map>
#include <vector>

#include "Xao.hxx"
#include "Step.hxx"

namespace XAO
{
    class BooleanStep : public Step
    {
    public:
        BooleanStep(const int& nbElements, const int& nbComponents);
        BooleanStep(const int& step, const int& nbElements, const int& nbComponents);
        BooleanStep(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

        virtual const XAO::Type getType() { return XAO::BOOLEAN; }

        /**
         * Gets all the values in a vector by elements and by components.
         * @return a vector containing all the values.
         */
        std::vector<bool> getValues();

        /**
         * Gets all the values for an element.
         * @param element the index of the element to get.
         * @return a vector containing all the values for the given element.
         */
        std::vector<bool> getElement(const int& element);

        /**
         * Gets all the values for a component.
         * @param component the index of the component to get.
         * @return a vector containing all the values for the given component.
         */
        std::vector<bool> getComponent(const int& component);

        /**
         * Gets a value for an element and a component.
         * @param element the index of the element.
         * @param component the index of the component.
         * @return the value.
         */
        const bool getValue(const int& element, const int& component);

        void setValues(const std::vector<bool>& values);
        void setElements(const int& element, const std::vector<bool>& elements);
        void setComponents(const int& component, const std::vector<bool>& components);
        void setValue(const int& element, const int& component, const bool& value);

    private:
        void Init(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

    private:
        std::vector< std::vector<bool> > m_values;
    };
}


#endif /* __XAO_BOOLEANSTEP_HXX__ */
