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

using namespace std;
#include "KinematicGUI_AnimDlg.h"

#include "QAD_RightFrame.h"
#include "OCCViewer_ViewFrame.h"
#include "QAD_FileDlg.h"
#include "QAD_Tools.h"

//=================================================================================
// class    : MyThread()
// purpose  : 
//=================================================================================
MyThread::MyThread(Kinematic_Animation* KAnimation, Handle(AIS_InteractiveContext) ic)
{
  myIC = ic;
  myKAnimation = KAnimation;
  myIsActive = false;
}

//=================================================================================
// function : ~MyThread()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
MyThread::~MyThread() {}

//=================================================================================
// class    : run()
// purpose  : 
//=================================================================================
void MyThread::run()
{
  qApp->lock();
  while(myIsActive) {
    emit frameChanged();

    qApp->syncX();

    double Step, Val;
    Step = 1.0 / myKAnimation->NbSeq();
    Val = myValue * Step;

    myKAnimation->myMovedShape.Clear();
    myKAnimation->myMovedShape.Add(myKAnimation->Frame());

    myKAnimation->myNbAIS = 0;
    myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->Frame(), Val);

    myIC->UpdateCurrentViewer();

    qApp->unlock();
    usleep((int)(myKAnimation->Duration() / myKAnimation->NbSeq() * 1e6));
    qApp->lock();

    if (!myIsActive) break;
  }

  qApp->unlock();
  emit stopped();
  QThread::exit();
  return;
}

//=================================================================================
// class    : InitAnim()
// purpose  : 
//=================================================================================
void MyThread::InitAnim()
{
  myKAnimation->myMovedShape.Clear();
  myKAnimation->myMovedShape.Add(myKAnimation->Frame());

  gp_Trsf aLoc;
  myLoc = aLoc;
  if(!myKAnimation->IsCreated)
    myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->Frame(), 1);
  myKAnimation->IsCreated = true;

  myAISFrame = new AIS_Shape(TopoDS_Shape());
  myAISFrame->Set(myKAnimation->Frame());
  if(myKAnimation->myIsShading) {
    myAISFrame->SetColor(Quantity_NOC_GOLDENROD);
    myIC->Display(myAISFrame, 1, 1, Standard_False);
  }
  else {
    myAISFrame->SetColor(Quantity_NOC_RED);
    myIC->Display(myAISFrame, 0, 1, Standard_False);
  }
  return;
}

//=================================================================================
// class    : ClearAnim()
// purpose  : 
//=================================================================================
void MyThread::ClearAnim(int ClearPrs)
{
  myIC->Erase(myAISFrame, Standard_False, Standard_False);
  myIC->ClearPrs(myAISFrame);

  if(!myKAnimation->ListOfAIS.IsEmpty()) {
    for(int i = 1; i <= myKAnimation->myNbAIS; i++) {
      Handle(AIS_Shape) myShape = Handle(AIS_Shape)::DownCast(myKAnimation->ListOfAIS.Value(i));
      myIC->Erase(myShape, Standard_False, Standard_False);
      if(ClearPrs)
	myIC->ClearPrs(myShape);
    }
  }
  myIC->UpdateCurrentViewer();
  return;
}

//=================================================================================
// class    : NextFrame()
// purpose  : 
//=================================================================================
void MyThread::NextFrame() {
  double Step, Val;
  Step = 1.0 / myKAnimation->NbSeq();
  Val = myValue * Step;

  myKAnimation->myMovedShape.Clear();
  myKAnimation->myMovedShape.Add(myKAnimation->Frame());

  myKAnimation->myNbAIS = 0;
  myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->Frame(), Val);

  myIC->UpdateCurrentViewer();
  return;
}

//=================================================================================
// class    : startAnimation()
// purpose  : 
//=================================================================================
void MyThread::startAnimation() {
  //DCQ : QThread doesn't work...
//   if(!myIsActive) {
//     myIsActive = true;
//     QThread::start();
//   }

  QApplication::setOverrideCursor(Qt::waitCursor);
  myIsActive = true;
  qApp->lock();
  while(myIsActive) {
    emit frameChanged();

    qApp->syncX();

    double Step, Val;
    Step = 1.0 / myKAnimation->NbSeq();
    Val = myValue * Step;

    myKAnimation->myMovedShape.Clear();
    myKAnimation->myMovedShape.Add(myKAnimation->Frame());

    myKAnimation->myNbAIS = 0;
    myKAnimation->GetNextShape(myIC, myLoc, myKAnimation->Frame(), Val);

    myIC->UpdateCurrentViewer();

    qApp->unlock();
    usleep((int)(myKAnimation->Duration() / myKAnimation->NbSeq() * 1e6));
    qApp->lock();

    if (!myIsActive) break;
  }

  QApplication::restoreOverrideCursor();
  qApp->unlock();
  emit stopped();
  return;
}

