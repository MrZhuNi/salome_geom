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
//  File   : KinematicGUI_AnimValuesDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "KinematicGUI_AnimValuesDlg.h"

using namespace std;

//=================================================================================
// class    : KinematicGUI_AnimValuesDlg()
// purpose  : Constructs a KinematicGUI_AnimValuesDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_AnimValuesDlg::KinematicGUI_AnimValuesDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, bool modal, WFlags fl)
  :GEOMBase_Skeleton(parent, name, Sel, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIMATION")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_ANIMVALUES_TITLE"));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_ANIMVALUES"));
  RadioButton1->setPixmap(image0);
  RadioButton2->close(TRUE);
  RadioButton3->close(TRUE);

  GroupBox1 = new QGroupBox( this, "GroupBox1" );
  GroupBox1->setColumnLayout(0, Qt::Vertical );
  GroupBox1->layout()->setSpacing( 6 );
  GroupBox1->layout()->setMargin( 11 );
  GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
  GroupBox1Layout->setAlignment( Qt::AlignTop );

  LayoutD = new QGridLayout( 0, 1, 1, 0, 6, "LayoutD"); 
  QSpacerItem* spacer = new QSpacerItem( 0, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
  LayoutD->addItem( spacer, 2, 1 );

  LayoutC = new QGridLayout( 0, 1, 1, 0, 6, "LayoutC"); 

  TextLabel8 = new QLabel( GroupBox1, "TextLabel8" );
  TextLabel8->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel8->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel8, 5, 0 );

  TextLabel9 = new QLabel( GroupBox1, "TextLabel9" );
  TextLabel9->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel9->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel9, 6, 0 );

  TextLabel4 = new QLabel( GroupBox1, "TextLabel4" );
  TextLabel4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel4->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel4, 1, 0 );

  TextLabel5 = new QLabel( GroupBox1, "TextLabel5" );
  TextLabel5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel5->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel5, 2, 0 );

  TextLabel3 = new QLabel( GroupBox1, "TextLabel3" );
  TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, TextLabel3->sizePolicy().hasHeightForWidth() ) );
  TextLabel3->setAlignment( int( QLabel::AlignCenter ) );
  LayoutC->addWidget( TextLabel3, 0, 2 );

  TextLabel2 = new QLabel( GroupBox1, "TextLabel2" );
  TextLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, TextLabel2->sizePolicy().hasHeightForWidth() ) );
  TextLabel2->setAlignment( int( QLabel::AlignCenter ) );
  LayoutC->addWidget( TextLabel2, 0, 1 );

  TextLabel6 = new QLabel( GroupBox1, "TextLabel6" );
  TextLabel6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel6->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel6, 3, 0 );

  TextLabel10 = new QLabel( GroupBox1, "TextLabel10" );
  TextLabel10->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel10->sizePolicy().hasHeightForWidth() ) );
  QFont TextLabel10_font(  TextLabel10->font() );
  TextLabel10_font.setBold( TRUE );
  TextLabel10->setFont( TextLabel10_font ); 
  LayoutC->addWidget( TextLabel10, 0, 0 );

  TextLabel7 = new QLabel( GroupBox1, "TextLabel7" );
  TextLabel7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel7->sizePolicy().hasHeightForWidth() ) );
  LayoutC->addWidget( TextLabel7, 4, 0 );

  SpinBox1 = new DlgRef_SpinBox(GroupBox1, "SpinBox1");
  LayoutC->addWidget( SpinBox1, 1, 1 );

  SpinBox2 = new DlgRef_SpinBox(GroupBox1, "SpinBox2");
  LayoutC->addWidget( SpinBox2, 1, 2 );

  SpinBox3 = new DlgRef_SpinBox(GroupBox1, "SpinBox3");
  LayoutC->addWidget( SpinBox3, 2, 1 );

  SpinBox4 = new DlgRef_SpinBox(GroupBox1, "SpinBox4");
  LayoutC->addWidget( SpinBox4, 2, 2 );

  SpinBox5 = new DlgRef_SpinBox(GroupBox1, "SpinBox5");
  LayoutC->addWidget( SpinBox5, 3, 1 );

  SpinBox6 = new DlgRef_SpinBox(GroupBox1, "SpinBox6");
  LayoutC->addWidget( SpinBox6, 3, 2 );

  SpinBox7 = new DlgRef_SpinBox(GroupBox1, "SpinBox7");
  LayoutC->addWidget( SpinBox7, 4, 1 );

  SpinBox8 = new DlgRef_SpinBox(GroupBox1, "SpinBox8");
  LayoutC->addWidget( SpinBox8, 4, 2 );

  SpinBox9 = new DlgRef_SpinBox(GroupBox1, "SpinBox9");
  LayoutC->addWidget( SpinBox9, 5, 1 );

  SpinBox10 = new DlgRef_SpinBox(GroupBox1, "SpinBox10");
  LayoutC->addWidget( SpinBox10, 5, 2 );

  SpinBox11 = new DlgRef_SpinBox(GroupBox1, "SpinBox11");
  LayoutC->addWidget( SpinBox11, 6, 1 );

  SpinBox12 = new DlgRef_SpinBox(GroupBox1, "SpinBox12");
  LayoutC->addWidget( SpinBox12, 6, 2 );

  LayoutD->addLayout( LayoutC, 1, 1 );

  ListBox1 = new QListBox( GroupBox1, "ListBox1" );
  ListBox1->insertItem( trUtf8( "New Item" ) );
  ListBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, ListBox1->sizePolicy().hasHeightForWidth() ) );
  LayoutD->addMultiCellWidget( ListBox1, 1, 2, 0, 0 );

  LayoutB = new QGridLayout( 0, 1, 1, 0, 6, "LayoutB"); 
  
  LineEdit1 = new QLineEdit( GroupBox1, "LineEdit1" );
  LayoutB->addWidget( LineEdit1, 0, 2 );

  PushButton1 = new QPushButton( GroupBox1, "PushButton1" );
  PushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton1->sizePolicy().hasHeightForWidth() ) );
  LayoutB->addWidget( PushButton1, 0, 1 );

  TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
  TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
  LayoutB->addWidget( TextLabel1, 0, 0 );

  LayoutD->addMultiCellLayout( LayoutB, 0, 0, 0, 1 );

  GroupBox1Layout->addLayout( LayoutD, 0, 0 );

  GroupBox1->setTitle(tr("GEOM_ARGUMENTS"));
  TextLabel1->setText(tr("GEOM_ANIMATION"));
  TextLabel2->setText(tr("GEOM_BEGIN"));
  TextLabel3->setText(tr("GEOM_END"));
  TextLabel4->setText(tr("GEOM_RX"));
  TextLabel5->setText(tr("GEOM_RY"));
  TextLabel6->setText(tr("GEOM_RZ"));
  TextLabel7->setText(tr("GEOM_TX"));
  TextLabel8->setText(tr("GEOM_TY"));
  TextLabel9->setText(tr("GEOM_TZ"));
  PushButton1->setPixmap(image1);

  Layout1->addWidget(GroupBox1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  resize(0, 250);
  myKinematicGUI = theKinematicGUI;
  Init();
}


