#include "ImageHandler.h"
#include "SpriteEffect.h"
#include "WriteToBitmap.h"
#include <string>

ImageHandler::ImageHandler( const RectI& clipArea,ToolMode& curTool )
	:
	art( canvSize.x,canvSize.y ),
	clipArea( clipArea ),
	artPos( { float( clipArea.left ),float( clipArea.top ) } ),
	bgPattern( clipArea.GetWidth() / bgGrainAmount,
		clipArea.GetHeight() / bgGrainAmount ),
	curTool( curTool )
{
	art.DrawRect( 0,0,art.GetWidth(),art.GetHeight(),chroma );

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
	Color& main,Color& off )
{
	if( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'S' ) )
	{
		WriteToBitmap::Write( art,"Output/Test.bmp" );
	}

	mousePos = mouse.GetPos();
	Vei2 mouseTemp = mouse.GetPos();

	if( ( tool == ToolMode::Brush || tool == ToolMode::Eraser ||
		tool == ToolMode::Bucket || tool == ToolMode::Sampler ) &&
		clipArea.ContainsPoint( Vei2( mouseTemp ) ) &&
		art.GetExpandedBy( Vei2( scale ) ).GetRect()
		.GetMovedBy( artPos ).ContainsPoint( Vei2( mouseTemp ) ) )
	{
		mouseTemp -= Vei2( artPos );
		mouseTemp.x /= int( scale.x );
		mouseTemp.y /= int( scale.y );

		const Color* drawColor = nullptr;

		if( mouse.LeftIsPressed() )
		{
			if( tool == ToolMode::Eraser ) drawColor = &chroma;
			else if( tool == ToolMode::Sampler )
			{
				main = art.GetPixel( mouseTemp.x,mouseTemp.y );
			}
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
				TryFillPlusAt( mouseTemp,*drawColor,
					art.GetPixel( mouseTemp.x,mouseTemp.y ) );
			}

			art.PutPixel( mouseTemp.x,mouseTemp.y,
				*drawColor );
		}
	}
	static constexpr float scaleFactor = 1.2f;
	static constexpr float moveSpeed = 5.5f;

	// Use ctrl + scroll or ctrl + plus/minus to zoom in/out.
	while( !mouse.IsEmpty() )
	{
		const auto e = mouse.Read();
		switch( e.GetType() )
		{
		case Mouse::Event::Type::WheelUp:
			if( kbd.KeyIsPressed( VK_CONTROL ) ) scale *= scaleFactor;
			else if( kbd.KeyIsPressed( VK_SHIFT ) ) artPos.x += moveSpeed;
			else artPos.y += moveSpeed;
			break;
		case Mouse::Event::Type::WheelDown:
			if( kbd.KeyIsPressed( VK_CONTROL ) ) scale /= scaleFactor;
			else if( kbd.KeyIsPressed( VK_SHIFT ) ) artPos.x -= moveSpeed;
			else artPos.y -= moveSpeed;
			break;
		}
	}
	if( kbd.KeyIsPressed( VK_CONTROL ) )
	{
		if( kbd.KeyIsPressed( VK_OEM_PLUS ) )
		{
			scale *= scaleFactor;
		}
		if( kbd.KeyIsPressed( VK_OEM_MINUS ) )
		{
			scale /= scaleFactor;
		}

		if( kbd.KeyIsPressed( '0' ) ) CenterImage();
	}

	// Click and drag or use arrow keys to move.
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

	if( curTool == ToolMode::Resizer &&
		clipArea.ContainsPoint( mousePos ) )
	{
		if( mouse.LeftIsPressed() )
		{
			const Vei2 pixelSize = Vei2( Vec2{
				( float( canvSize.x ) * scale.x ) / float( canvSize.x ),
				( float( canvSize.y ) * scale.y ) / float( canvSize.y ) } );

			appliedCrop = false;
			if( canCrop )
			{
				cropStart = mouse.GetPos();
				while( cropStart.x % pixelSize.x != 0 ) --cropStart.x;
				while( cropStart.y % pixelSize.y != 0 ) --cropStart.y;

				canCrop = false;
			}

			cropEnd = mouse.GetPos();
			while( cropEnd.x % pixelSize.x != 0 ) --cropEnd.x;
			while( cropEnd.y % pixelSize.y != 0 ) --cropEnd.y;

			if( kbd.KeyIsPressed( VK_SHIFT ) )
			{
				RectI resizeArea = { cropStart.x,cropEnd.x,
					cropStart.y,cropEnd.y };
				if( resizeArea.GetWidth() != resizeArea.GetHeight() )
				{
					resizeArea.Squareize();
					cropEnd.x = resizeArea.right;
					cropEnd.y = resizeArea.bottom;
					while( cropEnd.x % pixelSize.x != 0 ) --cropEnd.x;
					while( cropEnd.y % pixelSize.y != 0 ) --cropEnd.y;
				}
			}
		}
		else
		{
			canCrop = true;
			// cropEnd = mouse.GetPos();
		}

		// if( kbd.KeyIsPressed( VK_RETURN ) )
		if( !mouse.LeftIsPressed() && !appliedCrop )
		{
			appliedCrop = true;

			RectI resizeArea = { cropStart.x,cropEnd.x,
				cropStart.y,cropEnd.y };
			// resizeArea.MoveBy( -artPos );

			resizeArea.FloatDivide( scale );

			Surface temp{ abs( resizeArea.GetWidth() ),
				abs( resizeArea.GetHeight() ) };
			temp.DrawRect( 0,0,temp.GetWidth(),temp.GetHeight(),chroma );
			temp.CopyInto( art );

			art = temp;

			artPos.x = float( cropStart.x );
			artPos.y = float( cropStart.y );
		}
	}

	oldMousePos = mouse.GetPos();
}

