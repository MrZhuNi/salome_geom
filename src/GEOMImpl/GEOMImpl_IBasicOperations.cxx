using namespace std;

#include "GEOMImpl_IBasicOperations.hxx"

#include "utilities.h"
#include "OpUtil.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <TFunction_DriverTable.hxx>
#include <TFunction_Driver.hxx>
#include <TFunction_Logbook.hxx>
#include <TDF_Tool.hxx>

#include "GEOM_Function.hxx"

#include "GEOMImpl_PointDriver.hxx"
#include "GEOMImpl_VectorDriver.hxx"
#include "GEOMImpl_LineDriver.hxx"
#include "GEOMImpl_PlaneDriver.hxx"
#include "GEOMImpl_MarkerDriver.hxx"

#include "GEOMImpl_IPoint.hxx"
#include "GEOMImpl_IVector.hxx"
#include "GEOMImpl_ILine.hxx"
#include "GEOMImpl_IPlane.hxx"
#include "GEOMImpl_IMarker.hxx"

#include "GEOMImpl_Types.hxx"

#include <Standard_ErrorHandler.hxx> // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC

//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================
GEOMImpl_IBasicOperations::GEOMImpl_IBasicOperations(GEOM_Engine* theEngine, int theDocID)
: GEOM_IOperations(theEngine, theDocID)
{
  MESSAGE("GEOMImpl_IBasicOperations::GEOMImpl_IBasicOperations");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================
GEOMImpl_IBasicOperations::~GEOMImpl_IBasicOperations()
{
  MESSAGE("GEOMImpl_IBasicOperations::~GEOMImpl_IBasicOperations");
}


//=============================================================================
/*!
 *  MakePointXYZ
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePointXYZ
                                        (double theX, double theY, double theZ)
{
  SetErrorCode(KO);

  //Add a new Point object
  Handle(GEOM_Object) aPoint = GetEngine()->AddObject(GetDocID(), GEOM_POINT);

  //Add a new Point function with XYZ parameters
  Handle(GEOM_Function) aFunction =
    aPoint->AddFunction(GEOMImpl_PointDriver::GetID(), POINT_XYZ);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PointDriver::GetID()) return NULL;

  GEOMImpl_IPoint aPI(aFunction);

  aPI.SetX(theX);
  aPI.SetY(theY);
  aPI.SetZ(theZ);

  //Compute the point value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Point driver failed");
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
  TDF_Tool::Entry(aPoint->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeVertex(");
  aDescr += (TCollection_AsciiString(theX)+", ");
  aDescr += (TCollection_AsciiString(theY)+", ");
  aDescr += (TCollection_AsciiString(theZ)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPoint;
}

//=============================================================================
/*!
 *  MakePointWithReference
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePointWithReference
      (Handle(GEOM_Object) theReference, double theX, double theY, double theZ)
{
  SetErrorCode(KO);

  if (theReference.IsNull()) return NULL;

  //Add a new Point object
  Handle(GEOM_Object) aPoint = GetEngine()->AddObject(GetDocID(), GEOM_POINT);

  //Add a new Point function for creation a point relativley another point
  Handle(GEOM_Function) aFunction = aPoint->AddFunction(GEOMImpl_PointDriver::GetID(), POINT_XYZ_REF);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PointDriver::GetID()) return NULL;

  GEOMImpl_IPoint aPI(aFunction);

  Handle(GEOM_Function) aRefFunction = theReference->GetLastFunction();
  if (aRefFunction.IsNull()) return NULL;

  aPI.SetRef(aRefFunction);
  aPI.SetX(theX);
  aPI.SetY(theY);
  aPI.SetZ(theZ);

  //Compute the point value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Point driver failed");
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
  TDF_Tool::Entry(aPoint->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeVertexWithRef(");
  TDF_Tool::Entry(theReference->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theX)+", ");
  aDescr += (TCollection_AsciiString(theY)+", ");
  aDescr += (TCollection_AsciiString(theZ)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPoint;
}

//=============================================================================
/*!
 *  MakePointOnCurve
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePointOnCurve
                            (Handle(GEOM_Object) theCurve, double theParameter)
{
  SetErrorCode(KO);

  if (theCurve.IsNull()) return NULL;

  //Add a new Point object
  Handle(GEOM_Object) aPoint = GetEngine()->AddObject(GetDocID(), GEOM_POINT);

  //Add a new Point function for creation a point relativley another point
  Handle(GEOM_Function) aFunction = aPoint->AddFunction(GEOMImpl_PointDriver::GetID(), POINT_CURVE_PAR);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PointDriver::GetID()) return NULL;

  GEOMImpl_IPoint aPI (aFunction);

  Handle(GEOM_Function) aRefFunction = theCurve->GetLastFunction();
  if (aRefFunction.IsNull()) return NULL;

  aPI.SetCurve(aRefFunction);
  aPI.SetParameter(theParameter);

  //Compute the point value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Point driver failed");
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
  TDF_Tool::Entry(aPoint->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeVertexOnCurve(");
  TDF_Tool::Entry(theCurve->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theParameter)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPoint;
}


//=============================================================================
/*!
 *  MakeVectorDXDYDZ
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakeVectorDXDYDZ
                                     (double theDX, double theDY, double theDZ)
{
  SetErrorCode(KO);

  //Add a new Vector object
  Handle(GEOM_Object) aVector = GetEngine()->AddObject(GetDocID(), GEOM_VECTOR);

  //Add a new Vector function with DXDYDZ parameters
  Handle(GEOM_Function) aFunction =
    aVector->AddFunction(GEOMImpl_VectorDriver::GetID(), VECTOR_DX_DY_DZ);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_VectorDriver::GetID()) return NULL;

  GEOMImpl_IVector aPI (aFunction);

  aPI.SetDX(theDX);
  aPI.SetDY(theDY);
  aPI.SetDZ(theDZ);

  //Compute the Vector value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Vector driver failed");
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
  TDF_Tool::Entry(aVector->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeVectorDXDYDZ(");
  aDescr += (TCollection_AsciiString(theDX)+", ");
  aDescr += (TCollection_AsciiString(theDY)+", ");
  aDescr += (TCollection_AsciiString(theDZ)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aVector;
}

//=============================================================================
/*!
 *  MakeVectorTwoPnt
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakeVectorTwoPnt
                     (Handle(GEOM_Object) thePnt1, Handle(GEOM_Object) thePnt2)
{
  SetErrorCode(KO);

  if (thePnt1.IsNull() || thePnt2.IsNull()) return NULL;

  //Add a new Vector object
  Handle(GEOM_Object) aVector = GetEngine()->AddObject(GetDocID(), GEOM_VECTOR);

  //Add a new Vector function
  Handle(GEOM_Function) aFunction =
    aVector->AddFunction(GEOMImpl_VectorDriver::GetID(), VECTOR_TWO_PNT);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_VectorDriver::GetID()) return NULL;

  GEOMImpl_IVector aPI (aFunction);

  Handle(GEOM_Function) aRef1 = thePnt1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = thePnt2->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull()) return NULL;

  aPI.SetPoint1(aRef1);
  aPI.SetPoint2(aRef2);

  //Compute the Vector value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Vector driver failed");
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
  TDF_Tool::Entry(aVector->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeVector(");
  TDF_Tool::Entry(thePnt1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aVector;
}


//=============================================================================
/*!
 *  MakeLine
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakeLine
                     (Handle(GEOM_Object) thePnt, Handle(GEOM_Object) theDir)
{
  SetErrorCode(KO);

  if (thePnt.IsNull() || theDir.IsNull()) return NULL;

  //Add a new Line object
  Handle(GEOM_Object) aLine = GetEngine()->AddObject(GetDocID(), GEOM_LINE);

  //Add a new Line function
  Handle(GEOM_Function) aFunction =
    aLine->AddFunction(GEOMImpl_LineDriver::GetID(), LINE_PNT_DIR);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_LineDriver::GetID()) return NULL;

  GEOMImpl_ILine aPI (aFunction);

  Handle(GEOM_Function) aRef1 = thePnt->GetLastFunction();
  Handle(GEOM_Function) aRef2 = theDir->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull()) return NULL;

  aPI.SetPoint1(aRef1);
  aPI.SetPoint2(aRef2);

  //Compute the Line value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Line driver failed");
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
  TDF_Tool::Entry(aLine->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeLine(");
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theDir->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aLine;
}

//=============================================================================
/*!
 *  MakeLineTwoPnt
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakeLineTwoPnt
                     (Handle(GEOM_Object) thePnt1, Handle(GEOM_Object) thePnt2)
{
  SetErrorCode(KO);

  if (thePnt1.IsNull() || thePnt2.IsNull()) return NULL;

  //Add a new Line object
  Handle(GEOM_Object) aLine = GetEngine()->AddObject(GetDocID(), GEOM_LINE);

  //Add a new Line function
  Handle(GEOM_Function) aFunction =
    aLine->AddFunction(GEOMImpl_LineDriver::GetID(), LINE_TWO_PNT);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_LineDriver::GetID()) return NULL;

  GEOMImpl_ILine aPI (aFunction);

  Handle(GEOM_Function) aRef1 = thePnt1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = thePnt2->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull()) return NULL;

  aPI.SetPoint1(aRef1);
  aPI.SetPoint2(aRef2);

  //Compute the Line value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Line driver failed");
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
  TDF_Tool::Entry(aLine->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakeLineTwoPnt(");
  TDF_Tool::Entry(thePnt1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aLine;
}


//=============================================================================
/*!
 *  MakePlaneThreePnt
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePlaneThreePnt
                     (Handle(GEOM_Object) thePnt1, Handle(GEOM_Object) thePnt2,
                      Handle(GEOM_Object) thePnt3, double theSize)
{
  SetErrorCode(KO);

  if (thePnt1.IsNull() || thePnt2.IsNull() || thePnt3.IsNull()) return NULL;

  //Add a new Plane object
  Handle(GEOM_Object) aPlane = GetEngine()->AddObject(GetDocID(), GEOM_PLANE);

  //Add a new Plane function
  Handle(GEOM_Function) aFunction =
    aPlane->AddFunction(GEOMImpl_PlaneDriver::GetID(), PLANE_THREE_PNT);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PlaneDriver::GetID()) return NULL;

  GEOMImpl_IPlane aPI (aFunction);

  Handle(GEOM_Function) aRef1 = thePnt1->GetLastFunction();
  Handle(GEOM_Function) aRef2 = thePnt2->GetLastFunction();
  Handle(GEOM_Function) aRef3 = thePnt3->GetLastFunction();
  if (aRef1.IsNull() || aRef2.IsNull() || aRef3.IsNull()) return NULL;

  aPI.SetPoint1(aRef1);
  aPI.SetPoint2(aRef2);
  aPI.SetPoint3(aRef3);
  aPI.SetSize(theSize);

  //Compute the Plane value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Plane driver failed");
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
  TDF_Tool::Entry(aPlane->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakePlaneThreePnt(");
  TDF_Tool::Entry(thePnt1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt2->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt3->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += TCollection_AsciiString(theSize) + ")";

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPlane;
}

//=============================================================================
/*!
 *  MakePlanePntVec
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePlanePntVec
                       (Handle(GEOM_Object) thePnt, Handle(GEOM_Object) theVec,
                        double theSize)
{
  SetErrorCode(KO);

  if (thePnt.IsNull() || theVec.IsNull()) return NULL;

  //Add a new Plane object
  Handle(GEOM_Object) aPlane = GetEngine()->AddObject(GetDocID(), GEOM_PLANE);

  //Add a new Plane function
  Handle(GEOM_Function) aFunction =
    aPlane->AddFunction(GEOMImpl_PlaneDriver::GetID(), PLANE_PNT_VEC);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PlaneDriver::GetID()) return NULL;

  GEOMImpl_IPlane aPI (aFunction);

  Handle(GEOM_Function) aRefPnt = thePnt->GetLastFunction();
  Handle(GEOM_Function) aRefVec = theVec->GetLastFunction();
  if (aRefPnt.IsNull() || aRefVec.IsNull()) return NULL;

  aPI.SetPoint(aRefPnt);
  aPI.SetVector(aRefVec);
  aPI.SetSize(theSize);

  //Compute the Plane value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Plane driver failed");
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
  TDF_Tool::Entry(aPlane->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakePlane(");
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVec->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += TCollection_AsciiString(theSize) + ")";

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPlane;
}

//=============================================================================
/*!
 *  MakePlaneFace
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakePlaneFace
                       (Handle(GEOM_Object) theFace, double theSize)
{
  SetErrorCode(KO);

  if (theFace.IsNull()) return NULL;

  //Add a new Plane object
  Handle(GEOM_Object) aPlane = GetEngine()->AddObject(GetDocID(), GEOM_PLANE);

  //Add a new Plane function
  Handle(GEOM_Function) aFunction =
    aPlane->AddFunction(GEOMImpl_PlaneDriver::GetID(), PLANE_FACE);

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PlaneDriver::GetID()) return NULL;

  GEOMImpl_IPlane aPI (aFunction);

  Handle(GEOM_Function) aRef = theFace->GetLastFunction();
  if (aRef.IsNull()) return NULL;

  aPI.SetFace(aRef);
  aPI.SetSize(theSize);

  //Compute the Plane value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Plane driver failed");
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
  TDF_Tool::Entry(aPlane->GetEntry(), anEntry);
  aDescr += (anEntry+" = geompy.MakePlaneFace(");
  TDF_Tool::Entry(theFace->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += TCollection_AsciiString(theSize) + ")";

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPlane;
}


//=============================================================================
/*!
 *  MakeMarker
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_IBasicOperations::MakeMarker
                                  (double theOX,  double theOY,  double theOZ,
                                   double theXDX, double theXDY, double theXDZ,
                                   double theYDX, double theYDY, double theYDZ)
{
  SetErrorCode(KO);

  //Add a new Marker object
  Handle(GEOM_Object) aMarker = GetEngine()->AddObject(GetDocID(), GEOM_MARKER);

  //Add a new Marker function
  Handle(GEOM_Function) aFunction =
    aMarker->AddFunction(GEOMImpl_MarkerDriver::GetID(), MARKER_CS);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_MarkerDriver::GetID()) return NULL;

  GEOMImpl_IMarker aPI(aFunction);

  aPI.SetOrigin(theOX, theOY, theOZ);
  aPI.SetXDir(theXDX, theXDY, theXDZ);
  aPI.SetYDir(theYDX, theYDY, theYDZ);

  //Compute the marker value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Marker driver failed");
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
  TDF_Tool::Entry(aMarker->GetEntry(), anEntry);
  aDescr += anEntry + " = geompy.MakeMarker(";
  aDescr += TCollection_AsciiString(theOX) + ", ";
  aDescr += TCollection_AsciiString(theOY) + ", ";
  aDescr += TCollection_AsciiString(theOZ) + ", ";
  aDescr += TCollection_AsciiString(theXDX) + ", ";
  aDescr += TCollection_AsciiString(theXDY) + ", ";
  aDescr += TCollection_AsciiString(theXDZ) + ", ";
  aDescr += TCollection_AsciiString(theYDX) + ", ";
  aDescr += TCollection_AsciiString(theYDY) + ", ";
  aDescr += TCollection_AsciiString(theYDZ) + ")";

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aMarker;
}
