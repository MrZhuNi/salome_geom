// Copyright (C) 2014  CEA/DEN, EDF R&D, OPEN CASCADE
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

// internal includes
#include "RepairGUI_InspectObjectDlg.h"

// GEOM includes
#include <GEOMBase.h>
#include <GEOM_Constants.h>

// GUI includes
#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Desktop.h>
#include <SUIT_ViewManager.h>
#include <SUIT_ViewWindow.h>

#include <LightApp_SelectionMgr.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>

#include <OCCViewer_ViewModel.h>
#include <SVTK_ViewModel.h>

// OCCT includes
#include <TopoDS_Iterator.hxx>

// Qt includes
#include <QGridLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QInputDialog>

//=================================================================================
// class    : RepairGUI_InspectObjectDlg
// purpose  : class for "Inspect Object" tree item creation
//=================================================================================
class RepairGUI_InspectObjectDlg::TreeWidgetItem : public QTreeWidgetItem
{
public:
  TreeWidgetItem( QTreeWidget*, const QStringList&, const TopoDS_Shape&, const Handle(SALOME_InteractiveObject)&, int = Type );
  TreeWidgetItem( QTreeWidgetItem*, const QStringList&, const TopoDS_Shape&, const QString&, int = Type );
  ~TreeWidgetItem();

  bool                             isVisible();
  void                             setVisible( bool, QIcon& );

  TopoDS_Shape                     getShape() const;
  Handle(SALOME_InteractiveObject) getIO() const;

private:
  bool                             myIsVisible;
  TopoDS_Shape                     myShape;
  Handle(SALOME_InteractiveObject) myIO;

};

RepairGUI_InspectObjectDlg::TreeWidgetItem::TreeWidgetItem( QTreeWidget* view, const QStringList &strings, const TopoDS_Shape& shape,
                                                            const Handle(SALOME_InteractiveObject)& io, int type )
: QTreeWidgetItem( view, strings, type ),
  myIsVisible( false ),
  myShape( shape ),
  myIO( io )
{
}

RepairGUI_InspectObjectDlg::TreeWidgetItem::TreeWidgetItem( QTreeWidgetItem* parent, const QStringList &strings,
                                                            const TopoDS_Shape& shape, const QString& entry, int type )
: QTreeWidgetItem( parent, strings, type ),
  myIsVisible( false ),
  myShape( shape )
{
  myIO = new SALOME_InteractiveObject( entry.toAscii(), "GEOM", "TEMP_IO" );
}

RepairGUI_InspectObjectDlg::TreeWidgetItem::~TreeWidgetItem()
{
}

bool RepairGUI_InspectObjectDlg::TreeWidgetItem::isVisible()
{
  return myIsVisible;
}

void RepairGUI_InspectObjectDlg::TreeWidgetItem::setVisible( bool isVisible, QIcon& icon )
{
  myIsVisible = isVisible;
  setIcon( 1, icon );
}

TopoDS_Shape RepairGUI_InspectObjectDlg::TreeWidgetItem::getShape() const
{
  return myShape;
}

Handle(SALOME_InteractiveObject) RepairGUI_InspectObjectDlg::TreeWidgetItem::getIO() const
{
  return myIO;
}

