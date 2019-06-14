#pragma once

#include "Rect.h"
#include "Graphics.h"
#include "Palette.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Surface.h"
#include "ImageHandler.h"
#include "ToolMode.h"
#include "ToolHandler.h"
#include "FileMenu.h"
#include "MainWindow.h"

class Canvas
{
public:
	Canvas( Mouse& mouse,MainWindow& wnd );

	void Update( const Keyboard& kbd );
	void Draw( Graphics& gfx ) const;
private:
	Mouse& mouse;
	const RectI screenArea = { 70,Graphics
		::ScreenWidth - 140,50,Graphics
		::ScreenHeight - 4 };
	static constexpr int width = 16;
	static constexpr int height = 16;
	Palette pal;
	Color& mainCol = pal.GetMainCol();
	Color& offCol = pal.GetOffCol();
	ImageHandler imgHand;
	ToolMode curTool = ToolMode::Brush;
	ToolHandler toolHand;
	FileMenu fMenu;
};