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


#ifndef __XAO_STEP_HXX__
#define __XAO_STEP_HXX__

#include "Xao.hxx"

namespace XAO
{
    class Step
    {
    protected:
        /** Default constructor. */
        Step() {}

    public:
        /**
         * Creates a step.
         * @param type the type of the values for the step.
         * @param step the index of the step.
         * @param stamp the stamp of the step.
         * @param nbElements the number of elements in the step.
         * @param nbComponents the number of components in the step.
         * @return
         */
        static Step* createStep(const XAO::Type& type, const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

        /**
         * Destructor.
         */
        virtual ~Step() {}

        /**
         * Gets the type of the step.
         * @return
         */
        virtual const XAO::Type getType() = 0;

        /**
         * Gets the step index.
         * @return the index of the step.
         */
        const int getStep() { return m_step; }

        /**
         * Sets the number of the step.
         * @param step the index to set.
         */
        void setStep(const int& step) { m_step = step; }

        /**
         * Gets the stamp of the index.
         * @return the stamp of the index.
         */
        const int getStamp() { return m_stamp; }

        /**
         * Sets the stamp of the index.
         * @param stamp the stamp to set.
         */
        void setStamp(const int& stamp) { m_stamp = stamp; }

        /**
         * Gets the number of components of the step.
         * @return the number of components.
         */
        const int countComponents() { return m_nbComponents; }

        /**
         * Gets the number of elements for the step.
         * @return the number of elements.
         */
        const int countElements() { return m_nbElements; }

        /**
         * Gets the number of values for the step.
         * @return the number of values.
         */
        const int countValues() { return m_nbElements * m_nbComponents; }

        /**
         * Gets a value as a string.
         * @param element the index of the element.
         * @param component the index of the component.
         * @return the value as a string.
         */
        virtual const std::string getStringValue(const int& element, const int& component) = 0;

        /**
         * Sets a value as a string
         * @param element the index of the element.
         * @param component the index of the component.
         * @param value the string value.
         * @throw SALOME_Exception if the value is not valid.
         */
        virtual void setStringValue(const int& element, const int& component, const std::string& value) = 0;

    protected:
        void checkElement(const int& element);
        void checkComponent(const int& component);

    protected:
        /** the index of the step. */
        int m_step;
        /** The stamp of the step. */
        int m_stamp;
        /** The number of components. */
        int m_nbComponents;
        /** The number of elements. */
        int m_nbElements;
    };
}


#endif /* __XAO_STEP_HXX__ */
