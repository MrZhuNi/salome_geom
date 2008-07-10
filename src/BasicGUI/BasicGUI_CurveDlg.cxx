//  GEOM GEOMGUI : GUI for Geometry component
//
//  Copyright (C) 2003  OPEN CASCADE 
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
//
//
//
//  File   : BasicGUI_CurveDlg.cxx
//  Author : Nicolas REJNERI
//  Module : GEOM
//  $Header$

#include "BasicGUI_CurveDlg.h"

#include "SUIT_Desktop.h"
#include "SUIT_Session.h"
#include "SalomeApp_Application.h"
#include "SalomeApp_Study.h"
#include "LightApp_SelectionMgr.h"

#include <qlabel.h>

#include "utilities.h"

#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOME_ListIO.hxx"

#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TopTools_IndexedMapOfShape.hxx>

#include "GEOMImpl_Types.hxx"

using namespace std;
#include <string>

//=================================================================================
// class    : BasicGUI_CurveDlg()
// purpose  : Constructs a BasicGUI_CurveDlg which is a child of 'parent', with the 
//            name 'name' and widget flags set to 'f'.
//            The dialog will by default be modeless, unless you set 'modal' to
//            TRUE to construct a modal dialog.
//=================================================================================
BasicGUI_CurveDlg::BasicGUI_CurveDlg(GeometryGUI* theGeometryGUI, QWidget* parent,
                                     const char* name, bool modal, WFlags fl)
  :GEOMBase_Skeleton(theGeometryGUI, parent, name, modal, WStyle_Customize |
                     WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu)
{
  QPixmap image0(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_DLG_POLYLINE")));
  QPixmap image2(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_DLG_SPLINE")));
  QPixmap image3(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_DLG_BEZIER")));

  QPixmap image1(SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM",tr("ICON_SELECT")));

  setCaption(tr("GEOM_CURVE_TITLE"));

  /***************************************************************/
  RadioButton1->setPixmap( image0 );
  RadioButton2->setPixmap( image3 );
  RadioButton3->setPixmap( image2 );

  GroupPoints = new DlgRef_1Sel_QTD( this, "GroupPoints" );
  GroupPoints->GroupBox1->setTitle( tr( "GEOM_NODES" ) );
  GroupPoints->TextLabel1->setText( tr("GEOM_POINTS") );
  GroupPoints->PushButton1->setPixmap(image1);

  GroupPoints->LineEdit1->setReadOnly( true );

  Layout1->addWidget(GroupPoints, 2, 0);
  /***************************************************************/

  setHelpFileName("create_curve_page.html");

  Init();
}


//=================================================================================
// function : ~BasicGUI_CurveDlg()
// purpose  : Destroys the object and frees any allocated resources
//=================================================================================
BasicGUI_CurveDlg::~BasicGUI_CurveDlg()
{
}


//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::Init()
{
  /* init variables */
  myEditCurrentArgument = GroupPoints->LineEdit1;

  myPoints = new GEOM::ListOfGO();
  myPoints->length( 0 );

  globalSelection(); // close local contexts, if any
  localSelection(GEOM::GEOM_Object::_nil(), TopAbs_VERTEX);

  /* signals and slots connections */
  connect(buttonCancel, SIGNAL(clicked()), this, SLOT(ClickOnCancel()));
  connect(myGeomGUI, SIGNAL(SignalDeactivateActiveDialog()), this, SLOT(DeactivateActiveDialog()));
  connect(myGeomGUI, SIGNAL(SignalCloseAllDialogs()), this, SLOT(ClickOnCancel()));
  
  connect(buttonOk, SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply, SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(GroupConstructors, SIGNAL(clicked(int)), this, SLOT(ConstructorsClicked(int)));

  connect(GroupPoints->PushButton1, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
  connect(GroupPoints->LineEdit1, SIGNAL(returnPressed()), this, SLOT(LineEditReturnPressed()));

  connect(myGeomGUI->getApp()->selectionMgr(), 
	  SIGNAL(currentSelectionChanged()), this, SLOT(SelectionIntoArgument())) ;

  initName( tr( "GEOM_CURVE" ) );
  ConstructorsClicked( 0 );
}

//=================================================================================
// function : ConstructorsClicked()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::ConstructorsClicked( int id )
{
  QString aTitle = tr( id == 0 ? "GEOM_POLYLINE" : id == 1 ? "GEOM_BEZIER" : "GEOM_INTERPOL" );
  GroupConstructors->setTitle( aTitle );
	
	myPoints = new GEOM::ListOfGO();
  myPoints->length( 0 );  

  myEditCurrentArgument->setText("");
}


//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::SetEditCurrentArgument()
{
  if ( sender() == GroupPoints->PushButton1 ) 
    myEditCurrentArgument = GroupPoints->LineEdit1;
  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}


//=================================================================================
// function : LineEditReturnPressed()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::LineEditReturnPressed()
{
  if ( sender() == GroupPoints->LineEdit1 ) 
  {
    myEditCurrentArgument = GroupPoints->LineEdit1;
    GEOMBase_Skeleton::LineEditReturnPressed();
  }
}

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::ClickOnOk()
{
  if ( ClickOnApply() )
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool BasicGUI_CurveDlg::ClickOnApply()
{
  if ( !onAccept() )
    return false;

  initName();
  ConstructorsClicked( getConstructorId() );
  globalSelection(); // close local contexts, if any
  localSelection(GEOM::GEOM_Object::_nil(), TopAbs_VERTEX);
  return true;
}

//=======================================================================
// function : ClickOnCancel()
// purpose  :
//=======================================================================
void BasicGUI_CurveDlg::ClickOnCancel()
{
  GEOMBase_Skeleton::ClickOnCancel();
}

//=================================================================================
/*! function : isPointInList()
 *  purpose  : Check is point (theObject) in the list \a thePoints.
 * \author enk
 * \retval -1, if point not in list, else 1 in list
 */
//=================================================================================
static int isPointInList(list<GEOM::GEOM_Object_var>& thePoints,
		 	 GEOM::GEOM_Object_var& theObject)
{
  int len = thePoints.size();
  
  if(len<1){
    return -1;
  }
  
  for(list<GEOM::GEOM_Object_var>::iterator i=thePoints.begin();i!=thePoints.end();i++)
    if (string((*i)->GetEntry()) == string(theObject->GetEntry())){
      return 1;
    }

  return -1;
}

//=================================================================================
/*! function : removeUnnecessaryPnt()
 *  purpose  : Remove unnecessary points from list \a theOldPoints
 * \author enk
 * \li \a theOldPoints - ordered sequence with unnecessary point
 * \li \a theNewPoints - not ordered sequence with necessary points
 */
//=================================================================================
static void removeUnnecessaryPnt(list<GEOM::GEOM_Object_var>& theOldPoints,
				 GEOM::ListOfGO_var& theNewPoints)
{
  list<GEOM::GEOM_Object_var> objs_to_remove;
  for(list<GEOM::GEOM_Object_var>::iterator i=theOldPoints.begin();i!=theOldPoints.end();i++){
    bool found = false;
    for (int j=0;j<theNewPoints->length() && !found ; j++){
      if(string((*i)->GetEntry()) == string(theNewPoints[j]->GetEntry())){
	found = true;
      }
    }
    if(!found){
      objs_to_remove.push_back(*i);
      //cout << "removed: " << (*i)->GetEntry() << endl;
    }
  }
  for(list<GEOM::GEOM_Object_var>::iterator i=objs_to_remove.begin();i!=objs_to_remove.end();i++){
    theOldPoints.remove(*i);
  }
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void BasicGUI_CurveDlg::SelectionIntoArgument()
{
  myEditCurrentArgument->setText("");

  Standard_Boolean aRes = Standard_False;
  int IOC = IObjectCount();
  //  bool is_append = myPoints->length() < IOC; // if true - add point, else remove
  //  myPoints->length( IOC ); // this length may be greater than number of objects,
                           // that will actually be put into myPoints

  LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
  SalomeApp_Application* app =
    dynamic_cast< SalomeApp_Application* >( SUIT_Session::session()->activeApplication() );
  SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>( app->activeStudy() );
  _PTR(Study) aDStudy = appStudy->studyDS();
  GEOM::GEOM_IShapesOperations_var aShapesOp = getGeomEngine()->GetIShapesOperations( getStudyId() );

  int anIndex;
  TopoDS_Shape aShape;
  TColStd_IndexedMapOfInteger aMapIndexes;
  GEOM::GEOM_Object_var anObject;
  std::list<GEOM::GEOM_Object_var> aList;
  SALOME_ListIO selected;
  aSelMgr->selectedObjects( selected, QString::null, false );

  for (SALOME_ListIteratorOfListIO anIt (selected); anIt.More(); anIt.Next())
  {
    GEOM::GEOM_Object_var aSelectedObject = GEOMBase::ConvertIOinGEOMObject( anIt.Value(), aRes );

    if (!CORBA::is_nil(aSelectedObject) && aRes)
    {
      if (GEOMBase::GetShape( aSelectedObject, aShape, TopAbs_SHAPE ) && !aShape.IsNull())
      {
        aSelMgr->GetIndexes( anIt.Value(), aMapIndexes );

        if (aMapIndexes.Extent() > 0)
        {
          for (int ii=1; ii <= aMapIndexes.Extent(); ii++) {
            anIndex = aMapIndexes(ii);
            QString aName = GEOMBase::GetName( aSelectedObject );
            aName = aName + ":vertex_" + QString::number( anIndex );
            anObject = aShapesOp->GetSubShape(aSelectedObject, anIndex);
            //Find Object in study
            _PTR(SObject) obj ( aDStudy->FindObjectID( anIt.Value()->getEntry() ) );
            bool inStudy = false;
            _PTR(ChildIterator) iit (aDStudy->NewChildIterator(obj));
            for (; iit->More() && !inStudy; iit->Next()) {
              _PTR(SObject) child (iit->Value());
              QString aChildName = child->GetName();
              if (aChildName == aName) {
                inStudy = true;
                CORBA::Object_var corbaObj = GeometryGUI::ClientSObjectToObject(iit->Value());
                anObject = GEOM::GEOM_Object::_narrow( corbaObj );
              }
            }

            if (!inStudy)
              GeometryGUI::GetGeomGen()->AddInStudy(GeometryGUI::ClientStudyToStudy(aDStudy),
                                                    anObject, aName, aSelectedObject);

            int pos = isPointInList(myOrderedSel, anObject);
            if (pos==-1) {
              myOrderedSel.push_back(anObject);
            }
            //		    if (!inStudy)
            aList.push_back(anObject);
          }
        } else { // aMap.Extent() == 0
          int pos = isPointInList(myOrderedSel,aSelectedObject);
          if (pos==-1)
            myOrderedSel.push_back(aSelectedObject);
          aList.push_back(aSelectedObject);
        } 
      }
    }
  }

  myPoints->length( aList.size()  );  

  int k=0;
  for (list<GEOM::GEOM_Object_var>::iterator j=aList.begin();j!=aList.end();j++)
    myPoints[k++] = *j;

  if(IOC == 0)
    myOrderedSel.clear();
  else
    removeUnnecessaryPnt(myOrderedSel, myPoints);

  // if ( myOrderedSel.size() == myPoints->length() ) {
  myPoints->length( myOrderedSel.size()  );  
  k=0;
  for (list<GEOM::GEOM_Object_var>::iterator j=myOrderedSel.begin();j!=myOrderedSel.end();j++)
    myPoints[k++] = *j;
  //  }

  if ( myPoints->length() > 0  )
    GroupPoints->LineEdit1->setText( QString::number( myPoints->length() ) + "_" + tr( "GEOM_POINT" ) + tr( "_S_" ) );
  
  displayPreview(); 
}


//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
          this, SLOT(SelectionIntoArgument()));

  globalSelection(); // close local contexts, if any
  localSelection( GEOM::GEOM_Object::_nil(), TopAbs_VERTEX );
  ConstructorsClicked( getConstructorId() );
}

//=================================================================================
// function : DeactivateActiveDialog()
// purpose  : public slot to deactivate if active
//=================================================================================
void BasicGUI_CurveDlg::DeactivateActiveDialog()
{
  // myGeomGUI->SetState( -1 );
  GEOMBase_Skeleton::DeactivateActiveDialog();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::enterEvent(QEvent* e)
{
  if ( !GroupConstructors->isEnabled() )
    ActivateThisDialog();
}

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr BasicGUI_CurveDlg::createOperation()
{
  return myGeomGUI->GetGeomGen()->GetICurvesOperations( getStudyId() );
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool BasicGUI_CurveDlg::isValid( QString& msg )
{
  return myPoints->length() > 1;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool BasicGUI_CurveDlg::execute( ObjectList& objects )
{
  bool res = false;

  GEOM::GEOM_Object_var anObj;

  switch ( getConstructorId() )
  {
  case 0 :
    anObj = GEOM::GEOM_ICurvesOperations::_narrow( getOperation() )->MakePolyline( myPoints );
    res = true;
    break;
  case 1 :
    anObj = GEOM::GEOM_ICurvesOperations::_narrow( getOperation() )->MakeSplineBezier( myPoints );
    res = true;
    break;
  case 2 :
    anObj = GEOM::GEOM_ICurvesOperations::_narrow( getOperation() )->MakeSplineInterpolation( myPoints );
    res = true;
    break;
  }

  if ( !anObj->_is_nil() )
    objects.push_back( anObj._retn() );

  return res;
}

//=================================================================================
// function : closeEvent
// purpose  :
//=================================================================================
void BasicGUI_CurveDlg::closeEvent( QCloseEvent* e )
{
  GEOMBase_Skeleton::closeEvent( e );
}

