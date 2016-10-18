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
// File      : GEOMGUI_TextTreeWdg.cxx
// Author    : Alexander KOVALEV (akl)

#include "GEOMGUI_TextTreeWdg.h"

#include "GEOMGUI_DimensionProperty.h"
//#include "GEOMGUI_ShapeAnnotations.h"
#include "GeometryGUI.h"
#include "GeometryGUI_Operations.h"
#include <GEOM_Constants.h>
#include <GEOMUtils.hxx>

// GUI includes
#include <LightApp_Module.h>
#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <SUIT_Desktop.h>
#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_DataBrowser.h>

// Qt includes
#include <QAction>
#include <QMenu>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHash>

GEOMGUI_TextTreeWdg::GEOMGUI_TextTreeWdg( SalomeApp_Application* app )
  : myDisplayer(NULL)
{
  myStudy = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
  myDisplayer = GEOM_Displayer( myStudy );

  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  myVisibleIcon = QIcon( resMgr->loadPixmap( "SUIT", tr( "ICON_DATAOBJ_VISIBLE" ) ) );
  myInvisibleIcon = QIcon( resMgr->loadPixmap( "SUIT", tr( "ICON_DATAOBJ_INVISIBLE" ) ) );

  setWindowTitle( tr( "TEXT_TREE_VIEW_TITLE" ) );
  setObjectName( "geomTextTreeWdg" );

  setRootIsDecorated( true );
  setSelectionMode( QAbstractItemView::ExtendedSelection );
  setAllColumnsShowFocus( true );
  setUniformRowHeights( true );

  QStringList columnNames;
  columnNames << tr("TEXT_TREE_VIEW_NAME") << "";
  QTreeWidgetItem * headerItem = new QTreeWidgetItem( columnNames );
  headerItem->setIcon( 1, myVisibleIcon );
  setHeaderItem ( headerItem ); 
  header()->moveSection( 1, 0 );
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  header()->setResizeMode( 1, QHeaderView::ResizeToContents );
#else
  header()->setSectionResizeMode( 1, QHeaderView::ResizeToContents );
#endif
  QStringList rootNames;
  rootNames << tr("GEOM_DIMENSIONS") << "";
  myDimensionsItem = new QTreeWidgetItem( this, rootNames );
  myDimensionsItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
  addTopLevelItem( myDimensionsItem );

  rootNames.clear();
  rootNames << tr("GEOM_ANNOTATIONS") << "";
  myAnnotationsItem = new QTreeWidgetItem( this, rootNames );
  myAnnotationsItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
  addTopLevelItem( myAnnotationsItem );

  // get a free dockable window id
  myWindowID = 11;
  while( app->dockWindow( myWindowID ))
    ++myWindowID;
  ++myWindowID;

  createActions();
  setContextMenuPolicy( Qt::CustomContextMenu );
  connect( this, SIGNAL( customContextMenuRequested(const QPoint&) ),
           this, SLOT( showContextMenu(const QPoint&) ) );

  connect( myStudy, SIGNAL( objVisibilityChanged( QString, Qtx::VisibilityState ) ), 
           this, SLOT( updateVisibilityColumn( QString, Qtx::VisibilityState ) ) );
  connect( app->objectBrowser(), SIGNAL( updated() ), this, SLOT( updateTree() ) );
  GeometryGUI* aGeomGUI = dynamic_cast<GeometryGUI*>( app->module( "Geometry" ) );
  connect( aGeomGUI, SIGNAL( DimensionsUpdated( const QString& ) ), this, SLOT( updateBranch( const QString& ) ) );
  connect( this, SIGNAL( itemClicked( QTreeWidgetItem*, int) ), 
           this, SLOT( onItemClicked( QTreeWidgetItem*, int ) ) );

}

GEOMGUI_TextTreeWdg::~GEOMGUI_TextTreeWdg()
{
  //std::cout<<"~GEOMGUI_TextTreeWdg"<<std::endl;
}