//=================================================================================
// function : ~KinematicGUI_AnimValuesDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_AnimValuesDlg::~KinematicGUI_AnimValuesDlg()
{  
  /* no need to delete child widgets, Qt does it all for us */
}



//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = LineEdit1;
  myOkAnimation =  false;

  /* min, max, step and decimals for spin boxes & initial values */
  SpinBox1->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox2->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox3->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox4->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox5->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox6->RangeStepAndValidator(-99999.999, 99999.999, 5.0, 3);
  SpinBox7->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);
  SpinBox8->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);
  SpinBox9->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);
  SpinBox10->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);
  SpinBox11->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);
  SpinBox12->RangeStepAndValidator(-99999.999, 99999.999, 100.0, 3);

  SpinBox1->SetValue(0.0);
  SpinBox2->SetValue(0.0);
  SpinBox3->SetValue(0.0);
  SpinBox4->SetValue(0.0);
  SpinBox5->SetValue(0.0);
  SpinBox6->SetValue(0.0);
  SpinBox7->SetValue(0.0);
  SpinBox8->SetValue(0.0);
  SpinBox9->SetValue(0.0);
  SpinBox10->SetValue(0.0);
  SpinBox11->SetValue(0.0);
  SpinBox12->SetValue(0.0);

   /* signals and slots connections */
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));

  connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(ListBox1, SIGNAL(highlighted(int)), this, SLOT(ClickOnListBox(int)));

  connect(SpinBox1, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox2, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox3, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox4, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox5, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox6, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox7, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox8, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox9, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox10, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox11, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));
  connect(SpinBox12, SIGNAL(valueChanged(double)), this, SLOT(ValueChangedInSpinBox(double)));

  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox1, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox2, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox3, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox4, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox5, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox6, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox7, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox8, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox9, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox10, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox11, SLOT(SetStep(double)));
  connect(myGeomGUI, SIGNAL(SignalDefaultStepValueChanged(double)), SpinBox12, SLOT(SetStep(double)));

  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* displays Dialog */
  this->SelectionIntoArgument();
  this->show();

  return;
}


