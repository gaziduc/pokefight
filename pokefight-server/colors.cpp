#include "colors.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void Colors::enable_colors() {
#ifdef _WIN32
    HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(handleOut, &consoleMode);
    if (!(consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(handleOut, consoleMode);
    }
#endif
}

std::string Colors::yellow() {
    return "\x1B[33m";
}

std::string Colors::green() {
    return "\x1B[32m";
}

std::string Colors::cyan() {
    return "\x1B[96m";
}

std::string Colors::reset() {
    return "\033[0m";
}
