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
//  File   : KinematicGUI_AnimDlg.cxx
//  Author : Damien COQUERET
//  Module : GEOM
//  $Header: 

#include "KinematicGUI_AnimDlg.h"

#include <Geom_Transformation.hxx>

#include "QAD_RightFrame.h"
#include "OCCViewer_ViewFrame.h"
#include "QAD_FileDlg.h"
#include "QAD_Tools.h"

using namespace std;

//=================================================================================
// class    : KinematicGUI_AnimDlg()
// purpose  : Constructs a KinematicGUI_AnimDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
KinematicGUI_AnimDlg::KinematicGUI_AnimDlg(QWidget* parent, const char* name, KinematicGUI* theKinematicGUI, SALOME_Selection* Sel, Handle(AIS_InteractiveContext) ic, bool modal, WFlags fl)
  :KinematicGUI_Skeleton_QTD(parent, name, modal, fl)
{
  if (!name)
    setName("KinematicGUI_AnimDlg");

  buttonClose->setText(tr("GEOM_BUT_CLOSE"));
  GroupMedium->close(TRUE);
  resize(0, 0);

  GroupBox1 = new QGroupBox( this, "GroupBox1" );
  GroupBox1->setTitle( trUtf8( "" ) );
  GroupBox1->setColumnLayout(0, Qt::Vertical );
  GroupBox1->layout()->setSpacing( 6 );
  GroupBox1->layout()->setMargin( 11 );
  GroupBox1Layout = new QGridLayout( GroupBox1->layout() );
  GroupBox1Layout->setAlignment( Qt::AlignTop );

  LayoutA = new QGridLayout( 0, 1, 1, 0, 6, "LayoutA"); 

  LayoutD = new QGridLayout( 0, 1, 1, 0, 6, "LayoutD"); 

  PushButton7 = new QPushButton( GroupBox1, "PushButton7" );
  PushButton7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton7->sizePolicy().hasHeightForWidth() ) );
  PushButton7->setText( trUtf8( "" ) );
    
  LayoutD->addWidget( PushButton7, 0, 1 );

  CheckButton1 = new QRadioButton( GroupBox1, "CheckButton1" );
  CheckButton1->setText( trUtf8( "" ) );

  LayoutD->addWidget( CheckButton1, 0, 0 );

  LayoutA->addLayout( LayoutD, 3, 0 );

  Slider1 = new QSlider( GroupBox1, "Slider1" );
  Slider1->setOrientation( QSlider::Horizontal );

  LayoutA->addWidget( Slider1, 1, 0 );

  LayoutB = new QGridLayout( 0, 1, 1, 0, 6, "LayoutB"); 

  TextLabel1 = new QLabel( GroupBox1, "TextLabel1" );
  TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
  TextLabel1->setText( trUtf8( "TL1" ) );

  LayoutB->addWidget( TextLabel1, 0, 0 );

  PushButton1 = new QPushButton( GroupBox1, "PushButton1" );
  PushButton1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton1->sizePolicy().hasHeightForWidth() ) );
  PushButton1->setText( trUtf8( "" ) );

  LayoutB->addWidget( PushButton1, 0, 1 );

  LineEdit1 = new QLineEdit( GroupBox1, "LineEdit1" );

  LayoutB->addWidget( LineEdit1, 0, 2 );

  LayoutA->addLayout( LayoutB, 0, 0 );
  QSpacerItem* spacer = new QSpacerItem( 0, 212, QSizePolicy::Minimum, QSizePolicy::Expanding );
  LayoutA->addItem( spacer, 4, 0 );

  LayoutC = new QGridLayout( 0, 1, 1, 0, 6, "LayoutC"); 

  PushButton4 = new QPushButton( GroupBox1, "PushButton4" );
  PushButton4->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton4->sizePolicy().hasHeightForWidth() ) );
  PushButton4->setText( trUtf8( "" ) );

  LayoutC->addWidget( PushButton4, 0, 2 );

  PushButton6 = new QPushButton( GroupBox1, "PushButton6" );
  PushButton6->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton6->sizePolicy().hasHeightForWidth() ) );
  PushButton6->setText( trUtf8( "" ) );

  LayoutC->addWidget( PushButton6, 0, 4 );

  PushButton2 = new QPushButton( GroupBox1, "PushButton2" );
  PushButton2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton2->sizePolicy().hasHeightForWidth() ) );
  PushButton2->setText( trUtf8( "" ) );

  LayoutC->addWidget( PushButton2, 0, 0 );

  PushButton5 = new QPushButton( GroupBox1, "PushButton5" );
  PushButton5->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton5->sizePolicy().hasHeightForWidth() ) );
  PushButton5->setText( trUtf8( "" ) );

  LayoutC->addWidget( PushButton5, 0, 3 );

  PushButton3 = new QPushButton( GroupBox1, "PushButton3" );
  PushButton3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, PushButton3->sizePolicy().hasHeightForWidth() ) );
  PushButton3->setText( trUtf8( "" ) );

  LayoutC->addWidget( PushButton3, 0, 1 );

  LayoutA->addLayout( LayoutC, 2, 0 );

  GroupBox1Layout->addLayout( LayoutA, 0, 0 );

  /***************************************************************/
  setCaption(tr("GEOM_ANIM_TITLE"));

  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIMATION")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_FIRST")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_PREV")));
  QPixmap image3(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_PLAY")));
  QPixmap image4(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_NEXT")));
  QPixmap image5(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_LAST")));
  QPixmap image6(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_SELECT")));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_ANIM"));
  RadioButton1->setPixmap(image0);
  PushButton1->setPixmap(image6);
  PushButton2->setPixmap(image1);
  PushButton3->setPixmap(image2);
  PushButton4->setPixmap(image3);
  PushButton5->setPixmap(image4);
  PushButton6->setPixmap(image5);
  TextLabel1->setText(tr("GEOM_OBJECT"));
  PushButton7->setText(tr("GEOM_ANIM_EXPORT"));
  CheckButton1->setText(tr("GEOM_ANIM_SHADING"));

  Layout1->addWidget(GroupBox1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  mySelection = Sel;
  myIC = ic;
  QApplication::setOverrideCursor(Qt::waitCursor);
  Init();
  QApplication::restoreOverrideCursor();
}


//=================================================================================
// function : ~KinematicGUI_AnimDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_AnimDlg::~KinematicGUI_AnimDlg()
{
  // no need to delete child widgets, Qt does it all for us
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::Init()
{
  /* init variables */
  myGeomBase = new GEOMBase();
  myGeomGUI = GEOMContext::GetGeomGUI();
  myGeomGUI->SetActiveDialogBox((QDialog*)this);
  
  RadioButton1->setChecked(TRUE);
  Slider1->setPageStep(1);
  Slider1->setValue(0);

  myDuration = 5.0;
  myNbSeq = 50;
  myIsActive = false;
  myType = 0;

  /* signals and slots connections */
  connect(buttonClose, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(Slider1, SIGNAL(valueChanged(int)), this, SLOT(ClickOnSlider(int)));
  connect(PushButton2, SIGNAL(clicked()), this, SLOT(ClickOnFirst()));
  connect(PushButton3, SIGNAL(clicked()), this, SLOT(ClickOnPrevious()));
  connect(PushButton4, SIGNAL(clicked()), this, SLOT(ClickOnPlay()));
  connect(PushButton5, SIGNAL(clicked()), this, SLOT(ClickOnNext()));
  connect(PushButton6, SIGNAL(clicked()), this, SLOT(ClickOnLast()));
  connect(PushButton7, SIGNAL(clicked()), this, SLOT(ClickOnExport()));
  connect(CheckButton1, SIGNAL(clicked()), this, SLOT(ClickOnShading()));

  connect(PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));

  /* Move widget on the botton right corner of main widget */
//   int x, y;
//   myGeomBase->DefineDlgPosition(this, x, y);
//   this->move(x, y);

  /* displays Dialog */
  this->SelectionIntoArgument();
  this->show();

  return;
}


//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void KinematicGUI_AnimDlg::SelectionIntoArgument()
{
  LineEdit1->setText("");
  QString aString = "";
  this->ClearAnim(true);
  myType = 0;

  int nbSel = myGeomBase->GetNameOfSelectedIObjects(mySelection, aString);
  if(nbSel != 1)
    return;

  /*  nbSel == 1  */
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();

  myGeomAnimation = myGeomBase->ConvertIOinAnimation(IO, testResult);
  if(!testResult) {
    myGeomContact = myGeomBase->ConvertIOinContact(IO, testResult);
    if(!testResult)
      return;

    myKContact = myKinematicGUI->CreateContact(myGeomContact);

    myType = 1;
    myNbSeq = 50;
    myDuration = 5.0;
  }
  else {
    myKAnimation = myKinematicGUI->CreateAnimation(myGeomAnimation);

    bool IsOk = false;
    for(int cpt = 1; cpt <= myKAnimation->myIndexToShape.Extent(); cpt++) {
      TopoDS_Shape myShape = myKAnimation->myIndexToShape.FindKey(cpt);
      if(myShape == myKAnimation->GetFrame())
	IsOk = true;
    }
    if(!IsOk)
      return;

    myType = 2;
    myNbSeq = myKAnimation->GetNbSeq();
    myDuration = myKAnimation->GetDuration();
    myKAnimation->myIsShading = CheckButton1->isChecked();
  }

  Slider1->setMaxValue(myNbSeq);
  LineEdit1->setText(aString);

  this->InitAnim();
  return;
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::SetEditCurrentArgument()
{
  LineEdit1->setFocus();
  SelectionIntoArgument();
  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnCancel()
{
  this->ClearAnim(true);
  disconnect(mySelection, 0, this, 0);
  reject();
  QApplication::restoreOverrideCursor();
  return;
}


//=================================================================================
// function : DeactivateActiveDialog()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::DeactivateActiveDialog()
{
  this->ClearAnim(true);
  this->setEnabled(false);
  disconnect(mySelection, 0, this, 0);
  myGeomGUI->SetActiveDialogBox(0);
  return;
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ActivateThisDialog()
{
  /* Emit a signal to deactivate the active dialog */
  myGeomGUI->EmitSignalDeactivateDialog();
  this->setEnabled(true);
  myGeomGUI->SetActiveDialogBox((QDialog*)this);
  connect(mySelection, SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument()));
  this->SelectionIntoArgument();
  return;
}


//=================================================================================
// function : closeEvent()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::closeEvent(QCloseEvent* e)
{
  /* same than click on cancel button */
  this->ClickOnCancel();
  return;
}


//=================================================================================
// function : enterEvent()
// purpose  : when mouse enter onto the QWidget
//=================================================================================
void KinematicGUI_AnimDlg::enterEvent(QEvent * e)
{
  if(GroupConstructors->isEnabled())
    return;
  this->ActivateThisDialog();
  return;
}


//=================================================================================
// function : ClickOnSlider()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnSlider(int newValue)
{
  myValue = newValue;
  this->NextFrame();
  return;
}


//=================================================================================
// function : ClickOnFirst()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnFirst()
{
  int aVal = Slider1->value();
  if(aVal == 0) {
    myValue = 0;
    this->NextFrame();
  }
  else
    Slider1->setValue(0);
  return;
}


//=================================================================================
// function : ClickOnPrevious()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnPrevious()
{
  int aVal = Slider1->value();
  if(aVal != 0)
    Slider1->setValue(aVal - 1);
  return;
}


//=================================================================================
// function : ClickOnPlay()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnPlay()
{
  QApplication::setOverrideCursor(Qt::waitCursor);
  myIsActive = true;
  qApp->lock();
  while(myIsActive) {
    this->ClickOnNext();
    qApp->syncX();

    qApp->unlock();
    usleep((int)(myDuration / myNbSeq * 1e6));
    qApp->lock();

    if (!myIsActive) break;
  }

  QApplication::restoreOverrideCursor();
  return;
}


//=================================================================================
// function : ClickOnNext()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnNext()
{
  int aVal = Slider1->value();
  if(aVal != myNbSeq)
    Slider1->setValue(aVal + 1);
  else
    myIsActive = false;
  return;
}


//=================================================================================
// function : ClickOnLast()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnLast()
{
  int aVal = Slider1->value();
  if(aVal == myNbSeq) {
    myValue = myNbSeq;
    this->NextFrame();
  }
  else
    Slider1->setValue(myNbSeq);

  return;
}


//=================================================================================
// class    : InitAnim()
// purpose  : 
//=================================================================================
void KinematicGUI_AnimDlg::InitAnim()
{
  if(myType == 0)
    return;

  myAISFrame = new AIS_Shape(TopoDS_Shape());

  if(myType == 1) { //Contact
    myAISFrame->Set(myKContact->GetShape1());

    myAISShape = new AIS_Shape(TopoDS_Shape());
    myAISShape->Set(myKContact->GetShape2());

    if(CheckButton1->isChecked()) { //In Shading
      myAISShape->SetColor(Quantity_NOC_GOLDENROD);
      myIC->Display(myAISShape, 1, 1, Standard_False);
    }
    else {
      myAISShape->SetColor(Quantity_NOC_GREEN);
      myIC->Display(myAISShape, 0, 1, Standard_False);
    }
  }
  else if(myType == 2) { //Assembly
    myKAnimation->myMovedShape.Clear();
    myKAnimation->myMovedShape.Add(myKAnimation->GetFrame());

    gp_Trsf aLoc;
    myLoc = aLoc;
    if(!myKAnimation->IsCreated)
      myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->GetFrame(), 1);
    myKAnimation->IsCreated = true;

    myAISFrame->Set(myKAnimation->GetFrame());
  }

  if(CheckButton1->isChecked()) { //In Shading
    myAISFrame->SetColor(Quantity_NOC_GOLDENROD);
    myIC->Display(myAISFrame, 1, 1, Standard_False);
  }
  else {
    myAISFrame->SetColor(Quantity_NOC_RED);
    myIC->Display(myAISFrame, 0, 1, Standard_False);
  }

  this->ClickOnFirst();
  this->NextFrame();
  return;
}


//=================================================================================
// class    : ClearAnim()
// purpose  : 
//=================================================================================
void KinematicGUI_AnimDlg::ClearAnim(int ClearPrs)
{
  myIC->Erase(myAISFrame, Standard_False, Standard_False);
  if(ClearPrs)
    myIC->ClearPrs(myAISFrame);

  if(myType == 1) { //Contact
    myIC->Erase(myAISShape, Standard_False, Standard_False);
    if(ClearPrs)
      myIC->ClearPrs(myAISShape);
  }
  if(myType == 2) { //Assembly
    if(!myKAnimation->myListOfAIS.IsEmpty()) {
      for(int i = 1; i <= myKAnimation->myNbAIS; i++) {
	Handle(AIS_Shape) myShape = Handle(AIS_Shape)::DownCast(myKAnimation->myListOfAIS.Value(i));
	myIC->Erase(myShape, Standard_False, Standard_False);
	if(ClearPrs)
	  myIC->ClearPrs(myShape);
      }
    }
  }
  myIC->UpdateCurrentViewer();
  return;
}


//=================================================================================
// class    : NextFrame()
// purpose  : 
//=================================================================================
void KinematicGUI_AnimDlg::NextFrame() {
  double Step, Val;
  Step = 1.0 / myNbSeq;
  Val = myValue * Step;

  if(myType == 1) { //Contact
    gp_Trsf aTrans = myKContact->GetTransformation(Val);
    Handle(Geom_Transformation) aGTrans = new Geom_Transformation();
    aGTrans->SetTrsf(aTrans);
    myAISShape->SetTransformation(aGTrans, false, false);

    if(CheckButton1->isChecked()) { //In Shading 
      myAISShape->SetColor(Quantity_NOC_GOLDENROD);
      myIC->SetDisplayMode(myAISShape, 1, Standard_False);
    }
    else {
      myAISShape->SetColor(Quantity_NOC_GREEN);
      myIC->SetDisplayMode(myAISShape, 0, Standard_False);
    }
    myIC->Display(myAISShape, Standard_True);
  }
  else if(myType == 2) { //Assembly
    myKAnimation->myMovedShape.Clear();
    myKAnimation->myMovedShape.Add(myKAnimation->GetFrame());

    myKAnimation->myNbAIS = 0;
    myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->GetFrame(), Val);

    myIC->UpdateCurrentViewer();
  }
  return;
}


//=================================================================================
// function : ClickOnShading()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnShading()
{
  if(myType == 0)
    return;

  QApplication::setOverrideCursor(Qt::waitCursor);
  this->ClearAnim(false);

  if(myType == 2)
    myKAnimation->myIsShading = CheckButton1->isChecked();

  this->InitAnim();
  QApplication::restoreOverrideCursor();

  return;
}


//=================================================================================
// function : ClickOnExport()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnExport()
{
  if(myType == 0)
    return;

  this->ClickOnFirst();

  QString fName = QAD_FileDlg::getFileName(QAD_Application::getDesktop(), QString::null, tr("OCC_IMAGE_FILES"), tr("INF_APP_DUMP_VIEW"), false);

  QString TestFile = QAD_Tools::getFileNameFromPath(fName, false);
  if(TestFile.isEmpty())
    return;

  QApplication::setOverrideCursor(Qt::waitCursor);
  qApp->processEvents();

  for(int i = 0; i <= myNbSeq; i++) {
    qApp->processEvents();
    QWidget* qw = ((OCCViewer_ViewFrame*)QAD_Application::getDesktop()->getActiveStudy()->getActiveStudyFrame()->getRightFrame()->getViewFrame())->getViewWidget();
    QPixmap px = QPixmap::grabWindow(qw->winId());

    QString fileName = QAD_Tools::getDirFromPath(fName, false);
    fileName = QAD_Tools::addSlash(fileName);
    fileName.append(TestFile);
    fileName.append(QString("_%1.").arg(i));
    QString fmt = QAD_Tools::getFileExtensionFromPath(fName);
    fileName.append(fmt);

    fmt = fmt.upper();
    if(fmt.isEmpty())
      fmt = QString("BMP"); // default format
    if(fmt == "JPG")
      fmt = "JPEG";
  
    bool bOk = px.save(fileName, fmt.latin1());

    this->ClickOnNext();
  }

  QApplication::restoreOverrideCursor();
  return;
}
