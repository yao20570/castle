#pragma once

#include "Core/BaseComponent.h"
#include "Core/BaseComponentDef.h"


struct FindData {
	int objType;
	int subType;
};




class PnlFind : public BaseComponent
{
public:
	PnlFind();
	virtual ~PnlFind();
	CREATE_COMPONENT_FUNC(PnlFind);
	virtual bool init(DlgBase* dlg);
	virtual void load();

	virtual void setVisible(bool b);

	void setEquipItemInfo(int index, ValueMap* equipCfg);
	void setSoilderItemInfo(int index, ValueMap* equipCfg);
	void setHeroItemInfo(int index, ValueMap* equipCfg);
	void setGoldItemInfo(int index, int goldNum);
	void setDiamondItemInfo(int index, int diamondNum);


public:
	virtual void updateUI();
	void updateGetPanel(int id);
	void onClose(Ref* sender, Widget::TouchEventType type);
	void onGoldFind(Ref* sender, Widget::TouchEventType type);
	void onDiamondFind(Ref* sender, Widget::TouchEventType type);
private:

private:

	Layout*			pnl_reward_container;
	Layout*			pnl_items[9];

	Layout*			pnl_gold_find;
	Layout*			pnl_diamond_find;

	Layout*			pnl_get;
	int				findCount = 0;

	ImageView*		img_half;
	Text*			txt_name;
	
};