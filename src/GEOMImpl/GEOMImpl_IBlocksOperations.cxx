using namespace std;

#include "GEOMImpl_IBlocksOperations.hxx"

#include "GEOMImpl_Types.hxx"

#include "GEOMImpl_BlockDriver.hxx"
#include "GEOMImpl_IBlocks.hxx"
#include "GEOMImpl_IBlockTrsf.hxx"
#include "GEOMImpl_CopyDriver.hxx"
#include "GEOMImpl_Block6Explorer.hxx"

#include "GEOM_Function.hxx"

#include "utilities.h"
#include "OpUtil.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <TFunction_DriverTable.hxx>
#include <TFunction_Driver.hxx>
#include <TFunction_Logbook.hxx>
#include <TDF_Tool.hxx>

#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepClass3d_SolidClassifier.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepGProp.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopTools_Array1OfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>

#include <Precision.hxx>
#include <GProp_GProps.hxx>
#include <TColStd_Array1OfInteger.hxx>

#include <Standard_ErrorHandler.hxx> // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC

//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================
GEOMImpl_IBlocksOperations::GEOMImpl_IBlocksOperations (GEOM_Engine* theEngine, int theDocID)
: GEOM_IOperations(theEngine, theDocID)
{
  MESSAGE("GEOMImpl_IBlocksOperations::GEOMImpl_IBlocksOperations");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================
GEOMImpl_IBlocksOperations::~GEOMImpl_IBlocksOperations()
{
  MESSAGE("GEOMImpl_IBlocksOperations::~GEOMImpl_IBlocksOperations");
}


//=============================================================================
/*!
 *  MakeQuadFaceFourVertices
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeQuadFaceFourVertices
                     (Handle(GEOM_Object) thePnt1, Handle(GEOM_Object) thePnt2,
                      Handle(GEOM_Object) thePnt3, Handle(GEOM_Object) thePnt4)
{
  SetErrorCode(KO);

  if (thePnt1.IsNull() || thePnt2.IsNull() ||
      thePnt3.IsNull() || thePnt4.IsNull()) return NULL;

  //Add a new Face object
  Handle(GEOM_Object) aFace = GetEngine()->AddObject(GetDocID(), GEOM_FACE);

  //Add a new Face function
  Handle(GEOM_Function) aFunction =
    aFace->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_FACE_FOUR_PNT);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlocks aPI (aFunction);

  Handle(GEOM_Function) aRef1 = thePnt1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = thePnt2->GetLastFunction();
  Handle(GEOM_Function) aRef3 = thePnt3->GetLastFunction();
  Handle(GEOM_Function) aRef4 = thePnt4->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull() ||
      aRef3.IsNull() || aRef4.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aShapesSeq = new TColStd_HSequenceOfTransient;
  aShapesSeq->Append(aRef1);
  aShapesSeq->Append(aRef2);
  aShapesSeq->Append(aRef3);
  aShapesSeq->Append(aRef4);

  aPI.SetShapes(aShapesSeq);

  //Compute the Face value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to compute a face");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aFace->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeQuadFaceFourVertices(");
  TDF_Tool::Entry(thePnt1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt2->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt3->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt4->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aFace;
}

//=============================================================================
/*!
 *  MakeQuadFaceFourEdges
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeQuadFaceFourEdges
                     (Handle(GEOM_Object) theEdge1, Handle(GEOM_Object) theEdge2,
                      Handle(GEOM_Object) theEdge3, Handle(GEOM_Object) theEdge4)
{
  SetErrorCode(KO);

  if (theEdge1.IsNull() || theEdge2.IsNull() ||
      theEdge3.IsNull() || theEdge4.IsNull()) return NULL;

  //Add a new Face object
  Handle(GEOM_Object) aFace = GetEngine()->AddObject(GetDocID(), GEOM_FACE);

  //Add a new Face function
  Handle(GEOM_Function) aFunction =
    aFace->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_FACE_FOUR_EDGES);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlocks aPI (aFunction);

  Handle(GEOM_Function) aRef1 = theEdge1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = theEdge2->GetLastFunction();
  Handle(GEOM_Function) aRef3 = theEdge3->GetLastFunction();
  Handle(GEOM_Function) aRef4 = theEdge4->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull() ||
      aRef3.IsNull() || aRef4.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aShapesSeq = new TColStd_HSequenceOfTransient;
  aShapesSeq->Append(aRef1);
  aShapesSeq->Append(aRef2);
  aShapesSeq->Append(aRef3);
  aShapesSeq->Append(aRef4);

  aPI.SetShapes(aShapesSeq);

  //Compute the Face value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to compute a face");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aFace->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeQuadFaceFourEdges(");
  TDF_Tool::Entry(theEdge1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge2->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge3->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge4->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aFace;
}

//=============================================================================
/*!
 *  MakeQuadFaceTwoEdges
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeQuadFaceTwoEdges
                     (Handle(GEOM_Object) theEdge1, Handle(GEOM_Object) theEdge2)
{
  SetErrorCode(KO);

  if (theEdge1.IsNull() || theEdge2.IsNull()) return NULL;

  //Add a new Face object
  Handle(GEOM_Object) aFace = GetEngine()->AddObject(GetDocID(), GEOM_FACE);

  //Add a new Face function
  Handle(GEOM_Function) aFunction =
    aFace->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_FACE_TWO_EDGES);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlocks aPI (aFunction);

  Handle(GEOM_Function) aRef1 = theEdge1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = theEdge2->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aShapesSeq = new TColStd_HSequenceOfTransient;
  aShapesSeq->Append(aRef1);
  aShapesSeq->Append(aRef2);

  aPI.SetShapes(aShapesSeq);

  //Compute the Face value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to compute a face");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aFace->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeQuadFaceTwoEdges(");
  TDF_Tool::Entry(theEdge1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aFace;
}

//=============================================================================
/*!
 *  MakeHexaSolidSixFaces
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeHexaSolidSixFaces
                     (Handle(GEOM_Object) theFace1, Handle(GEOM_Object) theFace2,
                      Handle(GEOM_Object) theFace3, Handle(GEOM_Object) theFace4,
                      Handle(GEOM_Object) theFace5, Handle(GEOM_Object) theFace6)
{
  SetErrorCode(KO);

  if (theFace1.IsNull() || theFace2.IsNull() ||
      theFace3.IsNull() || theFace4.IsNull() ||
      theFace5.IsNull() || theFace6.IsNull()) return NULL;

  //Add a new Solid object
  Handle(GEOM_Object) aBlock = GetEngine()->AddObject(GetDocID(), GEOM_BLOCK);

  //Add a new Block function
  Handle(GEOM_Function) aFunction =
    aBlock->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_SIX_FACES);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlocks aPI (aFunction);

  Handle(GEOM_Function) aRef1 = theFace1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = theFace2->GetLastFunction();
  Handle(GEOM_Function) aRef3 = theFace3->GetLastFunction();
  Handle(GEOM_Function) aRef4 = theFace4->GetLastFunction();
  Handle(GEOM_Function) aRef5 = theFace5->GetLastFunction();
  Handle(GEOM_Function) aRef6 = theFace6->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull() ||
      aRef3.IsNull() || aRef4.IsNull() ||
      aRef5.IsNull() || aRef6.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aShapesSeq = new TColStd_HSequenceOfTransient;
  aShapesSeq->Append(aRef1);
  aShapesSeq->Append(aRef2);
  aShapesSeq->Append(aRef3);
  aShapesSeq->Append(aRef4);
  aShapesSeq->Append(aRef5);
  aShapesSeq->Append(aRef6);

  aPI.SetShapes(aShapesSeq);

  //Compute the Block value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to compute a block");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aBlock->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeHexaSolidSixFaces(");
  TDF_Tool::Entry(theFace1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace2->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace3->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace4->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace5->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace6->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aBlock;
}

//=============================================================================
/*!
 *  MakeHexaSolidTwoFaces
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeHexaSolidTwoFaces
                   (Handle(GEOM_Object) theFace1, Handle(GEOM_Object) theFace2)
{
  SetErrorCode(KO);

  if (theFace1.IsNull() || theFace2.IsNull()) return NULL;

  //Add a new Solid object
  Handle(GEOM_Object) aBlock = GetEngine()->AddObject(GetDocID(), GEOM_BLOCK);

  //Add a new Block function
  Handle(GEOM_Function) aFunction =
    aBlock->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_TWO_FACES);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlocks aPI (aFunction);

  Handle(GEOM_Function) aRef1 = theFace1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = theFace2->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aShapesSeq = new TColStd_HSequenceOfTransient;
  aShapesSeq->Append(aRef1);
  aShapesSeq->Append(aRef2);

  aPI.SetShapes(aShapesSeq);

  //Compute the Block value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to compute a block");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aBlock->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeHexaSolidTwoFaces(");
  TDF_Tool::Entry(theFace1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aBlock;
}

//=============================================================================
/*!
 *  GetEdge
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetEdge
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) thePoint1,
                                                 Handle(GEOM_Object) thePoint2)
{
  SetErrorCode(KO);

  //New Edge object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() || thePoint1.IsNull() || thePoint2.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block or compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID &&
      aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg1 = thePoint1->GetValue();
  TopoDS_Shape anArg2 = thePoint2->GetValue();
  if (anArg1.IsNull() || anArg2.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg1.ShapeType() != TopAbs_VERTEX ||
      anArg2.ShapeType() != TopAbs_VERTEX) {
    SetErrorCode("Element for edge identification is not a vertex");
    return NULL;
  }

  //Compute the Edge value
  try {
    TopTools_IndexedDataMapOfShapeListOfShape MVE;
    GEOMImpl_Block6Explorer::MapShapesAndAncestors
      (aBlockOrComp, TopAbs_VERTEX, TopAbs_EDGE, MVE);

    TopoDS_Shape V1,V2;
    Standard_Integer ish, ext = MVE.Extent();

    if (MVE.Contains(anArg1)) {
      V1 = anArg1;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVE.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg1), TopoDS::Vertex(aShi))) {
          V1 = aShi;
          break;
        }
      }
    }

    if (MVE.Contains(anArg2)) {
      V2 = anArg2;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVE.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg2), TopoDS::Vertex(aShi))) {
          V2 = aShi;
          break;
        }
      }
    }

    if (V1.IsNull() || V2.IsNull()) {
      SetErrorCode("The given vertex does not belong to the shape");
      return NULL;
    }

    const TopTools_ListOfShape& anEdgesOfV1 = MVE.FindFromKey(V1);
    const TopTools_ListOfShape& anEdgesOfV2 = MVE.FindFromKey(V2);

    Standard_Integer isFound = 0;
    TopTools_ListIteratorOfListOfShape anIterE1 (anEdgesOfV1);
    TopTools_MapOfShape mapShape1;
    TopoDS_Shape anEdge;
    for (; anIterE1.More(); anIterE1.Next()) {

      if (mapShape1.Add(anIterE1.Value())) {
        TopTools_ListIteratorOfListOfShape anIterE2 (anEdgesOfV2);
        TopTools_MapOfShape mapShape2;
        for (; anIterE2.More(); anIterE2.Next()) {

          if (mapShape2.Add(anIterE2.Value())) {
            if (anIterE1.Value().IsSame(anIterE2.Value())) {
              isFound++;

              // Store the edge, defined by two vertices
              anEdge = anIterE1.Value();
            }
          }
        }
      }
    }
    if (isFound == 0) {
      SetErrorCode("The given vertices do not belong to one edge of the given shape");
      return NULL;
    } else if (isFound > 1) {
      SetErrorCode("Multiple edges found by the given vertices of the shape");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(anEdge));
      aResult = GetEngine()->AddSubShape(theShape, anArray);
    }
  } catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetEdge() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetEdge(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetFaceByPoints
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetFaceByPoints
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) thePoint1,
                                                 Handle(GEOM_Object) thePoint2,
                                                 Handle(GEOM_Object) thePoint3,
                                                 Handle(GEOM_Object) thePoint4)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() ||
      thePoint1.IsNull() || thePoint2.IsNull() ||
      thePoint3.IsNull() || thePoint4.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block or compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID &&
      aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg1 = thePoint1->GetValue();
  TopoDS_Shape anArg2 = thePoint2->GetValue();
  TopoDS_Shape anArg3 = thePoint3->GetValue();
  TopoDS_Shape anArg4 = thePoint4->GetValue();
  if (anArg1.IsNull() || anArg2.IsNull() ||
      anArg3.IsNull() || anArg4.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg1.ShapeType() != TopAbs_VERTEX ||
      anArg2.ShapeType() != TopAbs_VERTEX ||
      anArg3.ShapeType() != TopAbs_VERTEX ||
      anArg4.ShapeType() != TopAbs_VERTEX) {
    SetErrorCode("Element for face identification is not a vertex");
    return NULL;
  }

  //Compute the Face value
  try {
    TopoDS_Shape aShape;

    TopTools_IndexedDataMapOfShapeListOfShape MVF;
    GEOMImpl_Block6Explorer::MapShapesAndAncestors(aBlockOrComp, TopAbs_VERTEX, TopAbs_FACE, MVF);

    TopoDS_Shape V1,V2,V3,V4;
    Standard_Integer ish, ext = MVF.Extent();

    if (MVF.Contains(anArg1)) {
      V1 = anArg1;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVF.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg1), TopoDS::Vertex(aShi))) {
          V1 = aShi;
          break;
        }
      }
    }

    if (MVF.Contains(anArg2)) {
      V2 = anArg2;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVF.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg2), TopoDS::Vertex(aShi))) {
          V2 = aShi;
          break;
        }
      }
    }

    if (MVF.Contains(anArg3)) {
      V3 = anArg3;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVF.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg3), TopoDS::Vertex(aShi))) {
          V3 = aShi;
          break;
        }
      }
    }

    if (MVF.Contains(anArg4)) {
      V4 = anArg4;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MVF.FindKey(ish);
        if (BRepTools::Compare(TopoDS::Vertex(anArg4), TopoDS::Vertex(aShi))) {
          V4 = aShi;
          break;
        }
      }
    }

    if (V1.IsNull() || V2.IsNull() || V3.IsNull() || V4.IsNull()) {
      SetErrorCode("The given vertex does not belong to the shape");
      return NULL;
    }

    const TopTools_ListOfShape& aFacesOfV1 = MVF.FindFromKey(V1);
    const TopTools_ListOfShape& aFacesOfV2 = MVF.FindFromKey(V2);
    const TopTools_ListOfShape& aFacesOfV3 = MVF.FindFromKey(V3);
    const TopTools_ListOfShape& aFacesOfV4 = MVF.FindFromKey(V4);

    Standard_Integer isFound = 0;
    TopTools_ListIteratorOfListOfShape anIterF1 (aFacesOfV1);
    for (; anIterF1.More(); anIterF1.Next()) {

      TopoDS_Shape aFace = anIterF1.Value();
      TopTools_ListIteratorOfListOfShape anIterF2 (aFacesOfV2);
      for (; anIterF2.More(); anIterF2.Next()) {

        if (aFace.IsSame(anIterF2.Value())) {
          TopTools_ListIteratorOfListOfShape anIterF3 (aFacesOfV3);
          for (; anIterF3.More(); anIterF3.Next()) {

            if (aFace.IsSame(anIterF3.Value())) {
              TopTools_ListIteratorOfListOfShape anIterF4 (aFacesOfV4);
              for (; anIterF4.More(); anIterF4.Next()) {

                if (aFace.IsSame(anIterF4.Value())) {
                  isFound++;

                  // Store the face, defined by four vertices
                  aShape = aFace;
                }
              }
            }
          }
        }
      }
    }
    if (isFound == 0) {
      SetErrorCode("The given vertices do not belong to one face of the given shape");
      return NULL;
    } else if (isFound > 1) {
      SetErrorCode("The given vertices belong to several faces of the given shape");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theShape, anArray);
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetFaceByPoints() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetFaceByPoints(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint2->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint3->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint4->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetFaceByEdges
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetFaceByEdges
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) theEdge1,
                                                 Handle(GEOM_Object) theEdge2)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() || theEdge1.IsNull() || theEdge2.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block or compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID &&
      aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg1 = theEdge1->GetValue();
  TopoDS_Shape anArg2 = theEdge2->GetValue();
  if (anArg1.IsNull() || anArg2.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg1.ShapeType() != TopAbs_EDGE ||
      anArg2.ShapeType() != TopAbs_EDGE) {
    SetErrorCode("Element for face identification is not an edge");
    return NULL;
  }

  //Compute the Face value
  try {
    TopoDS_Shape aShape;

    TopTools_IndexedDataMapOfShapeListOfShape MEF;
    GEOMImpl_Block6Explorer::MapShapesAndAncestors(aBlockOrComp, TopAbs_EDGE, TopAbs_FACE, MEF);

    TopoDS_Shape E1,E2;
    Standard_Integer ish, ext = MEF.Extent();

    if (MEF.Contains(anArg1)) {
      E1 = anArg1;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MEF.FindKey(ish);
        if (GEOMImpl_Block6Explorer::IsSimilarEdges(anArg1, aShi)) {
          E1 = aShi;
        }
      }
    }

    if (MEF.Contains(anArg2)) {
      E2 = anArg2;
    } else {
      for (ish = 1; ish <= ext; ish++) {
        TopoDS_Shape aShi = MEF.FindKey(ish);
        if (GEOMImpl_Block6Explorer::IsSimilarEdges(anArg2, aShi)) {
          E2 = aShi;
        }
      }
    }

    if (E1.IsNull() || E2.IsNull()) {
      SetErrorCode("The given edge does not belong to the shape");
      return NULL;
    }

    const TopTools_ListOfShape& aFacesOfE1 = MEF.FindFromKey(E1);
    const TopTools_ListOfShape& aFacesOfE2 = MEF.FindFromKey(E2);

    Standard_Integer isFound = 0;
    TopTools_ListIteratorOfListOfShape anIterF1 (aFacesOfE1);
    for (; anIterF1.More(); anIterF1.Next()) {

      TopTools_ListIteratorOfListOfShape anIterF2 (aFacesOfE2);
      for (; anIterF2.More(); anIterF2.Next()) {

        if (anIterF1.Value().IsSame(anIterF2.Value())) {
          isFound++;

          // Store the face, defined by two edges
          aShape = anIterF1.Value();
        }
      }
    }
    if (isFound == 0) {
      SetErrorCode("The given edges do not belong to one face of the given shape");
      return NULL;
    } else if (isFound > 1) {
      SetErrorCode("The given edges belong to several faces of the given shape");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theShape, anArray);
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetFaceByEdges() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetFaceByEdges(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theEdge2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetOppositeFace
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetOppositeFace
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) theFace)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() || theFace.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID) {
    SetErrorCode("Shape is not a block");
    return NULL;
  }

  TopoDS_Shape anArg = theFace->GetValue();
  if (anArg.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg.ShapeType() != TopAbs_FACE) {
    SetErrorCode("Element for face identification is not a face");
    return NULL;
  }

  //Compute the Face value
  try {
    TopoDS_Shape aShape;

    GEOMImpl_Block6Explorer aBlockTool;
    aBlockTool.InitByBlockAndFace(aBlockOrComp, anArg);
    aShape = aBlockTool.GetFace(2);

    TopTools_IndexedMapOfShape anIndices;
    TopExp::MapShapes(aBlockOrComp, anIndices);
    Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
    anArray->SetValue(1, anIndices.FindIndex(aShape));
    aResult = GetEngine()->AddSubShape(theShape, anArray);
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetOppositeFace() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetOppositeFace(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theFace->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetFaceNearPoint
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetFaceNearPoint
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) thePoint)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() || thePoint.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block or compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID &&
      aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg = thePoint->GetValue();
  if (anArg.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg.ShapeType() != TopAbs_VERTEX) {
    SetErrorCode("Element for face identification is not a vertex");
    return NULL;
  }

  //Compute the Face value
  try {
    TopoDS_Shape aShape;

    TopoDS_Vertex aVert = TopoDS::Vertex(anArg);
    gp_Pnt aPnt = BRep_Tool::Pnt(aVert);

    // 1. Explode blocks on faces
    TopTools_MapOfShape mapShape;
    Standard_Integer nbFaces = 0;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_FACE);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        nbFaces++;
      }
    }

    mapShape.Clear();
    Standard_Integer ind = 1;
    TopTools_Array1OfShape aFaces (1, nbFaces);
    TColStd_Array1OfInteger aDistances (1, nbFaces);
    for (exp.Init(aBlockOrComp, TopAbs_FACE); exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aFace = exp.Current();
        aFaces(ind) = aFace;

        // 2. Classify the point relatively each face
        BRepClass_FaceClassifier FC (TopoDS::Face(aFace), aPnt, Precision::Confusion());
        if (FC.State() == TopAbs_IN) {
          aDistances(ind) = -1;
        } else if (FC.State() == TopAbs_ON) {
          aDistances(ind) = 0;
        } else { // OUT
          aDistances(ind) = 1;
        }
        ind++;
      }
    }

    // 3. Define face, containing the point or having minimum distance to it
    Standard_Integer nearest = 2, nbFound = 0;
    for (ind = 1; ind <= nbFaces; ind++) {
      if (aDistances(ind) < nearest) {
        nearest = aDistances(ind);
        aShape = aFaces(ind);
        nbFound = 1;
      } else if (aDistances(ind) == nearest) {
        nbFound++;
      } else {
      }
    }
    if (nbFound > 1) {
      if (nearest == 0) {
        // The point is on boundary of some faces and there are
        // no faces, having the point inside
        SetErrorCode("Multiple faces near the given point are found");
        return NULL;

      } else if (nearest == 1) {
        // The point is outside some faces and there are
        // no faces, having the point inside or on boundary.
        // We will get a nearest face
        Standard_Real minDist = RealLast();
        for (ind = 1; ind <= nbFaces; ind++) {
          if (aDistances(ind) == 1) {
            BRepExtrema_DistShapeShape aDistTool (aVert, aFaces(ind));
            if (!aDistTool.IsDone()) {
              SetErrorCode("Can not find a distance from the given point to one of faces");
              return NULL;
            }
            Standard_Real aDist = aDistTool.Value();
            if (aDist < minDist) {
              minDist = aDist;
              aShape = aFaces(ind);
            }
          }
        }
      } else { // nearest == -1
        // The point is inside some faces.
        // We will get a face with nearest center
        Standard_Real minDist = RealLast();
        for (ind = 1; ind <= nbFaces; ind++) {
          if (aDistances(ind) == -1) {
            GProp_GProps aSystem;
            BRepGProp::SurfaceProperties(aFaces(ind), aSystem);
            gp_Pnt aCenterMass = aSystem.CentreOfMass();

            Standard_Real aDist = aCenterMass.Distance(aPnt);
            if (aDist < minDist) {
              minDist = aDist;
              aShape = aFaces(ind);
            }
          }
        }
      }
    }

    if (nbFound == 0) {
      SetErrorCode("There are no faces near the given point");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theShape, anArray);
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetFaceNearPoint() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetFaceNearPoint(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetFaceByNormale
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetFaceByNormale
                                                (Handle(GEOM_Object) theShape,
                                                 Handle(GEOM_Object) theVector)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theShape.IsNull() || theVector.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theShape->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Block or compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_SOLID &&
      aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg = theVector->GetValue();
  if (anArg.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg.ShapeType() != TopAbs_EDGE) {
    SetErrorCode("Element for normale identification is not an edge");
    return NULL;
  }

  //Compute the Face value
  try {
    TopoDS_Shape aShape;

    TopoDS_Edge anEdge = TopoDS::Edge(anArg);
    TopoDS_Vertex V1, V2;
    TopExp::Vertices(anEdge, V1, V2, Standard_True);
    gp_Pnt P1 = BRep_Tool::Pnt(V1);
    gp_Pnt P2 = BRep_Tool::Pnt(V2);
    gp_Vec aVec (P1, P2);
    if (aVec.Magnitude() < Precision::Confusion()) {
      SetErrorCode("Vector with null magnitude is given");
      return NULL;
    }

    Standard_Real minAngle = RealLast();
    TopTools_MapOfShape mapShape;
    Standard_Integer nbFaces = 0;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_FACE);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Face aFace = TopoDS::Face(exp.Current());
        BRepAdaptor_Surface SF (aFace);

        Standard_Real u, v, x;

        // find a point on the surface to get normal direction in
        u = SF.FirstUParameter();
        x = SF.LastUParameter();
        if (Precision::IsInfinite(u)) {
          u =  (Precision::IsInfinite(x)) ? 0. : x;
        } else if (!Precision::IsInfinite(x)) {
          u = (u+x) / 2.;
        }

        v = SF.FirstVParameter();
        x = SF.LastVParameter();
        if (Precision::IsInfinite(v)) {
          v =  (Precision::IsInfinite(x)) ? 0. : x;
        } else if (!Precision::IsInfinite(x)) {
          v = (v+x) / 2.;
        }

        // compute the normal direction
        gp_Vec Vec1,Vec2;
        SF.D1(u,v,P1,Vec1,Vec2);
        gp_Vec V = Vec1.Crossed(Vec2);
        x = V.Magnitude();
        if (V.Magnitude() < Precision::Confusion()) {
          SetErrorCode("Normal vector of a face has null magnitude");
          return NULL;
        }

        // consider the face orientation
        if (aFace.Orientation() == TopAbs_REVERSED ||
            aFace.Orientation() == TopAbs_INTERNAL) {
          V = - V;
        }

        // compute the angle and compare with the minimal one
        Standard_Real anAngle = aVec.Angle(V);
        if (anAngle < minAngle) {
          minAngle = anAngle;
          aShape = aFace;
        }
      }
    }

    if (aShape.IsNull()) {
      SetErrorCode("Failed to find a face by the given normale");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theShape, anArray);
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetFaceByNormale() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theShape->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetFaceByNormale(");
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVector->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  IsCompoundOfBlocks
 */
//=============================================================================
Standard_Boolean GEOMImpl_IBlocksOperations::IsCompoundOfBlocks
                                                (Handle(GEOM_Object)    theCompound,
                                                 const Standard_Integer theMinNbFaces,
                                                 const Standard_Integer theMaxNbFaces,
                                                 Standard_Integer&      theNbBlocks)
{
  SetErrorCode(KO);
  Standard_Boolean isCompOfBlocks = Standard_False;
  theNbBlocks = 0;

  if (theCompound.IsNull()) return isCompOfBlocks;
  TopoDS_Shape aBlockOrComp = theCompound->GetValue();

  //Check
  isCompOfBlocks = Standard_True;
  try {
    TopTools_MapOfShape mapShape;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_SOLID);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aSolid = exp.Current();

        TopTools_MapOfShape mapFaces;
        TopExp_Explorer expF (aSolid, TopAbs_FACE);
        Standard_Integer nbFaces = 0;
        for (; expF.More(); expF.Next()) {
          if (mapFaces.Add(expF.Current())) {
            nbFaces++;
            if (nbFaces > theMaxNbFaces) {
              isCompOfBlocks = Standard_False;
              break;
            }
          }
        }
        if (nbFaces < theMinNbFaces || theMaxNbFaces < nbFaces) {
          isCompOfBlocks = Standard_False;
        } else {
          theNbBlocks++;
        }
      }
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return isCompOfBlocks;
  }

  SetErrorCode(OK);
  return isCompOfBlocks;
}

