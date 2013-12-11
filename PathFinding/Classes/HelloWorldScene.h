#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LineContainer.h"
#include "DragSprite.h"
#include "Array2D.h"
#include "Cell.h"
#include <deque>


USING_NS_CC;
using namespace std;

typedef enum{
	kDistanceBase,
	kSimpleHeuristic,
	kComplexHeuristic,
	kAStar
}PathFindingType;

typedef bool (*compareTwoCells)(Cell *c1, Cell *c2);

class HelloWorld : public cocos2d::CCLayer
{
public:

	HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuDistanceBasedFindingCallback(CCObject* pSender);
	void menuSimpleHeuristicBasedFindingCallback(CCObject* pSender);
	void menuComplexHeuristicBasedFindingCallback(CCObject* pSender);
	void menuAStarFindingCallback(CCObject* pSender);
	void menuPathFinding(PathFindingType type);
	void menuQuickFindingCallback(CCObject* pSender);
	void menuSlowerFindingCallback(CCObject* pSender);
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
	void update(float dt);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);

private:
	DragSprite *_player;
	DragSprite *_goal;
	LineContainer *_lineContainer;
	CCSize _screenSize;
	float _eachBoxWidth;
	float _eachBoxHeight;

	Array2D<Cell> _m_Map;
	int _playerX;
	int _playerY;
	int _goalX;
	int _goalY;

    deque<Cell*> _vecCommandCell;
	deque<Cell*> _vecPathCell;
	bool _startDrawProgress;
	float _pastTime;
	float _drawProgressPerSecond;

	void initCells();
	void clearCells();
	void startPathFinding(compareTwoCells compareMethod, int startX,int startY,int goalX,int goalY);
	void fullFillPathData();
	void fullFillPlayerAndGoalData();
	void clearPathAndCells();
};

#endif  // __HELLOWORLD_SCENE_H__