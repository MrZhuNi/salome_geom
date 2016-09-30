// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : MeasureGUI_AnnotationDlg.cxx

#include "MeasureGUI_AnnotationDlg.h"

#include <DlgRef.h>
#include <GEOMBase.h>
#include <GEOMBase_Skeleton.h>
#include <GEOM_Displayer.h>
#include <GeometryGUI.h>
#include <GEOM_Annotation.hxx>

#include <SOCC_Prs.h>
#include <SOCC_ViewModel.h>

#include <SalomeApp_Application.h>
#include <SalomeApp_Study.h>
#include <SalomeApp_Tools.h>
#include <SalomeApp_IntSpinBox.h>
#include <SalomeApp_DoubleSpinBox.h>

#include <LightApp_SelectionMgr.h>

#include <OCCViewer_ViewModel.h>
#include <OCCViewer_ViewManager.h>
#include <SVTK_ViewModel.h>
#include <SALOME_Prs.h>
#include <SALOME_ListIO.hxx>

#include <SUIT_Desktop.h>
#include <SUIT_MessageBox.h>
#include <SUIT_OverrideCursor.h>
#include <SUIT_ResourceMgr.h>
#include <SUIT_Session.h>
#include <SUIT_ViewWindow.h>
#include <SUIT_ViewManager.h>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <TColStd_MapOfInteger.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

#include <limits>

#include <SALOMEDS_wrap.hxx>
#include <GEOMImpl_Types.hxx>

#ifdef max
#undef max
#endif

//=======================================================================
//function : MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::MeasureGUI_AnnotationDlg( GeometryGUI* theGeometryGUI,
        const bool theIsCreate, QWidget* parent, bool modal, Qt::WindowFlags fl )
: GEOMBase_Skeleton( theGeometryGUI, parent, modal, fl ),
  myIsCreation( theIsCreate )
//myCurStepID( stepID ),
//myCurStepTable( NULL ),
//myIsHiddenMain( false )
{
    SUIT_ResourceMgr* resMgr = SUIT_Session::session()->resourceMgr();
    QPixmap iconSelect(resMgr->loadPixmap("GEOM", tr("ICON_SELECT")));

    setWindowTitle(
            myIsCreation ?
                    tr("CREATE_ANNOTATION_TITLE") :
                    tr("EDIT_ANNOTATION_TITLE"));

    // Shape type button group
    mainFrame()->GroupConstructors->hide();

    // Field name
    mainFrame()->GroupBoxName->setTitle(tr("ANNOTATION_NAME"));

    // Field properties

    QGroupBox* propGroup = new QGroupBox(tr("PROPERTIES"), centralWidget());
    QGridLayout* propLayout = new QGridLayout(propGroup);
    propLayout->setMargin(9);
    propLayout->setSpacing(6);

    QLabel* textLabel = new QLabel(tr("TEXT"), propGroup);
    myTextEdit = new QLineEdit(propGroup);
    propLayout->addWidget(textLabel, 0, 0);
    propLayout->addWidget(myTextEdit, 0, 1, 1, 2);

    // shape
    QLabel* shapeLabel = new QLabel(tr("SHAPE"), propGroup);
    myShapeSelBtn = new QPushButton(propGroup);
    myShapeSelBtn->setIcon(iconSelect);
    myShapeSelBtn->setEnabled(myIsCreation);
    myShapeName = new QLineEdit(propGroup);
    myShapeName->setReadOnly(true);
    myShapeName->setEnabled(myIsCreation);

    // data type
    QLabel* typeLabel = new QLabel( tr("ANNOTATION_TYPE"), propGroup );
    myTypeCombo = new QComboBox( propGroup );
    myTypeCombo->insertItem( 0, tr("3D"));
    myTypeCombo->insertItem( 1, tr("2D"));
    myTypeCombo->setCurrentIndex( 0 ); // double
/*
     // dimension
     QLabel* dimLabel = new QLabel( tr("SHAPE_TYPE"), propGroup );
     myDimCombo = new QComboBox( propGroup );

     // nb components
     QLabel* nbCompsLabel = new QLabel( tr("NB_COMPS"), propGroup );
     myNbCompsSpin = new SalomeApp_IntSpinBox( 1, 1000, 1, propGroup, true, true );
     myNbCompsSpin->setValue( 1 );*/
    propLayout->addWidget(shapeLabel, 1, 0);
    propLayout->addWidget(myShapeSelBtn, 1, 1);
    propLayout->addWidget(myShapeName, 1, 2);
    propLayout->addWidget(typeLabel,          2, 0, 1, 2);
    propLayout->addWidget(myTypeCombo,        2, 2);
     /*propLayout->addWidget(dimLabel,           2, 0, 1, 2);
     propLayout->addWidget(myDimCombo,         2, 2);
     propLayout->addWidget(nbCompsLabel,       3, 0, 1, 2);
     propLayout->addWidget(myNbCompsSpin,      3, 2);
     // propLayout->addWidget(shapeLabel,         0, 0);
     // propLayout->addWidget(myShapeName,        0, 1);
     // propLayout->addWidget(typeLabel,          1, 0);
     // propLayout->addWidget(myTypeCombo,        1, 1);
     // propLayout->addWidget(dimLabel,           2, 0);
     // propLayout->addWidget(myDimCombo,         2, 1);
     // propLayout->addWidget(nbCompsLabel,       3, 0);
     // propLayout->addWidget(myNbCompsSpin,      3, 1);
     */
    propLayout->setColumnStretch(2, 5);

    QLabel* shapeTypeLabel = new QLabel( tr("SUB_SHAPE"), propGroup );
    mySubShapeTypeCombo = new QComboBox( propGroup );
    QMap<QString, TopAbs_ShapeEnum> aShapeTypes;
    aShapeTypes["NONE"] = TopAbs_SHAPE;
    aShapeTypes["VERTEX"] = TopAbs_VERTEX;
    aShapeTypes["EDGE"] = TopAbs_EDGE;
    aShapeTypes["FACE"] = TopAbs_FACE;
    aShapeTypes["SOLID"] = TopAbs_SOLID;
    QMap<QString, TopAbs_ShapeEnum>::const_iterator anIt = aShapeTypes.begin(),
                                                    aLast = aShapeTypes.end();
    for ( ; anIt != aLast; anIt++)
      mySubShapeTypeCombo->addItem( anIt.key(), anIt.value());
    mySubShapeTypeCombo->setCurrentIndex( 0 ); // VERTEX

    propLayout->addWidget(shapeTypeLabel, 3, 0);
    propLayout->addWidget(mySubShapeTypeCombo, 3, 1, 1, 2);


    // sub-shape
    mySubShapeSelBtn = new QPushButton(propGroup);
    mySubShapeSelBtn->setIcon(iconSelect);
    mySubShapeSelBtn->setEnabled(myIsCreation);
    mySubShapeName = new QLineEdit(propGroup);
    mySubShapeName->setReadOnly(true);
    mySubShapeName->setEnabled(myIsCreation);

    propLayout->addWidget(mySubShapeSelBtn, 4, 1);
    propLayout->addWidget(mySubShapeName, 4, 2);

    // Field values
    /*
     QGroupBox* valsGroup = new QGroupBox(tr("VALUES"), centralWidget());
     QGridLayout* valsLayout = new QGridLayout(valsGroup);
     valsLayout->setMargin(9);
     valsLayout->setSpacing(6);

     // value table
     mySwitchTableWdg = new QWidget(valsGroup);
     QVBoxLayout* switchTableGrpLayout = new QVBoxLayout(mySwitchTableWdg);
     switchTableGrpLayout->setMargin(0);
     switchTableGrpLayout->setSpacing(0);

     // step add/rm controls
     QPushButton* addStepBtn = new QPushButton( tr("ADD_STEP"), valsGroup );
     QLabel* curStepLbl = new QLabel(tr("STEP"), valsGroup );
     myStepEdit = new QLineEdit( valsGroup );
     myStepsCombo = new QComboBox(valsGroup);
     myStepsCombo->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
     myRmStepBtn = new QPushButton( tr("REMOVE_STEP"), valsGroup );

     // step browse controls
     myPrevStepBtn = new QPushButton( tr("PREV_STEP"), valsGroup );
     QLabel* stampLbl = new QLabel(tr("STAMP"), valsGroup );
     myStampSpin = new SalomeApp_IntSpinBox( -theIntLimit, theIntLimit, 1, valsGroup, true, true);
     myNextStepBtn = new QPushButton( tr("NEXT_STEP"), valsGroup );

     valsLayout->addWidget(mySwitchTableWdg,   0, 0, 1, 5);
     valsLayout->addWidget(addStepBtn,         1, 0);
     valsLayout->addWidget(curStepLbl,         1, 1);
     valsLayout->addWidget(myStepEdit,         1, 2);
     valsLayout->addWidget(myStepsCombo,       1, 3);
     valsLayout->addWidget(myRmStepBtn,        1, 4);
     valsLayout->addWidget(myPrevStepBtn,      2, 0);
     valsLayout->addWidget(stampLbl,           2, 1);
     valsLayout->addWidget(myStampSpin,        2, 2, 1, 2);
     valsLayout->addWidget(myNextStepBtn,      2, 4);

     valsLayout->setColumnStretch(3, 5);
     valsLayout->setRowStretch   (0, 5);
     */
    QVBoxLayout* layout = new QVBoxLayout(centralWidget());
    layout->setMargin(0);
    layout->setSpacing(6);
    layout->addWidget(propGroup);
    //layout->addWidget(valsGroup);

    setHelpFileName("geom_field_page.html");

    LightApp_SelectionMgr* aSelMgr = myGeomGUI->getApp()->selectionMgr();
    connect(aSelMgr, SIGNAL(currentSelectionChanged()), this,
            SLOT(SelectionIntoArgument()));
    /*
     connect(myPrevStepBtn, SIGNAL(clicked()),                 this, SLOT( onPrevStep() ));
     connect(myNextStepBtn, SIGNAL(clicked()),                 this, SLOT( onNextStep() ));
     connect(addStepBtn,    SIGNAL(clicked()),                 this, SLOT( onAddStep() ));
     connect(myRmStepBtn,   SIGNAL(clicked()),                 this, SLOT( onRmStep() ));
     connect(myStampSpin,   SIGNAL(valueChanged(int)),         this, SLOT( onStampChange() ));
     connect(myStepsCombo,  SIGNAL(currentIndexChanged(int)),  this, SLOT( showCurStep() ));
     connect(myStepsCombo,  SIGNAL(activated(int)),            this, SLOT( showCurStep() ));
     */
    connect(buttonOk(), SIGNAL(clicked()), this, SLOT(ClickOnOk()));
    connect(buttonApply(), SIGNAL(clicked()), this, SLOT(ClickOnApply()));

    Init();
    //updateState();
}

