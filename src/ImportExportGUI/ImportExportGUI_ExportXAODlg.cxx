//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com

#include "ImportExportGUI_ExportXAODlg.h"
//#include "ImportExportGUI_Widgets.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <LightApp_SelectionMgr.h>

#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QListWidget>
#include <QMap>
//#include <ui_ImportExportGUI_1Sel1LineEdit2ListWidget_QTD.h>

// OCCT Includes
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include <GEOMImpl_Types.hxx>

//=================================================================================
// Constructor
//=================================================================================
ImportExportGUI_ExportXAODlg::ImportExportGUI_ExportXAODlg (GeometryGUI* theGeometryGUI, QWidget* parent)
  : GEOMBase_Skeleton(theGeometryGUI, parent, false)
{
  myMainObj = GEOM::GEOM_Object::_nil();
  
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  QPixmap imageOp  (resMgr->loadPixmap("GEOM", tr("ICON_DLG_EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS")));
  QPixmap iconSelect (resMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

  setWindowTitle(tr("GEOM_EXPORTXAO_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_EXPORTXAO_TITLE" ) );
  mainFrame()->RadioButton1->setIcon( imageOp );
  mainFrame()->RadioButton2->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();
  
 
  QGroupBox *GroupBoxExport = new QGroupBox(parent);
  
  QGridLayout *gridLayoutExport = new QGridLayout(GroupBoxExport);
#ifndef Q_OS_MAC
  gridLayoutExport->setSpacing(6);
#endif
#ifndef Q_OS_MAC
  gridLayoutExport->setContentsMargins(9, 9, 9, 9);
#endif
  gridLayoutExport->setObjectName(QString::fromUtf8("gridLayoutExport"));

  //****************************
  QLabel *TextLabel1 = new QLabel(tr( "GEOM_EXPORTXAO_EXPORTINGSHAPE" ),GroupBoxExport);
  gridLayoutExport->addWidget(TextLabel1, 0, 0, 1, 1);

  mySelBtn = new QPushButton(GroupBoxExport);
  mySelBtn->setIcon(iconSelect);
  gridLayoutExport->addWidget(mySelBtn, 0, 1, 1, 1);

  myMainShape = new QLineEdit(GroupBoxExport);
  myMainShape->setMinimumSize(QSize(100, 0));
  gridLayoutExport->addWidget(myMainShape, 0, 2, 1, 1);

  //****************************
  QLabel *TextLabel2 = new QLabel(tr( "GEOM_EXPORTXAO_FILENAME" ),GroupBoxExport);
  gridLayoutExport->addWidget(TextLabel2, 1, 0, 1, 1);

  myMainFile = new QLineEdit(GroupBoxExport);
  gridLayoutExport->addWidget(myMainFile, 1, 1, 1, 2);

  //****************************
  QLabel *TextLabel3 = new QLabel(tr( "GEOM_EXPORTXAO_LGROUPS" ),GroupBoxExport);
  gridLayoutExport->addWidget(TextLabel3, 2, 0, 1, 2);
  
  myListGroups = new QListWidget(GroupBoxExport);
  gridLayoutExport->addWidget(myListGroups, 3, 0, 1, 2);

  QLabel *TextLabel4 = new QLabel(tr( "GEOM_EXPORTXAO_LFIELDS" ),GroupBoxExport);
  gridLayoutExport->addWidget(TextLabel4, 2, 2, 1, 1);

  myListFields = new QListWidget(GroupBoxExport);
  gridLayoutExport->addWidget(myListFields, 3, 2, 1, 1);


  QVBoxLayout* layout = new QVBoxLayout(centralWidget());
  layout->setMargin(0); layout->setSpacing(6);
  layout->addWidget(GroupBoxExport);

        //QWidget::setTabOrder(PushButton1, LineEdit1);
        //QWidget::setTabOrder(LineEdit1, LineEdit2);
  
  setHelpFileName("create_exportxao_page.html");

  Init();
}

//=================================================================================
// Destructor
//=================================================================================
ImportExportGUI_ExportXAODlg::~ImportExportGUI_ExportXAODlg()
{
  // no need to delete child widgets, Qt does it all for us
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::Init()
{
  // Get setting of step value from file configuration
  myGroups.clear();
  myFields.clear();

  // Signal/slot connections
  connect(buttonOk(),    SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect( mySelBtn, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
  connect( ( (SalomeApp_Application*)( SUIT_Session::session()->activeApplication() ) )->selectionMgr(),
           SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  initName(tr("GEOM_EXPORTXAO"));
  SelectionIntoArgument();
}

//=================================================================================
// function : processObject()
// purpose  : Fill dialog fields in accordance with myObj
//=================================================================================
void ImportExportGUI_ExportXAODlg::processObject()
{
  if ( myMainObj->_is_nil() ) {
    myMainShape->setText( "" );
    myMainFile->setText( "" );
  }
  else {   
    myMainShape->setText( GEOMBase::GetName( myMainObj ) );
    GEOM::GEOM_IShapesOperations_var aShOp = getGeomEngine()->GetIShapesOperations(getStudyId());
    GEOM::ListOfGO_var Groups = aShOp->GetExistingSubObjects(myMainObj, true);
    // Affichage des noms des groupes
    for ( int i = 0, n = Groups->length(); i < n; i++ )
    {
      myListGroups->addItem( GEOMBase::GetName( Groups[i] ) );
    }
  }
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::ClickOnOk()
{
  if (ClickOnApply())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool ImportExportGUI_ExportXAODlg::ClickOnApply()
{
  if (!onAccept())
    return false;

  initName();

  return true;
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void ImportExportGUI_ExportXAODlg::SelectionIntoArgument()
{
  myMainObj = GEOM::GEOM_Object::_nil();

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);

  if (aSelList.Extent() != 1) {
    processObject();
    return;
  }

  /*GEOM::GEOM_Object_var aSelectedObject*/myMainObj = GEOMBase::ConvertIOinGEOMObject( aSelList.First() );

  /*if ( aSelectedObject->_is_nil() ) {
    processObject();
    return;
  }*/

  //myMainObj = aSelectedObject;
  
  processObject();
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::SetEditCurrentArgument()
{
  myMainShape->setFocus();
  myEditCurrentArgument = myMainShape;
  SelectionIntoArgument();
}
//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
}

//=================================================================================
// function : enterEvent [REDEFINED]
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::enterEvent (QEvent*)
{
  if (!mainFrame()->GroupConstructors->isEnabled())
    ActivateThisDialog();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr ImportExportGUI_ExportXAODlg::createOperation()
{
  return getGeomEngine()->GetIImportExportOperations(getStudyId());
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool ImportExportGUI_ExportXAODlg::isValid (QString& msg)
{
  bool ok = true;

  //@@ add custom validation actions here @@//

  return ok;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool ImportExportGUI_ExportXAODlg::execute (ObjectList& objects)
{
  bool res = false;

  GEOM::GEOM_Object_var anObj;

  GEOM::GEOM_IImportExportOperations_var anOper = GEOM::GEOM_IImportExportOperations::_narrow(getOperation());
  GEOM::GEOM_IShapesOperations_var anShapesOper = getGeomEngine()->GetIShapesOperations(getStudyId());
  GEOM::ListOfGO_var mylGroups = anShapesOper->GetExistingSubObjects(myMainObj, true);
  GEOM::ListOfGO_var mylFields = new GEOM::ListOfGO();

  mylGroups->length( myGroups.count() );
  for ( int i = 0; i < myGroups.count(); i++ )
    mylGroups[i] = myGroups[i].copy();
    
  mylFields->length( myFields.count() );
  for ( int i = 0; i < myFields.count(); i++ )
    mylFields[i] = myFields[i].copy();

  // call engine function
  res = anOper->ExportXAO(myMainObj, myMainFile->text().toStdString().c_str(), mylGroups, mylFields);

  return res;
}
