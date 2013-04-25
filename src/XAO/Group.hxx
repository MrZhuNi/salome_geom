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

#ifndef __XAO_GROUP_HXX__
#define __XAO_GROUP_HXX__

# include <iostream>
#include <string>
#include <vector>

#include "Xao.hxx"

namespace XAO
{
    class Group
    {
    public:
        Group();
        ~Group();

        void setName(const char* name)
        {
            m_name = name;
        }
        const char *getName() const
        {
            return m_name.c_str();
        }

        const Dimension getDimension();
        void setDimension(const Dimension dim);

        int getCount()
        {
            return m_elements.size();
        }

        void addElement(const int value)
        {
            m_elements.push_back(value);
        }

        const int getElement(const int index)
        {
            return m_elements[index];
        }

    private:
        std::string m_name;
        Dimension m_dimension;
        int m_count;
        std::vector<int> m_elements;
    };
}

#endif