//=======================================================================
//function : ~MeasureGUI_AnnotationDlg
//purpose  : 
//=======================================================================

MeasureGUI_AnnotationDlg::~MeasureGUI_AnnotationDlg() {
    /*GEOM_Displayer* aDisplayer = getDisplayer();
     if (myIsHiddenMain) {
     aDisplayer->Display(myShape);
     myIsHiddenMain = false;
     }
     // if ( !myAnnotation->_is_nil())
     //   aDisplayer->Display(myAnnotation);

     QListIterator<int> anIter( myHiddenFieldStepIds );
     while( anIter.hasNext() )
     {
     const int aStepId = anIter.next();
     GEOM::GEOM_FieldStep_var step = myAnnotation->GetStep( aStepId );
     if( !step->_is_nil() )
     aDisplayer->Display( step, false );
     }
     aDisplayer->UpdateViewer();*/
}

bool MeasureGUI_AnnotationDlg::isCreateOperation() const {
    return myIsCreation;
}

//=================================================================================
// function : Init()
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::Init() {
    /*myDmMode = -1;
     myHiddenFieldStepIds.clear();
     myStepsCombo->clear();
     */
    if (myIsCreation/* || myAnnotation->_is_nil()*/) {
        //myIsCreation = true;
        /*myStepTables.clear();
         if ( myCurStepTable )
         myCurStepTable->hide();
         myCurStepTable = NULL;
         myCurStepID = 0;
         */
        initName(tr("ANNOTATION_PREFIX"));
        //myStampSpin->setValue(0);


        myAnnotationProperties.Position = gp_Pnt(50, 50, 50);
        gp_Pnt Attach;

        SelectionIntoArgument();

        //onAddStep();

        mySelectionModes.clear();
        mySelectionModes.append(TopAbs_SHAPE);

        connect(myShapeSelBtn, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));
        connect(mySubShapeSelBtn, SIGNAL(clicked()), this, SLOT(SetEditCurrentArgument()));

        connect(myTextEdit,   SIGNAL(currentIndexChanged(int)),  this, SLOT( onTextChange()));
        connect(myTypeCombo,   SIGNAL(currentIndexChanged(int)),  this, SLOT( onTypeChange()));
        connect(mySubShapeTypeCombo,   SIGNAL(currentIndexChanged(int)),  this, SLOT( onSubShapeTypeChange()));


        /* connect(myDimCombo,    SIGNAL(currentIndexChanged(int)),  this, SLOT( onDimChange()));
         connect(myNbCompsSpin, SIGNAL(valueChanged(int)),         this, SLOT( onNbCompsChange()));*/

    } else // edition
    {
        /*// 1) get and sort step IDs
         // 2) hide all displayed field steps' presentations
         GEOM_Displayer* aDisplayer = getDisplayer();
         GEOM::ListOfLong_var stepIDs = myAnnotation->GetSteps();
         size_t i;
         QList< int > stepsList;
         for ( i = 0; i < stepIDs->length(); ++i )
         {
         int aStepId = stepIDs[i];
         stepsList.push_back( aStepId );
         GEOM::GEOM_FieldStep_var step = myAnnotation->GetStep( aStepId );
         if( !step->_is_nil() )
         {
         QString anEntry( step->GetStudyEntry() );
         if( aDisplayer->IsDisplayed( anEntry ) )
         {
         aDisplayer->Erase( step, false, false );
         myHiddenFieldStepIds << aStepId;
         }
         }
         }
         qSort( stepsList.begin(), stepsList.end() );

         myIsCreation = false;

         CORBA::String_var fName = "Annotation_name";//myShape->GetName();
         myMainFrame->ResultName->setText( fName.in() );

         //myShape = myAnnotation->GetShape();
         CORBA::String_var sName;
         if ( !myShape->_is_nil() )
         sName = myShape->GetName();
         myShapeName->setText( sName.in() ? sName.in() : "");

         myTypeCombo->setCurrentIndex( myAnnotation->GetDataType() );

         updateDims( myAnnotation->GetDimension() );
         updateShapeIDs();

         GEOM::string_array_var compNames = myAnnotation->GetComponents();
         myNbCompsSpin->setValue( compNames->length() );

         myShapeSelBtn->setEnabled( false );
         myShapeName->setEnabled( false );

         myTypeCombo->setEnabled( false );
         myDimCombo->setEnabled( false );
         myNbCompsSpin->setEnabled( false );

         myStepsCombo->blockSignals( true );
         for ( i = 0; i < stepIDs->length(); ++i )
         {
         myStepsCombo->insertItem( i, QString::number( stepsList[i] ));
         if ( myCurStepID == stepsList[i] )
         myStepsCombo->setCurrentIndex( i );
         }
         // if ( myStepsCombo->count() == 0 )
         //   myStepsCombo->insertItem( 0, QString::number( myCurStepID ));
         myStepsCombo->blockSignals( false );

         showCurStep();*/
        //activateSelection();
    }
    //GEOMBase_Helper::displayPreview( true, false, true, true, myLineWidth );
}

