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

	// TODO: Heckin make this whole method cleaner.
	void Update( Mouse& mouse,
		const Keyboard& kbd,ToolMode tool,
		Color main,Color off );
	void Draw( Graphics& gfx ) const;

	void CenterImage();
private:
	// Recursive, call to fill until hitting "walls".
	void TryFillPlusAt( const Vei2& pos,Color c,Color baseFill );
private:
	static constexpr Vei2 canvSize = { 8,8 };
	Surface art;
	Surface artProxy;
	const RectI& clipArea;
	Vec2 artPos;
	Vec2 scale = { 10.0f,10.0f };
	static constexpr Color chroma = Colors::Magenta;

	Vei2 oldMousePos = { 0,0 };
	static constexpr int bgGrainAmount = 10;
	Surface bgPattern;
};