//=================================================================================
// function : createActions
// purpose  : Create context popup menu actions.
//=================================================================================
void GEOMGUI_TextTreeWdg::createActions()
{
  QAction* a = new QAction( tr( "MEN_DISPLAY" ), this );
  a->setIcon( myVisibleIcon );
  myActions.insert( GEOMOp::OpShow, a );
  
  QAction* b = new QAction( tr( "MEN_ERASE" ), this );
  b->setIcon( myInvisibleIcon );
  myActions.insert( GEOMOp::OpHide, b );
}

//=================================================================================
// function : updateTree
// purpose  :
//=================================================================================
void GEOMGUI_TextTreeWdg::updateTree()
{
  myStudy = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );
  _PTR(Study) aDSStudy = myStudy->studyDS();
  if ( aDSStudy ) {
    _PTR(SComponent) SC ( aDSStudy->FindComponent( "GEOM" ) );
    if ( SC ) {
      _PTR(ChildIterator) anIter ( aDSStudy->NewChildIterator( SC ) );
      anIter->InitEx( true );
      QList<QString> aGeomObjEntries = getObjects( Geometry ).keys();
      QList<QString> anAnnotationObjEntries = getObjects( AnnotationShape ).keys();
      while( anIter->More() ) {
        _PTR(SObject) valSO ( anIter->Value() );
        _PTR(SObject) refSO;
        if ( !valSO->ReferencedObject( refSO ) ) {
          // update tree of object's dimensions
          QString anEntry = valSO->GetID().c_str();
          updateBranch( anEntry );
          aGeomObjEntries.removeAll( anEntry );
          anAnnotationObjEntries.removeAll( anEntry );
        }
        anIter->Next();
      }
      foreach (QString entry, aGeomObjEntries) {
        removeBranch( Geometry, entry, true );
      }
      foreach (QString entry, anAnnotationObjEntries) {
        removeBranch( AnnotationShape, entry, true );
      }
    }
  }
}
  
//=================================================================================
// function : updateBranch
// purpose  :
//=================================================================================
void GEOMGUI_TextTreeWdg::updateBranch( const QString& theEntry )
{
  /// dimension property branch
  fillBranch( Geometry, theEntry );

  // annotation property branch
  // fillBranch(AnnotationShape, theEntry);
}

void GEOMGUI_TextTreeWdg::fillBranch( const BranchType& theBranchType, const QString& theEntry )
{
  myStudy = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );

  if ( myStudy && !theEntry.isEmpty() ) {
    VisualPropertiesPtr aProp = getVisualProperty( theBranchType );
    int aNumber = aProp->GetNumber();
    const std::string anEntry = theEntry.toStdString();

    aProp->LoadFromAttribute( myStudy, theEntry.toStdString() );

    if (!aProp)
      return;

    _PTR(Study) aStudyDS = myStudy->studyDS();
    if ( aStudyDS ) {
      _PTR(SObject) obj( aStudyDS->FindObjectID( theEntry.toStdString() ) );
      QString aName = obj->GetName().c_str();
  
      int nbProps = aProp->GetNumber();

      QTreeWidgetItem* objectItem = itemFromEntry( theBranchType, theEntry );
      if ( objectItem ) {
        removeBranch( theBranchType, theEntry, nbProps > 0 ? false : true );
      }
      QStringList itemName;
      if ( nbProps > 0 ) {
        itemName << aName << "";
        if ( !objectItem ) {
          QTreeWidgetItem* aPropRootItem = getPropertyRootItem(theBranchType);

          objectItem = new QTreeWidgetItem( aPropRootItem, itemName );
          objectItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
          objectItem->setData( 1, Qt::UserRole, theEntry );
          aPropRootItem->addChild( objectItem );
          getObjects( theBranchType ).insert( theEntry, objectItem );
          if ( aPropRootItem->childCount() == 1 )
            aPropRootItem->setExpanded( true );
        }
        bool isDisplayed = myDisplayer.IsDisplayed( theEntry );
        // read dimension records from property
        for ( int anIt = 0; anIt < aProp->GetNumber(); ++anIt ) {
          QString aName  = aProp->GetName( anIt );
          bool isVisible = aProp->IsVisible( anIt );

          QTreeWidgetItem* anItem = new QTreeWidgetItem;
          anItem->setText( 0, aName );
          //    if ( isDisplayed )
          anItem->setIcon( 1, isVisible ? myVisibleIcon : myInvisibleIcon );
          anItem->setData( 0, Qt::UserRole, anIt );
          anItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
          objectItem->addChild( anItem );
        }
      }
    }
  }
}

