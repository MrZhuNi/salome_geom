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

#include <cstring>
#include <sstream>
#include <iosfwd>
#include <Utils_SALOME_Exception.hxx>

#include "Xao.hxx"
#include "Field.hxx"
#include "XaoUtils.hxx"


using namespace XAO;

/*const char* XaoUtils::intToString(const int value)
{
    std::ostringstream str;
    str << value;
    return str.str().c_str();
}*/
const std::string XaoUtils::intToString(const int value)
{
    std::ostringstream str;
    str << value;
    return str.str();
}

const int XaoUtils::stringToInt(const std::string value)
{
    int res;
    std::istringstream convert(value);
    if ( !(convert >> res) )
        res = 0;
    return res;
}

const std::string XaoUtils::doubleToString(const double value)
{
    std::ostringstream str;
    str << value;
    return str.str();
}

const char* XaoUtils::dimensionToString(const XAO::Dimension dimension)
{
    if (dimension == VERTEX)
        return "vertex";
    if (dimension == EDGE)
        return "edge";
    if (dimension == FACE)
        return "face";
    if (dimension == SOLID)
        return "solid";
    if (dimension == WHOLE)
        return "whole";
    throw SALOME_Exception("Bad dimension");
}

const XAO::Dimension XaoUtils::stringToDimension(const char* dimension)
{
    if (strcmp(dimension, "vertex") == 0)
        return VERTEX;
    if (strcmp(dimension, "edge") == 0)
        return EDGE;
    if (strcmp(dimension, "face") == 0)
        return FACE;
    if (strcmp(dimension, "solid") == 0)
        return SOLID;
    if (strcmp(dimension, "whole") == 0)
        return WHOLE;
    throw SALOME_Exception("Bad dimension");
}

const char* XaoUtils::fieldTypeToString(const XAO::Type type)
{
    if (type == BOOLEAN)
        return "boolean";
    if (type == INTEGER)
        return "integer";
    if (type == DOUBLE)
        return "double";
    if (type == STRING)
        return "string";
    throw SALOME_Exception("Bad type");
}

const XAO::Type XaoUtils::stringToFieldType(const char* type)
{
    if (strcmp(type, "boolean") == 0)
        return BOOLEAN;
    if (strcmp(type, "integer") == 0)
        return INTEGER;
    if (strcmp(type, "double") == 0)
        return DOUBLE;
    if (strcmp(type, "string") == 0)
        return STRING;
    throw SALOME_Exception("Bad type");
}
