#pragma once

#include "Surface.h"
#include "Rect.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ToolMode.h"

class ImageHandler
{
public:
	ImageHandler( const RectI& clipArea );

	void Update( Mouse& mouse,
		const Keyboard& kbd,ToolMode tool,
		Color main,Color off );
	void Draw( Graphics& gfx ) const;
private:
	static constexpr Vei2 canvSize = { 8,8 };
	Surface art;
	Surface artProxy;
	const RectI& clipArea;
	Vec2 artPos;
	Vec2 scale = { 10.0f,10.0f };
	static constexpr Color chroma = Colors::Magenta;

	Vei2 oldMousePos = { 0,0 };
};