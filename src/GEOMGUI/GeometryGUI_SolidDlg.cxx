//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : GeometryGUI_SolidDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "GeometryGUI_SolidDlg.h"

#include "GeometryGUI.h"
#include "QAD_Desktop.h"

//=================================================================================
// class    : GeometryGUI_SolidDlg()
// purpose  : Constructs a GeometryGUI_SolidDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
GeometryGUI_SolidDlg::GeometryGUI_SolidDlg(QWidget* parent, const char* name, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GeometryGUI_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM", tr("ICON_DLG_BUILD_SOLID")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM", tr("ICON_SELECT")));

  setCaption(tr("GEOM_SOLID_TITLE"));
    
  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_SOLID"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  GroupSolid = new GeometryGUI_1Sel_QTD(this, "GroupSolid");
  GroupSolid->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  GroupSolid->TextLabel1->setText(tr("GEOM_OBJECTS"));
  GroupSolid->PushButton1->setPixmap(image1);
  
  Layout1->addWidget(GroupSolid, 1, 0);
  /***************************************************************/

  /* Initialisations */
  Init(Sel);
}


//=================================================================================
// function : ~GeometryGUI_SolidDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
GeometryGUI_SolidDlg::~GeometryGUI_SolidDlg()
{
    // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::Init(SALOME_Selection* Sel)
{
  /* init variables */
  myEditCurrentArgument = GroupSolid->LineEdit1;
  myOkListShapes = false;

  myShellFilter = new GEOM_ShapeTypeFilter(TopAbs_SHELL, myGeom);
  /* filter for next selection */
  mySelection->AddFilter(myShellFilter);

  /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(GroupSolid->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
 
  /* displays Dialog */
  GroupSolid->show();
  this->show();

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return ;
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::ClickOnApply()
{
  myGeomGUI->GetDesktop()->putInfo(tr("")); 

  if(myOkListShapes)  
    myGeomGUI->MakeSolidAndDisplay(myListShapes);

  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void GeometryGUI_SolidDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");
  QString aString = "";

  myOkListShapes = false;
  int nbSel = mySelection->IObjectCount();
  if (nbSel == 0) 
    return;

  aString = tr("%1_objects").arg(nbSel);
  
  myGeomGUI->ConvertListOfIOInListOfIOR(mySelection->StoredIObjects(), myListShapes);
  myEditCurrentArgument->setText(aString);
  myOkListShapes = true;

  return ;
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();
  mySelection->ClearFilters() ;

  if(send == GroupSolid->PushButton1) {
    GroupSolid->LineEdit1->setFocus();
    myEditCurrentArgument = GroupSolid->LineEdit1;
    mySelection->AddFilter(myShellFilter);
  }
  SelectionIntoArgument();

  return ;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::ActivateThisDialog()
{
  GeometryGUI_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  return ;
}


//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void GeometryGUI_SolidDlg::enterEvent(QEvent* e)
{
  if (GroupConstructors->isEnabled())
    return;  
  this->ActivateThisDialog();
  return;
}
