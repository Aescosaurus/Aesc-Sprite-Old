#pragma once

#include "Surface.h"
#include "Rect.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ToolMode.h"
#include "Font.h"

class ImageHandler
{
public:
	ImageHandler( const RectI& clipArea,ToolMode& curTool );

	// TODO: Heckin make this whole method cleaner.
	void Update( Mouse& mouse,
		const Keyboard& kbd,ToolMode tool,
		Color& main,Color& off );
	void Draw( Graphics& gfx ) const;

	void CenterImage();
private:
	// Recursive, call to fill until hitting "walls".
	void TryFillPlusAt( const Vei2& pos,Color c,Color baseFill );
	void ResizeCanvas( const Vei2& newSize );

	void DrawCursor( Graphics& gfx ) const;
private:
	Vei2 canvSize = { 8,8 };
	Surface art;
	const RectI& clipArea;
	Vec2 artPos;
	Vec2 scale = { 10.0f,10.0f };
	static constexpr Color chroma = Colors::Magenta;

	Vei2 mousePos = { 0,0 };
	Vei2 oldMousePos = { 0,0 };
	ToolMode& curTool;
	static constexpr int bgGrainAmount = 10;
	Surface bgPattern;

	Vei2 cropStart = { 0,0 };
	bool canCrop = false;
	Vei2 cropEnd = { 0,0 };
	bool appliedCrop = true;

	const Surface miniHand = { Surface{ "Icons/MiniHand.bmp" },Vei2{ 3,3 } };
	const Surface miniZoomer = { Surface{ "Icons/MiniZoomer.bmp" },Vei2{ 3,3 } };
	const Surface miniBucket = { Surface{ "Icons/MiniBucket.bmp" },Vei2{ 3,3 } };
	const Surface miniSampler = { Surface{ "Icons/MiniSampler.bmp" },Vei2{ 3,3 } };
	const Surface miniResizer = { Surface{ "Icons/MiniResizer.bmp" },Vei2{ 3,3 } };

	const Font luckyPixel = "Fonts/LuckyPixel24x36.bmp";
};