//=================================================================================
// function : getShapeType()
// purpose  :
//=================================================================================
TopAbs_ShapeEnum MeasureGUI_AnnotationDlg::getShapeType() const
{
    return (TopAbs_ShapeEnum)mySubShapeTypeCombo->itemData( mySubShapeTypeCombo->currentIndex() ).toInt();
}

//=================================================================================
// function : enterEvent()
// purpose  :
//=================================================================================
/*void MeasureGUI_AnnotationDlg::enterEvent(QEvent* e)
 {
 //if (!buttonCancel()->isEnabled())
 //  ActivateThisDialog();
 }*/

//=================================================================================
// function : ClickOnOk()
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::ClickOnOk()
{
    setIsApplyAndClose(true);
    if (ClickOnApply())
        ClickOnCancel();
    setIsApplyAndClose(false);
}

//=================================================================================
// function : ClickOnApply()
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::ClickOnApply()
{
    if(!isApplyAndClose()) {
        setIsDisableBrowsing( true );
        setIsDisplayResult( false );
    }

    QString msg;
    if ( !isValid( msg ) ) {
        showError( msg );
        return false;
    }
    SUIT_OverrideCursor wc;
    SUIT_Session::session()->activeApplication()->putInfo( "" );

    try
    {
        if ( openCommand() )
            if (!execute (/*isApplyAndClose()*/))
            {
                abortCommand();
                showError();
                return false;
            }
    }
    catch( const SALOME::SALOME_Exception& e ) {
        SalomeApp_Tools::QtCatchCorbaException( e );
        abortCommand();
        return false;
    }
    commitCommand();

    if(!isApplyAndClose()) {
        setIsDisableBrowsing( false );
        setIsDisplayResult( true );
    }

    /*if ( myIsCreation )
    {
        myAnnotation = GEOM::GEOM_Field::_nil();
        if ( !isApplyAndClose() )
            Init();
    }*/
    return true;
 }

//=================================================================================
// function : ActivateThisDialog()
// purpose  :
//=================================================================================
/*void MeasureGUI_AnnotationDlg::ActivateThisDialog()
 {
 GEOMBase_Skeleton::ActivateThisDialog();

 //connect(myGeomGUI->getApp()->selectionMgr(), SIGNAL(currentSelectionChanged()),
 //        this, SLOT(SelectionIntoArgument()));

 //activateSelection();
 }*/

//=================================================================================
// function : SetEditCurrentArgument()
// purpose  :
//=================================================================================
void MeasureGUI_AnnotationDlg::SetEditCurrentArgument()
{
    QPushButton* aSelectButton = (QPushButton*)sender();
    QPushButton* anOtherButton = 0;
    if (aSelectButton == myShapeSelBtn) {
        myEditCurrentArgument = myShapeName;
        anOtherButton = mySubShapeSelBtn;
    }
    else if (aSelectButton == mySubShapeSelBtn) {
        myEditCurrentArgument = mySubShapeName;
        anOtherButton = myShapeSelBtn;
    }
    myEditCurrentArgument->setFocus();

    if ( aSelectButton->isChecked() )
      anOtherButton->setDown(false);

    SelectionIntoArgument();
}

