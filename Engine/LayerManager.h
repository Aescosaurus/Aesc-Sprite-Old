#pragma once

#include "Surface.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Button.h"

class LayerManager
{
public:
	LayerManager( const RectI& clipArea,const Vei2& canvSize );

	bool Update( const Keyboard& kbd,const Mouse& mouse,Surface& art );
	void Draw( Graphics& gfx ) const;

	void ResizeCanvas( const Vei2& newSize );
	void CreateNewLayer( Surface& art );

	const std::vector<Surface>& GetLayers() const;
	const std::vector<bool>& GetHiddenLayers() const;
	const std::vector<bool>& GetLockedLayers() const;
	bool IsSelectedLayerLocked() const;
	// Returns layer you're hovering.
	int GetSelectedLayer() const;
	int GetActualSelectedLayer() const;
private:
	Vei2 canvSize;
	static constexpr Vei2 padding = { 5,5 };
	static constexpr Vei2 buttonSize = { 8 * 3,8 * 3 };

	std::vector<Surface> layers;
	std::vector<Button> layerButtons;
	std::vector<Button> hideLayerButtons;
	std::vector<Button> unhideLayerButtons;
	std::vector<bool> hiddenLayers; // true = hidden.
	std::vector<Button> lockLayerButtons;
	std::vector<Button> unlockLayerButtons;
	std::vector<bool> lockLayers; // true = locked.
	int selectedLayer = 0;

	const RectI drawArea;

	Button addLayer = Button{ Surface{ Surface{ "Icons/AddLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x,
		drawArea.bottom - padding.y - buttonSize.y } };
	Button dupeLayer = Button{ Surface{ Surface{ "Icons/DupeLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x * 2 + buttonSize.x,
		drawArea.bottom - padding.y - buttonSize.y } };
	Button removeLayer = Button{ Surface{ Surface{ "Icons/RemoveLayerButton.bmp" },Vei2{ 3,3 } },
		Vei2{ drawArea.left + padding.x * 3 + buttonSize.x * 2,
		drawArea.bottom - padding.y - buttonSize.y } };

	const Surface layerSelectedButton = Surface{ Surface{ "Icons/LayerSelectButton.bmp" },Vei2{ 3,3 } };

	bool canCreateLayer = false;
	bool canDupeLayer = false;
	bool canDeleteLayer = false;
};