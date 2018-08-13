#include "BaseSprite.h"
#include "Utils/ConfigMgr.h"
#include "Skill/SkillMgr.h"
#include "Skill/SkillEffectFactory.h"
#include "Dlg/Fight/AIMgr.h"
#include "Cocos2dEx/SkillEffectAnim.h"

BaseSprite::BaseSprite() 
		: _ai(nullptr)	
		, _is_boos(false)
		, _prePosList()
		, _arm(nullptr)
		, _xixue(0)
		, _hurt_more(0)
		, _speed_diff(0)
		, _speed_per(0)		
		, _damage(0)	
		, _damage_diff(0)
		, _damage_per(0)
		, _def(0)	
		, _def_diff(0)
		, _shoot_range_diff(0)
		, _def_per(0)	
		, _crit(0)			//暴击值
		, _crit_rate(0)		//暴击率
		, _mgr_skill(nullptr)
		, _radius(0)
		, _cdBar(nullptr)
{
}
BaseSprite::~BaseSprite() {
	
	CC_SAFE_DELETE(this->_mgr_skill);
	

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
			else if (_target->_isbroken == false && _ai->isWithinShootRange(getPosition(), _target->getPosition(), this->getShootRange())) {

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
				if (_ai->isWithinShootRange(getPosition(), _target->getPosition(), this->getShootRange())) {
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

	//处理特效
	{
		auto it = this->_skill_effects.begin();
		while (it != this->_skill_effects.end()){
			it->second->update(dt);
			if (it->second->isCanRemove()){
				it = _skill_effects.erase(it);
			}
			else{
				++it;
			}
		}
	} 
}

void BaseSprite::atk() { CCLOG("Base atk"); }
void BaseSprite::run() { CCLOG("Base run"); }
 //受伤
void BaseSprite::hurt(int hurtType, int x, BaseSprite* atk, bool isShowTxt)
{
	if (_isbroken == true || _healthPoint <= 0) {
		//_arm->getAnimation()->stop();
		_ai->setObjDead(this);
		_isbroken = true;
		return;
	}
	int oldHp = _healthPoint;
	int temp = 0;
	switch (hurtType){
		case 1:	//物理伤害
		{
			//伤害-防御
			temp = x - getDef();

			//计算普攻暴击
			if (atk){
				int r = rand() % 100;
				if (r < atk->_crit_rate){
					temp = temp * (100 + this->_crit) / 100;
				}
			}

			//不能破防减0血
			temp = max(temp, 0);

			//攻击者吸血
			if (atk){
				atk->hurt(3, temp, nullptr);
			}
			
			temp = ( max(0.0, (1.0 + (float)this->_hurt_more / 100)) * temp);
			_healthPoint -= temp;
			break;
		}
		case 2://法术伤害
		{
			if (x > 0){
				temp = ( max(0.0, (1.0 + (float)this->_hurt_more / 100)) * x);
			}
			else{
				temp = x;
			}
			_healthPoint -= temp;			
			_healthPoint = min(_healthPoint, _totalHP);
			break;
		}
		case 3://吸血
		{
			if (this->_xixue <= 0) {
				return;
			}
			temp =  - x * this->_xixue / 100;
			_healthPoint -= temp;
			_healthPoint = min(_healthPoint, _totalHP);
			break;
		}
	}

	

	if (_healthPoint <= 0) {
		_isbroken = true;
		//this->setVisible(false);
		_ai->setObjDead(this);
		setState(STATE_DEATH, _dir);
		//_arm->getAnimation()->stop();
	}
	else {
		_hpBar->setPercent(100.0 * _healthPoint / _totalHP);
		_txt_hp->setString(cocos2d::Value(_healthPoint).asString());
	}

	if (isShowTxt) {
		//飘字
		Vec2 txtPos = Vec2(40, _arm->getContentSize().height / 2 - 20);
		Color4B txtColor(255, 0, 0, 255);
		if (temp < 0) {
			txtColor = Color4B(0, 255, 0, 255);
		}
		Text* txtHurt = this->flyHurtNum(temp, txtPos);
		txtHurt->setTextColor(txtColor);
	}

	//受到伤害时触发
	if (temp > 0){
		this->_mgr_skill->triggerSkill(SkillTriggerType::Hurt, this->getPosition());
	}
	
}
void BaseSprite::changeHpBar(){CCLOG("Base changeHpBar");}
void BaseSprite::hurtEffect(int x, int bulletType) { CCLOG("Base hurtEffect"); }
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

void BaseSprite::showSkillRange(){
	DrawNode* dn = DrawNode::create();
	this->addChild(dn);
	dn->drawCircle(this->getPosition(),100,100,100,true,Color4F(1,1,1,1));
}

int BaseSprite::getCamp(){
	return this->_camp;
}

void BaseSprite::setCamp(int camp){
	this->_camp = camp;
	changeHpBar();
}

int BaseSprite::getHp(){
	return this->_healthPoint;
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

int BaseSprite::getSpeed(){
	int speed = max(0, this->_speed + this->_speed_diff);
	int speedPer = max(-100, this->_speed_per);
	int ret = (float)speed * (1.0 + (float)speedPer / 100);
	return ret;
}

int BaseSprite::getShootRange(){
	int ret = max(0, this->_shoot_range + this->_shoot_range_diff);	
	return ret;
}

int BaseSprite::getXiXue(){
	return this->_xixue;
}

int BaseSprite::getHurtMore(){
	return this->_hurt_more;
}

void BaseSprite::addSkillEffect(int skillEffectId, BaseSprite* caster){
	//SkillEffect* skillEffect = new SkillEffect(this, skillEffectId);
	SkillEffect* skillEffect = SEF()->newSkillEffect(this, skillEffectId, caster);
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

void BaseSprite::clearBadSkillEffect(){
	auto it = this->_skill_effects.begin();
	auto itEnd = this->_skill_effects.end();
	while (it != itEnd){
		if (it->second->getKind() == SkillEffectKind::Bad){
			CC_SAFE_DELETE(it->second);
			it = this->_skill_effects.erase(it);
		}
		else{
			++it;
		}
	}
}

void BaseSprite::addSkillEffectAnim(SkillEffectAnim* arm){
	this->addChild(arm);
	this->_skill_effect_anims.push_back(arm);
}

void BaseSprite::delSkillEffectAnim(int key){
	SkillEffectAnim* temp = nullptr;
	auto it = this->_skill_effect_anims.begin();
	auto itEnd = this->_skill_effect_anims.end();	
	while (it != itEnd){
		if ((*it)->getKey() == key){	
			//(*it)->removeFromParent();
			temp = (*it);
			this->_skill_effect_anims.erase(it);
			break;
		}
		++it;
	}
	if (temp){
		temp->removeFromParent();
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

