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

// File:        CurveCreator_Curve.hxx
// Created:     Thu Jun  20 9:54:14 2013
// Author:      Sergey KHROMOV
//

#ifndef _CurveCreator_Curve_HeaderFile
#define _CurveCreator_Curve_HeaderFile


#include <CurveCreator.hxx>
#include <CurveCreator_Macro.hxx>
#include <CurveCreator_Operation.hxx>

#include <QString>

class CurveCreator_Section;
class CurveCreator_Listener;

/**
 *  The CurveCreator_Curve object is represented as one or more sets of
 *  connected points; thus CurveCreator_Curve object can contain several
 *  not connected curves (polylines or b-splines), each such curve has two
 *  only ends � start and end points � in other words non-manifold curves
 *  are not supported.
 */
class CURVECREATOR_EXPORT CurveCreator_Curve
{

  //! List of curves
  typedef std::deque<CurveCreator_Section *> Sections;

public:
  //! Constructor of the curve.
  /** The dimension is explicitly specified in the constructor
   *  and cannot be changed later.
   */
  CurveCreator_Curve(const CurveCreator::Dimension theDimension);

  //! Destructor.
  ~CurveCreator_Curve();

  //! Returns true if this curve is locked by a curve editor.
  bool isLocked() const;

  //! Get the dimension.
  CurveCreator::Dimension getDimension() const;

  //! Get number of sections.
  int getNbSections() const;

  /** Get number of points in specified section or (the total number of points
   *  in Curve if theISection is equal to -1).
   */
  int getNbPoints(const int theISection = -1) const;

  //! Get coordinates of specified point
  CurveCreator::Coordinates getCoordinates
                  (const int theISection, const int theIPnt) const;

  //! Get points of a section.
  const CurveCreator::Coordinates &getPoints(const int theISection) const;

  //! Get type of the specified section
  CurveCreator::Type getType(const int theISection) const;

  //! Get �closed� flag of the specified section
  bool isClosed(const int theISection) const;

  //! Returns specifyed section name
  std::string   getSectionName(const int theISection) const;

  /**
   * Return unic section name
   */
  std::string getUnicSectionName();

  /**
   * Set curve creator listener object
   */
  void setListener( CurveCreator_Listener*   myWatcher );

  /**
   * Remove curve creator listener object
   */
  void removeListener();

protected:

  /** Set type of the specified section (or all sections
   *  if \a theISection is -1).
   */
  void setType(const CurveCreator::Type theType, const int theISection = -1);

  /** Add points to the specified section (or last section
   *  if \a theISection is -1).
   */
  void addPoints
    (const CurveCreator::Coordinates &thePoints, const int theISection = -1);

  //! Add a new section.
  void addSection (const std::string &theName, const CurveCreator::Type theType,
                   const bool theIsClosed,
                   const CurveCreator::Coordinates &thePoints);

  //! Removes the section. If theISection equals -1, removes the last section.
  void removeSection(const int theISection = -1);

  /** Insert points in the given position (add to the end of list
   *  if \a theIPnt parameter is -1) of the specified section
   *  (or last section if \a theISection parameter is -1).
   */
  void insertPoints(const CurveCreator::Coordinates &thePoints,
                    const int theISection = -1,
                    const int theIPnt = -1);

  /** Remove \a nbPoints points from given \a theISection,
   *  starting from given \a theIPnt (of all points up to the end of
   *  section if \a theNbPoints is -1).
   */
  void removePoints(const int theISection,
                    const int theIPnt,
                    const int theNbPoints = -1);

  /** Move specified  point within section to new position
   */
  void movePoint(const int theISection,
                 const int theIPointFrom,
                 const int theNewIndex);

  //! Remove all sections.
  void clear();

  //! Set coordinates of specified point
  void setCoordinates(const CurveCreator::Coordinates &theCoords,
                      const int theISection,
                      const int theIPnt);

  /** Set �closed� flag of the specified section (all sections if
   *  \a theISection is -1).
   */
  void setClosed(const bool theIsClosed, const int theISection = -1);

  /** Set name of the specified section.
   */
  void setName( const std::string& theName, const int theISection );

  /** Move specified \a theISection to the specified position
   *  in the sections list.
   */
  void moveSection(const int theISection, const int theNewIndex);

  //! Join two sections to one section
  void join(const int theISectionTo, const int theISectionFrom);

  //! Join all sections to the single curve
  void join();

  /**
   * This method converts the point index to the index in
   * an array of coordinates.
   */
  int toICoord(const int theIPnt) const;

protected:

  bool                    myIsLocked;
  Sections                mySections;   //!< curve data
  CurveCreator::Dimension myDimension;  //!< curve dimension
  CurveCreator_Listener*  myListener;   //!< listener

  friend class CurveCreator_CurveEditor;
  friend class CurveCreator_Operation;

};

#endif