//=================================================================================
// class    : RepairGUI_InspectObjectDlg()
// purpose  : Constructs a RepairGUI_InspectObjectDlg which is a child of 'parent'.
//=================================================================================
RepairGUI_InspectObjectDlg::RepairGUI_InspectObjectDlg( SUIT_Desktop* parent )
: GEOMBase_Helper( parent ),
  QDialog( parent ),
  myTransparency( 0.0 ),
  myIsSelectAll( false )
{
  QIcon iconSelect( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );
  myVisible = QIcon( SUIT_Session::session()->resourceMgr()->loadPixmap( "SUIT", tr( "ICON_DATAOBJ_VISIBLE" ) ) );
  myInvisible = QIcon( SUIT_Session::session()->resourceMgr()->loadPixmap( "SUIT", tr( "ICON_DATAOBJ_INVISIBLE" ) ) );

  setWindowTitle( tr( "GEOM_INSPECT_OBJECT_TITLE" ) );
  setAttribute( Qt::WA_DeleteOnClose );

  myApp = dynamic_cast< SalomeApp_Application* >( SUIT_Session::session()->activeApplication() );
  if ( !myApp ) return;

  SUIT_ViewWindow* aViewWindow = 0;
  SUIT_Study* activeStudy = myApp->activeStudy();
  if (activeStudy)
    aViewWindow = myApp->desktop()->activeWindow();
  if (aViewWindow == 0) return;
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( myApp->activeStudy() );
  SUIT_ViewManager* aViewManager = aViewWindow->getViewManager();
  if( aViewManager->getType() != OCCViewer_Viewer::Type() &&
      aViewManager->getType() != SVTK_Viewer::Type())
    return;

  myGlobalId = aViewManager->getGlobalId();

  QGridLayout* topLayout = new QGridLayout( this );
  topLayout->setMargin( 11 ); topLayout->setSpacing( 6 );

  /**********************   Inspected Object    **********************/

  QHBoxLayout* mainShapeLayout = new QHBoxLayout();

  QLabel* label = new QLabel( tr( "GEOM_INSPECT_OBJECT_MAIN_SHAPE" ) );
  QPushButton* selBtn = new QPushButton();
  selBtn->setIcon( iconSelect );
  myEditMainShape = new QLineEdit();
  myEditMainShape->setReadOnly(true);

  mainShapeLayout->addWidget( label );
  mainShapeLayout->addWidget( selBtn );
  mainShapeLayout->addWidget( myEditMainShape );

  /**********************   Sub-objects tree    **********************/

  myTreeObjects = new QTreeWidget();
  myTreeObjects->setColumnCount( 2 );
  QStringList columnNames;
  columnNames.append( tr( "GEOM_INSPECT_OBJECT_NAME" ) );
  columnNames.append("");
  myTreeObjects->setHeaderLabels( columnNames );
  QTreeWidgetItem* headerItem = new QTreeWidgetItem( columnNames );
  myTreeObjects->setHeaderItem( headerItem );
  myTreeObjects->header()->moveSection( 1, 0 );
  myTreeObjects->header()->setClickable( true );
  myTreeObjects->header()->setMovable( false );
  myTreeObjects->header()->setResizeMode( 1, QHeaderView::ResizeToContents );
  myTreeObjects->setSelectionMode( QAbstractItemView::ExtendedSelection );

  /**********************        Buttons        **********************/

  QVBoxLayout* buttonsLayout1 = new QVBoxLayout();

  QPushButton* buttonShow = new QPushButton( tr( "GEOM_INSPECT_OBJECT_SHOW" ) );
  QPushButton* buttonShowOnly = new QPushButton( tr( "GEOM_INSPECT_OBJECT_SHOW_ONLY" ) );
  QPushButton* buttonHide = new QPushButton( tr( "GEOM_INSPECT_OBJECT_HIDE" ) );
  QPushButton* buttonPublish = new QPushButton( tr( "GEOM_INSPECT_OBJECT_PUBLISH" ) );

  buttonsLayout1->addWidget( buttonShow );
  buttonsLayout1->addWidget( buttonShowOnly );
  buttonsLayout1->addWidget( buttonHide );
  buttonsLayout1->addStretch();
  buttonsLayout1->addWidget( buttonPublish );
  buttonsLayout1->addStretch();

  QHBoxLayout* buttonsLayout2 = new QHBoxLayout();

  QPushButton* buttonClose = new QPushButton( tr( "GEOM_BUT_CLOSE" ) );
  QPushButton* buttonHelp = new QPushButton( tr( "GEOM_BUT_HELP" ) );

  buttonsLayout2->addWidget( buttonClose );
  buttonsLayout2->addStretch();
  buttonsLayout2->addWidget( buttonHelp );

  topLayout->addLayout( mainShapeLayout, 0, 0 );
  topLayout->addWidget( myTreeObjects, 1, 0 );
  topLayout->addLayout( buttonsLayout1, 0, 1, 2, 1 );
  topLayout->addLayout( buttonsLayout2, 2, 0, 1, 2 );

  // Signals and slots connections

  connect( selBtn, SIGNAL( clicked() ), this, SLOT( onEditMainShape() ) );

  connect( myTreeObjects, SIGNAL( itemClicked( QTreeWidgetItem*, int ) ),
           this, SLOT( onItemClicked( QTreeWidgetItem*, int ) ) );
  connect( myTreeObjects, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ),
           this, SLOT( onItemDoubleClicked( QTreeWidgetItem*, int ) ) );
  connect( myTreeObjects, SIGNAL( itemExpanded ( QTreeWidgetItem* ) ),
           this, SLOT( onItemExpanded( QTreeWidgetItem* ) ) );
  connect( myTreeObjects, SIGNAL( itemSelectionChanged() ),
           this, SLOT( onItemSelectionChanged() ) );

  connect( myTreeObjects->header(), SIGNAL( sectionClicked( int ) ), this, SLOT( onHeaderClicked( int ) ) );

  connect( buttonShow,     SIGNAL( clicked() ), this, SLOT( clickOnShow() ) );
  connect( buttonShowOnly, SIGNAL( clicked() ), this, SLOT( clickOnShowOnly() ) );
  connect( buttonHide,     SIGNAL( clicked() ), this, SLOT( clickOnHide() ) );
  connect( buttonPublish,  SIGNAL( clicked() ), this, SLOT( clickOnPublish() ) );

  connect( buttonClose,    SIGNAL( clicked() ), this, SLOT( reject() ) );
  connect( buttonHelp,     SIGNAL( clicked() ), this, SLOT( clickOnHelp() ) );

  connect( myApp->selectionMgr(), SIGNAL( currentSelectionChanged() ),
           this, SLOT( onViewSelectionChanged() ) );

  init();
}

