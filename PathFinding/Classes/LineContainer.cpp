#include "LineContainer.h"
#include "Constants.h"



LineContainer::LineContainer():_goalX(-1),_goalY(-1),_m_map(xLineCount,yLineCount),_startDrawPath(false){
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	float totalWidth = _screenSize.width - (leftMargin + rightMargin);
	float totalHeight = _screenSize.height - (topMargin + bottomMargin);
	_eachBoxWidth = totalWidth / xLineCount;
	_eachBoxHeight = totalHeight / yLineCount;
}

void LineContainer::draw(){
	
	//draw white background
	ccDrawSolidRect(ccp(0.0,0.0),ccp(_screenSize.width,_screenSize.height),ccc4f(1.0,1.0,1.0,1.0));

	drawCells(_m_map);

	drawLineMap();

	if(_startDrawPath){
		drawPath(_deqPathCells);
	}
	

}


void LineContainer::updateMapParameter(Array2D<Cell> &m_map){
	_m_map = m_map;
}

void LineContainer::setPathParameters(deque<Cell*> &m_map){
	_deqPathCells = m_map;
	_startDrawPath = true;
}

void LineContainer::drawLineMap(){
	

	//draw black background
	ccDrawColor4F(0.0,0.0,0.0,1.0);
	float startPointX = leftMargin;
	float startPointY = bottomMargin;
	float endPointX = _screenSize.width - rightMargin;
	float endPointY = bottomMargin;
	for(int i = 0; i <= yLineCount; ++i){
		ccDrawLine(ccp(startPointX,startPointY),ccp(endPointX,endPointY));//draw horizontal line
		startPointY += _eachBoxHeight;
		endPointY = startPointY;
	}

	startPointX = leftMargin;
	startPointY = bottomMargin;
	endPointX = leftMargin;
	endPointY = _screenSize.height - topMargin;
	for(int i = 0; i <= xLineCount; ++i){
		ccDrawLine(ccp(startPointX,startPointY),ccp(endPointX,endPointY));//draw vertical line
		startPointX += _eachBoxWidth;
		endPointX = startPointX;
	}
}

void LineContainer::drawCells(Array2D<Cell> &m_map){
	Cell *cell;
	int startX,startY,endX,endY;
	for(int i = 0; i < xLineCount; ++i){
		for(int j = 0; j < yLineCount; ++j){
			cell = m_map.Get(i,j);
			startX = leftMargin + cell->getX() * _eachBoxWidth + cellPadding;
			startY = bottomMargin + cell->getY() * _eachBoxHeight + cellPadding;
			endX = leftMargin + (cell->getX() + 1) * _eachBoxWidth - cellPadding;
			endY = bottomMargin + (cell->getY() + 1) * _eachBoxHeight - cellPadding;

			ccDrawSolidRect(ccp(startX,startY),ccp(endX,endY),CellColors[cell->getWeight() - 1]);
			if(cell->getDrawProgress()){
				ccDrawSolidRect(ccp(startX,startY),ccp(endX,endY),ccc4f(0.0f,1.0f,0.6f,0.5f));
			}
		}
	}
}


void LineContainer::drawPath(deque<Cell*> &depCells){
	ccDrawColor4F(0.0,0.0,0.0,1.0);
	Cell* cell;
	float startX,startY,endX,endY;
	for(deque<Cell*>::iterator itr = depCells.begin(); itr != depCells.end(); ++itr){
		startX = leftMargin + _eachBoxWidth * (*itr)->getX() + _eachBoxWidth / 2;
		startY = bottomMargin + _eachBoxHeight * (*itr)->getY() + _eachBoxHeight / 2;	
		++itr;
		if(itr != depCells.end()){
			endX = leftMargin + _eachBoxWidth * (*itr)->getX() + _eachBoxWidth / 2;
			endY = bottomMargin + _eachBoxHeight * (*itr)->getY() + _eachBoxHeight / 2;	
			ccDrawLine(ccp(startX,startY),ccp(endX,endY));
		}
		--itr;
	}

}

