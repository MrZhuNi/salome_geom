using namespace std;

#include "GEOMImpl_I3DPrimOperations.hxx"

#include "utilities.h"
#include "OpUtil.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <TFunction_DriverTable.hxx>
#include <TFunction_Driver.hxx>
#include <TFunction_Logbook.hxx>
#include <TDF_Tool.hxx>

#include "GEOM_Function.hxx"
#include "GEOMImpl_Types.hxx"

#include "GEOMImpl_BoxDriver.hxx"
#include "GEOMImpl_CylinderDriver.hxx"
#include "GEOMImpl_ConeDriver.hxx"
#include "GEOMImpl_SphereDriver.hxx"
#include "GEOMImpl_TorusDriver.hxx"
#include "GEOMImpl_PrismDriver.hxx"
#include "GEOMImpl_PipeDriver.hxx"
#include "GEOMImpl_RevolutionDriver.hxx"
#include "GEOMImpl_ShapeDriver.hxx"
#include "GEOMImpl_FillingDriver.hxx"

#include "GEOMImpl_IBox.hxx"
#include "GEOMImpl_ICylinder.hxx"
#include "GEOMImpl_ICone.hxx"
#include "GEOMImpl_ISphere.hxx"
#include "GEOMImpl_ITorus.hxx"
#include "GEOMImpl_IPrism.hxx"
#include "GEOMImpl_IPipe.hxx"
#include "GEOMImpl_IRevolution.hxx"
#include "GEOMImpl_IShapes.hxx"
#include "GEOMImpl_IFilling.hxx"

#include <Standard_ErrorHandler.hxx> // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC

