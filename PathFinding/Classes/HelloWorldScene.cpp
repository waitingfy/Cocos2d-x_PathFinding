#include "HelloWorldScene.h"
#include "Constants.h"
#include <vector>
#include <deque>
#include <algorithm>
#include "main.h"

//test change
using namespace cocos2d;
using namespace std;

int g_goalX;
int g_goalY;

bool compareTwoCellsByDistance(Cell *c1, Cell *c2){
	if(c1->getDistance() <= c2->getDistance()){
		return false;
	}else{
		return true;
	}
}

bool comparebyWhichNearerGoalSimpleWay(Cell *c1, Cell *c2){
	int distanceOfC1AndGoal = abs(g_goalX - c1->getX()) + abs(g_goalY - c1->getY());
	int distanceOfC2AndGoal = abs(g_goalX - c2->getX()) + abs(g_goalY - c2->getY());
	if(distanceOfC1AndGoal <= distanceOfC2AndGoal){
		return false;
	}else{
		return true;
	}
}

float distanceBetweenTwoCells(float c1X,float c1Y, float c2X, float c2Y){

	return sqrt(pow(c2X - c1X,2) + pow(c2Y - c1Y,2));
}

bool comparebyWhichNearerGoalPhysicWay(Cell *c1, Cell *c2){
	float distanceOfC1AndGoal = distanceBetweenTwoCells((float)c1->getX(),(float)c1->getY(),(float)g_goalX,(float) g_goalY);

	float distanceOfC2AndGoal = distanceBetweenTwoCells((float)c2->getX(),(float)c2->getY(),(float)g_goalX,(float) g_goalY);
	if(distanceOfC1AndGoal <= distanceOfC2AndGoal){
		return false;
	}else{
		return true;
	}
}

bool comparebyDistanceBetweenStartAndGoal(Cell *c1, Cell *c2){
	float distanceOfC1AndGoal = c1->getDistance() + 
		distanceBetweenTwoCells((float)c1->getX(),(float)c1->getY(),(float)g_goalX,(float) g_goalY);

	float distanceOfC2AndGoal = c2->getDistance() + 
		distanceBetweenTwoCells((float)c2->getX(),(float)c2->getY(),(float)g_goalX,(float) g_goalY);
	if(distanceOfC1AndGoal <= distanceOfC2AndGoal){
		return false;
	}else{
		return true;
	}
}

HelloWorld::HelloWorld():_m_Map(xLineCount,yLineCount),_pastTime(0),_startDrawProgress(false),
_drawProgressPerSecond(drawPathSpeed){
}



CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

		_screenSize = CCDirector::sharedDirector()->getWinSize();
		_eachBoxWidth = (_screenSize.width - (leftMargin + rightMargin)) / xLineCount;
		_eachBoxHeight = (_screenSize.height - (topMargin + bottomMargin)) / yLineCount;

		initCells();

		_lineContainer = LineContainer::create();
		_lineContainer->updateMapParameter(_m_Map);
		this->addChild(_lineContainer);

		CCTexture2D *pPlayerTexture = CCTextureCache::sharedTextureCache()->addImage("start.png");
		CCTexture2D *pGoalTexture = CCTextureCache::sharedTextureCache()->addImage("goal.png");

		_player = DragSprite::spriteWithTexture(pPlayerTexture);
		_player->setPosition(ccp(200,100));
		this->addChild(_player,1);

		_goal = DragSprite::spriteWithTexture(pGoalTexture);
		_goal->setPosition(ccp(300,100));
		this->addChild(_goal,1);

		CCMenuItemFont *pBeginFindingByDistanceItem = CCMenuItemFont::create("Distance",
			this,menu_selector(HelloWorld::menuDistanceBasedFindingCallback));

		CCMenuItemFont *pBeginFindingBySimpleHeuristicItem = CCMenuItemFont::create("Simple Heuristic",
			this,menu_selector(HelloWorld::menuSimpleHeuristicBasedFindingCallback));

		CCMenuItemFont *pBeginFindingByComplexHeuristicItem = CCMenuItemFont::create("Complex Heuristic",
			this,menu_selector(HelloWorld::menuComplexHeuristicBasedFindingCallback));

		CCMenuItemFont *pBeginFindingByAStarItem = CCMenuItemFont::create("A Star",
			this,menu_selector(HelloWorld::menuAStarFindingCallback));

		CCMenuItemFont *pQuickerFindingItem = CCMenuItemFont::create("+++",this,
			menu_selector(HelloWorld::menuQuickFindingCallback));

		CCMenuItemFont *pSlowerFindingItem = CCMenuItemFont::create("---",this,
			menu_selector(HelloWorld::menuSlowerFindingCallback));

		// start finding menu
		CCMenu* pMenu = CCMenu::create(pBeginFindingByDistanceItem,pBeginFindingBySimpleHeuristicItem,
			pBeginFindingByComplexHeuristicItem,pBeginFindingByAStarItem,pQuickerFindingItem,
			pSlowerFindingItem, NULL);
		pMenu->alignItemsHorizontallyWithPadding(20);
		pMenu->setPosition(ccp(385,70));
		CC_BREAK_IF(! pMenu);

		this->addChild(pMenu);


		this->setTouchEnabled(true);
		
		this->schedule(schedule_selector(HelloWorld::update));

	
        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuPathFinding(PathFindingType type){

	_drawProgressPerSecond = drawPathSpeed;//reset draw speed

	
	fullFillPlayerAndGoalData();

	if(_playerX >= 0 && _playerX < xLineCount && _playerY >=0 && _playerY <= yLineCount
		&& _goalX >= 0 && _goalX < xLineCount && _goalY >=0 && _goalY <= yLineCount){
		clearPathAndCells();

		switch (type){
		case kDistanceBase:
			startPathFinding(compareTwoCellsByDistance,_playerX,_playerY,_goalX,_goalY);
			break;
		case kSimpleHeuristic:
			startPathFinding(comparebyWhichNearerGoalSimpleWay,_playerX,_playerY,_goalX,_goalY);
			break;
		case kComplexHeuristic:
			startPathFinding(comparebyWhichNearerGoalPhysicWay,_playerX,_playerY,_goalX,_goalY);
			break;
		case kAStar:
			startPathFinding(comparebyDistanceBetweenStartAndGoal,_playerX,_playerY,_goalX,_goalY);
			break;
		};
		_startDrawProgress = true;

		//build path
		fullFillPathData();

		CCLOG("the lowest distance: %f",_m_Map.Get(_goalX,_goalY)->getDistance());
	}else{
		CCLOG("invalidate position!");
	}

	
};


void HelloWorld::fullFillPathData() 
{
	_vecPathCell.clear();
	Cell* cell = _m_Map.Get(_goalX,_goalY);
	while(cell->getLastX() != -1){
		_vecPathCell.push_front(cell);
		cell = _m_Map.Get(cell->getLastX(),cell->getLastY());
	}
	_vecPathCell.push_front(_m_Map.Get(_playerX, _playerY));
}

void HelloWorld::fullFillPlayerAndGoalData(){
	CCPoint playerPosition = _player->getPosition();
	CCPoint goalPosition = _goal->getPosition();

	_playerX = (playerPosition.x - leftMargin) / _eachBoxWidth;
	_playerY = (playerPosition.y - bottomMargin) / _eachBoxHeight;

	_goalX = (goalPosition.x - leftMargin) / _eachBoxWidth;
	_goalY = (goalPosition.y - bottomMargin) / _eachBoxHeight;

	g_goalX = _goalX;
	g_goalY = _goalY;
}

void HelloWorld::menuDistanceBasedFindingCallback(CCObject* pSender)
{
	menuPathFinding(kDistanceBase);
}

void HelloWorld::menuSimpleHeuristicBasedFindingCallback(CCObject* pSender)
{
	menuPathFinding(kSimpleHeuristic);
}


void HelloWorld::menuComplexHeuristicBasedFindingCallback(CCObject* pSender)
{
	menuPathFinding(kComplexHeuristic);
}


void HelloWorld::menuAStarFindingCallback(CCObject* pSender)
{
	menuPathFinding(kAStar);
}


void HelloWorld::menuQuickFindingCallback(CCObject* pSender){
	_drawProgressPerSecond -= 0.1f;
	if(_drawProgressPerSecond <= 0){
		_drawProgressPerSecond = 0.0f;
	}
}

void HelloWorld::menuSlowerFindingCallback(CCObject* pSender){
	_drawProgressPerSecond += 0.1f;
	if(_drawProgressPerSecond >= drawPathSpeed){
		_drawProgressPerSecond = drawPathSpeed;
	}
}

