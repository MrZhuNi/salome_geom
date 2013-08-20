#include "CurveCreator_Widget.h"
#include "CurveCreator_TreeView.h"
#include "QVBoxLayout"
#include "CurveCreator_Curve.hxx"
#include "CurveCreator_CurveEditor.hxx"
#include "CurveCreator.hxx"
#include "CurveCreator_NewPointDlg.h"
#include "CurveCreator_NewSectionDlg.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QToolButton>
#include <QToolBar>
#include <QAction>
#include <QMenu>

CurveCreator_Widget::CurveCreator_Widget(QWidget *parent) :
    QWidget(parent), myNewPointEditor(NULL)
{
//TODO remove it for debug only
    myCurve = new CurveCreator_Curve(CurveCreator::Dim2d);
    myEdit = new CurveCreator_CurveEditor( myCurve );
    CurveCreator::Coordinates aCoords;

    aCoords.push_back(10);
    aCoords.push_back(20);
    aCoords.push_back(1);
    aCoords.push_back(2);

    myEdit->addSection("",CurveCreator::BSpline, true, aCoords );

    aCoords.clear();
    aCoords.push_back(11);
    aCoords.push_back(21);
    aCoords.push_back(111);
    aCoords.push_back(211);
    aCoords.push_back(101);
    aCoords.push_back(201);
    aCoords.push_back(13);
    aCoords.push_back(25);
    myEdit->addSection("",CurveCreator::Polyline, false, aCoords );

    aCoords.clear();
    for( int i = 0 ; i < 1000000 ; i++ ){
        double anX = ((double)i)/10000.;
        double anY = ((double)i)/10000. + 3.4;
        aCoords.push_back(anX);
        aCoords.push_back(anY);
    }
    myEdit->addSection("",CurveCreator::Polyline, true, aCoords );

//TODO end debug
    myNewPointEditor = new CurveCreator_NewPointDlg(myCurve->getDimension(), this);
    connect( myNewPointEditor, SIGNAL(addPoint()), this, SLOT(onAddNewPoint()));

    myNewSectionEditor = new CurveCreator_NewSectionDlg(this);
    connect( myNewSectionEditor, SIGNAL(addSection()), this, SLOT(onAddNewSection()));

    QHBoxLayout* aNameLayout = new QHBoxLayout();
    QLabel* aNameLabel = new QLabel(tr("CURVE_NAME_TLT"), this);
    aNameLayout->addWidget(aNameLabel);
    QLineEdit* aNameEdit = new QLineEdit(this);
    aNameLayout->addWidget(aNameEdit);

    QGroupBox* aSectionGroup = new QGroupBox(tr("SECTION_GROUP_TLT"),this);

    mySectionView = new CurveCreator_TreeView(myCurve, aSectionGroup);
    connect( mySectionView, SIGNAL(selectionChanged()), this, SLOT( onSelectionChanged() ) );
    connect( mySectionView, SIGNAL(pointEntered(int,int)), this, SLOT(onEditPoint(int,int)) );
    connect( mySectionView, SIGNAL(sectionEntered(int)), this, SLOT(onEditSection(int)) );
    connect( mySectionView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)) );
    QToolBar* aTB = new QToolBar(tr("TOOL_BAR_TLT"), aSectionGroup);
