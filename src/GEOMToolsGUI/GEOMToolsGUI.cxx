// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

// GEOM GEOMGUI : GUI for Geometry component
// File   : GEOMBase_Tools.cxx
// Author : Damien COQUERET, Open CASCADE S.A.S.

#include "GEOMToolsGUI.h"
#include "GEOMToolsGUI_DeleteDlg.h"

#include <GeometryGUI.h>
#include "GeometryGUI_Operations.h"
#include <GEOMBase.h>
#include <GEOM_Operation.h>
#include <GEOM_Displayer.h>

#include <SUIT_Session.h>
#include <SUIT_OverrideCursor.h>
#include <SUIT_MessageBox.h>
#include <SUIT_Tools.h>
#include <SUIT_FileDlg.h>
#include <SUIT_Desktop.h>
#include <SUIT_ViewModel.h>
#include <SUIT_ViewManager.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <LightApp_SelectionMgr.h>
#include <GEOMImpl_Types.hxx>

#include <SALOME_ListIO.hxx>
#include <SALOME_ListIteratorOfListIO.hxx>
#include <SALOME_Prs.h>

// QT Includes
#include <QApplication>
#include <QMap>
#include <QRegExp>

// OCCT Includes
#include <TCollection_AsciiString.hxx>

typedef QMap<QString, QString> FilterMap;
static QString lastUsedFilter;

//=======================================================================
// function : getFileName
// purpose  : Selection of a file name for Import/Export. Returns also
//            the selected file type code through <filter> argument.
//=======================================================================
static QString getFileName( QWidget*           parent,
                            const QString&     initial,
                            const FilterMap&   filterMap,
                            const QStringList& filters,
                            const QString&     caption,
                            bool               open,
                            QString&           format,
                            bool               showCurrentDirInitially = false )
{
  //QStringList filters;
  QString aBrepFilter;
  for (FilterMap::const_iterator it = filterMap.begin(); it != filterMap.end(); ++it) {
    //filters.push_back( it.key() );
    if (it.key().contains( "BREP", Qt::CaseInsensitive ))
      aBrepFilter = it.key();
  }

  SUIT_FileDlg* fd = new SUIT_FileDlg( parent, open, true, true );
  if ( !caption.isEmpty() )
    fd->setWindowTitle( caption );

  if ( !initial.isEmpty() )
    fd->selectFile( initial );

  if ( showCurrentDirInitially && SUIT_FileDlg::getLastVisitedPath().isEmpty() )
    fd->setDirectory( QDir::currentPath() );

  fd->setFilters( filters );

  if ( !lastUsedFilter.isEmpty() && filterMap.contains( lastUsedFilter ) ) {
    fd->selectFilter( lastUsedFilter );
  }
  else if ( !aBrepFilter.isEmpty() ) {
    fd->selectFilter( aBrepFilter );
  }

  QString filename;
  if ( fd->exec() == QDialog::Accepted ) {
    filename = fd->selectedFile();
    format = filterMap[fd->selectedFilter()];
    lastUsedFilter = fd->selectedFilter();
  }

  delete fd;
  qApp->processEvents();
  return filename;
}

//=======================================================================
// function : getFileNames
// purpose  : Select list of files for Import operation. Returns also
//            the selected file type code through <format> argument.
//=======================================================================
static QStringList getFileNames( QWidget*           parent,
                                 const QString&     initial,
                                 const FilterMap&   filterMap,
                                 const QString&     caption,
                                 QString&           format,
                                 bool               showCurrentDirInitially = false)
{
  QString aBrepFilter;
  QStringList allFilters;
  QStringList filters;
  QRegExp re( "\\((.*)\\)" );
  re.setMinimal( true );
  for ( FilterMap::const_iterator it = filterMap.begin(); it != filterMap.end(); ++it ) {
    if ( it.value().contains( "BREP", Qt::CaseInsensitive ) && aBrepFilter.isEmpty() )
      aBrepFilter = it.key();
    filters.append( it.key() );
    int pos = 0;
    while ( re.indexIn( it.key(), pos ) >= 0 ) {
      QString f = re.cap(1);
      pos = re.pos() + f.length() + 2;
      allFilters.append( f.simplified() );
    }
  }
  filters.append( QObject::tr( "GEOM_ALL_IMPORT_FILES" ).arg( allFilters.join( " " ) ) );

  SUIT_FileDlg fd( parent, true, true, true );
  fd.setFileMode( SUIT_FileDlg::ExistingFiles );
  if ( !caption.isEmpty() )
    fd.setWindowTitle( caption );
  if ( !initial.isEmpty() )
    fd.selectFile( initial );

  if ( showCurrentDirInitially && SUIT_FileDlg::getLastVisitedPath().isEmpty() )
    fd.setDirectory( QDir::currentPath() );

  fd.setFilters( filters );

  if ( !lastUsedFilter.isEmpty() && filterMap.contains( lastUsedFilter ) )
    fd.selectFilter( lastUsedFilter );
  else if ( !aBrepFilter.isEmpty() )
    fd.selectFilter( aBrepFilter );

  QStringList filenames;
  if ( fd.exec() ) {
    filenames = fd.selectedFiles();
    format = filterMap.contains( fd.selectedFilter() ) ? filterMap[ fd.selectedFilter() ] : QString();
    lastUsedFilter = fd.selectedFilter();
  }
  qApp->processEvents();
  return filenames;
}

