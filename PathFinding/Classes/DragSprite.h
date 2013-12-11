#ifndef _DRAGSPRITE_H_
#define _DRAGSPRITE_H_

#include "cocos2d.h"

USING_NS_CC;

typedef enum tagSpriteState 
{
	kSpriteStateGrabbed,
	kSpriteStateUngrabbed
} SpriteState; 

class DragSprite : public CCSprite, public CCTargetedTouchDelegate{
public:
	DragSprite();
	static DragSprite* spriteWithTexture(CCTexture2D* aTexture);
	bool initWithTexture(CCTexture2D* aTexture);
	
	bool containsTouchLocation(CCTouch* touch);

	CCRect rect();
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
private:
	SpriteState  m_state;
};




#endif