RepairGUI_InspectObjectDlg::~RepairGUI_InspectObjectDlg()
{
  // restore initial parameters for viewer
  getDisplayer()->UnsetColor();
  getDisplayer()->UnsetWidth();
  // no need to delete child widgets, Qt does it all for us
}

//=================================================================================
// function : init()
// purpose  : initialize dialog data
//=================================================================================
void RepairGUI_InspectObjectDlg::init()
{
  //get shape from selection
  SALOME_ListIO selected;
  myApp->selectionMgr()->selectedObjects(selected);

  if( selected.Extent() != 1 )
    return;

  TopoDS_Shape aShape = GEOMBase::GetTopoFromSelection( selected );
  if( aShape.IsNull() )
    return;

  Handle(SALOME_InteractiveObject) anIO = selected.First();
  GEOM::GEOM_Object_var anObject = GEOMBase::ConvertIOinGEOMObject( anIO );
  QString aName = anObject->GetName();
  CORBA::String_var anEntry = anObject->GetStudyEntry();

  myEditMainShape->setText( aName );
  myEditMainShape->setEnabled( false );

  // remember initial transparency value
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( myApp->activeStudy() );
  QVariant v = aStudy->getObjectProperty( myGlobalId, QString( anEntry.in() ),
                                          GEOM::propertyName( GEOM::Transparency ), myTransparency );
  if( v.canConvert( QVariant::Double ) )
    myTransparency = v.toDouble();
  aStudy->setObjectProperty( myGlobalId, QString( anEntry.in() ), GEOM::propertyName( GEOM::Transparency ), 0.5 );

  if ( GEOM_Displayer::GetActiveView()->isVisible( anIO ) )
    getDisplayer()->Redisplay( anIO, true );

  TreeWidgetItem* anItem = new TreeWidgetItem( myTreeObjects, QStringList() << aName, aShape, anIO );
  if( getDisplayer()->IsDisplayed( anEntry.in() ) )
    anItem->setVisible( true, myVisible );
  else
    anItem->setVisible( false, myInvisible );

  // add sub-objects in the tree
  addSubObjects( anItem );

  // check icon for tree header
  checkVisibleIcon();
}

