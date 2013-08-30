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
// Author : Nathalie Gore (OpenCascade), Frederic Pons (OpenCascade)

#include <sstream>
#include "Group.hxx"
#include <Utils_SALOME_Exception.hxx>

using namespace XAO;


Group::Group(const XAO::Dimension& dim, const int& nbElements)
{
    initGroup("", dim, nbElements);
}

Group::Group(const std::string& name, const XAO::Dimension& dim, const int& nbElements)
{
    initGroup(name, dim, nbElements);
}

void Group::initGroup(const std::string& name, const XAO::Dimension& dim, const int& nbElements)
{
    m_name = name;
    m_dimension = dim;
    m_count = 0;
    m_nbElements = nbElements;
}

Group::~Group()
{
}

void Group::checkElement(const int& element)
{
    if (element >= m_nbElements || element < 0)
    {
        std::ostringstream str;
        str << "IndexOutOfRange element: " << element << " >= " << m_nbElements; // TODO
        throw SALOME_Exception(str.str().c_str());
    }
}
