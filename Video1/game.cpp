#include "game.h"

Game::~Game() {
    this->renderer.reset();
    this->window.reset();

    SDL_Quit();

    std::cout << "all clean!" << std::endl;
}

void Game::init() { this->init_sdl(); }

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
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Game::draw() const {
    SDL_RenderClear(this->renderer.get());

    SDL_RenderPresent(this->renderer.get());
}

void Game::run() {
    while (this->is_running) {
        this->events();

        this->draw();

        SDL_Delay(16);
    }
}