//=================================================================================
// function : checkVisibleIcon()
// purpose  : set visible or invisible icon in the header of tree
//=================================================================================
void RepairGUI_InspectObjectDlg::checkVisibleIcon()
{
  bool isInvisible = false;
  QTreeWidgetItemIterator it( myTreeObjects );
  while(*it) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
    if( !anItem->isVisible() )
      isInvisible = true;
    ++it;
  }

  if( isInvisible ) {
    myTreeObjects->headerItem()->setIcon( 1, myInvisible );
    myIsSelectAll = false;
  }
  else {
    myTreeObjects->headerItem()->setIcon( 1, myVisible );
    myIsSelectAll = true;
  }
}

//=================================================================================
// function : addSubObjects()
// purpose  : add sub-objects to parent object in the tree
//=================================================================================
void RepairGUI_InspectObjectDlg::addSubObjects( TreeWidgetItem* theParentItem )
{
  TreeWidgetItem* aMainItem = dynamic_cast<TreeWidgetItem*>( myTreeObjects->topLevelItem(0) );
  TopoDS_Iterator it( theParentItem->getShape() );
  for( ; it.More(); it.Next() ) {
    TopoDS_Shape aSubShape = it.Value();
    int anIndex = GEOMBase::GetIndex( aSubShape, aMainItem->getShape() );
    QString anEntry = QString( "TEMP_" ) + aMainItem->getIO()->getEntry() + QString("_%1").arg( anIndex );
    TreeWidgetItem* anItem = new TreeWidgetItem( theParentItem, QStringList(), aSubShape, anEntry);
    anItem->setVisible( false, myInvisible );
    addSubObjects( anItem );
  }
}

//=================================================================================
// function : displayItem()
// purpose  : display sub-object of inspected object according its tree item
//=================================================================================
void RepairGUI_InspectObjectDlg::displayItem( TreeWidgetItem* theItem )
{
  GEOM_Displayer* aDisplayer = getDisplayer();
  if( theItem == myTreeObjects->topLevelItem(0) ) {
    aDisplayer->UnsetColor();
    aDisplayer->UnsetWidth();
  }
  else if( aDisplayer->GetColor() != Quantity_NOC_VIOLET && aDisplayer->GetWidth() != 2.0 ) {
    aDisplayer->SetColor( Quantity_NOC_VIOLET );
    aDisplayer->SetWidth( 2.0 );
  }

  SALOME_Prs* aPrs = aDisplayer->buildSubshapePresentation( theItem->getShape(), theItem->getIO()->getEntry(),
                                                            GEOM_Displayer::GetActiveView() );
  if( aPrs )
    displayPreview( aPrs, true, false );
}

//=================================================================================
// function : setItemDisplayStatus()
// purpose  : set visible or invisible status for the same items in the tree
//=================================================================================
void RepairGUI_InspectObjectDlg::setItemDisplayStatus( TreeWidgetItem* theItem, bool theIsVisible )
{
  QTreeWidgetItemIterator it( myTreeObjects );
  while(*it) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
    if( anItem->getShape().IsSame( theItem->getShape() ) )
      anItem->setVisible( theIsVisible, theIsVisible ? myVisible : myInvisible );
    ++it;
  }
}

