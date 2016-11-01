// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef GEOMGUI_ANNOTATIONMGR_H
#define GEOMGUI_ANNOTATIONMGR_H

#include "GEOM_GEOMGUI.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(GEOM_Gen)

#include <QObject>
#include <GEOMGUI_AnnotationAttrs.h>

#include <SALOME_Prs.h>

class SalomeApp_Application;
class GEOM_Displayer;

/*!
 * \brief A help class to process visualizatin of annotation presentations.
 * It contains a map of presentations shown in each viewer.
 * and correct 2D position persistent properties of annotations of active viewer.
 * When the viewer is closed, information about the viewer is removed from the manager 
 */
class GEOMGUI_EXPORT GEOMGUI_AnnotationMgr : public QObject
{
  Q_OBJECT;

public:
  GEOMGUI_AnnotationMgr( SalomeApp_Application* theApplication );
  ~GEOMGUI_AnnotationMgr() {}

  SALOME_Prs* CreatePresentation( const GEOMGUI_AnnotationAttrs::Properties& theProperty,
                                  GEOM::GEOM_Object_ptr theObject );

  bool IsDisplayed( const QString& theEntry, const int theIndex, SALOME_View* theView = 0 ) const;
  void Display( const QString& theEntry, const int theIndex, SALOME_View* theView = 0 );
  void Erase( const QString& theEntry, const int theIndex, SALOME_View* theView = 0 );

  void DisplayVisibleAnnotations( const QString& theEntry, SALOME_View* theView = 0 );
  void EraseVisibleAnnotations( const QString& theEntry, SALOME_View* theView = 0 );

  void RemoveView( SALOME_View* theView);

  QString getDisplayedIndicesInfo( const QString& theEntry, SALOME_View* theView ) const;

  void setDisplayedIndicesInfo( const QString& theEntry, SALOME_View* theView, const QString theIndicesInfo );

protected:
  SalomeApp_Application* getApplication() const { return myApplication; }

  GEOM_Displayer* getDisplayer() const;

  SALOME_View* viewOrActiveView(SALOME_View* theView) const;

  void getObject( const QString& theEntry, const int theIndex,
                  GEOM::GEOM_Object_ptr& anObject,
                  GEOMGUI_AnnotationAttrs::Properties& aProperty );

  void storeVisibleState( const QString& theEntry, SALOME_View* theView );

private:
  SalomeApp_Application* myApplication;

  typedef QMap<int, SALOME_Prs*> AnnotationToPrs;
  typedef QMap<QString, AnnotationToPrs> EntryToAnnotations;
  QMap<SALOME_View*, EntryToAnnotations> myVisualized;
};
#endif
