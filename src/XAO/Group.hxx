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

#include <string>
#include <vector>

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

        void setDimension(int dimension)
        {
            m_dimension = dimension;
        }
        int getDimension()
        {
            return m_dimension;
        }

        int getCount()
        {
            return m_elements.size();
        }

        void addElement(const char* value)
        {
            m_elements.push_back(value);
        }

        const char* getElement(const int index)
        {
            return m_elements[index].c_str();
        }

    private:
        std::string m_name;
        int m_dimension;
        int m_count;
        std::vector<std::string> m_elements;
    };
}

#endif
