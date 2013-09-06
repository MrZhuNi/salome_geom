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


#ifndef __XAO_STRINGSTEP_HXX__
#define __XAO_STRINGSTEP_HXX__

#include <string>
#include <vector>

#include "XAO_Xao.hxx"
#include "XAO_Step.hxx"

namespace XAO
{
    class StringStep : public Step
    {
    public:
        StringStep(const int& nbElements, const int& nbComponents);
        StringStep(const int& step, const int& nbElements, const int& nbComponents);
        StringStep(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

        virtual const XAO::Type getType() { return XAO::STRING; }

        std::vector<std::string> getValues();
        std::vector<std::string> getElement(const int& element);
        std::vector<std::string> getComponent(const int& component);

        const std::string getValue(const int& element, const int& component);

        void setValues(const std::vector<std::string>& values);
        void setElements(const int& element, const std::vector<std::string>& elements);
        void setComponents(const int& component, const std::vector<std::string>& components);
        void setValue(const int& element, const int& component, const std::string& value);

        virtual const std::string getStringValue(const int& element, const int& component);
        virtual void setStringValue(const int& element, const int& component, const std::string& value);

    private:
        std::vector< std::vector<std::string> > m_values;
    };
}


#endif /* __XAO_STRINGSTEP_HXX__ */
