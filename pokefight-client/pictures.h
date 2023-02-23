#pragma once

#include "texture.h"
#include <map>
#include <string>
#include <memory>

enum Picture {
	// Types pictures
	NORMAL_PIC,
	FIRE_PIC,
	WATER_PIC,
	ELECTRIC_PIC,
	GRASS_PIC,
	ICE_PIC,
	FIGHTING_PIC,
	POISON_PIC,
	GROUND_PIC,
	FLYING_PIC,
	PSYCHIC_PIC,
	BUG_PIC,
	ROCK_PIC,
	GHOST_PIC,
	DRAGON_PIC,
	DARK_PIC,
	STEEL_PIC,
	FAIRY_PIC,

	// Other
	MENU_SELECTED,
	POKEBALL_ICON,
	MENU_BACKGROUND,
	FIGHT_BACKGROUND,
	TITLE,
};

// forward declaration
class Window;

class Pictures {
private:
	std::map<Picture, std::shared_ptr<Texture>> _pics;

public:
	Pictures();

	void add_texture(const Window& window, const enum Picture pic_num, const std::string& filename);

	std::shared_ptr<Texture> get_texture(const enum Picture pic_num) const;
};