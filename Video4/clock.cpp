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
    this->images.resize(2);
    for (auto &first : this->images) {
        first.clear();
        first.resize(8);
        for (auto &second : first) {
            second.clear();
            for (int _ : std::views::iota(0, 2)) {
                auto texture =
                    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
                        nullptr, SDL_DestroyTexture);
                second.push_back(std::move(texture));
            }
        }
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

void Clock::generate_image(int image_index, SDL_Color front_color,
                           SDL_Color back_color, bool color) {

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

    this->images[0][image_index][color].reset(SDL_CreateTextureFromSurface(
        this->renderer.get(), this->target_surf.get()));
    if (!this->images[0][image_index][color]) {
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

    this->images[1][image_index][color].reset(SDL_CreateTextureFromSurface(
        this->renderer.get(), this->target_surf.get()));
    if (!this->images[1][image_index][color]) {
        auto error =
            std::format("Error creating a texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }
}

void Clock::generate_images() {
    std::vector<SDL_Color> colors{
        {245, 224, 220, 255}, {243, 139, 168, 255}, {250, 179, 135, 255},
        {249, 226, 175, 255}, {166, 227, 161, 255}, {148, 226, 213, 255},
        {137, 180, 250, 255}, {203, 166, 247, 255}, {30, 30, 46, 255},
        {223, 223, 223, 255}, {31, 31, 31, 255},
    };

    for (int i : std::views::iota(0, 8)) {
        this->generate_image(i, colors[9], colors[10], false);

        this->generate_image(i, colors[i % 8], colors[8], true);
    }
}

void Clock::update() {
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&in_time_t);

    std::bitset<8> hours_binary(bt.tm_hour);
    std::bitset<8> minutes_binary(bt.tm_min);
    std::bitset<8> seconds_binary(bt.tm_sec);

    for (int i : std::views::iota(0, 8)) {
        this->digits[i].digit = hours_binary[7 - i];
        this->digits[i + 8].digit = minutes_binary[7 - i];
        this->digits[i + 16].digit = seconds_binary[7 - i];
    }
}

void Clock::draw() const {
    for (int i : std::views::iota(0, DIGITS_LENGTH)) {
        SDL_RenderCopy(
            this->renderer.get(),
            this->images[this->digits[i].digit][i % 8][this->enable_color]
                .get(),
            nullptr, &this->digits[i].rect);
    }
}
