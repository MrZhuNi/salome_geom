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
//  File   : KinematicGUI_ContactDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

using namespace std;
#include "KinematicGUI_ContactDlg.h"

//=================================================================================
// class    : KinematicGUI_ContactDlg()
// purpose  : Constructs a KinematicGUI_ContactDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_ContactDlg::KinematicGUI_ContactDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, int type, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_CONTACT")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_CONTACT_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_CONTACT"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  Group1 = new KinematicGUI_3Sel1List_QTD(this, "Group1");
  Group1->GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  Group1->TextLabel1->setText(tr("GEOM_ASSEMBLY"));
  Group1->TextLabel2->setText(tr("GEOM_TYPE"));
  Group1->TextLabel3->setText(tr("GEOM_OBJECT_I").arg("1"));
  Group1->TextLabel4->setText(tr("GEOM_OBJECT_I").arg("2"));
  Group1->PushButton1->setPixmap(image1);
  Group1->PushButton2->setPixmap(image1);
  Group1->PushButton3->setPixmap(image1);

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  myType = type;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_ContactDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_ContactDlg::~KinematicGUI_ContactDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = Group1->LineEdit1;
  myOkAssembly = myOkShape1 = myOkShape2 = false;

  /* type for sub shape selection */
  Group1->ComboBox1->insertItem("Embedding");
  Group1->ComboBox1->insertItem("Pivot");
  Group1->ComboBox1->insertItem("Slide");
  Group1->ComboBox1->insertItem("Sliding Pivot");
  Group1->ComboBox1->insertItem("Spherical");
  Group1->ComboBox1->insertItem("Plane");
  Group1->ComboBox1->insertItem("Annular");
  Group1->ComboBox1->insertItem("Rectilinear");
  Group1->ComboBox1->insertItem("Ponctual");

  Group1->ComboBox1->setCurrentItem(myType);

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(Group1->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group1->LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(Group1->LineEdit3, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group1->PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(Group1->PushButton3, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(Group1->ComboBox1, SIGNAL(activated(int)), this, SLOT(ComboTextChanged()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return ;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::ClickOnApply()
{
  QAD_Application::getDesktop()->putInfo(tr(""));

  if(myOkAssembly && myOkShape1 && myOkShape2)
    myKinematicGUI->AddContact(myGeomAssembly, myGeomShape1, myGeomShape2, myType);

  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void KinematicGUI_ContactDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");
  QString aString = "";

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1) {
    if(myEditCurrentArgument == Group1->LineEdit1)
      myOkAssembly = false;
    else if( myEditCurrentArgument == Group1->LineEdit2)
      myOkShape1 = false;
    else if( myEditCurrentArgument == Group1->LineEdit3)
      myOkShape2 = false;
    return;
  }

  /*  nbSel == 1  */
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();

  if(myEditCurrentArgument == Group1->LineEdit1) {
    myGeomAssembly = myGeomBase->ConvertIOinAssembly(IO, testResult);
    if(!testResult)
      return;
    Group1->LineEdit1->setText(aString);
    myOkAssembly = true;
  }
  else {
    TopoDS_Shape S;
    if(!myGeomBase->GetTopoFromSelection(mySelection, S))
      return;

    if(myEditCurrentArgument == Group1->LineEdit2) {
      myGeomShape1 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
      if(!testResult)
	return;
      Group1->LineEdit2->setText(aString);
      myOkShape1 = true;
    }
    else if(myEditCurrentArgument == Group1->LineEdit3) {
      myGeomShape2 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
      if(!testResult)
	return;
      Group1->LineEdit3->setText(aString);
      myOkShape2 = true;
    }
  }
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if(send == Group1->PushButton1) {
    Group1->LineEdit1->setFocus();
    myEditCurrentArgument = Group1->LineEdit1;
  }
  else if(send == Group1->PushButton2) {
    Group1->LineEdit2->setFocus();
    myEditCurrentArgument = Group1->LineEdit2;
  }
  else if(send == Group1->PushButton3) {
    Group1->LineEdit3->setFocus();
    myEditCurrentArgument = Group1->LineEdit3;
  }
  SelectionIntoArgument();

  return;
}



//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::LineEditReturnPressed()
{
  QLineEdit* send = (QLineEdit*)sender();
  if(send == Group1->LineEdit1)
    myEditCurrentArgument = Group1->LineEdit1;
  else if(send == Group1->LineEdit2)
    myEditCurrentArgument = Group1->LineEdit2;
  else if(send == Group1->LineEdit3)
    myEditCurrentArgument = Group1->LineEdit3;
  else
    return;

  GEOMBase_Skeleton::LineEditReturnPressed();
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void KinematicGUI_ContactDlg::enterEvent(QEvent * e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ComboTextChanged()
// purpose  : 
//=================================================================================
void KinematicGUI_ContactDlg::ComboTextChanged()
{
  myType = Group1->ComboBox1->currentItem();
  return;
}
