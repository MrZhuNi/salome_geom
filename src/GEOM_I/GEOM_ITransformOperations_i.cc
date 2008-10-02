// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#include <Standard_Stream.hxx>

#include "GEOM_ITransformOperations_i.hh"

#include "utilities.h"
#include "OpUtil.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TCollection_AsciiString.hxx>
#include "GEOM_Engine.hxx"
#include "GEOM_Object.hxx"

#define SUBSHAPE_ERROR "Sub shape cannot be transformed"

//=============================================================================
/*!
 *   constructor:
 */
//=============================================================================

GEOM_ITransformOperations_i::GEOM_ITransformOperations_i (PortableServer::POA_ptr thePOA,
							  GEOM::GEOM_Gen_ptr theEngine,
							  ::GEOMImpl_ITransformOperations* theImpl)
     :GEOM_IOperations_i(thePOA, theEngine, theImpl)
{
  MESSAGE("GEOM_ITransformOperations_i::GEOM_ITransformOperations_i");
}

//=============================================================================
/*!
 *  destructor
 */
//=============================================================================

GEOM_ITransformOperations_i::~GEOM_ITransformOperations_i()
{
  MESSAGE("GEOM_ITransformOperations_i::~GEOM_ITransformOperations_i");
}


//=============================================================================
/*!
 *  TranslateTwoPoints
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateTwoPoints
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint1,
					      GEOM::GEOM_Object_ptr thePoint2)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject;

  if (thePoint1 == NULL || thePoint2 == NULL || theObject == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if(!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the first point of translation
  CORBA::String_var aP1Entry = thePoint1->GetEntry();
  Handle(GEOM_Object) aPoint1 =
    GetOperations()->GetEngine()->GetObject(thePoint1->GetStudyID(), aP1Entry);
  if (aPoint1.IsNull()) return aGEOMObject._retn();

  //Get the second point of translation
  CORBA::String_var aP2Entry = thePoint2->GetEntry();
  Handle(GEOM_Object) aPoint2 =
    GetOperations()->GetEngine()->GetObject(thePoint2->GetStudyID(), aP2Entry);
  if (aPoint2.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  GetOperations()->TranslateTwoPoints(anObject, aPoint1, aPoint2);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  TranslateTwoPointsCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateTwoPointsCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint1,
					      GEOM::GEOM_Object_ptr thePoint2)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (thePoint1 == NULL || thePoint2 == NULL || theObject == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the first point of translation
  CORBA::String_var aP1Entry = thePoint1->GetEntry();
  Handle(GEOM_Object) aPoint1 =
    GetOperations()->GetEngine()->GetObject(thePoint1->GetStudyID(), aP1Entry);
  if (aPoint1.IsNull()) return aGEOMObject._retn();

  //Get the second point of translation
  CORBA::String_var aP2Entry = thePoint2->GetEntry();
  Handle(GEOM_Object) aPoint2 =
    GetOperations()->GetEngine()->GetObject(thePoint2->GetStudyID(), aP2Entry);
  if (aPoint2.IsNull()) return aGEOMObject._retn();

  //Create the translated shape
  Handle(GEOM_Object) anObject =
    GetOperations()->TranslateTwoPointsCopy(aBasicObject, aPoint1, aPoint2);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  TranslateDXDYDZ
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateDXDYDZ
                                                   (GEOM::GEOM_Object_ptr theObject,
						    CORBA::Double theDX, CORBA::Double theDY, CORBA::Double theDZ)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject ;

  if (theObject == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if(!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  GetOperations()->TranslateDXDYDZ(anObject, theDX, theDY, theDZ);

  return aGEOMObject._retn();
}


//=============================================================================
/*!
 *  TranslateDXDYDZCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateDXDYDZCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      CORBA::Double theDX, CORBA::Double theDY, CORBA::Double theDZ)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Create the translated shape
  Handle(GEOM_Object) anObject =
    GetOperations()->TranslateDXDYDZCopy(aBasicObject, theDX, theDY, theDZ);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}


//=============================================================================
/*!
 *  TranslateVector
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateVector
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr theVector)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if(!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aVEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  GetOperations()->TranslateVector(anObject, aVector);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  TranslateVectorCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateVectorCopy
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr theVector)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aVEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  Handle(GEOM_Object) anObject = GetOperations()->TranslateVectorCopy(aBasicObject, aVector);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  TranslateVectorDistance
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::TranslateVectorDistance
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr theVector,
					     CORBA::Double theDistance,
					     CORBA::Boolean theCopy)
{
  GEOM::GEOM_Object_var aGEOMObject;
  GetOperations()->SetNotDone(); //Set a not done flag

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theCopy && !theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  if (!theCopy)
    aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aVecEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVecEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  if (theCopy) {
    Handle(GEOM_Object) anObject = GetOperations()->
      TranslateVectorDistance(aBasicObject, aVector, theDistance, theCopy);
    if (!GetOperations()->IsDone() || anObject.IsNull())
      return aGEOMObject._retn();

    return GetObject(anObject);
  }

  GetOperations()->TranslateVectorDistance(aBasicObject, aVector, theDistance, theCopy);
  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  Rotate
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::Rotate (GEOM::GEOM_Object_ptr theObject,
							   GEOM::GEOM_Object_ptr theAxis,
							   CORBA::Double theAngle)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theAxis == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if(!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the axis of revolution
  CORBA::String_var anAEntry = theAxis->GetEntry();
  Handle(GEOM_Object) anAxis =
    GetOperations()->GetEngine()->GetObject(theAxis->GetStudyID(), anAEntry);
  if (anAxis.IsNull()) return aGEOMObject._retn();

  //Perform the rotation
  GetOperations()->Rotate(anObject, anAxis, theAngle);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  RotateCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::RotateCopy (GEOM::GEOM_Object_ptr theObject,
							       GEOM::GEOM_Object_ptr theAxis,
							       CORBA::Double theAngle)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theAxis == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the axis of rotation
  CORBA::String_var anAEntry = theAxis->GetEntry();
  Handle(GEOM_Object) anAxis =
    GetOperations()->GetEngine()->GetObject(theAxis->GetStudyID(), anAEntry);
  if (anAxis.IsNull()) return aGEOMObject._retn();

  //Perform the rotation
  Handle(GEOM_Object) anObject = GetOperations()->RotateCopy(aBasicObject, anAxis, theAngle);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}


//=============================================================================
/*!
 *  MirrorPlane
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorPlane
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr thePlane)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || thePlane == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the plane
  CORBA::String_var aPlnEntry = thePlane->GetEntry();
  Handle(GEOM_Object) aPlane =
    GetOperations()->GetEngine()->GetObject(thePlane->GetStudyID(), aPlnEntry);
  if (aPlane.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  GetOperations()->MirrorPlane(anObject, aPlane);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  MirrorPlaneCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorPlaneCopy
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr thePlane)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || thePlane == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aPlnEntry = thePlane->GetEntry();
  Handle(GEOM_Object) aPlane =
    GetOperations()->GetEngine()->GetObject(thePlane->GetStudyID(), aPlnEntry);
  if (aPlane.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  Handle(GEOM_Object) anObject = GetOperations()->MirrorPlaneCopy(aBasicObject, aPlane);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MirrorAxis
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorAxis
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr theAxis)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theAxis == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if(!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the axis
  CORBA::String_var anAEntry = theAxis->GetEntry();
  Handle(GEOM_Object) aAxis =
    GetOperations()->GetEngine()->GetObject(theAxis->GetStudyID(), anAEntry);
  if (aAxis.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  GetOperations()->MirrorAxis(anObject, aAxis);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  MirrorAxisCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorAxisCopy
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr theAxis)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theAxis == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var anAEntry = theAxis->GetEntry();
  Handle(GEOM_Object) aAxis =
    GetOperations()->GetEngine()->GetObject(theAxis->GetStudyID(), anAEntry);
  if (aAxis.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  Handle(GEOM_Object) anObject = GetOperations()->MirrorAxisCopy(aBasicObject, aAxis);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MirrorPoint
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorPoint
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr thePoint)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || thePoint == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the point
  CORBA::String_var aPntEntry = thePoint->GetEntry();
  Handle(GEOM_Object) aPoint =
    GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
  if (aPoint.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  GetOperations()->MirrorPoint(anObject, aPoint);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  MirrorPointCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MirrorPointCopy
                                            (GEOM::GEOM_Object_ptr theObject,
					     GEOM::GEOM_Object_ptr thePoint)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || thePoint == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aPntEntry = thePoint->GetEntry();
  Handle(GEOM_Object) aPoint =
    GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
  if (aPoint.IsNull()) return aGEOMObject._retn();

  //Perform the mirror
  Handle(GEOM_Object) anObject = GetOperations()->MirrorPointCopy(aBasicObject, aPoint);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}


//=============================================================================
/*!
 *  OffsetShape
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::OffsetShape
                                             (GEOM::GEOM_Object_ptr theObject,
					      CORBA::Double theOffset)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Create the offset shape
  GetOperations()->OffsetShape(aBasicObject, theOffset);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  OffsetShapeCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::OffsetShapeCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      CORBA::Double theOffset)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL) return aGEOMObject._retn();

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Create the offset shape
  Handle(GEOM_Object) anObject = GetOperations()->OffsetShapeCopy(aBasicObject, theOffset);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}


//=============================================================================
/*!
 *  ScaleShape
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::ScaleShape
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint,
					      CORBA::Double         theFactor)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject->_is_nil()) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the point
  Handle(GEOM_Object) aPoint;
  if (!thePoint->_is_nil()) {
    CORBA::String_var aPntEntry = thePoint->GetEntry();
    aPoint = GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
    if (aPoint.IsNull()) return aGEOMObject._retn();
  }

  //Perform the scale
  GetOperations()->ScaleShape(anObject, aPoint, theFactor);

  return  aGEOMObject._retn();
}

//=============================================================================
/*!
 *  ScaleShapeCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::ScaleShapeCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint,
					      CORBA::Double         theFactor)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject->_is_nil()) return aGEOMObject._retn();

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the point
  Handle(GEOM_Object) aPoint;
  if (!thePoint->_is_nil()) {
    CORBA::String_var aPntEntry = thePoint->GetEntry();
    aPoint = GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
    if (aPoint.IsNull()) return aGEOMObject._retn();
  }

  //Perform the scale
  Handle(GEOM_Object) anObject =
    GetOperations()->ScaleShapeCopy(aBasicObject, aPoint, theFactor);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  ScaleShapeAlongAxes
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::ScaleShapeAlongAxes
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint,
					      CORBA::Double         theFactorX,
					      CORBA::Double         theFactorY,
					      CORBA::Double         theFactorZ)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject->_is_nil()) return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the point
  Handle(GEOM_Object) aPoint;
  if (!thePoint->_is_nil()) {
    CORBA::String_var aPntEntry = thePoint->GetEntry();
    aPoint = GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
    if (aPoint.IsNull()) return aGEOMObject._retn();
  }

  //Perform the scale
  GetOperations()->ScaleShapeAlongAxes
    (anObject, aPoint, theFactorX, theFactorY, theFactorZ, /*doCopy*/false);

  return  aGEOMObject._retn();
}

