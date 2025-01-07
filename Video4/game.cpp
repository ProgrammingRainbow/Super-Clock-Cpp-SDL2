#include "game.h"

Game::Game()
    : window{nullptr, SDL_DestroyWindow},
      renderer{nullptr, SDL_DestroyRenderer},
      is_running{true} {}

Game::~Game() {
    this->clock.reset();

    this->renderer.reset();
    this->window.reset();

    SDL_Quit();

    std::cout << "all clean!" << std::endl;
}

void Game::init() {
    this->init_sdl();

    this->clock.reset(new Clock(this->renderer));
    this->clock->init();
}

void Game::events() {
    while (SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
        case SDL_QUIT:
            this->is_running = false;
            return;
        case SDL_KEYDOWN:
            switch (this->event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                this->is_running = false;
                return;
            case SDL_SCANCODE_C:
                this->clock->toggle_color();
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
}

void Game::draw() {
    SDL_RenderClear(this->renderer.get());

    this->clock->draw();

    SDL_RenderPresent(this->renderer.get());
}

void Game::run() {
    while (this->is_running) {
        this->events();

        this->clock->update();

        this->draw();

        SDL_Delay(16);
    }
}
