#include "window.h"
#include "texture.h"
#include "pictures.h"
#include "settings.h"
#include "input.h"
#include "menu.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <stdexcept>
#include <memory>


Window::Window(const int initial_width, const int initial_height) {
	_initial_width = initial_width;
	_initial_height = initial_height;
}

void Window::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error("Error while initializing SDL2:\n" + std::string(SDL_GetError()));

	int img_flags = IMG_INIT_PNG;
	int img_initted = IMG_Init(img_flags);
	if ((img_initted & img_flags) != img_flags)
		throw std::runtime_error("Error while initializing SDL2_image:\n" + std::string(IMG_GetError()));

	if (TTF_Init() == -1)
		throw std::runtime_error("Error while initializing SDL2_ttf:\n" + std::string(TTF_GetError()));

	if (SDLNet_Init() == -1)
		throw std::runtime_error("Error while initializing SDL2_net:\n" + std::string(SDLNet_GetError()));

	int mix_flags = MIX_INIT_OGG;
	int mix_initted = Mix_Init(mix_flags);
	if ((mix_initted & mix_flags) != mix_flags)
		throw std::runtime_error("Error while initializing SDL2_mixer:\n" + std::string(Mix_GetError()));

	Mix_AllocateChannels(8);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		throw std::runtime_error("Error while initializing SDL2_mixer:\n" + std::string(Mix_GetError()));

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _initial_width, _initial_height, SDL_WINDOW_RESIZABLE);
	if (_window == nullptr)
		throw std::runtime_error("Error while creating window:\n" + std::string(SDL_GetError()));

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (_renderer == nullptr)
		throw std::runtime_error("Error while creating renderer:\n" + std::string(SDL_GetError()));

	// Enable blending (alpha)
	SDL_SetRenderDrawBlendMode(get_renderer(), SDL_BLENDMODE_BLEND);

	// Load common textures
	_pics.add_texture(*this, Picture::MENU_SELECTED, "data/selected.png");
	_pics.add_texture(*this, Picture::POKEBALL_ICON, "data/pokeball.png");

	// Fonts
	_fonts.emplace(FontSize::SMALL, TTF_OpenFont("data/segoeui.ttf", 25));
	_fonts.emplace(FontSize::NORMAL, TTF_OpenFont("data/segoeui.ttf", 35));

	// Settings
	_settings.load_settings();

	// Framerate
	SDL_initFramerate(&_fps_manager);
	SDL_setFramerate(&_fps_manager, 60);

	Mix_Music* music = Mix_LoadMUS("data/route-203.ogg");
	if (music == nullptr) {
		throw std::runtime_error("Error while loading music:\n" + std::string(Mix_GetError()));
	}

	Mix_PlayMusic(music, -1);
}

