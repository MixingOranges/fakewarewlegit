#include "csgostructs.h"
#include "../Core/Math.h"
#include "../Core/Utilities.h"
#include "../Core/DrawManager.h"
bool C_BaseEntity::IsPlayer()
{
	//index: 152
	//ref: "effects/nightvision"
	//sig: 8B 92 ? ? ? ? FF D2 84 C0 0F 45 F7 85 F6
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
}

bool C_BaseEntity::IsWeapon()
{
	//index: 160
	//ref: "CNewParticleEffect::DrawModel"
	//sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
	return CallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
}

bool C_BaseEntity::IsPlantedC4()
{
	return GetClientClass()->m_ClassInfo == CPlantedC4;
}
void C_BaseEntity::SetModelIndex(int nModelIndex) {
	// DT_BaseEntity -> m_nModelIndex_
	*(int*)((DWORD)this + 0x254) = nModelIndex;
}
bool C_BaseEntity::IsDefuseKit()
{
	return GetClientClass()->m_ClassInfo == CBaseAnimating;
}
bool C_BasePlayer::IsVisible(int bone)
{
	Ray_t ray;
	trace_t tr;
	m_visible = false;
	ray.Init(g_Globals::ptrLocalPlayer->GetEyePos(), this->GetBonePosition(bone));

	CTraceFilter filter;
	filter.pSkip = g_Globals::ptrLocalPlayer;

	g_EngineTrace->TraceRay(ray, MASK_NPCWORLDSTATIC | CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX, &filter, &tr);

	if (tr.m_pEnt == this || tr.fraction > 0.99f)
	{
		m_visible = true;
		return true;
	}

	return false;
}
CCSWeaponInfo* C_BaseCombatWeapon::GetCSWeaponData()
{
	static auto fnGetWpnData
		= reinterpret_cast<CCSWeaponInfo*(__thiscall*)(void*)>(
			Utilities::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B")
			);
	return fnGetWpnData(this);
}

bool C_BaseCombatWeapon::HasBullets()
{
	return !IsReloading() && m_iClip1() > 0;
}

bool C_BaseCombatWeapon::CanFire()
{
	if (IsReloading() || m_iClip1() <= 0)
		return false;

	if (!g_LocalPlayer)
		return false;

	float flServerTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	return m_flNextPrimaryAttack() <= flServerTime;
}

bool C_BaseCombatWeapon::IsGrenade()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_GRENADE;
}

bool C_BaseCombatWeapon::IsKnife()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_KNIFE;
}

bool C_BaseCombatWeapon::IsRifle()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_RIFLE;
}

bool C_BaseCombatWeapon::IsPistol()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_PISTOL;
}

bool C_BaseCombatWeapon::IsSniper()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_SNIPER_RIFLE;
}

bool C_BaseCombatWeapon::IsSMG()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_SUBMACHINEGUN;
}

bool C_BaseCombatWeapon::IsShotgun()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_SHOTGUN;
}

bool C_BaseCombatWeapon::IsHeavy()
{
	return GetCSWeaponData()->WeaponType == WEAPONTYPE_MACHINEGUN;
}

bool C_BaseCombatWeapon::IsReloading()
{
	static auto inReload = *(uint32_t*)(Utilities::PatternScan(GetModuleHandleW(L"client.dll"), "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
	return *(bool*)((uintptr_t)this + inReload);
}

float C_BaseCombatWeapon::GetInaccuracy()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 469)(this);
}

float C_BaseCombatWeapon::GetSpread()
{
	return CallVFunction<float(__thiscall*)(void*)>(this, 439)(this);
}

void C_BaseCombatWeapon::UpdateAccuracyPenalty()
{
	CallVFunction<void(__thiscall*)(void*)>(this, 470)(this);
}

CUserCmd*& C_BasePlayer::m_pCurrentCommand()
{
	static auto currentCommand = *(uint32_t*)(Utilities::PatternScan(GetModuleHandleW(L"client.dll"), "89 BE ? ? ? ? E8 ? ? ? ? 85 FF") + 2);
	return *(CUserCmd**)((uintptr_t)this + currentCommand);
}

Vector C_BasePlayer::GetEyePos()
{
	return m_vecOrigin() + m_vecViewOffset();
}

player_info_t C_BasePlayer::GetPlayerInfo()
{
	player_info_t info;
	g_EngineClient->GetPlayerInfo(EntIndex(), &info);
	return info;
}

