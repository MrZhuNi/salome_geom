#include "CurveCreator_TreeView.h"
#include "CurveCreator_Curve.hxx"

#include <SUIT_Session.h>
#include <SUIT_ResourceMgr.h>

#include <QHeaderView>
#include <QtAlgorithms>

#define ID_SECTION -1

CurveCreator_TreeViewModel::CurveCreator_TreeViewModel( CurveCreator_Curve* theCurve, QObject* parent ) :
  QAbstractItemModel(parent), myCurve(theCurve)
{
  SUIT_ResourceMgr* aResMgr = SUIT_Session::session()->resourceMgr();
  QPixmap aSplineIcon(aResMgr->loadPixmap("GEOM", tr("ICON_CC_SPLINE")));
  QPixmap aPolylineIcon(aResMgr->loadPixmap("GEOM", tr("ICON_CC_POLYLINE")));
  QPixmap aClosedSplineIcon(aResMgr->loadPixmap("GEOM", tr("ICON_CC_CLOSED_SPLINE")));
  QPixmap aClosedPolylineIcon(aResMgr->loadPixmap("GEOM", tr("ICON_CC_CLOSED_POLYLINE")));
  QPixmap aPointIcon(aResMgr->loadPixmap("GEOM", tr("ICON_CC_POINT")));

/*  QPixmap aSplineIcon(tr(":images/ICON_SPLINE"));
  QPixmap aPolylineIcon(tr(":images/ICON_POLYLINE"));
  QPixmap aClosedPolylineIcon(tr(":images/ICON_CLOSED_POLYLINE"));
  QPixmap aClosedSplineIcon(tr(":images/ICON_CLOSED_SPLINE"));
  QPixmap aPointIcon(tr(":images/ICON_POINT")); */

  if( !aSplineIcon.isNull() )
    myCachedIcons[ICON_SPLINE] = aSplineIcon;

  if( !aPolylineIcon.isNull() )
    myCachedIcons[ICON_POLYLINE] = aPolylineIcon;

  if( !aPolylineIcon.isNull() )
    myCachedIcons[ICON_CLOSED_POLYLINE] = aClosedPolylineIcon;

  if( !aPolylineIcon.isNull() )
    myCachedIcons[ICON_CLOSED_SPLINE] = aClosedSplineIcon;

  if( !aPointIcon.isNull() )
    myCachedIcons[ICON_POINT] = aPointIcon;

}

int	CurveCreator_TreeViewModel::columnCount(const QModelIndex & parent ) const
{
  if( parent.internalId() == ID_SECTION )
    return 1;
  else
    return 1;
}

QVariant	CurveCreator_TreeViewModel::data(const QModelIndex & index, int role ) const
{
  int aRow = index.row();
  int aColumn = index.column();
  if( myCurve ){
    if( index.internalId() == ID_SECTION ){
      if( role == Qt::DisplayRole ){
        if( aColumn == 0 )
          return QString::fromStdString(myCurve->getSectionName(aRow));
        return QVariant();
      }
      else if( role == Qt::DecorationRole ){
        if( aColumn == 0 ){
          CurveCreator::Type aSectionType = myCurve->getType(aRow);
          if( aSectionType == CurveCreator::Polyline ){
            if( myCurve->isClosed(aRow) ){
              return myCachedIcons[ICON_CLOSED_POLYLINE];
            }
            else{
              return myCachedIcons[ICON_POLYLINE];
            }
          }
          else{
            if( myCurve->isClosed(aRow) ){
              return myCachedIcons[ICON_CLOSED_SPLINE];
            }
            else{
              return myCachedIcons[ICON_SPLINE];
            }
          }
        }
      }
    }
    else{
      if( role == Qt::DisplayRole ){
        if( aColumn == 1 )
          return QVariant();
        //                    return "Point";
        else if( aColumn == 0 ){
          CurveCreator::Coordinates aCoords = myCurve->getCoordinates(index.internalId(),index.row() );
          QString anOut;
          if( myCurve->getDimension() == CurveCreator::Dim2d ){
            anOut = QString(tr("X=%1, Y=%2")).arg(aCoords[0]).arg(aCoords[1]);
          }
          else{
            anOut = QString(tr("X=%1, Y=%2, Z=%3")).arg(aCoords[0]).arg(aCoords[1]).arg(aCoords[2]);
          }
          return anOut;
        }
      }
      else if( role == Qt::DecorationRole ){
        if( aColumn == 0 ){
          return myCachedIcons[ICON_POINT];
        }
      }
    }
  }
  return QVariant();
}

