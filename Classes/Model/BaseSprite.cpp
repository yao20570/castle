#include "BaseSprite.h"
#include "Utils/ConfigMgr.h"
#include "Skill/SkillMgr.h"
#include "Skill/SkillEffectFactory.h"
#include "Dlg/Fight/AIMgr.h"

BaseSprite::BaseSprite() 
		: _ai(nullptr)	
		, _prePosList()
		, _xixue(0)
		, _hurt_more(0)
		, _speed_diff(0)
		, _speed_per(0)		
		, _damage(0)	
		, _damage_diff(0)
		, _damage_per(0)
		, _def(0)	
		, _def_diff(0)
		, _def_per(0)	
		, _mgr_skill(nullptr)
{
}
BaseSprite::~BaseSprite() {
	
	CC_SAFE_FREE(this->_mgr_skill);
	
}

bool BaseSprite::init()
{
    if ( !Sprite::init() ) {
        return false;
    }
    
    return true;
}

void BaseSprite::update(float dt)
{
	if (_isbroken == true) {
		this->unscheduleAllCallbacks();
		return;
	}


	switch (_state)
	{
		case STATE_WIN:// 
		{
			int tempDir = GM()->getDir(getPosition(), _target->getPosition());
			setState(STATE_IDLE, _dir);
			break;
		}
		case STATE_YUN:// 眩晕
		{
			setState(STATE_YUN, _dir);
			break;
		}
		case STATE_IDLE:// 悠闲
		{
			setState(STATE_IDLE, _dir);
			break;
		}
		case STATE_RUN:// 走路		
		{
			int minDis = 100000;
			_target = nullptr;


			switch (_camp)
			{
				case 1:
					for (auto it : _ai->_objEnemy) {
						if (it->isDeath()) {
							continue;
						}
						int dis = (int)it->getPosition().getDistance(this->getPosition());
						if (dis < minDis) {
							minDis = dis;
							_target = it;
						}
					}
					break;
				case 2:
					for (auto it : _ai->_objSelf) {
						if (it->isDeath()) {
							continue;
						}
						int dis = (int)it->getPosition().getDistance(this->getPosition());
						if (dis < minDis) {
							minDis = dis;
							_target = it;
						}
					}
					break;
			}


			if (_target == nullptr) {
				setState(STATE_IDLE, _dir);
			}
			else if (_target->_isbroken == false && _ai->isWithinShootRange(getPosition(), _target->getPosition(), _shootRange)) {

				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
				setState(STATE_ATK, tempDir);
			}
			else {
				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
				setState(STATE_RUN, tempDir);

				Vec2 disPos = _target->getPosition() - getPosition();
				int speed = this->getSpeed();
				Vec2 nextPos(getPositionX() + disPos.x * speed / minDis / 60, getPositionY() + disPos.y * speed / minDis / 60);

				_ai->setObjPos(this, nextPos);
			}
			break;
		}

		case STATE_ATK:// 攻击
		{
			if (_target == nullptr || _target->isDeath()) {
				// 失去目标，变成悠闲
				_target = nullptr;

				setState(STATE_RUN, _dir);
			}

			// 向目标移动、或攻击目标
			else {
				int tempDir = GM()->getDir(getPosition(), _target->getPosition());
				// 攻击
				if (_ai->isWithinShootRange(getPosition(), _target->getPosition(), _shootRange)) {
					setState(STATE_ATK, tempDir);
				}
				// 走路
				else {
					setState(STATE_RUN, tempDir);
				}
			}
			break;
		}

	}

	for ( auto& it : this->_skill_effects){		
		it.second->update(dt);
	}
}

void BaseSprite::atk() { CCLOG("Base atk"); }
void BaseSprite::run() { CCLOG("Base run"); }
void BaseSprite::hurt(int hurtType, int x, BaseSprite* atk) { CCLOG("Base hurt"); }
void BaseSprite::hurtEffect(int x) { CCLOG("Base hurtEffect"); }
void BaseSprite::death() { CCLOG("Base death"); }
void BaseSprite::idle() { CCLOG("Base idle"); }
bool BaseSprite::isDeath() {  CCLOG("Base isDeath"); return false; }
void BaseSprite::setSelect(bool b) {  CCLOG("Base setSelect"); }
int BaseSprite::getDir() { return _dir; }
void BaseSprite::setState(int state, int dir) {  CCLOG("Base setState"); }
void BaseSprite::showName(bool b) {
	if (_txtName != nullptr) {
		_txtName->setVisible(b);
	}
}
void BaseSprite::setObjName(string name) {  CCLOG("Base setName"); }

string BaseSprite::getObjName(){
	return this->_objname;
}

int BaseSprite::getHp(){
	return this->_healthPoint;
}

int BaseSprite::getSpeed(){
	int speed = max(0, this->_speed + this->_speed_diff);
	int speedPer = max(-100, this->_speed_per);
	int ret = (float)speed * (1.0 + (float)speedPer / 100);
	return ret;
}

int BaseSprite::getDamage(){
	int damage = max(0, this->_damage + this->_damage_diff);
	int damagePer = max(-100, this->_damage_per);
	int ret = (float)damage * (1.0 + (float)damagePer / 100);
	return ret;
}

int BaseSprite::getDef(){
	int def = max(0, this->_def + this->_def_diff);
	int defPer = max(-100, this->_def_per);
	int ret = (float)def * (1.0 +(float)defPer / 100);
	return ret;
}

int BaseSprite::getXiXue(){
	return this->_xixue;
}

int BaseSprite::getHurtMore(){
	return this->_hurt_more;
}

void BaseSprite::addSkillEffect(int skillEffectId){
	//SkillEffect* skillEffect = new SkillEffect(this, skillEffectId);
	SkillEffect* skillEffect = SEF()->newSkillEffect(this, skillEffectId);
	if (skillEffect){
		this->delSkillEffect(skillEffectId);
		this->_skill_effects[skillEffectId] = skillEffect;
	}else{
		CCAssert(false, "");
	}
}

void BaseSprite::delSkillEffect(int skillEffectId){
	auto it = this->_skill_effects.find(skillEffectId);
	if (it != this->_skill_effects.end()){
		CC_SAFE_DELETE(it->second);
		this->_skill_effects.erase(it);
	}
}

void BaseSprite::triggerSkill(SkillTriggerType tt, const Vec2& targetPos){
	if (this->_mgr_skill){
		this->_mgr_skill->triggerSkill(tt, targetPos);
	}
}

Text* BaseSprite::flyHurtNum(int value, Vec2 textPos){
	Text* txtHurt = Text::create();
	txtHurt->setFontSize(30);
	txtHurt->setPosition(textPos);
	this->addChild(txtHurt);

	txtHurt->setString(cocos2d::Value(value).asString());

	Vec2 r = Vec2(cocos2d::random(1, 30), cocos2d::random(1, 30));
	auto mt = MoveTo::create(0.2, Vec2(10, 10) + txtHurt->getPosition() + r);
	auto dt = DelayTime::create(0.5);
	auto rs = RemoveSelf::create(true);
	auto seq = Sequence::create(mt, dt, rs, nullptr);
	txtHurt->runAction(seq);

	return txtHurt;
}

