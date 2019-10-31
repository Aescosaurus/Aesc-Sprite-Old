#include "ImageHandler.h"
#include "SpriteEffect.h"
// #include "WriteToBitmap.h"
#include <string>
#include "Utils.h"

ImageHandler::ImageHandler( const RectI& clipArea,ToolMode& curTool,
	Mouse& mouse,Keyboard& kbd )
	:
	art( canvSize.x,canvSize.y ),
	clipArea( clipArea ),
	artPos( { float( clipArea.left ),float( clipArea.top ) } ),
	bgPattern( canvSize.x,canvSize.y ),
	curTool( curTool ),
	mouse( mouse ),
	kbd( kbd ),
	drawSurf( art.GetExpandedBy( Vei2( scale ) ) ),
	bigPattern( bgPattern.GetExpandedBy( Vei2( scale ) ) ),
	layerManager( clipArea,canvSize )
{
	art.DrawRect( 0,0,art.GetWidth(),art.GetHeight(),chroma );

	ResizeCanvas( canvSize );

	drawSurf = art.GetExpandedBy( Vei2( scale ) );
	bigPattern = bgPattern.GetExpandedBy( Vei2( scale ) );

	selectEnd = art.GetSize();
}

void ImageHandler::Update( const Keyboard& kbd,ToolMode tool,
	Color& main,Color& off )
{
	// This has to be at the top.
	if( ( kbd.KeyIsPressed( VK_SPACE ) || tool == ToolMode::Hand ) &&
		mouse.LeftIsPressed() )
	{
		artPos += ( mouse.GetPos() - oldMousePos );
		oldMousePos = mouse.GetPos();
		return;
	}

	const auto oldScale = scale;
	const auto oldArt = art;

	if( tool == ToolMode::Ruler &&
		clipArea.ContainsPoint( mouse.GetPos() ) )
	{
		if( mouse.RightIsPressed() )
		{
			mousePos = mouse.GetPos();
			Vei2 mouseTemp = mouse.GetPos();
			mouseTemp -= Vei2( artPos );
			mouseTemp.x /= int( scale.x );
			mouseTemp.y /= int( scale.y );

			const auto diff = mouse.GetPos() - lastClickPos;
			if( abs( diff.x ) > abs( diff.y ) )
			{
				aesc::remove_item( xGuidelines,mouseTemp.x );
			}
			else
			{
				aesc::remove_item( yGuidelines,mouseTemp.y );
			}
		}
		if( mouse.LeftIsPressed() )
		{
			mousePos = mouse.GetPos();
			Vei2 mouseTemp = mouse.GetPos();
			mouseTemp -= Vei2( artPos );
			mouseTemp.x /= int( scale.x );
			mouseTemp.y /= int( scale.y );

			const auto diff = mouse.GetPos() - lastClickPos;
			if( abs( diff.x ) > abs( diff.y ) )
			{
				guidelineX = true;
				tempGuideline = mouseTemp.x;
			}
			else
			{
				guidelineX = false;
				tempGuideline = mouseTemp.y;
			}

			if( !draggingRuler )
			{
				draggingRuler = true;
				lastClickPos = mouse.GetPos();
			}
		}
		else if( draggingRuler )
		{
			if( guidelineX )
			{
				xGuidelines.emplace_back( tempGuideline );
			}
			else
			{
				yGuidelines.emplace_back( tempGuideline );
			}
			draggingRuler = false;
		}
	}
	else draggingRuler = false;

	if( mouse.LeftIsPressed() && ( tool == ToolMode::Brush ||
		tool == ToolMode::Eraser ) )
	{
		mousePos = mouse.GetPos();
		Vei2 mouseTemp = mouse.GetPos();
		mouseTemp -= Vei2( artPos );
		mouseTemp.x /= int( scale.x );
		mouseTemp.y /= int( scale.y );

		if( kbd.KeyIsPressed( VK_SHIFT ) )
		{
			auto col = main;
			if( tool == ToolMode::Eraser ) col = chroma;
			art.DrawLine( lastClickPos,mouseTemp,col );
		}

		lastClickPos = mouseTemp;
	}

	if( ( kbd.KeyIsPressed( VK_MENU ) || tool == ToolMode::Sampler )
		&& mouse.LeftIsPressed() )
	{
		mousePos = mouse.GetPos();
		Vei2 mouseTemp = mouse.GetPos();
		mouseTemp -= Vei2( artPos );
		mouseTemp.x /= int( scale.x );
		mouseTemp.y /= int( scale.y );
		main = art.GetPixel( mouseTemp.x,mouseTemp.y );
		return;
	}

	// if( kbd.KeyIsPressed( VK_CONTROL ) &&
	// 	kbd.KeyIsPressed( 'S' ) )
	// {
	// 	WriteToBitmap::Write( art,"Output/Test.bmp" );
	// }

	mousePos = mouse.GetPos();
	Vei2 mouseTemp = mouse.GetPos();

	if( ( tool == ToolMode::Brush || tool == ToolMode::Eraser ||
		tool == ToolMode::Bucket || tool == ToolMode::Sampler ) &&
		clipArea.ContainsPoint( Vei2( mouseTemp ) ) &&
		drawSurf.GetRect().GetMovedBy( artPos )
		.ContainsPoint( Vei2( mouseTemp ) ) )
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

		if( drawColor != nullptr &&
			!kbd.KeyIsPressed( VK_SPACE ) )
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
		if( kbd.KeyIsPressed( VK_SPACE ) ||
			tool == ToolMode::Hand )
		{
			artPos += ( mouse.GetPos() - oldMousePos );
		}
		if( tool == ToolMode::Zoomer )
		{
			static constexpr float scaleFactor = 1.025f;
			const auto amount = ( mouse.GetPos().x - oldMousePos.x );
			if( amount > 1 ) scale *= scaleFactor;
			else if( amount < -1 ) scale /= scaleFactor;
		}
	}

	if( curTool == ToolMode::Resizer &&
		clipArea.ContainsPoint( mousePos ) )
	{
		if( mouse.LeftIsPressed() )
		{
			const Vei2 pixelSize = Vei2( Vec2{
				( int( canvSize.x ) * scale.x ) / int( canvSize.x ),
				( int( canvSize.y ) * scale.y ) / int( canvSize.y ) } );

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

			resizeArea.FloatDivide( Vei2( scale ) );

			ResizeCanvas( { abs( resizeArea.GetWidth() ),
				abs( resizeArea.GetHeight() ) } );

			artPos.x = float( cropStart.x );
			artPos.y = float( cropStart.y );

			UpdateArt();
		}
	}

	if( curTool == ToolMode::Selector )
	{
		if( mouse.LeftIsPressed() )
		{
			selectingStuff = true;
			if( canSelect )
			{
				selectStart = mouse.GetPos();
				selectStart -= Vei2( artPos );
				selectStart.x /= int( scale.x );
				selectStart.y /= int( scale.y );

				canSelect = false;
			}

			selectEnd = mouse.GetPos();
			selectEnd -= Vei2( artPos );
			selectEnd.x /= int( scale.x );
			selectEnd.y /= int( scale.y );

			if( kbd.KeyIsPressed( VK_SHIFT ) )
			{
				RectI resizeArea = { selectStart.x,selectEnd.x,
					selectStart.y,selectEnd.y };
				if( resizeArea.GetWidth() != resizeArea.GetHeight() )
				{
					resizeArea.Squareize();
					selectEnd.x = resizeArea.right;
					selectEnd.y = resizeArea.bottom;
				}
			}
		}
		else
		{
			if( selectEnd.x < selectStart.x )
			{
				std::swap( selectStart.x,selectEnd.x );
			}
			if( selectEnd.y < selectStart.y )
			{
				std::swap( selectStart.y,selectEnd.y );
			}
			canSelect = true;
		}
	}

	if( curTool == ToolMode::Pointer )
	{
		// oldMousePos
		// clickingLastFrame
		if( mouse.LeftIsPressed() )
		{
			if( !draggingPointer )
			{
				draggingPointer = true;
				pointerStart = mouse.GetPos();
				pointerDelta = pointerStart - Vei2(
					( selectStart.x + int( artPos.x ) ) *
					int( scale.x ),
					( selectStart.y + int( artPos.y ) ) *
					int( scale.y ) );
				pointerDelta = pointerStart - Vei2(
					selectStart.x * int( scale.x ) + int( artPos.x ),
					selectStart.y * int( scale.y ) + int( artPos.y ) );
				miniPointerMoveClip = art.GetClipped( RectI{
					selectStart,selectEnd } );
				pointerMoveClip = miniPointerMoveClip
					.GetExpandedBy( Vei2( scale ) );

				// remove rect from art
				art.DrawRect( selectStart.x,selectStart.y,
					selectEnd.x - selectStart.x,
					selectEnd.y - selectStart.y,
					Colors::Magenta );
			}
			else
			{
				pointerPos = mouse.GetPos() - pointerDelta;
				pointerPos -= Vei2( artPos );
				pointerPos.x /= int( scale.x );
				pointerPos.y /= int( scale.y );
				pointerPos.x *= int( scale.x );
				pointerPos.y *= int( scale.y );
				pointerPos += Vei2( artPos );

				const auto pointerClipMovePos = Vei2{
					( pointerPos.x - int( artPos.x ) ) / int( scale.x ),
					( pointerPos.y - int( artPos.y ) ) / int( scale.y ) };
				selectStart = pointerClipMovePos;
				selectEnd = pointerClipMovePos + Vei2(
					pointerMoveClip.GetWidth() / int( scale.x ),
					pointerMoveClip.GetHeight() / int( scale.y ) );
			}
		}
		else if( draggingPointer )
		{
			if( selectStart.x >= 0 && selectStart.y >= 0 &&
				selectStart.x <= art.GetWidth() && selectStart.y <= art.GetHeight() &&
				selectEnd.x >= 0 && selectEnd.y >= 0 &&
				selectEnd.x <= art.GetWidth() && selectEnd.y <= art.GetHeight() )
			{
				draggingPointer = false;

				const auto pointerClipMovePos = Vei2{
					( pointerPos.x - int( artPos.x ) ) / int( scale.x ),
					( pointerPos.y - int( artPos.y ) ) / int( scale.y ) };

				art.LightCopyIntoPos( miniPointerMoveClip,
					selectStart );

				pointerMoveClip = Surface{ 0,0 };
			}
		}
	}

	if( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'A' ) )
	{
		selectingStuff = true;
		selectStart = Vei2{ 0,0 };
		selectEnd = Vei2{ art.GetWidth(),art.GetHeight() };
	}
	if( kbd.KeyIsPressed( VK_ESCAPE ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'D' ) ) )
	{
		// selectingStuff = false;
		selectStart = Vei2{ 0,0 };
		selectEnd = Vei2{ art.GetWidth(),art.GetHeight() };
	}
	if( // selectingStuff &&
		kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'C' ) )
	{
		clipboard = art.GetCropped( selectStart,selectEnd );
		clipboardPos = selectStart;
	}
	if( clipboard.GetWidth() > 0 && clipboard.GetHeight() > 0 &&
		kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'V' ) )
	{
		if( canPaste )
		{
			layerManager.CreateNewLayer( art );
			art.DrawRect( 0,0,art.GetWidth(),art.GetHeight(),
				Colors::Magenta );
			art.CopyIntoPos( clipboard,clipboardPos );
		}
		canPaste = false;
	}
	else canPaste = true;

	// if( !mouse.LeftIsPressed() && !appliedSelect )
	// {
	// 	appliedSelect = true;
	// 
	// 	RectI resizeArea = { selectStart.x,selectEnd.x,
	// 		selectStart.y,selectEnd.y };
	// 
	// 	resizeArea.FloatDivide( Vei2( scale ) );
	// }

	const bool willUpdate = layerManager.Update( kbd,mouse,art );
	const bool isHoveringLayer = layerManager.GetSelectedLayer() != -1;

	if( scale.x != oldScale.x || scale.y != oldScale.y ||
		art != oldArt || isHoveringLayer || hoveringLastFrame ||
		willUpdate )
	{
		if( layerManager.IsSelectedLayerLocked() ) art = oldArt;
		if( isHoveringLayer ) hoveringLastFrame = true;
		else hoveringLastFrame = false;
		UpdateArt();
	}

	oldMousePos = mouse.GetPos();
	clickingLastFrame = mouse.LeftIsPressed();
}

