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
	const Surface zoomerImg = { { "Icons/Zoomer.bmp" },Vei2{ 3,3 } };
	const Surface bucketImg = { { "Icons/Bucket.bmp" },Vei2{ 3,3 } };
	const Surface samplerImg = { { "Icons/Sampler.bmp" },Vei2{ 3,3 } };
	const Surface resizerImg = { { "Icons/Resizer.bmp" },Vei2{ 3,3 } };
	const Surface rulerImg = { { "Icons/Ruler.bmp" },Vei2{ 3,3 } };
	const Surface selectorImg = { { "Icons/Selector.bmp" },Vei2{ 3,3 } };
	const Surface pointerImg = { { "Icons/Pointer.bmp" },Vei2{ 3,3 } };
	// Buttons and stuff can go here I guess.
	Button brush = { brushImg,Vei2{ 55 + 50 * 1,1 } };
	Button eraser = { eraserImg,Vei2{ 55 + 50 * 2,1 } };
	Button hand = { handImg,Vei2{ 55 + 50 * 3,1 } };
	Button zoomer = { zoomerImg,Vei2{ 55 + 50 * 4,1 } };
	Button bucket = { bucketImg,Vei2{ 55 + 50 * 5,1 } };
	Button sampler = { samplerImg,Vei2{ 55 + 50 * 6,1 } };
	Button resizer = { resizerImg,Vei2{ 55 + 50 * 7,1 } };
	Button ruler = { rulerImg,Vei2{ 55 + 50 * 8,1 } };
	Button selector = { selectorImg,Vei2{ 55 + 50 * 9,1 } };
	Button pointer = { pointerImg,Vei2{ 55 + 50 * 10,1 } };
};