#pragma once

#include <string>
#include "../SDK/Misc/Color.h"
#define OPTION(type, var, val) type var = val

class Config
{
public:

	struct
	{
		bool Enable;
		bool BoundingBox;
		bool Flags;
		bool Healthbar;
		bool Name;
		bool Weapon;
		bool EnemiesOnly;
		bool Bomb;
		bool ThrownNades;
			bool chamsenable;
			int chamstype;
	} Visuals;
	struct
	{
		bool MainSwitch;
		bool backtrack;
		struct
		{
			bool AutoPistol;
			bool	Resolver;

		} Aimbot;

		bool enablemain;
		int MainKey = 1;
		float MainSmooth = 1;
		float Mainfov;
		float main_random_Smooth = 1;
		float main_recoil_min;
		float main_recoil_max;
		float main_randomized_angle;


		bool enablepistol;
		int PistolKey = 6;
		float Pistolfov;
		float PistolSmooth = 1;
		float pistol_random_Smooth = 1;
		float pistol_recoil_min;
		float pistol_recoil_max;
		float pistol_randomized_angle;


		bool enablesniper;
		int SniperKey = 6;
		float Sniperfov;
		float SniperSmooth = 1;
		float sniper_random_Smooth = 1;
		float sniper_recoil_min;
		float sniper_recoil_max;
		float sniper_randomized_angle;

		bool enablesmg;
		int SMGKey = 6;
		float SMGfov;
		float SMGSmooth = 1;
		float SMG_random_Smooth = 1;
		float SMG_recoil_min;
		float SMG_recoil_max;
		float SMG_randomized_angle;

		bool enableshotgun;
		int ShotgunKey = 6;
		float Shotgunfov;
		float ShotgunSmooth = 1;
		float Shotgun_random_Smooth = 1;
		float Shotgun_recoil_min;
		float Shotgun_recoil_max;
		float Shotgun_randomized_angle;

		bool enableheavy;
		int HeavyKey = 6;
		float Heavyfov;
		float HeavySmooth = 1;
		float Heavy_random_Smooth = 1;
		float Heavy_recoil_min;
		float Heavy_recoil_max;
		float Heavy_randomized_angle;


		struct
		{
			bool	Enabled;
			float Delay;
			int Key = 6;
			float hitchance;
			struct
			{
				bool Head;
				bool Arms;
				bool Chest;
				bool Stomach;
				bool Legs;
			} Filter;

		} Triggerbot;

	} Legitbot;

#pragma region Visuals
	OPTION(bool, VIS_Chams, false);
	OPTION(bool, VIS_ESP, false);
	OPTION(bool, VIS_ESP_Box, false);
	OPTION(bool, VIS_ESP_Bone, false);
	OPTION(bool, VIS_ESP_Name, false);
	OPTION(bool, VIS_ESP_Health, false);
	OPTION(bool, VIS_ESP_Weapon, false);
	OPTION(bool, VIS_ESP_Snaplines, false);
	OPTION(bool, bEnableSkinChange, false);
	OPTION(int, knifemodel, 0);
#pragma endregion
#pragma region Misc
	OPTION(bool, MISC_Bunnyhop, false);
	OPTION(bool, MISC_AutoAccept, true);
	OPTION(bool, MISC_RankReveal, false);
	OPTION(bool, MISC_Backtrack, true);
	OPTION(float, MISC_ViewmodelFOV, 0);
	OPTION(std::wstring, MISC_ClanTag, L"");
#pragma endregion
};

extern Config g_Options;