//=============================================================================
/*!
 *  ScaleShapeAlongAxesCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::ScaleShapeAlongAxesCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr thePoint,
					      CORBA::Double         theFactorX,
					      CORBA::Double         theFactorY,
					      CORBA::Double         theFactorZ)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject->_is_nil()) return aGEOMObject._retn();

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the point
  Handle(GEOM_Object) aPoint;
  if (!thePoint->_is_nil()) {
    CORBA::String_var aPntEntry = thePoint->GetEntry();
    aPoint = GetOperations()->GetEngine()->GetObject(thePoint->GetStudyID(), aPntEntry);
    if (aPoint.IsNull()) return aGEOMObject._retn();
  }

  //Perform the scale
  Handle(GEOM_Object) anObject = GetOperations()->ScaleShapeAlongAxes
    (aBasicObject, aPoint, theFactorX, theFactorY, theFactorZ, /*doCopy*/true);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  PositionShape
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::PositionShape
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr theStartLCS,
					      GEOM::GEOM_Object_ptr theEndLCS)
{
  GEOM::GEOM_Object_var aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theEndLCS == NULL)
    return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the Start LCS (may be NULL for positioning from global LCS)
  Handle(GEOM_Object) aStartLCS = NULL;
  if (theStartLCS != NULL && !CORBA::is_nil(theStartLCS)) {
    CORBA::String_var aStartLCSEntry = theStartLCS->GetEntry();
    aStartLCS = GetOperations()->GetEngine()->GetObject(theStartLCS->GetStudyID(), aStartLCSEntry);
    if (aStartLCS.IsNull()) return aGEOMObject._retn();
  }

  //Get the End LCS
  CORBA::String_var anEndLCSEntry = theEndLCS->GetEntry();
  Handle(GEOM_Object) aEndLCS =
    GetOperations()->GetEngine()->GetObject(theEndLCS->GetStudyID(), anEndLCSEntry);
  if (aEndLCS.IsNull()) return aGEOMObject._retn();

  //Perform the Position
  GetOperations()->PositionShape(anObject, aStartLCS, aEndLCS);

  return  aGEOMObject._retn();
}

