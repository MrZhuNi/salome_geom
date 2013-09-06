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

#ifndef __XAO_DOUBLEFIELD_HXX__
#define __XAO_DOUBLEFIELD_HXX__

#include <string>

#include "XAO_Xao.hxx"
#include "XAO_Field.hxx"
#include "XAO_DoubleStep.hxx"

namespace XAO
{
    class DoubleField : public Field
    {
    public:
        DoubleField(const std::string& name, const XAO::Dimension& dimension, const int& nbElements, const int& nbComponents);

        virtual const XAO::Type getType() { return XAO::DOUBLE; }

        virtual Step* addNewStep(const int& step);
        DoubleStep* addStep(const int& step);
        DoubleStep* addStep(const int& step, const int& stamp);
        DoubleStep* getStep(const int& index);
    };
}

#endif /* __XAO_DOUBLEFIELD_HXX__ */