//=================================================================================
// function : onEditMainShape()
// purpose  : called when selection button was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::onEditMainShape()
{
  if( myEditMainShape->text().isEmpty() )
    return;

  GEOM_Displayer* aDisplayer = getDisplayer();
  // restore initial parameters for viewer
  aDisplayer->UnsetColor();
  aDisplayer->UnsetWidth();

  // restore transparency of main object
  TreeWidgetItem* aMainItem = dynamic_cast<TreeWidgetItem*>( myTreeObjects->topLevelItem(0) );
  SalomeApp_Study* aStudy = dynamic_cast<SalomeApp_Study*>( myApp->activeStudy() );
  aStudy->setObjectProperty( myGlobalId, QString(aMainItem->getIO()->getEntry()) ,
                             GEOM::propertyName( GEOM::Transparency ), myTransparency );

  if( GEOM_Displayer::GetActiveView()->isVisible( aMainItem->getIO() ) )
    aDisplayer->Redisplay( aMainItem->getIO(), false );

  // erase sub-shapes
  SALOME_ListIO aListOfIO;
  QTreeWidgetItemIterator it( myTreeObjects );
  while(*it) {
    if( *it != aMainItem ) {
      TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
      aListOfIO.Append( anItem->getIO() );
    }
    ++it;
  }
  aDisplayer->Erase( aListOfIO );

  // restore initial parameters for dialog box
  myEditMainShape->setEnabled( true );
  myEditMainShape->setText("");
  myEditMainShape->setFocus();
  myTreeObjects->clear();
  myTreeObjects->update();
}

//=================================================================================
// function : onItemClicked()
// purpose  : called when tree item was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::onItemClicked( QTreeWidgetItem* theItem, int theColumn )
{
  if( theColumn!= 1 || !( theItem->flags() & Qt::ItemIsSelectable ) )
    return;

  GEOM_Displayer* aDisplayer = getDisplayer();

  TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>( theItem );

  if( anItem->isVisible() ) {
    aDisplayer->Erase( anItem->getIO(), false, true );
    setItemDisplayStatus( anItem, false );
  }
  else {
    displayItem( anItem );
    setItemDisplayStatus( anItem, true );
  }
  aDisplayer->UpdateViewer();
  checkVisibleIcon();
}

//=================================================================================
// function : onItemDoubleClicked()
// purpose  : called when tree item was double clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::onItemDoubleClicked( QTreeWidgetItem* theItem, int theColumn )
{
  if( theColumn!= 0 || !( theItem->flags() & Qt::ItemIsSelectable ) )
    return;

  bool bOk;
  QString label = QInputDialog::getText( this, RepairGUI_InspectObjectDlg::tr( "RENAME_COMPONENT" ),
                                         RepairGUI_InspectObjectDlg::tr ( "COMPONENT_NAME" ), QLineEdit::Normal,
                                         theItem->text(0), &bOk );
  if ( bOk && !label.isEmpty() )
    theItem->setText( 0, label );
}

//=================================================================================
// function : onItemExpanded()
// purpose  : called when tree item was expanded
//=================================================================================
void RepairGUI_InspectObjectDlg::onItemExpanded( QTreeWidgetItem* theItem )
{
  TreeWidgetItem* aMainItem = dynamic_cast<TreeWidgetItem*>( myTreeObjects->topLevelItem(0) );
  GEOM::GEOM_Object_var aMainObject = GEOMBase::ConvertIOinGEOMObject( aMainItem->getIO() );

  for( int i = 0; i < theItem->childCount(); i++ ) {
    TreeWidgetItem* aSubItem = dynamic_cast<TreeWidgetItem*>( theItem->child(i) );
    int anIndex = GEOMBase::GetIndex( aSubItem->getShape(), aMainItem->getShape() );
    if( aSubItem->text(0).isEmpty() ) {
      char* aName = aMainObject->GetSubName( anIndex );
      if( !QString( aName ).isEmpty() )
        aSubItem->setText( 0, QString( aName ) );
      else
        aSubItem->setText( 0, GEOMBase::TypeName( aSubItem->getShape().ShapeType() ) +
                                                  QString("_%1").arg( anIndex ) );
    }
  }
}

//=================================================================================
// function : onItemSelectionChanged()
// purpose  : called when tree item was selected
//=================================================================================
void RepairGUI_InspectObjectDlg::onItemSelectionChanged()
{
  QList<QTreeWidgetItem*> listItem = myTreeObjects->selectedItems();
  SALOME_ListIO aSelected;
  for( int i = 0; i < listItem.size(); i++ ) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>( listItem.at(i) );
    aSelected.Append( anItem->getIO() );
  }
  myApp->selectionMgr()->setSelectedObjects( aSelected );
}

