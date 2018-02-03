#include "Visuals.h"
#include "../Core/Math.h"
#include "../Core/Draw.h"
#include "../Core/Font.h"
#include "../Core/options.h"
#include "../Core/DrawManager.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include "../SDK/csgostructs.h"

CESP* g_ESP = new CESP;

char* GetWeaponName(C_BaseCombatWeapon *pWeapon)
{
	int ID = pWeapon->WeaponID();

	switch (ID) {
	case 4:
		return "Glock-18";
	case 2:
		return "dual berettas";
	case 36:
		return "P250";
	case 30:
		return "Tec9";
	case 1:
		return "Desert Eagle";
	case 32:
		return "P2000";
	case 3:
		return "FiveSeven";
	case 64:
		return "R8 Revolver";
	case 63:
		return "CZ75 Auto";
	case 61:
		return "USP-S";
	case 35:
		return "Nova";
	case 25:
		return "XM1014";
	case 29:
		return "Sawed Off";
	case 14:
		return "M249";
	case 28:
		return "Negev";
	case 27:
		return "Mag7";
	case 17:
		return "Mac10";
	case 33:
		return "MP7";
	case 24:
		return "UMP-45";
	case 19:
		return "P90";
	case 26:
		return "PP-Bizon";
	case 34:
		return "MP9";
	case 10:
		return "Famas";
	case 16:
		return "M4A4";
	case 40:
		return "SSG08";
	case 8:
		return "Aug";
	case 9:
		return "AWP";
	case 38:
		return "SCAR20";
	case 13:
		return "Galil AR";
	case 7:
		return "AK47";
	case 39:
		return "SG553";
	case 11:
		return "G3SG1";
	case 60:
		return "M4A1-S";
	case 46: 
		return "Molotov";
	case 48:
		return "Incendiary Grenade";
	case 44:
		return "High Explosive Grenade";
	case 43:
		return "Flashbang";
	case 45:
		return "Smoke Grenade";
	case 47:
		return "Decoy Grenade";
	case 31:
		return "ZEUSx27";
	default:
		return "Knife";
	}
}

struct
{
	C_BasePlayer* pl;
	bool          is_enemy;
	bool          is_visible;
	Color         clr;
	Vector        head_pos;
	Vector        feet_pos;
	RECT          bbox;
} esp_ctx;

RECT GetBBox(C_BaseEntity* ent)
{
	RECT rect{};
	auto collideable = ent->GetCollideable();

	if (!collideable)
		return rect;

	auto min = collideable->OBBMins();
	auto max = collideable->OBBMaxs();

	const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		Math::VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	Vector screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!Math::WorldToScreen(pointsTransformed[i], screen_points[i]))
			return rect;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
bool CESP::Begin(C_BasePlayer* pl)
{
	esp_ctx.pl = pl;
	esp_ctx.is_enemy = g_LocalPlayer->m_iTeamNum() != pl->m_iTeamNum();
	esp_ctx.is_visible = g_LocalPlayer->CanSeePlayer(pl, HITBOX_CHEST);

	if (!esp_ctx.is_enemy && g_Options.Visuals.EnemiesOnly)
		return false;

	auto head = pl->GetHitboxPos(HITBOX_HEAD);
	auto origin = pl->m_vecOrigin();

	head.z += 15;

	if (!Math::WorldToScreen(head, esp_ctx.head_pos) ||
		!Math::WorldToScreen(origin, esp_ctx.feet_pos))
		return false;

	auto h = fabs(esp_ctx.head_pos.y - esp_ctx.feet_pos.y);
	auto w = h / 1.65f;

	esp_ctx.bbox.left = static_cast<long>(esp_ctx.feet_pos.x - w * 0.5f);
	esp_ctx.bbox.right = static_cast<long>(esp_ctx.bbox.left + w);
	esp_ctx.bbox.bottom = static_cast<long>(esp_ctx.feet_pos.y);
	esp_ctx.bbox.top = static_cast<long>(esp_ctx.head_pos.y);

	return true;
}