//=================================================================================
// function : SetEnabledValues()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::SetEnabledValues()
{
  cout<<"KinematicGUI_AnimValuesDlg::SetEnabledValues"<<endl;
  int type = myGeomContact->GetType();

  TCollection_AsciiString aTypeName = myKinematicGUI->GetNameFromType(type);
  TextLabel10->setText(aTypeName.ToCString());
  
  cout<<"KinematicGUI_AnimValuesDlg::SetEnabledValues : 1"<<endl;
  SpinBox1->SetValue(myList[0]);
  SpinBox2->SetValue(myList[1]);
  SpinBox3->SetValue(myList[2]);
  SpinBox4->SetValue(myList[3]);
  SpinBox5->SetValue(myList[4]);
  SpinBox6->SetValue(myList[5]);
  SpinBox7->SetValue(myList[6]);
  SpinBox8->SetValue(myList[7]);
  SpinBox9->SetValue(myList[8]);
  SpinBox10->SetValue(myList[9]);
  SpinBox11->SetValue(myList[10]);
  SpinBox12->SetValue(myList[11]);
  cout<<"KinematicGUI_AnimValuesDlg::SetEnabledValues : 2"<<endl;

  SpinBox1->show();
  SpinBox2->show();
  TextLabel4->show();

  SpinBox3->show();
  SpinBox4->show();
  TextLabel5->show();

  SpinBox5->show();
  SpinBox6->show();
  TextLabel6->show();

  SpinBox7->show();
  SpinBox8->show();
  TextLabel7->show();

  SpinBox9->show();
  SpinBox10->show();
  TextLabel8->show();

  SpinBox11->show();
  SpinBox12->show();
  TextLabel9->show();

  if(type == 0 || type == 2 || type == 5) { // RX
    SpinBox1->hide();
    SpinBox2->hide();
    TextLabel4->hide();
  }
  if(type == 0 || type == 1 || type == 2 || type == 3 || type == 5 || type == 7 || type == 9) { // RY
    SpinBox3->hide();
    SpinBox4->hide();
    TextLabel5->hide();
  }
  if(type == 0 || type == 1 || type == 2 || type == 3 || type == 9) { // RZ
    SpinBox5->hide();
    SpinBox6->hide();
    TextLabel6->hide();
  }
  if(type == 0 || type == 1 || type == 4 || type == 8) { // TX
    SpinBox7->hide();
    SpinBox8->hide();
    TextLabel7->hide();
  }
  if(type == 0 || type == 1 || type == 2 || type == 3 || type == 4 || type == 6 || type == 9) { // TY
    SpinBox9->hide();
    SpinBox10->hide();
    TextLabel8->hide();
  }
  if(type == 0 || type == 1 || type == 2 || type == 3 || type == 4 || type == 5 || type == 6 || type == 7 || type == 9) { // TZ
    SpinBox11->hide();
    SpinBox12->hide();
    TextLabel9->hide();
  }

  return;
}


//=================================================================================
// function : ClickOnListBox()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::ClickOnListBox(int Item)
{
  cout<<"KinematicGUI_AnimValuesDlg::ClickOnListBox"<<endl;
  GEOM::ListOfContact_var aContactList = myGeomAnimation->GetAssembly()->GetContactList();
  myGeomContact = aContactList[Item];
  myOkContact = true;
  myList = myGeomAnimation->GetDisplacement(myGeomContact);
  this->SetEnabledValues();
  return;
}


//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::ClickOnOk()
{
  this->ClickOnApply();
  ClickOnCancel();
  return;
}