//=================================================================================
// function : onHeaderClicked()
// purpose  : called when header item of tree was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::onHeaderClicked( int theColumn )
{
  if( theColumn != 1 )
    return;

  GEOM_Displayer* aDisplayer = getDisplayer();

  if( myIsSelectAll ) {
    myIsSelectAll = false;
    myTreeObjects->headerItem()->setIcon( 1, myInvisible );
    SALOME_ListIO aListOfIO;
    QTreeWidgetItemIterator it( myTreeObjects );
    while(*it) {
      TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
      if( ( anItem->flags() & Qt::ItemIsSelectable ) && anItem->isVisible() ) {
        aListOfIO.Append( anItem->getIO() );
        anItem->setVisible( false, myInvisible );
      }
      ++it;
    }
    aDisplayer->Erase( aListOfIO );
  }
  else {
    myIsSelectAll = true;
    myTreeObjects->headerItem()->setIcon( 1, myVisible );
    QTreeWidgetItemIterator it( myTreeObjects );
    while(*it) {
      TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
      if( ( anItem->flags() & Qt::ItemIsSelectable ) && !anItem->isVisible() ) {
        displayItem( anItem );
        anItem->setVisible( true, myVisible );
      }
      ++it;
    }
  }

  aDisplayer->UpdateViewer();
}

//=================================================================================
// function : onViewSelectionChanged()
// purpose  : called when selection of object browser was changed
//=================================================================================
void RepairGUI_InspectObjectDlg::onViewSelectionChanged()
{
  if( myEditMainShape->isEnabled() )
    init();
}

//=================================================================================
// function : clickOnShow()
// purpose  : called when "Show selected" button was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::clickOnShow()
{
  QList<QTreeWidgetItem*> listItem = myTreeObjects->selectedItems();
  for( int i = 0; i < listItem.size(); i++ ) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>( listItem.at(i) );
    if( !anItem->isVisible() ) {
      displayItem( anItem );
      setItemDisplayStatus( anItem, true );
    }
  }
  getDisplayer()->UpdateViewer();
  checkVisibleIcon();
}

//=================================================================================
// function : clickOnShowOnly()
// purpose  : called when "Show only selected" button was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::clickOnShowOnly()
{
  SALOME_ListIO aListOfIO;
  QTreeWidgetItemIterator it( myTreeObjects );
  while(*it) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>(*it);
    if( ( anItem->flags() & Qt::ItemIsSelectable ) && anItem->isVisible() ) {
      aListOfIO.Append( anItem->getIO() );
      anItem->setVisible( false, myInvisible );
    }
    ++it;
  }
  getDisplayer()->Erase( aListOfIO );

  clickOnShow();
}

//=================================================================================
// function : clickOnHide()
// purpose  : called when "Hide selected" button was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::clickOnHide()
{
  QList<QTreeWidgetItem*> listItem = myTreeObjects->selectedItems();
  for( int i = 0; i < listItem.size(); i++ ) {
    TreeWidgetItem* anItem = dynamic_cast<TreeWidgetItem*>( listItem.at(i) );
    if( anItem->isVisible() ) {
      getDisplayer()->Erase( anItem->getIO(), false, true );
      setItemDisplayStatus( anItem, false );
    }
  }
  getDisplayer()->UpdateViewer();
  checkVisibleIcon();
}

//=================================================================================
// function : clickOnPublish()
// purpose  : called when "Publish selected" button was clicked
//=================================================================================
void RepairGUI_InspectObjectDlg::clickOnPublish()
{
}

//=================================================================================
// function : clickOnHelp()
// purpose  : called when Help button was clicked to open a help page
//=================================================================================
void RepairGUI_InspectObjectDlg::clickOnHelp()
{
  myApp->onHelpContextModule( "GEOM", "inspect_object_page.html" );
}
