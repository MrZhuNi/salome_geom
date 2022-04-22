// Copyright (C) 2013-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "GEOMImpl_ConformityDriver.hxx"

// internal includes
#include "GEOMImpl_IConformity.hxx"

// KERNEL includes
#include <utilities.h>
#include <Basics_Utils.hxx>

// GEOM includes
#include "GEOM_Function.hxx"
#include "GEOMImpl_Types.hxx"

#include <BOPAlgo_ArgumentAnalyzer.hxx>
#include <BRepTools.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepLib.hxx>
#include <BRep_Tool.hxx>

#include <TopExp_Explorer.hxx>
#include <GEOMUtils.hxx>
#include <TColStd_HArray2OfInteger.hxx>

namespace
{
  //=======================================================================
  //function : GetPairsOfFaultyShapes
  //purpose  :
  //=======================================================================
  NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> GetPairsOfFaultyShapes(
                                                          const BOPAlgo_ArgumentAnalyzer& theAnalyzer,
                                                          const BOPAlgo_CheckStatus       theErrorStatus)
  {
    NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> aResList;

    const BOPAlgo_ListOfCheckResult& aResult = theAnalyzer.GetCheckResult();
    BOPAlgo_ListIteratorOfListOfCheckResult anIt(aResult);
    for (; anIt.More(); anIt.Next())
    {
      if (anIt.Value().GetCheckStatus() == theErrorStatus)
      {
        auto aFaultyShapes = anIt.Value().GetFaultyShapes1();

        aResList.Append({ aFaultyShapes.First(),
                        aFaultyShapes.Size() == 1 ? TopoDS_Shape() : aFaultyShapes.Last() });
      }
    }

    return aResList;
  }

  //=======================================================================
  //function : GetListOfSmallEdges
  //purpose  :
  //=======================================================================
  TopTools_ListOfShape GetListOfSmallEdges(const BOPAlgo_ArgumentAnalyzer& theAnalyzer)
  {
    TopTools_ListOfShape aResList;

    const BOPAlgo_ListOfCheckResult& aResult = theAnalyzer.GetCheckResult();
    BOPAlgo_ListIteratorOfListOfCheckResult anIt(aResult);
    for (; anIt.More(); anIt.Next())
    {
      if (anIt.Value().GetCheckStatus() == BOPAlgo_CheckStatus::BOPAlgo_TooSmallEdge)
      {
        auto aFaultyShapes = anIt.Value().GetFaultyShapes1();
        aResList.Append(aFaultyShapes.First());
      }
    }

    return aResList;
  }

  //=======================================================================
  //function : ConvertShapesToIndices
  //purpose  : Convert sub-shapes of shapes to sequence of indices
  //=======================================================================
  Handle(TColStd_HArray1OfInteger) ConvertShapesToIndices(const TopoDS_Shape& theShape,
                                                          const TopTools_ListOfShape& theShapes)
  {
    Handle(TColStd_HArray1OfInteger) aSeqOfIDs = new TColStd_HArray1OfInteger(1, theShapes.Size());

    TopTools_IndexedMapOfShape anIndices;
    TopExp::MapShapes(theShape, anIndices);

    TopTools_ListIteratorOfListOfShape itSub(theShapes);
    for (int index = 1; itSub.More(); itSub.Next(), ++index)
    {
      int id = anIndices.FindIndex(itSub.Value());
      aSeqOfIDs->SetValue(index, id);
    }

    return aSeqOfIDs;
  }

  //=======================================================================
  //function : ConvertShapesToIndices
  //purpose  : Convert list of pair shapes to sequence of indices
  //=======================================================================
  Handle(TColStd_HArray2OfInteger) ConvertShapesToIndices(
    const TopoDS_Shape& theShape,
    const NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>>& theShapes)
  {
    Handle(TColStd_HArray2OfInteger) aSeqOfIDs = new TColStd_HArray2OfInteger(1, theShapes.Size(), 1, 2);

    TopTools_IndexedMapOfShape anIndices;
    TopExp::MapShapes(theShape, anIndices);

    NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>>::Iterator itSub(theShapes);
    for (int index = 1; itSub.More(); itSub.Next(), ++index)
    {
      int anID1 = anIndices.FindIndex(itSub.Value().first);
      aSeqOfIDs->SetValue(index, 1, anID1);

      int anID2 = anIndices.FindIndex(itSub.Value().second);
      aSeqOfIDs->SetValue(index, 2, anID2);
    }

    return aSeqOfIDs;
  }
}

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& GEOMImpl_ConformityDriver::GetID()
{
  static Standard_GUID aGUID("B77BABDA-C0A1-4E65-9B1E-7EFC4448077E");
  return aGUID;
}

