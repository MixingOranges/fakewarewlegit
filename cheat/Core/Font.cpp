#include "Font.h"

void Font::Create()
{
	Overlay = g_VGuiSurface->CreateFont_();
	MenuText = g_VGuiSurface->CreateFont_();
	ESP = g_VGuiSurface->CreateFont_();
	ESPMini = g_VGuiSurface->CreateFont_();
	font_espnum = g_VGuiSurface->CreateFont_();
	g_VGuiSurface->SetFontGlyphSet(font_espnum, "Tahoma", 9, FW_NORMAL, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	g_VGuiSurface->SetFontGlyphSet(Overlay, "Tahoma", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);
	g_VGuiSurface->SetFontGlyphSet(MenuText, "Tahoma", 12, 550, 0, 0, FONTFLAG_ANTIALIAS);
	g_VGuiSurface->SetFontGlyphSet(ESP, "Verdana Fett", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);
	g_VGuiSurface->SetFontGlyphSet(ESPMini, "Smallest Pixel-7", 10, 400, 0, 0, FONTFLAG_OUTLINE);
}
