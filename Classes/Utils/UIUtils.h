#pragma once

#include "Utils/PublicDefault.h"

static void setTextColor(Text* text, int quality) {
	switch (quality) {
	case 1:
		text->setTextColor(Color4B(255, 255, 255, 255));
		break;
	case 2:
		text->setTextColor(Color4B(00, 128, 00, 255));
		break;
	case 3:
		text->setTextColor(Color4B(0, 0, 255, 255));
		break;
	case 4:
		text->setTextColor(Color4B(128, 0, 128, 255));
		break;
	case 5:
		text->setTextColor(Color4B(255, 165, 0, 255));
		break;
	case 6:
		text->setTextColor(Color4B(255, 0, 0, 255));
		break;
	}
}