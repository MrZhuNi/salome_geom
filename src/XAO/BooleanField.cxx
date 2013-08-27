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

#include "BooleanField.hxx"
#include "BooleanStep.hxx"

#include <Utils_SALOME_Exception.hxx>

using namespace XAO;

BooleanField::BooleanField(const XAO::Dimension dimension, const int nbComponents)
{
    m_dimension = dimension;
    m_nbComponents = nbComponents;
}

BooleanField::BooleanField(const std::string name, const XAO::Dimension dimension, const int nbComponents)
{
    m_dimension = dimension;
    m_nbComponents = nbComponents;
    m_name = name;
}

BooleanStep* BooleanField::addStep(const int step)
{
    BooleanStep* bstep = new BooleanStep(step, m_nbElements, m_nbComponents);
    m_steps.push_back(bstep);
    return bstep;
}

BooleanStep* BooleanField::addStep(const int step, const int stamp)
{
    BooleanStep* bstep = new BooleanStep(step, stamp, m_nbElements, m_nbComponents);
    m_steps.push_back(bstep);
    return bstep;
}

BooleanStep* BooleanField::getStep(const int index)
{
    if (index < m_steps.size())
        return m_steps[index];
   throw SALOME_Exception("IndexOutOfRange");
}
