#pragma once
#include "../Core/stdafx.h"
#include "../SDK/csgostructs.h"
#include "../Core/singleton.h"
#include "Draw.h"
#pragma once
struct player_esp
{
	C_BasePlayer* pPlayer = nullptr;
	bool is_visible = false;
	bool render = false;
	int screen_x = 0, screen_y = 0, width = 0, height = 0;
	DWORD color = D3DCOLOR_RGBA(255, 255, 255, 255);

	player_esp()
	{

	}
};
class CESP
{
public:
	player_esp player_draw_array[64];
	float flPlayerAlpha[64];
	bool Begin(C_BasePlayer* pl);
	void Armorbar(C_BasePlayer* pPlayer);
	void DrawPlayer(C_BasePlayer* pPlayer, C_BasePlayer* pLocalPlayer);
	void DrawC4(C_BasePlayer* pBomb, bool is_planted, C_BasePlayer* pLocalPlayer);

	void DrawThrowable(C_BasePlayer* pThrowable);
	void DrawDroppedWeapon(C_BaseCombatWeapon* pWeapon);
	void Loop();


}; extern CESP* g_ESP;