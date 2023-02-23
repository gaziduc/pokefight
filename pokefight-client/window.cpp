#include "window.h"
#include "texture.h"
#include "pictures.h"
#include "settings.h"
#include "input.h"
#include "menu.h"
#include "utils.h"
#include "toaster.h"
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

	_sfx.emplace(Sound::HIT, Mix_LoadWAV("data/hit.wav"));

	// Settings
	_settings.load_settings();

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	if (_window == nullptr)
		throw std::runtime_error("Error while creating window:\n" + std::string(SDL_GetError()));

	_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (_renderer == nullptr)
		throw std::runtime_error("Error while creating renderer:\n" + std::string(SDL_GetError()));

	if (_settings.get_is_fullscreen()) {
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		SDL_SetWindowSize(_window, displayMode.w, displayMode.h);
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
	}

	// Enable blending (alpha)
	SDL_SetRenderDrawBlendMode(get_renderer(), SDL_BLENDMODE_BLEND);

	// Load common textures
	_pics.add_texture(*this, Picture::MENU_SELECTED, "data/selected.png");
	_pics.add_texture(*this, Picture::POKEBALL_ICON, "data/pokeball.png");
	_pics.add_texture(*this, Picture::MENU_BACKGROUND, "data/menu.png");
	_pics.add_texture(*this, Picture::FIGHT_BACKGROUND, "data/fight.png");
	_pics.add_texture(*this, Picture::TITLE, "data/title.png");
	_pics.add_texture(*this, Picture::NORMAL_PIC, "data/types/normal.png");
	_pics.add_texture(*this, Picture::FIRE_PIC, "data/types/fire.png");
	_pics.add_texture(*this, Picture::WATER_PIC, "data/types/water.png");
	_pics.add_texture(*this, Picture::ELECTRIC_PIC, "data/types/electric.png");
	_pics.add_texture(*this, Picture::GRASS_PIC,"data/types/grass.png");
	_pics.add_texture(*this, Picture::ICE_PIC,"data/types/ice.png");
	_pics.add_texture(*this, Picture::FIGHTING_PIC, "data/types/fighting.png");
	_pics.add_texture(*this, Picture::POISON_PIC,"data/types/poison.png");
	_pics.add_texture(*this, Picture::GROUND_PIC,"data/types/ground.png");
	_pics.add_texture(*this, Picture::FLYING_PIC, "data/types/flying.png");
	_pics.add_texture(*this, Picture::PSYCHIC_PIC,"data/types/psychic.png");
	_pics.add_texture(*this, Picture::BUG_PIC,"data/types/bug.png");
	_pics.add_texture(*this, Picture::ROCK_PIC,"data/types/rock.png");
	_pics.add_texture(*this, Picture::GHOST_PIC,"data/types/ghost.png");
	_pics.add_texture(*this, Picture::DRAGON_PIC,"data/types/dragon.png");
	_pics.add_texture(*this, Picture::DARK_PIC,"data/types/dark.png");
	_pics.add_texture(*this, Picture::STEEL_PIC,"data/types/steel.png");
	_pics.add_texture(*this, Picture::FAIRY_PIC, "data/types/fairy.png");

	// Fonts
	_fonts.emplace(FontSize::VERY_SMALL, TTF_OpenFont("data/segoeui.ttf", 18));
	_fonts.emplace(FontSize::SMALL, TTF_OpenFont("data/segoeui.ttf", 24));
	_fonts.emplace(FontSize::NORMAL, TTF_OpenFont("data/segoeui.ttf", 34));

	// Framerate
	SDL_initFramerate(&_fps_manager);
	SDL_setFramerate(&_fps_manager, 60);

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	load_and_play_music("data/route-203.ogg");
}