bool GetCCBaseEntityBox(C_BasePlayer* pBaseEntity, Vector& BotCenter, Vector& TopCenter, float& Left, float& Right, bool is_player)
{
	if (!pBaseEntity)
		return false;

	if (!is_player)
	{
		const VMatrix& trans = pBaseEntity->GetCollisionBoundTrans();

		CollisionProperty* collision = pBaseEntity->GetCollision();

		if (!collision)
			return false;

		Vector min, max;

		//pBaseEntity->GetRenderable()->GetRenderBounds(min, max);
		min = collision->VecMins();
		max = collision->VecMaxs();

		Vector points[] = { Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z) };

		auto vector_transform = [](const Vector in1, const VMatrix& in2)
		{
			auto dot_product = [](const Vector &v1, const float *v2)
			{
				float ret = v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				return ret;
			};

			auto out = Vector();
			out[0] = dot_product(in1, in2[0]) + in2[0][3];
			out[1] = dot_product(in1, in2[1]) + in2[1][3];
			out[2] = dot_product(in1, in2[2]) + in2[2][3];
			return out;
		};

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++)
		{
			pointsTransformed[i] = vector_transform(points[i], trans);
		}

		Vector pos = pBaseEntity->GetAbsOrigin();
		Vector flb;
		Vector brt;
		Vector blb;
		Vector frt;
		Vector frb;
		Vector brb;
		Vector blt;
		Vector flt;


		if (!Math::WorldToScreen(pointsTransformed[3], flb) || !Math::WorldToScreen(pointsTransformed[5], brt)
			|| !Math::WorldToScreen(pointsTransformed[0], blb) || !Math::WorldToScreen(pointsTransformed[4], frt)
			|| !Math::WorldToScreen(pointsTransformed[2], frb) || !Math::WorldToScreen(pointsTransformed[1], brb)
			|| !Math::WorldToScreen(pointsTransformed[6], blt) || !Math::WorldToScreen(pointsTransformed[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		//+1 for each cuz of borders at the original box
		float left = flb.x;        // left
		float top = flb.y;        // top
		float right = flb.x;    // right
		float bottom = flb.y;    // bottom

		for (int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (top < arr[i].y)
				top = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (bottom > arr[i].y)
				bottom = arr[i].y;
		}
		BotCenter = Vector(right - ((right - left) / 2), top, 0);
		TopCenter = Vector(right - ((right - left) / 2), bottom, 0);
		Left = left;
		Right = right;
	}
	else
	{
		Vector org = pBaseEntity->GetAbsOrigin();
		Vector head;
		if ((pBaseEntity->m_fFlags() & FL_DUCKING))
			head = org + Vector(0.f, 0.f, 52.f);
		else
			head = org + Vector(0.f, 0.f, 72.f);

		Vector org_screen, head_screen;
		if (!Math::WorldToScreen(org, org_screen) || !Math::WorldToScreen(head, head_screen))
			return false;

		int height = int(org_screen.y - head_screen.y);
		int width = int(height / 2);
		Left = int(head_screen.x - width / 2);
		Right = int(head_screen.x + width / 2);
		BotCenter = Vector(head_screen.x, org_screen.y);
		TopCenter = BotCenter; TopCenter.y = head_screen.y;
	}
	return true;
}


RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_EngineClient->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void DrawTexturedPoly(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = g_VGuiSurface->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_VGuiSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	g_VGuiSurface->DrawSetColor(col);
	g_VGuiSurface->DrawSetTexture(texture_id);
	g_VGuiSurface->DrawTexturedPolygon(n, vertice);
}

void DrawFilledCircle(Vector2D center, Color color, float radius, float points)
{
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

	DrawTexturedPoly((int)points, vertices.data(), color);
}

void CESP::DrawPlayer(C_BasePlayer* pPlayer, C_BasePlayer* pLocalPlayer)
{
	Vector m_position = pPlayer->GetAbsOrigin();
	Vector m_top_position = m_position;

	if (pPlayer->m_fFlags() & IN_DUCK)
		m_top_position += Vector(0, 0, 52);
	else
		m_top_position += Vector(0, 0, 72);

	Vector footpos, headpos;

	if (pPlayer->IsDormant() && flPlayerAlpha[pPlayer->EntIndex()] > 0.f)
		flPlayerAlpha[pPlayer->EntIndex()] -= 0.3f;
	else if (flPlayerAlpha[pPlayer->EntIndex()] < 255.f && !pPlayer->IsDormant())
		flPlayerAlpha[pPlayer->EntIndex()] = 255.f;



	if (Math::WorldToScreen(m_position, footpos) && Math::WorldToScreen(m_top_position, headpos))
	{
		Vector Bot, Top;
		float Left, Right;
		GetCCBaseEntityBox(pPlayer, Bot, Top, Left, Right, true);
		int height = Bot.y - Top.y;
		int width = Right - Left;
		int x = Left;
		int y = Top.y;

		C_BaseCombatWeapon* pMainWeapon = pPlayer->GetWeapon();

		if (g_Options.Visuals.BoundingBox)
		{
			float Red = 255;
			float Green = 255;
			float Blue = 255;

			if (pPlayer->IsDormant())
			{
				g_Draw.Box(x - 1, y - 1, width + 2, height + 2, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
				g_Draw.Box(x, y, width, height, 140, 140, 140, flPlayerAlpha[pPlayer->EntIndex()]); 
				g_Draw.Box(x + 1, y + 1, width - 2, height - 2, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
			}
			else
			{
				g_Draw.Box(x - 1, y - 1, width + 2, height + 2, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
				g_Draw.Box(x, y, width, height, Red, Green, Blue, flPlayerAlpha[pPlayer->EntIndex()]); 
				g_Draw.Box(x + 1, y + 1, width - 2, height - 2, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
			}
		}

		if (g_Options.Visuals.Flags)
		{
			if (pPlayer->m_bIsScoped())
				g_Draw.StringA(Font::Get().ESPMini, false, x + width + 3, y + 20, 96, 146, 144, flPlayerAlpha[pPlayer->EntIndex()], "ZOOM");
		}

		if (g_Options.Visuals.Healthbar)
		{
			int health = 100 - pPlayer->m_iHealth();
			int w = 4;
			if (width < 4)
				w = width;

			int hr, hg, hb;

			hr = 255 - (pPlayer->m_iHealth()*2.55);
			hg = pPlayer->m_iHealth() * 2.55;
			hb = 0;

			if (pPlayer->IsDormant())
			{
				g_Draw.FillRGBA(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()] - 150);
				g_Draw.FillRGBA(x - (5), y + health * height / 100, w, pPlayer->m_iHealth()*height / 100, 140, 140, 140, flPlayerAlpha[pPlayer->EntIndex()]);
				g_Draw.Box(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
			}
			else
			{
				g_Draw.FillRGBA(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()] - 150);
				g_Draw.FillRGBA(x - (5), y + health * height / 100, w, pPlayer->m_iHealth()*height / 100, hr, hg, hb, flPlayerAlpha[pPlayer->EntIndex()]);
				g_Draw.Box(x - (5), y, w, height, 0, 0, 0, flPlayerAlpha[pPlayer->EntIndex()]);
			}
			if (pPlayer->m_iHealth() < 100)
			{
				char hp[20];
				sprintf_s(hp, sizeof(hp), "%i", pPlayer->m_iHealth());

				g_Draw.StringA(Font::Get().ESPMini, false, x - g_Draw.getWidht(hp, Font::Get().font_espnum), y + health * height / 100 - 6, 255, 255, 255, flPlayerAlpha[pPlayer->EntIndex()], "%i", pPlayer->m_iHealth());
			}
		}


		int bot_add = 0;
		int top_add = 0;

		if (g_Options.Visuals.Name)
		{
			player_info_s info;
			g_EngineClient->GetPlayerInfo(pPlayer->EntIndex(), &info);
			g_Draw.StringA(Font::Get().ESP, true, x + width / 2, y - 12 - top_add, 255, 255, 255, flPlayerAlpha[pPlayer->EntIndex()], "%s", info.szName);
			top_add += 12;
		}

		if (g_Options.Visuals.Weapon)
		{
			UINT* hWeapons = pPlayer->GetWeapons();
			if (hWeapons)
			{
				if (pMainWeapon)
				{
					std::string s1 = GetWeaponName(pMainWeapon);
					g_Draw.StringA(Font::Get().ESPMini, true, x + width / 2, y + height + bot_add, 255, 255, 255, flPlayerAlpha[pPlayer->EntIndex()], XorStr("%s"), s1.c_str());
				}
			}
		}
	}
}
void CESP::Armorbar(C_BasePlayer* pPlayer)
{
	//if (g_Options.Visuals.Healthbar)
	//{
	//	auto animLayer = pPlayer->GetAnimOverlay(1);
	//	if (!animLayer->m_pOwner)
	//		return;

	//	auto activity = pPlayer->GetSequenceActivity(animLayer->m_nSequence);

	//	int iClip = pPlayer->m_hActiveWeapon().Get()->m_iClip1();
	//	int iClipMax = pPlayer->m_hActiveWeapon().Get()->GetCSWeaponData()->iMaxClip1;

	//	float box_w = (float)fabs(esp_ctx.bbox.right - esp_ctx.bbox.left);
	//	float width;
	//	if (activity == 967 && animLayer->m_flWeight != 0.f)
	//	{
	//		float cycle = animLayer->m_flCycle; // 1 = finished 0 = just started
	//		width = (((box_w * cycle) / 1.f));
	//	}
	//	else
	//		width = (((box_w * iClip) / iClipMax));

	//	g_Draw.DrawRect(esp_ctx.bbox.left - 1, esp_ctx.bbox.bottom + 1, esp_ctx.bbox.right + 1, esp_ctx.bbox.bottom + 5, Color(5, 5, 5, 170));
	//	g_Draw.DrawRect(esp_ctx.bbox.left, esp_ctx.bbox.bottom + 2, esp_ctx.bbox.left + width, esp_ctx.bbox.bottom + 4, Color(61, 135, 255));
	//}
}
void CESP::DrawC4(C_BasePlayer* pBomb, bool is_planted, C_BasePlayer* pLocalPlayer)
{

}

void CESP::DrawThrowable(C_BasePlayer* pThrowable)
{
	const model_t* nadeModel = pThrowable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_ModelInfo->GetStudioModel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->szName, XorStr("thrown")) && !strstr(hdr->szName, XorStr("dropped")))
		return;

	std::string nadeName = XorStr("Unknown Grenade");

	IMaterial* mats[32];
	g_ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), XorStr("flashbang")))
		{
			nadeName = XorStr("Flashbang");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("m67_grenade")) || strstr(mat->GetName(), XorStr("hegrenade")))
		{
			nadeName = XorStr("HE Grenade");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("smoke")))
		{
			nadeName = XorStr("Smoke");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("decoy")))
		{
			nadeName = XorStr("Decoy");

			break;
		}
		else if (strstr(mat->GetName(), XorStr("incendiary")) || strstr(mat->GetName(), XorStr("molotov")))
		{
			nadeName = XorStr("Molotov");

			break;
		}
	}
	Vector Bot, Top;
	float Left, Right;
	GetCCBaseEntityBox(pThrowable, Bot, Top, Left, Right, false);
	int height = Bot.y - Top.y;
	int width = Right - Left;

	//g_pRender->Text((char*)nadeName.c_str(), Left + width / 2, Bot.y, centered, g_pRender->Fonts.esp, true, WHITE(255), BLACK(255));
	g_Draw.StringA(Font::Get().ESPMini, true, Left + width / 2, Bot.y, 255, 255, 255, 255, nadeName.c_str());
}