QModelIndex	CurveCreator_TreeViewModel::index(int row, int column, const QModelIndex & parent ) const
{
  if( parent.isValid() ){
    return createIndex(row, column, parent.row() );
  }
  else{
    QModelIndex aParent = createIndex(row, column, ID_SECTION );
    return aParent;
  }
  return QModelIndex();
}

QModelIndex	CurveCreator_TreeViewModel::parent(const QModelIndex & theIndex) const
{
  if( !theIndex.isValid() )
    return QModelIndex();

  if( theIndex.internalId() == ID_SECTION ){
    return QModelIndex();
  }
  return createIndex( theIndex.internalId(), 0, ID_SECTION  );
}

int	CurveCreator_TreeViewModel::rowCount(const QModelIndex & parent ) const
{
  int aRowCnt = 0;
  if( myCurve != NULL ){
    if( !parent.isValid() ){
      //Points level
      aRowCnt =  myCurve->getNbSections();
    }
    else{
      //Section level
      if( parent.internalId() == ID_SECTION ){
        aRowCnt = myCurve->getNbPoints(parent.row());
      }
    }
  }
  return aRowCnt;
}

QModelIndex CurveCreator_TreeViewModel::sectionIndex( int theSection ) const
{
  return createIndex( theSection, 0, ID_SECTION );
}

QModelIndex CurveCreator_TreeViewModel::pointIndex( int theSection, int thePoint ) const
{
  return createIndex( thePoint, 0, theSection );
}

bool CurveCreator_TreeViewModel::isSection( const QModelIndex& theIndx ) const
{
  if( theIndx.internalId() == ID_SECTION )
    return true;
  return false;
}

int CurveCreator_TreeViewModel::getSection( const QModelIndex& theIndx ) const
{
  if( theIndx.internalId() == ID_SECTION )
    return theIndx.row();
  return theIndx.internalId();
}

int CurveCreator_TreeViewModel::getPoint( const QModelIndex& theIndx ) const
{
  if( theIndx.internalId() == ID_SECTION )
    return -1;
  return theIndx.row();
}

void CurveCreator_TreeViewModel::setCurve( CurveCreator_Curve* theCurve )
{
  myCurve = theCurve;
  reset();
}

/*****************************************************************************************/
CurveCreator_TreeView::CurveCreator_TreeView( CurveCreator_Curve* theCurve, QWidget *parent) :
  QTreeView(parent)
{
  header()->hide();
  setUniformRowHeights(true);
  setContextMenuPolicy( Qt::CustomContextMenu );
  CurveCreator_TreeViewModel* aModel = new CurveCreator_TreeViewModel(theCurve, this);
  setModel(aModel);
  setSelectionBehavior(SelectRows);
  setSelectionMode(ExtendedSelection);
  setExpandsOnDoubleClick(false);
  connect( selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SIGNAL(selectionChanged()) );
  connect( this, SIGNAL(activated(QModelIndex)), this, SLOT(onActivated(QModelIndex)));
}

QList<int> CurveCreator_TreeView::getSelectedSections() const
{
  QList<int> aSect;
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( !aModel )
    return aSect;
  QModelIndexList anIndxs = selectionModel()->selectedIndexes();
  for( int i = 0 ; i < anIndxs.size() ; i++ ){
    if( aModel->isSection(anIndxs[i]) ){
      aSect << aModel->getSection( anIndxs[i] );
    }
  }
  qSort(aSect.begin(), aSect.end());
  return aSect;
}

