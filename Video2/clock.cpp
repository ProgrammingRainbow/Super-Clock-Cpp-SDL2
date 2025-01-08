#include "clock.h"

void Clock::init() {
    this->digits.clear();

    Digit digit;
    digit.rect = SDL_Rect{0, 0, DIGIT_SIZE, DIGIT_SIZE};

    for (int _ : std::views::iota(0, DIGITS_LENGTH)) {
        this->digits.push_back(digit);
    }

    this->update_layout();

    this->images.clear();
    for (int i = 0; i < 2; ++i) {
        auto texture =
            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
                nullptr, SDL_DestroyTexture);
        this->images.push_back(std::move(texture));
    }

    this->generate_images();
}

void Clock::update_layout() {
    Uint32 i = 0;
    for (int column : std::views::iota(0, 6)) {
        int offset = (column > 3)   ? DIGIT_OFFSET * 7
                     : (column > 1) ? DIGIT_OFFSET * 4
                                    : DIGIT_OFFSET;
        for (int row : std::views::iota(0, 4)) {
            if (i < DIGITS_LENGTH) {
                this->digits[i].rect.x =
                    (column * (DIGIT_SIZE + DIGIT_OFFSET) + offset);
                this->digits[i].rect.y =
                    (row * (DIGIT_SIZE + DIGIT_OFFSET) + DIGIT_OFFSET);
                this->digits[i].rect.w = DIGIT_SIZE;
                this->digits[i].rect.h = DIGIT_SIZE;
                i++;
            }
        }
    }
}

void Clock::generate_image(SDL_Color front_color, SDL_Color back_color) {

    if (this->target_surf) {
        SDL_FreeSurface(this->target_surf.get());
        this->target_surf.reset();
    }

    this->target_surf.reset(
        SDL_CreateRGBSurface(0, DIGIT_SIZE, DIGIT_SIZE, 32, 0, 0, 0, 255));
    if (!this->target_surf) {
        auto error = std::format("Error creating a target text surface: {}",
                                 SDL_GetError());
        throw std::runtime_error(error);
    }

    if (SDL_FillRect(this->target_surf.get(), nullptr,
                     SDL_MapRGB(this->target_surf.get()->format, back_color.r,
                                back_color.g, back_color.b)) != 0) {
        auto error =
            std::format("Error creating filled rect: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->images[0].reset(SDL_CreateTextureFromSurface(
        this->renderer.get(), this->target_surf.get()));
    if (!this->images[0]) {
        auto error =
            std::format("Error creating a texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    if (SDL_FillRect(this->target_surf.get(), nullptr,
                     SDL_MapRGB(this->target_surf.get()->format, front_color.r,
                                front_color.g, front_color.b)) != 0) {
        auto error =
            std::format("Error creating filled rect: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->images[1].reset(SDL_CreateTextureFromSurface(
        this->renderer.get(), this->target_surf.get()));
    if (!this->images[1]) {
        auto error =
            std::format("Error creating a texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }
}

void Clock::generate_images() {
    std::vector<SDL_Color> colors{
        {223, 223, 223, 255},
        {31, 31, 31, 255},
    };

    this->generate_image(colors[0], colors[1]);
}

void Clock::draw() const {
    for (std::size_t i = 0; i < DIGITS_LENGTH; i++) {
        SDL_RenderCopy(this->renderer.get(), this->images[1].get(), nullptr,
                       &this->digits[i].rect);
    }
}