//    QToolButton* anUndoBtn = new QToolButton(aTB);
    QAction* anAct = createAction( UNDO_ID, tr("UNDO"), tr(":images/ICON_UNDO"), tr("UNDO_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_Z) );
    aTB->addAction(anAct);

    anAct = createAction( REDO_ID, tr("REDO"), tr(":images/ICON_REDO"), tr("REDO_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_Y) );
    aTB->addAction(anAct);

    aTB->addSeparator();

    anAct = createAction( NEW_SECTION_ID, tr("ICON_NEW_SECTION"), tr(":images/ICON_NEW_SECTION"), tr("NEW_SECTION_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_N) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onNewSection()) );
    aTB->addAction(anAct);

    anAct = createAction( INSERT_SECTION_BEFORE_ID, tr("INSERT_SECTION_BEFORE"), tr(":images/ICON_INSERT_SECTION_BEFORE"), tr("INSERT_SECTION_BEFORE_TLT"),
                          QKeySequence(Qt::ControlModifier | Qt::Key_Insert ) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onInsertSectionBefore()) );

    anAct = createAction( INSERT_SECTION_AFTER_ID, tr("INSERT_SECTION_AFTER"), tr(":images/ICON_INSERT_SECTION_AFTER"), tr("INSERT_SECTION_AFTER_TLT"),
                          QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_Insert ) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onInsertSectionAfter()) );

    anAct = createAction( NEW_POINT_ID, tr("ICON_NEW_POINT"), tr(":images/ICON_NEW_POINT"), tr("NEW_POINT_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_P) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onNewPoint()) );
    aTB->addAction(anAct);
    aTB->addSeparator();

    anAct = createAction( INSERT_POINT_BEFORE_ID, tr("INSERT_POINT_BEFORE"), tr(":images/ICON_POINT_BEFORE"), tr("INSERT_POINT_BEFORE_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_B) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onInsertPointBefore()) );

    anAct = createAction( INSERT_POINT_AFTER_ID, tr("INSERT_POINT_AFTER"), tr(":images/ICON_POINT_AFTER"), tr("INSERT_POINT_AFTER_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_M) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onInsertPointAfter()) );

    anAct = createAction( CLOSE_SECTIONS_ID, tr("CLOSE_SECTIONS"), tr(":images/ICON_CLOSE_SECTIONS"), tr("CLOSE_SECTIONS_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_W) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onCloseSections()) );

    anAct = createAction( UNCLOSE_SECTIONS_ID, tr("UNCLOSE_SECTIONS"), tr(":images/ICON_UNCLOSE_SECTIONS"), tr("UNCLOSE_SECTIONS_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_S) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onUncloseSections()) );

    anAct = createAction( SET_SECTIONS_POLYLINE_ID, tr("SET_SECTIONS_POLYLINE"), tr(":images/ICON_POLYLINE"), tr("SET_POLYLINE_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_E) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onSetPolyline()) );

    anAct = createAction( SET_SECTIONS_SPLINE_ID, tr("SET_SECTIONS_SPLINE"), tr(":images/ICON_SPLINE"), tr("SET_SPLINE_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_R) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onSetSpline()) );

    anAct = createAction( REMOVE_ID, tr("ICON_REMOVE"), tr(":images/ICON_REMOVE"), tr("REMOVE_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_Delete ) );
    connect(anAct, SIGNAL(triggered()), this, SLOT(onRemove()) );
    aTB->addAction(anAct);
    aTB->addSeparator();

    anAct = createAction( JOIN_ID, tr("ICON_JOIN"), tr(":images/ICON_JOIN"), tr("JOIN_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_Plus ) );
    connect( anAct, SIGNAL(triggered()), this, SLOT(onJoin()) );
    aTB->addAction(anAct);
    aTB->addSeparator();

    anAct = createAction( UP_ID, tr("ICON_STEP_UP"), tr(":images/ICON_STEP_UP"), tr("STEP_UP_TLT"), QKeySequence(Qt::ControlModifier|Qt::Key_Up ) );
    connect( anAct, SIGNAL(triggered()), this, SLOT(onMoveUp()) );
    aTB->addAction(anAct);

    anAct = createAction( DOWN_ID, tr("ICON_STEP_DOWN"), tr(":images/ICON_STEP_DOWN"), tr("STEP_DOWN"), QKeySequence(Qt::ControlModifier|Qt::Key_Down ) );
    connect( anAct, SIGNAL(triggered()), this, SLOT(onMoveDown()) );
    aTB->addAction(anAct);

    anAct = createAction( CLEAR_ALL_ID, tr("CLEAR_ALL"), tr(":images/ICON_CLEAR_ALL"), tr("CLEAR_ALL_TLT"), QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_Delete ) );
    connect( anAct, SIGNAL(triggered()), this, SLOT( onClearAll()) );
    aTB->addAction(anAct);

    anAct = createAction( JOIN_ALL_ID, tr("JOIN_ALL"), tr(":images/ICON_JOIN_ALL"), tr("JOIN_ALL_TLT"), QKeySequence(Qt::ControlModifier | Qt::ShiftModifier | Qt::Key_Plus ) );
    connect( anAct, SIGNAL(triggered()), this, SLOT(onJoinAll()) );
    aTB->addAction(anAct);

    QVBoxLayout* aSectLayout = new QVBoxLayout();
    aSectLayout->addWidget(aTB);
    aSectLayout->addWidget(mySectionView);
    aSectionGroup->setLayout(aSectLayout);
    QVBoxLayout* aLay = new QVBoxLayout();
    aLay->addLayout(aNameLayout);
    aLay->addWidget(aSectionGroup);
    setLayout(aLay);
    onSelectionChanged();
}

void CurveCreator_Widget::onSelectionChanged()
{
    QList<ActionId> anEnabledAct;
    anEnabledAct << NEW_SECTION_ID;
    int aSectCnt = myCurve->getNbPoints();
    if( aSectCnt > 0 )
        anEnabledAct << CLEAR_ALL_ID;
    if( aSectCnt > 1 )
        anEnabledAct << JOIN_ALL_ID;
    QList<int> aSelSections = mySectionView->getSelectedSections();
    QList< QPair< int, int > > aSelPoints = mySectionView->getSelectedPoints();
    CurveCreator_TreeView::SelectionType aSelType = mySectionView->getSelectionType();
    switch( aSelType ){
    case CurveCreator_TreeView::ST_NOSEL:{
        break;
    }
    case CurveCreator_TreeView::ST_SECTIONS:{
        if( aSelSections.size() > 1 ){
            anEnabledAct << JOIN_ID;
        }
        if( aSelSections[0] > 0 ){
            anEnabledAct << UP_ID;
        }
        if( aSelSections.size() == 1 ){
            anEnabledAct << NEW_POINT_ID << INSERT_SECTION_BEFORE_ID << INSERT_SECTION_AFTER_ID;
        }
        if( aSelSections[ aSelSections.size() - 1 ] < ( myCurve->getNbSections() - 1 ) ){
            anEnabledAct << DOWN_ID;
        }
        anEnabledAct << CLOSE_SECTIONS_ID << UNCLOSE_SECTIONS_ID << SET_SECTIONS_POLYLINE_ID << SET_SECTIONS_SPLINE_ID;
        break;
    }
    case CurveCreator_TreeView::ST_POINTS_ONE_SECTION:{
        if( aSelPoints[0].second > 0 ){
            anEnabledAct << UP_ID;
        }
        int aLastIndex = aSelPoints.size()-1;
        int aSect = aSelPoints[0].first;
        if( aSelPoints[aLastIndex].second < (myCurve->getNbPoints(aSect) - 1)){
            anEnabledAct << DOWN_ID;
        }
        if( aSelPoints.size() == 1){
            anEnabledAct << INSERT_POINT_BEFORE_ID << INSERT_POINT_AFTER_ID;
        }
        break;
    }
    }
    int aSelObjsCnt = aSelPoints.size() + aSelSections.size();
    if( aSelObjsCnt > 0 ){
        anEnabledAct << REMOVE_ID;
    }
    if( myCurve->getNbSections() > 0 ){
        anEnabledAct << REMOVE_ALL_ID;
    }
    if( myCurve->getNbSections() > 1 ){
        anEnabledAct << JOIN_ALL_ID;
    }
    QList<ActionId> anIds = myActionMap.keys();
    for( int i = 0 ; i < anIds.size() ; i++ ){
        if( myActionMap.contains(anIds[i]) ){
            if( anEnabledAct.contains(anIds[i]) ){
                myActionMap[anIds[i]]->setEnabled(true);
            }
            else{
                myActionMap[anIds[i]]->setEnabled(false);
            }
        }
    }
}

void CurveCreator_Widget::onNewPoint()
{
    mySection= -1;
    myPointNum = -1;
    QList<int> aSelSection = mySectionView->getSelectedSections();
    if( aSelSection.size() > 0 ){
        mySection = aSelSection[0];
    }
    else{
        QList< QPair<int,int> > aSelPoints = mySectionView->getSelectedPoints();
        if( aSelPoints.size() > 0 ){
            mySection = aSelPoints[0].first;
            myPointNum = aSelPoints[0].second + 1;
        }
    }
    QString aSectName;
    if( mySection < 0 ){
        mySection = myCurve->getNbSections() - 1;
    }
    aSectName = QString::fromStdString( myCurve->getSectionName(mySection));
    if( myPointNum < 0 ){
        myPointNum = myCurve->getNbPoints(mySection);
    }
    myNewPointEditor->clear();
    myNewPointEditor->setEditMode(false);
    myNewPointEditor->setSectionName(aSectName);
    if( myNewPointEditor->exec() == QDialog::Accepted ){
        onAddNewPoint();
    }
}

void CurveCreator_Widget::onAddNewPoint()
{
    CurveCreator::Coordinates aCoords = myNewPointEditor->getCoordinates();
    myEdit->insertPoints(aCoords, mySection, myPointNum );
    mySectionView->pointsAdded( mySection, myPointNum );
    myNewPointEditor->clear();
    myPointNum++;
}

void CurveCreator_Widget::onNewSection()
{
    myNewSectionEditor->clear();
    myNewSectionEditor->setEditMode(false);
    QString aSectName = QString( myCurve->getUnicSectionName().c_str() );
    myNewSectionEditor->setSectionParameters(aSectName, true, CurveCreator::Polyline );
    if( myNewSectionEditor->exec() == QDialog::Accepted ){
        onAddNewSection();
    }
}

void CurveCreator_Widget::onAddNewSection()
{
    CurveCreator::Coordinates aCoords;
    myEdit->addSection( myNewSectionEditor->getName().toStdString(), myNewSectionEditor->getSectionType(),
                        myNewSectionEditor->isClosed(), aCoords  );
    mySectionView->sectionAdded( mySection );
    QString aNewName = QString(myCurve->getUnicSectionName().c_str());
    myNewSectionEditor->setSectionName(aNewName);
    mySection++;
}

QAction* CurveCreator_Widget::createAction( ActionId theId, const QString& theName, const QString& theImageName,
                                            const QString& theToolTip, const QKeySequence& theShortcut )
{
    QAction* anAct = new QAction(theName,this);
    QIcon anIcon(theImageName);
    if( !anIcon.isNull() ){
        anAct->setIcon(anIcon);
    }
    anAct->setShortcut(theShortcut);
    anAct->setToolTip(theToolTip);
    myActionMap[theId] = anAct;
    return anAct;
}

QAction* CurveCreator_Widget::getAction(ActionId theId)
{
    if( myActionMap.contains(theId) )
        return myActionMap[theId];
    return NULL;
}

void CurveCreator_Widget::onEditSection( int theSection )
{
    mySection = theSection;
    QString aSectName = QString::fromStdString( myCurve->getSectionName(theSection));
    bool isClosed = myCurve->isClosed(theSection);
    CurveCreator::Type aType = myCurve->getType(theSection);
    myNewSectionEditor->setEditMode(true);
    myNewSectionEditor->setSectionParameters( aSectName, isClosed, aType );
    if( myNewSectionEditor->exec() == QDialog::Accepted ){
        QString aName = myNewSectionEditor->getName();
        bool    isClosed = myNewSectionEditor->isClosed();
        CurveCreator::Type aSectType = myNewSectionEditor->getSectionType();
        myEdit->setClosed( isClosed, mySection );
        myEdit->setName( aName.toStdString(), mySection );
        myEdit->setType( aSectType, mySection );
        mySectionView->sectionChanged(mySection);
    }
}

void CurveCreator_Widget::onEditPoint( int theSection, int thePoint )
{
    if( !myNewPointEditor ){
    }
    QString aSectName = QString::fromStdString( myCurve->getSectionName(theSection));
    myNewPointEditor->setEditMode(true);
    myNewPointEditor->setSectionName(aSectName);
    CurveCreator::Coordinates aCoords = myCurve->getCoordinates(theSection,thePoint);
    myNewPointEditor->setCoordinates(aCoords);
    if( myNewPointEditor->exec() == QDialog::Accepted ){
        aCoords = myNewPointEditor->getCoordinates();
        myEdit->setCoordinates(aCoords, theSection, thePoint);
        mySectionView->pointDataChanged(theSection, thePoint );
    }
}

void CurveCreator_Widget::onJoin()
{
    QList<int> aSections = mySectionView->getSelectedSections();
    if( aSections.size() == 0 ){
        return;
    }
    int aMainSect = aSections[0];
    int aMainSectSize = myCurve->getNbPoints(aMainSect);
    for( int i = 1 ; i < aSections.size() ; i++ ){
        int aSectNum = aSections[i] - (i-1);
        myEdit->join( aMainSect, aSectNum );
        mySectionView->sectionsRemoved( aSectNum );
    }
    int aNewSectSize = myCurve->getNbPoints(aMainSect);
    if( aNewSectSize != aMainSectSize )
        mySectionView->pointsAdded( aMainSect, aMainSectSize, aNewSectSize-aMainSectSize );
}

void CurveCreator_Widget::onRemove()
{
    QList< QPair<int,int> > aSelPoints = mySectionView->getSelectedPoints();
    int aCurrSect=-1;
    int aRemoveCnt = 0;
    for( int i = 0 ; i < aSelPoints.size() ; i++ ){
        if( aCurrSect != aSelPoints[i].first ){
            aRemoveCnt = 0;
            aCurrSect = aSelPoints[i].first;
        }
        int aPntIndx = aSelPoints[i].second - aRemoveCnt;
        myEdit->removePoints(aCurrSect,aPntIndx, 1);
        mySectionView->pointsRemoved( aCurrSect, aPntIndx );
        aRemoveCnt++;
    }
    QList<int> aSections = mySectionView->getSelectedSections();
    for( int i = 0 ; i < aSections.size() ; i++ ){
        int aSectNum = aSections[i] - (i);
        myEdit->removeSection( aSectNum );
        mySectionView->sectionsRemoved( aSectNum );
    }
    mySectionView->clearSelection();
}

void CurveCreator_Widget::onMoveUp()
{
    if( mySectionView->getSelectionType() == CurveCreator_TreeView::ST_SECTIONS ){
//Move sections
        QList<int> aSections = mySectionView->getSelectedSections();
        for( int i = 0 ; i < aSections.size() ; i++ ){
            int anIndx = aSections[i];
            myEdit->moveSection( anIndx, anIndx-1);
            mySectionView->sectionsSwapped( anIndx, -1 );
        }
    }
    else{
//Move points
        QList< QPair<int,int> > aPoints = mySectionView->getSelectedPoints();
        for( int i = 0 ; i < aPoints.size() ; i++ ){
            int aSection = aPoints[i].first;
            int aPoint = aPoints[i].second;
            myEdit->movePoint(aSection, aPoint, aPoint-2);
            mySectionView->pointsSwapped( aSection, aPoint, -1 );
        }
    }
}

void CurveCreator_Widget::onMoveDown()
{
    if( mySectionView->getSelectionType() == CurveCreator_TreeView::ST_SECTIONS ){
//Move sections
        QList<int> aSections = mySectionView->getSelectedSections();
        for( int i = aSections.size()-1 ; i >=0 ; i-- ){
            int anIndx = aSections[i];
            myEdit->moveSection( anIndx, anIndx+1);
            mySectionView->sectionsSwapped( anIndx, 1 );
        }
    }
    else{
//Move points
        QList< QPair<int,int> > aPoints = mySectionView->getSelectedPoints();
        for( int i = aPoints.size() - 1; i >= 0 ; i--  ){
            int aSection = aPoints[i].first;
            int aPoint = aPoints[i].second;
            myEdit->movePoint(aSection, aPoint, aPoint+1);
            mySectionView->pointsSwapped( aSection, aPoint, 1 );
        }
    }
}

void CurveCreator_Widget::onClearAll()
{
    myEdit->clear();
    mySectionView->reset();
    onSelectionChanged();
}

void CurveCreator_Widget::onJoinAll()
{
    myEdit->join();
    mySectionView->reset();
    onSelectionChanged();
}

void CurveCreator_Widget::onInsertSectionBefore()
{

}

void CurveCreator_Widget::onInsertSectionAfter()
{

}

void CurveCreator_Widget::onInsertPointBefore()
{

}

void CurveCreator_Widget::onInsertPointAfter()
{

}

void CurveCreator_Widget::onUndoSettings()
{

}

void CurveCreator_Widget::onSetSpline()
{
    QList<int> aSelSections = mySectionView->getSelectedSections();
    for( int i = 0 ; i < aSelSections.size() ; i++ ){
        myEdit->setType(CurveCreator::BSpline, aSelSections[i]);
        mySectionView->sectionChanged(aSelSections[i]);
    }
}

void CurveCreator_Widget::onSetPolyline()
{
    QList<int> aSelSections = mySectionView->getSelectedSections();
    for( int i = 0 ; i < aSelSections.size() ; i++ ){
        myEdit->setType(CurveCreator::Polyline, aSelSections[i]);
        mySectionView->sectionChanged(aSelSections[i]);
    }
}

void CurveCreator_Widget::onCloseSections()
{
    QList<int> aSelSections = mySectionView->getSelectedSections();
    for( int i = 0 ; i < aSelSections.size() ; i++ ){
        myEdit->setClosed(true, aSelSections[i]);
        mySectionView->sectionChanged(aSelSections[i]);
    }
}

void CurveCreator_Widget::onUncloseSections()
{
    QList<int> aSelSections = mySectionView->getSelectedSections();
    for( int i = 0 ; i < aSelSections.size() ; i++ ){
        myEdit->setClosed(false, aSelSections[i]);
        mySectionView->sectionChanged(aSelSections[i]);
    }
}

void CurveCreator_Widget::onContextMenu( QPoint thePoint )
{
    QList<ActionId> aContextActions;
    aContextActions << CLEAR_ALL_ID << JOIN_ALL_ID << SEPARATOR_ID <<
                       INSERT_SECTION_BEFORE_ID << INSERT_SECTION_AFTER_ID << SEPARATOR_ID <<
                       CLOSE_SECTIONS_ID << UNCLOSE_SECTIONS_ID << SET_SECTIONS_POLYLINE_ID <<
                       SET_SECTIONS_SPLINE_ID << SEPARATOR_ID <<
                       INSERT_POINT_BEFORE_ID << INSERT_POINT_AFTER_ID;
    QPoint aGlPoint = mySectionView->mapToGlobal(thePoint);
    bool isVis = false;
    QList<ActionId> aResAct;
    for( int i = 0 ; i < aContextActions.size() ; i++ ){
        if( aContextActions[i] != SEPARATOR_ID ){
            if( myActionMap.contains(aContextActions[i]) ){
                QAction* anAct = myActionMap[aContextActions[i]];
                if( anAct->isEnabled() ){
                    aResAct << aContextActions[i];
                    isVis = true;
                }
            }
        }
        else{
            aResAct << SEPARATOR_ID;
        }
    }
    if( !isVis )
        return;

    QMenu* aMenu = new QMenu(this);
    for( int i = 0 ; i < aResAct.size() ; i++ ){
        if( aResAct[i] == SEPARATOR_ID ){
            aMenu->addSeparator();
        }
        else{
            QAction* anAct = myActionMap[aResAct[i]];
            aMenu->insertAction(NULL, anAct);
        }
    }
    aMenu->exec(aGlPoint);
}