//=============================================================================
/*!
 *  ExplodeCompoundOfBlocks
 */
//=============================================================================
Handle(TColStd_HSequenceOfTransient) GEOMImpl_IBlocksOperations::ExplodeCompoundOfBlocks
                                                (Handle(GEOM_Object)    theCompound,
                                                 const Standard_Integer theMinNbFaces,
                                                 const Standard_Integer theMaxNbFaces)
{
  SetErrorCode(KO);

  if (theCompound.IsNull()) return NULL;
  TopoDS_Shape aBlockOrComp = theCompound->GetValue();
  if (aBlockOrComp.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aBlocks = new TColStd_HSequenceOfTransient;
  Handle(GEOM_Object) anObj;
  Handle(GEOM_Function) aFunction;

  TopTools_MapOfShape mapShape;
  TCollection_AsciiString anAsciiList = "[", anEntry;

  // Map shapes
  TopTools_IndexedMapOfShape anIndices;
  TopExp::MapShapes(aBlockOrComp, anIndices);
  Handle(TColStd_HArray1OfInteger) anArray;

  // Explode
  try {
    TopExp_Explorer exp (aBlockOrComp, TopAbs_SOLID);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aSolid = exp.Current();

        TopTools_MapOfShape mapFaces;
        TopExp_Explorer expF (aSolid, TopAbs_FACE);
        Standard_Integer nbFaces = 0;
        for (; expF.More(); expF.Next()) {
          if (mapFaces.Add(expF.Current())) {
            nbFaces++;
          }
        }

        if (theMinNbFaces <= nbFaces && nbFaces <= theMaxNbFaces) {
          anArray = new TColStd_HArray1OfInteger(1,1);
          anArray->SetValue(1, anIndices.FindIndex(aSolid));
          anObj = GetEngine()->AddSubShape(theCompound, anArray);
          aBlocks->Append(anObj);

          //Make a Python command
          TDF_Tool::Entry(anObj->GetEntry(), anEntry);
          anAsciiList += anEntry;
          anAsciiList += ",";
        }
      }
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return aBlocks;
  }

  if (aBlocks->IsEmpty()) {
    SetErrorCode("There are no specified blocks in the given shape");
    return aBlocks;
  }

  anAsciiList.Trunc(anAsciiList.Length() - 1);
  anAsciiList += "]";

  //The explode doesn't change object so no new function is required.
  aFunction = theCompound->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString aDescr (anAsciiList);
  aDescr += " = IBlocksOperations.ExplodeCompoundOfBlocks(";
  TDF_Tool::Entry(theCompound->GetEntry(), anEntry);
  aDescr += anEntry + ", ";
  aDescr += TCollection_AsciiString(theMinNbFaces) + ", ";
  aDescr += TCollection_AsciiString(theMaxNbFaces) + ")";

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aBlocks;
}