void CESP::DrawDroppedWeapon(C_BaseCombatWeapon* pWeapon)
{
	C_BasePlayer* pCBaseEntity = (C_BasePlayer*)pWeapon;
	C_BaseCombatWeapon* Weapon = (C_BaseCombatWeapon*)pWeapon;

	if (!pCBaseEntity || !g_ModelInfo)
		return;

	std::string sCurWeapon = g_ModelInfo->GetModelName(pCBaseEntity->GetModel());
	if (!sCurWeapon.empty() && sCurWeapon.find(XorStr("w_")) != std::string::npos)
	{
		std::string name;

		if (sCurWeapon.find(XorStr("defuser")) != std::string::npos/* insert check for defuser here*/)
			name = XorStr("defuser");
		else
		{
			C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pCBaseEntity;
			if (!pWeapon)return;
			name = GetWeaponName(pWeapon);//pWeapon->GetName(false);
		}

		Vector Bot, Top;
		float Left, Right;
		GetCCBaseEntityBox(pCBaseEntity, Bot, Top, Left, Right, false);
		int width = Right - Left;

		float Red = 255;
		float Green = 255;
		float Blue = 255;

		g_Draw.StringA(Font::Get().ESP, true, Left + width / 2, Bot.y, Red, Green, Blue, 255, name.c_str());
	}
}

