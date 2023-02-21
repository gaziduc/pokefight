#pragma once

#include <SDL2/SDL.h>
#include <string>

// Forward declaration
class Window;

class Events {
private:
    bool _quit = false;
    bool _keys[SDL_NUM_SCANCODES] = { false };
    std::string _text;
    bool _focus_lost = false;

public:
    bool is_quit() const;
    bool is_key_down(const int key) const;
    void press_up_key(const int key);
    bool is_enter_down() const;
    bool is_escape_down() const;
    std::string get_text() const;
    bool is_focus_lost() const;

    void update_events(Window& window);
};