//=======================================================================
// function : getParentComponent
// purpose  : Get object's parent component entry
//=======================================================================
static QString getParentComponent( _PTR( SObject ) obj )
{
  if ( obj ) {
    _PTR(SComponent) comp = obj->GetFatherComponent();
    if ( comp )
      return QString( comp->GetID().c_str() );
  }
  return QString();
}

//=====================================================================================
// function : inUse
// purpose  : check if the object(s) passed as the the second arguments are used
//            by the other objects in the study
//=====================================================================================
static bool inUse( _PTR(Study) study, const QString& component, const QMap<QString,QString>& objects )
{
  _PTR(SObject) comp = study->FindObjectID( component.toLatin1().data() );
  if ( !comp )
    return false;

  // collect all GEOM objects being deleted
  QMap<QString, GEOM::GEOM_BaseObject_var> gobjects;
  QMap<QString, QString>::ConstIterator oit;
  std::list<_PTR(SObject)> aSelectedSO;
  for ( oit = objects.begin(); oit != objects.end(); ++oit ) {
    _PTR(SObject) so = study->FindObjectID( oit.key().toLatin1().data() );
    if ( !so )
      continue;
    aSelectedSO.push_back(so);
    CORBA::Object_var corbaObj_rem = GeometryGUI::ClientSObjectToObject( so );
    GEOM::GEOM_BaseObject_var geomObj_rem = GEOM::GEOM_BaseObject::_narrow( corbaObj_rem );
    if( CORBA::is_nil( geomObj_rem ) )
      continue;
    gobjects.insert( oit.key(), geomObj_rem );
  }

  // Search References with other Modules
  std::list< _PTR(SObject) >::iterator itSO = aSelectedSO.begin();
  for ( ; itSO != aSelectedSO.end(); ++itSO ) {
    std::vector<_PTR(SObject)> aReferences = study->FindDependances( *itSO  );
    int aRefLength = aReferences.size();
    if (aRefLength) {
      for (int i = 0; i < aRefLength; i++) {
        _PTR(SObject) firstSO( aReferences[i] );
        _PTR(SComponent) aComponent = firstSO->GetFatherComponent();
        QString type = aComponent->ComponentDataType().c_str();
        if ( type == "SMESH" )
          return true;
      }
    }
  }

  // browse through all GEOM data tree
  _PTR(ChildIterator) it ( study->NewChildIterator( comp ) );
  for ( it->InitEx( true ); it->More(); it->Next() ) {
    _PTR(SObject) child( it->Value() );
    CORBA::Object_var corbaObj = GeometryGUI::ClientSObjectToObject( child );
    GEOM::GEOM_Object_var geomObj = GEOM::GEOM_Object::_narrow( corbaObj );
    if( CORBA::is_nil( geomObj ) )
      continue;

    GEOM::ListOfGBO_var list = geomObj->GetDependency();
    if( list->length() == 0 )
      continue;

    for( int i = 0; i < list->length(); i++ ) {
      bool depends = false;
      bool deleted = false;
      QMap<QString, GEOM::GEOM_BaseObject_var>::Iterator git;
      for ( git = gobjects.begin(); git != gobjects.end() && ( !depends || !deleted ); ++git ) {
        depends = depends || list[i]->_is_equivalent( *git );
        deleted = deleted || git.key() == child->GetID().c_str() ;//geomObj->_is_equivalent( *git );
      }
      if ( depends && !deleted )
        return true;
    }
  }
  return false;
}

//=======================================================================
// function : getGeomChildrenAndFolders
// purpose  : Get direct (1-level) GEOM objects under each folder, sub-folder, etc. and these folders itself
//=======================================================================
static void getGeomChildrenAndFolders( _PTR(SObject) theSO, 
                                       QMap<QString,QString>& geomObjList, 
                                       QMap<QString,QString>& folderList ) {
  if ( !theSO ) return;
  _PTR(Study) aStudy = theSO->GetStudy();
  if ( !aStudy ) return;
  _PTR(UseCaseBuilder) aUseCaseBuilder = aStudy->GetUseCaseBuilder();

  bool isFolder = false;
  _PTR(GenericAttribute) anAttr;
  if ( theSO->FindAttribute(anAttr, "AttributeLocalID") ) {
    _PTR(AttributeLocalID) aLocalID( anAttr );
    isFolder = aLocalID->Value() == 999;
  }
  QString anEntry = theSO->GetID().c_str();
  QString aName = theSO->GetName().c_str();
  if ( isFolder ) {
    folderList.insert( anEntry, aName );
    _PTR(UseCaseIterator) ucit ( aUseCaseBuilder->GetUseCaseIterator( theSO ) );
    for ( ucit->Init( false ); ucit->More(); ucit->Next() ) {
      getGeomChildrenAndFolders( ucit->Value(), geomObjList, folderList );
    }
  } else {
    geomObjList.insert( anEntry, aName );
  }
}

//=======================================================================
// function : GEOMToolsGUI()
// purpose  : Constructor
//=======================================================================
GEOMToolsGUI::GEOMToolsGUI( GeometryGUI* parent )
: GEOMGUI( parent )
{
}