void HelloWorld::initCells(){
	//init map
	for(int i = 0; i < xLineCount; ++i){
		for(int j = 0; j < yLineCount; ++j){
			Cell *cell = new Cell();
			cell->setX(i);
			cell->setY(j);
			_m_Map.Set(i,j,cell);
		}
	}
}

void HelloWorld::clearCells(){
	//init map
	for(int i = 0; i < xLineCount; ++i){
		for(int j = 0; j < yLineCount; ++j){
			Cell *cell = _m_Map.Get(i,j);
			cell->setMarked(false);
			cell->setDistance(0);
			cell->setLastX(-1);
			cell->setLastY(-1);
			cell->setDrawProgress(false);
			_m_Map.Set(i,j,cell);
		}
	}
}

void HelloWorld::startPathFinding(compareTwoCells compareMethod, int startX,int startY,int goalX,int goalY){
	Cell *startCell = _m_Map.Get(startX, startY);
	vector<Cell*> vecCells;
	vecCells.push_back(startCell);
	make_heap(vecCells.begin(),vecCells.end(),compareMethod);
	startCell->setMarked(true);
	Cell *nowProcessCell;


	while(vecCells.size() != 0){
		pop_heap(vecCells.begin(),vecCells.end(),compareMethod);
		nowProcessCell = vecCells.back();
		vecCells.pop_back();

		_vecCommandCell.push_back(nowProcessCell);//just record the finding progress

		if(nowProcessCell->getX() == _goalX && nowProcessCell->getY() == _goalY){//the goal is reach
			return;
		}

		for(int i = 0; i < 8; ++i){ //check eight direction

			int indexX = nowProcessCell->getX() + DIRECTION[i][0];
			int indexY = nowProcessCell->getY() + DIRECTION[i][1];

			if(indexX >= 0 && indexX < xLineCount && indexY >= 0 && indexY < yLineCount
				&& _m_Map.Get(indexX,indexY)->getPassable() == true){//check is a OK cell or not
					Cell *cell = _m_Map.Get(indexX,indexY);
					float beforeDistance = DISTANCE[i] * cell->getWeight() + _m_Map.Get(nowProcessCell->getX(),
						nowProcessCell->getY())->getDistance();//calculate the distance
					if(cell->getMarked() == false){ 
						cell->setMarked(true);
						cell->setLastX(nowProcessCell->getX());
						cell->setLastY(nowProcessCell->getY());
						cell->setDistance(beforeDistance);
						vecCells.push_back(cell);//only push the unmarked cell into the vector
						push_heap(vecCells.begin(),vecCells.end(),compareMethod);
					}else{// if find a lower distance, update it 
						if(beforeDistance < cell->getDistance()){
							cell->setDistance(beforeDistance);
							cell->setLastX(nowProcessCell->getX());
							cell->setLastY(nowProcessCell->getY());
							make_heap(vecCells.begin(),vecCells.end(),compareMethod);//distance change,so make heap again
						}
					}
			}

		}
	}
}

void HelloWorld::update(float dt){
	_pastTime += dt;
	if(_pastTime >= _drawProgressPerSecond){
		_pastTime = 0;
		if(_startDrawProgress && _vecCommandCell.size() > 0){//draw path finding progress		
			Cell* cell = _vecCommandCell.front();
			_vecCommandCell.pop_front();
			_m_Map.Get(cell->getX(),cell->getY())->setDrawProgress(true);
			_lineContainer->updateMapParameter(_m_Map);
		}

		if(_startDrawProgress && _vecCommandCell.size() == 0){
			_lineContainer->setPathParameters(_vecPathCell);//draw path
		}
		
	}
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	int indexX = (touch->getLocation().x - leftMargin) / _eachBoxWidth;
	int indexY = (touch->getLocation().y - bottomMargin) / _eachBoxHeight;

	if(indexX >=0 && indexX < xLineCount && indexY >=0 && indexY < yLineCount){
		_m_Map.Get(indexX,indexY)->setWeight(_m_Map.Get(indexX,indexY)->getWeight() + 1);
		_lineContainer->updateMapParameter(_m_Map);
	}
}

void HelloWorld::clearPathAndCells(){
	clearCells();
	_vecCommandCell.clear();
	_lineContainer->setStartDrawPath(false);
}