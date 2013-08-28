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

#include "Xao.hxx"
#include "Field.hxx"

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
        //static const char* intToString(const int value);
        static const std::string intToString(const int& value);
        static const int stringToInt(const std::string& value);

        static const std::string doubleToString(const double& value);
        static const double stringToDouble(const std::string& value);

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
    };
}



#endif /* __XAO_UTILS_HXX__ */