void CESP::Loop()
{
	if (!g_Globals::ptrLocalPlayer)
		return;

	for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++)
	{
		C_BasePlayer* pBaseEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);

		if (!pBaseEntity)
			continue;

		ClientClass* pClass = (ClientClass*)pBaseEntity->GetClientClass();

		if (pClass->m_ClassInfo == int(CCSPlayer) && g_Options.Visuals.Enable)
		{
			if (pBaseEntity->m_iHealth() < 1)
				continue;

			bool is_friendly = pBaseEntity->m_iTeamNum() == g_Globals::ptrLocalPlayer->m_iTeamNum();

			if (!is_friendly || is_friendly && !g_Options.Visuals.EnemiesOnly)
				g_ESP->DrawPlayer(pBaseEntity, g_Globals::ptrLocalPlayer);
		}

		Vector buf, pos = pBaseEntity->GetAbsOrigin();

		if (pos.x == 0 || pos.y == 0 || pos.z == 0 || !Math::WorldToScreen(pos, buf))
			continue;

		if ((pClass->m_ClassInfo != int(CBaseWeaponWorldModel) && (strstr(pClass->m_pNetworkName, XorStr("Weapon")) || pClass->m_ClassInfo == int(CDEagle) || pClass->m_ClassInfo == int(CAK47))) && g_Options.Visuals.Weapon == 1)
			DrawDroppedWeapon((C_BaseCombatWeapon*)pBaseEntity);

		if (strstr(pClass->m_pNetworkName, XorStr("Projectile")) && g_Options.Visuals.ThrownNades)
			DrawThrowable(pBaseEntity);
	}
}
