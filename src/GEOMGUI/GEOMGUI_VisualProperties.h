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

// File   : GEOMGUI_VisualProperties.h
// Author : Anton POLETAEV, Open CASCADE S.A.S.
//

#ifndef GEOMGUI_VisualProperties_H
#define GEOMGUI_VisualProperties_H

// OCCT includes
#include <Standard_Macro.hxx>
#include <QSharedPointer>

class GEOMGUI_VisualProperties;
typedef QSharedPointer<GEOMGUI_VisualProperties> VisualPropertiesPtr;

class SalomeApp_Study;

/*!
 * \brief Utility class to presentations as object property of study.
 *
 * This is an abstract class with interface to manipulate with a property object
 * Methods should be implemented in inherited classes.
 */
class Standard_EXPORT GEOMGUI_VisualProperties
{
public:

  /*!
   * \brief Constructor. Inits empty property.
   */
  GEOMGUI_VisualProperties() {}

  /*!
   * \brief Destructor.
   */
  ~GEOMGUI_VisualProperties() {}

  /*!
   * \brief Returns number of dimension records.
   */
  virtual int GetNumber() const = 0;

  /*!
   * \brief Removes record by its index.
   * \param theIndex [in] the index of dimension record.
   */
  //virtual void RemoveRecord( const int theIndex ) = 0;
  /*!
   * \brief Clears property data.
   */
  //virtual void Clear() = 0;
public:

  /*!
   * \brief Returns visibility state of dimension record by its index.
   *
   * \param theIndex [in] the index of the dimension record.
   */
  virtual bool IsVisible( const int theIndex ) const = 0;

  /*!
   * \brief Changes visibility state of the dimension record.
   *
   * \param theIndex [in] the index of the dimension record.
   * \param theIsVisible [in] the new visibility state.
   */
  virtual void SetVisible( const int theIndex, const bool theIsVisible ) = 0;

  /*!
   * \brief Returns name of dimension record by its index.
   *
   * \param theIndex [in] the index of the dimension record.
   */
  virtual QString GetName( const int theIndex ) const = 0;

  /*!
   * \brief Changes name of dimension record.
   *
   * \param theIndex [in] the index of the dimension record.
   * \param theName [in] the new name.
   */
  virtual void SetName( const int theIndex, const QString& theName ) = 0;

public:

  /*!
   * \brief Loads properties data from attribute "AttributeTableOfReal".
   * \param theStudy [in] the study.
   * \param theEntry [in] the entry of GEOM object to operate with.
   */
  virtual void LoadFromAttribute( SalomeApp_Study* theStudy, const std::string& theEntry ) = 0;

  /*!
   * \brief Saves properties data to attribute "AttributeTableOfReal".
   * \param theStudy [in] the study.
   * \param theEntry [in] the entry of GEOM object to operate with.
   */
  virtual void SaveToAttribute( SalomeApp_Study* theStudy, const std::string& theEntry ) = 0;
};

#endif
