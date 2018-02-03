#include "../Core/SDK.h"
#include "Skins.h"
#include "../Core/DrawManager.h"
#include "../Core/options.h"
#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
// Store the original proxy functions.
RecvVarProxyFn oSequenceProxyFn = NULL;

bool requestFullUpdate = false;
int Random(int min, int max)
{
	if (min == max)
		return min;

	if (max % 2 == 0)
		return min + (rand() % (max - min));

	return min + (rand() % (max - min + 1));
}
class SkinChangerInfo
{
public:
	char model[128];
	int fallbackPaintKit;
	int itemDefinitionIndex;

	SkinChangerInfo(char *model_, int fallbackPaintKit_, int itemDefinitionIndex_)
	{
		strcpy(model, model_);
		fallbackPaintKit = fallbackPaintKit_;
		itemDefinitionIndex = itemDefinitionIndex_;
	}
};

//RecvVarProxyFn oRecvnModelIndex = NULL;

void SkinChanger(ClientFrameStage_t Stage)
{
	C_BasePlayer *pLocal = g_Globals::ptrLocalPlayer;

	static std::vector<SkinChangerInfo> knifeModelReplace;

	if (knifeModelReplace.size() == 0)
	{
		knifeModelReplace.push_back(SkinChangerInfo(XorStr("models/weapons/v_knife_karam.mdl"), 38, WEAPON_KNIFE_KARAMBIT));
		knifeModelReplace.push_back(SkinChangerInfo(XorStr("models/weapons/v_knife_m9_bay.mdl"), 421, WEAPON_KNIFE_M9_BAYONET));
		knifeModelReplace.push_back(SkinChangerInfo(XorStr("models/weapons/v_knife_butterfly.mdl"), 12, WEAPON_KNIFE_BUTTERFLY));
	}

	if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (!requestFullUpdate && g_Options.knifemodel != 0)
		{
			UINT* hWeapons = pLocal->getWeapons();

			if (hWeapons)
			{
				int nLocalPlayerID = g_EngineClient->GetLocalPlayer();

				player_info_t LocalPlayerInfo;
				g_EngineClient->GetPlayerInfo(nLocalPlayerID, &LocalPlayerInfo);

				static int default_t = g_ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_default_t.mdl"));
				static int default_ct = g_ModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_default_ct.mdl"));

				int replace = g_ModelInfo->GetModelIndex(knifeModelReplace[g_Options.knifemodel - 1].model);

				// Loop through weapons and run our skin function on them.
				for (int nIndex = 0; hWeapons[nIndex]; nIndex++)
				{
					C_BaseAttributableItem* pWeapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(hWeapons[nIndex]);

					if (!pWeapon)
						continue;

					// Compare original owner XUIDs.
					if (LocalPlayerInfo.xuid_low != pWeapon->m_OriginalOwnerXuidLow())
						continue;

					if (LocalPlayerInfo.xuid_high != pWeapon->m_OriginalOwnerXuidHigh())
						continue;

					if (pWeapon->GetClientClass()->m_ClassInfo == 93) // knives
					{
						pWeapon->m_nFallbackPaintKit() = knifeModelReplace[g_Options.knifemodel - 1].fallbackPaintKit;
						pWeapon->m_iItemDefinitionIndex() = knifeModelReplace[g_Options.knifemodel - 1].itemDefinitionIndex;

						pWeapon->m_iItemIDHigh() = -1;
						pWeapon->m_flFallbackWear() = 0.0001f;
						pWeapon->m_iEntityQuality() = 3;
						/**pWeapon->GetViewModelIndex() = replace;
						*pWeapon->GetWorldModelIndex() = replace + 1;*/

						pWeapon->SetModelIndex(replace);

						// Fix up the account ID so StatTrak will display correctly.
						pWeapon->m_iAccountID() = LocalPlayerInfo.xuid_low;

						C_BaseViewModel *pViewModel = (C_BaseViewModel*)g_EntityList->GetClientEntityFromHandle(pLocal->GetViewModel());
						C_BaseAttributableItem *pViewModelWeapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(pViewModel->GetWeapon());
						C_BaseAttributableItem *pWorldModel = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(pWeapon->GetWorldModel());

						if (pWorldModel)
						{
							pWorldModel->SetModelIndex(replace + 1);
						}

						if (pLocal->GetViewModel() != -1)
						{
							if (pViewModelWeapon == pWeapon)
							{
								pViewModel->SetModelIndex(replace);
							}
						}
					}
					else
					{
						if (pWeapon->m_iItemDefinitionIndex() == WEAPON_SCAR20)
						{
							pWeapon->m_nFallbackPaintKit() = 597;
						}
						else if (pWeapon->m_iItemDefinitionIndex() == WEAPON_G3SG1)
						{
							pWeapon->m_nFallbackPaintKit() = 465;
						}
						else if (pWeapon->m_iItemDefinitionIndex() == WEAPON_AK47)
						{
							pWeapon->m_nFallbackPaintKit() = 639;
						}
						else if (pWeapon->m_iItemDefinitionIndex() == WEAPON_AWP)
						{
							pWeapon->m_nFallbackPaintKit() = 279;
						}
						else if (pWeapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER)
						{
							pWeapon->m_nFallbackPaintKit() = 595;
						}

						pWeapon->m_iItemIDHigh() = -1;
						pWeapon->m_flFallbackWear() = 0.0001f;
						pWeapon->m_iEntityQuality() = 3;
						pWeapon->m_iAccountID() = LocalPlayerInfo.xuid_low;
					}
				}
			}
		}

		if (requestFullUpdate)
		{
			static ConCommand * cl_fullupdate = NULL;

			if (!cl_fullupdate)
				cl_fullupdate = g_CVar->FindCommand(XorStr("cl_fullupdate"));

			if (cl_fullupdate)
			{
				cl_fullupdate->m_fnCommandCallbackV1();
			}

			requestFullUpdate = false;
		}
	}
}


