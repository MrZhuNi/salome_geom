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

#ifndef __XAO_GEOMETRICELEMENT_HXX__
#define __XAO_GEOMETRICELEMENT_HXX__

#include <string>
#include <map>



namespace XAO
{
    class GeometricElement
    {
    public:
        GeometricElement();
        GeometricElement(const char* name, const char* reference);
        ~GeometricElement();

        const char* getName()
        {
            return m_name.c_str();
        }

        void setName(const char* name)
        {
            m_name = name;
        }

        const char* getReference()
        {
            return m_reference.c_str();
        }

        void setReference(const char* reference)
        {
            m_reference = reference;
        }

    private:
        std::string m_name;
        std::string m_reference;
    };

    class GeometricElementList
    {
    public:
        GeometricElementList();
        GeometricElementList(const int nb);
        ~GeometricElementList() {}

        int getSize() { return m_count; }
        void setSize(const int nb);

        void setElement(const int index, const char* name, const char* reference);
        const char* getName(const int index);
        void setName(const int index, const char* name);
        const char* getReference(const int index);
        void setReference(const int index, const char* reference);

    private:
        int m_count;
        std::map<int, GeometricElement> m_elements;
    };
}

#endif /* __XAO_GEOMETRICELEMENT_HXX__ */