Window::~Window() {
	for (auto& font_key_value : _fonts) {
		TTF_CloseFont(font_key_value.second);
	}

	if (_renderer != nullptr)
		SDL_DestroyRenderer(_renderer);
	if (_window != nullptr)
		SDL_DestroyWindow(_window);

	Mix_CloseAudio();
	SDLNet_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
SDL_Renderer* Window::get_renderer() const {
	return _renderer;
}

int Window::get_width() const {
	int width = 0;
	SDL_GetWindowSize(_window, &width, nullptr);
	return width;
}

int Window::get_height() const {
	int height = 0;
	SDL_GetWindowSize(_window, nullptr, &height);
	return height;
}

Texture* Window::get_texture(const enum Picture pic_num) const {
	return _pics.get_texture(pic_num).get();
}

TTF_Font* Window::get_font(const FontSize font_size) const {
	return _fonts.at(font_size);
}

void Window::render_clear() const {
	SDL_SetRenderDrawColor(_renderer, 135, 206, 235, 255);
	SDL_RenderClear(_renderer);
}

void Window::render_present() {
	SDL_RenderPresent(_renderer);
	SDL_framerateDelay(&_fps_manager);
}

void Window::close_window() {
	if (_renderer != nullptr)
		SDL_DestroyRenderer(_renderer);
	if (_window != nullptr)
		SDL_DestroyWindow(_window);
}

bool Window::is_quit() const {
	return _events.is_quit();
}

bool Window::is_key_down(const int key) {
	return _events.is_key_down(key);
}

bool Window::is_enter_down() const {
	return _events.is_enter_down();
}

bool Window::is_escape_down() const {
	return _events.is_escape_down();
}

std::string Window::get_text() const {
	return _events.get_text();
}

void Window::press_up_key(const int key) {
	_events.press_up_key(key);
}

void Window::update_events() {
	_events.update_events();
}

Events& Window::get_events() {
	return _events;
}

void Window::settings_menu(const Pokemons& pokemons, Texture& title_texture) {
	Menu menu(std::nullopt, get_font(FontSize::NORMAL));
	menu.add_choice(SettingsMenu::NICKNAME, "Change nickname");
	menu.add_choice(SettingsMenu::POKEMON, "Change pokemon");
	menu.add_choice(SettingsMenu::BACK_TO_MAIN_MENU, "Back to main menu");


	while (true) {
		update_events();

		if (is_quit())
			exit(0);

		if (is_enter_down()) {
			press_up_key(SDL_SCANCODE_RETURN);
			press_up_key(SDL_SCANCODE_KP_ENTER);

			switch (menu.get_selected_choice())
			{
			case SettingsMenu::NICKNAME:
				if (ask_nickname_if_necessary(true))
					_settings.save_settings();
				break;
			case SettingsMenu::POKEMON:
				if (ask_pokemon_if_necessary(true, pokemons))
					_settings.save_settings();
				break;
			case SettingsMenu::BACK_TO_MAIN_MENU:
				return;
			default:
				break;
			}
		}

		menu.update_selected_choice_from_events(get_events());

		render_clear();
		title_texture.set_pos_dst(get_width() / 2 - title_texture.get_width() / 2, 80);
		title_texture.render(*this);
		std::shared_ptr<Anim> chosed_anim_ptr = pokemons.get_pokemon_anim_ptr(_settings.get_pokemon_num());
		chosed_anim_ptr->set_pos_dst(get_width() / 2 - chosed_anim_ptr->get_width() / 2, get_height() / 2 - chosed_anim_ptr->get_height() / 2 - 50);
		chosed_anim_ptr->render_anim(*this);
		menu.render_menu(*this);
		render_present();
	}
}


bool Window::ask_nickname_if_necessary(const bool forceAsk) {
	if (forceAsk || _settings.get_nickname().empty()) {
		do {
			Input input_nickname("Enter your nickname:", get_font(FontSize::NORMAL), _settings.get_nickname());
			input_nickname.show_input_menu(*this);
			_settings.set_nickname(input_nickname.get_current_input());
		} while (_settings.get_nickname().empty());

		return true;
	}

	return false;
}

bool Window::ask_pokemon_if_necessary(const bool forceAsk, const Pokemons& pokemon_list) {
	if (_settings.get_pokemon_num() == -1 || forceAsk) {
		Menu choose_pkmn_menu("Choose your Pokemon:", get_font(FontSize::NORMAL));
		for (int choice_num = 0; choice_num < pokemon_list.get_num_pokemons(); choice_num++) {
			choose_pkmn_menu.add_choice(choice_num, pokemon_list.get_pokemon_name(choice_num));
			std::shared_ptr<Anim> anim_ptr = pokemon_list.get_pokemon_anim_ptr(choice_num);
			anim_ptr->set_pos_dst(get_width() / 2 + ((choice_num - 1) * 200) - anim_ptr->get_width() / 2, get_height() / 2 - anim_ptr->get_height() / 2 - 150);
		}

		SDL_Color text_color = { 0, 0, 0, 255 };

		while (true) {
			update_events();

			if (is_quit())
				exit(0);

			choose_pkmn_menu.update_selected_choice_from_events(get_events());

			if (is_enter_down()) {
				press_up_key(SDL_SCANCODE_RETURN);
				press_up_key(SDL_SCANCODE_KP_ENTER);

				_settings.set_pokemon_num(choose_pkmn_menu.get_selected_choice());
				break;
			}

			render_clear();
			for (int choice_num = 0; choice_num < pokemon_list.get_num_pokemons(); choice_num++) {
				pokemon_list.get_pokemon_anim_ptr(choice_num)->render_anim(*this);

				SDL_Rect pos_dst;
				SDL_Texture* texture = get_text_texture(*this, get_font(FontSize::NORMAL), pokemon_list.get_pokemon_name(choice_num), text_color);
				SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
				pos_dst.x = get_width() / 2 + ((choice_num - 1) * 200) - pos_dst.w / 2;
				pos_dst.y = get_height() / 2 - 75;
				SDL_RenderCopy(get_renderer(), texture, nullptr, &pos_dst);
				SDL_DestroyTexture(texture);
			}
			choose_pkmn_menu.render_menu(*this);
			render_present();
		}

		return true;
	}

	return false;
}


Settings& Window::get_settings() {
	return _settings;
}