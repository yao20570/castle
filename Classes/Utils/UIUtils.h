#pragma once

#include "Utils/PublicDefault.h"

//Æ·ÖÊ
static void setTextColorByQuality(Text* text, int quality) {
	switch (quality) {
		case 1://°×
			text->setTextColor(Color4B(255, 255, 255, 255));
			break;
		case 2://ÂÌ
			text->setTextColor(Color4B(0, 128, 00, 255));
			break;
		case 3://À¶
			text->setTextColor(Color4B(0, 0, 255, 255));
			break;
		case 4://³È
			text->setTextColor(Color4B(128, 0, 128, 255));
			break;
		case 5://×Ï
			text->setTextColor(Color4B(255, 165, 0, 255));
			break;
		case 6://ºì
			text->setTextColor(Color4B(255, 0, 0, 255));
			break;
	}
};

//Îä½«ÖÖÀà
static void setTextColorByKind(Text* text, int kind) {
	switch (kind)
	{
		case 1:
			//Ì¹¿Ë-ºì
			text->setTextColor(Color4B(255, 0, 0, 255));
			break;
		case 2:
			//¸¨Öú-ÂÌ
			text->setTextColor(Color4B(00, 128, 00, 255));
			break;
		case 3:
			//½üÕ½-³È
			text->setTextColor(Color4B(128, 0, 128, 255));
			break;
		case 4:
			//Ô¶³Ì-À¶
			text->setTextColor(Color4B(0, 0, 255, 255));
			break;
	}
}