//=============================================================================
/*!
 *  PositionShapeCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::PositionShapeCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr theStartLCS,
					      GEOM::GEOM_Object_ptr theEndLCS)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theObject == NULL || theEndLCS == NULL)
    return aGEOMObject._retn();

  //Get the basic object
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the Start LCS (may be NULL for positioning from global LCS)
  Handle(GEOM_Object) aStartLCS = NULL;
  if (theStartLCS != NULL && !CORBA::is_nil(theStartLCS)) {
    CORBA::String_var aStartLCSEntry = theStartLCS->GetEntry();
    aStartLCS = GetOperations()->GetEngine()->GetObject(theStartLCS->GetStudyID(), aStartLCSEntry);
    if (aStartLCS.IsNull()) return aGEOMObject._retn();
  }

  //Get the End LCS
  CORBA::String_var anEndLCSEntry = theEndLCS->GetEntry();
  Handle(GEOM_Object) aEndLCS =
    GetOperations()->GetEngine()->GetObject(theEndLCS->GetStudyID(), anEndLCSEntry);
  if (aEndLCS.IsNull()) return aGEOMObject._retn();

  //Perform the position
  Handle(GEOM_Object) anObject =
    GetOperations()->PositionShapeCopy(aBasicObject, aStartLCS, aEndLCS);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MultiTranslate1D
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MultiTranslate1D
                           (GEOM::GEOM_Object_ptr theObject,
			    GEOM::GEOM_Object_ptr theVector,
			    CORBA::Double theStep, CORBA::Long theNbTimes)
{
  //Set a not done flag
  GetOperations()->SetNotDone();

  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector of translation
  CORBA::String_var aVecEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVecEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  Handle(GEOM_Object) anObject =
    GetOperations()->Translate1D(aBasicObject, aVector, theStep, theNbTimes);
  if (!GetOperations()->IsDone() || anObject.IsNull()) return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MultiTranslate2D
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MultiTranslate2D (GEOM::GEOM_Object_ptr theObject,
								     GEOM::GEOM_Object_ptr theVector1,
								     CORBA::Double theStep1,
								     CORBA::Long theNbTimes1,
								     GEOM::GEOM_Object_ptr theVector2,
								     CORBA::Double theStep2,
								     CORBA::Long theNbTimes2)
{
  //Set a not done flag
  GetOperations()->SetNotDone();

  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theVector1 == NULL || theVector2 == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the vector1 of translation
  CORBA::String_var aVec1Entry = theVector1->GetEntry();
  Handle(GEOM_Object) aVector1 =
    GetOperations()->GetEngine()->GetObject(theVector1->GetStudyID(), aVec1Entry);
  if (aVector1.IsNull()) return aGEOMObject._retn();

  //Get the vector2 of translation
  CORBA::String_var aVec2Entry = theVector2->GetEntry();
  Handle(GEOM_Object) aVector2 =
    GetOperations()->GetEngine()->GetObject(theVector2->GetStudyID(), aVec2Entry);
  if (aVector2.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  Handle(GEOM_Object) anObject = GetOperations()->Translate2D
    (aBasicObject, aVector1, theStep1, theNbTimes1, aVector2, theStep2, theNbTimes2);
  if (!GetOperations()->IsDone() || anObject.IsNull()) return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MultiRotate1D
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MultiRotate1D (GEOM::GEOM_Object_ptr theObject,
								  GEOM::GEOM_Object_ptr theVector,
								  CORBA::Long theNbTimes)
{
  //Set a not done flag
  GetOperations()->SetNotDone();

  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the a directon of rotation
  CORBA::String_var aVecEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVecEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the rotation
  Handle(GEOM_Object) anObject = GetOperations()->Rotate1D(aBasicObject, aVector, theNbTimes);
  if (!GetOperations()->IsDone() || anObject.IsNull()) return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  MultiRotate2D
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::MultiRotate2D (GEOM::GEOM_Object_ptr theObject,
								  GEOM::GEOM_Object_ptr theVector,
								  CORBA::Double theAngle,
								  CORBA::Long theNbTimes1,
								  CORBA::Double theStep,
								  CORBA::Long theNbTimes2)
{
  //Set a not done flag
  GetOperations()->SetNotDone();

  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL || theVector == NULL) return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the a directon of rotation
  CORBA::String_var aVecEntry = theVector->GetEntry();
  Handle(GEOM_Object) aVector =
    GetOperations()->GetEngine()->GetObject(theVector->GetStudyID(), aVecEntry);
  if (aVector.IsNull()) return aGEOMObject._retn();

  //Perform the rotation
  Handle(GEOM_Object) anObject = GetOperations()->Rotate2D
    (aBasicObject, aVector, theAngle, theNbTimes1, theStep, theNbTimes2);
  if (!GetOperations()->IsDone() || anObject.IsNull()) return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  RotateThreePoints
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::RotateThreePoints
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr theCentPoint,
					      GEOM::GEOM_Object_ptr thePoint1,
					      GEOM::GEOM_Object_ptr thePoint2)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject;

  if (theCentPoint == NULL || thePoint1 == NULL || thePoint2 == NULL || theObject == NULL)
    return aGEOMObject._retn();

  //check if the object is a subshape
  if (!theObject->IsMainShape()) {
    GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
    return aGEOMObject._retn();
  }

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Get the central point of rotation
  CORBA::String_var aCPEntry = theCentPoint->GetEntry();
  Handle(GEOM_Object) aCentPoint =
    GetOperations()->GetEngine()->GetObject(theCentPoint->GetStudyID(), aCPEntry);
  if (aCentPoint.IsNull()) return aGEOMObject._retn();

  //Get the first point
  CORBA::String_var aP1Entry = thePoint1->GetEntry();
  Handle(GEOM_Object) aPoint1 =
    GetOperations()->GetEngine()->GetObject(thePoint1->GetStudyID(), aP1Entry);
  if (aPoint1.IsNull()) return aGEOMObject._retn();

  //Get the second point
  CORBA::String_var aP2Entry = thePoint2->GetEntry();
  Handle(GEOM_Object) aPoint2 =
    GetOperations()->GetEngine()->GetObject(thePoint2->GetStudyID(), aP2Entry);
  if (aPoint2.IsNull()) return aGEOMObject._retn();

  //Perform the translation
  GetOperations()->RotateThreePoints(anObject, aCentPoint, aPoint1, aPoint2);

  return aGEOMObject._retn();
}

//=============================================================================
/*!
 *  RotateThreePointsCopy
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::RotateThreePointsCopy
                                             (GEOM::GEOM_Object_ptr theObject,
					      GEOM::GEOM_Object_ptr theCentPoint,
					      GEOM::GEOM_Object_ptr thePoint1,
					      GEOM::GEOM_Object_ptr thePoint2)
{
  GEOM::GEOM_Object_var aGEOMObject;

  //Set a not done flag
  GetOperations()->SetNotDone();

  if (theCentPoint == NULL || thePoint1 == NULL || thePoint2 == NULL || theObject == NULL)
    return aGEOMObject._retn();

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) aBasicObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (aBasicObject.IsNull()) return aGEOMObject._retn();

  //Get the central point of rotation
  CORBA::String_var aCPEntry = theCentPoint->GetEntry();
  Handle(GEOM_Object) aCentPoint =
    GetOperations()->GetEngine()->GetObject(theCentPoint->GetStudyID(), aCPEntry);
  if (aCentPoint.IsNull()) return aGEOMObject._retn();

  //Get the first point
  CORBA::String_var aP1Entry = thePoint1->GetEntry();
  Handle(GEOM_Object) aPoint1 =
    GetOperations()->GetEngine()->GetObject(thePoint1->GetStudyID(), aP1Entry);
  if (aPoint1.IsNull()) return aGEOMObject._retn();

  //Get the second point
  CORBA::String_var aP2Entry = thePoint2->GetEntry();
  Handle(GEOM_Object) aPoint2 =
    GetOperations()->GetEngine()->GetObject(thePoint2->GetStudyID(), aP2Entry);
  if (aPoint2.IsNull()) return aGEOMObject._retn();

  //Perform the rotation
  Handle(GEOM_Object) anObject =
    GetOperations()->RotateThreePointsCopy(aBasicObject, aCentPoint, aPoint1, aPoint2);
  if (!GetOperations()->IsDone() || anObject.IsNull())
    return aGEOMObject._retn();

  return GetObject(anObject);
}

//=============================================================================
/*!
 *  RecomputeObject
 */
//=============================================================================
GEOM::GEOM_Object_ptr GEOM_ITransformOperations_i::RecomputeObject
                                             (GEOM::GEOM_Object_ptr theObject)
{
  //Set a not done flag
  GetOperations()->SetNotDone();
  GEOM::GEOM_Object_var aGEOMObject;

  if (theObject == NULL) return aGEOMObject._retn();

  //check if the object is a subshape
  //if (!theObject->IsMainShape()) {
  //  GetOperations()->SetErrorCode(SUBSHAPE_ERROR);
  //  return aGEOMObject._retn();
  //}

  aGEOMObject = GEOM::GEOM_Object::_duplicate(theObject);

  //Get the object itself
  CORBA::String_var anEntry = theObject->GetEntry();
  Handle(GEOM_Object) anObject =
    GetOperations()->GetEngine()->GetObject(theObject->GetStudyID(), anEntry);
  if (anObject.IsNull()) return aGEOMObject._retn();

  //Perform the recomputation
  Handle(GEOM_Function) aLastFunction = anObject->GetLastFunction();
  if (aLastFunction.IsNull()) return aGEOMObject._retn();
  GetOperations()->GetSolver()->ComputeFunction(aLastFunction);

  return aGEOMObject._retn();
}
