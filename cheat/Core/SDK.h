#pragma once

#define NOMINMAX

#include "stdafx.h"
#include <d3d9.h>

#include "../SDK/Misc/Enums.h"
#include "../SDK/Misc/vfunc.h"

#include "../SDK/Math/VMatrix.h"
#include "../SDK/Math/QAngle.h"
#include "../SDK/Math/Vector.h"
#include "../SDK/Misc/Studio.h"

#include "../SDK/Interfaces/IAppSystem.h"
#include "../SDK/Interfaces/IBaseClientDll.h"
#include "../SDK/Interfaces/IClientEntity.h"
#include "../SDK/Interfaces/IClientEntityList.h"
#include "../SDK/Interfaces/IClientMode.h"
#include "../SDK/Interfaces/IConVar.h"
#include "../SDK/Interfaces/ICvar.h"
#include "../SDK/Interfaces/IEngineTrace.h"
#include "../SDK/Interfaces/IVEngineClient.h"
#include "../SDK/Interfaces/IVDebugOverlay.h"
#include "../SDK/Interfaces/ISurface.h"
#include "../SDK/Interfaces/CInput.h"
#include "../SDK/Interfaces/IVModelInfoClient.h"
#include "../SDK/Interfaces/IVModelRender.h"
#include "../SDK/Interfaces/IRenderView.h"
#include "../SDK/Interfaces/IGameEventmanager.h"
#include "../SDK/Interfaces/IMaterialSystem.h"
#include "../SDK/Interfaces/IMoveHelper.h"
#include "../SDK/Interfaces/IPrediction.h"
#include "../SDK/Interfaces/IPanel.h"
#include "../SDK/Interfaces/IEngineSound.h"
#include "../SDK/Interfaces/IViewRender.h"
#include "../SDK/Interfaces/CClientState.h"
#include "../SDK/Interfaces/IPhysics.h"
#include "../SDK/Interfaces/ILocalize.h"

#include "../SDK/Misc/Convar.h"
#include "../SDK/Misc/CUserCmd.h"
#include "../SDK/Misc/glow_outline_effect.h"
#include "../SDK/Misc/KeyValues.h"

#include "../SDK/netvars.h"

namespace Interfaces
{
	void Initialize();
}

typedef void(__cdecl *MsgFn)(const char *, ...);
typedef void(__cdecl *WarningFn)(const char *, ...);
extern MsgFn g_Msg;
extern WarningFn g_Warning;
extern IVEngineClient* g_EngineClient;
extern IBaseClientDLL* g_CHLClient;
extern IClientEntityList* g_EntityList;
extern CGlobalVarsBase* g_GlobalVars;
extern IEngineTrace* g_EngineTrace;
extern ICvar* g_CVar;
extern IPanel* g_VGuiPanel;
extern IClientMode* g_ClientMode;
extern IVDebugOverlay* g_DebugOverlay;
extern ISurface* g_VGuiSurface;
extern CInput* g_Input;
extern IVModelInfoClient* g_ModelInfo;
extern IVModelRender* g_ModelRender;
extern IVRenderView* g_RenderView;
extern IMaterialSystem* g_MatSystem;
extern IGameEventManager2* g_GameEvents;
extern IMoveHelper* g_MoveHelper;
extern IPrediction* g_Prediction;
extern CGameMovement* g_GameMovement;
extern IEngineSound* g_EngineSound;
extern CGlowObjectManager* g_GlowObjManager;
extern IViewRender* g_ViewRender;
extern IDirect3DDevice9* g_D3DDevice9;
extern CClientState* g_ClientState;
extern IPhysicsSurfaceProps* g_PhysSurface;
extern ILocalize* g_Localize;
extern bool Die;
#include "../SDK/Misc/EHandle.h"

class C_LocalPlayer
{
	friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
	C_LocalPlayer() : m_local(nullptr) {}

	operator bool() const { return *m_local != nullptr; }
	operator C_BasePlayer*() const { return *m_local; }

	C_BasePlayer* operator->() { return *m_local; }

private:
	C_BasePlayer** m_local;
};

extern C_LocalPlayer g_LocalPlayer;

class AnimationLayer
{
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038