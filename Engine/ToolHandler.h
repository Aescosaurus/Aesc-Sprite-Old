#pragma once

#include "Mouse.h"
#include "Keyboard.h"
#include "ToolMode.h"
#include "Graphics.h"

class ToolHandler
{
public:
	void Update( const Mouse& mouse,
		const Keyboard& kbd,ToolMode& tool,
		Color& main,Color& off );

	void Draw( Graphics& gfx ) const;
private:
	bool pressingSwap = false;
	// Buttons and stuff can go here I guess.
};