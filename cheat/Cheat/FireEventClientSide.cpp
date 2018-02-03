#include "../Core/Hooks.h"

//--------------------------------------------------------------------------------
bool __stdcall Hooks::hkFireEventClientSide(IGameEvent* event)
{

	static auto ofunc = GameEventManagerHook.get_original<FireEventClientSide>(Index::FireEventClientSide);
	return ofunc(g_GameEvents, event);
}