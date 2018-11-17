#include "ToolHandler.h"
#include "SpriteEffect.h"
#include <cassert>

ToolHandler::ToolHandler( ToolMode& tool )
	:
	tool( tool )
{}

void ToolHandler::Update( const Mouse& mouse,
	const Keyboard& kbd,Color& main,Color& off )
{
	if( kbd.KeyIsPressed( 'B' ) ) tool = ToolMode::Brush;
	if( kbd.KeyIsPressed( 'E' ) ) tool = ToolMode::Eraser;
	if( kbd.KeyIsPressed( 'H' ) ) tool = ToolMode::Hand;
	if( kbd.KeyIsPressed( 'Z' ) ) tool = ToolMode::Zoomer;
	if( kbd.KeyIsPressed( 'G' ) ) tool = ToolMode::Bucket;
	if( kbd.KeyIsPressed( 'I' ) ) tool = ToolMode::Sampler;

	if( kbd.KeyIsPressed( 'X' ) )
	{
		if( !pressingSwap )
		{
			pressingSwap = true;
			std::swap( main,off );
		}
	}
	else pressingSwap = false;

	if( kbd.KeyIsPressed( 'D' ) )
	{
		main = Colors::Black;
		off = Colors::White;
	}

	// Button stuff.
	if( brush.Update( mouse ) ) tool = ToolMode::Brush;
	if( eraser.Update( mouse ) ) tool = ToolMode::Eraser;
	if( hand.Update( mouse ) ) tool = ToolMode::Hand;
	if( zoomer.Update( mouse ) ) tool = ToolMode::Zoomer;
	if( bucket.Update( mouse ) ) tool = ToolMode::Bucket;
	if( sampler.Update( mouse ) ) tool = ToolMode::Sampler;
}

void ToolHandler::Draw( Graphics& gfx ) const
{
	gfx.DrawSprite( 55,1,GetToolSurf( tool ),
		SpriteEffect::Chroma{ Colors::Magenta } );

	brush.Draw( gfx );
	eraser.Draw( gfx );
	hand.Draw( gfx );
	zoomer.Draw( gfx );
	bucket.Draw( gfx );
	sampler.Draw( gfx );
}

const Surface& ToolHandler::GetToolSurf( ToolMode tool ) const
{
	switch( tool )
	{
	case ToolMode::Brush:
		return( brushImg );
	case ToolMode::Eraser:
		return( eraserImg );
	case ToolMode::Hand:
		return( handImg );
	case ToolMode::Zoomer:
		return( zoomerImg );
	case ToolMode::Bucket:
		return( bucketImg );
	case ToolMode::Sampler:
		return( samplerImg );
	default:
		assert( false );
		return( brushImg );
	}
}
