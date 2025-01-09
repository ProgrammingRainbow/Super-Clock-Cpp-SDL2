#include "game.h"

Game::~Game() {
    this->clock.reset();

    this->renderer.reset();
    this->window.reset();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    std::cout << "all clean!" << std::endl;
}

void Game::init() {
    this->init_sdl();

    this->clock.reset(new Clock(this->window, this->renderer));
    this->clock->init();
}

void Game::events() {
    while (SDL_PollEvent(&this->event)) {
        switch (this->event.type) {
        case SDL_QUIT:
            this->is_running = false;
            return;
        case CLOCK_TIMER_EVENT:
            this->clock->hide_time();
            break;
        case SDL_KEYDOWN:
            switch (this->event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                this->is_running = false;
                return;
            case SDL_SCANCODE_B:
                this->clock->toggle_size();
                break;
            case SDL_SCANCODE_C:
                this->clock->toggle_color();
                break;
            case SDL_SCANCODE_L:
            case SDL_SCANCODE_S:
                this->clock->update_layout(true);
                break;
            case SDL_SCANCODE_T:
                this->clock->toggle_text();
                break;
            case SDL_SCANCODE_SPACE:
                this->clock->show_time();
                break;
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
