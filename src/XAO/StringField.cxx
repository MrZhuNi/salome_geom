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

#include "StringField.hxx"
#include "StringStep.hxx"

#include <Utils_SALOME_Exception.hxx>

using namespace XAO;

StringField::StringField(const XAO::Dimension& dimension, const int& nbElements, const int& nbComponents)
    : Field("", dimension, nbElements, nbComponents)
{
}

StringField::StringField(const std::string& name, const XAO::Dimension& dimension, const int& nbElements, const int& nbComponents)
    : Field(name, dimension, nbElements, nbComponents)
{
}

Step* StringField::addNewStep(const int& step)
{
    return addStep(step, 0);
}

StringStep* StringField::addStep(const int& step)
{
    return addStep(step, 0);
}

StringStep* StringField::addStep(const int& step, const int& stamp)
{
    StringStep* bstep = new StringStep(step, stamp, m_nbElements, m_nbComponents);
    m_steps.push_back(bstep);
    return bstep;
}

StringStep* StringField::getStep(const int& index)
{
    checkStepIndex(index);
    return (StringStep*)m_steps[index];
}
