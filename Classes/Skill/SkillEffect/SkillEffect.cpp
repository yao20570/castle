#include "SkillEffect.h"

#include "Utils/ConfigMgr.h"

SkillEffect::SkillEffect(BaseSprite* obj, int skillEffectId)
	: m_obj(obj)
	, m_isCanRemove(false)
	, m_id(skillEffectId)
	, m_curTimes(0)
{
	auto& cfg = *(CFG()->getSkillEffectById(this->m_id));

	//this->m_type = SkillEffectType(cfg["Type"].asInt());
	this->m_name = cfg["Name"].asString();
	this->m_value = cfg["Value"].asInt();
	this->m_times = cfg["Tiems"].asInt();
	this->m_cd = cfg["CD"].asInt();
	this->m_lastTime = cfg["LastTime"].asInt();

	//����Ч����ʼ,����ʱ��
	this->m_startMTimestamp = GM()->getMTimeStamp();
	this->m_endMTimestamp = GM()->getMTimeStamp() + this->m_lastTime;

	CCLOG("SkillEffect create===>ptr:%p, start:%lld, end:%lld,", this, this->m_startMTimestamp, this->m_endMTimestamp);
}

SkillEffect::~SkillEffect(){
	
}

void SkillEffect::update(float dt){
	if (this->m_isCanRemove){
		return;
	}
	CCLOG("SkillEffect update===>ptr:%p", this);
	//���Դ���
	this->onTrigger();

	//���Խ���
	this->onEnd();
}

bool SkillEffect::isCanTrigger(){
	if (this->m_curTimes < this->m_times){
		//��ǰʱ��
		INT64 curMTimestamp = GM()->getMTimeStamp();

		//�´μ���Ч����ʼʱ��
		INT64 nextMTimestamp = this->m_startMTimestamp + this->m_cd * this->m_curTimes;

		//��ʼ�´�Ч��
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
	
	this->trigger();
}

//����һ��Ч����������ʵ��
void SkillEffect::trigger(){

}

bool SkillEffect::isCanEnd(){
	//��ǰʱ��
	INT64 curMTimestamp = GM()->getMTimeStamp();

	//����ʱ���Ƿ��ѵ�,���Ҵ��������Ѵ�����
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

	this->end();
}

//Ч��������������ʵ��
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