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

// File:        GEOMAlgo_Extractor.hxx
// Author:      Sergey KHROMOV

#ifndef _GEOMAlgo_Extractor_HeaderFile
#define _GEOMAlgo_Extractor_HeaderFile


#include <GEOMAlgo_Algo.hxx>

#include <TopoDS_Shape.hxx>
#include <TopTools_ListOfShape.hxx>


/**
 * \brief This class encapsulates an algorithm of extraction of sub-shapes
 * from the main shape.
 */
class GEOMAlgo_Extractor : public GEOMAlgo_Algo
{
public:

  /**
   * \brief Empty constructor.
   */
  Standard_EXPORT GEOMAlgo_Extractor();

  /**
   * \brief Virtual destructor.
   */
  Standard_EXPORT virtual ~GEOMAlgo_Extractor();

  /**
   * \brief This method sets the main shape.
   *
   * \param theShape the main shape.
   */
  Standard_EXPORT void SetShape(const TopoDS_Shape &theShape);

  /**
   * \brief This method returns the main shape.
   *
   * \return the main shape.
   */
  const TopoDS_Shape &GetShape() const
  { return myShape; }

  /**
   * \brief This method sets the list of sub-shapes to be removed
   *  from the main shape.
   *
   * \param theSubShapes the sub-shapes to be removed.
   */
  Standard_EXPORT void SetShapesToRemove
      (const TopTools_ListOfShape &theSubShapes);

  /**
   * \brief This method returns the list of sub-shapes to be removed
   *  from the main shape.
   *
   * \return the list of sub-shapes to be removed.
   */
  const TopTools_ListOfShape &GetShapesToRemove() const
  { return mySubShapes; }

  /**
   * This method performs computation of the extracted shape.
   */
  Standard_EXPORT virtual void Perform();

  /**
   * This method returns the result of the algorithm.
   *
   * \return the result of the operation.
   */
  Standard_EXPORT const TopoDS_Shape &GetResult() const;

  /**
   * \brief This method returns the sub-shapes removed from the main shape.
   *
   * \return the list of removed sub-shapes.
   */
  const TopTools_ListOfShape &GetRemoved() const
  { return myRemoved; }

  /**
   * \brief This method returns the sub-shapes modified in the main shape.
   *
   * \return the list of modified sub-shapes.
   */
  const TopTools_ListOfShape &GetModified() const
  { return myModified; }

  /**
   * \brief This method returns the newly created sub-shapes in the result
   * shape.
   *
   * \return the list of new sub-shapes in result.
   */
  const TopTools_ListOfShape &GetNew() const
  { return myNew; }

private:

  /**
   * This method reinitializes the shape.
   */
  void clear();

  /**
   * This method checks the input data.
   */
  void checkData();

protected:

  TopoDS_Shape         myShape;
  TopoDS_Shape         myResult;
  TopTools_ListOfShape mySubShapes;
  TopTools_ListOfShape myRemoved;
  TopTools_ListOfShape myModified;
  TopTools_ListOfShape myNew;

};

#endif
