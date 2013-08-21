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

// File:        CurveCreator_Curve.cxx
// Created:     Thu Jun  20 9:54:07 2013
// Author:      Sergey KHROMOV
//


#include <CurveCreator_Curve.hxx>
#include <CurveCreator_Section.hxx>

#include <stdio.h>

//=======================================================================
// function: Constructor
// purpose:
//=======================================================================
CurveCreator_Curve::CurveCreator_Curve
                (const CurveCreator::Dimension theDimension)
: myIsLocked  (false),
  myDimension (theDimension)
{
}

//=======================================================================
// function: Destructor
// purpose:
//=======================================================================
CurveCreator_Curve::~CurveCreator_Curve()
{
  // Delete all allocated data.
  clear();
}

//=======================================================================
// function: isLocked
// purpose:
//=======================================================================
bool CurveCreator_Curve::isLocked() const
{
  return myIsLocked;
}

//=======================================================================
// function: getDimension
// purpose:
//=======================================================================
CurveCreator::Dimension CurveCreator_Curve::getDimension() const
{
  return myDimension;
}

//=======================================================================
// function: getNbPoints
// purpose:
//=======================================================================
int CurveCreator_Curve::getNbPoints(const int theISection) const
{
  int aNbCoords = 0;

  if (theISection == -1) {
    int i = 0;
    const int aNbSections = getNbSections();

    for (; i < aNbSections; i++) {
      aNbCoords += mySections[i]->myPoints.size();
    }
  } else {
    aNbCoords = mySections.at(theISection)->myPoints.size();
  }

  return aNbCoords/myDimension;
}

//=======================================================================
// function: getNbSections
// purpose:
//=======================================================================
int CurveCreator_Curve::getNbSections() const
{
  return mySections.size();
}

//=======================================================================
// function: getCoordinates
// purpose:
//=======================================================================
CurveCreator::Coordinates CurveCreator_Curve::getCoordinates
                            (const int theISection, const int theIPnt) const
{
  CurveCreator_Section *aSection = mySections.at(theISection);
  CurveCreator::Coordinates::const_iterator
    anIter = aSection->myPoints.begin() + toICoord(theIPnt);
  CurveCreator::Coordinates aResult(anIter, anIter + myDimension);

  return aResult;
}

//=======================================================================
// function: getType
// purpose:
//=======================================================================
CurveCreator::Type CurveCreator_Curve::getType(const int theISection) const
{
  return mySections.at(theISection)->myType;
}

//=======================================================================
// function: getPoints
// purpose:
//=======================================================================
const CurveCreator::Coordinates &CurveCreator_Curve::getPoints
                  (const int theISection) const
{
  return mySections.at(theISection)->myPoints;
}

//=======================================================================
// function: isClosed
// purpose:
//=======================================================================
bool CurveCreator_Curve::isClosed(const int theISection) const
{
  return mySections.at(theISection)->myIsClosed;
}

std::string CurveCreator_Curve::getSectionName(const int theISection) const
{
    return mySections.at(theISection)->myName;
}

//=======================================================================
// function: setType
// purpose:
//=======================================================================
void CurveCreator_Curve::setType
    (const CurveCreator::Type theType, const int theISection)
{
  if (theISection == -1) {
    int i = 0;
    const int aNbSections = getNbSections();

    for (; i < aNbSections; i++) {
      mySections[i]->myType = theType;
    }
  } else {
    mySections.at(theISection)->myType = theType;
  }
}

//=======================================================================
// function: addPoints
// purpose:
//=======================================================================
void CurveCreator_Curve::addPoints
   (const CurveCreator::Coordinates &thePoints, const int theISection)
{
  CurveCreator_Section *aSection =
    (theISection == -1 ? mySections.back() : mySections.at(theISection));

  aSection->myPoints.insert(aSection->myPoints.end(),
                            thePoints.begin(), thePoints.end());
}

//=======================================================================
// function: addSection
// purpose:
//=======================================================================
void CurveCreator_Curve::addSection
                  (const std::string& theName,
                   const CurveCreator::Type theType,
                   const bool theIsClosed,
                   const CurveCreator::Coordinates &thePoints)
{
  CurveCreator_Section *aSection = new CurveCreator_Section;

  std::string aName = theName;
  if( aName.empty() ){
      aName = getUnicSectionName();
  }
  aSection->myName     = aName;
  aSection->myType     = theType;
  aSection->myIsClosed = theIsClosed;
  aSection->myPoints   = thePoints;
  mySections.push_back(aSection);
}

//=======================================================================
// function: removeSection
// purpose:
//=======================================================================
void CurveCreator_Curve::removeSection(const int theISection)
{
  if (theISection == -1) {
    delete mySections.back();
    mySections.pop_back();
  } else {
    Sections::iterator anIterRm = mySections.begin() + theISection;

    delete *anIterRm;
    mySections.erase(anIterRm);
  }
}

//=======================================================================
// function: insertPoints
// purpose:
//=======================================================================
void CurveCreator_Curve::insertPoints
                   (const CurveCreator::Coordinates &thePoints,
                    const int theISection,
                    const int theIPnt)
{
  if (theIPnt == -1) {
    // Add points to the end of section points.
    addPoints(thePoints, theISection);
  } else {
    CurveCreator_Section *aSection =
      (theISection == -1 ? mySections.back() : mySections.at(theISection));

    aSection->myPoints.insert(aSection->myPoints.begin() + toICoord(theIPnt),
                             thePoints.begin(), thePoints.end());
  }
}