//=================================================================================
// function : getVisualProperty
// purpose  :
//=================================================================================
VisualPropertiesPtr GEOMGUI_TextTreeWdg::getVisualProperty( const BranchType& theBranchType )
{
  VisualPropertiesPtr aProp;

  if ( theBranchType == Geometry ) {
    aProp = QSharedPointer<GEOMGUI_DimensionProperty>( new GEOMGUI_DimensionProperty() );
  }
  else {
    //aProp = QSharedPointer<GEOMGUI_ShapeAnnotations>( new GEOMGUI_ShapeAnnotations() );
  }

return aProp;
}

//=================================================================================
// function : removeBranch
// purpose  :
//=================================================================================
void GEOMGUI_TextTreeWdg::removeBranch( const BranchType& theBranchType, const QString& theEntry,
                                        bool force )
{
  QTreeWidgetItem* objectItem = itemFromEntry( theBranchType, theEntry );
  if ( !objectItem )
    return;
  qDeleteAll( objectItem->takeChildren() );
  if ( force ) {
    QTreeWidgetItem* aPropRootItem = getPropertyRootItem( theBranchType );
    aPropRootItem->removeChild( objectItem );
    getObjects( theBranchType ).remove( theEntry );
  }
}

//=================================================================================
// function : onItemClicked()
// purpose  : called when tree item was clicked
//=================================================================================
void GEOMGUI_TextTreeWdg::onItemClicked( QTreeWidgetItem* theItem, int theColumn )
{
  if( theColumn != 1 || theItem->icon( 1 ).isNull() || theItem->isDisabled() )
    return;
  
  std::string anEntry = entryFromItem( theItem->parent() ).toStdString();
  BranchType aBranchType = branchTypeFromItem( theItem );

  int aDimIndex = idFromItem( theItem );

  VisualPropertiesPtr aProp = getVisualProperty( aBranchType );
  aProp->LoadFromAttribute( myStudy, anEntry );
  if ( aProp->IsVisible( aDimIndex ) ) {
    aProp->SetVisible( aDimIndex, false );
    theItem->setIcon( 1, myInvisibleIcon );
  } else {
    aProp->SetVisible( aDimIndex, true );
    theItem->setIcon( 1, myVisibleIcon );
  }
  aProp->SaveToAttribute( myStudy, anEntry );

  redisplay( anEntry.c_str() );
}

//=================================================================================
// function : idFromItem
// purpose  :
//=================================================================================
int GEOMGUI_TextTreeWdg::idFromItem( QTreeWidgetItem* theItem )
{
  if ( !theItem )
    return -1;

  bool isIdOK = false;
  const int anId = theItem->data( 0, Qt::UserRole ).toInt( &isIdOK );

  return isIdOK ? anId : -1;
}

//=================================================================================
// function : entryFromItem
// purpose  :
//=================================================================================
QString GEOMGUI_TextTreeWdg::entryFromItem( QTreeWidgetItem* theShapeItem )
{
  if ( !theShapeItem )
    return "";

  return theShapeItem->data( 1, Qt::UserRole ).toString();
}

//=================================================================================
// function : itemFromEntry
// purpose  :
//=================================================================================
QTreeWidgetItem* GEOMGUI_TextTreeWdg::itemFromEntry( const BranchType& theBranchType, QString theEntry )
{
  if ( theEntry.isEmpty() )
    return 0;

  return getObjects(theBranchType).value( theEntry, 0 );
}

