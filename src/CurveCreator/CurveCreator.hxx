// Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

// File:        CurveCreator.hxx
// Created:     Tue Jun  25 16:34:39 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_HeaderFile
#define _CurveCreator_HeaderFile


#include <deque>


namespace CurveCreator
{

  //! Dimension of the curve
  enum Dimension
  {
    Dim2d = 2,
    Dim3d = 3
  };

  //! Type of the section
  enum Type
  {
    Polyline,
    BSpline
  };

  //! Points coordinates
  typedef float TypeCoord;

  typedef std::deque<TypeCoord> Coordinates;

};

#endif
