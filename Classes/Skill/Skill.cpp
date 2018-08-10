
#include "Skill.h"

#include "Dlg/Fight/AIMgr.h"
#include "Model/BaseSprite.h"
#include "Utils/JsonManager.h"
#include "Core/Msg/MsgMgr.h"

Skill::Skill(BaseSprite* obj, AIMgr* ai, const rapidjson::Value& skillCfg)
	:m_obj(obj)
	, m_ai(ai)
	, m_useMSec(0)
	, m_CDMTimestamp(0)
{
	this->Id = skillCfg["ID"].GetInt();
	this->Name = skillCfg["Name"].GetString();
	this->triggerType = SkillTriggerType(skillCfg["Trigger"].GetInt());
	this->targetType = SkillTargetType(skillCfg["Target"].GetInt());
	this->scopeType = SkillScopeType(skillCfg["ScopeType"].GetInt());
	this->radius = skillCfg["Radius"].GetInt();
	this->shootRange = skillCfg["ShootRange"].GetInt();
	this->cd = skillCfg["CD"].GetInt();

	//效果
	{
		const rapidjson::Value &pArray = skillCfg["Detail"];
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)  {
			const rapidjson::Value& effectId = pArray[i];
			this->m_effectIds.push_back(effectId.GetInt());
		}
	}


	//触发条件
	{
		const rapidjson::Value &pArray = skillCfg["Condition"];
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)  {
			const rapidjson::Value& conditionCfg = pArray[i];
			this->m_conditions.emplace_back();
			SkilCondition& condition = this->m_conditions.back();
			condition.type = (SkillTriggerCondition)conditionCfg["Type"].GetInt();
			condition.value = conditionCfg["Value"].GetInt();
		}
	}

	//动画
	{
		char str[256] = {0};

		const rapidjson::Value &pArray = skillCfg["Amin"];
		for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)  {
			const rapidjson::Value& animCfg = pArray[i];
			this->m_anims.emplace_back();
			SkilAnimData& anim = this->m_anims.back();
			anim.layerType = (SkillAnimLayerType)animCfg["LayerType"].GetInt();
			anim.posType = (SkillAnimPosType)animCfg["PosType"].GetInt();
			anim.loop = animCfg["Loop"].GetInt();
			anim.fileName = animCfg["FileName"].GetString();
			anim.key = GM()->getAutoKey();
		}
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

bool Skill::checkCondition(){
	for (SkilCondition& condition : this->m_conditions){
		switch (condition.type)
		{
			case SkillTriggerCondition::HpLeft:{
				int per = (float)this->m_obj->getHp() / (float)this->m_obj->_totalHP * 100;
				if (per > condition.value){
					return false;
				}
				break;
			}
			case SkillTriggerCondition::Random:{
				int n = rand() % 100;
				CCLOG("===========>rand() : %d", n);
				if (n > condition.value){
					return false;
				}
				break;
			}
		}
	}
	return true;
}

void Skill::useSkill(const Vec2& targetPos){

	//特殊条件判断
	if (this->checkCondition() == false){
		return;
	}

	//技能效果开始,结束时间
	INT64 curMTimeStamp = GM()->getMTimeStamp();
	if (curMTimeStamp < this->m_CDMTimestamp){
		return;
	}

	//通过目标类型筛选
	const set<BaseSprite*>& targets = this->getObjsByTarget();
	
	//通过范围筛选
	set<BaseSprite*> retTarget;
	this->getObjsByScope(targetPos, targets, retTarget);

	for (auto obj : retTarget){
		for (auto skillEffectId : this->m_effectIds){
			obj->addSkillEffect(skillEffectId, this->m_obj);
		}
	}

	//播放动画	
	switch (this->scopeType)
	{
		case SkillScopeType::SINGLE:
			if (retTarget.empty()){
			}
			else{
				this->playAnim(targetPos);
				this->m_CDMTimestamp = curMTimeStamp + this->cd;
			}
			break;
		case SkillScopeType::ALL:
		case SkillScopeType::ROUND:
		case SkillScopeType::RECT_SCOPE:
		case SkillScopeType::FAN:
			this->playAnim(targetPos);
			this->m_CDMTimestamp = curMTimeStamp + this->cd;
			break;
	}
	
}

void Skill::playAnim(const Vec2& targetPos){
	for (auto& it : this->m_anims){
		ArmatureDataManager::getInstance()->addArmatureFileInfo(it.getFilePath());
		it.obj = this->m_obj;
		if (SkillScopeType::RECT_SCOPE == this->scopeType){

			//技能释放角度
			Vec2 temp1 = targetPos - this->m_obj->getPosition();
			float radians = temp1.getAngle();
			it.angle = CC_RADIANS_TO_DEGREES(-radians);

			int distance = this->m_obj->getPosition().distance(targetPos);
			float rate = (float)this->shootRange / distance;
			int x = this->m_obj->getPositionX() + (targetPos.x - this->m_obj->getPositionX()) * rate; 
			int y = this->m_obj->getPositionY() + (targetPos.y - this->m_obj->getPositionY()) * rate; 
			it.targetPos = Vec2(x, y);
		}
		else{
			it.targetPos = targetPos;
		}
		MsgCenter()->dispatch(Msg_AddSkillAnim, &it);
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
		case SkillScopeType::SINGLE:		this->getObjBySingle(targetPos, inObjs, outObjs);	break;
		case SkillScopeType::ALL:			this->getObjByALL(targetPos, inObjs, outObjs);		break;
		case SkillScopeType::ROUND:			this->getObjByRound(targetPos, inObjs, outObjs);	break;
		case SkillScopeType::RECT_SCOPE:	this->getObjByRect(targetPos, inObjs, outObjs);		break;
		case SkillScopeType::FAN:			this->getObjByFan(targetPos, inObjs, outObjs);		break;
	}
}

void Skill::getObjBySingle(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){

	BaseSprite* nearOjb = nullptr;
	int nearDistance = 0;
	for (auto it : inObjs){
		Vec2 pos = it->getPosition() + Vec2(0, 60);
		int distance = pos.distance(targetPos);
		if (distance < 80){
			if (nearOjb == nullptr || distance < nearDistance){
				nearOjb = it;
				nearDistance = distance;
			}
		}
	}

	if (nearOjb){
		outObjs.insert(nearOjb);
	}	
}

void Skill::getObjByALL(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	for (auto it : inObjs){
		outObjs.insert(it);
	}
}

void Skill::getObjByRound(const Vec2& targetPos, const set<BaseSprite*>& inObjs, set<BaseSprite*>& outObjs){
	int distance = this->m_obj->getPosition().distance(targetPos);
	int diff = 20; //给一个偏移值
	if (distance > this->shootRange + diff){
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
		temp2.rotate(Vec2(0, 0), -angle);
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
