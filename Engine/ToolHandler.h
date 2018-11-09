#pragma once

#include "Mouse.h"
#include "Keyboard.h"
#include "ToolMode.h"
#include "Graphics.h"
#include "Button.h"

class ToolHandler
{
public:
	ToolHandler( ToolMode& tool );

	void Update( const Mouse& mouse,
		const Keyboard& kbd,Color& main,Color& off );

	void Draw( Graphics& gfx ) const;
private:
	const Surface& GetToolSurf( ToolMode tool ) const;
private:
	ToolMode& tool;
	bool pressingSwap = false;
	// Tool images.
	const Surface brushImg = { { "Icons/Brush.bmp" },Vei2{ 3,3 } };
	const Surface eraserImg = { { "Icons/Eraser.bmp" },Vei2{ 3,3 } };
	const Surface handImg = { { "Icons/Hand.bmp" },Vei2{ 3,3 } };
	// Buttons and stuff can go here I guess.
	Button brush = { brushImg,Vei2{ 50 + 50 * 1,1 } };
	Button eraser = { eraserImg,Vei2{ 50 + 50 * 2,1 } };
	Button hand = { handImg,Vei2{ 50 + 50 * 3,1 } };
};