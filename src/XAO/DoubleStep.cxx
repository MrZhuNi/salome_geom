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

#include "DoubleStep.hxx"
#include <Utils_SALOME_Exception.hxx>
#include "XaoUtils.hxx"

using namespace XAO;

DoubleStep::DoubleStep(const int& nbElements, const int& nbComponents)
{
    init(0, 0, nbElements, nbComponents);
}

DoubleStep::DoubleStep(const int& step, const int& nbElements, const int& nbComponents)
{
    init(step, 0, nbElements, nbComponents);
}

DoubleStep::DoubleStep(const int& step, const int& stamp, const int& nbElements, const int& nbComponents)
{
    init(step, stamp, nbElements, nbComponents);
}

void DoubleStep::init(const int& step, const int& stamp, const int& nbElements, const int& nbComponents)
{
    m_nbElements = nbElements;
    m_nbComponents = nbComponents;
    m_step = step;
    m_stamp = stamp;

    m_values.reserve(m_nbElements);
    for (int i = 0; i < m_nbElements; ++i)
    {
        std::vector<double> row;
        row.reserve(m_nbComponents);
        for (int j = 0; j < m_nbComponents; ++j)
            row.push_back(0);
        m_values.push_back(row);
    }
}

std::vector<double> DoubleStep::getValues()
{
    std::vector<double> result;
    result.reserve(m_nbElements * m_nbComponents);

    std::vector< std::vector<double>  >::iterator it;
    for (it = m_values.begin(); it != m_values.end(); ++it)
    {
        std::vector<double> eltValues = *it;
        result.insert(result.end(), eltValues.begin(), eltValues.end());
    }

    return result;
}

std::vector<double> DoubleStep::getElement(const int& element)
{
    checkElement(element);

    std::vector<double> result(m_values[element]);
    return result;
}

std::vector<double> DoubleStep::getComponent(const int& component)
{
    checkComponent(component);

    std::vector<double> result;
    result.reserve(m_nbElements);

    std::vector< std::vector<double>  >::iterator it;
    for (it = m_values.begin(); it != m_values.end(); ++it)
    {
        std::vector<double> eltValues = *it;
        result.push_back(eltValues[component]);
    }

    return result;
}

const double DoubleStep::getValue(const int& element, const int& component)
{
    checkElement(element);
    checkComponent(component);

    return m_values[element][component];
}

const std::string DoubleStep::getStringValue(const int& element, const int& component)
{
    return XaoUtils::doubleToString(getValue(element, component));
}

void DoubleStep::setValues(const std::vector<double>& values)
{
    if (values.size() != m_nbComponents * m_nbElements)
        throw SALOME_Exception("bad size"); // TODO

    for (int i = 0; i < m_nbElements; ++i)
    {
        for (int j = 0; j < m_nbComponents; ++j)
        {
            m_values[i][j] = values[i * m_nbComponents + j];
        }
    }
}

void DoubleStep::setElements(const int& element, const std::vector<double>& elements)
{
    checkElement(element);
    if (elements.size() != m_nbComponents)
        throw SALOME_Exception("bad size"); // TODO

    for (int i = 0; i < m_nbComponents; ++i)
        m_values[element][i] = elements[i];
}

void DoubleStep::setComponents(const int& component, const std::vector<double>& components)
{
    checkElement(component);
    if (components.size() != m_nbElements)
        throw SALOME_Exception("bad size"); // TODO

    for (int i = 0; i < m_nbElements; ++i)
        m_values[i][component] = components[i];
}

void DoubleStep::setValue(const int& element, const int& component, const double& value)
{
    checkElement(element);
    checkComponent(component);

    m_values[element][component] = value;
}

void DoubleStep::setStringValue(const int& element, const int& component, const std::string& value)
{
    setValue(element, component, XaoUtils::stringToDouble(value));
}
