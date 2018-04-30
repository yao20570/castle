#include "PlayerObj.h"
#include "UI/BattleScene/AIManager.h"
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
	:_ai(nullptr)
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

	_objType = 3;
	_prePosList = list<Vec2>();
	_isbroken = false;
	_healthPoint = 60;
	_dotX = -1;
	_dotY = -1;



	_camp = camp; 
	_ai = ai;
	_ai->addObj(this, _camp);

	loadData();
	showUI();

	//schedule(schedule_selector(PlayerObj::update), 0.81f);
	schedule(schedule_selector(PlayerObj::update));

	return true;
}


void PlayerObj::loadData()
{

}


void PlayerObj::showUI()
{
	//int y = _camp == 1 ? -1 : 1;
	//_dir = GM()->getDir(Vec2(0, y));
	//_arm = Armature::create(ANIM_NAME_ARAGORN);
	//_arm->getAnimation()->play("idle" + GM()->getIntToStr(_dir));
	//_arm->setPositionY(20);
	//_arm->pause();
	//_arm->getAnimation()->setMovementEventCallFunc(this, SEL_MovementEventCallFunc(&PlayerObj::atk));

	//this->addChild(_arm);

	char str[50] = "\0";
	sprintf(str, "%d", _healthPoint);
	_txt_hp = Text::create(str, FONT_ARIAL, 16);
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
void PlayerObj::hurt(int x)
{
	if (_isbroken == true || _healthPoint <= 0) {
		//_arm->getAnimation()->stop();
		_ai->setObjDead(this);
		_isbroken = true;
		return;
	}

	_healthPoint -= 1;
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
		//_hpBar->setPercent(100.0 * _healthPoint / _totalHP);
	}
}
