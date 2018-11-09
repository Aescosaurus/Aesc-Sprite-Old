#include "Palette.h"

Palette::Palette( int nColors )
	:
	Palette( GetRandColors( nColors ) )
{}

Palette::Palette( const std::vector<Color>& colors )
{
	swatches.reserve( colors.size() + 1 );

	Vei2 sPos = Vei2( area.left,area.top );
	for( const auto& col : colors )
	{
		swatches.emplace_back( Swatch{ col,RectI{ sPos,
			swatchSize,swatchSize } } );

		sPos.x += swatchSize + swatchPadding;
		if( sPos.x > area.right )
		{
			sPos.x = area.left;
			sPos.y += swatchSize + swatchPadding;
		}
	}
}

Palette::Palette( const std::string& path )
	:
	Palette( GetPalColors( path ) )
{}

void Palette::Update( const Mouse& mouse )
{
	for( Swatch& sw : swatches )
	{
		sw.selected = false;
		if( sw.area.ContainsPoint( mouse.GetPos() ) )
		{
			sw.selected = true;

			if( mouse.LeftIsPressed() ) mainColor = sw.col;
			if( mouse.RightIsPressed() ) offColor = sw.col;
		}
	}
}

void Palette::Draw( Graphics& gfx ) const
{
	Vei2 drawPos = Vei2( area.left,area.top );
	for( const Swatch& sw : swatches )
	{
		const auto swRect = sw.area;

		if( sw.selected )
		{
			const auto bigger = swRect.GetExpanded( swatchPadding / 2 );
			gfx.DrawRect( bigger.left,bigger.top,
				bigger.GetWidth(),bigger.GetHeight(),
				Colors::White );
		}

		gfx.DrawRect( swRect.left,swRect.top,
			swRect.GetWidth(),swRect.GetHeight(),
			sw.col );
	}
}

Color& Palette::GetMainCol()
{
	return( mainColor );
}

Color& Palette::GetOffCol()
{
	return( offColor );
}

std::vector<Color> Palette::GetRandColors( int nColors ) const
{
	std::vector<Color> colors;
	colors.reserve( nColors + 1 );

	typedef unsigned char uchar;
	for( int i = 0; i < nColors; ++i )
	{
		colors.emplace_back( Color{
			uchar( Random::RangeI( 0,255 ) ),
			uchar( Random::RangeI( 0,255 ) ),
			uchar( Random::RangeI( 0,255 ) ) } );
	}

	return( colors );
}

std::vector<Color> Palette::GetPalColors( const std::string& path ) const
{
	Surface temp = path;
	const auto& data = temp.GetRawPixelData();
	std::vector<Color> colors;
	colors.reserve( data.size() + 1 );

	for( const Color c : data )
	{
		colors.emplace_back( c );
	}

	return( colors );
}
