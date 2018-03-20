#include "Canvas.h"

Canvas::Canvas()
	:
	pal( 32 )
{
}

void Canvas::Draw( Graphics& gfx ) const
{
	gfx.DrawRectDim( screenArea.left,screenArea.top,
		screenArea.right,screenArea.bottom,
		Colors::Cyan );

	pal.Draw( gfx );
}
