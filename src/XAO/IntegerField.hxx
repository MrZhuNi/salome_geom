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

#ifndef __XAO_INTEGERFIELD_HXX__
#define __XAO_INTEGERFIELD_HXX__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Xao.hxx"
#include "Field.hxx"
#include "IntegerStep.hxx"

namespace XAO
{
    class IntegerField : public Field
    {
    public:
        IntegerField(const XAO::Dimension dimension, const int nbComponents);
        IntegerField(const std::string name, const XAO::Dimension dimension, const int nbComponents);

        virtual const XAO::Type getType() { return XAO::INTEGER; }

        IntegerStep* addStep(const int step);
        IntegerStep* addStep(const int step, const int stamp);
        IntegerStep* getStep(const int index);
    };
}

#endif /* __XAO_INTEGERFIELD_HXX__ */
