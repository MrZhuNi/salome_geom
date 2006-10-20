// Copyright (C) 2006 SAMTECH
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
// File:	NMTDS_Iterator.cxx
// Created:	Sun May 07 15:04:41 2006
// Author:	Peter KURNEV
//		<peter@PREFEX>


#include <NMTDS_Iterator.ixx>
#include <NMTDS_CArray1OfIndexRange.hxx>
#include <NMTDS_IndexRange.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Shape.hxx>
#include <TColStd_ListOfInteger.hxx>
#include <TColStd_Array1OfListOfInteger.hxx>
#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <TColStd_MapOfInteger.hxx>
#include <TColStd_MapIteratorOfMapOfInteger.hxx>
#include <NMTDS_PassKeyBoolean.hxx>
#include <NMTDS_MapOfPassKeyBoolean.hxx>

static 
  Bnd_Box& ComputeBoxEx(const Standard_Integer iX,
			NMTDS_ShapesDataStructure* pDS,
			Bnd_Box* pBoxes);
static
  Standard_Integer TypeToInteger(const TopAbs_ShapeEnum aType);

static
  Standard_Integer TypeToInteger(const TopAbs_ShapeEnum aType1,
				 const TopAbs_ShapeEnum aType2);
static
  void PropagateState(const Standard_Integer aIDS1,
		      const Standard_Integer aIDS2,
		      const Standard_Integer iStatus,
		      const Standard_Integer aNb,
		      NMTDS_ShapesDataStructure* pDS,
		      Standard_Integer *pTable);
static
  void FillSuccessors(const Standard_Integer aIDS,
		      NMTDS_ShapesDataStructure* pDS,
		      TColStd_MapOfInteger& aMS);

