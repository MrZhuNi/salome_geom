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

#include "Xao.hxx"
#include "Step.hxx"
#include "BooleanStep.hxx"
#include "IntegerStep.hxx"
#include "DoubleStep.hxx"
#include "StringStep.hxx"

#include <Utils_SALOME_Exception.hxx>

using namespace XAO;

Step* Step::createStep(const XAO::Type& type, const int& nbElements, const int &nbComponents)
{
    return createStep(type, 0, 0, nbElements, nbComponents);
}

Step* Step::createStep(const XAO::Type& type, const int& step, const int& nbElements, const int& nbComponents)
{
    return createStep(type, step, 0, nbElements, nbComponents);
}

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

void Step::checkElement(const int& element)
{
    if (element >= m_nbElements)
        throw SALOME_Exception("IndexOutOfRange element"); // TODO
}

void Step::checkComponent(const int& component)
{
    if (component >= m_nbComponents)
        throw SALOME_Exception("IndexOutOfRange component"); // TODO
}
