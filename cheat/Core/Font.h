#pragma once
#define NOMINMAX
#include "SDK.h"
#include <Windows.h>
#include "singleton.h"

class Font: public Singleton<Font>
{
public:
	vgui::HFont Overlay;
	vgui::HFont MenuText;
	vgui::HFont ESP;
	vgui::HFont ESPMini;
	vgui::HFont font_espnum;

	void Create();
};
