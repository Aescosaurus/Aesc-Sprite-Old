#pragma once

#include "Surface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class LayerManager
{
public:
	LayerManager( const RectI& clipArea,Surface& artRef );

	// void Update( const Keyboard& kbd,const Mouse& mouse );
	void Draw( Graphics& gfx ) const;

	// void ResizeCanvas( const Vei2& newSize );
private:
	Surface& artRef;

	const RectI drawArea;
	static constexpr Vei2 padding = { 5,5 };
};