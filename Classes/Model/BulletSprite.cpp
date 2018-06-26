#include "BulletSprite.h"


BulletSprite* BulletSprite::create(Vec2 src, Vec2 des, int damage, BaseSprite* atk, BaseSprite* target, string img, int srcType)
{
    BulletSprite *pRet = new(std::nothrow) BulletSprite();
	if (pRet && pRet->init(src, des, damage, atk, target, img, srcType)) {
        pRet->autorelease();
        return pRet;
    }
    else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}


bool BulletSprite::init(Vec2 src, Vec2 des, int damage, BaseSprite* atk, BaseSprite* target, string img, int srcType)
{
    if ( !Sprite::init() ) {
        return false;
    }
    
	_srcType = srcType;
    _src = src;
    _des = des;
    _damage = damage;
	_atk = atk;
    _target = target;
    _img = img;

	this->effectDir = GM()->getDir(src, des);
    showUI();
    
    return true;
}


void BulletSprite::showUI()
{
	this->setTexture(_img);	
    this->setPosition(_src);
       

	//this->setAnchorPoint(Vec2(0, 0.5));
	

    auto delta = _des - _src;
    auto ang = atan2(delta.y, delta.x) * 180 / acos(-1.0);
    ang = 360 - ang;
    this->setRotation(ang);
    
    auto move = MoveBy::create(0.2f, delta);
    auto func = CallFunc::create(CC_CALLBACK_0(BulletSprite::atk, this));
    this->runAction(Sequence::create(move, func, nullptr));
}


void BulletSprite::atk()
{
	if (_target->_objType == 3) {
		_target->hurt(_srcType, this->_atk);
	}
	else {
		_target->hurt(_damage, this->_atk);
		if ("images/bullet/fashu.png" == _img) {
			_target->hurtEffect(this->effectDir);
		}
	}
    this->removeFromParent();
}

