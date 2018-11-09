#include "Canvas.h"

Canvas::Canvas()
	:
	pal( "Palettes/Default.bmp" ),
	art( canvSize.x,canvSize.y )
{}

void Canvas::Update( const Mouse& mouse,const Keyboard& kbd )
{
	pal.Update( mouse );
}

void Canvas::Draw( Graphics& gfx ) const
{
	gfx.DrawRectDim( screenArea.left,screenArea.top,
		screenArea.right,screenArea.bottom,
		Colors::Gray );

	gfx.DrawCircle( 20,20,15,mainCol );
	gfx.DrawCircle( 35,30,15,offCol );

	pal.Draw( gfx );
}
