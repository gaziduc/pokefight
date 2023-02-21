#pragma once

#include "texture.h"
#include <map>
#include <string>
#include <memory>

enum Picture {
	MENU_SELECTED = 0,
	POKEBALL_ICON = 1,
	MENU_BACKGROUND = 2,
	FIGHT_BACKGROUND = 3,
	TITLE = 4
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