//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================
GEOMImpl_I3DPrimOperations::GEOMImpl_I3DPrimOperations (GEOM_Engine* theEngine, int theDocID)
: GEOM_IOperations(theEngine, theDocID)
{
  MESSAGE("GEOMImpl_I3DPrimOperations::GEOMImpl_I3DPrimOperations");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================
GEOMImpl_I3DPrimOperations::~GEOMImpl_I3DPrimOperations()
{
  MESSAGE("GEOMImpl_I3DPrimOperations::~GEOMImpl_I3DPrimOperations");
}


//=============================================================================
/*!
 *  MakeBoxDXDYDZ
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeBoxDXDYDZ (double theDX, double theDY, double theDZ)
{
  SetErrorCode(KO);

  //Add a new Box object
  Handle(GEOM_Object) aBox = GetEngine()->AddObject(GetDocID(), GEOM_BOX);

  //Add a new Box function with DX_DY_DZ parameters
  Handle(GEOM_Function) aFunction = aBox->AddFunction(GEOMImpl_BoxDriver::GetID(), BOX_DX_DY_DZ);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BoxDriver::GetID()) return NULL;

  GEOMImpl_IBox aBI (aFunction);

  aBI.SetDX(theDX);
  aBI.SetDY(theDY);
  aBI.SetDZ(theDZ);

  //Compute the box value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Box driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aBox->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeBoxDXDYDZ(";
  aDescr += (TCollection_AsciiString(theDX)+", ");
  aDescr += (TCollection_AsciiString(theDY)+", ");
  aDescr += (TCollection_AsciiString(theDZ)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aBox;
}


//=============================================================================
/*!
 *  MakeBoxTwoPnt
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeBoxTwoPnt (Handle(GEOM_Object) thePnt1,
                                                               Handle(GEOM_Object) thePnt2)
{
  SetErrorCode(KO);

  if (thePnt1.IsNull() || thePnt2.IsNull()) return NULL;

  //Add a new Box object
  Handle(GEOM_Object) aBox = GetEngine()->AddObject(GetDocID(), GEOM_BOX);

  //Add a new Box function for creation a box relatively to two points
  Handle(GEOM_Function) aFunction = aBox->AddFunction(GEOMImpl_BoxDriver::GetID(), BOX_TWO_PNT);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_BoxDriver::GetID()) return aBox;

  GEOMImpl_IBox aBI (aFunction);

  Handle(GEOM_Function) aRefFunction1 = thePnt1->GetLastFunction();
  Handle(GEOM_Function) aRefFunction2 = thePnt2->GetLastFunction();

  if (aRefFunction1.IsNull() || aRefFunction2.IsNull()) return aBox;

  aBI.SetRef1(aRefFunction1);
  aBI.SetRef2(aRefFunction2);

  //Compute the Box value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Box driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aBox->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeBoxTwoPnt(";
  TDF_Tool::Entry(thePnt1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePnt2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aBox;
}


//=============================================================================
/*!
 *  MakeCylinderRH
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeCylinderRH (double theR, double theH)
{
  SetErrorCode(KO);

  //Add a new Cylinder object
  Handle(GEOM_Object) aCylinder = GetEngine()->AddObject(GetDocID(), GEOM_CYLINDER);

  //Add a new Cylinder function with R and H parameters
  Handle(GEOM_Function) aFunction = aCylinder->AddFunction(GEOMImpl_CylinderDriver::GetID(), CYLINDER_R_H);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_CylinderDriver::GetID()) return NULL;

  GEOMImpl_ICylinder aCI (aFunction);

  aCI.SetR(theR);
  aCI.SetH(theH);

  //Compute the Cylinder value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Cylinder driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aCylinder->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeCylinderRH(";
  aDescr += (TCollection_AsciiString(theR)+", ");
  aDescr += (TCollection_AsciiString(theH)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCylinder;
}


//=============================================================================
/*!
 *  MakeCylinderPntVecRH
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeCylinderPntVecRH (Handle(GEOM_Object) thePnt,
                                                                      Handle(GEOM_Object) theVec,
                                                                      double theR, double theH)
{
  SetErrorCode(KO);

  if (thePnt.IsNull() || theVec.IsNull()) return NULL;

  //Add a new Cylinder object
  Handle(GEOM_Object) aCylinder = GetEngine()->AddObject(GetDocID(), GEOM_CYLINDER);

  //Add a new Cylinder function for creation a cylinder relatively to point and vector
  Handle(GEOM_Function) aFunction =
    aCylinder->AddFunction(GEOMImpl_CylinderDriver::GetID(), CYLINDER_PNT_VEC_R_H);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_CylinderDriver::GetID()) return NULL;

  GEOMImpl_ICylinder aCI (aFunction);

  Handle(GEOM_Function) aRefPnt = thePnt->GetLastFunction();
  Handle(GEOM_Function) aRefVec = theVec->GetLastFunction();

  if (aRefPnt.IsNull() || aRefVec.IsNull()) return NULL;

  aCI.SetPoint(aRefPnt);
  aCI.SetVector(aRefVec);
  aCI.SetR(theR);
  aCI.SetH(theH);

  //Compute the Cylinder value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Cylinder driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aCylinder->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeCylinder(";
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVec->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theR)+", ");
  aDescr += (TCollection_AsciiString(theH)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCylinder;
}


//=============================================================================
/*!
 *  MakeConeR1R2H
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeConeR1R2H (double theR1, double theR2,
                                                               double theH)
{
  SetErrorCode(KO);

  //Add a new Cone object
  Handle(GEOM_Object) aCone = GetEngine()->AddObject(GetDocID(), GEOM_CONE);

  //Add a new Cone function with R and H parameters
  Handle(GEOM_Function) aFunction =
    aCone->AddFunction(GEOMImpl_ConeDriver::GetID(), CONE_R1_R2_H);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_ConeDriver::GetID()) return NULL;

  GEOMImpl_ICone aCI (aFunction);

  aCI.SetR1(theR1);
  aCI.SetR2(theR2);
  aCI.SetH(theH);

  //Compute the Cone value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Cone driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aCone->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeConeR1R2H(";
  aDescr += (TCollection_AsciiString(theR1)+", ");
  aDescr += (TCollection_AsciiString(theR2)+", ");
  aDescr += (TCollection_AsciiString(theH)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCone;
}


//=============================================================================
/*!
 *  MakeConePntVecR1R2H
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeConePntVecR1R2H (Handle(GEOM_Object) thePnt,
                                                                     Handle(GEOM_Object) theVec,
                                                                     double theR1, double theR2,
                                                                     double theH)
{
  SetErrorCode(KO);

  if (thePnt.IsNull() || theVec.IsNull()) return NULL;

  //Add a new Cone object
  Handle(GEOM_Object) aCone = GetEngine()->AddObject(GetDocID(), GEOM_CONE);

  //Add a new Cone function for creation a cone relatively to point and vector
  Handle(GEOM_Function) aFunction =
    aCone->AddFunction(GEOMImpl_ConeDriver::GetID(), CONE_PNT_VEC_R1_R2_H);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_ConeDriver::GetID()) return NULL;

  GEOMImpl_ICone aCI (aFunction);

  Handle(GEOM_Function) aRefPnt = thePnt->GetLastFunction();
  Handle(GEOM_Function) aRefVec = theVec->GetLastFunction();

  if (aRefPnt.IsNull() || aRefVec.IsNull()) return NULL;

  aCI.SetPoint(aRefPnt);
  aCI.SetVector(aRefVec);
  aCI.SetR1(theR1);
  aCI.SetR2(theR2);
  aCI.SetH(theH);

  //Compute the Cone value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Cone driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aCone->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeCone(";
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVec->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theR1)+", ");
  aDescr += (TCollection_AsciiString(theR2)+", ");
  aDescr += (TCollection_AsciiString(theH)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aCone;
}


//=============================================================================
/*!
 *  MakeSphereR
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeSphereR (double theR)
{
  SetErrorCode(KO);

  //Add a new Sphere object
  Handle(GEOM_Object) aSphere = GetEngine()->AddObject(GetDocID(), GEOM_SPHERE);

  //Add a new Sphere function with R parameter
  Handle(GEOM_Function) aFunction = aSphere->AddFunction(GEOMImpl_SphereDriver::GetID(), SPHERE_R);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_SphereDriver::GetID()) return NULL;

  GEOMImpl_ISphere aCI (aFunction);

  aCI.SetR(theR);

  //Compute the Sphere value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Sphere driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aSphere->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeSphereR(";
  aDescr += (TCollection_AsciiString(theR)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aSphere;
}


//=============================================================================
/*!
 *  MakeSpherePntR
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeSpherePntR (Handle(GEOM_Object) thePnt,
                                                                double theR)
{
  SetErrorCode(KO);

  if (thePnt.IsNull()) return NULL;

  //Add a new Point object
  Handle(GEOM_Object) aSphere = GetEngine()->AddObject(GetDocID(), GEOM_SPHERE);

  //Add a new Sphere function for creation a sphere relatively to point
  Handle(GEOM_Function) aFunction = aSphere->AddFunction(GEOMImpl_SphereDriver::GetID(), SPHERE_PNT_R);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_SphereDriver::GetID()) return NULL;

  GEOMImpl_ISphere aCI (aFunction);

  Handle(GEOM_Function) aRefPnt = thePnt->GetLastFunction();

  if (aRefPnt.IsNull()) return NULL;

  aCI.SetPoint(aRefPnt);
  aCI.SetR(theR);

  //Compute the Sphere value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Sphere driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aSphere->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeSpherePntR(";
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theR)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aSphere;
}


//=============================================================================
/*!
 *  MakeTorusRR
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeTorusRR
                                           (double theRMajor, double theRMinor)
{
  SetErrorCode(KO);

  //Add a new Torus object
  Handle(GEOM_Object) anEll = GetEngine()->AddObject(GetDocID(), GEOM_TORUS);

  //Add a new Torus function
  Handle(GEOM_Function) aFunction =
    anEll->AddFunction(GEOMImpl_TorusDriver::GetID(), TORUS_RR);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_TorusDriver::GetID()) return NULL;

  GEOMImpl_ITorus aCI (aFunction);

  aCI.SetRMajor(theRMajor);
  aCI.SetRMinor(theRMinor);

  //Compute the Torus value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Torus driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(anEll->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeTorusRR(";
  aDescr += (TCollection_AsciiString(theRMajor)+", ");
  aDescr += (TCollection_AsciiString(theRMinor)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return anEll;
}

//=============================================================================
/*!
 *  MakeTorusPntVecRR
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeTorusPntVecRR
                       (Handle(GEOM_Object) thePnt, Handle(GEOM_Object) theVec,
                        double theRMajor, double theRMinor)
{
  SetErrorCode(KO);

  if (thePnt.IsNull() || theVec.IsNull()) return NULL;

  //Add a new Torus object
  Handle(GEOM_Object) anEll = GetEngine()->AddObject(GetDocID(), GEOM_TORUS);

  //Add a new Torus function
  Handle(GEOM_Function) aFunction =
    anEll->AddFunction(GEOMImpl_TorusDriver::GetID(), TORUS_PNT_VEC_RR);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_TorusDriver::GetID()) return NULL;

  GEOMImpl_ITorus aCI (aFunction);

  Handle(GEOM_Function) aRefPnt = thePnt->GetLastFunction();
  Handle(GEOM_Function) aRefVec = theVec->GetLastFunction();

  if (aRefPnt.IsNull() || aRefVec.IsNull()) return NULL;

  aCI.SetCenter(aRefPnt);
  aCI.SetVector(aRefVec);
  aCI.SetRMajor(theRMajor);
  aCI.SetRMinor(theRMinor);

  //Compute the Torus value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Torus driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(anEll->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeTorus(";
  TDF_Tool::Entry(thePnt->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVec->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theRMajor)+", ");
  aDescr += (TCollection_AsciiString(theRMinor)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return anEll;
}


//=============================================================================
/*!
 *  MakePrismVecH
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakePrismVecH (Handle(GEOM_Object) theBase,
                                                               Handle(GEOM_Object) theVec,
                                                               double theH)
{
  SetErrorCode(KO);

  if (theBase.IsNull() || theVec.IsNull()) return NULL;

  //Add a new Prism object
  Handle(GEOM_Object) aPrism = GetEngine()->AddObject(GetDocID(), GEOM_PRISM);

  //Add a new Prism function for creation a Prism relatively to vector
  Handle(GEOM_Function) aFunction =
    aPrism->AddFunction(GEOMImpl_PrismDriver::GetID(), PRISM_BASE_VEC_H);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PrismDriver::GetID()) return NULL;

  GEOMImpl_IPrism aCI (aFunction);

  Handle(GEOM_Function) aRefBase = theBase->GetLastFunction();
  Handle(GEOM_Function) aRefVec  = theVec->GetLastFunction();

  if (aRefBase.IsNull() || aRefVec.IsNull()) return NULL;

  aCI.SetBase(aRefBase);
  aCI.SetVector(aRefVec);
  aCI.SetH(theH);

  //Compute the Prism value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Prism driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aPrism->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakePrismVecH(";
  TDF_Tool::Entry(theBase->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theVec->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theH)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPrism;
}

//=============================================================================
/*!
 *  MakePrismTwoPnt
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakePrismTwoPnt
       (Handle(GEOM_Object) theBase,
        Handle(GEOM_Object) thePoint1, Handle(GEOM_Object) thePoint2)
{
  SetErrorCode(KO);

  if (theBase.IsNull() || thePoint1.IsNull() || thePoint2.IsNull()) return NULL;

  //Add a new Prism object
  Handle(GEOM_Object) aPrism = GetEngine()->AddObject(GetDocID(), GEOM_PRISM);

  //Add a new Prism function for creation a Prism relatively to two points
  Handle(GEOM_Function) aFunction =
    aPrism->AddFunction(GEOMImpl_PrismDriver::GetID(), PRISM_BASE_TWO_PNT);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PrismDriver::GetID()) return NULL;

  GEOMImpl_IPrism aCI (aFunction);

  Handle(GEOM_Function) aRefBase = theBase->GetLastFunction();
  Handle(GEOM_Function) aRefPnt1 = thePoint1->GetLastFunction();
  Handle(GEOM_Function) aRefPnt2 = thePoint2->GetLastFunction();

  if (aRefBase.IsNull() || aRefPnt1.IsNull() || aRefPnt2.IsNull()) return NULL;

  aCI.SetBase(aRefBase);
  aCI.SetFirstPoint(aRefPnt1);
  aCI.SetLastPoint(aRefPnt2);

  //Compute the Prism value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Prism driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aPrism->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakePrism(";
  TDF_Tool::Entry(theBase->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint1->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(thePoint2->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPrism;
}


//=============================================================================
/*!
 *  MakePipe
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakePipe (Handle(GEOM_Object) theBase,
                                                          Handle(GEOM_Object) thePath)
{
  SetErrorCode(KO);

  if (theBase.IsNull() || thePath.IsNull()) return NULL;

  //Add a new Pipe object
  Handle(GEOM_Object) aPipe = GetEngine()->AddObject(GetDocID(), GEOM_PIPE);

  //Add a new Pipe function
  Handle(GEOM_Function) aFunction =
    aPipe->AddFunction(GEOMImpl_PipeDriver::GetID(), PIPE_BASE_PATH);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_PipeDriver::GetID()) return NULL;

  GEOMImpl_IPipe aCI (aFunction);

  Handle(GEOM_Function) aRefBase = theBase->GetLastFunction();
  Handle(GEOM_Function) aRefPath = thePath->GetLastFunction();

  if (aRefBase.IsNull() || aRefPath.IsNull()) return NULL;

  aCI.SetBase(aRefBase);
  aCI.SetPath(aRefPath);

  //Compute the Pipe value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Pipe driver failed");
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
  TDF_Tool::Entry(aPipe->GetEntry(), anEntry);
  aDescr += (anEntry + " = geompy.MakePipe(");
  TDF_Tool::Entry(theBase->GetEntry(), anEntry);
  aDescr += (anEntry + ", ");
  TDF_Tool::Entry(thePath->GetEntry(), anEntry);
  aDescr += (anEntry + ")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aPipe;
}


//=============================================================================
/*!
 *  MakeRevolutionAxisAngle
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeRevolutionAxisAngle (Handle(GEOM_Object) theBase,
                                                                         Handle(GEOM_Object) theAxis,
                                                                         double theAngle)
{
  SetErrorCode(KO);

  if (theBase.IsNull() || theAxis.IsNull()) return NULL;

  //Add a new Revolution object
  Handle(GEOM_Object) aRevolution = GetEngine()->AddObject(GetDocID(), GEOM_REVOLUTION);

  //Add a new Revolution function for creation a revolution relatively to axis
  Handle(GEOM_Function) aFunction =
    aRevolution->AddFunction(GEOMImpl_RevolutionDriver::GetID(), REVOLUTION_BASE_AXIS_ANGLE);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_RevolutionDriver::GetID()) return NULL;

  GEOMImpl_IRevolution aCI (aFunction);

  Handle(GEOM_Function) aRefBase = theBase->GetLastFunction();
  Handle(GEOM_Function) aRefAxis = theAxis->GetLastFunction();

  if (aRefBase.IsNull() || aRefAxis.IsNull()) return NULL;

  aCI.SetBase(aRefBase);
  aCI.SetAxis(aRefAxis);
  aCI.SetAngle(theAngle);

  //Compute the Revolution value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Revolution driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aRevolution->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeRevolution(";
  TDF_Tool::Entry(theBase->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  TDF_Tool::Entry(theAxis->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theAngle)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aRevolution;
}


//=============================================================================
/*!
 *  MakeSolidShell
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeSolidShell (Handle(GEOM_Object) theShell)
{
  SetErrorCode(KO);

  if (theShell.IsNull()) return NULL;

  //Add a new Solid object
  Handle(GEOM_Object) aSolid = GetEngine()->AddObject(GetDocID(), GEOM_SOLID);

  //Add a new Solid function for creation a solid from a shell
  Handle(GEOM_Function) aFunction =
    aSolid->AddFunction(GEOMImpl_ShapeDriver::GetID(), SOLID_SHELL);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_ShapeDriver::GetID()) return NULL;

  GEOMImpl_IShapes aCI (aFunction);

  Handle(GEOM_Function) aRefShell = theShell->GetLastFunction();

  if (aRefShell.IsNull()) return NULL;

  aCI.SetBase(aRefShell);

  //Compute the Solid value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Solid driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aSolid->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeSolid(";
  TDF_Tool::Entry(theShell->GetEntry(), anEntry);
  aDescr += (anEntry+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aSolid;
}

//=============================================================================
/*!
 *  MakeFilling
 */
//=============================================================================
Handle(GEOM_Object) GEOMImpl_I3DPrimOperations::MakeFilling (Handle(GEOM_Object) theShape, int theMinDeg, int theMaxDeg, double theTol2D, double theTol3D, int theNbIter)
{
  SetErrorCode(KO);

  if (theShape.IsNull()) return NULL;

  //Add a new Filling object
  Handle(GEOM_Object) aFilling = GetEngine()->AddObject(GetDocID(), GEOM_FILLING);

  //Add a new Filling function for creation a filling  from a compound
  Handle(GEOM_Function) aFunction = aFilling->AddFunction(GEOMImpl_FillingDriver::GetID(), BASIC_FILLING);
  if (aFunction.IsNull()) return NULL;

  //Check if the function is set correctly
  if (aFunction->GetDriverGUID() != GEOMImpl_FillingDriver::GetID()) return NULL;

  GEOMImpl_IFilling aFI (aFunction);

  Handle(GEOM_Function) aRefShape = theShape->GetLastFunction();

  if (aRefShape.IsNull()) return NULL;

  aFI.SetShape(aRefShape);
  aFI.SetMinDeg(theMinDeg);
  aFI.SetMaxDeg(theMaxDeg);
  aFI.SetTol2D(theTol2D);
  aFI.SetTol3D(theTol3D);
  aFI.SetNbIter(theNbIter);

  //Compute the Solid value
  try {
    if (!GetSolver()->ComputeFunction(aFunction)) {
      SetErrorCode("Fiiling driver failed");
      return NULL;
    }
  }
  catch (Standard_Failure) {
    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    SetErrorCode(aFail->GetMessageString());
    return NULL;
  }

  //Make a Python command
  TCollection_AsciiString anEntry, aDescr("");
  TDF_Tool::Entry(aFilling->GetEntry(), anEntry);
  aDescr += anEntry;
  aDescr += " = geompy.MakeFilling(";
  TDF_Tool::Entry(theShape->GetEntry(), anEntry);
  aDescr += (anEntry+", ");
  aDescr += (TCollection_AsciiString(theMinDeg)+", ");
  aDescr += (TCollection_AsciiString(theMaxDeg)+", ");
  aDescr += (TCollection_AsciiString(theTol2D)+", ");
  aDescr += (TCollection_AsciiString(theTol3D)+", ");
  aDescr += (TCollection_AsciiString(theNbIter)+")");

  aFunction->SetDescription(aDescr);

  SetErrorCode(OK);
  return aFilling;
}
