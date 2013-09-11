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

#ifndef __XAO_UTILS_HXX__
#define __XAO_UTILS_HXX__

#include <sstream>
#include <string>

#include "XAO_Xao.hxx"
#include "XAO_Field.hxx"

namespace XAO
{
    /**
     * \class XaoUtils
     * Utilities class to convert types.
     */
    class XaoUtils
    {
    public:
        /**
         * Converts an integer into a string.
         * \param value the integer to convert.
         * \return the string.
         */
        static const std::string intToString(const int& value);

        /**
         * Converts a string into an integer.
         * \param value the string to convert.
         * \return the integer value.
         */
        static const int stringToInt(const std::string& value);

        /**
         * Converts a double into a string.
         * @param value the double to convert.
         * @return the string.
         */
        static const std::string doubleToString(const double& value);
        /**
         * Converts a string into a double.
         * @param value the string to convert.
         * @return the double value.
         */
        static const double stringToDouble(const std::string& value);

        /**
         * Converts a boolean into a string.
         * @param value the boolean to convert.
         * @return the string.
         */
        static const std::string booleanToString(const bool& value);
        /**
         * Converts a string into a boolean.
         * @param value the string to convert.
         * @return the boolean value.
         */
        static const bool stringToBoolean(const std::string& value);

        /**
         * Converts a Dimension to string.
         * \param dimension the Dimension to convert.
         * \return the dimension as a string.
         * \throw SALOME_Exception
         */
        static const std::string dimensionToString(const XAO::Dimension& dimension);

        /**
         * Converts a string into a Dimension.
         * \param dimension the dimension as a string.
         * \return the converted Dimension.
         * \throw SALOME_Exception
         */
        static const XAO::Dimension stringToDimension(const std::string& dimension);

        /**
         * Converts a Type to string.
         * \param type the Type to convert.
         * \return the Type as a string.
         * \throw SALOME_Exception
         */
        static const std::string fieldTypeToString(const XAO::Type& type);

        /**
         * Converts a string into a Type.
         * \param type the Type as a string.
         * \return the converted Type.
         * \throw SALOME_Exception
         */
        static const XAO::Type stringToFieldType(const std::string& type);

        /**
         * Converts a Format to string.
         * \param format the Format to convert.
         * \return the Format as a string.
         * \throw SALOME_Exception
         */
        static const std::string shapeFormatToString(const XAO::Format& format);

        /**
         * Converts a string into a Format.
         * \param format the Format as a string.
         * \return the converted Format.
         * \throw SALOME_Exception
         */
        static const XAO::Format stringToShapeFormat(const std::string& format);
    };

    /**
     * @class MsgBuilder
     * MsgBuilder can be use to easily create messages.
     */
    class MsgBuilder
    {
    public:
        /** Constructor. */
        MsgBuilder() {};
        /** Destructor. */
        ~MsgBuilder() {};

        /** Stream operator. */
        template <typename T>
        MsgBuilder& operator <<(const T& t)
        {
            m_stream << t;
            return *this;
        }

        /**
         * Conversion operator to char*.
         */
        operator const char*() const   { return m_stream.str().c_str(); }

    private :
        std::stringstream m_stream;
    };

}



#endif /* __XAO_UTILS_HXX__ */