//=======================================================================
//function : GEOMImpl_ConformityDriver
//purpose  :
//=======================================================================
GEOMImpl_ConformityDriver::GEOMImpl_ConformityDriver()
{
}

//=======================================================================
//function : Execute
//purpose  :
//=======================================================================
Standard_Integer GEOMImpl_ConformityDriver::Execute(Handle(TFunction_Logbook)& log) const
{
  if (Label().IsNull()) return 0;
  Handle(GEOM_Function) aFunction = GEOM_Function::GetFunction(Label());
  GEOMImpl_IConformity aCI(aFunction);

  Standard_Integer aType = aFunction->GetType();

  Handle(GEOM_Function) aRefShape = aCI.GetShape();
  if (aRefShape.IsNull()) return 0;

  TopoDS_Shape aShape = aRefShape->GetValue();

  switch (aType)
  {
  case CONFORMITY_DISTANT_SHAPES:
  {
    Standard_Integer aType1 = aCI.GetShapeType1();
    Standard_Integer aType2 = aCI.GetShapeType2();
    NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> aRes;
    if (aType1 == -1 && aType2 == -1)
    {
      aRes = distantShapes(aShape);
    }
    else if (aType1 == -1)
    {
      aRes = distantShapes(aShape, (TopAbs_ShapeEnum)aType2);
    }
    else if (aType2 == -1)
    {
      aRes = distantShapes(aShape, (TopAbs_ShapeEnum)aType1);
    }
    else
    {
      aRes = distantShapes(aShape, (TopAbs_ShapeEnum)aType1, (TopAbs_ShapeEnum)aType2);
    }
    if (!aRes.IsEmpty())
    {
      Handle(TColStd_HArray2OfInteger) anArray = ConvertShapesToIndices(aShape, aRes);
      aCI.SetListOfShapesIndices(anArray);
    }
  }
  break;
  case CONFORMITY_UPDATE_TOL:
  {
    Standard_Real aTolerance = updateTolerance(aShape);
    aFunction->SetReal(CHECKCONFORMITY_RET_TOLERANCE, aTolerance);
  }
  break;
  case CONFORMITY_CHECK_SHAPE:
  {
    NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> aFailedShape;
    Handle(TColStd_HArray1OfInteger) aTypesOfCheck;
    checkShape(aShape, aFailedShape, aTypesOfCheck);

    Handle(TColStd_HArray2OfInteger) anArray = ConvertShapesToIndices(aShape, aFailedShape);
    aFunction->SetIntegerArray(CHECKCONFORMITY_RET_TYPES_CHECKS, aTypesOfCheck);
    aCI.SetListOfShapesIndices(anArray);
  }
  }

  return 1;
}

//=======================================================================
//function : distantShapes
//purpose  : TODO: Not implemented! Wait for required functionality!
//=======================================================================
NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> GEOMImpl_ConformityDriver::distantShapes(
  const TopoDS_Shape& theShape,
  const TopAbs_ShapeEnum theShapeType,
  const TopAbs_ShapeEnum theSubShapeType,
  Standard_Real          theTolerance) const
{
  // TODO: Not implemented! Wait for required functionality!
  return NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>>();
}

//=======================================================================
//function : distantShapes
//purpose  : TODO: Not implemented! Wait for required functionality!
//=======================================================================
NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> GEOMImpl_ConformityDriver::distantShapes(
  const TopoDS_Shape& theShape,
  const TopAbs_ShapeEnum theShapeType,
  Standard_Real          theTolerance) const
{
  NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> aDistShapes;
  switch (theShapeType)
  {
  case TopAbs_VERTEX:
    aDistShapes.Append(distantShapes(theShape, TopAbs_EDGE, theShapeType, theTolerance));
    aDistShapes.Append(distantShapes(theShape, TopAbs_FACE, theShapeType, theTolerance));
    break;
  case TopAbs_EDGE:
    aDistShapes.Append(distantShapes(theShape, theShapeType, TopAbs_VERTEX, theTolerance));
    aDistShapes.Append(distantShapes(theShape, TopAbs_FACE, theShapeType, theTolerance));
    break;
  case TopAbs_FACE:
    aDistShapes.Append(distantShapes(theShape, theShapeType, TopAbs_VERTEX, theTolerance));
    aDistShapes.Append(distantShapes(theShape, theShapeType, TopAbs_EDGE, theTolerance));
    break;
  }

  return aDistShapes;
}

