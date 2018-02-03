#define NOMINMAX
#include "stdafx.h"
#include "Utilities.h"
#include "SDK.h"
#include "Hooks.h"
#include "InputSys.h"
#include "../Cheat/Cheats.h"
#include "../Menu.h"
#include "DrawManager.h"
#include <fstream>

HMODULE Library;

void Cheat(LPVOID lpReserved)
{
	if (Utilities::WaitForModules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
		return;

	/* find window for wndproc hook etc */
	while (!(g_Globals::hwndWindow = FindWindowA("Valve001", 0)))
		std::this_thread::sleep_for(50ms);

	//Utilities::SetupConsole();
	Utilities::ConsolePrint("Initializing...");
	Interfaces::Initialize();
	NetvarSys::Get().Initialize();
	InputSys::Get().Initialize();
	Hooks::Initialize();
	Utilities::ConsolePrint("Done!");

	//InputSys::Get().RegisterHotkey(VK_DELETE, [lpReserved]()
	//{
	//	Die = true;
	//});

	Utilities::ConsolePrint("Ready!");

	while (!Die)
		Sleep(500);

	Utilities::ConsolePrint("Killing!");

	FreeLibraryAndExitThread(Library, true);
}

DWORD WINAPI DllMain(HMODULE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			Library = hinstDLL;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE) Cheat, lpReserved, 0, 0);
			return TRUE;
		}
		case DLL_PROCESS_DETACH:
		{
			Utilities::CloseConsole();
			Hooks::Kill();
			return TRUE;
		}
		default:
			break;
	}

	return FALSE;
}