//=======================================================================
// function : ~GEOMToolsGUI()
// purpose  : Destructor
//=======================================================================
GEOMToolsGUI::~GEOMToolsGUI()
{
}

//=======================================================================
// function : OnGUIEvent()
// purpose  :
//=======================================================================
bool GEOMToolsGUI::OnGUIEvent(int theCommandID, SUIT_Desktop* parent)
{
  getGeometryGUI()->EmitSignalDeactivateDialog();

  switch ( theCommandID ) {
  case GEOMOp::OpDelete:         // EDIT - DELETE
    OnEditDelete();
    break;
  case GEOMOp::OpImport:         // FILE - IMPORT
    Import();
    break;
  case GEOMOp::OpExport:         // FILE - EXPORT
    Export();
    break;
  case GEOMOp::OpCheckGeom:      // TOOLS - CHECK GEOMETRY
    OnCheckGeometry();
    break;
  case GEOMOp::OpSelectVertex:   // POPUP - SELECT ONLY - VERTEX
    OnSelectOnly( GEOM_POINT );
    break;
  case GEOMOp::OpSelectEdge:     // POPUP - SELECT ONLY - EDGE
    OnSelectOnly( GEOM_EDGE );
    break;
  case GEOMOp::OpSelectWire:     // POPUP - SELECT ONLY - WIRE
    OnSelectOnly( GEOM_WIRE );
    break;
  case GEOMOp::OpSelectFace:     // POPUP - SELECT ONLY - FACE
    OnSelectOnly( GEOM_FACE );
    break;
  case GEOMOp::OpSelectShell:    // POPUP - SELECT ONLY - SHELL
    OnSelectOnly( GEOM_SHELL );
    break;
  case GEOMOp::OpSelectSolid:    // POPUP - SELECT ONLY - SOLID
    OnSelectOnly( GEOM_SOLID );
    break;
  case GEOMOp::OpSelectCompound: // POPUP - SELECT ONLY - COMPOUND
    OnSelectOnly( GEOM_COMPOUND );
    break;
  case GEOMOp::OpSelectAll:      // POPUP - SELECT ONLY - SELECT ALL
    OnSelectOnly( GEOM_ALLOBJECTS );
    break;
  case GEOMOp::OpDeflection:     // POPUP - DEFLECTION ANGLE
    OnDeflection();
    break;
  case GEOMOp::OpColor:          // POPUP - COLOR
    OnColor();
    break;
  case GEOMOp::OpSetTexture:     // POPUP - TEXTURE
    OnTexture();
    break;
  case GEOMOp::OpTransparency:   // POPUP - TRANSPARENCY
    OnTransparency();
    break;
  case GEOMOp::OpIncrTransparency: // SHORTCUT   - INCREASE TRANSPARENCY
    OnChangeTransparency( true );
    break;
  case GEOMOp::OpDecrTransparency: // SHORTCUT   - DECREASE TRANSPARENCY
    OnChangeTransparency( false );
    break;
  case GEOMOp::OpIsos:           // POPUP - ISOS
    OnNbIsos();
    break;
  case GEOMOp::OpIncrNbIsos:     // SHORTCUT   - INCREASE NB ISOLINES
    OnNbIsos( INCR );
    break;
  case GEOMOp::OpDecrNbIsos:     // SHORTCUT   - DECREASE NB ISOLINES
    OnNbIsos( DECR );
    break;
  case GEOMOp::OpMaterialProperties: // POPUP - MATERIAL PROPERTIES
    OnMaterialProperties();
    break;
  case GEOMOp::OpPredefMaterCustom:  // POPUP  - MATERIAL PROPERTIES - CUSTOM...
    OnMaterialProperties();
    break;
  case GEOMOp::OpMaterialsLibrary:    // POPUP MENU - MATERIAL PROPERTIES
    OnMaterialsLibrary();
    break;
  case GEOMOp::OpAutoColor:      // POPUP - AUTO COLOR
    OnAutoColor();
    break;
  case GEOMOp::OpNoAutoColor:    // POPUP - DISABLE AUTO COLOR
    OnDisableAutoColor();
    break;
  case GEOMOp::OpDiscloseChildren:   // POPUP - SHOW CHILDREN
  case GEOMOp::OpConcealChildren:   // POPUP - HIDE CHILDREN
    OnDiscloseConcealChildren( theCommandID == GEOMOp::OpDiscloseChildren );
    break;
  case GEOMOp::OpPointMarker:    // POPUP - POINT MARKER
    OnPointMarker();
    break;
  case GEOMOp::OpUnpublishObject:// POPUP - UNPUBLISH
    OnUnpublishObject();
    break;
  case GEOMOp::OpPublishObject:// GEOM ROOT OBJECT - POPUP - PUBLISH
    OnPublishObject();
    break;
  case GEOMOp::OpEdgeWidth:
    OnEdgeWidth();
    break;
  case GEOMOp::OpIsosWidth:
    OnIsosWidth();
    break;
  case GEOMOp::OpBringToFront:
    OnBringToFront();
    break;
  case GEOMOp::OpClsBringToFront:
    OnClsBringToFront();
     break;
  case GEOMOp::OpCreateFolder:
    OnCreateFolder();
     break;
  case GEOMOp::OpSortChildren:
    OnSortChildren();
    break;
  case GEOMOp::OpShowDependencyTree:
    OnShowDependencyTree();
     break;
  case GEOMOp::OpReduceStudy:
    OnReduceStudy();
    break;
  default:
    SUIT_Session::session()->activeApplication()->putInfo(tr("GEOM_PRP_COMMAND").arg(theCommandID));
    break;
  }
  return true;
}