void CurveCreator_TreeView::pointsAdded( int theSection, int thePoint, int thePointsCnt )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    QModelIndex aSectIndx = aModel->sectionIndex( theSection );
    rowsInserted(aSectIndx, thePoint, thePoint + thePointsCnt - 1 );
    expand( aSectIndx );
  }
}

void CurveCreator_TreeView::pointDataChanged( int theSection, int thePoint )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    QModelIndex aPointIndx = aModel->pointIndex( theSection, thePoint );
    dataChanged( aPointIndx, aPointIndx );
  }
}

void CurveCreator_TreeView::pointsRemoved( int theSection, int thePoint, int thePointsCnt )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    for( int i = 0 ; i < thePointsCnt ; i++ ){
      QModelIndex aSectIndx = aModel->pointIndex(theSection, thePoint + i);
      selectionModel()->select(aSectIndx,QItemSelectionModel::Deselect);
    }
    QModelIndex aSectIndx = aModel->sectionIndex( theSection );
    rowsRemoved(aSectIndx, thePoint, thePoint + thePointsCnt - 1 );
  }
}

void CurveCreator_TreeView::sectionAdded( int theSection )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    rowsInserted(QModelIndex(), theSection, theSection );
  }
}

void CurveCreator_TreeView::sectionChanged( int theSection, int aSectCnt )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    QModelIndex aFirstSectIndx = aModel->sectionIndex( theSection );
    QModelIndex aLastSectIndx = aModel->sectionIndex( theSection + aSectCnt - 1);
    dataChanged( aFirstSectIndx, aLastSectIndx );
  }
}

void CurveCreator_TreeView::sectionsRemoved( int theSection, int theSectionCnt )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    for( int i = 0 ; i < theSectionCnt ; i++ ){
      QModelIndex aSectIndx = aModel->sectionIndex(theSection + i);
      this->selectionModel()->select(aSectIndx,QItemSelectionModel::Deselect);
    }
    rowsRemoved( QModelIndex(), theSection, theSection+theSectionCnt-1 );
  }
}

void CurveCreator_TreeView::setIndexState( const QModelIndex& theIndx, bool& isExpanded, bool& isSelected, bool& isCurrent )
{
  setExpanded( theIndx, isExpanded );
  QItemSelectionModel::SelectionFlags aFlag = QItemSelectionModel::Select;
  if( !isSelected ){
    aFlag = QItemSelectionModel::Deselect;
  }
  selectionModel()->select( theIndx, aFlag );
}

void CurveCreator_TreeView::getIndexInfo( const QModelIndex& theIndx, bool& isExpand, bool& isSelected, bool& isCurrent )
{
  isExpand = isExpanded(theIndx);
  isSelected = selectionModel()->isSelected(theIndx);
  isCurrent = (theIndx == selectionModel()->currentIndex());
}

void CurveCreator_TreeView::swapIndexes( const QModelIndex& theFirst, const QModelIndex& theSecond )
{
  bool isFirstSelected;
  bool isFirstExpanded;
  bool isFirstCurrent;
  getIndexInfo( theFirst, isFirstExpanded, isFirstSelected, isFirstCurrent );

  bool isSecondSelected;
  bool isSecondExpanded;
  bool isSecondCurrent;
  getIndexInfo( theSecond, isSecondExpanded, isSecondSelected, isSecondCurrent );

  setIndexState( theFirst, isSecondExpanded, isSecondSelected, isSecondCurrent );
  setIndexState( theSecond, isFirstExpanded, isFirstSelected, isFirstCurrent );
  dataChanged(theFirst,theFirst);
  dataChanged(theSecond,theSecond);
}

void CurveCreator_TreeView::sectionsSwapped( int theSection, int theOffset )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    QModelIndex aFirstIndex = aModel->sectionIndex( theSection );
    QModelIndex aSecondIndex = aModel->sectionIndex( theSection + theOffset );
    swapIndexes( aFirstIndex, aSecondIndex );
  }
}