//=================================================================================
// function : updateVisibilityColumn
// purpose  : Update visible state of icons of entry items.
//=================================================================================
void GEOMGUI_TextTreeWdg::updateVisibilityColumn( QString theEntry, Qtx::VisibilityState theState )
{
  //BranchType theBranchType,
  updateVisibilityColumn( Geometry, theEntry, theState );
  updateVisibilityColumn( AnnotationShape, theEntry, theState );
}

//=================================================================================
// function : updateVisibilityColumn
// purpose  : Update icons of dimension items.
//=================================================================================
void GEOMGUI_TextTreeWdg::updateVisibilityColumn( const BranchType& theBranchType, QString theEntry,
                                                  Qtx::VisibilityState theState )
{
  QTreeWidgetItem* anItem = itemFromEntry( theBranchType, theEntry );
  if ( !anItem )
    return;
  anItem->setDisabled( theState != Qtx::ShownState );
  QTreeWidgetItem* aChildItem;

  VisualPropertiesPtr aProp = getVisualProperty( theBranchType );

  for ( int i=0; i < anItem->childCount(); i++ ) {
    aChildItem = anItem->child( i );
    if ( theState == Qtx::ShownState ) {
      aProp->LoadFromAttribute( myStudy, theEntry.toStdString() );
      if ( aProp->GetNumber() == 0 )
        continue;
      aChildItem->setIcon( 1, aProp->IsVisible( idFromItem( aChildItem ) ) ? myVisibleIcon
                                                                           : myInvisibleIcon );
      aChildItem->setDisabled( false );
    } else {
      aChildItem->setIcon( 1, QIcon() );
      aChildItem->setDisabled( true );
    }
  }
}

//=================================================================================
// function : showContextMenu
// purpose  : 
//=================================================================================
void GEOMGUI_TextTreeWdg::showContextMenu( const QPoint& pos )
{
  if ( selectedItems().isEmpty() )
    return;
  QMenu aMenu;
  aMenu.addAction( myActions[GEOMOp::OpShow] );
  aMenu.addAction( myActions[GEOMOp::OpHide] );

  if ( selectedItems().count() == 1 ) {
    QTreeWidgetItem* anItem = selectedItems().first();
    QString anEntry = entryFromItem( anItem->parent() );
    if ( !anEntry.isEmpty() ) {
      BranchType aBranchType = branchTypeFromItem( anItem );
      VisualPropertiesPtr aProp = getVisualProperty( aBranchType );

      aProp->LoadFromAttribute( myStudy, anEntry.toStdString() );
      if ( aProp->GetNumber() == 0 )
        return;
      aMenu.clear();
      if ( aProp->IsVisible( idFromItem( anItem ) ) )
        aMenu.addAction( myActions[GEOMOp::OpHide] );
      else
        aMenu.addAction( myActions[GEOMOp::OpShow] );
    }
  }
  QAction* selPopupItem = aMenu.exec( viewport()->mapToGlobal(pos) );

  if ( selPopupItem == myActions[GEOMOp::OpShow] ||
       selPopupItem == myActions[GEOMOp::OpHide] ) {
    bool isVisible = selPopupItem == myActions[GEOMOp::OpShow];
    foreach ( QTreeWidgetItem* anItem, selectedItems() ) {
      setVisibility( anItem, isVisible );
    }
  }
}

