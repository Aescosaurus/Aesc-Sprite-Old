#include "Canvas.h"

Canvas::Canvas()
	:
	pal( "Palettes/Default.bmp" ),
	imgHand( screenArea )
{}

void Canvas::Update( Mouse& mouse,const Keyboard& kbd )
{
	pal.Update( mouse );
	imgHand.Update( mouse,kbd,curTool,mainCol,offCol );
	toolHand.Update( mouse,kbd,curTool,mainCol,offCol );
}

void Canvas::Draw( Graphics& gfx ) const
{
	gfx.DrawRectDim( screenArea.left,screenArea.top,
		screenArea.right,screenArea.bottom,
		Colors::Gray );

	gfx.DrawCircle( 35,30,15,offCol );
	gfx.DrawCircle( 20,20,15,mainCol );

	pal.Draw( gfx );

	imgHand.Draw( gfx );
	toolHand.Draw( gfx );
}