//=============================================================================
/*!
 *  GetBlockNearPoint
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetBlockNearPoint
                                                (Handle(GEOM_Object) theCompound,
                                                 Handle(GEOM_Object) thePoint)
{
  SetErrorCode(KO);

  //New object
  Handle(GEOM_Object) aResult;

  // Arguments
  if (theCompound.IsNull() || thePoint.IsNull()) return NULL;

  TopoDS_Shape aBlockOrComp = theCompound->GetValue();
  if (aBlockOrComp.IsNull()) {
    SetErrorCode("Compound is null");
    return NULL;
  }
  if (aBlockOrComp.ShapeType() != TopAbs_COMPOUND &&
      aBlockOrComp.ShapeType() != TopAbs_COMPSOLID) {
    SetErrorCode("Shape is neither a block, nor a compound of blocks");
    return NULL;
  }

  TopoDS_Shape anArg = thePoint->GetValue();
  if (anArg.IsNull()) {
    SetErrorCode("Null shape is given as argument");
    return NULL;
  }
  if (anArg.ShapeType() != TopAbs_VERTEX) {
    SetErrorCode("Element for block identification is not a vertex");
    return NULL;
  }

  //Compute the Block value
  try {
    TopoDS_Shape aShape;
    TopoDS_Vertex aVert = TopoDS::Vertex(anArg);
    gp_Pnt aPnt = BRep_Tool::Pnt(aVert);

    // 1. Explode compound on blocks
    TopTools_MapOfShape mapShape;
    Standard_Integer nbSolids = 0;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_SOLID);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        nbSolids++;
      }
    }

    mapShape.Clear();
    Standard_Integer ind = 1;
    TopTools_Array1OfShape aSolids (1, nbSolids);
    TColStd_Array1OfInteger aDistances (1, nbSolids);
    for (exp.Init(aBlockOrComp, TopAbs_SOLID); exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aSolid = exp.Current();
        aSolids(ind) = aSolid;

        // 2. Classify the point relatively each block
        BRepClass3d_SolidClassifier SC (aSolid, aPnt, Precision::Confusion());
        if (SC.State() == TopAbs_IN) {
          aDistances(ind) = -1;
        } else if (SC.State() == TopAbs_ON) {
          aDistances(ind) = 0;
        } else { // OUT
          aDistances(ind) = 1;
        }
        ind++;
      }
    }

    // 3. Define block, containing the point or having minimum distance to it
    Standard_Integer nearest = 2, nbFound = 0;
    for (ind = 1; ind <= nbSolids; ind++) {
      if (aDistances(ind) < nearest) {
        nearest = aDistances(ind);
        aShape = aSolids(ind);
        nbFound = 1;
      } else if (aDistances(ind) == nearest) {
        nbFound++;
      } else {
      }
    }
    if (nbFound > 1) {
      if (nearest == 0) {
        // The point is on boundary of some blocks and there are
        // no blocks, having the point inside their volume
        SetErrorCode("Multiple blocks near the given point are found");
        return NULL;

      } else if (nearest == 1) {
        // The point is outside some blocks and there are
        // no blocks, having the point inside or on boundary.
        // We will get a nearest block
        Standard_Real minDist = RealLast();
        for (ind = 1; ind <= nbSolids; ind++) {
          if (aDistances(ind) == 1) {
            BRepExtrema_DistShapeShape aDistTool (aVert, aSolids(ind));
            if (!aDistTool.IsDone()) {
              SetErrorCode("Can not find a distance from the given point to one of blocks");
              return NULL;
            }
            Standard_Real aDist = aDistTool.Value();
            if (aDist < minDist) {
              minDist = aDist;
              aShape = aSolids(ind);
            }
          }
        }
      } else { // nearest == -1
        // The point is inside some blocks.
        // We will get a block with nearest center
        Standard_Real minDist = RealLast();
        for (ind = 1; ind <= nbSolids; ind++) {
          if (aDistances(ind) == -1) {
            GProp_GProps aSystem;
            BRepGProp::VolumeProperties(aSolids(ind), aSystem);
            gp_Pnt aCenterMass = aSystem.CentreOfMass();

            Standard_Real aDist = aCenterMass.Distance(aPnt);
            if (aDist < minDist) {
              minDist = aDist;
              aShape = aSolids(ind);
            }
          }
        }
      }
    }

    if (nbFound == 0) {
      SetErrorCode("There are no blocks near the given point");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theCompound, anArray);
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetBlockNearPoint() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theCompound->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.GetBlockNearPoint(");
  TDF_Tool::Entry(theCompound->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetBlockByParts
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::GetBlockByParts
                      (Handle(GEOM_Object)                         theCompound,
                       const Handle(TColStd_HSequenceOfTransient)& theParts)
{
  SetErrorCode(KO);

  Handle(GEOM_Object) aResult;

  if (theCompound.IsNull() || theParts.IsNull()) return NULL;
  TopoDS_Shape aBlockOrComp = theCompound->GetValue();
  if (aBlockOrComp.IsNull()) return NULL;

  //Get the parts
  Standard_Integer argi, aLen = theParts->Length();
  TopTools_Array1OfShape anArgs (1, aLen);
  TCollection_AsciiString anEntry, aPartsDescr;
  for (argi = 1; argi <= aLen; argi++) {
    Handle(GEOM_Object) anObj = Handle(GEOM_Object)::DownCast(theParts->Value(argi));
    Handle(GEOM_Function) aRef = anObj->GetLastFunction();
    if (aRef.IsNull()) return NULL;

    TopoDS_Shape anArg = aRef->GetValue();
    if (anArg.IsNull()) {
      SetErrorCode("Null shape is given as argument");
      return NULL;
    }
    anArgs(argi) = anArg;

    // For Python command
    TDF_Tool::Entry(anObj->GetEntry(), anEntry);
    if (argi > 1) aPartsDescr += ", ";
    aPartsDescr += anEntry;
  }

  //Compute the Block value
  try {
    // 1. Explode compound on solids
    TopTools_MapOfShape mapShape;
    Standard_Integer nbSolids = 0;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_SOLID);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        nbSolids++;
      }
    }

    mapShape.Clear();
    Standard_Integer ind = 1;
    TopTools_Array1OfShape aSolids (1, nbSolids);
    TColStd_Array1OfInteger aNbParts (1, nbSolids);
    for (exp.Init(aBlockOrComp, TopAbs_SOLID); exp.More(); exp.Next(), ind++) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aSolid = exp.Current();
        aSolids(ind) = aSolid;
        aNbParts(ind) = 0;

        // 2. Define quantity of parts, contained in each solid
        TopTools_IndexedMapOfShape aSubShapes;
        TopExp::MapShapes(aSolid, aSubShapes);
        for (argi = 1; argi <= aLen; argi++) {
          if (aSubShapes.Contains(anArgs(argi))) {
            aNbParts(ind)++;
          }
        }
      }
    }

    // 3. Define solid, containing maximum quantity of parts
    Standard_Integer maxNb = 0, nbFound = 0;
    TopoDS_Shape aShape;
    for (ind = 1; ind <= nbSolids; ind++) {
      if (aNbParts(ind) > maxNb) {
        maxNb = aNbParts(ind);
        aShape = aSolids(ind);
        nbFound = 1;
      } else if (aNbParts(ind) == maxNb) {
        nbFound++;
      } else {
      }
    }
    if (nbFound > 1) {
      SetErrorCode("Multiple blocks, containing maximum quantity of the given parts, are found");
      return NULL;
    } else if (nbFound == 0) {
      SetErrorCode("There are no blocks, containing the given parts");
      return NULL;
    } else {
      TopTools_IndexedMapOfShape anIndices;
      TopExp::MapShapes(aBlockOrComp, anIndices);
      Handle(TColStd_HArray1OfInteger) anArray = new TColStd_HArray1OfInteger(1,1);
      anArray->SetValue(1, anIndices.FindIndex(aShape));
      aResult = GetEngine()->AddSubShape(theCompound, anArray);
    }
  } catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //The GetBlockByParts() doesn't change object so no new function is required.
  Handle(GEOM_Function) aFunction = theCompound->GetLastFunction();

  //Make a Python command
  TDF_Tool::Entry(aResult->GetEntry(), anEntry);
  TCollection_AsciiString aDescr (anEntry);
  aDescr += " = IBlocksOperations.GetBlockByParts(";
  TDF_Tool::Entry(theCompound->GetEntry(), anEntry);
  aDescr += anEntry + ", [";
  aDescr += aPartsDescr + "])";

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aResult;
}

//=============================================================================
/*!
 *  GetBlocksByParts
 */
