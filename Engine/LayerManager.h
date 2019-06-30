#pragma once

#include "Surface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Button.h"

class LayerManager
{
public:
	LayerManager( const RectI& clipArea,Surface& artRef );

	void Update( const Keyboard& kbd,const Mouse& mouse );
	void Draw( Graphics& gfx ) const;

	// void ResizeCanvas( const Vei2& newSize );
private:
	Surface& artRef;

	const RectI drawArea;
	static constexpr Vei2 padding = { 5,5 };
	static constexpr Vei2 buttonSize = { 8 * 3,8 * 3 };

	Button addLayer = Button{ Surface{ Surface{ "Icons/AddLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x,
		drawArea.bottom - padding.y - buttonSize.y } };
	Button dupeLayer = Button{ Surface{ Surface{ "Icons/DupeLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x * 2 + buttonSize.x,
		drawArea.bottom - padding.y - buttonSize.y } };
	Button removeLayer = Button{ Surface{ Surface{ "Icons/RemoveLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x * 3 + buttonSize.x * 2,
		drawArea.bottom - padding.y - buttonSize.y } };
};