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
	ImageHandler( const RectI& clipArea,ToolMode& curTool,
		Mouse& mouse );

	// TODO: Heckin make this whole method cleaner.
	void Update( const Keyboard& kbd,ToolMode tool,
		Color& main,Color& off );
	void Draw( Graphics& gfx ) const;

	void CenterImage();
	void DrawCursor( Graphics& gfx ) const;
private:
	// Recursive, call to fill until hitting "walls".
	void TryFillPlusAt( const Vei2& pos,Color c,Color baseFill );
	void ResizeCanvas( const Vei2& newSize );

private:
	Mouse& mouse;
	Vei2 canvSize = { 8,8 };
	Surface art;
	const RectI& clipArea;
	Vec2 artPos;
	Vec2 scale = { 10.0f,10.0f };
	static constexpr Color chroma = Colors::Magenta;

	Vei2 mousePos = { 0,0 };
	Vei2 oldMousePos = { 0,0 };
	bool clickingLastFrame = false;
	ToolMode& curTool;
	// static constexpr int bgGrainAmount = 10;
	Surface bgPattern;

	Surface drawSurf;
	Surface bigPattern;

	Vei2 cropStart = { 0,0 };
	bool canCrop = false;
	Vei2 cropEnd = { 0,0 };
	bool appliedCrop = true;

	Vei2 lastClickPos = { 0,0 };

	std::vector<int> xGuidelines;
	std::vector<int> yGuidelines;
	int tempGuideline = -9999;
	bool guidelineX = false;
	bool draggingRuler = false;

	const Surface miniHand = { Surface{ "Icons/MiniHand.bmp" },Vei2{ 3,3 } };
	const Surface miniZoomer = { Surface{ "Icons/MiniZoomer.bmp" },Vei2{ 3,3 } };
	const Surface miniBucket = { Surface{ "Icons/MiniBucket.bmp" },Vei2{ 3,3 } };
	const Surface miniSampler = { Surface{ "Icons/MiniSampler.bmp" },Vei2{ 3,3 } };
	const Surface miniResizer = { Surface{ "Icons/MiniResizer.bmp" },Vei2{ 3,3 } };
	const Surface miniRuler = { Surface{ "Icons/MiniRuler.bmp" },Vei2{ 3,3 } };
	const Surface miniPointer = { Surface{ "Icons/MiniPointer.bmp" },Vei2{ 3,3 } };

	const Font luckyPixel = "Fonts/LuckyPixel24x36.bmp";
};