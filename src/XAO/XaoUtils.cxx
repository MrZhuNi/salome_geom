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
#include "XaoUtils.hxx"


using namespace XAO;

const char* XaoUtils::intToString(const int value)
{
    std::stringstream str;
    str << value;
    return str.str().c_str();
}

const char* XaoUtils::dimensionToString(const Dimension dimension)
{
    if (dimension == VERTEX)
        return "vertex";
    if (dimension == EDGE)
        return "edge";
    if (dimension == FACE)
        return "face";
    if (dimension == SOLID)
        return "solid";
    throw SALOME_Exception("Bad dimension");
}

const Dimension XaoUtils::stringToDimension(const char* dimension)
{
    if (strcmp(dimension, "vertex") == 0)
        return VERTEX;
    if (strcmp(dimension, "edge") == 0)
        return EDGE;
    if (strcmp(dimension, "face") == 0)
        return FACE;
    if (strcmp(dimension, "solid") == 0)
        return SOLID;
    throw SALOME_Exception("Bad dimension");
}