void ImageHandler::Draw( Graphics& gfx ) const
{
	const auto drawPos = Vei2( artPos );
	const auto drawRect = drawSurf.GetRect();

	gfx.DrawSprite( drawPos.x,drawPos.y,
		bigPattern.GetRect(),clipArea,bigPattern,
		SpriteEffect::Chroma{ Colors::Magenta } );

	gfx.DrawSprite( drawPos.x,drawPos.y,drawRect,
		clipArea,drawSurf,
		SpriteEffect::Chroma{ Colors::Magenta } );

	// if( !selectingStuff )
	// {
	// 	if( selectedLayerRect.left != -1 &&
	// 		selectedLayerRect.right != -1 &&
	// 		selectedLayerRect.top != -1 &&
	// 		selectedLayerRect.bottom != -1 )
	// 	{
	// 		gfx.DrawHitboxCorners( selectedLayerRect
	// 			.GetMovedBy( drawPos ),Colors::Gray );
	// 	}
	// }
	// else
	{
		RectI selectorRect = { selectStart.x,selectEnd.x,
			selectStart.y,selectEnd.y };
		selectorRect = selectorRect.GetExpandedByScale( Vei2( scale ) );
		selectorRect.MoveBy( Vei2( artPos ) );
		gfx.DrawHitboxCorners( selectorRect,Colors::Gray );
	}

	// Guidelines stuff.
	const auto clipTop = float( clipArea.top );
	const auto clipBot = float( clipArea.bottom );
	const auto clipLeft = float( clipArea.left );
	const auto clipRight = float( clipArea.right );
	for( const auto x : xGuidelines )
	{
		const auto pos = float( x * int( scale.x ) +
			int( artPos.x ) );
		if( pos > clipLeft && pos < clipRight )
		{
			gfx.DrawLine( { pos,clipTop },{ pos,clipBot },
				Colors::Cyan );
		}
	}
	for( const auto y : yGuidelines )
	{
		const auto pos = float( y * int( scale.y ) +
			int( artPos.y ) );
		if( pos > clipTop && pos < clipBot )
		{
			gfx.DrawLine( { clipLeft,pos },{ clipRight,pos },
				Colors::Cyan );
		}
	}
	if( draggingRuler )
	{
		if( guidelineX )
		{
			const auto pos = float( tempGuideline *
				int( scale.x ) + int( artPos.x ) );
			if( pos > clipLeft && pos < clipRight )
			{
				gfx.DrawLine( { pos,clipTop },{ pos,clipBot },
					Colors::Cyan );
			}
		}
		else
		{
			const auto pos = float( tempGuideline *
				int( scale.y ) + int( artPos.y ) );
			if( pos > clipTop && pos < clipBot )
			{
				gfx.DrawLine( { clipLeft,pos },{ clipRight,pos },
					Colors::Cyan );
			}
		}
	}

	if( curTool == ToolMode::Pointer && draggingPointer )
	{
		gfx.DrawSprite( pointerPos.x,
			pointerPos.y,
			pointerMoveClip.GetRect(),clipArea,
			pointerMoveClip,
			SpriteEffect::Chroma{ Colors::Magenta } );

	}

	layerManager.Draw( gfx );

	// DrawCursor( gfx );
}