void CurveCreator_Curve::movePoint(const int theISection, const int theIPointFrom, const int theNewIndex)
{
    CurveCreator::Coordinates aCoords = getCoordinates(theISection, theIPointFrom );
    insertPoints(aCoords, theISection, theNewIndex+1);
    int aRemPntIndx = theIPointFrom;
    if( theNewIndex < theIPointFrom )
        aRemPntIndx++;
    removePoints(theISection, aRemPntIndx, 1 );
}

//=======================================================================
// function: removePoints
// purpose:
//=======================================================================
void CurveCreator_Curve::removePoints(const int theISection,
                                      const int theIPnt,
                                      const int theNbPoints)
{
  CurveCreator_Section *aSection = mySections.at(theISection);
  CurveCreator::Coordinates::iterator anIterBegin =
    aSection->myPoints.begin() + toICoord(theIPnt);
  CurveCreator::Coordinates::iterator anIterEnd =
    (theNbPoints == -1 ?
    aSection->myPoints.end() : anIterBegin + toICoord(theNbPoints));

  aSection->myPoints.erase(anIterBegin, anIterEnd);
}

//=======================================================================
// function: clear
// purpose:
//=======================================================================
void CurveCreator_Curve::clear()
{
  // Delete all allocated data.
  int i = 0;
  const int aNbSections = getNbSections();

  for (; i < aNbSections; i++) {
    delete mySections[i];
  }

  mySections.clear();
}

//=======================================================================
// function: setCoordinates
// purpose:
//=======================================================================
void CurveCreator_Curve::setCoordinates
                     (const CurveCreator::Coordinates &theCoords,
                      const int theISection,
                      const int theIPnt)
{
  if (theCoords.size() == myDimension) {
    CurveCreator_Section *aSection = mySections.at(theISection);
    int i;

    for (i = 0; i < myDimension; i++) {
      aSection->myPoints.at(toICoord(theIPnt) + i) = theCoords[i];
    }
  }
}

//=======================================================================
// function: setClosed
// purpose:
//=======================================================================
void CurveCreator_Curve::setClosed(const bool theIsClosed,
                                   const int theISection)
{
  if (theISection == -1) {
    int aSize = mySections.size();
    int i;

    for (i = 0; i < aSize; i++) {
      mySections[i]->myIsClosed = theIsClosed;
    }
  } else {
    mySections.at(theISection)->myIsClosed = theIsClosed;
  }
}

/** Set name of the specified section.
 */
void CurveCreator_Curve::setName( const std::string& theName, const int theISection )
{
    if( ( theISection >= 0 ) && ( theISection < mySections.size() )){
        mySections.at(theISection)->myName = theName;
    }
}

//=======================================================================
// function: moveSection
// purpose:
//=======================================================================
void CurveCreator_Curve::moveSection(const int theISection,
                                     const int theNewIndex)
{
  if (theISection != theNewIndex) {
    CurveCreator_Section *aSection = mySections.at(theISection);

    // Remove section
    Sections::iterator anIter = mySections.begin() + theISection;

    mySections.erase(anIter);

    // Insert section.
    anIter = mySections.begin() + theNewIndex;
    mySections.insert(anIter, aSection);
  }
}

//=======================================================================
// function: join
// purpose:
//=======================================================================
void CurveCreator_Curve::join(const int theISectionTo,
                              const int theISectionFrom)
{
  if (theISectionTo != theISectionFrom) {
    CurveCreator_Section *aSection1 = mySections.at(theISectionTo);
    CurveCreator_Section *aSection2 = mySections.at(theISectionFrom);

    aSection1->myPoints.insert(aSection1->myPoints.end(),
      aSection2->myPoints.begin(), aSection2->myPoints.end());

    removeSection(theISectionFrom);
  }
}

//=======================================================================
// function: join
// purpose:
//=======================================================================
void CurveCreator_Curve::join()
{
  const int aSize = mySections.size();

  if (aSize > 1) {
    CurveCreator_Section *aSection1 = mySections[0];
    int i;

    for (i = 1; i < aSize; i++) {
      CurveCreator_Section *aSection2 = mySections[i];

      aSection1->myPoints.insert(aSection1->myPoints.end(),
        aSection2->myPoints.begin(), aSection2->myPoints.end());
      delete aSection2;
    }

    // Just erace section pointers as they were deleted before.
    mySections.erase(mySections.begin() + 1, mySections.end());
  }
}

//=======================================================================
// function: toICoord
// purpose:
//=======================================================================
int CurveCreator_Curve::toICoord(const int theIPnt) const
{
  return theIPnt*myDimension;
}

//=======================================================================
// function: getUnicSectionName
// purpose: return unic section name
//=======================================================================
std::string CurveCreator_Curve::getUnicSectionName()
{
    for( int i = 0 ; i < 1000000 ; i++ ){
        char aBuffer[255];
        sprintf( aBuffer, "Section_%d", i+1 );
        std::string aName(aBuffer);
        int j;
        for( j = 0 ; j < mySections.size() ; j++ ){
            if( mySections[j]->myName == aName )
              break;
        }
        if( j == mySections.size() )
            return aName;
    }
    return "";
}
