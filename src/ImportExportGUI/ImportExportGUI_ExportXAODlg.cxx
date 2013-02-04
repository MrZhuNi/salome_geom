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
#include "ImportExportGUI_Widgets.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
#include <LightApp_SelectionMgr.h>

#include <QLabel>
#include <QLineEdit>
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
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  QPixmap imageOp  (resMgr->loadPixmap("GEOM", tr("ICON_DLG_EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS")));
  QPixmap imageSel (resMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

  setWindowTitle(tr("GEOM_EXPORTXAO_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle( tr( "GEOM_EXPORTXAO_TITLE" ) );
  mainFrame()->RadioButton1->setIcon( imageOp );
  mainFrame()->RadioButton2->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute( Qt::WA_DeleteOnClose );
  mainFrame()->RadioButton3->close();
  
  myGrp = new ImportExportGUI_1Sel1LineEdit2ListWidget( centralWidget() );
  myGrp->GroupBox1->setTitle( tr( "GEOM_EXPORTXAO" ) );
  myGrp->TextLabel1->setText( tr( "GEOM_EXPORTXAO_EXPORTINGSHAPE" ) );
  myGrp->TextLabel2->setText( tr( "GEOM_EXPORTXAO_FILENAME" ) );
  myGrp->TextLabel3->setText( tr( "GEOM_EXPORTXAO_LGROUPS" ) );
  myGrp->TextLabel4->setText( tr( "GEOM_EXPORTXAO_LFIELDS" ) );
  myGrp->PushButton1->setIcon( imageSel );
  
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

  // Signal/slot connections
  connect(buttonOk(),    SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect( myGrp->PushButton1, SIGNAL( clicked() ),       this, SLOT( SetEditCurrentArgument() ) );
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
    myGrp->LineEdit1->setText( "" );
    myGrp->LineEdit2->setText( "" );
    erasePreview();
  }
  else {   
    myGrp->LineEdit1->setText( GEOMBase::GetName( myMainObj ) );

    SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
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
  erasePreview();
  myMainObj = GEOM::GEOM_Object::_nil();

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SALOME_ListIO aSelList;
  aSelMgr->selectedObjects(aSelList);

  if (aSelList.Extent() != 1) {
    processObject();
    return;
  }

  GEOM::GEOM_Object_var aSelectedObject =
    GEOMBase::ConvertIOinGEOMObject( aSelList.First() );

  if ( aSelectedObject->_is_nil() ) {
    processObject();
    return;
  }

  myMainObj = aSelectedObject;
  processObject();
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void ImportExportGUI_ExportXAODlg::SetEditCurrentArgument()
{
  myGrp->LineEdit1->setFocus();
  myEditCurrentArgument = myGrp->LineEdit1;
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

  /*//@@ retrieve input values from the widgets here @@//
  CORBA::Double theExportingShape = @@ init parameter value from dialog box @@;
  CORBA::String_var theFileName = @@ init parameter value from dialog box @@;
  CORBA::Boolean thelGroups = @@ init parameter value from dialog box @@;
  CORBA::Boolean thelFields = @@ init parameter value from dialog box @@;

  // call engine function
  anObj = anOper->ExportXAO(theExportingShape, theFileName, thelGroups, thelFields);
  res = !anObj->_is_nil();
  if (res && !IsPreview())
  {
    QStringList aParameters;
    //@@ put stringified input parameters to the string list here to store in the data model for notebook @@//
    aParameters << @@ stringified parameter value @@; // ExportingShape parameter
    aParameters << @@ stringified parameter value @@; // FileName parameter
    aParameters << @@ stringified parameter value @@; // lGroups parameter
    aParameters << @@ stringified parameter value @@; // lFields parameter
    if ( aParameters.count() > 0 ) anObj->SetParameters(aParameters.join(":").toLatin1().constData());
  }
  
  if (res)
    objects.push_back(anObj._retn());*/

  return res;
}