void ImageHandler::CenterImage()
{
	const auto artRect = art.GetExpandedBy( scale ).GetRect();

	artPos = Vec2( clipArea.GetSize() ) / 2.0f -
		Vec2( artRect.GetSize() ) / 2.0f +
		Vec2{ float( clipArea.left ),float( clipArea.top ) };
}

void ImageHandler::UpdateArt()
{
	layerManager.Update( kbd,mouse,art );

	drawSurf = Surface{ art.GetWidth(),art.GetHeight() };
	drawSurf.DrawRect( 0,0,
		drawSurf.GetWidth(),drawSurf.GetHeight(),
		Colors::Magenta );

	const auto& layers = layerManager.GetLayers();
	const auto& hiddenLayers = layerManager.GetHiddenLayers();
	const auto curSelectedLayer = layerManager.GetSelectedLayer();
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( !hiddenLayers[i] )
		{
			drawSurf.LightCopyInto( layers[i] );
		}
		if( ( i == layerManager.GetActualSelectedLayer() &&
			curSelectedLayer == -1 ) || i == curSelectedLayer )
		{
			selectedLayerRect = layers[i].GetNonMagentaRect();
		}
	}
	drawSurf = drawSurf.GetExpandedBy( Vei2( scale ) );
	bigPattern = bgPattern.GetExpandedBy( Vei2( scale ) );
	
	if( selectedLayerRect.left != -1 &&
		selectedLayerRect.right != -1 &&
		selectedLayerRect.top != -1 &&
		selectedLayerRect.bottom != -1 )
	{
		++selectedLayerRect.right;
		++selectedLayerRect.bottom;
		selectedLayerRect = selectedLayerRect
			.GetExpandedByScale( Vei2( scale ) );
	}
}

