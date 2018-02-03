#pragma once
#include "Cheat\Skins.h"
#include "../cheat/Cheat/Cheats.h"
#include "Core\options.h"
#include <vector>
#include <xstring>
extern ImFont* Default;
extern ImFont* Tabs;
extern ImFont* WeaponTabs;
static std::vector<std::string> skins;
static char skinsStr[1024];
extern const char* keyNames[124];
extern const char* aimBones[7];
extern const char *opt_Chams[4];
class CMenu
{
public:
	void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
	{
		ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

		Color colColor(255, 255, 255, 255);

		flRainbow += flSpeed;
		if (flRainbow > 1.f) flRainbow = 0.f;

		for (int i = 0; i < width; i++)
		{
			float hue = (2.5f / (float)width) * i;
			hue -= flRainbow;
			if (hue < 0.f) hue += 1.f;

			Color colRainbow = colColor.FromHSB(1.f, 0.f, 1.f);
			windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
		}
	}

	void ThinkKeyboard()
	{
		static bool PressedButtons = false;

		if (!PressedButtons && GetAsyncKeyState(VK_INSERT))
		{
			PressedButtons = true;
		}
		else if (PressedButtons && !GetAsyncKeyState(VK_INSERT))
		{
			PressedButtons = false;
			m_opened = !m_opened;
		}
	}

