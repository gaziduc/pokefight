#include "events.h"
#include <SDL2/SDL.h>
#include <string>


bool Events::is_quit() const {
    return _quit;
}

bool Events::is_key_down(const int key) const {
    return _keys[key];
}

void Events::press_up_key(const int key) {
    _keys[key] = false;
}

bool Events::is_enter_down() const {
    return is_key_down(SDL_SCANCODE_RETURN) || is_key_down(SDL_SCANCODE_KP_ENTER);
}

bool Events::is_escape_down() const {
    return is_key_down(SDL_SCANCODE_ESCAPE);
}

std::string Events::get_text() const {
    return _text;
}

void Events::update_events() {
    _quit = false;
    _text = "";

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        // Quit event
        case SDL_QUIT:
            _quit = true;
            break;

        // Keyboard events
        case SDL_KEYDOWN:
            _keys[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            _keys[event.key.keysym.scancode] = false;
            break;

        case SDL_TEXTINPUT:
            _text = std::string(event.text.text);
            break;

        default:
            break;
        }
    }
}