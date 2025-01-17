#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <format>
#include <iostream>
#include <memory>

constexpr int SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_AUDIO);

constexpr const char *WINDOW_TITLE = "Super Clock";
constexpr int WINDOW_WIDTH = 294;
constexpr int WINDOW_HEIGHT = 174;

#endif
