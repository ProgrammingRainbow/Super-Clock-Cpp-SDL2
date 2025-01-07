#ifndef GAME_H
#define GAME_H

#include "clock.h"

class Game {
    public:
        Game();
        ~Game();

        void init_sdl();
        void init();
        void run();

    private:
        void events();
        void update();
        void draw();

        std::unique_ptr<Clock> clock;

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;
        SDL_Event event;
        bool is_running;
};

#endif