//=================================================================================
// class    : stopAnimation()
// purpose  : 
//=================================================================================
void MyThread::stopAnimation() {
  myIsActive = false;
  return;
}


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

  setCaption(tr("GEOM_ANIM_TITLE"));

  QPixmap image0(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIMATION")));
  QPixmap image1(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_FIRST")));
  QPixmap image2(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_PREV")));
  QPixmap image3(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_STOP")));
  QPixmap image4(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_PLAY")));
  QPixmap image5(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_NEXT")));
  QPixmap image6(QAD_Desktop::getResourceManager()->loadPixmap("GEOM",tr("ICON_ANIM_LAST")));

  /***************************************************************/
  GroupConstructors->setTitle(tr("GEOM_ANIM"));
  RadioButton1->setPixmap(image0);

  Group1 = new KinematicGUI_Anim_QTD(this, "Group1");
  Group1->PushButton1->setPixmap(image1);
  Group1->PushButton2->setPixmap(image2);
  Group1->PushButton3->setPixmap(image3);
  Group1->PushButton4->setPixmap(image4);
  Group1->PushButton5->setPixmap(image5);
  Group1->PushButton6->setPixmap(image6);
  Group1->PushButton7->setText(tr("GEOM_ANIM_EXPORT"));
  Group1->CheckButton1->setText(tr("GEOM_ANIM_SHADING"));
  Group1->CheckButton2->setText(tr("GEOM_ANIM_INLOOP"));

  Layout1->addWidget(Group1, 1, 0);
  /***************************************************************/

  /* Initialisation */
  myKinematicGUI = theKinematicGUI;
  QApplication::setOverrideCursor(Qt::waitCursor);
  Init(Sel, ic);
  QApplication::restoreOverrideCursor();
}


//=================================================================================
// function : ~KinematicGUI_AnimDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
KinematicGUI_AnimDlg::~KinematicGUI_AnimDlg()
{
  // no need to delete child widgets, Qt does it all for us
  this->destroy(TRUE, TRUE);
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::Init(SALOME_Selection* Sel, Handle(AIS_InteractiveContext) ic)
{
  /* init variables */
  mySelection = Sel;

  myGeomBase = new GEOMBase();
  myGeomGUI = GEOMContext::GetGeomGUI();
  myGeomGUI->SetActiveDialogBox((QDialog*)this);
  myGeom = myGeomGUI->myComponentGeom;

  
  Standard_Boolean testResult;
  Handle(SALOME_InteractiveObject) IO = mySelection->firstIObject();
  GEOM::GEOM_Animation_var myGeomAnim = myGeomBase->ConvertIOinAnimation(IO, testResult);
  if(!testResult)
    ClickOnCancel();

  Kinematic_Animation* KAnimation = myKinematicGUI->CreateAnimation(myGeomAnim);

  bool IsOk = false;
  for(int cpt = 1; cpt <= KAnimation->myIndexToShape.Extent(); cpt++) {
    TopoDS_Shape myShape = KAnimation->myIndexToShape.FindKey(cpt);
    if(myShape == KAnimation->Frame())
      IsOk = true;
  }
  if(!IsOk)
    ClickOnCancel();

  Group1->CheckButton2->setEnabled(false);

  Group1->Slider1->setMaxValue(KAnimation->NbSeq());
  Group1->Slider1->setPageStep(1);
  Group1->Slider1->setValue(0);

  /* signals and slots connections */
  connect(buttonClose, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));

  connect(Group1->Slider1, SIGNAL(valueChanged(int)), this, SLOT(ClickOnSlider(int)));
  connect(Group1->PushButton1, SIGNAL(clicked()), this, SLOT(ClickOnFirst()));
  connect(Group1->PushButton2, SIGNAL(clicked()), this, SLOT(ClickOnPrevious()));
  connect(Group1->PushButton3, SIGNAL(clicked()), this, SLOT(ClickOnStop()));
  connect(Group1->PushButton4, SIGNAL(clicked()), this, SLOT(ClickOnPlay()));
  connect(Group1->PushButton5, SIGNAL(clicked()), this, SLOT(ClickOnNext()));
  connect(Group1->PushButton6, SIGNAL(clicked()), this, SLOT(ClickOnLast()));
  connect(Group1->PushButton7, SIGNAL(clicked()), this, SLOT(ClickOnExport()));
  connect(Group1->CheckButton1, SIGNAL(clicked()), this, SLOT(ClickOnShading()));

  myAnimator = new MyThread(KAnimation, ic);
  connect(myAnimator, SIGNAL(frameChanged()), this, SLOT(OnNext()));
  connect(myAnimator, SIGNAL(stopped()), this, SLOT(ClickOnStop()));
  myAnimator->InitAnim();
  myAnimator->myValue = 0;
  myAnimator->NextFrame();

  /* Move widget on the botton right corner of main widget */
//   int x, y;
//   myGeomBase->DefineDlgPosition(this, x, y);
//   this->move(x, y);

  /* displays Dialog */
  RadioButton1->setChecked(TRUE);
  Group1->show();
  this->show();

  return;
}


//=================================================================================
// function : ClickOnCancel()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnCancel()
{
  this->ClickOnStop();
  myAnimator->ClearAnim(true);
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
  myAnimator->myValue = newValue;
  if(!myAnimator->running())
    myAnimator->NextFrame();
  return;
}


//=================================================================================
// function : ClickOnFirst()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnFirst()
{
  this->ClickOnStop();
  int aVal = Group1->Slider1->value();
  if(aVal == 0) {
    myAnimator->myValue = 0;
    myAnimator->NextFrame();
  }
  else
    Group1->Slider1->setValue(0);
  return;
}


//=================================================================================
// function : ClickOnPrevious()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnPrevious()
{
  this->ClickOnStop();
  int aVal = Group1->Slider1->value();
  if(aVal != 0)
    Group1->Slider1->setValue(aVal - 1);
  return;
}


//=================================================================================
// function : ClickOnStop()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnStop()
{
  myAnimator->stopAnimation();
  return;
}


//=================================================================================
// function : ClickOnPlay()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnPlay()
{
  if(!myAnimator->running())
    myAnimator->startAnimation();
  return;
}


//=================================================================================
// function : OnNext()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::OnNext()
{
  int aVal = Group1->Slider1->value();
  if(aVal != myAnimator->myKAnimation->NbSeq())
    Group1->Slider1->setValue(aVal + 1);
  else {
    if(Group1->CheckButton2->isChecked())
      Group1->Slider1->setValue(0);
    else
      this->ClickOnStop();
  }
  return;
}


//=================================================================================
// function : ClickOnNext()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnNext()
{
  this->ClickOnStop();
  this->OnNext();
  return;
}


//=================================================================================
// function : ClickOnLast()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnLast()
{
  this->ClickOnStop();
  int aVal = Group1->Slider1->value();
  if(aVal == myAnimator->myKAnimation->NbSeq()) {
    myAnimator->myValue = myAnimator->myKAnimation->NbSeq();
    myAnimator->NextFrame();
  }
  else
    Group1->Slider1->setValue(myAnimator->myKAnimation->NbSeq());

  return;
}


//=================================================================================
// function : ClickOnShading()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnShading()
{
  QApplication::setOverrideCursor(Qt::waitCursor);
  this->ClickOnStop();
  myAnimator->ClearAnim(false);
  myAnimator->myKAnimation->myIsShading = Group1->CheckButton1->isChecked();
  myAnimator->InitAnim();
  this->ClickOnFirst();
  QApplication::restoreOverrideCursor();
  return;
}


//=================================================================================
// function : ClickOnExport()
// purpose  :
//=================================================================================
void KinematicGUI_AnimDlg::ClickOnExport()
{
  this->ClickOnFirst();

  QString fName = QAD_FileDlg::getFileName(QAD_Application::getDesktop(), QString::null, tr("OCC_IMAGE_FILES"), tr("INF_APP_DUMP_VIEW"), false);

  QString TestFile = QAD_Tools::getFileNameFromPath(fName, false);
  if(TestFile.isEmpty())
    return;

  QApplication::setOverrideCursor(Qt::waitCursor);
  qApp->processEvents();

  for(int i = 0; i <= myAnimator->myKAnimation->NbSeq(); i++) {
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

    this->OnNext();
  }

  QApplication::restoreOverrideCursor();
  return;
}
