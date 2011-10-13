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

#include "AdvancedGUI_SmoothingSurfaceDlg.h"

#include <DlgRef.h>
#include <GeometryGUI.h>
#include <GEOMBase.h>

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>
#include <SalomeApp_Application.h>
//#include <SalomeApp_Study.h>
#include <LightApp_SelectionMgr.h>

//#include <SALOME_ListIteratorOfListIO.hxx>
//#include <SALOME_ListIO.hxx>

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
AdvancedGUI_SmoothingSurfaceDlg::AdvancedGUI_SmoothingSurfaceDlg (GeometryGUI* theGeometryGUI, QWidget* parent)
  : GEOMBase_Skeleton(theGeometryGUI, parent, false)
{
  QPixmap imageOp  (SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_DLG_SMOOTHINGSURFACE_POINTS_ISCLOSED")));
  QPixmap imageSel (SUIT_Session::session()->resourceMgr()->loadPixmap("GEOM", tr("ICON_SELECT")));
  QPixmap image1( SUIT_Session::session()->resourceMgr()->loadPixmap( "GEOM", tr( "ICON_SELECT" ) ) );

  setWindowTitle(tr("GEOM_SMOOTHINGSURFACE_TITLE"));

  /***************************************************************/
  mainFrame()->GroupConstructors->setTitle(tr("GEOM_SMOOTHINGSURFACE"));
  mainFrame()->RadioButton1->setIcon(imageOp);
  mainFrame()->RadioButton2->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton2->close();
  mainFrame()->RadioButton3->setAttribute(Qt::WA_DeleteOnClose);
  mainFrame()->RadioButton3->close();

  GroupPoints = new DlgRef_1Sel1Check( centralWidget() );

  GroupPoints->GroupBox1->setTitle( tr( "GEOM_NODES" ) );
  GroupPoints->TextLabel1->setText( tr( "GEOM_POINTS" ) );
  GroupPoints->PushButton1->setIcon( image1 );
  GroupPoints->LineEdit1->setReadOnly( true );
  GroupPoints->CheckButton1->setText( tr( "GEOM_IS_CLOSED" ) );
  GroupPoints->CheckButton1->setChecked(false);

  QVBoxLayout* layout = new QVBoxLayout(centralWidget());
  layout->setMargin(0); layout->setSpacing(6);
  layout->addWidget(GroupPoints);
  /***************************************************************/

  setHelpFileName("create_smoothingsurface_page.html");

  Init();
}

//=================================================================================
// Destructor
//=================================================================================
AdvancedGUI_SmoothingSurfaceDlg::~AdvancedGUI_SmoothingSurfaceDlg()
{
  // no need to delete child widgets, Qt does it all for us
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::Init()
{
  // Get setting of step value from file configuration
  SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
  double step = resMgr->doubleValue("Geometry", "SettingsGeomStep", 100);

  //@@ initialize dialog box widgets here @@//

  // Signal/slot connections
  connect(buttonOk(),    SIGNAL(clicked()), this, SLOT(ClickOnOk()));
  connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));
  connect(myGeomGUI,     SIGNAL(SignalDefaultStepValueChanged(double)),
          this,          SLOT(SetDoubleSpinBoxStep(double)));
  connect( myGeomGUI->getApp()->selectionMgr(),
           SIGNAL( currentSelectionChanged() ), this, SLOT( SelectionIntoArgument() ) );

  initName(tr("GEOM_SMOOTHINGSURFACE"));
  //displayPreview();
}