//=======================================================================
//function : distantShapes
//purpose  : TODO: Not implemented! Wait for required functionality!
//=======================================================================
NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> GEOMImpl_ConformityDriver::distantShapes(
  const TopoDS_Shape& theShape,
  Standard_Real       theTolerance) const
{
  NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>> aDistShapes;

  aDistShapes.Append(distantShapes(theShape, TopAbs_EDGE, TopAbs_VERTEX, theTolerance));
  aDistShapes.Append(distantShapes(theShape, TopAbs_FACE, TopAbs_VERTEX, theTolerance));
  aDistShapes.Append(distantShapes(theShape, TopAbs_FACE, TopAbs_EDGE, theTolerance));

  return aDistShapes;
}

void GEOMImpl_ConformityDriver::checkShape(const TopoDS_Shape & theShape,
                                           NCollection_List<std::pair<TopoDS_Shape, TopoDS_Shape>>& theFailedShape,
                                           Handle(TColStd_HArray1OfInteger)& theTypesOfCheck) const
{
  BOPAlgo_ArgumentAnalyzer anAnalyzer;
  performAnalyze(theShape, anAnalyzer);

  const BOPAlgo_ListOfCheckResult& aResult = anAnalyzer.GetCheckResult();
  theTypesOfCheck = new TColStd_HArray1OfInteger(1, aResult.Size());

  BOPAlgo_ListOfCheckResult::Iterator anIter(aResult);
  for (int index = 1; anIter.More(); anIter.Next(), ++index)
  {
    theTypesOfCheck->SetValue(index, anIter.Value().GetCheckStatus());
    std::pair<TopoDS_Shape, TopoDS_Shape> aPair;
    aPair.first = anIter.Value().GetFaultyShapes1().First();
    if (anIter.Value().GetFaultyShapes1().Size() != 1)
      aPair.second = anIter.Value().GetFaultyShapes1().Last();
    theFailedShape.Append(aPair);
  }
}

//=======================================================================
//function : updateTolerance
//purpose  :
//=======================================================================
Standard_Real GEOMImpl_ConformityDriver::updateTolerance(const TopoDS_Shape& theShape) const
{
  Standard_Real aTolerance = INFINITY;

  TopoDS_Shape aResShape = theShape;

  BRepLib::UpdateTolerances(aResShape, Standard_False);

  for (const auto& aType : { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE })
  {
    for (TopExp_Explorer anExp(aResShape, aType); anExp.More(); anExp.Next())
    {
      Standard_Real aCurTolerance = INFINITY;
      switch (aType)
      {
      case TopAbs_VERTEX:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Vertex(anExp.Value()));
        break;
      case TopAbs_EDGE:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Edge(anExp.Value()));
        break;
      case TopAbs_FACE:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Face(anExp.Value()));
        break;
      }
      aTolerance = Min(aTolerance, aCurTolerance);
    }
  }

  TopoDS_Shape aShape = theShape;
  GEOMUtils::FixShapeTolerance(aShape, aTolerance, Standard_True);

  Standard_Real aResTol = 0.;
  for (const auto& aType : { TopAbs_VERTEX, TopAbs_EDGE, TopAbs_FACE })
  {
    for (TopExp_Explorer anExp(aShape, aType); anExp.More(); anExp.Next())
    {
      Standard_Real aCurTolerance = INFINITY;
      switch (aType)
      {
      case TopAbs_VERTEX:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Vertex(anExp.Value()));
        break;
      case TopAbs_EDGE:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Edge(anExp.Value()));
        break;
      case TopAbs_FACE:
        aCurTolerance = BRep_Tool::Tolerance(TopoDS::Face(anExp.Value()));
        break;
      }
      aResTol = Max(aResTol, aCurTolerance);
    }
  }

  return aResTol;
}

//=======================================================================
//function : performAnalyze
//purpose  : 
//=======================================================================
void GEOMImpl_ConformityDriver::performAnalyze(const TopoDS_Shape& theShape,
                                               BOPAlgo_ArgumentAnalyzer& theAnalyzer) const
{
  theAnalyzer.SetShape1(theShape);

  theAnalyzer.CurveOnSurfaceMode() = Standard_True;
  theAnalyzer.SelfInterMode() = Standard_True;
  theAnalyzer.SmallEdgeMode() = Standard_True;

  theAnalyzer.Perform();
}
