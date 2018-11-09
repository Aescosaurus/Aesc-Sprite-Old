#include "ImageHandler.h"
#include "SpriteEffect.h"

ImageHandler::ImageHandler( const RectI& clipArea )
	:
	art( canvSize.x,canvSize.y ),
	artProxy( canvSize.x,canvSize.y ),
	clipArea( clipArea ),
	artPos( { float( clipArea.left ),float( clipArea.top ) } )
{
	for( int y = 0; y < art.GetHeight(); ++y )
	{
		for( int x = 0; x < art.GetWidth(); ++x )
		{
			art.PutPixel( x,y,Colors::Red );
		}
	}
	artProxy = art;
}

void ImageHandler::Update( Mouse& mouse,
	const Keyboard& kbd,ToolMode tool,
	Color main,Color off )
{
	artProxy = art;
	Vec2 mousePos = Vec2( mouse.GetPos() );

	if( ( tool == ToolMode::Brush || tool == ToolMode::Eraser ) &&
		clipArea.ContainsPoint( Vei2( mousePos ) ) &&
		art.GetExpandedBy( Vei2( scale ) ).GetRect()
		.GetMovedBy( artPos ).ContainsPoint( Vei2( mousePos ) ) )
	{
		mousePos -= artPos;
		mousePos.x /= scale.x;
		mousePos.y /= scale.y;

		const Color* drawColor = nullptr;

		if( mouse.LeftIsPressed() )
		{
			if( tool == ToolMode::Eraser ) drawColor = &chroma;
			else drawColor = &main;
		}
		else if( mouse.RightIsPressed() )
		{
			if( tool != ToolMode::Eraser ) drawColor = &off;
		}

		if( drawColor != nullptr )
		{
			art.PutPixel( int( mousePos.x ),
				int( mousePos.y ),*drawColor );
		}
		else
		{
			Color preview = main;
			if( tool == ToolMode::Eraser ) preview = chroma;
			artProxy.PutPixel( int( mousePos.x ),
				int( mousePos.y ),preview );
		}
	}

	// Use ctrl + scroll or ctrl + plus/minus to zoom in/out.
	if( kbd.KeyIsPressed( VK_CONTROL ) )
	{
		static constexpr float scaleFactor = 1.2f;
		while( !mouse.IsEmpty() )
		{
			const auto e = mouse.Read();
			switch( e.GetType() )
			{
			case Mouse::Event::Type::WheelUp:
				scale *= scaleFactor;
				break;
			case Mouse::Event::Type::WheelDown:
				scale /= scaleFactor;
				break;
			}
		}
		if( kbd.KeyIsPressed( VK_OEM_PLUS ) )
		{
			scale *= scaleFactor;
		}
		if( kbd.KeyIsPressed( VK_OEM_MINUS ) )
		{
			scale /= scaleFactor;
		}
	}

	// Click and drag or use arrow keys to move.
	static constexpr float moveSpeed = 5.5f;
	static constexpr float slowAmount = 0.2f;
	float speed = 2.0f;
	if( kbd.KeyIsPressed( VK_SHIFT ) ) speed *= moveSpeed;
	if( kbd.KeyIsPressed( VK_CONTROL ) ) speed *= slowAmount;
	if( kbd.KeyIsPressed( VK_UP ) ) artPos.y -= speed;
	if( kbd.KeyIsPressed( VK_DOWN ) ) artPos.y += speed;
	if( kbd.KeyIsPressed( VK_LEFT ) ) artPos.x -= speed;
	if( kbd.KeyIsPressed( VK_RIGHT ) ) artPos.x += speed;
	if( tool == ToolMode::Hand )
	{
		if( mouse.LeftIsPressed() )
		{
			artPos += ( mouse.GetPos() - oldMousePos );
		}
		oldMousePos = mouse.GetPos();
	}
}

void ImageHandler::Draw( Graphics& gfx ) const
{
	const Surface drawSurf = artProxy.GetExpandedBy( Vei2( scale ) );
	const auto drawPos = Vei2( artPos );
	gfx.DrawSprite( drawPos.x,drawPos.y,drawSurf.GetRect(),
		clipArea,drawSurf,
		SpriteEffect::Chroma{ Colors::Magenta } );
}
