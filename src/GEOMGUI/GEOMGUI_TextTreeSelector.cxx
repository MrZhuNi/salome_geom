// Copyright (C) 2015-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

// File   : GEOMGUI_TextTreeSelector.cxx
// Author :
//
#include "GEOMGUI_TextTreeSelector.h"
#include "GEOMGUI_TextTreeWdg.h"
#include "GEOMGUI_AnnotationMgr.h"

#include "LightApp_DataOwner.h"
#include "LightApp_DataObject.h"
#include "LightApp_Application.h"
#include <SUIT_Session.h>
#include <SUIT_DataObjectIterator.h>

/*!
  \class GEOMGUI_TextTreeSelector
  \brief Text tree selection handler class.
*/

/*!
  \brief Constructor.
  \param widget text tree widget
  \param theManager selection manager
*/
GEOMGUI_TextTreeSelector::GEOMGUI_TextTreeSelector( GEOMGUI_TextTreeWdg* theWidget,
                                                    GEOMGUI_AnnotationMgr* theAnnotationMgr,
                                                    SUIT_SelectionMgr* theManager )
: SUIT_Selector( theManager, theWidget ),
  myTextTree( theWidget ), myAnnotationMgr( theAnnotationMgr )
{
  if ( myTextTree ) {
    connect( myTextTree, SIGNAL( itemSelectionChanged() ), this, SLOT( onSelectionChanged() ) );
  }
}

/*!
  \brief Destructor.
*/
GEOMGUI_TextTreeSelector::~GEOMGUI_TextTreeSelector()
{
}

/*!
  \brief Get object browser.
  \return a pointer to the object browser
*/
GEOMGUI_TextTreeWdg* GEOMGUI_TextTreeSelector::textTree() const
{
  return myTextTree;
}

/*!
  \brief Get selector type.
  \return selector type
*/
QString GEOMGUI_TextTreeSelector::type() const
{ 
  return "TextTree";
}

/*!
  \brief Called when the Object browser selection is changed.
*/
void GEOMGUI_TextTreeSelector::onSelectionChanged()
{
  //QTime t1 = QTime::currentTime();
  mySelectedList.clear();
  selectionChanged();
  //QTime t2 = QTime::currentTime();
  //qDebug( QString( "selection time = %1 msecs" ).arg( t1.msecsTo( t2 ) ).toLatin1().constData() );
}

/*!
  \brief Get list of currently selected objects.
  \param theList list to be filled with the selected objects owners
*/
void GEOMGUI_TextTreeSelector::getSelection( SUIT_DataOwnerPtrList& theList ) const
{
  if ( mySelectedList.count() == 0 ) {

    GEOMGUI_TextTreeSelector* aThisSelector = (GEOMGUI_TextTreeSelector*)this;
    QMap<QString, QList<int> > aSelectedAnnotations;
    myTextTree->getSelected(aSelectedAnnotations);

    QMap<QString, QList<int> >::const_iterator anIt = aSelectedAnnotations.begin(),
                                               aLast = aSelectedAnnotations.end();
    for ( ; anIt != aLast; anIt++ ) {
      QString anEntry = anIt.key();
      QList<int> anIndices = anIt.value();
      QList<int>::const_iterator anIdIt = anIndices.begin(), anIdLast = anIndices.end();
      for ( ; anIdIt != anIdLast; anIdIt++ ) {
        int anIndex = *anIdIt;
        Handle(SALOME_InteractiveObject) anIO = myAnnotationMgr->FindInteractiveObject( anEntry, anIndex );
        if ( anIO.IsNull() )
          continue;

        LightApp_DataOwner* owner = new LightApp_DataOwner( anIO );
        aThisSelector->mySelectedList.append( SUIT_DataOwnerPtr( owner ) );
      }
    }
  }
  theList = mySelectedList;

  /*if ( aSelectedAnnotations.count() == 0 ) {
    SUIT_Session* session = SUIT_Session::session();
    SUIT_Application* sapp = session ? session->activeApplication() : 0;
    LightApp_Application* app = dynamic_cast<LightApp_Application*>( sapp );
    if( !app || !myBrowser )
      return;

    DataObjectList objlist;
    myBrowser->getSelected( objlist );
    GEOMGUI_TextTreeSelector* that = (GEOMGUI_TextTreeSelector*)this;
    QListIterator<SUIT_DataObject*> it( objlist );
    while ( it.hasNext() ) {
      LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( it.next() );
      if ( obj && app->checkDataObject( obj) ) {
#ifndef DISABLE_SALOMEOBJECT
        Handle(SALOME_InteractiveObject) aSObj = new SALOME_InteractiveObject
          ( obj->entry().toLatin1().constData(),
            obj->componentDataType().toLatin1().constData(),
            obj->name().toLatin1().constData() );
        LightApp_DataOwner* owner = new LightApp_DataOwner( aSObj  );
#else
        LightApp_DataOwner* owner = new LightApp_DataOwner( obj->entry() );
#endif
        that->mySelectedList.append( SUIT_DataOwnerPtr( owner ) );
      }
    }
  }
  theList = mySelectedList;*/
}

/*!
  \brief Set selection.
  \param theList list of the object owners to be set selected
*/
void GEOMGUI_TextTreeSelector::setSelection( const SUIT_DataOwnerPtrList& theList )
{
  if ( !myTextTree )
    return;

  //if( myEntries.count() == 0 || myModifiedTime < myBrowser->getModifiedTime() )
  //  fillEntries( myEntries );

  QMap<QString, QList<int> > aSelectedAnnotations;
  DataObjectList objList;
  for ( SUIT_DataOwnerPtrList::const_iterator it = theList.begin(); 
        it != theList.end(); ++it ) {
    const LightApp_DataOwner* anOwner = dynamic_cast<const LightApp_DataOwner*>( (*it).operator->() );
    if ( !anOwner )
      continue;
    Handle(SALOME_InteractiveObject) anIO = anOwner->IO();
    if ( anIO.IsNull() )
      continue;

    QString anIOEntry = anIO->getEntry();
    QStringList anAnnotationInfo = anIOEntry.split(GEOMGUI_AnnotationMgr::GetEntrySeparator());
    if (anAnnotationInfo.size() != 2 )
      continue;

    QString anEntry = anAnnotationInfo[0];
    int anAnnotationId = anAnnotationInfo[1].toInt();//myAnnotationMgr->FindAnnotationIndex( anIO );
    if ( anAnnotationId <  0)
      continue;

    QList<int> anIndices;
    if ( aSelectedAnnotations.contains( anEntry ) )
      anIndices = aSelectedAnnotations[anEntry];

    if ( !anIndices.contains( anAnnotationId ) )
      anIndices.append( anAnnotationId );

    aSelectedAnnotations[anEntry] = anIndices;
  }
  myTextTree->setSelected(aSelectedAnnotations);
  mySelectedList.clear();
}

/*!
  \brief Fill map of the data objects currently shown in the Object Browser.
  \param entries map to be filled
*/
/*void GEOMGUI_TextTreeSelector::fillEntries( QMap<QString, LightApp_DataObject*>& entries )
{
  entries.clear();

  if ( !myBrowser )
    return;

  for ( SUIT_DataObjectIterator it( myBrowser->root(),
                                    SUIT_DataObjectIterator::DepthLeft ); it.current(); ++it ) {
    LightApp_DataObject* obj = dynamic_cast<LightApp_DataObject*>( it.current() );
    if ( obj )
      entries.insert( obj->entry(), obj );
  }

  setModified();
}*/

