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

#include "KinematicGUI_ContactDlg.h"

using namespace std;

//=================================================================================
// class    : KinematicGUI_ContactDlg()
// purpose  : Constructs a KinematicGUI_ContactDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_ContactDlg::KinematicGUI_ContactDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
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

  GroupBox1 = new QGroupBox( this, "GroupBox1" );
  GroupBox1->setTitle( trUtf8( "" ) );
  GroupBox1->setColumnLayout(0, Qt::Vertical );
  GroupBox1->layout()->setSpacing( 6 );
  GroupBox1->layout()->setMargin( 11 );
  GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
  GroupBox1Layout->setAlignment( Qt::AlignTop );

  LayoutA = new QGridLayout( 0, 1, 1, 0, 6, "LayoutA"); 

  LineEdit3 = new QLineEdit( GroupBox1, "LineEdit3" );

  LayoutA->addWidget( LineEdit3, 3, 2 );

  TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );
  TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel4->sizePolicy().hasHeightForWidth() ) );
  TextLabel4->setText( trUtf8( "TL4" ) );

  LayoutA->addWidget( TextLabel4, 3, 0 );

  LayoutB = new QGridLayout( 0, 1, 1, 0, 6, "LayoutB"); 

  TextLabel5 = new QLabel( GroupBox1, "TextLabel5" );
  TextLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel5->sizePolicy().hasHeightForWidth() ) );
  TextLabel5->setText( trUtf8( "TL5" ) );
  
  LayoutB->addWidget( TextLabel5, 0, 0 );

  SpinBox_DX = new DlgRef_SpinBox(GroupBox1, "SpinBox_DX");
  LayoutB->addWidget( SpinBox_DX, 0, 1 );

  LayoutA->addMultiCellLayout( LayoutB, 4, 4, 0, 2 );

  PushButton2 = new QPushButton( GroupBox1, "PushButton2" );
  PushButton2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton2->sizePolicy().hasHeightForWidth() ) );
  PushButton2->setText( trUtf8( "" ) );
  
  LayoutA->addWidget( PushButton2, 2, 1 );
  
  LineEdit1 = new QLineEdit( GroupBox1, "LineEdit1" );

  LayoutA->addWidget( LineEdit1, 0, 2 );

  PushButton1 = new QPushButton( GroupBox1, "PushButton1" );
  PushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton1->sizePolicy().hasHeightForWidth() ) );
  PushButton1->setText( trUtf8( "" ) );

  LayoutA->addWidget( PushButton1, 0, 1 );

  TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
  TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
  TextLabel1->setText( trUtf8( "TL1" ) );

  LayoutA->addWidget( TextLabel1, 0, 0 );

  LineEdit2 = new QLineEdit( GroupBox1, "LineEdit2" );

  LayoutA->addWidget( LineEdit2, 2, 2 );

  TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
  TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel3->sizePolicy().hasHeightForWidth() ) );
  TextLabel3->setText( trUtf8( "TL3" ) );

  LayoutA->addWidget( TextLabel3, 2, 0 );

  LayoutB = new QGridLayout( 0, 1, 1, 0, 6, "LayoutB"); 

  TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
  TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel2->sizePolicy().hasHeightForWidth() ) );
  TextLabel2->setText( trUtf8( "TL2" ) );

  LayoutB->addWidget( TextLabel2, 0, 0 );

  ComboBox1 = new QComboBox( FALSE, GroupBox1, "ComboBox1" );
  ComboBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, ComboBox1->sizePolicy().hasHeightForWidth() ) );

  LayoutB->addWidget( ComboBox1, 0, 1 );

  LayoutA->addMultiCellLayout( LayoutB, 1, 1, 0, 2 );
  QSpacerItem* spacer = new QSpacerItem( 0, 90, QSizePolicy::Minimum, QSizePolicy::Expanding );
  LayoutA->addItem( spacer, 5, 2 );

  PushButton3 = new QPushButton( GroupBox1, "PushButton3" );
  PushButton3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton3->sizePolicy().hasHeightForWidth() ) );
  PushButton3->setText( trUtf8( "" ) );

  LayoutA->addWidget( PushButton3, 3, 1 );
  
  GroupBox1Layout->addLayout( LayoutA, 0, 0 );
    
  GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  TextLabel1->setText(tr("GEOM_ASSEMBLY"));
  TextLabel2->setText(tr("GEOM_TYPE"));
  TextLabel3->setText(tr("GEOM_OBJECT_I").arg("1"));
  TextLabel4->setText(tr("GEOM_OBJECT_I").arg("2"));
  TextLabel5->setText(tr("GEOM_STEP"));
  PushButton1->setPixmap(image1);
  PushButton2->setPixmap(image1);
  PushButton3->setPixmap(image1);

  Layout1->addWidget(GroupBox1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
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
  myEditCurrentArgument = LineEdit1;
  myOkAssembly = myOkShape1 = myOkShape2 = false;
  myType = 0;
  myStep = 0.0;

  /* min, max, step and decimals for spin boxes & initial values */
  SpinBox_DX->RangeStepAndValidator(0.0, 999.999, 0.1, 3);
  SpinBox_DX->SetValue(myStep);
  SpinBox_DX->setEnabled(false);

  /* type for sub shape selection */
  ComboBox1->insertItem("Embedding");
  ComboBox1->insertItem("Pivot");
  ComboBox1->insertItem("Slide");
  ComboBox1->insertItem("Sliding Pivot");
  ComboBox1->insertItem("Spherical");
  ComboBox1->insertItem("Plane");
  ComboBox1->insertItem("Annular");
  ComboBox1->insertItem("Rectilinear");
  ComboBox1->insertItem("Ponctual");
  ComboBox1->insertItem("Helicoidal");

  ComboBox1->setCurrentItem(myType);

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(LineEdit2, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));
  connect(LineEdit3, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(PushButton2, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(PushButton3, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

  connect(SpinBox_DX, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox_DX, SLOT(SetStep(double)));

  connect(ComboBox1, SIGNAL(activated(int)), this, SLOT(ComboTextChanged()));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  this->SelectionIntoArgument();
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
  buttonApply->setFocus();
  QAD_Application::getDesktop()->putInfo(tr(""));

  if(myOkAssembly && myOkShape1 && myOkShape2)
    myKinematicGUI->AddContact(myGeomAssembly, myGeomShape1, myGeomShape2, myType, myStep);

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
    if(myEditCurrentArgument == LineEdit1)
      myOkAssembly = false;
    else if( myEditCurrentArgument == LineEdit2)
      myOkShape1 = false;
    else if( myEditCurrentArgument == LineEdit3)
      myOkShape2 = false;
    return;
  }

  /*  nbSel == 1  */
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();

  if(myEditCurrentArgument == LineEdit1) {
    myGeomAssembly = myGeomBase->ConvertIOinAssembly(IO, testResult);
    if(!testResult)
      return;
    LineEdit1->setText(aString);
    myOkAssembly = true;
  }
  else {
    TopoDS_Shape S;
    if(!myGeomBase->GetTopoFromSelection(mySelection, S))
      return;

    if(myEditCurrentArgument == LineEdit2) {
      myGeomShape1 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
      if(!testResult)
	return;
      LineEdit2->setText(aString);
      myOkShape1 = true;
    }
    else if(myEditCurrentArgument == LineEdit3) {
      myGeomShape2 = myGeomBase->ConvertIOinGEOMShape(IO, testResult);
      if(!testResult)
	return;
      LineEdit3->setText(aString);
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

  if(send == PushButton1) {
    LineEdit1->setFocus();
    myEditCurrentArgument = LineEdit1;
  }
  else if(send == PushButton2) {
    LineEdit2->setFocus();
    myEditCurrentArgument = LineEdit2;
  }
  else if(send == PushButton3) {
    LineEdit3->setFocus();
    myEditCurrentArgument = LineEdit3;
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
  if(send == LineEdit1)
    myEditCurrentArgument = LineEdit1;
  else if(send == LineEdit2)
    myEditCurrentArgument = LineEdit2;
  else if(send == LineEdit3)
    myEditCurrentArgument = LineEdit3;
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
  myType = ComboBox1->currentItem();
  if(myType == 9) {
    SpinBox_DX->SetValue(1.0);
    SpinBox_DX->setEnabled(true);
  }
  else {
    SpinBox_DX->SetValue(0.0);
    SpinBox_DX->setEnabled(false);
  }

  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_ContactDlg::ValueChangedInSpinBox(double newValue)
{
  myStep = newValue;
  return;
}