/*//=================================================================================
// function : SetDoubleSpinBoxStep()
// purpose  : Double spin box management
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::SetDoubleSpinBoxStep (double step)
{
  //@@ set double spin box step for all spin boxes here @@//
}*/

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::ClickOnOk()
{
  if (ClickOnApply())
    ClickOnCancel();
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool AdvancedGUI_SmoothingSurfaceDlg::ClickOnApply()
{
  if (!onAccept())
    return false;

  initName();

  return true;
}

//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::ActivateThisDialog()
{
  GEOMBase_Skeleton::ActivateThisDialog();
  //displayPreview();
}

//=================================================================================
// function : enterEvent [REDEFINED]
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::enterEvent (QEvent*)
{
  if (!mainFrame()->GroupConstructors->isEnabled())
    ActivateThisDialog();
}

/*//=================================================================================
// function : ValueChangedInSpinBox()
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::ValueChangedInSpinBox()
{
  //@@ connect custom spin boxes or other widget to this slot in the Init() method for automatic preview update @@//
  //displayPreview();
}*/

//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
GEOM::GEOM_IOperations_ptr AdvancedGUI_SmoothingSurfaceDlg::createOperation()
{
  return getGeomEngine()->GetIAdvancedOperations(getStudyId());
}

//=================================================================================
// function : isValid
// purpose  :
//=================================================================================
bool AdvancedGUI_SmoothingSurfaceDlg::isValid (QString& msg)
{
  bool ok = true;

  //@@ add custom validation actions here @@//

  return ok;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool AdvancedGUI_SmoothingSurfaceDlg::execute (ObjectList& objects)
{
  bool res = false;

  GEOM::GEOM_Object_var anObj;

  GEOM::GEOM_IAdvancedOperations_var anOper = GEOM::GEOM_IAdvancedOperations::_narrow(getOperation());

  //@@ retrieve input values from the widgets here @@//
  GEOM::ListOfGO_var points = new GEOM::ListOfGO();
  points->length( myPoints.count() );
  for ( int i = 0; i < myPoints.count(); i++ )
    points[i] = myPoints[i].copy();

  CORBA::Boolean theisClosed = GroupPoints->CheckButton1->isChecked();

  // call engine function
  anObj = anOper->MakeSmoothingSurface(points, theisClosed);
  res = !anObj->_is_nil();
  if (res && !IsPreview())
  {
    QStringList aParameters;
    //@@ put stringified input parameters to the string list here to store in the data model for notebook @@//
    /*aParameters << @@ stringified parameter value @@; // Points parameter
    aParameters << @@ stringified parameter value @@; // isClosed parameter*/
    if ( aParameters.count() > 0 ) anObj->SetParameters(aParameters.join(":").toLatin1().constData());
  }
  
  if (res)
    objects.push_back(anObj._retn());

  return res;
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection as changed or other case
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::SelectionIntoArgument()
{
  QList<GEOM::GeomObjPtr> points = getSelected( TopAbs_VERTEX, -1 );
  synchronize( myPoints, points );
  if ( !myPoints.isEmpty()  )
    GroupPoints->LineEdit1->setText( QString::number( myPoints.count() ) + "_" + tr( "GEOM_POINT" ) + tr( "_S_" ) );
  else
    GroupPoints->LineEdit1->setText( "" );
  processPreview();
}

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void AdvancedGUI_SmoothingSurfaceDlg::SetEditCurrentArgument()
{
  if ( sender() == GroupPoints->PushButton1 )
    myEditCurrentArgument = GroupPoints->LineEdit1;
  myEditCurrentArgument->setFocus();
  SelectionIntoArgument();
}

void AdvancedGUI_SmoothingSurfaceDlg::synchronize( QList<GEOM::GeomObjPtr>& left, QList<GEOM::GeomObjPtr>& right )
{
  // 1. remove items from the "left" list that are not in the "right" list
  QMutableListIterator<GEOM::GeomObjPtr> it1( left );
  while ( it1.hasNext() ) {
    GEOM::GeomObjPtr o1 = it1.next();
    bool found = false;
    QMutableListIterator<GEOM::GeomObjPtr> it2( right );
    while ( it2.hasNext() && !found )
      found = o1 == it2.next();
    if ( !found )
      it1.remove();
  }
  // 2. add items from the "right" list that are not in the "left" list (to keep selection order)
  it1 = right;
  while ( it1.hasNext() ) {
    GEOM::GeomObjPtr o1 = it1.next();
    bool found = false;
    QMutableListIterator<GEOM::GeomObjPtr> it2( left );
    while ( it2.hasNext() && !found )
      found = o1 == it2.next();
    if ( !found )
      left << o1;
  }
}