//=================================================================================
// function : SelectionIntoArgument()
// purpose  : Called when selection has changed
//=================================================================================
void MeasureGUI_AnnotationDlg::SelectionIntoArgument()
{
    if ( myIsCreation )
    {
        GEOM::GeomObjPtr anObj = getSelected( mySelectionModes );

        QString aName = GEOMBase::GetName( anObj.get() );
        myEditCurrentArgument->setText( aName );

        gp_Pnt anAttachPoint;
        int aSubShapeIndex = -1;
        if ( myEditCurrentArgument == myShapeName ) { // Selection of a shape is active
            if ( myShape->_is_nil() )
              myShape = GEOM::GEOM_Object::_nil();
            else
              myShape = anObj;
            anAttachPoint = gp_Pnt(100, 100, 100);
        }
        else if ( myEditCurrentArgument == myShapeName ) {
            if ( !myShape->_is_nil() ) {
                TopTools_IndexedMapOfShape aMainMap;
                TopoDS_Shape aMainShape;
                GEOMBase::GetShape(myShape.get(), aMainShape);
                TopExp::MapShapes(aMainShape, aMainMap);

                TopExp_Explorer anExp (aMainShape, getShapeType());
                bool isShowWarning = true;

                for (; anExp.More() && aSubShapeIndex < 0; anExp.Next()) {
                  TopoDS_Shape aSubShape = anExp.Current();
                  int anIndex = aMainMap.FindIndex(aSubShape);
                  if (anIndex >= 0)
                      aSubShapeIndex = anIndex;
                }
                anAttachPoint = gp_Pnt(150, 150, 150);
            }
        }
        myAnnotationProperties.Attach = anAttachPoint;

         /*if (myShape->_is_nil()) {
        //    myShapeName->setText("");
        //    myShape = GEOM::GEOM_Object::_nil();


        this->myCurrentSelector->clear();
        mySelectedShapes[myCurrentSelector] = GEOM::GeomObjPtr();

        //if ( GEOMBase::IsShape(anObj) && anObj->IsMainShape() ) {
            //  if (myIsHiddenMain) {
            //    GEOM_Displayer* aDisplayer = getDisplayer();
            //    aDisplayer->Display(myShape);
            //    myIsHiddenMain = false;
            //  }
            myShapeName->setText(GEOMBase::GetName(anObj));
            if (!anObj->_is_nil() /*&& !anObj->IsSame( myShape )*+/) {
                myShape = anObj;
                //myShapeMap.Clear();
                //myEditCurrentArgument = 0;

                // re-fill myDimCombo
                //const int curDim = getDim();
                //updateDims( -100 );

                // update table
                //onDimChange();
                //}
            }
        }*/
        /*else {
         if (myIsHiddenMain) {
         GEOM_Displayer* aDisplayer = getDisplayer();
         aDisplayer->Display(myShape);
         myIsHiddenMain = false;
         }
         }*/
    }
    /*else // select rows of a table according to shape selection
     {
     /* if ( !myCurStepTable )
     return;

     bool isSignalsBlocked = myCurStepTable->signalsBlocked();
     myCurStepTable->blockSignals(true);

     QList< int > rowsToSelect;
     TColStd_IndexedMapOfInteger aMapIndex;
     if ( getSelectedSubshapes(aMapIndex ))
     for (int ii = 1, nn = aMapIndex.Extent(); ii <= nn; ii++) {
     const int shapeID = aMapIndex( ii );
     for (int row = 0, n = myShapeIDs.size(); row < n; row++)
     if ( myShapeIDs[row] == shapeID ) {
     rowsToSelect.push_back( row );
     break;
     }
     }
     myCurStepTable->selectRows( rowsToSelect );

     myCurStepTable->blockSignals(isSignalsBlocked);*+/
     }*/
}
/*
 //=================================================================================
 // function : onDimChange()
 // purpose  : update dialog at change of dimension
 //=================================================================================
 void MeasureGUI_AnnotationDlg::onDimChange()
 {
 // get order of sub-shapes - myShapeIDs
 updateShapeIDs();

 // update size of tables
 const int nbRows = myShapeIDs.size();
 QString subName = ( getDim() == -1 ) ? tr("WHOLE_SHAPE_VHEADER") : myDimCombo->currentText();
 QMap< int, StepTable* >::iterator tblIt = myStepTables.begin();
 for ( ; tblIt != myStepTables.end(); ++tblIt )
 if ( tblIt.value() )
 tblIt.value()->setDim( nbRows, subName );

 activateSelection();
 }

 //=======================================================================
 //function : onNbCompsChange
 //purpose  :
 //=======================================================================

 void MeasureGUI_AnnotationDlg::onNbCompsChange()
 {
 QMap< int, StepTable* >::iterator tblIt = myStepTables.begin();
 for ( ; tblIt != myStepTables.end(); ++tblIt )
 if ( tblIt.value() )
 tblIt.value()->setNbComps( getNbComps() );
 }
*/
//=======================================================================
//function : onTextChange
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::onTextChange()
{
    myAnnotationProperties.Text = myTextEdit->text();
    processPreview();
}

//=======================================================================
//function : onTypeChange
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::onTypeChange()
{
    myAnnotationProperties.IsScreenFixed = myTypeCombo->currentIndex() == 1;
    processPreview();
}

