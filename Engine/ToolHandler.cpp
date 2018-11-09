#include "ToolHandler.h"

void ToolHandler::Update( const Mouse& mouse,
	const Keyboard& kbd,ToolMode& tool,
	Color& main,Color& off )
{
	if( kbd.KeyIsPressed( 'B' ) )
	{
		tool = ToolMode::Brush;
	}

	if( kbd.KeyIsPressed( 'E' ) )
	{
		tool = ToolMode::Eraser;
	}

	if( kbd.KeyIsPressed( 'H' ) )
	{
		tool = ToolMode::Hand;
	}

	if( kbd.KeyIsPressed( 'X' ) )
	{
		if( !pressingSwap )
		{
			pressingSwap = true;
			std::swap( main,off );
		}
	}
	else pressingSwap = false;
}

void ToolHandler::Draw( Graphics& gfx ) const
{
}
