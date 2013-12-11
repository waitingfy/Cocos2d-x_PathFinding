#include "DragSprite.h"

DragSprite::DragSprite(){
	m_state = kSpriteStateUngrabbed;
}


DragSprite* DragSprite::spriteWithTexture(CCTexture2D* aTexture)
{
	DragSprite* pDragSprite = new DragSprite();
	pDragSprite->initWithTexture( aTexture );
	pDragSprite->autorelease();

	return pDragSprite;
}

bool DragSprite::initWithTexture(CCTexture2D* aTexture)
{
	if( CCSprite::initWithTexture(aTexture) ) 
	{
		m_state = kSpriteStateUngrabbed;
	}

	return true;
}

void DragSprite::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCSprite::onEnter();
}

void DragSprite::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}    

CCRect DragSprite::rect()
{
	CCSize s = getTexture()->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool DragSprite::containsTouchLocation(CCTouch* touch)
{
	return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}


bool DragSprite::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (m_state != kSpriteStateUngrabbed) return false;
	if ( !containsTouchLocation(touch) ) return false;//touch on the sprite, then we do something

	m_state = kSpriteStateGrabbed;
	return true;
}

void DragSprite::ccTouchMoved(CCTouch* touch, CCEvent* event)
{

	CCAssert(m_state == kSpriteStateGrabbed, "Player - Unexpected state!");    
	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();


	CCPoint tap = touch->getLocation();
	this->setPosition(tap);

}



void DragSprite::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCAssert(m_state == kSpriteStateGrabbed, "Player - Unexpected state!");    

	m_state = kSpriteStateUngrabbed;
} 

void DragSprite::touchDelegateRetain()
{
	this->retain();
}

void DragSprite::touchDelegateRelease()
{
	this->release();
}