Window::~Window() {
	if (_renderer != nullptr)
		SDL_DestroyRenderer(_renderer);
	if (_window != nullptr)
		SDL_DestroyWindow(_window);

	for (auto& font_key_value : _fonts) {
		TTF_CloseFont(font_key_value.second);
	}

	for (auto& sfx : _sfx) {
		Mix_FreeChunk(sfx.second);
	}

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

void Window::play_sound(const Sound sound_num) const {
	Mix_PlayChannel(-1, _sfx.at(sound_num), 0);
}

void Window::render_clear() const {
	SDL_SetRenderDrawColor(_renderer, 135, 206, 235, 255);
	SDL_RenderClear(_renderer);
}

void Window::render_present() {
	show_toasters();

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
	_events.update_events(*this);
}

Events& Window::get_events() {
	return _events;
}

void Window::settings_menu(const Pokemons& pokemons) {
	Menu menu(std::nullopt, get_font(FontSize::NORMAL), 600);
	menu.add_choice(SettingsMenu::NICKNAME, "Change nickname");
	menu.add_choice(SettingsMenu::POKEMON, "Change pokemon");
	menu.add_choice(SettingsMenu::TOOGLE_FULLSCREEN, "Toggle Fullscreen");
	menu.add_choice(SettingsMenu::BACK_TO_MAIN_MENU, "Back to main menu...");


	while (true) {
		update_events();

		if (is_quit())
			exit(0);

		if (is_escape_down()) {
			press_up_key(SDL_SCANCODE_ESCAPE);
			return;
		}

		if (is_enter_down()) {
			press_up_key(SDL_SCANCODE_RETURN);
			press_up_key(SDL_SCANCODE_KP_ENTER);

			switch (menu.get_selected_choice_num())
			{
			case SettingsMenu::NICKNAME:
				if (ask_nickname_if_necessary(true))
					_settings.save_settings();
				break;
			case SettingsMenu::POKEMON:
				if (ask_pokemon_if_necessary(true, pokemons))
					_settings.save_settings();
				break;
			case SettingsMenu::TOOGLE_FULLSCREEN:
				toggle_full_screen();
				break;
			case SettingsMenu::BACK_TO_MAIN_MENU:
				return;
			default:
				break;
			}
		}

		menu.update_selected_choice_from_events(get_events());

		render_clear();
		get_texture(Picture::MENU_BACKGROUND)->render_without_pos_dst(*this);
		get_texture(Picture::TITLE)->set_pos_dst(get_width() / 2 - get_texture(Picture::TITLE)->get_width() / 2, 80);
		get_texture(Picture::TITLE)->render(*this);
		std::shared_ptr<Anim> chosed_anim_ptr = pokemons.get_pokemon_anim_ptr(_settings.get_pokemon_num());
		chosed_anim_ptr->set_pos_dst(get_width() / 2 - chosed_anim_ptr->get_width() / 2, get_height() / 2 - chosed_anim_ptr->get_height() / 2 - 50);
		chosed_anim_ptr->render_anim(*this, true);
		menu.render_menu(*this);
		render_present();
	}
}


bool Window::ask_nickname_if_necessary(const bool forceAsk) {
	if (forceAsk || _settings.get_nickname().empty()) {
		do {
			Input input_nickname("Enter your nickname:", get_font(FontSize::NORMAL), _settings.get_nickname(), true);
			input_nickname.show_input_menu(*this);
			_settings.set_nickname(input_nickname.get_current_input());
		} while (_settings.get_nickname().empty());

		return true;
	}

	return false;
}

bool Window::ask_pokemon_if_necessary(const bool forceAsk, const Pokemons& pokemon_list) {
	if (_settings.get_pokemon_num() == -1 || forceAsk) {
		Menu choose_pkmn_menu("Choose your Pokemon:", get_font(FontSize::NORMAL), 600);
		for (int choice_num = 0; choice_num < pokemon_list.get_num_pokemons(); choice_num++) {
			choose_pkmn_menu.add_choice(choice_num, pokemon_list.get_pokemon_name(choice_num));
			std::shared_ptr<Anim> anim_ptr = pokemon_list.get_pokemon_anim_ptr(choice_num);
			
		}

		SDL_Color unselected_text_color = { 0, 0, 0, 255 };

		while (true) {
			update_events();

			if (is_quit())
				exit(0);

			if (is_escape_down()) {
				press_up_key(SDL_SCANCODE_ESCAPE);

				if (forceAsk)
					return false;
			}

			choose_pkmn_menu.update_selected_choice_from_events(get_events());

			if (is_enter_down()) {
				press_up_key(SDL_SCANCODE_RETURN);
				press_up_key(SDL_SCANCODE_KP_ENTER);

				_settings.set_pokemon_num(choose_pkmn_menu.get_selected_choice_num());
				break;
			}

			render_clear();
			get_texture(Picture::MENU_BACKGROUND)->render_without_pos_dst(*this);
			get_texture(Picture::TITLE)->set_pos_dst(get_width() / 2 - get_texture(Picture::TITLE)->get_width() / 2, 80);
			get_texture(Picture::TITLE)->render(*this);

			Uint64 alpha = SDL_GetTicks64() % 512;
			if (alpha > 255)
				alpha = -alpha + 511;
			if (alpha == 0)
				alpha = 1;

			SDL_Color selected_text_color = { 0, 0, 0, alpha };

			for (int choice_num = 0; choice_num < pokemon_list.get_num_pokemons(); choice_num++) {
				auto anim = pokemon_list.get_pokemon_anim_ptr(choice_num);
				anim->set_pos_dst(get_width() / 2 + ((choice_num - 2) * 200) - anim->get_width() / 2, get_height() / 2 - anim->get_height() / 2 - 200);
				pokemon_list.get_pokemon_anim_ptr(choice_num)->render_anim(*this, true);

				SDL_Rect pos_dst;
				
				SDL_Texture* texture = get_text_texture(*this, get_font(FontSize::NORMAL), pokemon_list.get_pokemon_name(choice_num), choice_num == choose_pkmn_menu.get_selected_choice_num() ? selected_text_color : unselected_text_color);
				SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
				pos_dst.x = get_width() / 2 + ((choice_num - 2) * 200) - pos_dst.w / 2;
				pos_dst.y = get_height() / 2 - 125;
				SDL_RenderCopy(get_renderer(), texture, nullptr, &pos_dst);
				SDL_DestroyTexture(texture);

				Type first_type = pokemon_list.get_pokemon_first_type(choice_num);
				Texture* type_texture = get_texture((Picture) first_type);
				type_texture->set_pos_dst(get_width() / 2 + ((choice_num - 2) * 200) - type_texture->get_width() / 2, get_height() / 2 - 50);
				type_texture->render(*this);

				if (pokemon_list.get_pokemon_second_type(choice_num).has_value()) {
					Type second_type = pokemon_list.get_pokemon_second_type(choice_num).value();
					Texture* second_type_texture = get_texture((Picture) second_type);
					second_type_texture->set_pos_dst(get_width() / 2 + ((choice_num - 2) * 200) - type_texture->get_width() / 2, get_height() / 2 - 10);
					second_type_texture->render(*this);
				}
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


void Window::add_toaster(const std::string& toaster_message) {
	_toasters.emplace_back(toaster_message, 5000);
}

std::string Window::get_toaster_message(const int toaster_index) const {
	return _toasters[toaster_index].get_message();
}

int Window::get_toaster_time_left(const int toaster_index) const {
	return _toasters[toaster_index].get_time_left();
}

void Window::decrease_toaster_time_left(const int toaster_index) {
	_toasters[toaster_index].decrease_time_left(); // TODO: be more precise
}

void Window::show_toasters() {
	SDL_Rect pos_dst = { .x = 0, .y = 80, .w = 0, .h = 0 };

	for (auto toaster_iter = _toasters.begin(); toaster_iter != _toasters.end();) {
		if (toaster_iter->get_time_left() >= 0) {
			int alpha = 255;
			if (toaster_iter->get_time_left() > 5000 - 255) {
				alpha = (5000 - toaster_iter->get_time_left());
			} else if (toaster_iter->get_time_left() < 255) {
				alpha = toaster_iter->get_time_left();
			}

			SDL_Color text_color = { 0, 0, 0, alpha };

			SDL_Texture* texture = get_text_texture(*this, get_font(FontSize::SMALL), toaster_iter->get_message(), text_color, true);
			SDL_QueryTexture(texture, nullptr, nullptr, &pos_dst.w, &pos_dst.h);
			pos_dst.x = get_width() - 80 - pos_dst.w - 10;
			pos_dst.w += 20;
			pos_dst.h += 20;

			SDL_SetRenderDrawColor(get_renderer(), 255, 255, 255, alpha);
			SDL_RenderFillRect(get_renderer(), &pos_dst);

			SDL_SetRenderDrawColor(get_renderer(), 0, 0, 0, alpha);
			SDL_RenderDrawRect(get_renderer(), &pos_dst);

			pos_dst.x += 10;
			pos_dst.w -= 20;
			pos_dst.y += 10;
			pos_dst.h -= 20;

			SDL_RenderCopy(get_renderer(), texture, nullptr, &pos_dst);
			SDL_DestroyTexture(texture);

			pos_dst.y += 90;
			toaster_iter->decrease_time_left();
			toaster_iter++;
		}
		else {
			toaster_iter = _toasters.erase(toaster_iter);
		}
	}
}

void Window::load_and_play_music(const std::string& music_to_load_path) {
	if (_music != nullptr) {
		Mix_FreeMusic(_music);
		_music = nullptr;
	}

	_music = Mix_LoadMUS(music_to_load_path.c_str());
	if (_music == nullptr) {
		throw std::runtime_error("Error while loading music:\n" + std::string(Mix_GetError()));
	}

	Mix_PlayMusic(_music, -1);
}

void Window::toggle_full_screen() {
	if (_settings.get_is_fullscreen()) {
		SDL_SetWindowFullscreen(_window, 0);
		_settings.set_is_fullscreen(false);
	} else {
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		SDL_SetWindowSize(_window, displayMode.w, displayMode.h);
		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
		_settings.set_is_fullscreen(true);
	}

	_settings.save_settings();
}
