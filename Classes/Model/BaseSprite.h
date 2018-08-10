#ifndef __BaseSprite_H__
#define __BaseSprite_H__

#include "Utils/PublicDefault.h"
#include "Skill/SkillEffect/SkillEffect.h"

class SkillMgr;
class AIMgr;


static const int STATE_IDLE = 0;
static const int STATE_RUN = 1;
static const int STATE_ATK = 2;
static const int STATE_SKILL = 3;
static const int STATE_DEATH = 4;
static const int STATE_YUN = 5;
static const int STATE_WIN = 6;

class BaseSprite : public cocos2d::Sprite
{
public:
    virtual bool init();
    CREATE_FUNC(BaseSprite);
    BaseSprite();
    virtual ~BaseSprite();

public:
	virtual void update(float dt);

    virtual void atk();
    virtual void run();
	virtual void hurt(int hurtType, int x, BaseSprite* atk);
	virtual void hurtEffect(int x);
	virtual void death();
    virtual void idle();
    virtual bool isDeath();
	virtual void setSelect(bool b);
	virtual int  getDir();
	virtual void setState(int state, int dir);
	virtual void showName(bool b);
	virtual void setObjName(string name);
	void showSkillRange();

	virtual void addSkillEffectAnim(SkillEffect* skillEffect);
	virtual void delSkillEffectAnim(SkillEffect* skillEffect);

	virtual void addSkillEffect(int skillEffectId, BaseSprite* caster);
	virtual void delSkillEffect(int skillEffectId);
	virtual void triggerSkill(SkillTriggerType tt, const Vec2& targetPos);

	virtual string getObjName();
	
	virtual int getHp();
	virtual int getDamage();
	virtual int getDef();
	virtual int getSpeed();
	virtual int getShootRange();
	virtual int getXiXue();
	virtual int getHurtMore();

	virtual Text* flyHurtNum(int value, Vec2 textPos);

public:
	int _objType;
	int _id;
	bool _isbroken;
	
	AIMgr* _ai;

	string _cur_anima_key;
	list<Vec2> _prePosList;
    Vec2 _pos;
	int _state;					//����״̬

	SkillMgr*				_mgr_skill;
	set<EffectState>		_obj_states;	//�����ܵ��ļ���״̬
	map<int, SkillEffect*>	_skill_effects;

	
	int _camp;			

    int _totalHP;		//��������ֵ
	int _xixue;			//��Ѫ
	int _hurt_more;		//�˺�����

	int _quality;		
	int _radius;
	int _dir;
	int _shootType;
	int _healthPoint;	//��ǰ����ֵ
	int _dotX; 
	int _dotY;
	int _speed;			//�ٶ�
	int _speed_diff;	//�ٶȶ�������
    int _shoot_range;	//��������
    int _shoot_range_diff;	//��������
	int _speed_per;		//�ٶȰٷֱ�
	int _damage;		//�˺�
	int _damage_diff;	//�˺���������
	int _damage_per;	//�˺��ٷֱ�
	int _def;			//����
	int _def_diff;		//������������
	int _def_per;		//�����ٷֱ�
	int _crit;			//����ֵ
	int _crit_rate;		//������

	int _life_left;		//�����������ֵ���ᴥ������

	BaseSprite* _target;
	Armature* _arm;
	string _animaName;
	string _objname;
	Text* _txtName;
	Text* _txt_hp;
	LoadingBar* _hpBar; // Ѫ��

	map<SkillEffect*, Armature*> _skill_effect_anims;
};


#endif // __BaseSprite_H__
