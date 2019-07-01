#include "LayerManager.h"
#include "SpriteEffect.h"

LayerManager::LayerManager( const RectI& clipArea,const Vei2& canvSize )
	:
	canvSize( canvSize ),
	drawArea( clipArea.right + padding.x,
		Graphics::ScreenWidth - padding.x,
		clipArea.bottom - 237,clipArea.bottom )
{
	layers.emplace_back( Surface{ canvSize.x,canvSize.y } );
	layers.back().DrawRect( 0,0,canvSize.x,canvSize.y,Colors::Magenta );

	const auto start = Vei2{ drawArea.left + padding.x * 2 + buttonSize.x,
		drawArea.top + padding.y };
	const auto start2 = Vei2{ drawArea.left + padding.x,
		drawArea.top + padding.y };
	for( int i = 0; i < 7; ++i )
	{
		layerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/LayerButton.bmp" },Vei2{ 3,3 } },
			start + ( padding.Y() + buttonSize.Y() ) * i } );

		hideLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/HideLayerButton.bmp" },Vei2{ 3,3 } },
			start2 + ( padding.Y() + buttonSize.Y() ) * i } );
		unhideLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/UnhideLayerButton.bmp" },Vei2{ 3,3 } },
			start2 + ( padding.Y() + buttonSize.Y() ) * i } );

		hiddenLayers.emplace_back( false );
	}
}

bool LayerManager::Update( const Keyboard& kbd,const Mouse& mouse,Surface& art )
{
	layers[selectedLayer].CopyInto( art );

	if( addLayer.Update( mouse ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'N' ) ) )
	{
		if( layers.size() < 7 )
		{
			layers.emplace_back( Surface{ canvSize.x,canvSize.y } );
			layers.back().DrawRect( 0,0,canvSize.x,canvSize.y,Colors::Magenta );
			selectedLayer = int( layers.size() ) - 1;
			art.CopyInto( layers[selectedLayer] );
		}
	}
	if( dupeLayer.Update( mouse ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'J' ) ) )
	{
		if( layers.size() < 7 )
		{
			// layers.push_back( layers.back() );
			// ++selectedLayer;
			layers.emplace_back( Surface{ layers[selectedLayer] } );
			selectedLayer = int( layers.size() ) - 1;
			art.CopyInto( layers[selectedLayer] );
		}
	}
	if( removeLayer.Update( mouse ) ||
		kbd.KeyIsPressed( VK_DELETE ) )
	{
		if( layers.size() > 1 )
		{
			// layers.pop_back();
			layers.erase( layers.begin() + selectedLayer );
			--selectedLayer;
			if( selectedLayer < 0 ) selectedLayer = 0;
			if( selectedLayer > int( layers.size() ) ) selectedLayer = int( layers.size() );
			art.CopyInto( layers[selectedLayer] );
		}
	}

	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( layerButtons[i].Update( mouse ) )
		{
			selectedLayer = i;
			art.CopyInto( layers[selectedLayer] );
		}

		if( !hiddenLayers[i] )
		{
			if( hideLayerButtons[i].Update( mouse ) )
			{
				hiddenLayers[i] = true;
				return( true );
			}
		}
		else
		{
			if( unhideLayerButtons[i].Update( mouse ) )
			{
				hiddenLayers[i] = false;
				return( true );
			}
		}
	}
	return( false );
}

void LayerManager::Draw( Graphics& gfx ) const
{
	// TODO: Remove this ----------------------------------
	gfx.DrawRect( drawArea.left,drawArea.top,
		drawArea.GetWidth(),drawArea.GetHeight(),
		Colors::DarkGray );
	// ----------------------------------------------------
	
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( i == selectedLayer )
		{
			// gfx.DrawSprite( layerButtons[i].GetPos().x + 27 * 3 + 5,
			// 	layerButtons[i].GetPos().y,layerSelectedButton,
			// 	SpriteEffect::Chroma{ Colors::Magenta } );
			const int layerHeight = buttonSize.y + padding.y;
			gfx.DrawRect( drawArea.left,drawArea.top +
				( layerHeight * i ) + 3,drawArea.GetWidth(),
				layerHeight + padding.y - 6,Colors::White );
		}

		layerButtons[i].Draw( gfx );

		if( !hiddenLayers[i] ) hideLayerButtons[i].Draw( gfx );
		else unhideLayerButtons[i].Draw( gfx );

		const float ratio = float( layers[i].GetWidth() ) /
			float( layers[i].GetHeight() );
		float width = ratio * float( 18 );
		float height = 18;
		if( width > 25 * 3 )
		{
			width = 25 * 3;
			height = width / ratio;
		}

		gfx.DrawSprite( layerButtons[i].GetPos().x + 3,
			layerButtons[i].GetPos().y + 3,layers[i]
			.GetInterpolatedTo( width,height ),
			SpriteEffect::Copy{} );
	}

	addLayer.Draw( gfx );
	dupeLayer.Draw( gfx );
	removeLayer.Draw( gfx );
}

void LayerManager::ResizeCanvas( const Vei2& newSize )
{
	canvSize = newSize;

	for( auto& layer : layers )
	{
		// Surface temp = layer;
		// layer = Surface{ newSize.x,newSize.y };
		// layer.CopyInto( temp );
		layer.Resize( newSize );
	}
}

void LayerManager::CreateNewLayer( Surface& art )
{
	if( layers.size() < 7 )
	{
		layers.emplace_back( Surface{ canvSize.x,canvSize.y } );
		layers.back().DrawRect( 0,0,canvSize.x,canvSize.y,Colors::Magenta );
		selectedLayer = int( layers.size() ) - 1;
		art.CopyInto( layers[selectedLayer] );
	}
}

const std::vector<Surface>& LayerManager::GetLayers() const
{
	return( layers );
}

const std::vector<bool>& LayerManager::GetHiddenLayers() const
{
	return( hiddenLayers );
}

int LayerManager::GetSelectedLayer() const
{
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		if( layerButtons[i].IsHovering() )
		{
			return( i );
		}
	}
	return( -1 );
}

int LayerManager::GetActualSelectedLayer() const
{
	return( selectedLayer );
}

