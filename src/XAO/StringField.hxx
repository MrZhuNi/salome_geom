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

#ifndef __XAO_STRINGFIELD_HXX__
#define __XAO_STRINGFIELD_HXX__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Xao.hxx"
#include "Field.hxx"
#include "StringStep.hxx"

namespace XAO
{
    class StringField : public Field
    {
    public:
        StringField(const XAO::Dimension dimension, const int nbComponents);
        StringField(const std::string name, const XAO::Dimension dimension, const int nbComponents);

        virtual const XAO::Type getType() { return XAO::STRING; }

        StringStep* addStep(const int step);
        StringStep* addStep(const int step, const int stamp);
        StringStep* getStep(const int index);
    };
}

#endif /* __XAO_STRINGFIELD_HXX__ */
