#include "SkillEffect.h"

#include "Utils/ConfigMgr.h"
#include "Core/Msg/MsgMgr.h"


SkillEffect::SkillEffect(BaseSprite* obj, int skillEffectId, BaseSprite* caster)
	: m_obj(obj)
	, m_caster(caster)
	, m_isCanRemove(false)
	, m_id(skillEffectId)
	, m_curTimes(0)
{
	m_auto_key = GM()->getAutoKey();

	auto& cfg = *(CFG()->getSkillEffectById(this->m_id));

	this->m_type = SkillEffectType(cfg["Type"].asInt());
	this->m_kind = SkillEffectKind(cfg["Kind"].asInt());
	this->m_name = cfg["Name"].asString();
	this->m_value1 = cfg["Value1"].asInt();
	this->m_value2 = cfg["Value2"].asInt();
	this->m_value3 = cfg["Value3"].asInt();
	this->m_times = cfg["Tiems"].asInt();
	this->m_cd = cfg["CD"].asInt();
	this->m_lastTime = cfg["LastTime"].asInt();
	if (this->m_lastTime < 0){
		this->m_lastTime = INT_MAX;
	}

	//技能效果开始,结束时间
	this->m_startMTimestamp = GM()->getMTimeStamp();
	this->m_endMTimestamp = GM()->getMTimeStamp() + this->m_lastTime;

	CCLOG("SkillEffect create===>ptr:%p, start:%lld, end:%lld,", this, this->m_startMTimestamp, this->m_endMTimestamp);

	SkilAnimData& anim = this->m_anim_data;
	anim.layerType = SkillAnimLayerType(cfg["AnimType"].asInt());
	anim.posType = SkillAnimPosType::Src;
	anim.loop = cfg["LastTime"].asInt();
	anim.fileName = cfg["AnimName"].asString();
	anim.key = GM()->getAutoKey();

	if (anim.loop == -1){
		this->addAnim();
	}
}

SkillEffect::~SkillEffect(){
	this->delAnim();
}

void SkillEffect::update(float dt){
	if (this->m_isCanRemove){
		return;
	}

	//尝试触发
	this->onTrigger();

	//尝试结束
	this->onEnd();
}

bool SkillEffect::isCanTrigger(){
	if (this->m_times == INFINITE_TIMES || this->m_curTimes < this->m_times){
		//当前时间
		INT64 curMTimestamp = GM()->getMTimeStamp();

		//下次技能效果开始时间
		INT64 nextMTimestamp = this->m_startMTimestamp + this->m_cd * this->m_curTimes;

		//开始下次效果
		bool b = curMTimestamp >= nextMTimestamp;

		CCLOG("SkillEffect isCanTrigger===>ptr:%p, cur:%lld, next:%lld, curTimes:%d, times:%d", this, curMTimestamp, nextMTimestamp, this->m_curTimes, this->m_times);
		return b;
	}

	return false;
}

void SkillEffect::onTrigger(){
	if (this->isCanTrigger() == false){
		return;
	}

	++this->m_curTimes;
	
	bool b = this->trigger();

	if (b && this->m_anim_data.loop != -1){
		this->addAnim();
	}
}

//触发一次效果，由子类实现
bool SkillEffect::trigger(){
	return false;
}

bool SkillEffect::isCanEnd(){
	if (this->m_times == INFINITE_TIMES){
		return false;
	}

	//当前时间
	INT64 curMTimestamp = GM()->getMTimeStamp();

	//持续时间是否已到,并且触发次数已达上限
	INT64 diff = curMTimestamp - this->m_endMTimestamp;
	bool b = diff >= 0 && this->m_curTimes >= this->m_times;
	if (b) {
		int i = 1;
	}
	CCLOG("SkillEffect isCanEnd===>ptr:%p, cur:%lld, next:%lld, diff:%d, curTimes:%d", this, curMTimestamp, this->m_endMTimestamp, diff, this->m_curTimes);
	return b;
}

void SkillEffect::onEnd(){
	if (this->isCanEnd() == false){
		return;
	}

	this->delAnim();

	this->end();
}

//效果结束，由子类实现
void SkillEffect::end(){

}

bool SkillEffect::isCanRemove(){	
	return this->m_isCanRemove;
}

BaseSprite* SkillEffect::getObj(){
	return m_obj;
}

int SkillEffect::getEffectId(){
	return m_id;
}

SkillEffectType SkillEffect::getType(){
	return m_type;
}

SkillEffectKind SkillEffect::getKind(){
	return m_kind;
}

SkillAnimLayerType SkillEffect::getAnimType(){
	return m_anim_type;
}

void SkillEffect::addAnim(){
	if (this->m_anim_data.layerType == SkillAnimLayerType::None){
		return;
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo(this->m_anim_data.getFilePath());
	this->m_anim_data.obj = this->m_obj;
	this->m_anim_data.targetPos = this->m_obj->getPosition();
	MsgCenter()->dispatch(Msg_AddSkillAnim, &this->m_anim_data);
}

void SkillEffect::delAnim(){
	MsgCenter()->dispatch(Msg_DelSkillAnim, &this->m_anim_data);
}