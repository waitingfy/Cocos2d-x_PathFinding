#ifndef _LINECONTAINER_H_
#define _LINECONTAINER_H_

#include "cocos2d.h"
#include "Array2D.h"
#include "Cell.h"
#include <vector>
#include <deque>

USING_NS_CC;
using namespace std;

class LineContainer : public CCNode{
public:
	LineContainer();
	CREATE_FUNC(LineContainer);

    CC_SYNTHESIZE(bool,_startDrawPath,StartDrawPath);

	virtual void draw();
	void drawLineMap();
	void drawCells(Array2D<Cell> &m_map);
	void drawPath(deque<Cell*> &depCells);
	void updateMapParameter(Array2D<Cell> &m_map);
	void setPathParameters(deque<Cell*> &depCells);
private:
	CCSize _screenSize;
	float _eachBoxWidth;
	float _eachBoxHeight;
	Array2D<Cell> _m_map;
	deque<Cell*> _deqPathCells;
	int _goalX;
	int _goalY;
};


#endif