//=============================================================================
Handle(TColStd_HSequenceOfTransient) GEOMImpl_IBlocksOperations::GetBlocksByParts
                      (Handle(GEOM_Object)                         theCompound,
                       const Handle(TColStd_HSequenceOfTransient)& theParts)
{
  SetErrorCode(KO);

  if (theCompound.IsNull() || theParts.IsNull()) return NULL;
  TopoDS_Shape aBlockOrComp = theCompound->GetValue();
  if (aBlockOrComp.IsNull()) return NULL;

  Handle(TColStd_HSequenceOfTransient) aBlocks = new TColStd_HSequenceOfTransient;
  Handle(GEOM_Object) anObj;
  Handle(GEOM_Function) aFunction;

  //Get the parts
  Standard_Integer argi, aLen = theParts->Length();
  TopTools_Array1OfShape anArgs (1, aLen);
  TCollection_AsciiString anEntry, aPartsDescr, anAsciiList = "[";
  for (argi = 1; argi <= aLen; argi++) {
    Handle(GEOM_Object) anObj = Handle(GEOM_Object)::DownCast(theParts->Value(argi));
    Handle(GEOM_Function) aRef = anObj->GetLastFunction();
    if (aRef.IsNull()) return NULL;

    TopoDS_Shape anArg = aRef->GetValue();
    if (anArg.IsNull()) {
      SetErrorCode("Null shape is given as argument");
      return NULL;
    }
    anArgs(argi) = anArg;

    // For Python command
    TDF_Tool::Entry(anObj->GetEntry(), anEntry);
    if (argi > 1) aPartsDescr += ", ";
    aPartsDescr += anEntry;
  }

  //Get the Blocks
  try {
    TopTools_MapOfShape mapShape;
    Standard_Integer nbSolids = 0;
    TopExp_Explorer exp (aBlockOrComp, TopAbs_SOLID);
    for (; exp.More(); exp.Next()) {
      if (mapShape.Add(exp.Current())) {
        nbSolids++;
      }
    }

    mapShape.Clear();
    Standard_Integer ind = 1;
    TopTools_Array1OfShape aSolids (1, nbSolids);
    TColStd_Array1OfInteger aNbParts (1, nbSolids);
    for (exp.Init(aBlockOrComp, TopAbs_SOLID); exp.More(); exp.Next(), ind++) {
      if (mapShape.Add(exp.Current())) {
        TopoDS_Shape aSolid = exp.Current();
        aSolids(ind) = aSolid;
        aNbParts(ind) = 0;

        // 2. Define quantity of parts, contained in each solid
        TopTools_IndexedMapOfShape aSubShapes;
        TopExp::MapShapes(aSolid, aSubShapes);
        for (argi = 1; argi <= aLen; argi++) {
          if (aSubShapes.Contains(anArgs(argi))) {
            aNbParts(ind)++;
          }
        }
      }
    }

    // 3. Define solid, containing maximum quantity of parts
    Standard_Integer maxNb = 0, nbFound = 0;
    for (ind = 1; ind <= nbSolids; ind++) {
      if (aNbParts(ind) > maxNb) {
        maxNb = aNbParts(ind);
        nbFound = 1;
      } else if (aNbParts(ind) == maxNb) {
        nbFound++;
      } else {
      }
    }
    if (nbFound == 0) {
      SetErrorCode("There are no blocks, containing the given parts");
      return NULL;
    }

    // Map shapes
    TopTools_IndexedMapOfShape anIndices;
    TopExp::MapShapes(aBlockOrComp, anIndices);
    Handle(TColStd_HArray1OfInteger) anArray;

    for (ind = 1; ind <= nbSolids; ind++) {
      if (aNbParts(ind) == maxNb) {
        anArray = new TColStd_HArray1OfInteger(1,1);
        anArray->SetValue(1, anIndices.FindIndex(aSolids(ind)));
        anObj = GetEngine()->AddSubShape(theCompound, anArray);
        aBlocks->Append(anObj);

        //Make a Python command
        TDF_Tool::Entry(anObj->GetEntry(), anEntry);
        anAsciiList += anEntry;
        anAsciiList += ",";
      }
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  anAsciiList.Trunc(anAsciiList.Length() - 1);
  anAsciiList += "]";

  //The GetBlocksByParts() doesn't change object so no new function is required.
  aFunction = theCompound->GetLastFunction();

  //Make a Python command
  TCollection_AsciiString aDescr (anAsciiList);
  aDescr += " = IBlocksOperations.GetBlocksByParts(";
  TDF_Tool::Entry(theCompound->GetEntry(), anEntry);
  aDescr += anEntry + ", [";
  aDescr += aPartsDescr + "])";

  TCollection_AsciiString aNewDescr = aFunction->GetDescription() + "\n";
  aNewDescr += aDescr;
  aFunction->SetDescription(aNewDescr);

  SetErrorCode(OK);
  return aBlocks;
}

//=============================================================================
/*!
 *  MakeMultiTransformation1D
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeMultiTransformation1D
                                             (Handle(GEOM_Object)    theObject,
                                              const Standard_Integer theDirFace1,
                                              const Standard_Integer theDirFace2,
                                              const Standard_Integer theNbTimes)
{
  SetErrorCode(KO);

  if (theObject.IsNull()) return NULL;

  Handle(GEOM_Function) aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be moved

  //Add a new Copy object
  Handle(GEOM_Object) aCopy = GetEngine()->AddObject(GetDocID(), GEOM_COPY);

  //Add a translate function
  Handle(GEOM_Function) aFunction =
    aCopy->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_MULTI_TRANSFORM_1D);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlockTrsf aTI (aFunction);
  aTI.SetOriginal(aLastFunction);
  aTI.SetFace1U(theDirFace1);
  aTI.SetFace2U(theDirFace2);
  aTI.SetNbIterU(theNbTimes);

  //Compute the transformation
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to make multi-transformation");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aCopy->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeMultiTransformation1D(");
  TDF_Tool::Entry(theObject->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theDirFace1)+", ");
  aDescr += (TCollection_AsciiString(theDirFace2)+", ");
  aDescr += (TCollection_AsciiString(theNbTimes)+") ");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCopy;
}

//=============================================================================
/*!
 *  MakeMultiTransformation2D
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBlocksOperations::MakeMultiTransformation2D
                                             (Handle(GEOM_Object)    theObject,
                                              const Standard_Integer theDirFace1U,
                                              const Standard_Integer theDirFace2U,
                                              const Standard_Integer theNbTimesU,
                                              const Standard_Integer theDirFace1V,
                                              const Standard_Integer theDirFace2V,
                                              const Standard_Integer theNbTimesV)
{
  SetErrorCode(KO);

  if (theObject.IsNull()) return NULL;

  Handle(GEOM_Function) aLastFunction = theObject->GetLastFunction();
  if (aLastFunction.IsNull()) return NULL; //There is no function which creates an object to be moved

  //Add a new Copy object
  Handle(GEOM_Object) aCopy = GetEngine()->AddObject(GetDocID(), GEOM_COPY);

  //Add a translate function
  Handle(GEOM_Function) aFunction =
    aCopy->AddFunction(GEOMImpl_BlockDriver::GetID(), BLOCK_MULTI_TRANSFORM_2D);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BlockDriver::GetID()) return NULL;

  GEOMImpl_IBlockTrsf aTI (aFunction);
  aTI.SetOriginal(aLastFunction);
  aTI.SetFace1U(theDirFace1U);
  aTI.SetFace2U(theDirFace2U);
  aTI.SetNbIterU(theNbTimesU);
  aTI.SetFace1V(theDirFace1V);
  aTI.SetFace2V(theDirFace2V);
  aTI.SetNbIterV(theNbTimesV);

  //Compute the transformation
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Block driver failed to make multi-transformation");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr;
  TDF_Tool::Entry(aCopy->GetEntry(), anEntry);
  aDescr += (anEntry+" = IBlocksOperations.MakeMultiTransformation2D(");
  TDF_Tool::Entry(theObject->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theDirFace1U)+", ");
  aDescr += (TCollection_AsciiString(theDirFace2U)+", ");
  aDescr += (TCollection_AsciiString(theNbTimesU)+", ");
  aDescr += (TCollection_AsciiString(theDirFace1V)+", ");
  aDescr += (TCollection_AsciiString(theDirFace2V)+", ");
  aDescr += (TCollection_AsciiString(theNbTimesV)+") ");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCopy;
}