//=======================================================================
// function : OnGUIEvent()
// purpose  :
//=======================================================================
bool GEOMToolsGUI::OnGUIEvent(int theCommandID, SUIT_Desktop* parent, const QVariant& theParam )
{
  getGeometryGUI()->EmitSignalDeactivateDialog();

  switch ( theCommandID ) {
  case GEOMOp::OpPredefMaterial:         // POPUP MENU - MATERIAL PROPERTIES - <SOME MATERIAL>
    OnSetMaterial( theParam );
    break;
  default:
    SUIT_Session::session()->activeApplication()->putInfo(tr("GEOM_PRP_COMMAND").arg(theCommandID));
    break;
  }
  return true;
}

//===============================================================================
// function : OnEditDelete()
// purpose  :
//===============================================================================
void GEOMToolsGUI::OnEditDelete()
{
  SALOME_ListIO selected;
  SalomeApp_Application* app =
    dynamic_cast< SalomeApp_Application* >( SUIT_Session::session()->activeApplication() );
  if ( !app )
    return;

  LightApp_SelectionMgr* aSelMgr = app->selectionMgr();
  SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
  if ( !aSelMgr || !appStudy )
    return;

  // get selection
  aSelMgr->selectedObjects( selected, "ObjectBrowser", false );
  if ( selected.IsEmpty() )
    return;

  _PTR(Study) aStudy = appStudy->studyDS();
  _PTR(UseCaseBuilder) aUseCaseBuilder = aStudy->GetUseCaseBuilder();

  // check if study is locked
  if ( _PTR(AttributeStudyProperties)( aStudy->GetProperties() )->IsLocked() ) {
    SUIT_MessageBox::warning( app->desktop(),
                              tr("WRN_WARNING"),
                              tr("WRN_STUDY_LOCKED") );
    return; // study is locked
  }

  // get GEOM component
  CORBA::String_var geomIOR = app->orb()->object_to_string( GeometryGUI::GetGeomGen() );
  QString geomComp = getParentComponent( aStudy->FindObjectIOR( geomIOR.in() ) );

  // check each selected object: if belongs to GEOM, if not reference...
  QMap<QString,QString> toBeDeleted;
  QMap<QString,QString> allDeleted;
  QMap<QString,QString> toBeDelFolders;
  bool isComponentSelected = false;

  for ( SALOME_ListIteratorOfListIO It( selected ); It.More(); It.Next() ) {
    Handle(SALOME_InteractiveObject) anIObject = It.Value();
    if ( !anIObject->hasEntry() )
      continue; // invalid object
    // ...
    QString entry = anIObject->getEntry();
    _PTR(SObject) obj = aStudy->FindObjectID( entry.toLatin1().data() );
    // check parent component
    QString parentComp = getParentComponent( obj );
    if ( parentComp != geomComp )  {
      SUIT_MessageBox::warning( app->desktop(),
                                QObject::tr("ERR_ERROR"),
                                QObject::tr("NON_GEOM_OBJECTS_SELECTED").arg( getGeometryGUI()->moduleName() ) );
      return; // not GEOM object selected
    }

    ///////////////////////////////////////////////////////
    // if GEOM component is selected, so skip other checks
    if ( isComponentSelected ) continue;
    ///////////////////////////////////////////////////////

    // check if object is reference
    _PTR(SObject) refobj;
    if ( obj && obj->ReferencedObject( refobj ) ) {
      // get the main object by reference IPAL 21354
      obj = refobj;
      entry = obj->GetID().c_str();
    }
    // ...
    QString aName = obj->GetName().c_str();
    if ( entry == geomComp ) {
      // GEOM component is selected, skip other checks
      isComponentSelected = true;
      continue;
    }
    // all sub-objects of folder have to be deleted
    getGeomChildrenAndFolders( obj, toBeDeleted, toBeDelFolders );
    allDeleted.insert( entry, aName ); // skip GEOM component
    // browse through all children recursively
    _PTR(UseCaseIterator) it ( aUseCaseBuilder->GetUseCaseIterator( obj ) );
    for ( it->Init( true ); it->More(); it->Next() ) {
      _PTR(SObject) child( it->Value() );
      if ( child && child->ReferencedObject( refobj ) )
        continue; // skip references
      aName = child->GetName().c_str();
      if ( !aName.isEmpty() )
        allDeleted.insert( child->GetID().c_str(), aName );
    }
  }

  // is there is anything to delete?
  if ( !isComponentSelected && allDeleted.count() <= 0 )
    return; // nothing to delete

  // show confirmation dialog box
  GEOMToolsGUI_DeleteDlg dlg( app->desktop(), allDeleted, isComponentSelected );
  if ( !dlg.exec() )
    return; // operation is cancelled by user

  // get currently opened views
  QList<SALOME_View*> views;
  SALOME_View* view;
  ViewManagerList vmans = app->viewManagers();
  SUIT_ViewManager* vman;
  Q_FOREACH ( vman, vmans ) {
    SUIT_ViewModel* vmod = vman->getViewModel();
    view = dynamic_cast<SALOME_View*> ( vmod ); // must work for OCC and VTK views
    if ( view )
      views.append( view );
  }

  _PTR(StudyBuilder) aStudyBuilder (aStudy->NewBuilder());
  GEOM_Displayer* disp = new GEOM_Displayer( appStudy );

  if ( isComponentSelected ) {
    // GEOM component is selected: delete all objects recursively
    _PTR(SObject) comp = aStudy->FindObjectID( geomComp.toLatin1().data() );
    if ( !comp )
      return;
    _PTR(ChildIterator) it ( aStudy->NewChildIterator( comp ) );
    // remove top-level objects only
    for ( it->InitEx( false ); it->More(); it->Next() ) {
      _PTR(SObject) child( it->Value() );
      // remove object from GEOM engine
      removeObjectWithChildren( child, aStudy, views, disp );
      // remove object from study
      aStudyBuilder->RemoveObjectWithChildren( child );
      // remove object from use case tree
      aUseCaseBuilder->Remove( child );
    }
  }
  else {
    // GEOM component is not selected: check if selected objects are in use
    if ( inUse( aStudy, geomComp, allDeleted ) && 
         SUIT_MessageBox::question( app->desktop(),
                                    QObject::tr("WRN_WARNING"),
                                    QObject::tr("DEP_OBJECT"),
                                    SUIT_MessageBox::Yes | SUIT_MessageBox::No,
                                    SUIT_MessageBox::No ) != SUIT_MessageBox::Yes ) {
      return; // object(s) in use
    }
    // ... and then delete all objects
    QMap<QString, QString>::Iterator it;
    for ( it = toBeDeleted.begin(); it != toBeDeleted.end(); ++it ) {
      _PTR(SObject) obj ( aStudy->FindObjectID( it.key().toLatin1().data() ) );
      // remove object from GEOM engine
      removeObjectWithChildren( obj, aStudy, views, disp );
      // remove objects from study
      aStudyBuilder->RemoveObjectWithChildren( obj );
      // remove object from use case tree
      aUseCaseBuilder->Remove( obj );
    }
    // ... and then delete all folders
    for ( it = toBeDelFolders.begin(); it != toBeDelFolders.end(); ++it ) {
      _PTR(SObject) obj ( aStudy->FindObjectID( it.key().toLatin1().data() ) );
      // remove object from GEOM engine
      removeObjectWithChildren( obj, aStudy, views, disp );
      // remove objects from study
      aStudyBuilder->RemoveObjectWithChildren( obj );
      // remove object from use case tree
      aUseCaseBuilder->Remove( obj );
    }
  }

  selected.Clear();
  aSelMgr->setSelectedObjects( selected );
  getGeometryGUI()->updateObjBrowser();
  app->updateActions(); //SRN: To update a Save button in the toolbar
}

