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
// Author : Nathalie Gore (OpenCascade)

#ifndef __XAO_FIELD_HXX__
#define __XAO_FIELD_HXX__

#include <string>
#include <vector>
#include <map>

#include "Xao.hxx"

namespace XAO
{
    enum FieldType
    {
        FIELD_BOOLEAN = 0,
        FIELD_INTEGER = 1,
        FIELD_DOUBLE = 2,
        FIELD_STRING = 3
    };

    enum FieldDimension
    {
        FIELD_VERTEX = 0,
        FIELD_EDGE = 1,
        FIELD_FACE = 2,
        FIELD_SOLID = 3,
        FIELD_WHOLE = -1
    };

    template <typename T>
    class Step
    {
    public:
    private:
        int m_number;
        int m_stamp;
        std::map<int, T> m_values;

    };

    class IField
    {
    public:
        virtual const char* getName() const = 0;
        virtual void setName(const char* name) = 0;
        virtual const FieldType getType() = 0;
        virtual const FieldDimension getDimension() = 0;
        virtual void setComponentName(const int index, const char* name) = 0;
        virtual const int countComponents() = 0;
        virtual const int getStepCount() = 0;
    };

    template <typename T>
    class Field : IField
    {
    public:
        Field(const FieldDimension dim, const int nbComponents);
        Field(const char* name, const FieldDimension dim, const int nbComponents);
        virtual ~Field();

        const char* getName() const
        {
            return m_name.c_str();
        }
        void setName(const char* name)
        {
            m_name = name;
        }

        virtual const FieldType getType();

        const FieldDimension getDimension()
        {
            return m_dimension;
        }

        void setComponentName(const int index, const char* name);
        const int countComponents()
        {
            return m_components.size();
        }

        const int getStepCount()
        {
            return m_steps.size();
        }

    private:
        std::string m_name;
        FieldDimension m_dimension;
        FieldType m_type;

        std::vector<std::string> m_components;
        std::list< Step<T>* > m_steps;
    };

    class BooleanField : Field<bool>
    {
    public:
        BooleanField(const FieldDimension dim, const int nbComponents);
        BooleanField(const char* name, const FieldDimension dim, const int nbComponents);

        virtual const FieldType getType()
        {
            return FIELD_BOOLEAN;
        }
    };
}

#endif
