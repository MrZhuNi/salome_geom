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


#ifndef __XAO_INTEGERSTEP_HXX__
#define __XAO_INTEGERSTEP_HXX__

#include <vector>

#include "Xao.hxx"
#include "Step.hxx"

namespace XAO
{
    class IntegerStep : public Step
    {
    public:
        IntegerStep(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

        virtual const XAO::Type getType() { return XAO::INTEGER; }

        std::vector<int> getValues();
        std::vector<int> getElement(const int& element);
        std::vector<int> getComponent(const int& component);

        const int getValue(const int& element, const int& component);

        void setValues(const std::vector<int>& values);
        void setElements(const int& element, const std::vector<int>& elements);
        void setComponents(const int& element, const std::vector<int>& components);
        void setValue(const int& element, const int& component, const int& value);

        virtual const std::string getStringValue(const int& element, const int& component);
        virtual void setStringValue(const int& element, const int& component, const std::string& value);

    private:
        std::vector< std::vector<int> > m_values;
    };
}


#endif /* __XAO_INTEGERSTEP_HXX__ */
