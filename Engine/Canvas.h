#pragma once

#include "Rect.h"
#include "Graphics.h"
#include "Palette.h"

class Canvas
{
public:
	Canvas();

	void Draw( Graphics& gfx ) const;
private:
	const RectI screenArea = { 70,Graphics
		::ScreenWidth - 4,50,Graphics
		::ScreenHeight - 4 };
	static constexpr int width = 16;
	static constexpr int height = 16;
	Palette pal;
};