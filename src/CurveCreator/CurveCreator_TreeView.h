#ifndef CURVECREATOR_TREEVIEW_H
#define CURVECREATOR_TREEVIEW_H

#include <QTreeView>
#include <QAbstractItemModel>

class CurveCreator_Curve;

class CurveCreator_TreeViewModel : public QAbstractItemModel
{
public:
  CurveCreator_TreeViewModel( CurveCreator_Curve* theCurve, QObject* parent );
  virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
  virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
  virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
  virtual QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
  virtual QModelIndex	parent(const QModelIndex & theIndex) const;

  QModelIndex sectionIndex( int theSection ) const;
  QModelIndex pointIndex( int theSection, int thePoint ) const;

  bool    isSection( const QModelIndex& theIndx ) const;
  int     getSection( const QModelIndex& theIndx ) const;
  int     getPoint( const QModelIndex& theIndx ) const;

  void    setCurve( CurveCreator_Curve* theCurve );

private:
  enum IconType{ ICON_POLYLINE, ICON_SPLINE, ICON_CLOSED_SPLINE, ICON_CLOSED_POLYLINE, ICON_POINT };
private:
  CurveCreator_Curve*          myCurve;
  QMap<IconType, QPixmap>      myCachedIcons;
};

class CurveCreator_TreeView : public QTreeView
{
  Q_OBJECT
public:
  enum SelectionType{ ST_NOSEL, ST_POINTS, ST_POINTS_ONE_SECTION, ST_SECTIONS, ST_MIXED };
public:
  explicit CurveCreator_TreeView( CurveCreator_Curve* theCurve, QWidget *parent = 0);
  SelectionType getSelectionType() const;
  QList<int> getSelectedSections() const;
  QList< QPair< int, int > > getSelectedPoints() const;

  void    pointsAdded( int theSection, int thePoint, int thePointsCnt=1 );
  void    pointDataChanged( int theSection, int thePoint );
  void    pointsRemoved(int theSection, int thePoint, int thePointsCnt=1 );
  void    pointsSwapped( int theSection, int thePointNum, int theOffset );

  void    sectionAdded( int theSection );
  void    sectionChanged(int theSection , int aSectCnt = 1);
  void    sectionsRemoved( int theSection, int theSectionCnt=1 );
  void    sectionsSwapped( int theSection, int theOffset );

  void    setSelectedSections( const QList<int>& theList );
  void    setSelectedPoints( const QList< QPair<int, int> >& thePointsList );

  void    setCurve( CurveCreator_Curve* theCurve );

signals:
  void    selectionChanged();
  void    sectionEntered(int);
  void    pointEntered(int,int);
public slots:
protected slots:
  void onActivated( QModelIndex theIndx );
protected:
  void setIndexState( const QModelIndex& theIndx, bool& isExpanded, bool& isSelected, bool& isCurrent );
  void swapIndexes( const QModelIndex& theFirst, const QModelIndex& theSecond );
  void getIndexInfo( const QModelIndex& theIndx, bool& isExpanded, bool& isSelected, bool& isCurrent );

};

#endif // CURVECREATOR_TREEVIEW_H
