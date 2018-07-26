
#include "Skill.h"

#include "Dlg/Fight/AIMgr.h"
#include "Model/BaseSprite.h"
#include "Utils/JsonManager.h"

Skill::Skill(BaseSprite* obj, AIMgr* ai, int skillId)
	:m_obj(obj)
	, m_ai(ai)
	, m_useMSec(0)
	, m_CDMTimestamp(0)
{
	const rapidjson::Value& skillCfg = JsonMgr()->getSkillById(skillId);
	this->Id = skillCfg["ID"].GetInt();
	this->Name = skillCfg["Name"].GetString();
	this->triggerType = SkillTriggerType(skillCfg["Trigger"].GetInt());
	this->targetType = SkillTargetType(skillCfg["Target"].GetInt());
	this->scopeType = SkillScopeType(skillCfg["ScopeType"].GetInt());
	this->shootRange = skillCfg["ShootRange"].GetInt();
	this->cd = skillCfg["CD"].GetInt();
	this->radius = skillCfg["Radius"].GetInt();

	//const rapidjson::Value &pArray = skillCfg["Detail"]; 
	//for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)  {
	//	const rapidjson::Value &effectCfg = pArray[i]; 
	//	
	//	effects.emplace_back(SkillEffectType(effectCfg["EffectType"].GetInt()));
	//	SkillEffectInfo& info = effects.back();
	//	info.value = effectCfg["Value"].GetInt();
	//	info.times = effectCfg["Tiems"].GetInt();
	//	info.cd = effectCfg["CD"].GetInt();
	//	info.lastTime = effectCfg["LastTime"].GetInt();
	//}

	const rapidjson::Value &pArray = skillCfg["Detail"]; 
	for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)  {
		const rapidjson::Value& effectId = pArray[i]; 
		this->m_effectIds.push_back(effectId.GetInt());
	}
}

Skill::~Skill(){

}

void Skill::update(float dt){

}

void Skill::reset(){
	struct timeval now;
	gettimeofday(&now, NULL);
	long time = now.tv_usec / 1000;

	this->m_useMSec = time;
}

void Skill::useSkill(const Vec2& targetPos){

		//技能效果开始,结束时间
	INT64 curMTimeStamp = GM()->getMTimeStamp();
	if (curMTimeStamp < this->m_CDMTimestamp){
		return;
	}else{
		this->m_CDMTimestamp = curMTimeStamp + this->cd;
	}

	//通过目标类型筛选
	const set<BaseSprite*>& targets = this->getObjsByTarget();

	
	//通过范围筛选
	set<BaseSprite*> retTarget;
	this->getObjsByScope(targetPos, targets, retTarget);

	for (auto obj : retTarget){
		for (auto skillEffectId : this->m_effectIds){
			obj->addSkillEffect(skillEffectId);
		}
	}
}

const set<BaseSprite*>& Skill::getObjsByTarget(){

	if (this->m_obj->_camp == 1){
		switch (this->targetType) {
			case SkillTargetType::Enemy:
				return this->m_ai->_objEnemy;
				break;
			case SkillTargetType::Friend:
				return this->m_ai->_objSelf;
				break;
		}
	}
	else{
		switch (this->targetType) {
			case SkillTargetType::Enemy:
				return this->m_ai->_objSelf;
				break;
			case SkillTargetType::Friend:
				return this->m_ai->_objEnemy;
				break;
		}
	}

	CCAssert(false, "");
	return this->m_ai->_objSelf;
}

void Skill::getObjsByScope(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	switch (this->scopeType)
	{
		case SkillScopeType::SINGLE:this->getObjBySingle(targetPos, inObjs, outObjs);	break;
		case SkillScopeType::ALL:	this->getObjByALL(targetPos, inObjs, outObjs);		break;
		case SkillScopeType::ROUND:	this->getObjByRound(targetPos, inObjs, outObjs);	break;
		case SkillScopeType::RECT_SCOPE:	this->getObjByRect(targetPos, inObjs, outObjs);		break;
		case SkillScopeType::FAN:	this->getObjByFan(targetPos, inObjs, outObjs);		break;
	}
}

void Skill::getObjBySingle(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){	
	for (auto it : inObjs){
		if (it->getPosition().distance(targetPos) < 1 ){
			outObjs.insert(it);
			return;
		}
	}
}

void Skill::getObjByALL(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	for (auto it : inObjs){
		outObjs.insert(it);
	}	
}

void Skill::getObjByRound(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	int distance = this->m_obj->getPosition().distance(targetPos);
	if (distance > this->shootRange){
		//超出了施法范围
		return;
	}

	for (auto it : inObjs){
		distance = it->getPosition().distance(targetPos);
		if (distance < this->radius){
			outObjs.insert(it);
		}
	}	
}

void Skill::getObjByRect(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	int distance = this->m_obj->getPosition().distance(targetPos);
	if (distance > this->shootRange){
		//超出了施法范围
		return;
	}
	
	//技能零角度范围
	Rect rect(0, -this->radius, this->shootRange, this->radius * 2);

	//技能释放角度
	Vec2 temp1 = targetPos - this->m_obj->getPosition();
	float angle = temp1.getAngle(); 
		
	//对象是否在范围内
	for (auto it : inObjs){
		Vec2 temp2 = it->getPosition() - this->m_obj->getPosition();
		temp2.rotate(Vec2(0,0), -angle);
		if (rect.containsPoint(temp2)){
			outObjs.insert(it);
		}
	}
}

void Skill::getObjByFan(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	int distance = this->m_obj->getPosition().distance(targetPos);
	if (distance > this->shootRange){
		//超出了施法范围
		return;
	}

	//技能释放角度
	Vec2 temp1 = targetPos - this->m_obj->getPosition();
	float angle1 = temp1.getAngle(); 

	//对象是否在范围内
	for (auto it : inObjs){
		Vec2 temp2 = targetPos - this->m_obj->getPosition();
		float angle2 = temp2.getAngle();
		if (abs(angle2 - angle1) < this->radius){
			outObjs.insert(it);
		}
	}
}