//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::ClickOnApply()
{
  buttonApply->setFocus();
  QAD_Application::getDesktop()->putInfo(tr(""));

  if(myOkAnimation && myOkContact)
    myKinematicGUI->SetDisplacement(myGeomAnimation, myGeomContact, myList);

  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void KinematicGUI_AnimValuesDlg::SelectionIntoArgument()
{
  cout<<"KinematicGUI_AnimValuesDlg::SelectionIntoArgument"<<endl;
  myEditCurrentArgument->setText("");
  QString aString = "";
  myOkAnimation = false;
  myOkContact = false;
  ListBox1->clear();
  TextLabel10->setText("");

  SpinBox1->SetValue(0.0);
  SpinBox2->SetValue(0.0);
  SpinBox3->SetValue(0.0);
  SpinBox4->SetValue(0.0);
  SpinBox5->SetValue(0.0);
  SpinBox6->SetValue(0.0);
  SpinBox7->SetValue(0.0);
  SpinBox8->SetValue(0.0);
  SpinBox9->SetValue(0.0);
  SpinBox10->SetValue(0.0);
  SpinBox11->SetValue(0.0);
  SpinBox12->SetValue(0.0);

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1)
    return;

  /*  nbSel == 1  */
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();

  if(myEditCurrentArgument == LineEdit1) {
    myGeomAnimation = myGeomBase->ConvertIOinAnimation(IO, testResult);
    if(!testResult)
      return;
    LineEdit1->setText(aString);
    myOkAnimation = true;

    SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
    GEOM::ListOfContact_var aContactList = myGeomAnimation->GetAssembly()->GetContactList();
    for(int i = 0; i < myGeomAnimation->GetAssembly()->NbContacts(); i++) {
      GEOM::GEOM_Contact_var aContact = aContactList[i];
      SALOMEDS::SObject_var SO = aStudy->FindObjectIOR(aContact->Name());
      ListBox1->insertItem(SO->GetName());
    }
    ListBox1->setSelected(0, true);
  }
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::SetEditCurrentArgument()
{
  QPushButton* send = (QPushButton*)sender();

  if(send == PushButton1) {
    LineEdit1->setFocus();
    myEditCurrentArgument = LineEdit1;
  }
  SelectionIntoArgument();

  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  this->SelectionIntoArgument();
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void KinematicGUI_AnimValuesDlg::enterEvent(QEvent * e)
{
  if (GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void KinematicGUI_AnimValuesDlg::ValueChangedInSpinBox(double newValue)
{
  DlgRef_SpinBox* send = (DlgRef_SpinBox*)sender();

  if(myOkContact == false)
    return;

  int type = myGeomContact->GetType();
  double step = myGeomContact->GetStep();
  GEOM::ListOfDouble_var aAngList = myGeomContact->GetAngularRange();
  GEOM::ListOfDouble_var aLinList = myGeomContact->GetLinearRange();

  int i, j;
  if(send == SpinBox1) {i = 0; j = 0;}
  else if(send == SpinBox2) {i = 0; j = 1;}
  else if(send == SpinBox3) {i = 1; j = 0;}
  else if(send == SpinBox4) {i = 1; j = 1;}
  else if(send == SpinBox5) {i = 2; j = 0;}
  else if(send == SpinBox6) {i = 2; j = 1;}
  else if(send == SpinBox7) {i = 3; j = 0;}
  else if(send == SpinBox8) {i = 3; j = 1;}
  else if(send == SpinBox9) {i = 4; j = 0;}
  else if(send == SpinBox10) {i = 4; j = 1;}
  else if(send == SpinBox11) {i = 5; j = 0;}
  else if(send == SpinBox12) {i = 5; j = 1;}

  if(newValue >= aAngList[2*i] && newValue <= aAngList[2*i+1]) {
    myList[2*i+j] = newValue;
    if(type == 9) {
      if(i == 0 && j == 0)
	SpinBox7->SetValue(newValue * step / 360);
      else if(i == 0 && j == 1)
	SpinBox8->SetValue(newValue * step / 360);
      else if(i == 3 && j == 0)
	SpinBox1->SetValue(newValue / step * 360);
      else if(i == 3 && j == 1)
	SpinBox2->SetValue(newValue / step * 360);
    }
  }
  else
    send->SetValue(myList[2*i+j]);

  return;
}
