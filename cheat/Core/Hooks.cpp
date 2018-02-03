#include "Hooks.h"
#include "InputSys.h"
#include "Font.h"
#include "math.h"
#include "../Menu.h"
#include "DrawManager.h"

//Globals
char g_Globals::szDirectoryDLL[MAX_PATH];
char* g_Globals::szLastFunction = "No function was called.";
HMODULE g_Globals::hmDLL = NULL;
C_BaseCombatWeapon* g_Globals::ptrLocalWeapon = nullptr;
CUserCmd* g_Globals::ptrCmd = nullptr;
C_BasePlayer* g_Globals::ptrLocalPlayer = nullptr;
HWND g_Globals::hwndWindow = NULL;
ImFont* Tabs;
ImFont* Default;
ImFont* WeaponTabs;

//Pointer
CMenu* g_pMenu = new CMenu();
bool initialized = false;

namespace Hooks
{
	vfunc_hook HLClientHook;
	vfunc_hook Direct3DHook;
	vfunc_hook VGUIPanelHook;
	vfunc_hook VGUISurfaceHook;
	vfunc_hook MDLRenderHook;
	vfunc_hook ClientModeHook;
	vfunc_hook RenderViewHook;
	vfunc_hook GameEventManagerHook;
	vfunc_hook MatSystemHook;
	vfunc_hook RenderView;
	RecvPropHook* SequenceHook;

	void Initialize()
	{
		// Set up virtual function hooks
		HLClientHook.setup(g_CHLClient);
		Direct3DHook.setup(g_D3DDevice9);
		VGUIPanelHook.setup(g_VGuiPanel);
		VGUISurfaceHook.setup(g_VGuiSurface);
		MDLRenderHook.setup(g_ModelRender);
		ClientModeHook.setup(g_ClientMode);
		GameEventManagerHook.setup(g_GameEvents);
		MatSystemHook.setup(g_MatSystem);
		RenderView.setup(g_RenderView);
		// Hook functions

		HLClientHook.hook_index(Index::FrameStageNotify, hkFrameStageNotify);
		HLClientHook.hook_index(Index::CreateMove, hkCreateMove_Proxy);

		Direct3DHook.hook_index(Index::Reset, hkReset);
		Direct3DHook.hook_index(Index::Present, hkPresent);

		VGUIPanelHook.hook_index(Index::PaintTraverse, hkPaintTraverse);

		VGUISurfaceHook.hook_index(Index::PlaySound, hkPlaySound);
		GameEventManagerHook.hook_index(Index::FireEventClientSide, hkFireEventClientSide);
		ClientModeHook.hook_index(Index::GetViewmodelFOV, hkGetViewmodelFOV);

		RenderView.hook_index(9, Hooked_SceneEnd);

		auto SequenceProp = C_BaseViewModel::GetSequenceProp();
		SequenceHook = new RecvPropHook(SequenceProp, hkSequenceProxyFn);

		Font::Get().Create();
	}

	void Kill()
	{
		// Kill all hooks
		HLClientHook.unhook_all();
		Direct3DHook.unhook_all();
		VGUIPanelHook.unhook_all();
		VGUISurfaceHook.unhook_all();
		MDLRenderHook.unhook_all();
		ClientModeHook.unhook_all();
		GameEventManagerHook.unhook_all();
		MatSystemHook.unhook_all();
	}
	void __stdcall Hooked_SceneEnd() 
	{ 
		static auto ofunc = RenderView.get_original<SceneEnd_t>(9); 
		ofunc(g_RenderView); 

		if (g_Options.Visuals.chamsenable)
		{
			constexpr float color_gray[4] = { 166, 167, 169, 255 };
			constexpr float Red[3] = { 1, 0, 0 };
			constexpr float Green[3] = { 0, 1, 0, 255 };
			IMaterial *mat =
				(g_Options.Visuals.chamstype < 2 ?
					g_MatSystem->FindMaterial("chams", TEXTURE_GROUP_MODEL) :
					g_MatSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL));

			if (!mat || mat->IsErrorMaterial())
				return;

			for (int i = 1; i < g_GlobalVars->maxClients; ++i) {
				auto ent = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
				if (ent && ent->IsAlive() && !ent->IsDormant()) {

					if (g_Options.Visuals.Enable && ent->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
						continue;

					if (g_Options.Visuals.chamstype == 1 || g_Options.Visuals.chamstype == 3)
					{	// XQZ Chams
						g_RenderView->SetColorModulation(1, 0, 0);

						mat->IncrementReferenceCount();
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

						g_ModelRender->ForcedMaterialOverride(mat);

						ent->DrawModel(0x1, 255);
						g_ModelRender->ForcedMaterialOverride(nullptr);

						g_RenderView->SetColorModulation(0, 1, 0);

						mat->IncrementReferenceCount();
						mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

						g_ModelRender->ForcedMaterialOverride(mat);

						ent->DrawModel(0x1, 255);
						g_ModelRender->ForcedMaterialOverride(nullptr);
					}
					else
					{	// Normal Chams
						g_RenderView->SetColorModulation(0, 1, 0);

						g_ModelRender->ForcedMaterialOverride(mat);

						ent->DrawModel(0x1, 255);

						g_ModelRender->ForcedMaterialOverride(nullptr);
					}
				}
			}
		}
	}


