#pragma once

#include <cstdint>
#include "../../ImGUI/imgui.h"
class Color
{
public:
	Color();
	Color(int _rgb);
	Color(int _r, int _g, int _b);
	Color(int _r, int _g, int _b, int _a);
	Color(float _r, float _g, float _b) : Color(_r, _g, _b, 1.0f) {}
	Color(float _r, float _g, float _b, float _a)
		: Color(
			static_cast<int>(_r * 255.0f),
			static_cast<int>(_g * 255.0f),
			static_cast<int>(_b * 255.0f),
			static_cast<int>(_a * 255.0f))
	{}
	explicit Color(float* rgb) : Color(rgb[0], rgb[1], rgb[2], 1.0f) {}
	explicit Color(unsigned long argb)
	{
		_CColor[2] = (unsigned char)((argb & 0x000000FF) >> (0 * 8));
		_CColor[1] = (unsigned char)((argb & 0x0000FF00) >> (1 * 8));
		_CColor[0] = (unsigned char)((argb & 0x00FF0000) >> (2 * 8));
		_CColor[3] = (unsigned char)((argb & 0xFF000000) >> (3 * 8));
	}

	static Color FromHSB(float hue, float saturation, float brightness);

	void    SetRawColor(int color32);
	int     GetRawColor() const;
	void    SetColor(int _r, int _g, int _b, int _a = 0);
	void    SetColor(float _r, float _g, float _b, float _a = 0);
	void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

	int r() const { return _CColor[0]; }
	int g() const { return _CColor[1]; }
	int b() const { return _CColor[2]; }
	int a() const { return _CColor[3]; }

	unsigned char &operator[](int index)
	{
		return _CColor[index];
	}
	const unsigned char &operator[](int index) const
	{
		return _CColor[index];
	}
	ImU32 GetU32()
	{
		return ((_CColor[3] & 0xff) << 24) + ((_CColor[2] & 0xff) << 16) + ((_CColor[1] & 0xff) << 8)
			+ (_CColor[0] & 0xff);
		//return (ImU32)(((_color[3] & 0xff) << 24) + ((_color[0] & 0xff) << 16) + ((_color[1] & 0xff) << 8) + (_color[2] & 0xff));
	}
	bool operator==(const Color &rhs) const;
	bool operator!=(const Color &rhs) const;
	Color &operator=(const Color &rhs);
	Color operator-(const int &rhs);

	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Error;

private:
	unsigned char _CColor[4];
};