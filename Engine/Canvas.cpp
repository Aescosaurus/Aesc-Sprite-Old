#include "Canvas.h"

Canvas::Canvas( Mouse& mouse,MainWindow& wnd )
	:
	mouse( mouse ),
	pal( "Palettes/Default.bmp" ),
	imgHand( screenArea,curTool,mouse,wnd.kbd ),
	toolHand( curTool ),
	fMenu( screenArea,imgHand.GetArt(),wnd )
{}

void Canvas::Update( const Keyboard& kbd )
{
	pal.Update( mouse );
	imgHand.Update( kbd,curTool,mainCol,offCol );
	toolHand.Update( mouse,kbd,mainCol,offCol );
	if( fMenu.Update( mouse,kbd ) )
	{
		imgHand.ResizeCanvas( imgHand.GetArt().GetSize() );
		// imgHand.CreateNewLayer();
		imgHand.UpdateArt();
	}
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
	fMenu.Draw( gfx );

	imgHand.DrawCursor( gfx );
}