//=====================================================================================
// function : Import
// purpose  : BRep, Iges, Step, ...
//=====================================================================================
bool GEOMToolsGUI::Import()
{
  SalomeApp_Application* app = dynamic_cast< SalomeApp_Application* >( getGeometryGUI()->getApp() );
  if ( !app ) return false;

  SalomeApp_Study* stud = dynamic_cast<SalomeApp_Study*> ( app->activeStudy() );
  if ( !stud ) {
    MESSAGE ( "FAILED to cast active study to SalomeApp_Study" );
    return false;
  }
  _PTR(Study) aStudy = stud->studyDS();

  // check if study is locked
  bool aLocked = (_PTR(AttributeStudyProperties)(aStudy->GetProperties()))->IsLocked();
  if ( aLocked ) {
    SUIT_MessageBox::warning( app->desktop(),
                              QObject::tr("WRN_WARNING"),
                              QObject::tr("WRN_STUDY_LOCKED") );
    return false;
  }

  // check if GEOM engine is available
  GEOM::GEOM_Gen_var eng = GeometryGUI::GetGeomGen();
  if ( CORBA::is_nil( eng ) ) {
    SUIT_MessageBox::critical( app->desktop(),
                               QObject::tr("WRN_WARNING"),
                               QObject::tr( "GEOM Engine is not started" ) );
    return false;
  }

  GEOM::GEOM_IInsertOperations_var aInsOp = eng->GetIInsertOperations( aStudy->StudyId() );
  if ( aInsOp->_is_nil() )
    return false;

  // obtain a list of available import formats
  FilterMap aMap;
  GEOM::string_array_var aFormats, aPatterns;
  aInsOp->ImportTranslators( aFormats, aPatterns );

  for ( int i = 0, n = aFormats->length(); i < n; i++ )
    aMap.insert( (char*)aPatterns[i], (char*)aFormats[i] );

  // select files to be imported
  QString fileType;
  QStringList fileNames = getFileNames( app->desktop(), "", aMap,
                                        tr( "GEOM_MEN_IMPORT" ), fileType, true );

  // set Wait cursor
  SUIT_OverrideCursor wc;

  if ( fileNames.count() == 0 )
    return false; // nothing selected, return

  QStringList errors;

  QList< GEOM::GEOM_Object_var > objsForDisplay;

  QStringList anEntryList;

  // iterate through all selected files

  SUIT_MessageBox::StandardButton igesAnswer = SUIT_MessageBox::NoButton;
  SUIT_MessageBox::StandardButton acisAnswer = SUIT_MessageBox::NoButton;

  for ( int i = 0; i < fileNames.count(); i++ ) {
    QString fileName = fileNames[i];

    if ( fileName.isEmpty() )
      continue;

    QString aCurrentType;
    if ( fileType.isEmpty() ) {
      // file type is not defined, try to detect
      QString ext = QFileInfo( fileName ).suffix().toUpper();
      QRegExp re( "\\*\\.(\\w+)" );
      for ( FilterMap::const_iterator it = aMap.begin();
            it != aMap.end() && aCurrentType.isEmpty(); ++it ) {
        int pos = 0;
        while ( re.indexIn( it.key(), pos ) >= 0 ) {
          QString f = re.cap(1).trimmed().toUpper();
          if ( ext == f ) { aCurrentType = it.value(); break; }
          pos = re.pos() + re.cap(1).length() + 2;
        }
      }
    }
    else {
      aCurrentType = fileType;
    }

    if ( aCurrentType.isEmpty() ) {
      errors.append( QString( "%1 : %2" ).arg( fileName ).arg( tr( "GEOM_UNSUPPORTED_TYPE" ) ) );
      continue;
    }

    GEOM_Operation* anOp = new GEOM_Operation( app, aInsOp.in() );
    try {
      app->putInfo( tr( "GEOM_PRP_LOADING" ).arg( SUIT_Tools::file( fileName, /*withExten=*/true ) ) );
      anOp->start();

      CORBA::String_var fileN = fileName.toUtf8().constData();
      CORBA::String_var fileT = aCurrentType.toUtf8().constData();

      // jfa 21.08.2012 for mantis issue 21511 (STEP file units)
      CORBA::String_var aUnits = aInsOp->ReadValue(fileN, fileT, "LEN_UNITS");
      QString aUnitsStr (aUnits.in());
      bool needConvert = true;
      if (aUnitsStr.isEmpty() || aUnitsStr == "M" || aUnitsStr.toLower() == "metre")
        needConvert = false;

      if (needConvert) {
        if (igesAnswer == SUIT_MessageBox::NoToAll) {
          // converting for all files is already approved
          fileT = (aCurrentType + "_SCALE").toLatin1().constData();
        }
        else if (igesAnswer != SUIT_MessageBox::YesToAll) {
          SUIT_MessageBox::StandardButtons btns = SUIT_MessageBox::Yes | SUIT_MessageBox::No;
          if (i < fileNames.count() - 1) btns = btns | SUIT_MessageBox::YesToAll | SUIT_MessageBox::NoToAll;
          igesAnswer = SUIT_MessageBox::question(app->desktop(),
                                                 "Question",//tr("WRN_WARNING"),
                                                 tr("GEOM_SCALE_DIMENSIONS").arg(aUnitsStr),
                                                 btns | SUIT_MessageBox::Cancel,
                                                 SUIT_MessageBox::No);
          switch (igesAnswer) {
          case SUIT_MessageBox::Cancel:
            return false;                // cancel (break) import operation
          case SUIT_MessageBox::Yes:
          case SUIT_MessageBox::YesToAll:
            break;                       // scaling is confirmed
          case SUIT_MessageBox::No:
          case SUIT_MessageBox::NoAll:
            fileT = (aCurrentType + "_SCALE").toLatin1().constData();
          default:
            break;                       // scaling is rejected
          } // switch ( igesAnswer )
        } // if ( igeAnswer != NoToAll )
      } // if ( needConvert )

      /*
      // skl 29.05.2009
      if ( aCurrentType == "IGES" ) {
        GEOM::GEOM_Object_var anObj = aInsOp->ImportFile( fileN, "IGES_UNIT" );
        bool needConvert = false;
        TCollection_AsciiString aUnitName = aInsOp->GetErrorCode();
        if ( aUnitName.SubString( 1, 4 ) == "UNIT" )
          needConvert = aUnitName.SubString( 6, aUnitName.Length() ) != "M";

        if ( needConvert ) {
          if ( igesAnswer == SUIT_MessageBox::NoToAll ) {
            // converting for all files is already approved
            fileT = "IGES_SCALE";
          }
          else if ( igesAnswer != SUIT_MessageBox::YesToAll ) {
            SUIT_MessageBox::StandardButtons btns = SUIT_MessageBox::Yes | SUIT_MessageBox::No;
            if ( i < fileNames.count()-1 ) btns = btns | SUIT_MessageBox::YesToAll | SUIT_MessageBox::NoToAll;
            igesAnswer = SUIT_MessageBox::question( app->desktop(),
                                                    "Question",//tr("WRN_WARNING"),
                                                    tr("GEOM_SCALE_DIMENSIONS"),
                                                    btns | SUIT_MessageBox::Cancel,
                                                    SUIT_MessageBox::No );
            switch ( igesAnswer ) {
            case SUIT_MessageBox::Cancel:
              return false;                // cancel (break) import operation
            case SUIT_MessageBox::Yes:
            case SUIT_MessageBox::YesToAll:
              break;                       // scaling is confirmed
            case SUIT_MessageBox::No:
            case SUIT_MessageBox::NoAll:
              fileT = "IGES_SCALE";
            default:
              break;                       // scaling is rejected
            } // switch ( igesAnswer )
          } // if ( igeAnswer != NoToAll )
        } // if ( needConvert )
      } // if ( aCurrentType == "IGES" )
      else if ( aCurrentType == "ACIS" ) {
      */

      if ( aCurrentType == "ACIS" ) {
        if ( acisAnswer != SUIT_MessageBox::YesToAll && acisAnswer != SUIT_MessageBox::NoToAll ) {
          SUIT_MessageBox::StandardButtons btns = SUIT_MessageBox::Yes | SUIT_MessageBox::No;
          if ( i < fileNames.count()-1 ) btns = btns | SUIT_MessageBox::YesToAll | SUIT_MessageBox::NoToAll;
          acisAnswer = SUIT_MessageBox::question( app->desktop(),
                                                  "Question",//tr("WRN_WARNING"),
                                                  tr("GEOM_PUBLISH_NAMED_SHAPES"),
                                                  btns | SUIT_MessageBox::Cancel,
                                                  SUIT_MessageBox::No );
          if ( acisAnswer == SUIT_MessageBox::Cancel )
            return false; // cancel (break) import operation
        } // if ( acisAnswer != YesToAll && acisAnswer != NoToAll )
      } // else if ( aCurrentType == "ACIS" )

      // IMPORT
      GEOM::ListOfGO_var anObj = aInsOp->ImportFile( fileN, fileT );

      if ( anObj->length() > 0 && aInsOp->IsDone() ) {
        GEOM::GEOM_Object_ptr aFather = anObj[0]._retn();
        QString aPublishObjName =
          GEOMBase::GetDefaultName( SUIT_Tools::file( fileName, /*withExten=*/true ) );

        SALOMEDS::Study_var aDSStudy = GeometryGUI::ClientStudyToStudy( aStudy );
        SALOMEDS::SObject_var aSO =
          GeometryGUI::GetGeomGen()->PublishInStudy( aDSStudy,
                                                     SALOMEDS::SObject::_nil(),
                                                     aFather,
                                                     aPublishObjName.toLatin1().constData() );
        if ( ( !aSO->_is_nil() ) )
          anEntryList.append( aSO->GetID() );

        objsForDisplay.append( aFather );

        if ( aCurrentType == "ACIS" ) {
          if ( acisAnswer == SUIT_MessageBox::Yes || acisAnswer == SUIT_MessageBox::YesToAll )
            GeometryGUI::GetGeomGen()->PublishNamedShapesInStudy( aDSStudy, aFather );
        }

        anOp->commit();

        // Treat group objects.
        for (int i = 1, n = anObj->length(); i < n; i++) {
          GEOM::GEOM_Object_ptr anObject = anObj[i]._retn();
          GeometryGUI::GetGeomGen()->AddInStudy(aDSStudy,
            anObject, tr(anObject->GetName()).toStdString().c_str(), aFather);
        }
      }
      else {
        anOp->abort();
        errors.append( QString( "%1 : %2" ).arg( fileName ).arg( aInsOp->GetErrorCode() ) );
      }
    }
    catch( const SALOME::SALOME_Exception& S_ex ) {
      anOp->abort();
      errors.append( QString( "%1 : %2" ).arg( fileName ).arg( tr( "GEOM_UNKNOWN_IMPORT_ERROR" ) ) );
    }
  }

  // update object browser
  getGeometryGUI()->updateObjBrowser( true );

  // browse published objects
  app->browseObjects( anEntryList );

  // display imported model (if only one file is selected)
  if ( objsForDisplay.count() == 1 )
    GEOM_Displayer( stud ).Display( objsForDisplay[0].in() );

  if ( errors.count() > 0 ) {
    SUIT_MessageBox::critical( app->desktop(),
                               QObject::tr( "GEOM_ERROR" ),
                               QObject::tr( "GEOM_IMPORT_ERRORS" ) + "\n" + errors.join( "\n" ) );
  }

  app->updateActions(); //SRN: To update a Save button in the toolbar

  return objsForDisplay.count() > 0;
}

