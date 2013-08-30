#ifndef CURVE_CREATOR_LISTENER_HXX
#define CURVE_CREATOR_LISTENER_HXX

class CurveCreator_Listener
{
public:
  CurveCreator_Listener(void){};
  virtual ~CurveCreator_Listener(void){};

  virtual void pointChanged( int theSection, int thePoint ){}
  virtual void pointRemoved( int theSection, int theFirstPoint, int thePointCnt ){}
  virtual void pointInserted( int theSection, int theIndx ){}

  virtual void sectionClosed( int theSection, bool isClosed ){}
  virtual void sectionAdded( int theSection ){}
  virtual void sectionRemoved( int theSection ){}
  virtual void sectionTypeChanged( int theSection ){}

  virtual void curveChanged(){}
};

#endif