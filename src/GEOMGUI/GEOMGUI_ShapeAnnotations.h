// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

// File   : GEOMGUI_ShapeAnnotations.h
// Author : Anton POLETAEV, Open CASCADE S.A.S.
//

#ifndef GEOMGUI_SHAPEANNOTATIONS_H
#define GEOMGUI_SHAPEANNOTATIONS_H

// OCCT includes
#include <gp_Ax3.hxx>
#include <QVariant>
#include <QVector>
#include <QSharedPointer>

#include <string>
#include <vector>
#include <list>

class SalomeApp_Study;

/*!
 * \brief Algorithms to translate and manitain list of shape annotation properties.
 *
 * Shape annotation presentations are store in relative coordinate system (LCS).
 * To ensure that dimension is bound to the equal shape irrespectively of its location
 * transformation.
 */
class Standard_EXPORT GEOMGUI_ShapeAnnotations
{
public:

  /*!
   * \ brief Structure representing properties of the shape annotation entry.
   */
  struct ShapeAnnotation
  {
    QString Text;
    bool IsVisible;
    bool IsScreenFixed;
    gp_Pnt Position;
    gp_Pnt Attach;
  };

public:

  /*!
   * \brief Constructor. Inits empty property.
   */
  GEOMGUI_ShapeAnnotations();

  /*!
   * \brief Copy constructor.
   */
  GEOMGUI_ShapeAnnotations( const GEOMGUI_ShapeAnnotations& theOther );

   /*!
   * \brief Constructor. Inits property from attribute.
   */
  GEOMGUI_ShapeAnnotations( SalomeApp_Study* theStudy, const std::string& theEntry );

   /*!
   * \brief Constructor. Inits property from formatted QString.
   */
  GEOMGUI_ShapeAnnotations( const QString& theProperty );

  /*!
   * \brief Destructor.
   */
  ~GEOMGUI_ShapeAnnotations();

  /*!
   * \brief Overload QVariant cast operator.
   */
  operator QVariant() const;

  /*!
   * \brief Overload QString cast operator.
   */
  operator QString() const;

  /*!
   * \brief Overload comparsion.
   */
  bool operator == (const GEOMGUI_ShapeAnnotations &theOther) const;

  /*!
   * \brief Overload comparsion.
   */
  bool operator != (const GEOMGUI_ShapeAnnotations &theOther) const
  {
    return !(operator == (theOther));
  }

public:

  /*!
   * \brief Adds new shape annotation entry using explicit definition.
   * \param theShapeAnnotation [in] the explicit definition of the annotation.
   */
  void Add( const ShapeAnnotation& theShapeAnnotation )
  {
    myAnnotations.append( theShapeAnnotation );
  }

  /*!
   * \brief Adds new entry using data of the interactive presentation given.
   * \param theShapeAnnotation [in] the interactive shape annotation holding properties.
   * \param theLCS [in] the local coordinate system of parent object.
   */
  void Add( const Handle(GEOM_Annotation)& theShapeAnnotation, const gp_Ax3& theLCS );

  /*!
   * \brief Update entry data using the explicit definition.
   * \param theIndex [in] the index of the dimension record.
   * \param theShapeAnnotation [in] the explicit definition of the annotation.
   */
  void SetValues( const int theIndex, const ShapeAnnotation& theShapeAnnotation )
  {
    myAnnotations[theIndex] = theShapeAnnotation;
  }

  /*!
   * \brief Sets annotation's entry data using the properties of interactive presentation.
   * \param theIndex [in] the index of the record.
   * \param theShapeAnnotation [in] the interactive presnetation.
   * \param theLCS [in] the local coordinate system of parent object.
   */
  void FromPresentation( const int theIndex,
                         const Handle(GEOM_Annotation)& theShapeAnnotation,
                         const gp_Ax3& theLCS );

  /*!
   * \brief Update presentation properties using the annotation record definition.
   * \param theIndex [in] the index of the dimension record.
   * \param theShapeAnnotation [in] the explicit definition of the annotation.
   */
  void ToPresentation( const int theIndex,
                       const Handle(GEOM_Annotation)& theShapeAnnotation,
                       const gp_Ax3& theLCS );

  /*!
   * \brief Get explicit definition of an annotation by index.
   * \param theIndex [in] the index of the entry.
   */
  const ShapeAnnotation& Get( const int theIndex ) const { return myAnnotations[theIndex]; }

  /*!
   * \brief Returns mutable reference on the annotation entry.
   * \param theIndex [in] the index of annotation entry.
   */
  ShapeAnnotation& Change( const int theIndex ) { return myAnnotations[theIndex]; }

  /*!
   * \brief Removes entry by its index.
   * \param theIndex [in] the index of annotation entry.
   */
  void Remove( const int theIndex ) { myAnnotations.remove( theIndex ); }

  /*!
   * \brief Clears property data.
   */
  void Clear() { myAnnotations.clear(); }

  /*!
   * \brief Returns number of shape annotation records.
   */
  int Count() const { return myAnnotations.size(); }

public:

  /*!
   * \brief Loads properties data from attribute.
   * \param theStudy [in] the study.
   * \param theEntry [in] the entry of GEOM object to operate with.
   */
  void LoadFromAttribute( SalomeApp_Study* theStudy, const std::string& theEntry );

  /*!
   * \brief Saves properties data to attribute.
   * \param theStudy [in] the study.
   * \param theEntry [in] the entry of GEOM object to operate with.
   */
  void SaveToAttribute( SalomeApp_Study* theStudy, const std::string& theEntry );

private:

  QVector<ShapeAnnotation> myAnnotations;
};

Q_DECLARE_METATYPE(GEOMGUI_ShapeAnnotations);

#endif
