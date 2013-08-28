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
        Step() {}

    public:
        static Step* createStep(const XAO::Type& type, const int& nbElements, const int& nbComponents);
        static Step* createStep(const XAO::Type& type, const int& step, const int& nbElements, const int& nbComponents);
        static Step* createStep(const XAO::Type& type, const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

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

        const int countComponents() { return m_nbComponents; }

        const int countElements() { return m_nbElements; }

        const int countValues() { return m_nbElements * m_nbComponents; }

        virtual const std::string getStringValue(const int& element, const int& component) = 0;
        virtual void setStringValue(const int& element, const int& component, const std::string& value) = 0;

    protected:
        void checkElement(const int& element);
        void checkComponent(const int& component);

    protected:
        int m_step;
        int m_stamp;
        int m_nbComponents;
        int m_nbElements;
    };
}


#endif /* __XAO_STEP_HXX__ */