//=====================================================================================
// function : Export
// purpose  : BRep, Iges, Step
//=====================================================================================
bool GEOMToolsGUI::Export()
{
  SalomeApp_Application* app = getGeometryGUI()->getApp();
  if (!app) return false;

  SalomeApp_Study* stud = dynamic_cast<SalomeApp_Study*> ( app->activeStudy() );
  if ( !stud ) {
    MESSAGE ( "FAILED to cast active study to SalomeApp_Study" );
    return false;
  }
  _PTR(Study) aStudy = stud->studyDS();

  GEOM::GEOM_Gen_var eng = GeometryGUI::GetGeomGen();
  if ( CORBA::is_nil( eng ) ) {
    SUIT_MessageBox::critical( app->desktop(),
                               QObject::tr("WRN_WARNING"),
                               QObject::tr( "GEOM Engine is not started" ) );
    return false;
  }

  GEOM::GEOM_IInsertOperations_var aInsOp = eng->GetIInsertOperations( aStudy->StudyId() );
  if ( aInsOp->_is_nil() )
    return false;

  // Obtain a list of available export formats
  FilterMap aMap;
  QStringList filters;
  GEOM::string_array_var aFormats, aPatterns;
  aInsOp->ExportTranslators( aFormats, aPatterns );
  for ( int i = 0, n = aFormats->length(); i < n; i++ ) {
    aMap.insert( (char*)aPatterns[i], (char*)aFormats[i] );
    filters.push_back( (char*)aPatterns[i] );
  }

  // Get selected objects
  LightApp_SelectionMgr* sm = app->selectionMgr();
  if ( !sm )
    return false;

  SALOME_ListIO selectedObjects;
  sm->selectedObjects( selectedObjects );
  bool appropriateObj = false;

  SALOME_ListIteratorOfListIO It( selectedObjects );
  for (; It.More(); It.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();
    GEOM::GEOM_Object_var anObj = GEOMBase::ConvertIOinGEOMObject( IObject );

    if ( anObj->_is_nil() )
      continue;

    QString fileType;
    QString file = getFileName(app->desktop(), QString( IObject->getName() ), aMap, filters,
                               tr("GEOM_MEN_EXPORT"), false, fileType, true);

    // User has pressed "Cancel" --> stop the operation
    if ( file.isEmpty() || fileType.isEmpty() )
      return false;

    GEOM_Operation* anOp = new GEOM_Operation( app, aInsOp.in() );
    try {
      SUIT_OverrideCursor wc;

      app->putInfo( tr("GEOM_PRP_EXPORT").arg(SUIT_Tools::file( file, /*withExten=*/true )) );

      anOp->start();

      aInsOp->Export( anObj, file.toUtf8().constData(), fileType.toUtf8().constData() );

      if (aInsOp->IsDone())
        anOp->commit();
      else {
        anOp->abort();
        wc.suspend();
        SUIT_MessageBox::critical(app->desktop(),
                                  QObject::tr("GEOM_ERROR"),
                                  QObject::tr("GEOM_PRP_ABORT") + "\n" + QObject::tr(aInsOp->GetErrorCode()));
        return false;
      }
    }
    catch (const SALOME::SALOME_Exception& S_ex) {
      //QtCatchCorbaException(S_ex);
      anOp->abort();
      return false;
    }
    appropriateObj = true;
  }

  if ( !appropriateObj )
    SUIT_MessageBox::warning( app->desktop(),
                              QObject::tr("WRN_WARNING"),
                              QObject::tr("GEOM_WRN_NO_APPROPRIATE_SELECTION") );
  return true;
}

