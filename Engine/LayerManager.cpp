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

	const auto layerButtonStart = Vei2{ drawArea.left + padding.x * 3 + buttonSize.x * 2,
		drawArea.top + padding.y };
	const auto hideStart = Vei2{ drawArea.left + padding.x,
		drawArea.top + padding.y };
	const auto lockStart = Vei2{ drawArea.left + padding.x * 2 + buttonSize.x,
		drawArea.top + padding.y };
	for( int i = 0; i < 7; ++i )
	{
		layerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/LayerButton.bmp" },Vei2{ 3,3 } },
			layerButtonStart + ( padding.Y() + buttonSize.Y() ) * i } );

		hideLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/HideLayerButton.bmp" },Vei2{ 3,3 } },
			hideStart + ( padding.Y() + buttonSize.Y() ) * i } );
		unhideLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/UnhideLayerButton.bmp" },Vei2{ 3,3 } },
			hideStart + ( padding.Y() + buttonSize.Y() ) * i } );

		hiddenLayers.emplace_back( false );

		lockLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/LockLayerButton.bmp" },Vei2{ 3,3 } },
			lockStart + ( padding.Y() + buttonSize.Y() ) * i } );
		unlockLayerButtons.emplace_back( Button{ Surface{ Surface{
			"Icons/UnlockLayerButton.bmp" },Vei2{ 3,3 } },
			lockStart + ( padding.Y() + buttonSize.Y() ) * i } );

		lockLayers.emplace_back( false );
	}
}

bool LayerManager::Update( const Keyboard& kbd,const Mouse& mouse,Surface& art )
{
	layers[selectedLayer].CopyInto( art );

	if( addLayer.Update( mouse ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'N' ) ) )
	{
		if( layers.size() < 7 && canCreateLayer )
		{
			// layers.emplace_back( Surface{ canvSize.x,canvSize.y } );
			layers.insert( layers.begin(),Surface{ canvSize.x,canvSize.y } );
			layers.front().DrawRect( 0,0,canvSize.x,canvSize.y,Colors::Magenta );
			selectedLayer = 0;
			art.CopyInto( layers[selectedLayer] );
		}
		canCreateLayer = false;
	}
	else canCreateLayer = true;

	if( dupeLayer.Update( mouse ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
		kbd.KeyIsPressed( 'J' ) ) )
	{
		if( layers.size() < 7 && canDupeLayer )
		{
			layers.insert( layers.begin() + selectedLayer,
				Surface{ layers[selectedLayer] } );
			art.CopyInto( layers[selectedLayer] );
		}
		canDupeLayer = false;
	}
	else canDupeLayer = true;

	if( removeLayer.Update( mouse ) ||
		kbd.KeyIsPressed( VK_DELETE ) )
	{
		if( layers.size() > 1 && canDeleteLayer )
		{
			// layers.pop_back();
			layers.erase( layers.begin() + selectedLayer );
			--selectedLayer;
			if( selectedLayer < 0 ) selectedLayer = 0;
			if( selectedLayer > int( layers.size() ) ) selectedLayer = int( layers.size() );
			art.CopyInto( layers[selectedLayer] );
		}
		canDeleteLayer = false;
	}
	else canDeleteLayer = true;

	if( mergeLayer.Update( mouse ) ||
		( kbd.KeyIsPressed( VK_CONTROL ) &&
			kbd.KeyIsPressed( 'E' ) ) )
	{
		if( selectedLayer < int( layers.size() ) - 1 && canMergeLayer )
		{
			layers[selectedLayer].LightCopyInto( layers[selectedLayer + 1] );

			layers.erase( layers.begin() + selectedLayer + 1 );
			art.CopyInto( layers[selectedLayer] );
		}
		canMergeLayer = false;
	}
	else canMergeLayer = true;

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

		if( !lockLayers[i] )
		{
			if( lockLayerButtons[i].Update( mouse ) )
			{
				lockLayers[i] = true;
				return( true );
			}
		}
		else
		{
			if( unlockLayerButtons[i].Update( mouse ) )
			{
				lockLayers[i] = false;
				return( true );
			}
		}
	}
	return( false );
}

void LayerManager::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( drawArea.left,drawArea.top,
		drawArea.GetWidth(),drawArea.GetHeight(),
		Colors::DarkGray );
	
	for( int i = 0; i < int( layers.size() ); ++i )
	{
		const int layerHeight = buttonSize.y + padding.y;
		if( lockLayers[i] )
		{
			gfx.DrawRect( drawArea.left,drawArea.top +
				( layerHeight * i ) + 3,drawArea.GetWidth(),
				layerHeight + padding.y - 6,Colors::Gray );
		}
		else if( i == selectedLayer )
		{
			gfx.DrawRect( drawArea.left,drawArea.top +
				( layerHeight * i ) + 3,drawArea.GetWidth(),
				layerHeight + padding.y - 6,Colors::White );
		}

		layerButtons[i].Draw( gfx );

		if( !hiddenLayers[i] ) hideLayerButtons[i].Draw( gfx );
		else unhideLayerButtons[i].Draw( gfx );

		if( !lockLayers[i] ) lockLayerButtons[i].Draw( gfx );
		else unlockLayerButtons[i].Draw( gfx );

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
			.GetInterpolatedTo( int( width ),int( height ) ),
			SpriteEffect::Copy{} );
	}

	addLayer.Draw( gfx );
	dupeLayer.Draw( gfx );
	removeLayer.Draw( gfx );
	mergeLayer.Draw( gfx );
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
		// const auto oldSelectedLayer = selectedLayer;
		// layers.emplace_back( Surface{ canvSize.x,canvSize.y } );
		// layers.back().DrawRect( 0,0,canvSize.x,canvSize.y,Colors::Magenta );
		// art.CopyInto( layers.back() );

		// selectedLayer = 0;

		// std::swap( layers[selectedLayer],layers[oldSelectedLayer] );
		// selectedLayer = oldSelectedLayer;

		layers.insert( layers.begin() + selectedLayer,
			Surface{ canvSize.x,canvSize.y, } );
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

const std::vector<bool>& LayerManager::GetLockedLayers() const
{
	return( lockLayers );
}

bool LayerManager::IsSelectedLayerLocked() const
{
	return( lockLayers[selectedLayer] );
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

