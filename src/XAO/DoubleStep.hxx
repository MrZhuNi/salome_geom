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


#ifndef __XAO_DOUBLESTEP_HXX__
#define __XAO_DOUBLESTEP_HXX__

#include <vector>

#include "Xao.hxx"
#include "Step.hxx"

namespace XAO
{
    class DoubleStep : public Step
    {
    public:
        DoubleStep(const int& nbElements, const int& nbComponents);
        DoubleStep(const int& step, const int& nbElements, const int& nbComponents);
        DoubleStep(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

        virtual const XAO::Type getType() { return XAO::DOUBLE; }

        std::vector<double> getValues();
        std::vector<double> getElement(const int& element);
        std::vector<double> getComponent(const int& component);

        const double getValue(const int& element, const int& component);

        void setValues(const std::vector<double>& values);
        void setElements(const int& element, const std::vector<double>& elements);
        void setComponents(const int& component, const std::vector<double>& components);
        void setValue(const int& element, const int& component, const double& value);

    private:
        void Init(const int& step, const int& stamp, const int& nbElements, const int& nbComponents);

    private:
        std::vector< std::vector<double> > m_values;
    };
}


#endif /* __XAO_DOUBLESTEP_HXX__ */