bool C_BasePlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool C_BasePlayer::HasC4()
{
	static auto fnHasC4
		= reinterpret_cast<bool(__thiscall*)(void*)>(
			Utilities::PatternScan(GetModuleHandleW(L"client.dll"), "56 8B F1 85 F6 74 31")
			);

	return fnHasC4(this);
}

Vector C_BasePlayer::GetBonePosition(int iBone)
{
	matrix3x4_t boneMatrixes[128];
	if (this->SetupBones(boneMatrixes, 128, 0x100, 0))
	{
		matrix3x4_t boneMatrix = boneMatrixes[iBone];
		return Vector(boneMatrix.m_flMatVal[0][3], boneMatrix.m_flMatVal[1][3], boneMatrix.m_flMatVal[2][3]);
	}
	else
		return Vector(0, 0, 0);
}


Vector C_BasePlayer::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f))
	{
		auto studio_model = g_ModelInfo->GetStudioModel(GetModel());
		if (studio_model)
		{
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox)
			{
				auto
					min = Vector{},
					max = Vector{};

				Math::VectorTransform(hitbox->bbmin, boneMatrix[hitbox->bone], min);
				Math::VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

bool C_BasePlayer::GetHitboxPos(int hitbox, Vector &output)
{
	if (hitbox >= MAXHITBOXES)
		return false;

	const model_t *model = this->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = g_ModelInfo->GetStudioModel(model);
	if (!studioHdr)
		return false;

	matrix3x4_t matrix[MAXSTUDIOBONES];
	if (!this->SetupBones(matrix, MAXSTUDIOBONES, 0x100, 0))
		return false;

	mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	Math::VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	Math::VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}
int C_BasePlayer::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x297C);
}

AnimationLayer *C_BasePlayer::GetAnimOverlays()
{
	// to find offset: use 9/12/17 dll
	// sig: 55 8B EC 51 53 8B 5D 08 33 C0
	return *(AnimationLayer**)((DWORD)this + 10608);
}

AnimationLayer *C_BasePlayer::GetAnimOverlay(int i)
{
	if (i < 15)
		return &GetAnimOverlays()[i];
}

int C_BasePlayer::GetSequenceActivity(int sequence)
{
	auto hdr = g_ModelInfo->GetStudioModel(this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utilities::PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 83 7D 08 FF 56 8B F1 74 3D"));

	return get_sequence_activity(this, hdr, sequence);
}
Vector C_BasePlayer::GetBonePos(int bone)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.0f))
	{
		return boneMatrix[bone].at(3);
	}
	return Vector{};
}

bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, int hitbox)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto endpos = player->GetHitboxPos(hitbox);

	ray.Init(GetEyePos(), endpos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}


bool C_BasePlayer::CanSeePlayer(C_BasePlayer* player, const Vector& pos)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = this;

	auto start = GetEyePos();
	auto dir = (pos - start).Normalized();

	ray.Init(start, pos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.hit_entity == player || tr.fraction > 0.97f;
}

Vector C_BasePlayer::GetPredicted(Vector P0)
{
	return Utilities::ExtrapolateTick(P0, this->m_vecVelocity());
}
UINT *C_BasePlayer::getWeapons()
{
	return (UINT*)((DWORD)this + 0x2DE8);
}
C_BaseCombatWeapon *C_BasePlayer::getWeapon()
{
	DWORD weap = *(DWORD*)((DWORD)this + 0x2EE8);
	auto plyrWeap = g_EntityList->GetClientEntityFromHandle(weap);

	if (plyrWeap)
		return reinterpret_cast<C_BaseCombatWeapon*>(plyrWeap);
	else
		return NULL;
}

C_BaseCombatWeapon* C_BasePlayer::GetWeapon()
{
	DWORD weaponData = *(DWORD*)((DWORD)this + 0x2EE8);
	return (C_BaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(weaponData);
}

int& C_BaseCombatWeapon::GetWeaponID()
{
	return *(int*)((DWORD)this + 0x1D8);
}

bool C_BaseCombatWeapon::IsEmpty()
{
	int clip = *(int*)((DWORD)this + 0x3204);
	return clip == 0;
}

bool C_BaseCombatWeapon::IsGun()
{
	if (!this)
		return false;

	int id = this->GetWeaponID();

	switch (id)
	{
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
		return true;
	case WEAPON_TASER:
		return false;
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
		return true;
	case WEAPON_KNIFE:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
	case WEAPON_KNIFE_T:
		return false;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}
float& C_BaseCombatWeapon::GetAccuracyPenalty()
{
	return *(float*)((DWORD)this + 0x32B0);
}
