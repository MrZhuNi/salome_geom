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
#include "XaoUtils.hxx"


using namespace XAO;

const char* XaoUtils::intToString(const int value)
{
    std::stringstream str;
    str << value;
    return str.str().c_str();
}

const char* XaoUtils::dimensionToString(const int dimension)
{
    if (dimension == 0)
        return "vertex";
    if (dimension == 1)
        return "edge";
    if (dimension == 2)
        return "face";
    if (dimension == 3)
        return "solid";
    throw SALOME_Exception("Bad dimension");
}

const int XaoUtils::stringToDimension(const char* dimension)
{
    if (strcmp(dimension, "vertex") == 0)
        return 0;
    if (strcmp(dimension, "edge") == 0)
        return 1;
    if (strcmp(dimension, "face") == 0)
        return 2;
    if (strcmp(dimension, "solid") == 0)
        return 3;
    throw SALOME_Exception("Bad dimension");
}
