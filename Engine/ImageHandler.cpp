#include "ImageHandler.h"
#include "SpriteEffect.h"

ImageHandler::ImageHandler( const RectI& clipArea )
	:
	art( canvSize.x,canvSize.y ),
	artProxy( canvSize.x,canvSize.y ),
	clipArea( clipArea ),
	artPos( { float( clipArea.left ),float( clipArea.top ) } ),
	bgPattern( clipArea.GetWidth() / bgGrainAmount,
		clipArea.GetHeight() / bgGrainAmount )
{
	for( int y = 0; y < art.GetHeight(); ++y )
	{
		for( int x = 0; x < art.GetWidth(); ++x )
		{
			art.PutPixel( x,y,chroma );
		}
	}
	artProxy = art;

	static constexpr Color col1 = Colors::MakeRGB( 255,255,255 );
	static constexpr Color col2 = Colors::MakeRGB( 204,204,204 );
	for( int y = 0; y < bgPattern.GetHeight(); ++y )
	{
		for( int x = 0; x < bgPattern.GetWidth(); ++x )
		{
			if( y % 2 == 0 )
			{
				bgPattern.PutPixel( x,y,x % 2 == 0 ? col1 : col2 );
			}
			else
			{
				bgPattern.PutPixel( x,y,x % 2 == 0 ? col2 : col1 );
			}
		}
	}
}

void ImageHandler::Update( Mouse& mouse,
	const Keyboard& kbd,ToolMode tool,
	Color main,Color off )
{
	artProxy = art;
	Vec2 mousePos = Vec2( mouse.GetPos() );

	if( ( tool == ToolMode::Brush || tool == ToolMode::Eraser ||
		tool == ToolMode::Bucket ) &&
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
			if( tool == ToolMode::Bucket )
			{
				TryFillPlusAt( Vei2( mousePos ),*drawColor,
					art.GetPixel( int( mousePos.x ),int( mousePos.y ) ) );
			}

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

		if( kbd.KeyIsPressed( '0' ) )
		{
			CenterImage();
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
	if( clipArea.ContainsPoint( mouse.GetPos() ) &&
		mouse.LeftIsPressed() )
	{
		switch( tool )
		{
		case ToolMode::Hand:
			artPos += ( mouse.GetPos() - oldMousePos );
			break;
		case ToolMode::Zoomer:
		{
			static constexpr float scaleFactor = 1.025f;
			const auto amount = ( mouse.GetPos().x - oldMousePos.x );
			if( amount > 1 ) scale *= scaleFactor;
			else if( amount < -1 ) scale /= scaleFactor;
		}
		break;
		}
	}
	oldMousePos = mouse.GetPos();
}

void ImageHandler::Draw( Graphics& gfx ) const
{
	const Surface drawSurf = artProxy.GetExpandedBy( Vei2( scale ) );
	const auto drawPos = Vei2( artPos );
	const auto drawRect = drawSurf.GetRect();
	// gfx.DrawHitbox( drawRect.GetMovedBy( drawPos ),Colors::Cyan );

	// gfx.DrawSprite( drawPos.x,drawPos.y,
	// 	drawRect.GetMovedBy( drawPos ),
	// 	drawRect.GetMovedBy( drawPos ).GetClipped( clipArea ),
	// 	bgPattern.GetExpandedBy( Vei2{ bgGrainAmount,bgGrainAmount } ),
	// 	SpriteEffect::Chroma{ Colors::Magenta } );
	gfx.DrawSprite( drawPos.x,drawPos.y,
		drawRect,clipArea,
		bgPattern.GetExpandedBy( Vei2{ bgGrainAmount,bgGrainAmount } ),
		SpriteEffect::Chroma{ Colors::Magenta } );

	gfx.DrawSprite( drawPos.x,drawPos.y,drawRect,
		clipArea,drawSurf,
		SpriteEffect::Chroma{ Colors::Magenta } );
}

void ImageHandler::CenterImage()
{
	// TODO: Fix this.
	// const Surface surf = artProxy.GetExpandedBy( Vei2( scale ) );
	// const auto surfRect = surf.GetRect();
	// artPos.x = clipArea.GetWidth() / 2 - surfRect.GetWidth() / 2;
	// artPos.y = clipArea.GetHeight() / 2 - surfRect.GetHeight() / 2;
}

void ImageHandler::TryFillPlusAt( const Vei2& pos,Color c,Color baseFill )
{
	if( c == baseFill ) return;

	art.PutPixel( pos.x,pos.y,c );

	if( pos.y > 0 )
	{
		if( art.GetPixel( pos.x,pos.y - 1 ) == baseFill )
		{
			TryFillPlusAt( pos + Vei2::Up(),c,baseFill );
		}
	}
	if( pos.y < art.GetHeight() - 1 )
	{
		if( art.GetPixel( pos.x,pos.y + 1 ) == baseFill )
		{
			TryFillPlusAt( pos + Vei2::Down(),c,baseFill );
		}
	}
	if( pos.x > 0 )
	{
		if( art.GetPixel( pos.x - 1,pos.y ) == baseFill )
		{
			TryFillPlusAt( pos + Vei2::Left(),c,baseFill );
		}
	}
	if( pos.x < art.GetWidth() - 1 )
	{
		if( art.GetPixel( pos.x + 1,pos.y ) == baseFill )
		{
			TryFillPlusAt( pos + Vei2::Right(),c,baseFill );
		}
	}

	return;
}
