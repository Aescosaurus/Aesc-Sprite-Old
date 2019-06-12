#include "Canvas.h"

Canvas::Canvas( Mouse& mouse )
	:
	mouse( mouse ),
	pal( "Palettes/Default.bmp" ),
	imgHand( screenArea,curTool,mouse ),
	toolHand( curTool )
{}

void Canvas::Update( const Keyboard& kbd )
{
	pal.Update( mouse );
	imgHand.Update( kbd,curTool,mainCol,offCol );
	toolHand.Update( mouse,kbd,mainCol,offCol );
}

void Canvas::Draw( Graphics& gfx ) const
{
	gfx.DrawRectDim( screenArea.left,screenArea.top,
		screenArea.right,screenArea.bottom,
		Colors::DarkGray );

	gfx.DrawCircle( 35,30,15,offCol );
	gfx.DrawCircle( 20,20,15,mainCol );

	pal.Draw( gfx );

	imgHand.Draw( gfx );
	toolHand.Draw( gfx );
	imgHand.DrawCursor( gfx );
}