//=================================================================================
// function : setVisibility
// purpose  : set item visible
//=================================================================================
void GEOMGUI_TextTreeWdg::setVisibility( QTreeWidgetItem* theItem, bool theVisibility )
{
  BranchType aBranchType = branchTypeFromItem( theItem );
  if ( theItem == myDimensionsItem ||
       theItem == myAnnotationsItem ) {
    // set visibility for all dimensions
    QTreeWidgetItem* anItem;
    foreach ( QString entry, getObjects( aBranchType ).keys() ) {
      anItem = itemFromEntry( aBranchType, entry );
      if ( !anItem->isDisabled() )
        setShapeDimensionsVisibility( aBranchType, entry, theVisibility );
    }
    return;
  }
  else {
    if ( !theItem->isDisabled() && !theItem->parent()->isSelected() ) {
      QString anEntry = entryFromItem( theItem );
      if ( !anEntry.isEmpty() ) {
        // it is a shape item
        setShapeDimensionsVisibility( aBranchType, anEntry, theVisibility );
      } else {
        // it is a dimension item
        anEntry = entryFromItem( theItem->parent() );
        setDimensionVisibility( aBranchType, anEntry, theItem, theVisibility );
      }
      }
  }
}

//=================================================================================
// function : setShapeDimensionsVisibility
// purpose  : 
//=================================================================================
void GEOMGUI_TextTreeWdg::setShapeDimensionsVisibility( const BranchType& theBranchType,
                                                        QString theEntry, bool theVisibility )
{
  QTreeWidgetItem* anItem = itemFromEntry( theBranchType, theEntry );
  QTreeWidgetItem* aChildItem;
  for ( int i=0; i < anItem->childCount(); i++ ) {
    aChildItem = anItem->child( i );
    setDimensionVisibility( theBranchType, theEntry, aChildItem, theVisibility );
  }
  redisplay( theEntry );
}

//=================================================================================
// function : setDimensionVisibility
// purpose  : 
//=================================================================================
void GEOMGUI_TextTreeWdg::setDimensionVisibility( const BranchType& theBranchType, QString theEntry,
                                                  QTreeWidgetItem* theDimItem, bool theVisibility )
{
  VisualPropertiesPtr aProp = getVisualProperty( theBranchType );

  aProp->LoadFromAttribute( myStudy, theEntry.toStdString() );
  int aDimIndex = idFromItem( theDimItem );
  if ( aProp->GetNumber() == 0  || aProp->IsVisible( aDimIndex ) == theVisibility )
    return;;
  aProp->SetVisible( aDimIndex, theVisibility );
  aProp->SaveToAttribute( myStudy, theEntry.toStdString() );

  theDimItem->setIcon( 1, theVisibility ? myVisibleIcon : myInvisibleIcon );
  redisplay( theEntry );
}

//=================================================================================
// function : redisplay
// purpose  : 
//=================================================================================
void GEOMGUI_TextTreeWdg::redisplay( QString theEntry )
{
  Handle(SALOME_InteractiveObject) io = new SALOME_InteractiveObject( theEntry.toLatin1().constData(), "GEOM", "TEMP_IO" );
  myDisplayer.Redisplay( io );
}

//=================================================================================
// function : getPropertyRootItem
// purpose  :
//=================================================================================
QTreeWidgetItem* GEOMGUI_TextTreeWdg::getPropertyRootItem( const BranchType& theBranchType )
{
  return theBranchType == Geometry ? myDimensionsItem : myAnnotationsItem;
}

//=================================================================================
// function : getObjects
// purpose  :
//=================================================================================
QHash<QString, QTreeWidgetItem*>& GEOMGUI_TextTreeWdg::getObjects( const BranchType& theBranchType )
{
  return theBranchType == Geometry ? myObjects : myAnnotationObjects;
}

//=================================================================================
// function : branchTypeFromItem
// purpose  :
//=================================================================================
GEOMGUI_TextTreeWdg::BranchType GEOMGUI_TextTreeWdg::branchTypeFromItem( QTreeWidgetItem* theItem )
{
  BranchType aBranchType = Geometry;

  bool aBranchTypeFound = false;
  QTreeWidgetItem* anItem = theItem;
  while( !aBranchTypeFound && anItem ) {
    if ( anItem == myDimensionsItem ||
         anItem == myAnnotationsItem) {
      aBranchTypeFound = true;
      aBranchType = anItem == myDimensionsItem ? Geometry : AnnotationShape;
    }
    else {
      anItem = anItem->parent();
    }
  }

  return aBranchType;
}