Surface& ImageHandler::GetArt()
{
	return( art );
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

	// Surface temp = art;
	// art = Surface{ canvSize.x,canvSize.y };
	// art.DrawRect( 0,0,art.GetWidth(),art.GetHeight(),chroma );
	// art.CopyInto( temp );
	art.Resize( newSize );

	bgPattern = { canvSize.x,canvSize.y };

	static constexpr Color c1 = Colors::MakeRGB( 255,255,255 );
	static constexpr Color c2 = Colors::MakeRGB( 204,204,204 );
	for( int y = 0; y < bgPattern.GetHeight(); ++y )
	{
		for( int x = 0; x < bgPattern.GetWidth(); ++x )
		{
			if( y % 2 == 0 )
			{
				bgPattern.PutPixel( x,y,x % 2 == 0 ? c1 : c2 );
			}
			else
			{
				bgPattern.PutPixel( x,y,x % 2 == 0 ? c2 : c1 );
			}
		}
	}

	layerManager.ResizeCanvas( newSize );
}

void ImageHandler::CreateNewLayer()
{
	layerManager.CreateNewLayer( art );
}

void ImageHandler::UpdateSelectArea()
{
	selectStart = { 0,0 };
	selectEnd = art.GetSize();
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

	const auto mousePos = mouse.GetPos();

	const Color cursorCol = art.GetExpandedBy( scale )
		.GetRect().GetMovedBy( artPos ).ContainsPoint( mousePos )
		? Colors::DarkGray : Colors::LightGray;
	if( clipArea.ContainsPoint( mouse.GetPos() ) )
	{
		switch( curTool )
		{
		case ToolMode::Brush:
		{
			const auto rect = DrawSquare( cursorCol,gfx );
			gfx.DrawLine( mousePos + Vei2::Left() * ( rect.GetWidth() / 4 ),
				mousePos + Vei2::Right() * ( rect.GetWidth() / 4 ),cursorCol );
			gfx.DrawLine( mousePos + Vei2::Up() * ( rect.GetHeight() / 4 ),
				mousePos + Vei2::Down() * ( rect.GetHeight() / 4 ),cursorCol );
		}
		break;
		case ToolMode::Eraser:
		{
			const auto rect = DrawSquare( cursorCol,gfx );
			gfx.DrawLine( mousePos + Vei2::Left() * ( rect.GetWidth() / 8 ),
				mousePos + Vei2::Right() * ( rect.GetWidth() / 8 ),cursorCol );
		}
		break;
		case ToolMode::Hand:
			gfx.DrawSprite( mouse.GetPosX(),mouse.GetPosY(),miniHand,
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

			if( resizeArea.IsContainedBy( clipArea ) && !canCrop )
			{
				gfx.DrawHitboxInverse( resizeArea );
			}

			if( !canCrop )
			{
				resizeArea.FloatDivide( Vec2( Vei2( scale ) ) );

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
		case ToolMode::Ruler:
			if( mouse.LeftIsPressed() )
			{
				const auto text = std::to_string( tempGuideline );
				luckyPixel.DrawText( text,mousePos + Vei2{ 32,0 },Colors::Black,
					SpriteEffect::Inverse{ Colors::White },gfx );
			}

			gfx.DrawSprite( mousePos.x,mousePos.y,miniRuler,
				SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
			break;
		case ToolMode::Selector:
			gfx.DrawSprite( mousePos.x,mousePos.y,miniSelector,
				SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
			break;
		case ToolMode::Pointer:
			gfx.DrawSprite( mousePos.x,mousePos.y,miniPointer,
				SpriteEffect::Substitution( Colors::Magenta,cursorCol ) );
		}
	}
	else
	{
		gfx.DrawSprite( mouse.GetPosX(),mouse.GetPosY(),
			miniPointer,SpriteEffect::Inverse{ Colors::Magenta } );
	}
}

Surface ImageHandler::GetLayeredArt() const
{
	auto temp = Surface{ art.GetWidth(),art.GetHeight() };
	temp.DrawRect( 0,0,
		temp.GetWidth(),temp.GetHeight(),
		Colors::Magenta );
	const auto& layers = layerManager.GetLayers();
	const auto& hiddenLayers = layerManager.GetHiddenLayers();
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( !hiddenLayers[i] )
		{
			temp.LightCopyInto( layers[i] );
		}
	}
	return( temp );
}