//=======================================================================
//function : onSubShapeTypeChange
//purpose  :
//=======================================================================
void MeasureGUI_AnnotationDlg::onSubShapeTypeChange()
{
    TopAbs_ShapeEnum aShapeType = getShapeType();
    myAnnotationProperties.ShapeType = aShapeType;

    processPreview();
}

 /*
 //=================================================================================
 // function : onPrevStep()
 // purpose  :
 //=================================================================================
 void MeasureGUI_AnnotationDlg::onPrevStep()
 {
 int i = myStepsCombo->currentIndex();
 if ( i > 0 )
 {
 myStepsCombo->setCurrentIndex( i-1 );
 showCurStep();
 }
 }

 //=================================================================================
 // function : onNextStep()
 // purpose  :
 //=================================================================================
 void MeasureGUI_AnnotationDlg::onNextStep()
 {
 int i = myStepsCombo->currentIndex();
 if ( i+1 < myStepsCombo->count() )
 {
 myStepsCombo->setCurrentIndex( i+1 );
 showCurStep();
 }
 }

 static int findInCombo( QComboBox* where, int what, bool& ok )
 {
 int idx = 0;
 ok = false;

 for ( ; idx < where->count() && !ok; idx++ ) {
 int step = where->itemText( idx ).toInt();
 if ( step == what ) {
 ok = true;
 }
 else if ( step > what )
 break;
 }

 return idx;
 }

 //=======================================================================
 //function : onAddStep
 //purpose  :
 //=======================================================================
 void MeasureGUI_AnnotationDlg::onAddStep()
 {
 int step = 0;

 if ( !myStepEdit->text().isEmpty() )
 step = myStepEdit->text().toInt();
 bool ok = false;
 int idx = findInCombo( myStepsCombo, step, ok );

 if ( ok ) {
 SUIT_MessageBox::critical(this, QObject::tr("ERR_ERROR"),
 tr("ERR_STEP_EXISTS"));
 return;
 }

 myStepsCombo->insertItem( idx, QString::number( step ));
 myStepsCombo->setCurrentIndex( idx );
 myRemovedSteps.remove( getCurStepID() );
 //showCurStep();
 }

 //=======================================================================
 //function : onRmStep
 //purpose  :
 //=======================================================================
 void MeasureGUI_AnnotationDlg::onRmStep()
 {
 if ( myStepsCombo->count() > 1 )
 {
 myStepTables.remove( getCurStepID() );
 myRemovedSteps.insert( getCurStepID() );
 if ( myCurStepTable )
 myCurStepTable->hide();
 myCurStepTable = NULL;
 myStepsCombo->removeItem( myStepsCombo->currentIndex() );
 //showCurStep();
 }
 }

 //=======================================================================
 //function : onStampChange
 //purpose  :
 //=======================================================================
 void MeasureGUI_AnnotationDlg::onStampChange()
 {
 if ( myCurStepTable )
 myCurStepTable->setStamp( myStampSpin->value() );
 }

 //=======================================================================
 //function : showCurStep
 //purpose  :
 //=======================================================================
 void MeasureGUI_AnnotationDlg::showCurStep()
 {
 myCurStepID = getCurStepID();
 myStepEdit->setText(QString::number( myCurStepID ));

 QStringList headers;
 if ( myCurStepTable )
 {
 if ( myCurStepTable->getStepID() == myCurStepID )
 {
 myCurStepTable->show();
 return;
 }
 else
 {
 myCurStepTable->hide();
 headers = myCurStepTable->getHeaders();
 myCurStepTable->getStamp();
 }
 }

 if ( myStepTables.count ( myCurStepID ))
 {
 myCurStepTable = myStepTables[ myCurStepID ];
 myCurStepTable->setHeaders( headers );
 }
 else
 {
 // if ( myStepsCombo->count() == 0 )
 //   return; // edit a field with no steps

 // get step values
 GEOM::GEOM_FieldStep_var stepVar;
 if ( !myAnnotation->_is_nil() )
 stepVar = myAnnotation->GetStep( myCurStepID );

 int nbComps = getNbComps();
 if ( headers.count() == 0 )
 {
 headers << tr("SUB_SHAPE_HEADER");
 if ( !myIsCreation && !myAnnotation->_is_nil() )
 {
 GEOM::string_array_var compNames = myAnnotation->GetComponents();
 for ( int iC = 0; iC < compNames->length(); ++iC )
 headers << compNames[ iC ].in();
 }
 else
 {
 for ( int iC = 0; iC < nbComps; ++iC )
 headers << QString("Comp %1").arg( iC + 1 );
 }
 }
 QString subName = ( getDim() == -1 ) ? tr("WHOLE_SHAPE_VHEADER") : myDimCombo->currentText();
 myCurStepTable = new StepTable( myCurStepID, getDataType(), myShapeIDs.size(),
 nbComps, subName, headers, stepVar, mySwitchTableWdg );
 myStepTables[ myCurStepID ] = myCurStepTable;
 mySwitchTableWdg->layout()->addWidget( myCurStepTable );
 }
 myCurStepTable->show();
 myStampSpin->setValue( myCurStepTable->getStamp() );

 connect(myCurStepTable, SIGNAL(itemSelectionChanged()), this, SLOT( highlightSubShapes() ));

 myPrevStepBtn->setEnabled( myStepsCombo->currentIndex() > 0 );
 myNextStepBtn->setEnabled( myStepsCombo->currentIndex()+1 < myStepsCombo->count() );
 myRmStepBtn->setEnabled( myStepsCombo->count() > 1 );
 }

 //=================================================================================
 // function : getSelectedSubshapes
 // purpose  :
 //=================================================================================
 int MeasureGUI_AnnotationDlg::getSelectedSubshapes (TColStd_IndexedMapOfInteger& theMapIndex)
 {
 theMapIndex.Clear();

 SalomeApp_Application* app = myGeomGUI->getApp();
 if (!app || myShape->_is_nil())
 return 0;

 LightApp_SelectionMgr* aSelMgr = app->selectionMgr();
 SALOME_ListIO aSelList;
 aSelMgr->selectedObjects(aSelList);

 // try to find out and process the global selection
 // (of not published objects and of published sub-shapes)
 {
 SALOME_ListIteratorOfListIO anIter (aSelList);
 for (int i = 0; anIter.More(); anIter.Next(), i++)
 {
 Handle(SALOME_InteractiveObject) anIObj = anIter.Value();
 QString anEntry = anIObj->getEntry();
 QString str = "_";
 int index = anEntry.lastIndexOf(str);
 if (index > 0) // selection among special preview
 {
 anEntry.remove(0, index+1);
 int anIndex = anEntry.toInt();
 if (anIndex)
 theMapIndex.Add(anIndex);
 }
 else // selection among published shapes
 {
 SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>(app->activeStudy());
 if (!appStudy) return 0;
 _PTR(Study) aStudy = appStudy->studyDS();

 _PTR(SObject) aSObj (aStudy->FindObjectID(anEntry.toLatin1().constData()));
 GEOM::GEOM_Object_var aGeomObj =
 GEOM::GEOM_Object::_narrow(GeometryGUI::ClientSObjectToObject(aSObj));
 TopoDS_Shape aShape;
 if (GEOMBase::GetShape(aGeomObj, aShape)) {
 if (aGeomObj->GetType() == GEOM_GROUP || aShape.ShapeType() == getShapeType())
 {
 TopExp_Explorer anExp (aShape, getShapeType());
 for (; anExp.More(); anExp.Next()) {
 TopoDS_Shape aSubShape = anExp.Current();
 int anIndex = myShapeMap.FindIndex(aSubShape);
 if (anIndex == 0) {
 SUIT_MessageBox::warning(app->desktop(), QObject::tr("WRN_WARNING"),
 tr("WRN_NOT_SUBSHAPE"));
 }
 theMapIndex.Add(anIndex);
 }
 }
 }
 }
 } // for aSelList
 }

 return theMapIndex.Extent();
 }

 //=======================================================================
 //function : getDim
 //purpose  :
 //=======================================================================
 int MeasureGUI_AnnotationDlg::getDim() const
 {
 int i = myDimCombo->currentIndex();
 return ( i < 0 ) ? -1 : myDimCombo->itemData( i ).toInt();
 }

 //=======================================================================
 //function : getDataType
 //purpose  :
 //=======================================================================
 int MeasureGUI_AnnotationDlg::getDataType() const
 {
 return myTypeCombo->currentIndex();
 }

 //=======================================================================
 //function : getCurStepID
 //purpose  :
 //=======================================================================
 int MeasureGUI_AnnotationDlg::getCurStepID() const
 {
 if ( myStepsCombo->count() > 0 )
 return myStepsCombo->currentText().toInt();
 return myCurStepID;
 }

 //=======================================================================
 //function : getNbComps
 //purpose  :
 //=======================================================================
 int MeasureGUI_AnnotationDlg::getNbComps() const
 {
 return myNbCompsSpin->value();
 }

 //=================================================================================
 // function : updateShapeIDs()
 // purpose  : update myShapeIDs
 //=================================================================================
 void MeasureGUI_AnnotationDlg::updateShapeIDs()
 {
 myShapeIDs.clear();
 //myShapeMap.Clear();
 if ( !myShape->_is_nil() )
 {
 TopoDS_Shape aShape =
 GEOM_Client::get_client().GetShape(GeometryGUI::GetGeomGen(), myShape);
 if ( myShapeMap.IsEmpty() )
 TopExp::MapShapes(aShape, myShapeMap);
 TopAbs_ShapeEnum shapeType = getShapeType();
 if ( shapeType == TopAbs_SHAPE )
 {
 myShapeIDs.push_back( 1 );
 }
 else
 {
 TopTools_IndexedMapOfShape dimSubShapesMap;
 TopExp::MapShapes(aShape, shapeType, dimSubShapesMap);
 myShapeIDs.resize( dimSubShapesMap.Extent() );
 for ( int i = 1; i <= dimSubShapesMap.Extent(); ++i )
 myShapeIDs[i-1] = myShapeMap.FindIndex( dimSubShapesMap(i) );
 }
 }
 }

 //=======================================================================
 //function : updateDims
 //purpose  : update myDimCombo
 //=======================================================================
 void MeasureGUI_AnnotationDlg::updateDims(int curDim)
 {
 myDimCombo->blockSignals( true );
 myDimCombo->clear();
 TopoDS_Shape aShape =
 GEOM_Client::get_client().GetShape(GeometryGUI::GetGeomGen(), myShape);

 if ( !aShape.IsNull() )
 {
 const char* subNames[4] = { "VERTEX","EDGE","FACE","SOLID" };
 TopExp_Explorer exp;
 for ( int dim = 0; dim < 4; ++dim )
 {
 exp.Init( aShape, getShapeType(&dim));
 if ( exp.More() && !aShape.IsSame( exp.Current() ))
 myDimCombo->insertItem( dim, tr( subNames[dim] ), dim );
 }
 myDimCombo->insertItem( 4, tr("WHOLE"), -1 );

 int idx = myDimCombo->findData( curDim );
 if ( idx != -1 )
 myDimCombo->setCurrentIndex( idx );
 else
 myDimCombo->setCurrentIndex( myDimCombo->count()-1-(myDimCombo->count()>1?1:0) );
 }
 myDimCombo->blockSignals( false );
 }

 //=================================================================================
 // function : activateSelection
 // purpose  : Activate local selection
 //=================================================================================
 void MeasureGUI_AnnotationDlg::activateSelection()
 {
 /*
 bool isApply = ((QPushButton*)sender() == buttonApply());
 if(!isApplyAndClose())
 erasePreview(false);

 // local selection
 if (!myShape->_is_nil() &&
 !myEditCurrentArgument &&
 myShapeIDs.size() > 1 ) // shape type is already choosen by user
 {
 GEOM_Displayer*   aDisplayer = getDisplayer();
 CORBA::String_var aMainEntry = myShape->GetStudyEntry();

 //display mode for main shape
 if ( myDmMode == -1 ) {
 SALOME_View* view = GEOM_Displayer::GetActiveView();
 if (view) {
 Handle(SALOME_InteractiveObject) io =
 new SALOME_InteractiveObject (aMainEntry.in(), "GEOM", "TEMP_IO");
 if ( view->isVisible( io ) ) {
 Handle(GEOM_AISShape) aSh = GEOMBase::ConvertIOinGEOMAISShape( io, true );
 if(!aSh.IsNull()) {
 myDmMode = aSh->isTopLevel() ? aSh->prevDisplayMode() : aSh->DisplayMode();
 }
 // Hide main shape, if explode on VERTEX
 if(getShapeType() != TopAbs_VERTEX) {
 aDisplayer->Erase(myShape, false, false);
 myIsHiddenMain = true;
 }
 }
 else
 myDmMode = SUIT_Session::session()->resourceMgr()->integerValue( "Geometry", "display_mode" );
 }
 }
 aDisplayer->SetDisplayMode(myDmMode);

 // Mantis issue 0021421: do not hide main shape, if explode on VERTEX
 if (getShapeType() == TopAbs_VERTEX) {
 if (myIsHiddenMain)
 aDisplayer->Display(myShape);
 }
 //aDisplayer->Erase(myAnnotation, false, false); ------- NOR YET implemented

 QColor aColor = SUIT_Session::session()->resourceMgr()->colorValue( "Geometry", "editField_color" );
 Quantity_NameOfColor aCol = SalomeApp_Tools::color( aColor ).Name();

 if(!isApplyAndClose()) {
 SUIT_ViewWindow* aViewWindow = 0;
 SUIT_Study* activeStudy = SUIT_Session::session()->activeApplication()->activeStudy();
 if (activeStudy)
 aViewWindow = SUIT_Session::session()->activeApplication()->desktop()->activeWindow();
 if (aViewWindow == 0) return;

 SUIT_ViewManager* aViewManager = aViewWindow->getViewManager();
 if (aViewManager->getType() != OCCViewer_Viewer::Type() &&
 aViewManager->getType() != SVTK_Viewer::Type())
 return;

 SUIT_ViewModel* aViewModel = aViewManager->getViewModel();
 SALOME_View* aView = dynamic_cast<SALOME_View*>(aViewModel);
 if (aView == 0) return;

 TopoDS_Shape aMainShape = myShapeMap(1);
 for ( int i = 0; i < myShapeIDs.size(); ++i )
 {
 int index = myShapeIDs[ i ];
 TopoDS_Shape aSubShape = myShapeMap( index );
 QString anEntry = QString( "TEMP_" ) + aMainEntry.in() + QString("_%1").arg(index);
 Handle(SALOME_InteractiveObject) io =
 new SALOME_InteractiveObject(anEntry.toLatin1(), "GEOM", "TEMP_IO");
 aDisplayer->SetColor( aCol );
 SALOME_Prs* aPrs = aDisplayer->buildSubshapePresentation(aSubShape, anEntry, aView);
 if (aPrs) {
 displayPreview(aPrs, true, false); // append, do not update
 // TODO: map or delete Prs
 }
 }
 aDisplayer->UnsetDisplayMode();
 aDisplayer->UnsetColor();
 aDisplayer->UpdateViewer();
 }
 }

 globalSelection(GEOM_ALLSHAPES);
 *+/
 SelectionIntoArgument();
 }

 //=================================================================================
 // function : highlightSubShapes
 // purpose  : select objects corresponding to selected table rows
 //=================================================================================
 void MeasureGUI_AnnotationDlg::highlightSubShapes()
 {
 if (CORBA::is_nil(myShape) || !myCurStepTable )
 return;

 TColStd_MapOfInteger anIds;
 QList<int> rows = myCurStepTable->selectedRows();
 int ii = 0, nn = rows.count();
 for (; ii < nn; ii++)
 {
 int shapeNum = rows[ ii ];
 if ( shapeNum < myShapeIDs.size() )
 anIds.Add( myShapeIDs[ shapeNum ]);
 }

 SalomeApp_Application* app = myGeomGUI->getApp();
 LightApp_SelectionMgr* aSelMgr = app->selectionMgr();
 aSelMgr->clearSelected();
 if ( anIds.IsEmpty() )
 return;

 SUIT_ViewWindow* aViewWindow = 0;
 SUIT_Study* activeStudy = app->activeStudy();
 if (activeStudy)
 aViewWindow = app->desktop()->activeWindow();
 if (aViewWindow == 0) return;

 SUIT_ViewManager* aViewManager = aViewWindow->getViewManager();
 if (aViewManager->getType() != OCCViewer_Viewer::Type() &&
 aViewManager->getType() != SVTK_Viewer::Type())
 return;

 SUIT_ViewModel* aViewModel = aViewManager->getViewModel();
 SALOME_View* aView = dynamic_cast<SALOME_View*>(aViewModel);
 if (aView == 0) return;

 // TODO??: use here GEOMBase_Helper::myPreview instead of ic->DisplayedObjects()

 OCCViewer_Viewer* v3d = ((OCCViewer_ViewManager*)aViewManager)->getOCCViewer();
 Handle(AIS_InteractiveContext) ic = v3d->getAISContext();
 AIS_ListOfInteractive List;
 //ic->DisplayedObjects(List);
 ic->ObjectsInside(List); // Mantis issue 0021367

 SALOME_ListIO aSelList;

 // To highlight the selected sub-shape in Object Browser, if it's already published under the main shape
 GEOM::GEOM_ILocalOperations_var aLocOp = getGeomEngine()->GetILocalOperations(getStudyId());
 QMap<int, QString> childsMap;
 SalomeApp_Study* appStudy = dynamic_cast<SalomeApp_Study*>(app->activeStudy());
 if (appStudy) {
 _PTR(Study) aStudy = appStudy->studyDS();
 CORBA::String_var aMainEntry = myShape->GetStudyEntry();
 _PTR(SObject) aSObj = aStudy->FindObjectID( aMainEntry.in() );
 _PTR(ChildIterator) anIt = aStudy->NewChildIterator(aSObj);
 for (anIt->InitEx(true); anIt->More(); anIt->Next()) {
 GEOM::GEOM_Object_var aChild =
 GEOM::GEOM_Object::_narrow(GeometryGUI::ClientSObjectToObject(anIt->Value()));
 if (!CORBA::is_nil(aChild)) {
 int index = aLocOp->GetSubShapeIndex(myShape, aChild);
 if ( anIds.Contains( index )) {
 CORBA::String_var aChildEntry = aChild->GetStudyEntry();
 //childsMap.insert(index, aChildEntry.in());
 Handle(SALOME_InteractiveObject) tmpIO =
 new SALOME_InteractiveObject( aChildEntry.in(), "GEOM", "TEMP_IO");
 aSelList.Append(tmpIO);
 }
 }
 }
 }

 AIS_ListIteratorOfListOfInteractive ite (List);
 for (; ite.More(); ite.Next()) {
 if (ite.Value()->IsInstance(STANDARD_TYPE(GEOM_AISShape))) {
 Handle(GEOM_AISShape) aSh = Handle(GEOM_AISShape)::DownCast(ite.Value());
 if (aSh->hasIO()) {
 Handle(SALOME_InteractiveObject) anIO = aSh->getIO();
 QString anEntry = anIO->getEntry();
 int index = anEntry.lastIndexOf("_");
 anEntry.remove(0, index+1);
 int anIndex = anEntry.toInt();
 if (anIds.Contains(anIndex)) {
 aSelList.Append(anIO);
 // if (childsMap.contains (anIndex)) {
 //   Handle(SALOME_InteractiveObject) tmpIO = new SALOME_InteractiveObject(childsMap.value(anIndex).toLatin1().constData(), "GEOM", "TEMP_IO");
 //   aSelList.Append(tmpIO);
 // }
 }
 }
 }
 }
 aSelMgr->setSelectedObjects(aSelList);
 }
 */