void ImageHandler::Draw( Graphics& gfx ) const
{
	const Surface drawSurf = art.GetExpandedBy( Vei2( scale ) );
	const auto drawPos = Vei2( artPos );
	const auto drawRect = drawSurf.GetRect();

	gfx.DrawSprite( drawPos.x,drawPos.y,
		drawRect,clipArea,
		bgPattern.GetExpandedBy( Vei2{ bgGrainAmount,bgGrainAmount } ),
		SpriteEffect::Chroma{ Colors::Magenta } );

	gfx.DrawSprite( drawPos.x,drawPos.y,drawRect,
		clipArea,drawSurf,
		SpriteEffect::Chroma{ Colors::Magenta } );

	DrawCursor( gfx );
}

void ImageHandler::CenterImage()
{
	const auto artRect = art.GetExpandedBy( scale ).GetRect();

	artPos = Vec2( clipArea.GetSize() ) / 2.0f -
		Vec2( artRect.GetSize() ) / 2.0f +
		Vec2{ float( clipArea.left ),float( clipArea.top ) };
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

void ImageHandler::ResizeCanvas( const Vei2& newSize )
{
	canvSize = newSize;

	Surface temp = art;
	art = Surface{ canvSize.x,canvSize.y };
	art.DrawRect( 0,0,art.GetWidth(),art.GetHeight(),chroma );
	art.CopyInto( temp );
}

void ImageHandler::DrawCursor( Graphics& gfx ) const
{
	const auto DrawSquare = [&]( Color lineColor,Graphics& gfx )
	{
		const Vei2 pixelSize = Vei2( Vec2{
			( float( canvSize.x ) * scale.x ) / float( canvSize.x ),
			( float( canvSize.y ) * scale.y ) / float( canvSize.y ) } );
		RectI rect = { 0,pixelSize.x,0,pixelSize.y };
		rect.MoveTo( mousePos - Vec2( rect.GetSize() ) / 2.0f );
		if( rect.IsContainedBy( clipArea ) )
		{
			gfx.DrawHitbox( rect,lineColor );
			return( rect );
		}
		return( RectI{ -9999,-9999,-9999,-9999 } );
	};

	const Color cursorCol = art.GetExpandedBy( scale )
		.GetRect().GetMovedBy( artPos ).ContainsPoint( mousePos )
		? Colors::DarkGray : Colors::LightGray;
	switch( curTool )
	{
	case ToolMode::Brush:
	{
		const auto rect = DrawSquare( cursorCol,gfx );
		if( rect.IsContainedBy( clipArea ) )
		{
			gfx.DrawLine( mousePos + Vei2::Left() * ( rect.GetWidth() / 4 ),
				mousePos + Vei2::Right() * ( rect.GetWidth() / 4 ),cursorCol );
			gfx.DrawLine( mousePos + Vei2::Up() * ( rect.GetHeight() / 4 ),
				mousePos + Vei2::Down() * ( rect.GetHeight() / 4 ),cursorCol );
		}
	}
	break;
	case ToolMode::Eraser:
	{
		const auto rect = DrawSquare( cursorCol,gfx );
		if( rect.IsContainedBy( clipArea ) )
		{
			gfx.DrawLine( mousePos + Vei2::Left() * ( rect.GetWidth() / 8 ),
				mousePos + Vei2::Right() * ( rect.GetWidth() / 8 ),cursorCol );
		}
	}
	break;
	case ToolMode::Hand:
		gfx.DrawSprite( mousePos.x,mousePos.y,miniHand,
			SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		break;
	case ToolMode::Zoomer:
		gfx.DrawSprite( mousePos.x,mousePos.y,miniZoomer,
			SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		break;
	case ToolMode::Bucket:
		gfx.DrawSprite( mousePos.x,mousePos.y,miniBucket,
			SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		break;
	case ToolMode::Sampler:
		gfx.DrawSprite( mousePos.x,mousePos.y,miniSampler,
			SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		break;
	case ToolMode::Resizer:
		RectI resizeArea = { cropStart.x,cropEnd.x,
			cropStart.y,cropEnd.y };
		if( resizeArea.IsContainedBy( clipArea ) )
		{
			gfx.DrawHitboxInverse( resizeArea );
		}

		if( !canCrop )
		{
			resizeArea.FloatDivide( scale );

			const auto text = std::to_string( resizeArea.GetWidth() ) +
				"x" + std::to_string( resizeArea.GetHeight() );
			luckyPixel.DrawText( text,mousePos,Colors::Black,
				SpriteEffect::Inverse{ Colors::White },gfx );
		}
		else
		{
			gfx.DrawSprite( mousePos.x,mousePos.y,miniResizer,
				SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		}
		break;
	}
}
