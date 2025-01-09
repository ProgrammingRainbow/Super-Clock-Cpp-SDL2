#ifndef GAME_H
#define GAME_H

#include "clock.h"

class Game {
    public:
        Game()
            : window{nullptr, SDL_DestroyWindow},
              renderer{nullptr, SDL_DestroyRenderer},
              clock{nullptr},
              event{},
              is_running{true} {}
        ~Game();

        void init_sdl();
        void init();
        void run();

    private:
        void events();
        void update();
        void draw() const;

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<Clock> clock;
        SDL_Event event;
        bool is_running;
};

#endif