//=======================================================================
//function : NMTDS_Iterator
//purpose  : 
//=======================================================================
  NMTDS_Iterator::NMTDS_Iterator()
{
  myPDS=NULL; 
  myLength=0;
}
//=======================================================================
//function : ~NMTDS_Iterator
//purpose  : 
//=======================================================================
  NMTDS_Iterator::~NMTDS_Iterator()
{
}
//=======================================================================
// function: SetDS
// purpose: 
//=======================================================================
  void NMTDS_Iterator::SetDS(const NMTDS_PShapesDataStructure& aDS)
{
  myPDS=aDS;
}
//=======================================================================
// function: DS
// purpose: 
//=======================================================================
  const NMTDS_ShapesDataStructure&  NMTDS_Iterator::DS()const
{
  return *myPDS;
}
//=======================================================================
// function: ExpectedLength
// purpose: 
//=======================================================================
  Standard_Integer NMTDS_Iterator::ExpectedLength() const
{
  return myLength;
}
//=======================================================================
// function: Initialize
// purpose: 
//=======================================================================
  void NMTDS_Iterator::Initialize(const TopAbs_ShapeEnum aType1,
				  const TopAbs_ShapeEnum aType2)
{
  Standard_Integer iX;
  //
  iX=TypeToInteger(aType1, aType2);
  if (iX>=0) {
    myIterator.Initialize(myLists[iX]);
    myLength=myLists[iX].Extent();
  }
  else {
    myIterator.Initialize(myEmptyList);
    myLength=0;
  }
}
//=======================================================================
// function: More
// purpose: 
//=======================================================================
  Standard_Boolean NMTDS_Iterator::More()const
{
  return myIterator.More();
}
//=======================================================================
// function: Next
// purpose: 
//=======================================================================
  void NMTDS_Iterator::Next()
{
  myIterator.Next();
}
//=======================================================================
// function: Current
// purpose: 
//=======================================================================
  void NMTDS_Iterator::Current(Standard_Integer& aIndex1,
			       Standard_Integer& aIndex2,
			       Standard_Boolean& aWithSubShape) const
{
  const NMTDS_PassKeyBoolean& aPKB=myIterator.Value();
  aPKB.Ids(aIndex1, aIndex2);
  aWithSubShape=aPKB.Flag();
}
//=======================================================================
// function: Prepare
// purpose: 
//=======================================================================
  void NMTDS_Iterator::Prepare()
{
  Standard_Boolean bFlag;
  Standard_Integer aNb, aNb2, aNbR, iR, jR1, jR2, jR;
  Standard_Integer *pTable, i, j, k, iT1, iT2, i1, i2, j1, j2, iT, jT;
  Standard_Integer iStatus, iX;
  Bnd_Box *pBoxes, aBox;
  TColStd_ListIteratorOfListOfInteger aItIT, aItJT;
  TopAbs_ShapeEnum aTi, aTj;
  NMTDS_PassKeyBoolean aPKXB; 
  NMTDS_MapOfPassKeyBoolean aMPKXB;
  //
  if (myPDS==NULL){
    return;
  }
  //
  myLength=0;
  for (i=0; i<6; ++i) {
    myLists[i].Clear();
  }
  // 
  aNb=myPDS->NumberOfShapesOfTheObject();
  aNb2=aNb*aNb;
  //
  pTable=new Standard_Integer [aNb2];
  pBoxes=new Bnd_Box [aNb];
  //
  // 1. Set status to Unknown
  for (i=0; i<aNb2; ++i) {
    pTable[i]=0; // Unknown
  }
  //
  // 2. Treat Ranges
  //    Set status to SameRange;
  //    Compute enlarged boxes  ;
  const NMTDS_CArray1OfIndexRange& aRanges=myPDS->Ranges();
  aNbR=aRanges.Extent();
  for (iR=1; iR<=aNbR; ++iR) {
    const NMTDS_IndexRange& aRange=aRanges(iR);
    jR1=aRange.First();
    jR2=aRange.Last();
    for (i=jR1; i<=jR2; ++i) {
      for (j=i; j<=jR2; ++j) {
	k=(i-1)*aNb+(j-1);
	pTable[k]=4; //SameRange
      }
      //
      ComputeBoxEx(i, myPDS, pBoxes);
    }
  }
  //
  // 3. Intersect shapes by ranges
  iT1=TypeToInteger(TopAbs_COMPOUND);
  iT2=TypeToInteger(TopAbs_VERTEX);
  //
  for (iR=1; iR<aNbR; ++iR) {
    TColStd_Array1OfListOfInteger aALIR(iT1, iT2), aALJR(iT1, iT2);
    //
    const NMTDS_IndexRange& aRi=aRanges(iR);
    i1=aRi.First();
    i2=aRi.Last();
    for (i=i1; i<=i2; ++i) {
      aALIR(TypeToInteger(myPDS->GetShapeType(i))).Append(i);
    }
    //
    for (jR=2; jR<=aNbR; ++jR) {
      const NMTDS_IndexRange& aRj=aRanges(jR);
      j1=aRj.First();
      j2=aRj.Last();
      for (j=j1; j<=j2; ++j) {
	aALJR(TypeToInteger(myPDS->GetShapeType(j))).Append(j);
      }
    }
    //
    for (iT=iT1; iT<=iT2; ++iT) {
      const TColStd_ListOfInteger& aLIT=aALIR(iT);
      if (aLIT.IsEmpty()) {
	continue;
      }
      for (jT=iT1; jT<=iT2; ++jT) {
	const TColStd_ListOfInteger& aLJT=aALJR(jT);
	if (aLJT.IsEmpty()) {
	  continue;
	}
	//
	aItIT.Initialize(aLIT);
	for (; aItIT.More(); aItIT.Next()) {
	  aItJT.Initialize(aLJT);
	  for (; aItJT.More(); aItJT.Next()) {
	    i1=aItIT.Value();
	    j1=aItJT.Value();
	    if (i1>j1) {
	      iX=i1;
	      i1=j1;
	      j1=iX;
	    }
	    //
	    i=i1-1;
	    j=j1-1;
	    k=i*aNb+j;
	    //
	    if (pTable[k]!=0 && pTable[k]!=2) {
	      continue;
	    }
	    // enlarged boxes
	    const Bnd_Box& aBoxEi=pBoxes[i];
	    const Bnd_Box& aBoxEj=pBoxes[j];
	    if (aBoxEi.IsOut(aBoxEj)) {
	      iStatus=3; // Non-intersected
	      PropagateState(i1, j1, iStatus, aNb, myPDS, pTable); 
	    }
	    else {
	      iStatus=1; // Bounding boxes are intersected
	      const Bnd_Box& aBoxi=myPDS->GetBoundingBox(i1);
	      const Bnd_Box& aBoxj=myPDS->GetBoundingBox(j1);
	      if (aBoxi.IsOut(aBoxj)) {
		iStatus=2; // Bounding boxes of Sub-shapes are intersected
	      }
	      pTable[k]=iStatus;
	      //
	      aTi=myPDS->GetShapeType(i1);
	      aTj=myPDS->GetShapeType(j1);
	      iX=TypeToInteger(aTi, aTj);
	      //
	      if (iX>=0) {
		aPKXB.SetIds(i1, j1);
		if (aMPKXB.Add(aPKXB)) {
		  bFlag=Standard_False;
		  if(iStatus==2) {
		    bFlag=!bFlag;
		  }
		  aPKXB.SetFlag(bFlag);
		  myLists[iX].Append(aPKXB);
		}
	      }//if (iX>=0) {
	    }// else {
	  }//for (; aItJT.More(); aItJT.Next()) {
	}//for (; aItIT.More(); aItIT.Next()) {
      }//for (jT=iT1; jT<=iT2; ++jT) {
    }//for (iT=iT1; iT<=iT2; ++iT) {
  }
  //
  //
  delete [] (Bnd_Box*)pBoxes;
  delete [] (Standard_Integer*)pTable;
}
//=======================================================================
// function: PropagateState
// purpose: 
//=======================================================================
void PropagateState(const Standard_Integer aIDS1,
		    const Standard_Integer aIDS2,
		    const Standard_Integer iStatus,
		    const Standard_Integer aNb,
		    NMTDS_ShapesDataStructure* pDS,
		    Standard_Integer *pTable)
{
  Standard_Integer i, j, k, i1, j1, iX, i11, j11;
  TColStd_MapOfInteger aMS1, aMS2;
  TColStd_MapIteratorOfMapOfInteger aIt1, aIt2;
  //
  FillSuccessors(aIDS1, pDS, aMS1);
  FillSuccessors(aIDS2, pDS, aMS2);
  //
  aIt1.Initialize(aMS1);
  for (; aIt1.More(); aIt1.Next()) {
    i1=aIt1.Key();
    //
    aIt2.Initialize(aMS2);
    for (; aIt2.More(); aIt2.Next()) {
      j1=aIt2.Key();
      //
      i11=i1;
      j11=j1;
      //
      if (i1>j1) {
	iX=i11;
	i11=j11;
	j11=iX;
      }
      i=i11-1;
      j=j11-1;
      k=i*aNb+j;
      if (!pTable[k]) {
	pTable[k]=iStatus;
      }
    }
  }
}
//=======================================================================
// function: FillSuccessors
// purpose: 
//=======================================================================
void FillSuccessors(const Standard_Integer aIDS,
		    NMTDS_ShapesDataStructure* pDS,
		    TColStd_MapOfInteger& aMS)
{
  Standard_Integer i, aNbS, iDS1;
  //
  aMS.Add(aIDS);
  aNbS=pDS->NumberOfSuccessors(aIDS);
  for (i=1; i<=aNbS; ++i) {
    iDS1=pDS->GetSuccessor(aIDS, i);
    FillSuccessors(iDS1, pDS, aMS);
  }
}
//=======================================================================
// function: ComputeBoxEx
// purpose: 
//=======================================================================
Bnd_Box& ComputeBoxEx(const Standard_Integer aIndexDS,
		     NMTDS_ShapesDataStructure* pDS,
		     Bnd_Box* pBoxes)
{
  Standard_Boolean bIsVoid;
  Standard_Integer iX;
  //
  iX=aIndexDS-1;
  Bnd_Box& aBoxEx=pBoxes[iX];
  //
  bIsVoid=aBoxEx.IsVoid();
  if (bIsVoid) {
    Standard_Integer i, aNb, iS;
    //
    const Bnd_Box& aBox=pDS->GetBoundingBox(aIndexDS);
    aBoxEx.Add(aBox);
    //
    aNb=pDS->NumberOfSuccessors(aIndexDS);
    for (i=1; i<=aNb; ++i) {
      iS=pDS->GetSuccessor(aIndexDS, i);
      Bnd_Box& aBoxS=ComputeBoxEx(iS, pDS, pBoxes);
      aBoxEx.Add(aBoxS);
    }
  }
  return aBoxEx;
}
//=======================================================================
// function: TypeToInteger
// purpose: 
//=======================================================================
Standard_Integer TypeToInteger(const TopAbs_ShapeEnum aType)
{
  return (Standard_Integer)aType;
}
//=======================================================================
// function: TypeToInteger
// purpose: 
//=======================================================================
Standard_Integer TypeToInteger(const TopAbs_ShapeEnum aType1,
			       const TopAbs_ShapeEnum aType2)
{
  Standard_Integer iRet, iT1, iT2, iX;
  //
  iRet=-1;
  iT1=(Standard_Integer)aType1;
  iT2=(Standard_Integer)aType2;
  //
  iX=iT2*10+iT1;
  switch (iX) {
    case 77:
      iRet=5; // VV
      break;
    case 76:
    case 67:
      iRet=4; // VE
      break;
    case 74:
    case 47:
      iRet=2; // VF
      break;
    case 66:
      iRet=3; // EE
      break;
    case 64:
    case 46:
      iRet=1; // EF
      break;
    case 44:
      iRet=0; // FF
      break;
    default:
      break;
  }
  return iRet; 
}

/*
  printf(" *** pTable ***\n");
  for (i=0; i<aNb; ++i) {
    for (j=0; j<aNb; ++j) {
      k=i*aNb+j;
      printf(" %2d", pTable[k]);
    }
    printf("\n");
  }
  */
