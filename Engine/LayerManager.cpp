#include "LayerManager.h"

LayerManager::LayerManager( const RectI& clipArea,Surface& artRef )
	:
	artRef( artRef ),
	drawArea( clipArea.right + padding.x,
		Graphics::ScreenWidth - padding.x,
		clipArea.bottom - 250,clipArea.bottom )
{}

void LayerManager::Draw( Graphics& gfx ) const
{
	// TODO: Remove this ----------------------------------
	gfx.DrawRect( drawArea.left,drawArea.top,
		drawArea.GetWidth(),drawArea.GetHeight(),
		Colors::Cyan );
	// ----------------------------------------------------
}
