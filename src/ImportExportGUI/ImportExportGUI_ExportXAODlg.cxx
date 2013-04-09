//  Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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
#include "ImportExportGUI_ExportXAODlg.h"
//#include "ImportExportGUI_Widgets.h"

//=================================================================================
// Constructor
//=================================================================================
ImportExportGUI_ExportXAODlg::ImportExportGUI_ExportXAODlg(GeometryGUI* geometryGUI, QWidget* parent)
:
        GEOMBase_Skeleton(geometryGUI, parent, false)
{
    m_mainObj = GEOM::GEOM_Object::_nil();

    SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
    QPixmap imageOp(resMgr->loadPixmap("GEOM", tr("ICON_DLG_EXPORTXAO_EXPORTINGSHAPE_FILENAME_LGROUPS_LFIELDS")));
    QPixmap iconSelect(resMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

    setWindowTitle(tr("GEOM_EXPORTXAO_TITLE"));

    /***************************************************************/
    mainFrame()->GroupConstructors->setTitle(tr("GEOM_EXPORTXAO_TITLE"));
    mainFrame()->RadioButton1->setIcon(imageOp);
    mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
    mainFrame()->RadioButton2->close();
    mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
    mainFrame()->RadioButton3->close();

    QGroupBox* gbxExport = new QGroupBox(parent);

    QGridLayout *gridLayoutExport = new QGridLayout(gbxExport);
#ifndef Q_OS_MAC
    gridLayoutExport->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    gridLayoutExport->setContentsMargins(9, 9, 9, 9);
#endif
    gridLayoutExport->setObjectName(QString::fromUtf8("gridLayoutExport"));

    //****************************
    QLabel* txtLabel1 = new QLabel(tr("GEOM_EXPORTXAO_EXPORTINGSHAPE"), gbxExport);
    gridLayoutExport->addWidget(txtLabel1, 0, 0, 1, 1);

    btnSelect = new QPushButton(gbxExport);
    btnSelect->setIcon(iconSelect);
    gridLayoutExport->addWidget(btnSelect, 0, 1, 1, 1);

    ledShape = new QLineEdit(gbxExport);
    ledShape->setMinimumSize(QSize(100, 0));
    gridLayoutExport->addWidget(ledShape, 0, 2, 1, 1);

    //****************************
    QLabel* txtLabel2 = new QLabel(tr("GEOM_EXPORTXAO_FILENAME"), gbxExport);
    gridLayoutExport->addWidget(txtLabel2, 1, 0, 1, 1);

    ledFileName = new QLineEdit(gbxExport);
    gridLayoutExport->addWidget(ledFileName, 1, 1, 1, 2);

    //****************************
    QLabel* txtLabel3 = new QLabel(tr("GEOM_EXPORTXAO_LGROUPS"), gbxExport);
    gridLayoutExport->addWidget(txtLabel3, 2, 0, 1, 2);

    lstGroups = new QListWidget(gbxExport);
    gridLayoutExport->addWidget(lstGroups, 3, 0, 1, 2);

    QLabel* txtLabel4 = new QLabel(tr("GEOM_EXPORTXAO_LFIELDS"), gbxExport);
    gridLayoutExport->addWidget(txtLabel4, 2, 2, 1, 1);

    lstFields = new QListWidget(gbxExport);
    gridLayoutExport->addWidget(lstFields, 3, 2, 1, 1);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget());
    layout->setMargin(0);
    layout->setSpacing(6);
    layout->addWidget(gbxExport);

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
    m_groups.clear();
    m_fields.clear();

    // Signal/slot connections
    connect(buttonOk(), SIGNAL(clicked()), this, SLOT(ClickOnOk()));
    connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

    connect(btnSelect, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
    connect(((SalomeApp_Application*) (SUIT_Session::session()->activeApplication()))->selectionMgr(),
            SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

    initName(tr("GEOM_EXPORTXAO"));
    SelectionIntoArgument();
}

//=================================================================================
// function : processObject()
// purpose  : Fill dialog fields in accordance with myObj
//=================================================================================
void ImportExportGUI_ExportXAODlg::processObject()
{
    if (m_mainObj->_is_nil())
    {
        ledShape->setText("");
        ledFileName->setText("");
    }
    else
    {
        ledShape->setText(GEOMBase::GetName(m_mainObj));
        GEOM::GEOM_IShapesOperations_var aShOp = getGeomEngine()->GetIShapesOperations(getStudyId());
        GEOM::ListOfGO_var Groups = aShOp->GetExistingSubObjects(m_mainObj, true);
        // Affichage des noms des groupes
        for (int i = 0, n = Groups->length(); i < n; i++)
        {
            lstGroups->addItem(GEOMBase::GetName(Groups[i]));
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
    m_mainObj = GEOM::GEOM_Object::_nil();

    LightApp_SelectionMgr* selMgr = myGeomGUI->getApp()->selectionMgr();
    SALOME_ListIO selList;
    selMgr->selectedObjects(selList);

    if (selList.Extent() != 1)
    {
        processObject();
        return;
    }

    /*GEOM::GEOM_Object_var aSelectedObject*/m_mainObj = GEOMBase::ConvertIOinGEOMObject(selList.First());

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
    ledShape->setFocus();
    myEditCurrentArgument = ledShape;
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
void ImportExportGUI_ExportXAODlg::enterEvent(QEvent*)
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
bool ImportExportGUI_ExportXAODlg::isValid(QString& msg)
{
    bool ok = true;

    //@@ add custom validation actions here @@//

    return ok;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool ImportExportGUI_ExportXAODlg::execute(ObjectList& objects)
{
    bool res = false;

    GEOM::GEOM_Object_var obj;

    GEOM::GEOM_IImportExportOperations_var ieOp = GEOM::GEOM_IImportExportOperations::_narrow(getOperation());
    GEOM::GEOM_IShapesOperations_var shapesOp = getGeomEngine()->GetIShapesOperations(getStudyId());
    GEOM::ListOfGO_var groups = shapesOp->GetExistingSubObjects(m_mainObj, true);
    GEOM::ListOfGO_var fields = new GEOM::ListOfGO();

    std::cout << " == Groups" << std::endl;
    groups->length(m_groups.count());
    for (int i = 0; i < m_groups.count(); i++)
        groups[i] = m_groups[i].copy();

    std::cout << " == Fields" << std::endl;
    fields->length(m_fields.count());
    for (int i = 0; i < m_fields.count(); i++)
        fields[i] = m_fields[i].copy();

    // call engine function
    std::cout << " >> ExportXAO" << std::endl;
    res = ieOp->ExportXAO(m_mainObj, ledFileName->text().toStdString().c_str(), groups, fields);
    std::cout << " << ExportXAO" << std::endl;

    return res;
}
