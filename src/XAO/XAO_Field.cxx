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

#include "XAO_Xao.hxx"
#include "XAO_Field.hxx"
#include "XAO_BooleanField.hxx"
#include "XAO_IntegerField.hxx"
#include "XAO_DoubleField.hxx"
#include "XAO_StringField.hxx"
#include "XAO_XaoUtils.hxx"

using namespace XAO;

// -------------------------------------------------------

Field::Field(const std::string& name, const XAO::Dimension& dimension,
        const int& nbElements, const int& nbComponents)
    : m_name(name), m_dimension(dimension), m_nbElements(nbElements), m_nbComponents(nbComponents)
{
    m_components.reserve(nbComponents);
    for (int i = 0; i < nbComponents; ++i)
        m_components.push_back("");
}

Field::~Field()
{
    for (int i = 0; i < m_steps.size(); ++i)
        delete m_steps[i];
}

Field* Field::createField(const XAO::Type& type, const XAO::Dimension& dimension,
        const int& nbElements, const int& nbComponents)
throw (XAO_Exception)
{
    return createField(type, "", dimension, nbElements, nbComponents);
}

Field* Field::createField(const XAO::Type& type, const std::string& name, const XAO::Dimension& dimension,
        const int& nbElements, const int& nbComponents)
throw (XAO_Exception)
{
    if (type == XAO::BOOLEAN)
        return new BooleanField(name, dimension, nbElements, nbComponents);
    if (type == XAO::INTEGER)
        return new IntegerField(name, dimension, nbElements, nbComponents);
    if (type == XAO::DOUBLE)
        return new DoubleField(name, dimension, nbElements, nbComponents);
    if (type == XAO::STRING)
        return new StringField(name, dimension, nbElements, nbComponents);

    throw XAO_Exception(MsgBuilder() << "Bad Type: " << type);
}

const std::string Field::getComponentName(const int& index)
throw (XAO_Exception)
{
    checkComponent(index);
    return m_components[index];
}

void Field::setComponentName(const int& index, const std::string& name)
throw (XAO_Exception)
{
    checkComponent(index);
    m_components[index] = name;
}

bool Field::removeStep(Step* step)
{
    std::vector<Step*>::iterator it = m_steps.begin();
    for (; it != m_steps.end(); ++it)
    {
        Step* current = *it;
        if (step == current)
        {
            m_steps.erase(it);
            return true;
        }
    }

    return false;
}

bool Field::hasStep(const int& step)
{
    std::vector<Step*>::iterator it = m_steps.begin();
    for (; it != m_steps.end(); ++it)
    {
        Step* current = *it;
        if (current->getStep() == step)
            return true;
    }

    return false;
}

void Field::checkComponent(const int& component)
throw (XAO_Exception)
{
    if (component < m_nbComponents && component >= 0)
        return;

    throw XAO_Exception(MsgBuilder() << "Step index is out of range [0, "
                                     << m_nbComponents << "]: " << component);
}

void Field::checkStepIndex(const int& step)
throw (XAO_Exception)
{
    if (step < m_steps.size() && step >= 0)
        return;

    throw XAO_Exception(MsgBuilder() << "Step index is out of range [0, "
                                     << m_steps.size() << "]: " << step);
}
