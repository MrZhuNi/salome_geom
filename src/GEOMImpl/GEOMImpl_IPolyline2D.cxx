// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
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


#include "GEOMImpl_IPolyline2D.hxx"

#include <TColStd_HArray1OfInteger.hxx>


//=============================================================================
/*!
 *  SetCoords
 */
//=============================================================================
void GEOMImpl_IPolyline2D::SetCoords
                  (const std::list <std::list <double> > &theValue)
{
  // Compute the total number of points and fill the array of start indices.
  Standard_Integer                                i;
  const Standard_Integer                          aNbSec    = theValue.size();
  Standard_Integer                                aNbCoords = 0;
  Handle(TColStd_HArray1OfInteger)                anIndices;
  std::list <std::list <double> >::const_iterator aSecIter;

  if (aNbSec == 1) {
    // There is only one section.
    aNbCoords = theValue.front().size();
  } else {
    // Here we assume that there are more than one section.
    anIndices  = new TColStd_HArray1OfInteger(1, aNbSec - 1);
    aSecIter   = theValue.begin();
    aNbCoords += aSecIter->size();

    for (i = 1, ++aSecIter; aSecIter != theValue.end(); ++aSecIter, ++i) {
      anIndices->SetValue(i, aNbCoords + 1);
      aNbCoords += aSecIter->size();
    }
  }

  // Fill the array of coordinates.
  Handle(TColStd_HArray1OfReal)     aCoords =
    new TColStd_HArray1OfReal(1, aNbCoords);
  std::list<double>::const_iterator aCIter;

  aSecIter = theValue.begin();

  for (i = 1; aSecIter != theValue.end(); ++aSecIter) {
    for (aCIter = aSecIter->begin(); aCIter != aSecIter->end(); ++aCIter) {
      aCoords->SetValue(i++, *aCIter);
    }
  }

  // Store the coordinates.
  _func->SetRealArray(POLY_ARG_COORDS, aCoords);

  if (anIndices.IsNull() == Standard_False) {
    _func->SetIntegerArray(POLY_ARG_START_INDICES, anIndices);
  }
}

//=============================================================================
/*!
 *  GetCoords
 */
//=============================================================================
void GEOMImpl_IPolyline2D::GetCoords(std::list <std::list <double> > &theValue)
{
  theValue.clear();

  Handle(TColStd_HArray1OfReal)    aCoords   =
    _func->GetRealArray(POLY_ARG_COORDS);
  Handle(TColStd_HArray1OfInteger) anIndices =
    _func->GetIntegerArray(POLY_ARG_START_INDICES);

  if (aCoords.IsNull() == Standard_False) {
    std::list <double> anEmptyList;
    Standard_Integer   i;
    Standard_Integer   iNextSec      = 0;
    Standard_Integer   aNextSecIndex = aCoords->Upper() + 1;

    if (anIndices.IsNull() == Standard_False) {
      iNextSec      = anIndices->Lower();
      aNextSecIndex = anIndices->Value(iNextSec);
    }

    theValue.push_back(anEmptyList);

    for (i = aCoords->Lower(); i <= aCoords->Upper(); ++i) {
      // Check if it is necessary to create a new section.
      // Assume a case if there are empty sections.
      while (i == aNextSecIndex) {
        // Create a next section.
        theValue.push_back(anEmptyList);
        ++iNextSec;

        if (iNextSec > anIndices->Upper()) {
          aNextSecIndex = aCoords->Upper() + 1;
        } else {
          aNextSecIndex = anIndices->Value(iNextSec);
        }
      }

      theValue.back().push_back(aCoords->Value(i));
    }
  }
}
