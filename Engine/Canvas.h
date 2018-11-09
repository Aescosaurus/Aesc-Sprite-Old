#pragma once

#include "Rect.h"
#include "Graphics.h"
#include "Palette.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Surface.h"

class Canvas
{
public:
	Canvas();

	void Update( const Mouse& mouse,const Keyboard& kbd );
	void Draw( Graphics& gfx ) const;
private:
	const RectI screenArea = { 70,Graphics
		::ScreenWidth - 4,50,Graphics
		::ScreenHeight - 4 };
	static constexpr int width = 16;
	static constexpr int height = 16;
	Palette pal;
	static constexpr Vei2 canvSize = { 8,8 };
	Surface art;
	Color& mainCol = pal.GetMainCol();
	Color& offCol = pal.GetOffCol();
};