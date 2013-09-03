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

#include "Xao.hxx"
#include "XaoUtils.hxx"
#include "Step.hxx"
#include "BooleanStep.hxx"
#include "IntegerStep.hxx"
#include "DoubleStep.hxx"
#include "StringStep.hxx"

using namespace XAO;

Step* Step::createStep(const XAO::Type& type, const int& step, const int& stamp, const int& nbElements, const int& nbComponents)
{
    if (type == XAO::BOOLEAN)
        return new BooleanStep(step, stamp, nbElements, nbComponents);
    if (type == XAO::INTEGER)
        return new IntegerStep(step, stamp, nbElements, nbComponents);
    if (type == XAO::DOUBLE)
        return new DoubleStep(step, stamp, nbElements, nbComponents);
    if (type == XAO::STRING)
        return new StringStep(step, stamp, nbElements, nbComponents);

    throw SALOME_Exception("Unknown Type");
}

void Step::checkElementIndex(const int& element)
{
    if (element < m_nbElements && element >= 0)
        return;

    throw SALOME_Exception(MsgBuilder() << "Element index is out of range [0, "
                                        << m_nbElements-1 << "] : " << element);
}

void Step::checkComponentIndex(const int& component)
{
    if (component < m_nbComponents && component >= 0)
        return;

    throw SALOME_Exception(MsgBuilder() << "Component index is out of range [0, "
                                        << m_nbComponents-1 << "] : " << component);
}

void Step::checkNbElements(const int& nbElements)
{
    if (nbElements == m_nbElements)
        return;

    throw SALOME_Exception(MsgBuilder() << "Invalid number of elements:" << nbElements
                                        << ", expected " << m_nbElements);
}

void Step::checkNbComponents(const int& nbComponents)
{
    if (nbComponents == m_nbComponents)
        return;

    throw SALOME_Exception(MsgBuilder() << "Invalid number of components:" << nbComponents
                                        << ", expected " << m_nbComponents);
}

void Step::checkNbValues(const int& nbValues)
{
    if (nbValues == m_nbElements * m_nbComponents)
        return;

    throw SALOME_Exception(MsgBuilder() << "Invalid number of values:" << nbValues
                                        << ", expected " << m_nbElements * m_nbComponents);
}
