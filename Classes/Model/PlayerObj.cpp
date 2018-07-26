#include "PlayerObj.h"
#include "Dlg/Fight/AIMgr.h"
#include "Utils/ConfigMgr.h"

PlayerObj* PlayerObj::create(int ID, AIMgr* ai, int camp)
{
	PlayerObj *pRet = new(std::nothrow) PlayerObj();
	if (pRet && pRet->init(ID, ai, camp)) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

PlayerObj::PlayerObj()
	:BaseSprite()
	,_ai(nullptr)
{

}

PlayerObj::~PlayerObj()
{

}

bool PlayerObj::init(int ID, AIMgr* ai, int camp)
{
	if (!BaseSprite::init()) {
		return false;
	}
	_txtName = nullptr;
	_objType = 3;
	_prePosList = list<Vec2>();
	_isbroken = false;
	_healthPoint = 100;
	_totalHP = _healthPoint;
	_dotX = -1;
	_dotY = -1;
	_heroAtkTime = 0;


	_camp = camp; 
	_ai = ai;
	_ai->addObj(this, _camp);

	loadData();
	showUI();
	addHPBar();

	//schedule(schedule_selector(PlayerObj::update), 0.81f);
	schedule(schedule_selector(PlayerObj::update));

	return true;
}


void PlayerObj::loadData()
{

}


void PlayerObj::showUI()
{
	char str[128] = { 0 };
	sprintf(str, "animation/Anim_Hero_Caocao/Anim_Hero_Caocao.ExportJson");
	ArmatureDataManager::getInstance()->addArmatureFileInfo(str);

	int y = _camp == 1 ? -1 : 1;
	_dir = GM()->getDir(Vec2(0, y));
	if (_camp == 1) {
		_arm = Armature::create("Anim_Hero_Liubei");
	}
	else
	{
		_arm = Armature::create("Anim_Hero_Caocao"); 
	}
	
	_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));

	_arm->setPositionY(0);

	this->addChild(_arm);

	
}
void PlayerObj::addHPBar()
{
	auto bg = Sprite::create(IMG_BUILD_PRO_BK);
	if (_camp == 1){
		_hpBar = LoadingBar::create(IMG_BUILD_PRO);
	}
	else{
		_hpBar = LoadingBar::create(IMG_BUILD_PRO_ENEMY);
	}
	bg->setPosition(0, _arm->getContentSize().height / 2 - 20);
	_hpBar->setPosition(bg->getContentSize() / 2);
	bg->addChild(_hpBar);
	this->addChild(bg, 9, "Bar");

	bg->setScale(0.7);
	_hpBar->setPercent(100.0 * _healthPoint / _totalHP);

	char str[128] = { 0 };
	sprintf(str, "%d", _healthPoint);
	_txt_hp = Text::create(str, FONT_ARIAL, 18);
	_txt_hp->setPosition(bg->getPosition());
	_txt_hp->setLocalZOrder(100000);
	this->addChild(_txt_hp);
}

void PlayerObj::update(float dt) {

}

//  «∑ÒÀ¿Õˆ
bool PlayerObj::isDeath()
{
	return _isbroken;
}

//  ‹…À
void PlayerObj::hurt(int hurtType, int x, BaseSprite* atk)
{
	if (_isbroken == true || _healthPoint <= 0) {
		//_arm->getAnimation()->stop();
		_ai->setObjDead(this);
		_isbroken = true;
		return;
	}

	//∆Æ◊÷
	Vec2 txtPos = Vec2(40, _arm->getContentSize().height / 2 - 20);
	Color4B txtColor(255, 255, 255, 255);
	int damage = 5;
	if (atk->_objType == 2) {
		damage = 10 + 5 * _heroAtkTime;
		_heroAtkTime++;
		txtColor = Color4B(255, 0, 0, 255);
	}
	Text* txtHurt = this->flyHurtNum(damage, txtPos);
	txtHurt->setTextColor(txtColor);

	//—™Ãı
	_healthPoint -= damage;
	if (_healthPoint <= 0) {
		_isbroken = true;
		this->setVisible(false);
		_ai->setObjDead(this);
		//_arm->getAnimation()->stop();
	}
	else {
		char str[50] = "\0";
		sprintf(str, "%d", _healthPoint);
		_txt_hp->setString(str);
		_hpBar->setPercent(100.0 * _healthPoint / _totalHP);
	}
}

void PlayerObj::addSkillEffect(int skillEffectId){
	
}

void PlayerObj::delSkillEffect(int skillEffectId){
	
}

void PlayerObj::triggerSkill(SkillTriggerType tt, const Vec2& targetPos){
	
}