//=====================================================================================
// function : RemoveObjectWithChildren
// purpose  : used by OnEditDelete() method
//=====================================================================================
void GEOMToolsGUI::removeObjectWithChildren(_PTR(SObject) obj,
                                            _PTR(Study) aStudy,
                                            QList<SALOME_View*> views,
                                            GEOM_Displayer* disp)
{
  // iterate through all children of obj
  for (_PTR(ChildIterator) it (aStudy->NewChildIterator(obj)); it->More(); it->Next()) {
  // for (_PTR(UseCaseIterator) it (aStudy->GetUseCaseBuilder()->GetUseCaseIterator(obj)); it->More(); it->Next()) {
    _PTR(SObject) child (it->Value());
    removeObjectWithChildren(child, aStudy, views, disp);
  }

  // erase object and remove it from engine
  _PTR(GenericAttribute) anAttr;
  if (obj->FindAttribute(anAttr, "AttributeIOR")) {
    _PTR(AttributeIOR) anIOR (anAttr);

    SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>( SUIT_Session::session()->activeApplication()->activeStudy() );

    // Delete shape in Client
    const TCollection_AsciiString ASCIor ((char*)anIOR->Value().c_str());
    getGeometryGUI()->GetShapeReader().RemoveShapeFromBuffer(ASCIor);

    CORBA::Object_var corbaObj = GeometryGUI::ClientSObjectToObject(obj);
    GEOM::GEOM_Object_var geomObj = GEOM::GEOM_Object::_narrow( corbaObj );
    if (!CORBA::is_nil(geomObj)) {

      //Remove visual properties of the object
      appStudy->removeObjectFromAll(obj->GetID().c_str());

      // Erase graphical object
      QListIterator<SALOME_View*> it( views );
      while ( it.hasNext() )
        if ( SALOME_View* view = it.next() )
          disp->Erase(geomObj, true, true, view);

      // Remove object from Engine
      // We can't directly remove object from engine. All we can do is to unpublish the object
      // from the study. Another client could be using the object.
      // Unpublishing is done just after in aStudyBuilder->RemoveObjectWithChildren( child );
      //GeometryGUI::GetGeomGen()->RemoveObject( geomObj );
    }
  }
}

//=================================================================================
// function : deactivate()
// purpose  : Called when GEOM component is deactivated
//=================================================================================
void GEOMToolsGUI::deactivate()
{
  SalomeApp_Application* app = dynamic_cast< SalomeApp_Application* >( SUIT_Session::session()->activeApplication() );
  if ( app ) {
    SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
    GEOM_Displayer aDisp (appStudy);
    aDisp.GlobalSelection();
    getGeometryGUI()->setLocalSelectionMode(GEOM_ALLOBJECTS);
  }
}

//=====================================================================================
// EXPORTED METHODS
//=====================================================================================
extern "C"
{
#ifdef WIN32
  __declspec( dllexport )
#endif
  GEOMGUI* GetLibGUI( GeometryGUI* parent )
  {
    return new GEOMToolsGUI( parent );
  }
}
