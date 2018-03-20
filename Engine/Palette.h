#pragma once

#include "Graphics.h"
#include <vector>
#include "Vec2.h"
#include "Rect.h"

class Palette
{
public:
	Palette( int nColors );
	Palette( const std::initializer_list<Color>& colors );

	void Draw( Graphics& gfx ) const;
private:
	RectI area = { 4,66,50,Graphics::ScreenHeight - 4 };
	static constexpr int swatchSize = 18;
	static constexpr int swatchPadding = 4;
	std::vector<Color> colors;
	Random rng;
};