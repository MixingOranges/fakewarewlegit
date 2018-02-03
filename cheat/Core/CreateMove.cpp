#include "Hooks.h"
#include "DrawManager.h"
#include "../Cheat/Legitbot.h"
__declspec(naked) void __stdcall Hooks::hkCreateMove_Proxy(int Sequence, float Frametime, bool Active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		lea  ecx, [esp]
		push ecx
		push dword ptr[Active]
		push dword ptr[Frametime]
		push dword ptr[Sequence]
		call Hooks::hkCreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

//--------------------------------------------------------------------------------

void __stdcall Hooks::hkCreateMove(int Sequence, float Frametime, bool Active, bool& bSendPacket)
{
	auto ofunc = HLClientHook.get_original<CreateMove>(Index::CreateMove);
	ofunc(g_CHLClient, Sequence, Frametime, Active);

	auto cmd = g_Input->GetUserCmd(Sequence);
	auto verified = g_Input->GetVerifiedCmd(Sequence);
	g_Globals::ptrLocalPlayer = reinterpret_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	g_Globals::ptrCmd = cmd;
	auto weapon = g_Globals::ptrLocalPlayer->m_hActiveWeapon().Get();
	if (!cmd || !cmd->command_number)
		return;

	if (g_Options.MISC_Bunnyhop && g_LocalPlayer && g_LocalPlayer->GetMoveType() != MOVETYPE_LADDER)
	{
		static bool bLastJumped = false;
		static bool bShouldFake = false;
		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			cmd->buttons |= IN_JUMP;
		} else if (cmd->buttons & IN_JUMP)
		{
			if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			{
				bLastJumped = true;
				bShouldFake = true;
			} else
			{
				cmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		} else
		{
			bLastJumped = false;
			bShouldFake = false;
		}
	}
	legitbot::Get().on_create_move(cmd, g_Globals::ptrLocalPlayer, weapon);
	TimeWarp::Get().CreateMove(cmd);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}