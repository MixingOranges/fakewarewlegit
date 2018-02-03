#include "Hooks.h"
#include "../Menu.h"

void __stdcall Hooks::hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	static auto ofunc = VGUIPanelHook.get_original<PaintTraverse>(Index::PaintTraverse);
	const auto panelName = g_VGuiPanel->GetName(panel);

	ofunc(g_VGuiPanel, panel, forceRepaint, allowForce);

	if (g_EngineClient->IsTakingScreenshot()) return;

	if (!panelId)
	{
		if (!strcmp(panelName, "FocusOverlayPanel"))
		{
			panelId = panel;
		}
	}
	else if (panelId == panel)
	{
		g_ESP->Loop();

		if (g_EngineClient->IsInGame() && !g_EngineClient->IsTakingScreenshot()) {

			if (!g_LocalPlayer)
				return;

			if (g_Options.Visuals.Enable) {
				for (auto i = 1; i <= g_EntityList->GetHighestEntityIndex(); ++i) {
					auto entity = C_BasePlayer::GetPlayerByIndex(i);

					if (!entity)
						continue;

					if (entity == g_LocalPlayer)
						continue;

					if (i < 65 && !entity->IsDormant() && entity->IsAlive()) {
						// Begin will calculate player screen coordinate, bounding box, etc
						// If it returns false it means the player is not inside the screen
						// or is an ally (and team check is enabled)
						if (g_ESP->Begin(entity)) {
							g_ESP->Armorbar(entity);
						}
					}
				}
			}
		}
	}
}