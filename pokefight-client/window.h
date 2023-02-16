
#pragma once

#include "pictures.h"
#include "events.h"
#include "settings.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_framerate.h>
#include <string>
#include <memory>
#include <map>

enum FontSize {
	SMALL = 0,
	NORMAL = 1
};

class Window {
private:
	int _initial_width;
	int _initial_height;
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	const std::string _title = "Pokefight";
	Pictures _pics;
	FPSmanager _fps_manager;
	Events _events;
	std::map<FontSize, TTF_Font*> _fonts;
	Settings _settings;

public:
	Window(const int initial_width, const int initial_height);
	~Window();

	void init();

	SDL_Renderer* get_renderer() const;
	int get_width() const;
	int get_height() const;
	Texture* get_texture(const enum Picture pic_num) const;
	TTF_Font* get_font(const FontSize font_size) const;
	void render_clear() const;
	void render_present();
	void close_window();

	// Events
	bool is_quit() const;
	bool is_key_down(const int key);
	bool is_enter_down() const;
	bool is_escape_down() const;
	std::string get_text() const;
	void press_up_key(const int key);
	void update_events();
	Events& get_events();


	void settings_menu(const Pokemons& pokemons, Texture& title_texture);
	Settings& get_settings();
	bool ask_nickname_if_necessary(const bool forceAsk);
	bool ask_pokemon_if_necessary(const bool forceAsk, const Pokemons& pokemon_list);
};