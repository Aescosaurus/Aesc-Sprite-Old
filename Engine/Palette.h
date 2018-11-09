#pragma once

#include "Graphics.h"
#include <vector>
#include "Vec2.h"
#include "Rect.h"
#include "Mouse.h"
#include <string>

class Palette
{
private:
	class Swatch
	{
	public:
		Swatch( Color col,const RectI& rect )
			:
			col( col ),
			area( rect )
		{}
	public:
		Color col;
		RectI area;
		bool selected = false;
	};
public:
	// Initializes with random colors.
	Palette( int nColors );
	// Initialize with specific list of colors.
	Palette( const std::vector<Color>& colors );
	// Initialize with palette image.
	Palette( const std::string& path );

	void Update( const Mouse& mouse );
	void Draw( Graphics& gfx ) const;

	Color& GetMainCol();
	Color& GetOffCol();
private:
	std::vector<Color> GetRandColors( int nColors ) const;
	std::vector<Color> GetPalColors( const std::string& path ) const;
private:
	RectI area = { 4,66,50,Graphics::ScreenHeight - 4 };
	static constexpr int swatchSize = 18;
	static constexpr int swatchPadding = 4;
	std::vector<Swatch> swatches;
	Color mainColor = Colors::White;
	Color offColor = Colors::Gray;
};