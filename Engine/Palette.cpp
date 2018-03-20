#include "Palette.h"

Palette::Palette( int nColors )
{
	for( int i = 0; i < nColors; ++i )
	{
		colors.emplace_back( Color{ unsigned char( rng
			.RangeI( 0,255 ) ),unsigned char( rng
			.RangeI( 0,255 ) ),unsigned char( rng
			.RangeI( 0,255 ) ) } );
	}
}

Palette::Palette( const std
	::initializer_list<Color>& colors )
	:
	colors( colors )
{
}

void Palette::Draw( Graphics& gfx ) const
{
	Vei2 drawPos = Vei2( area.left,area.top );
	for( const Color& c : colors )
	{
		gfx.DrawRect( drawPos.x,drawPos.y,
			swatchSize,swatchSize,c );

		drawPos.x += swatchSize + swatchPadding;
		if( drawPos.x > area.right )
		{
			drawPos.x = area.left;
			drawPos.y += swatchSize + swatchPadding;
		}
	}
}