void CurveCreator_TreeView::pointsSwapped( int theSection, int thePointNum, int theOffset )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    QModelIndex aFirstIndex = aModel->pointIndex( theSection, thePointNum );
    QModelIndex aSecondIndex = aModel->pointIndex( theSection, thePointNum + theOffset );
    swapIndexes( aFirstIndex, aSecondIndex );
  }
}

void CurveCreator_TreeView::setSelectedSections( const QList<int>& theList )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    selectionModel()->clearSelection();
    for( int i = 0 ; i < theList.size() ; i++ ){
      QModelIndex aSectIndx = aModel->sectionIndex(theList[i]);
      selectionModel()->select(aSectIndx, QItemSelectionModel::Select );
    }
  }
}

void CurveCreator_TreeView::setSelectedPoints( const QList< QPair<int, int> >& thePointsList )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel ){
    selectionModel()->clearSelection();
    for( int i = 0 ; i < thePointsList.size() ; i++ ){
      QModelIndex aSectIndx = aModel->pointIndex( thePointsList[i].first, thePointsList[i].second );
      selectionModel()->select(aSectIndx, QItemSelectionModel::Select );
    }
  }
}

bool pointLessThan(const QPair<int,int> &s1, const QPair<int,int> &s2)
{
  if( s1.first < s2.first )
    return true;
  if( s1.first > s2.first )
    return false;
  return s1.second < s2.second;
}

QList< QPair< int, int > > CurveCreator_TreeView::getSelectedPoints() const
{
  QList< QPair< int, int > > aPoints;
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( !aModel )
    return aPoints;
  QModelIndexList anIndxs = selectionModel()->selectedIndexes();
  for( int i = 0 ; i < anIndxs.size() ; i++ ){
    if( !aModel->isSection( anIndxs[i] ) ){
      int aSect = aModel->getSection(anIndxs[i]);
      int aPointNum = aModel->getPoint(anIndxs[i]);
      QPair< int, int > aPoint = QPair<int,int>( aSect, aPointNum );
      aPoints.push_back( aPoint );
    }
  }
  qSort( aPoints.begin(), aPoints.end(), pointLessThan );
  return aPoints;
}

CurveCreator_TreeView::SelectionType CurveCreator_TreeView::getSelectionType() const
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( !aModel )
    return ST_NOSEL;
  bool isPointSel = false;
  bool isSectSel = false;
  bool isOneSection = true;
  int aSectNum = -1;
  QModelIndexList aLst = selectionModel()->selectedIndexes();
  for( int i = 0 ; i < aLst.size() ; i++ ){
    if( aModel->isSection( aLst[i] ) ){
      isSectSel = true;
    }
    else{
      isPointSel = true;
      if( aSectNum == -1 ){
        aSectNum = aModel->getSection(aLst[i]);
      }
      else{
        if( aSectNum != aModel->getSection( aLst[i] ) ){
          isOneSection = false;
        }
      }
    }
  }
  if( isSectSel && !isPointSel )
    return ST_SECTIONS;
  if( isPointSel && !isSectSel ){
    if( isOneSection ){
      return ST_POINTS_ONE_SECTION;
    }
    return ST_POINTS;
  }
  if( isPointSel && isSectSel )
    return ST_MIXED;
  return ST_NOSEL;
}

void CurveCreator_TreeView::onActivated( QModelIndex theIndx )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( !aModel )
    return;
  int aSect = aModel->getSection(theIndx);
  if( aModel->isSection(theIndx) ){
    emit sectionEntered( aSect );
    return;
  }
  int aPointNum = aModel->getPoint( theIndx );
  emit pointEntered( aSect, aPointNum );
}

void CurveCreator_TreeView::setCurve( CurveCreator_Curve* theCurve )
{
  CurveCreator_TreeViewModel* aModel = dynamic_cast<CurveCreator_TreeViewModel*>(model());
  if( aModel )
    aModel->setCurve(theCurve);
  reset();
}