//=================================================================================
// function : createOperation
// purpose  :
//=================================================================================
/*GEOM::GEOM_IOperations_ptr MeasureGUI_AnnotationDlg::createOperation()
 {
 return getGeomEngine()->GetIAnnotationOperations(getStudyId());
 }*/

#define RETURN_WITH_MSG(a, b) \
  if ((a)) { \
    theMessage += (b); \
    return false; \
  }

//=================================================================================
// function : isValid()
// purpose  : Verify validity of input data
//=================================================================================
bool MeasureGUI_AnnotationDlg::isValid(QString& theMessage) {
    SalomeApp_Study* study = getStudy();
    RETURN_WITH_MSG(!study, tr("GEOM_NO_STUDY"))
    RETURN_WITH_MSG(study->studyDS()->GetProperties()->IsLocked(),
            tr("GEOM_STUDY_LOCKED"))

    if (myIsCreation) {
        //RETURN_WITH_MSG(CORBA::is_nil(myShape), tr("NO_SHAPE"))
    }
    else {
        //RETURN_WITH_MSG(CORBA::is_nil(myShape), tr("NO_FIELD"))
    }

    if (getShapeType() != TopAbs_SHAPE) {
        if (myIsCreation) {
            //RETURN_WITH_MSG(myAnnotationProperties.SubShapeId >= 0, tr("NO_SUB_SHAPE"))
        }
        else {
            //RETURN_WITH_MSG(CORBA::is_nil(myShape), tr("NO_FIELD"))
        }
    }

    //QString aName = getNewObjectName().trimmed();
    //RETURN_WITH_MSG  (aName.isEmpty(), tr("EMPTY_NAME"))

    //RETURN_WITH_MSG  ( !myCurStepTable, tr("NO_VALUES"))
    return true;
}