	//--------------------------------------------------------------------------------
	HRESULT __stdcall hkPresent(IDirect3DDevice9* pDevice, const RECT* pSourceRect,const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
	{
		IDirect3DStateBlock9* State;
		pDevice->CreateStateBlock(D3DSBT_ALL, &State);
		static bool done = false;

		if (!done)
		{
			g_pDraw->Init(pDevice);
			ImGui_ImplDX9_Init(g_Globals::hwndWindow, pDevice);
			ImGuiIO& io = ImGui::GetIO();
			Default = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14.0f);
			Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\skeettab.ttf", 45.0f);
			WeaponTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\AstriumWep.ttf", 25.0f);
			g_pMenu->InitializeTheme();

			done = initialized = true;
		}

		if (g_Options.MISC_RankReveal && InputSys::Get().IsKeyDown(VK_TAB))
			Utilities::RevealRanks();

		g_pDraw->Reset();

		DWORD dwOld_D3DRS_COLORWRITEENABLE;

		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

		// Hierhin ESP
		// e.g: g_pESP->Think();

		ImGui_ImplDX9_NewFrame();

		g_pDraw->String(8, 8, FONT_LEFT, g_pDraw->fonts.esp, true, WHITE(255), "mythic");
		g_pDraw->String(8, 20, FONT_LEFT, g_pDraw->fonts.esp_small, true, WHITE(255), "%s", __DATE__);

		g_pMenu->Think();

		ImGuiStyle &style = ImGui::GetStyle();

		if (g_pMenu->m_opened)
		{
			if (style.Alpha > 1.f)
				style.Alpha = 1.f;
			else if (style.Alpha != 1.f)
				style.Alpha += 0.01f;
		}
		else
		{
			if (style.Alpha < 0.f)
				style.Alpha = 0.f;
			else if (style.Alpha != 0.f)
				style.Alpha -= 0.01f;
		}

		ImGui::Render();

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

		State->Apply();
		State->Release();

		static auto oPresent = Direct3DHook.get_original<Present_t>(Index::Present);
		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		auto ofunc = Direct3DHook.get_original<Reset>(Index::Reset);

		if (!initialized)
			return ofunc(device, pPresentationParameters);

		ImGui_ImplDX9_InvalidateDeviceObjects();

		auto res = ofunc(device, pPresentationParameters);

		ImGui_ImplDX9_CreateDeviceObjects();

		return res;
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkPlaySound(const char* name)
	{
		static auto ofunc = VGUISurfaceHook.get_original<PlaySound>(Index::PlaySound);
		ofunc(g_VGuiSurface, name);

		if (g_Options.MISC_AutoAccept)
		{
			// Auto Accept
			if (strstr(name, "!UI/competitive_accept_beep.wav"))
			{
				static auto fnAccept = (void(*)())Utilities::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");

				fnAccept();

				//This will flash the CSGO window on the taskbar
				//so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		switch (stage)
		{
		case FRAME_UNDEFINED:
			break;
		case FRAME_START:
			break;
		case FRAME_NET_UPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:
			break;
		case FRAME_RENDER_END:
			break;
		}


		static auto ofunc = HLClientHook.get_original<FrameStageNotify>(Index::FrameStageNotify);
		ofunc(g_CHLClient, stage);

		SkinChanger(stage);
	}
	//--------------------------------------------------------------------------------
	float __stdcall hkGetViewmodelFOV()
	{
		static auto ofunc = ClientModeHook.get_original<GetViewmodelFOV>(Index::GetViewmodelFOV);
		if (g_EngineClient->IsTakingScreenshot()) return ofunc();
		else return ofunc() + g_Options.MISC_ViewmodelFOV;
	}
	//--------------------------------------------------------------------------------

}