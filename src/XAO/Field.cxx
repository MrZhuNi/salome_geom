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

#include <string>
#include <iostream>
#include "Xao.hxx"
#include "Field.hxx"
#include "BooleanField.hxx"
#include "IntegerField.hxx"
#include "DoubleField.hxx"
#include "StringField.hxx"

#include <Utils_SALOME_Exception.hxx>

using namespace XAO;

// -------------------------------------------------------

Field* Field::createField(const XAO::Type type, const XAO::Dimension dimension, const int nbComponents)
{
    return createField(type, "", dimension, nbComponents);
}

Field* Field::createField(const XAO::Type type, const std::string name, const XAO::Dimension dimension, const int nbComponents)
{
    if (type == XAO::BOOLEAN)
        return new BooleanField(name, dimension, nbComponents);
    if (type == XAO::INTEGER)
        return new IntegerField(name, dimension, nbComponents);
    if (type == XAO::DOUBLE)
        return new DoubleField(name, dimension, nbComponents);
    if (type == XAO::STRING)
        return new StringField(name, dimension, nbComponents);

    throw SALOME_Exception("Bad Type");
}

const std::string Field::getComponentName(const int index)
{
    if (index < m_components.size())
        return m_components[index];

    // TODO: throw
    return "";
}

void Field::setComponentName(const int index, const std::string name)
{
    if (index < m_components.size())
    {
        m_components[index] = name;
    }
    // TODO: throw
}