	void color()
	{
		int MenuRed, MenuGreen, MenuBlue;

		MenuRed = 255;
		MenuGreen = 255;
		MenuBlue = 255;

		ImColor mainColor = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
		ImColor bodyColor = ImColor(34, 32, 34, 255);
		ImColor fontColor = ImColor(255, 255, 255, 255);

		ImGuiStyle& styled = ImGui::GetStyle();

		ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
		ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
		ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
		ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
		ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

		styled.Colors[ImGuiCol_Text] = fontColor;
		styled.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		styled.Colors[ImGuiCol_WindowBg] = bodyColor;
		styled.Colors[ImGuiCol_ChildWindowBg] = ImColor(20, 20, 20, 255);
		styled.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
		styled.Colors[ImGuiCol_Border] = ImColor(1, 1, 1, 255);
		styled.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		styled.Colors[ImGuiCol_FrameBg] = frameBgColor;
		styled.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
		styled.Colors[ImGuiCol_TitleBg] = mainColor;
		styled.Colors[ImGuiCol_TitleBgCollapsed] = mainColor;
		styled.Colors[ImGuiCol_TitleBgActive] = mainColor;
		styled.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		styled.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .1f, frameBgColor.y + .1f, frameBgColor.z + .1f, frameBgColor.w);
		styled.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
		styled.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
		styled.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
		styled.Colors[ImGuiCol_ComboBg] = frameBgColor;
		styled.Colors[ImGuiCol_CheckMark] = mainColor;
		styled.Colors[ImGuiCol_SliderGrab] = mainColor;
		styled.Colors[ImGuiCol_SliderGrabActive] = mainColorHovered;
		styled.Colors[ImGuiCol_Button] = mainColor;
		styled.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_ButtonActive] = mainColorActive;
		styled.Colors[ImGuiCol_Header] = mainColor;
		styled.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_HeaderActive] = mainColorActive;

		styled.Colors[ImGuiCol_Column] = ImVec4(.30f, .30f, .30f, 1.f);
		styled.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_ColumnActive] = mainColorActive;

		styled.Colors[ImGuiCol_ResizeGrip] = mainColor;
		styled.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
		styled.Colors[ImGuiCol_CloseButton] = ImVec4(0.f, 0.f, 0.f, 0.f);
		styled.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
		styled.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
		styled.Colors[ImGuiCol_PlotLines] = mainColor;
		styled.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_PlotHistogram] = mainColor;
		styled.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
		styled.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		styled.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	void TextColor(bool active)
	{
		auto& style = ImGui::GetStyle();

		if (active)
			style.Colors[ImGuiCol_Text] = ImColor(205, 205, 205, 255);
		else
			style.Colors[ImGuiCol_Text] = ImColor(93, 93, 93, 255);
	}

	void TextColor2(bool active)
	{
		auto& style = ImGui::GetStyle();

		if (active)
			style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
		else
			style.Colors[ImGuiCol_Text] = ImColor(93, 93, 93, 255);
	}

	void VectorToArray(std::vector<std::string> vec, char *arr)
	{
		int k = 0;
		for (int i = 0; i < (int)vec.size(); i++)
		{
			int len = (int)strlen(vec[i].c_str());

			for (int j = 0; j < len; j++)
				arr[k++] = vec[i].c_str()[j];

			arr[k++] = '\0';
		}
	}

	void buttonColor(bool bActive)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		if (bActive)
		{
			style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
			style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
			style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
		}
		else
		{
			style.Colors[ImGuiCol_Button] = ImColor(26, 25, 27);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(26, 25, 27);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(26, 25, 27);
		}
	}

	void lineColor(bool bActive)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		if (bActive)
		{
			style.Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(60, 60, 60);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 60, 60);
		}
		else
		{
			style.Colors[ImGuiCol_Button] = ImColor(26, 25, 27);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(26, 25, 27);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(26, 25, 27);
		}
	}

	void lineColor1(bool bActive)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		if (bActive)
		{
			style.Colors[ImGuiCol_Button] = ImColor(60, 60, 60);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(60, 60, 60);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(60, 60, 60);
		}
		else
		{
			style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
			style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
			style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
		}
	}

	void Border(bool active)
	{
		auto& style = ImGui::GetStyle();

		if (active)
			style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
		else
			style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	}

	void InitializeTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = .0f;
		style.WindowPadding = ImVec2(14, 8);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 0.5f; //4.0
		style.ChildWindowRounding = 0.0f;
		style.FramePadding = ImVec2(4, 1);
		style.FrameRounding = 0.0f; //2.0
		style.ItemSpacing = ImVec2(0, 0);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 3.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 16.0f; //16.0
		style.GrabMinSize = 0.1f;
		style.GrabRounding = 16.0f; //16.0
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;
	}

	void Think()
	{
		auto& style = ImGui::GetStyle();
		static bool last = m_opened;

		if (m_opened != last)
		{
			last = m_opened;

			std::string msg = ("cl_mouseenable ") + std::to_string(!m_opened);
			g_EngineClient->ExecuteClientCmd(msg.c_str());
		}

		ImGui::GetIO().MouseDrawCursor = m_opened;

		static int tab;
		if (m_opened)
		{
			ImGui::PushFont(Default);
			ImVec2 mainWindowPos;
			color();
			ImGui::SetNextWindowSize(ImVec2(649, 539));
			ImGui::Begin("mythic", &m_opened, ImVec2(550, 660), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);
			{
				mainWindowPos = ImGui::GetWindowPos();
				static float flRainbow;
				float flSpeed = 0.0000f;
				int curWidth = 1;
				ImVec2 curPos = ImGui::GetCursorPos();
				ImVec2 curWindowPos = ImGui::GetWindowPos();
				curPos.x += curWindowPos.x;
				curPos.y += curWindowPos.y;
				int size;
				int y;
				g_EngineClient->GetScreenSize(size, y);
				DrawRectRainbow(curPos.x, curPos.y * -1, ImGui::GetWindowSize().x + size, curPos.y * 1.0, 0, flRainbow);
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, 100);
				style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 0);
				style.ItemSpacing = ImVec2(0, 0);

				ImGui::PushFont(Tabs);
				TextColor(tab == 0);
				buttonColor(false);
				ImGui::Button("##upprspacing", ImVec2(74, 20));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##uppr_t", ImVec2(1, 20));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 0);
				ImGui::Button("##rage_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##rage_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 0);
				if (ImGui::Button("A", ImVec2(74, 76))) tab = 0;
				TextColor(tab == 1);
				ImGui::SameLine();
				lineColor1(tab != 0);
				ImGui::Button("rage_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 0 || tab == 1);
				ImGui::Button("##legit_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##legit_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 1);
				if (ImGui::Button("B", ImVec2(74, 76))) tab = 1;
				TextColor(tab == 2);
				ImGui::SameLine();
				lineColor1(tab != 1);
				ImGui::Button("legit_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 1 || tab == 2);
				ImGui::Button("##vis_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##vis_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 2);
				if (ImGui::Button("C", ImVec2(74, 76))) tab = 2;
				TextColor(tab == 3);
				ImGui::SameLine();
				lineColor1(tab != 2);
				ImGui::Button("vis_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 2 || tab == 3);
				ImGui::Button("##vis_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##vis_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 3);
				if (ImGui::Button("D", ImVec2(74, 76))) tab = 3;
				TextColor(tab == 4);
				ImGui::SameLine();
				lineColor1(tab != 3);
				ImGui::Button("misc_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 3 || tab == 4);
				ImGui::Button("##skin_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##skin_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 4);
				if (ImGui::Button("E", ImVec2(74, 76))) tab = 4;
				TextColor(tab == 5);
				ImGui::SameLine();
				lineColor1(tab != 4);
				ImGui::Button("skin_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////

				lineColor(tab == 4 || tab == 5);
				ImGui::Button("##clr_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##clr_d", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(tab == 5);
				if (ImGui::Button("F", ImVec2(74, 76))) tab = 5;
				ImGui::SameLine();
				lineColor1(tab != 5);
				ImGui::Button("clr_a", ImVec2(1, 76));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				lineColor(tab == 5);
				ImGui::Button("down_t", ImVec2(74, 1));
				ImGui::SameLine();
				lineColor(1);
				ImGui::Button("down_b", ImVec2(1, 1));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				buttonColor(false);
				ImGui::Button("##downspacing", ImVec2(74, 49));
				ImGui::SameLine();
				lineColor(true);
				ImGui::Button("##down_t", ImVec2(1, 49));
				style.ButtonTextAlign = ImVec2(0.4f, 0.5f);
				//////////////////////////////////////////////////////
				ImGui::PopFont();
				ImGui::PushFont(Default);
				/*//////////////////////////*/

				ImGui::NextColumn();
				TextColor(true);
				Border(true);
				color();
				style.ItemSpacing = ImVec2(8, 6);
				style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 0);
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 0);
				style.WindowPadding = ImVec2(8, 8);
				ImGui::BeginChild("tabs", ImVec2(534, 530), true); //effektiv sind das (515, 530) der Rest ist auﬂerhalb
				{
					style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
					switch (tab) 
					{
					case 0:
						break;
					case 1:
						//init
						ImGui::NewLine();
						static int LegitSubTab;
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						style.ItemSpacing = ImVec2(0, 0);

						ImGui::PushFont(WeaponTabs);
						//subtabs
						TextColor(LegitSubTab == 0);
						buttonColor(false);
						if (ImGui::Button("G", ImVec2(85.83, 50))) LegitSubTab = 0;
						ImGui::SameLine();
						TextColor(LegitSubTab == 1);
						buttonColor(false);
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						if (ImGui::Button("P", ImVec2(85.83, 50))) LegitSubTab = 1;
						ImGui::SameLine();
						TextColor(LegitSubTab == 2);
						buttonColor(false);
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						if (ImGui::Button("W", ImVec2(85.83, 50))) LegitSubTab = 2;
						ImGui::SameLine();
						TextColor(LegitSubTab == 3);
						buttonColor(false);
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						if (ImGui::Button("d", ImVec2(85.83, 50))) LegitSubTab = 3;
						ImGui::SameLine();
						TextColor(LegitSubTab == 4);
						buttonColor(false);
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						if (ImGui::Button("f", ImVec2(85.83, 50))) LegitSubTab = 4;
						ImGui::SameLine();
						TextColor(LegitSubTab == 5);
						buttonColor(false);
						style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
						if (ImGui::Button("a", ImVec2(85.83, 50))) LegitSubTab = 5;
						ImGui::PopFont();
						ImGui::NewLine();
						//checkboxes
						if (LegitSubTab == 0) {

							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enablepistol);
								ImGui::Combo("Key##1", &g_Options.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##1"), &menu.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##1", &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##1", &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						else if (LegitSubTab == 1) {
							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes2", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enablesmg);
								ImGui::Combo("Key##152", &g_Options.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.SMGSmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.SMGfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.SMG_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.SMG_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						else if (LegitSubTab == 2) {
							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes3", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enablemain);
								ImGui::Combo("Key##15", &g_Options.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						else if (LegitSubTab == 3) {
							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes4", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enableshotgun);
								ImGui::Combo("Key##14", &g_Options.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.ShotgunSmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Shotgunfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.Shotgun_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.Shotgun_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						else if (LegitSubTab == 4) {
							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes5", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enableheavy);
								ImGui::Combo("Key##12", &g_Options.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##1"), &menu.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.HeavySmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Heavyfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##1", &g_Options.Legitbot.Heavy_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##1", &g_Options.Legitbot.Heavy_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);

							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						else if (LegitSubTab == 5) {
							TextColor2(true);
							style.ItemSpacing = ImVec2(7, 7);
							ImGui::PushFont(Default);
							ImGui::Columns(2, "yes6", false);
							ImGui::BeginChild("##aimbot", ImVec2(246, 425), true);
							{
								ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.enablesniper);
								ImGui::Combo("Key##13", &g_Options.Legitbot.SniperKey, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Key##2"), &menu.Legitbot.SniperKey, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat("Smooth##2", &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("FOV##2", &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
								ImGui::SliderFloat("Min RCS##2", &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
								ImGui::SliderFloat("Max RCS##2", &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");
							}ImGui::EndChild();

							ImGui::NextColumn();
							ImGui::BeginChild("##trigger", ImVec2(246, 300), true);
							{
								//ImGui::Separator();
								ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
								ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
								ImGui::Combo("Key##3", &g_Options.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								//ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
								ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");
								ImGui::Separator();
								ImGui::Text(("Filter"));
								ImGui::Separator();
								ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
								ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
								ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
								ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
								ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
							}ImGui::EndChild();

							ImGui::BeginChild("##miscaimbot", ImVec2(246, 120), true);
							{
								ImGui::Checkbox("Backtrack", &g_Options.MISC_Backtrack);
							}ImGui::EndChild();

							ImGui::PopFont();
						}
						break;
					case 2:
							ImGui::Checkbox("Enable Visuals", &g_Options.Visuals.Enable);
							ImGui::Checkbox("Enemies Only", &g_Options.Visuals.EnemiesOnly);
							ImGui::Checkbox("Bounding Box", &g_Options.Visuals.BoundingBox);
							ImGui::Checkbox("Name", &g_Options.Visuals.Name);
							ImGui::Checkbox("Weapon", &g_Options.Visuals.Weapon);
							ImGui::Checkbox("Flags", &g_Options.Visuals.Flags);
							ImGui::Checkbox("Healthbar", &g_Options.Visuals.Healthbar);
							ImGui::Checkbox("Chams##ESP", &g_Options.Visuals.chamsenable);
							ImGui::Combo("Chams Type##ESP", &g_Options.Visuals.chamstype, opt_Chams, 4);
						break;
					case 3:
						break;
					case 4:
						ImGui::Text("Skins");
						ImGui::Separator();

						if (skins.size() == 0)
						{
							skins.push_back("None");
							skins.push_back("Karambit");
							skins.push_back("M9 Bayonet");
							skins.push_back("Butterfly");

							VectorToArray(skins, skinsStr);
						}

						if (ImGui::Combo("Knife Model", &g_Options.knifemodel, skinsStr))
							requestFullUpdate = true;
						break;
					case 5:
						break;
					}
				} ImGui::EndChild();
				ImGui::PopFont();
			} ImGui::End();
			ImGui::PopFont();
			style.WindowPadding = ImVec2(2, 4);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);

			ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x - 5, mainWindowPos.y - 5));
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
			ImGui::SetNextWindowSize(ImVec2(659, 549));
			ImGui::Begin("##border2", &m_opened, ImVec2(659, 549), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
			{
				mainWindowPos = ImGui::GetWindowPos();
			} ImGui::End();

		}
	}


	bool m_opened = false;
}; extern CMenu* g_pMenu;
