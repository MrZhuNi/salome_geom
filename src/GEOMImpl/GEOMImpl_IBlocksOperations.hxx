
#ifndef _GEOMImpl_IBlocksOperations_HXX_
#define _GEOMImpl_IBlocksOperations_HXX_

#include "GEOM_IOperations.hxx"

#include <TopTools_ListOfShape.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

#include <list>

class GEOM_Engine;
class Handle(GEOM_Object);
class Handle(TColStd_HArray1OfInteger);

class GEOMImpl_IBlocksOperations : public GEOM_IOperations {
 public:
  GEOMImpl_IBlocksOperations(GEOM_Engine* theEngine, int theDocID);
  ~GEOMImpl_IBlocksOperations();

  // Creation of blocks and block faces
  Handle(GEOM_Object) MakeQuadFaceFourVertices (Handle(GEOM_Object) thePoint1,
                                                Handle(GEOM_Object) thePoint2,
                                                Handle(GEOM_Object) thePoint3,
                                                Handle(GEOM_Object) thePoint4);

  Handle(GEOM_Object) MakeQuadFaceFourEdges (Handle(GEOM_Object) theEdge1,
                                             Handle(GEOM_Object) theEdge2,
                                             Handle(GEOM_Object) theEdge3,
                                             Handle(GEOM_Object) theEdge4);

  Handle(GEOM_Object) MakeQuadFaceTwoEdges (Handle(GEOM_Object) theEdge1,
                                            Handle(GEOM_Object) theEdge2);

  Handle(GEOM_Object) MakeHexaSolidSixFaces (Handle(GEOM_Object) theFace1,
                                             Handle(GEOM_Object) theFace2,
                                             Handle(GEOM_Object) theFace3,
                                             Handle(GEOM_Object) theFace4,
                                             Handle(GEOM_Object) theFace5,
                                             Handle(GEOM_Object) theFace6);

  Handle(GEOM_Object) MakeHexaSolidTwoFaces (Handle(GEOM_Object) theFace1,
                                             Handle(GEOM_Object) theFace2);

  // Extract elements of blocks and blocks compounds
  Handle(GEOM_Object) GetEdge (Handle(GEOM_Object) theShape,
                               Handle(GEOM_Object) thePoint1,
                               Handle(GEOM_Object) thePoint2);

  Handle(GEOM_Object) GetFaceByPoints (Handle(GEOM_Object) theShape,
                                       Handle(GEOM_Object) thePoint1,
                                       Handle(GEOM_Object) thePoint2,
                                       Handle(GEOM_Object) thePoint3,
                                       Handle(GEOM_Object) thePoint4);

  Handle(GEOM_Object) GetFaceByEdges (Handle(GEOM_Object) theShape,
                                      Handle(GEOM_Object) theEdge1,
                                      Handle(GEOM_Object) theEdge2);

  Handle(GEOM_Object) GetOppositeFace (Handle(GEOM_Object) theBlock,
                                       Handle(GEOM_Object) theFace);

  Handle(GEOM_Object) GetFaceNearPoint (Handle(GEOM_Object) theBlock,
                                        Handle(GEOM_Object) thePoint);

  Handle(GEOM_Object) GetFaceByNormale (Handle(GEOM_Object) theBlock,
                                        Handle(GEOM_Object) theVector);

  // Extract blocks from blocks compounds
  Standard_Boolean IsCompoundOfBlocks (Handle(GEOM_Object)    theCompound,
                                       const Standard_Integer theMinNbFaces,
                                       const Standard_Integer theMaxNbFaces,
                                       Standard_Integer&      theNbBlocks);

  Handle(TColStd_HSequenceOfTransient) ExplodeCompoundOfBlocks
                                      (Handle(GEOM_Object)    theCompound,
                                       const Standard_Integer theMinNbFaces,
                                       const Standard_Integer theMaxNbFaces);

  Handle(GEOM_Object) GetBlockNearPoint (Handle(GEOM_Object) theCompound,
                                         Handle(GEOM_Object) thePoint);

  Handle(GEOM_Object) GetBlockByParts
                     (Handle(GEOM_Object)                         theCompound,
                      const Handle(TColStd_HSequenceOfTransient)& theParts);

  Handle(TColStd_HSequenceOfTransient) GetBlocksByParts
                     (Handle(GEOM_Object)                         theCompound,
                      const Handle(TColStd_HSequenceOfTransient)& theParts);

  // Operations on blocks with gluing of result
  Handle(GEOM_Object) MakeMultiTransformation1D (Handle(GEOM_Object)    theBlock,
                                                 const Standard_Integer theDirFace1,
                                                 const Standard_Integer theDirFace2,
                                                 const Standard_Integer theNbTimes);

  Handle(GEOM_Object) MakeMultiTransformation2D (Handle(GEOM_Object)    theBlock,
                                                 const Standard_Integer theDirFace1U,
                                                 const Standard_Integer theDirFace2U,
                                                 const Standard_Integer theNbTimesU,
                                                 const Standard_Integer theDirFace1V,
                                                 const Standard_Integer theDirFace2V,
                                                 const Standard_Integer theNbTimesV);

  Handle(GEOM_Object) MakeMultiTranslation1D (Handle(GEOM_Object)    theBlock,
                                              const Standard_Integer theDirFace,
                                              const Standard_Integer theNbTimes);

  Handle(GEOM_Object) MakeMultiTranslation2D (Handle(GEOM_Object)    theBlock,
                                              const Standard_Integer theDirFace1,
                                              const Standard_Integer theNbTimes1,
                                              const Standard_Integer theDirFace2,
                                              const Standard_Integer theNbTimes2);
};

#endif
