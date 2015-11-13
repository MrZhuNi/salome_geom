// Copyright (C) 2007-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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

// File:        GEOMAlgo_Extractor.cxx
// Created:
// Author:      Sergey KHROMOV
//


#include <GEOMAlgo_Extractor.hxx>

#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>


//=======================================================================
//function : GEOMAlgo_Extractor
//purpose  :
//=======================================================================
GEOMAlgo_Extractor::GEOMAlgo_Extractor()
{
}

//=======================================================================
//function : ~GEOMAlgo_Extractor
//purpose  :
//=======================================================================
GEOMAlgo_Extractor::~GEOMAlgo_Extractor()
{
}

//=======================================================================
//function : SetShape
//purpose  :
//=======================================================================
void GEOMAlgo_Extractor::SetShape(const TopoDS_Shape &theShape)
{
  myShape = theShape;
  clear();
}

//=======================================================================
//function : SetShapesToRemove
//purpose  :
//=======================================================================
void GEOMAlgo_Extractor::SetShapesToRemove
                        (const TopTools_ListOfShape &theSubShapes)
{
  mySubShapes.Assign(theSubShapes);
  clear();
}

//=======================================================================
//function : Perform
//purpose  :
//=======================================================================
void GEOMAlgo_Extractor::Perform()
{
  clear();
  myErrorStatus = 0;
  //
  checkData();

  if(myErrorStatus) {
    return;
  }

  if (myWarningStatus == 10) {
    // The result is the same shape. Nothing is modified.
    myResult = myShape;
    return;
  }
}

//=======================================================================
//function : GetResult
//purpose  :
//=======================================================================
const TopoDS_Shape &GEOMAlgo_Extractor::GetResult() const
{
  return myResult;
}

//=======================================================================
//function : clear
//purpose  :
//=======================================================================
void GEOMAlgo_Extractor::clear()
{
  myErrorStatus   = 1;
  myWarningStatus = 0;
  myResult.Nullify();
  myRemoved.Clear();
  myModified.Clear();
  myNew.Clear();
}

//=======================================================================
//function : checkData
//purpose  :
//=======================================================================
void GEOMAlgo_Extractor::checkData()
{
  if (myShape.IsNull()) {
    myErrorStatus = 10;
    return;
  }

  if (mySubShapes.IsEmpty()) {
    myWarningStatus = 10;
    return;
  }

  TopTools_ListIteratorOfListOfShape anIter(mySubShapes);
  TopTools_IndexedMapOfShape         anIndices;

  TopExp::MapShapes(myShape, anIndices);

  for (; anIter.More(); anIter.Next()) {
    const TopoDS_Shape &aSubShape = anIter.Value();

    if (!anIndices.Contains(aSubShape)) {
      myErrorStatus = 11;
      return;
    }
  }
}


//
// myErrorStatus :
//
// 10 -myShape=NULL
// 11 -mySubShapes contains not only sub-shapes of myShape.
//
// myWarningStatus :
//
// 10 -mySubShapes is empty
//
