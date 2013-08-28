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


const std::string XaoUtils::intToString(const int& value)
{
    std::ostringstream str;
    str << value;
    return str.str();
}

const int XaoUtils::stringToInt(const std::string& value)
{
    int res;
    std::istringstream convert(value);
    if ( !(convert >> res) )
        res = 0;
    return res;
}

const std::string XaoUtils::doubleToString(const double& value)
{
    std::ostringstream str;
    str << value;
    return str.str();
}

const double XaoUtils::stringToDouble(const std::string& value)
{
    double res;
    std::istringstream convert(value);
    if ( !(convert >> res) )
        res = 0;
    return res;
}

const std::string XaoUtils::dimensionToString(const XAO::Dimension& dimension)
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

const XAO::Dimension XaoUtils::stringToDimension(const std::string& dimension)
{
    if (dimension == "vertex")
        return VERTEX;
    if (dimension == "edge")
        return EDGE;
    if (dimension == "face")
        return FACE;
    if (dimension == "solid")
        return SOLID;
    if (dimension == "whole")
        return WHOLE;
    throw SALOME_Exception("Bad dimension");
}

const std::string XaoUtils::fieldTypeToString(const XAO::Type& type)
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

const XAO::Type XaoUtils::stringToFieldType(const std::string& type)
{
    if (type == "boolean")
        return BOOLEAN;
    if (type == "integer")
        return INTEGER;
    if (type == "double")
        return DOUBLE;
    if (type == "string")
        return STRING;
    throw SALOME_Exception("Bad type");
}
