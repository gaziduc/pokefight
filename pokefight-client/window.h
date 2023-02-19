
#pragma once

#include "pictures.h"
#include "events.h"
#include "settings.h"
#include "toaster.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <memory>
#include <map>
#include <vector>

enum FontSize {
	VERY_SMALL = 0,
	SMALL = 1,
	NORMAL = 2
};

enum Sound {
	HIT = 0,
};

// Forward declaration
class Toaster;

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
	std::map<Sound, Mix_Chunk*> _sfx;
	Settings _settings;
	std::vector<Toaster> _toasters;

public:
	Window(const int initial_width, const int initial_height);
	~Window();

	void init();

	SDL_Renderer* get_renderer() const;
	int get_width() const;
	int get_height() const;
	Texture* get_texture(const enum Picture pic_num) const;
	TTF_Font* get_font(const FontSize font_size) const;
    void play_sound(const Sound sound_num) const;
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

	// Settings
	void settings_menu(const Pokemons& pokemons, std::shared_ptr<Texture> title_texture, std::shared_ptr<Texture> menu_texture);
	Settings& get_settings();
	bool ask_nickname_if_necessary(const bool forceAsk, std::shared_ptr<Texture> title_texture, std::shared_ptr<Texture> menu_texture);
	bool ask_pokemon_if_necessary(const bool forceAsk, const Pokemons& pokemon_list, std::shared_ptr<Texture> title_texture, std::shared_ptr<Texture> menu_texture);

	// Toasters
	void add_toaster(const std::string& toaster_message);
	std::string get_toaster_message(const int toaster_index) const;
	int get_toaster_time_left(const int toaster_index) const;
	void decrease_toaster_time_left(const int toaster_index);
	void show_toasters();
};