//=================================================================================
// function : execute
// purpose  :
//=================================================================================
bool MeasureGUI_AnnotationDlg::execute() {
    QString anError;
    if (!isValid(anError))
        return false;

    Handle (GEOM_Annotation)
    aPresentation = new GEOM_Annotation();

    TopoDS_Shape aShape;
    GEOMBase::GetShape(myShape.get(), aShape);
    gp_Ax3 aShapeLCS = gp_Ax3().Transformed(aShape.Location().Transformation());
    myAnnotationProperties.ToPresentation(aPresentation, aShapeLCS);

    // add Prs to preview
    SUIT_ViewWindow* vw =
            SUIT_Session::session()->activeApplication()->desktop()->activeWindow();
    SOCC_Prs* aPrs =
            dynamic_cast<SOCC_Prs*>(((SOCC_Viewer*) (vw->getViewManager()->getViewModel()))->CreatePrs(
                    0));

    if (aPrs)
        aPrs->AddObject(aPresentation);
    GEOMBase_Helper::displayPreview(aPrs, false, true);

    /*SALOMEDS::Study_var aStudyDS = GeometryGUI::ClientStudyToStudy( getStudy()->studyDS() );
     SALOMEDS::StudyBuilder_var aBuilder = aStudyDS->NewBuilder();

     QString aName = getNewObjectName().trimmed();
     QStringList anEntryList;

     if ( myAnnotation->_is_nil() ) // create field
     {
     /*QStringList columnNames = myCurStepTable->getHeaders();
     int nbComps = columnNames.count() - 1;
     GEOM::string_array_var compNames = new GEOM::string_array();
     compNames->length( nbComps );
     for ( int iC = 0; iC < nbComps; ++iC )
     compNames[ iC ] = columnNames[ iC+1 ].toLatin1().constData();
     *+/
     GEOM::GEOM_IFieldOperations_var anOper = GEOM::GEOM_IFieldOperations::_narrow(getOperation());
     myAnnotation = anOper->CreateField( myShape,
     aName.toLatin1().constData()/*,
     GEOM::field_data_type( getDataType() ),
     CORBA::Short( getDim() ),
     compNames*+/ );
     if ( myAnnotation->_is_nil() )
     return false;

     SALOMEDS::SObject_wrap aSO =
     getGeomEngine()->AddInStudy( aStudyDS, myAnnotation, aName.toLatin1().constData(), myShape );
     if ( !aSO->_is_nil() ) {
     myAnnotation->UnRegister();
     CORBA::String_var entry = aSO->GetID();
     anEntryList << entry.in();
     }
     }
     else // update field name
     {
     myAnnotation->SetName( aName.toLatin1().constData() );

     CORBA::String_var entry = myAnnotation->GetStudyEntry();
     if ( entry.in() ) {
     SALOMEDS::SObject_wrap SO = aStudyDS->FindObjectID( entry.in() );
     if ( !SO->_is_nil() ) {
     aBuilder->SetName(SO, aName.toLatin1().constData());
     }
     }
     }

     // create / update steps
     /*QMap< int, StepTable* >::iterator i_tbl = myStepTables.begin();
     for ( ; i_tbl != myStepTables.end(); ++i_tbl )
     {
     StepTable* tbl = i_tbl.value();
     QString stepName = (tr("STEP")+" %1 %2").arg( tbl->getStepID() ).arg( tbl->getStamp() );

     GEOM::GEOM_FieldStep_var step = tbl->getStep();
     if ( step->_is_nil() )
     {
     step = myAnnotation->GetStep( tbl->getStepID() );
     if ( step->_is_nil() )
     {
     step = myAnnotation->AddStep( tbl->getStepID(), tbl->getStamp() );

     SALOMEDS::SObject_wrap aSO =
     getGeomEngine()->AddInStudy( aStudyDS, step, stepName.toLatin1().constData(), myAnnotation );
     if ( /*!myIsCreation &&*+/ !aSO->_is_nil() ) {
     step->UnRegister();
     CORBA::String_var entry = aSO->GetID();
     anEntryList << entry.in();
     }
     }
     }
     else if ( step->GetStamp() != tbl->getStamp() )
     {
     // update a stamp in the object browser
     CORBA::String_var entry = step->GetStudyEntry();
     if ( entry.in() ) {
     SALOMEDS::SObject_wrap SO = aStudyDS->FindObjectID( entry.in() );
     if ( !SO->_is_nil() )
     aBuilder->SetName( SO, stepName.toLatin1().constData() );
     }
     }

     tbl->setValues( step );

     // update the presentation if it is displayed
     CORBA::String_var aStepEntry = step->GetStudyEntry();
     Handle(SALOME_InteractiveObject) aStepIO =
     new SALOME_InteractiveObject( aStepEntry.in(), "GEOM", "TEMP_IO" );
     getDisplayer()->Redisplay( aStepIO, false, false );
     }*+/
     getDisplayer()->UpdateViewer();

     // remove steps
     if ( !myIsCreation )
     {
     /*QSet< int >::iterator stepID = myRemovedSteps.begin();
     for ( ; stepID != myRemovedSteps.end(); ++stepID )
     {
     GEOM::GEOM_FieldStep_var step = myAnnotation->GetStep( *stepID );
     if ( !step->_is_nil() )
     {
     CORBA::String_var entry = step->GetStudyEntry();
     myAnnotation->RemoveStep( *stepID );

     if ( entry.in() ) {
     SALOMEDS::SObject_wrap SO = aStudyDS->FindObjectID( entry.in() );
     if ( !SO->_is_nil() )
     aBuilder->RemoveObjectWithChildren( SO );
     }
     }
     }*+/
     }
     myRemovedSteps.clear();

     updateObjBrowser();

     if( SUIT_Application* anApp = SUIT_Session::session()->activeApplication() ) {
     LightApp_Application* aLightApp = dynamic_cast<LightApp_Application*>( anApp );
     if( aLightApp && !isDisableBrowsing() && anEntryList.count() )
     {
     aLightApp->browseObjects( anEntryList, isApplyAndClose(), isOptimizedBrowsing() );
     anApp->putInfo( QObject::tr("GEOM_PRP_DONE") );
     }
     }
     */
    return true;
}

//=================================================================================
// function : getSourceObjects
// purpose  : virtual method to get source objects
//=================================================================================
/*QList<GEOM::GeomObjPtr> MeasureGUI_AnnotationDlg::getSourceObjects()
 {
 QList<GEOM::GeomObjPtr> res;
 GEOM::GeomObjPtr aGeomObjPtr(myAnnotation->GetShape());
 res << aGeomObjPtr;
 return res;
 }*/

