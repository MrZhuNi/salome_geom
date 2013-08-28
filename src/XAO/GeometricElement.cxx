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

#include <Utils_SALOME_Exception.hxx>

#include "GeometricElement.hxx"

using namespace XAO;


GeometricElement::GeometricElement()
{
    m_name = "";
    m_reference = "";
}

GeometricElement::GeometricElement(const std::string& name, const std::string& reference)
{
    m_name = name;
    m_reference = reference;
}

GeometricElement::~GeometricElement()
{
}

bool GeometricElement::hasName()
{
    return !m_name.empty();
}

GeometricElementList::GeometricElementList()
{
    setSize(0);
}

GeometricElementList::GeometricElementList(const int& count)
{
    setSize(m_count);
}

void GeometricElementList::setSize(const int& nb)
{
    m_count = nb;
    m_elements.clear();
    for (int i = 0; i < nb; ++i)
    {
        m_elements[i] = GeometricElement();
    }
}

void GeometricElementList::setElement(const int& index, const std::string& name, const std::string& reference)
{
    if (m_count == 0 || index > m_count)
        throw SALOME_Exception("Problem with number of elements");

    m_elements[index].setName(name);
    m_elements[index].setReference(reference);
}

const std::string GeometricElementList::getName(const int& index)
{
    if (m_count == 0 || index > m_count)
        throw SALOME_Exception("Problem with number of elements");

    return m_elements[index].getName();
}

void GeometricElementList::setName(const int& index, const std::string& name)
{
    if (m_count == 0 || index > m_count)
    {
        throw SALOME_Exception("Problem with number of elements");
    }

    m_elements[index].setName(name);
}

const bool GeometricElementList::hasName(const int& index)
{
    if (m_count == 0 || index > m_count)
    {
        throw SALOME_Exception("Problem with number of elements");
    }

    return m_elements[index].hasName();
}

const std::string GeometricElementList::getReference(const int& index)
{
    if (m_count == 0 || index > m_count)
        throw SALOME_Exception("Problem with number of elements");

    return m_elements[index].getReference();
}

void GeometricElementList::setReference(const int& index, const std::string& name)
{
    if (m_count == 0 || index > m_count)
        throw SALOME_Exception("Problem with number of elements");

    m_elements[index].setReference(name);
}

const int GeometricElementList::getIndexByReference(const std::string& ref)
{
    for (int index = 0; index < m_count; ++index)
    {
        if (ref == m_elements[index].getReference())
        {
            return index;
        }
    }
    return -1;
}
