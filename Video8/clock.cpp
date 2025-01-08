#include "clock.h"
#include "main.h"

void Clock::init() {
    this->digits.clear();

    Digit digit;
    digit.rect = SDL_Rect{0, 0, DIGIT_SIZE, DIGIT_SIZE};

    for (int _ : std::views::iota(0, DIGITS_LENGTH)) {
        this->digits.push_back(digit);
    }

    this->update_layout(false);

    this->images.clear();
    this->images.resize(2);
    for (auto &first : this->images) {
        first.clear();
        first.resize(8);
        for (auto &second : first) {
            second.clear();
            second.resize(2);
            for (auto &third : second) {
                third.clear();
                third.resize(2);
                for (auto &fourth : third) {
                    fourth.clear();
                    for (int _ : std::views::iota(0, 2)) {
                        auto texture =
                            std::unique_ptr<SDL_Texture,
                                            decltype(&SDL_DestroyTexture)>(
                                nullptr, SDL_DestroyTexture);
                        fourth.push_back(std::move(texture));
                    }
                }
            }
        }
    }

    this->generate_images();
}

void Clock::update_layout(bool switch_layout) {
    if (switch_layout) {
        this->layout = (this->layout + 1) % 3;
    }
    int multiplier = this->enable_large ? 2 : 1;
    Uint32 i = 0;
    int width = 0;
    int height = 0;

    switch (this->layout) {
    case 0:
        width = (DIGIT_SIZE * 6 + DIGIT_OFFSET * 13) * multiplier;
        height = (DIGIT_SIZE * 4 + DIGIT_OFFSET * 5) * multiplier;
        for (int column : std::views::iota(0, 6)) {
            int offset = (column > 3)   ? DIGIT_OFFSET * 7
                         : (column > 1) ? DIGIT_OFFSET * 4
                                        : DIGIT_OFFSET;
            for (int row : std::views::iota(0, 4)) {
                if (i < DIGITS_LENGTH) {
                    this->digits[i].rect.x =
                        (column * (DIGIT_SIZE + DIGIT_OFFSET) + offset) *
                        multiplier;
                    this->digits[i].rect.y =
                        (row * (DIGIT_SIZE + DIGIT_OFFSET) + DIGIT_OFFSET) *
                        multiplier;
                    this->digits[i].rect.w = DIGIT_SIZE * multiplier;
                    this->digits[i].rect.h = DIGIT_SIZE * multiplier;
                    i++;
                }
            }
        }
        break;
    case 1:
        width = (DIGIT_SIZE * 3 + DIGIT_OFFSET * 10) * multiplier;
        height = (DIGIT_SIZE * 8 + DIGIT_OFFSET * 9) * multiplier;
        for (int column : std::views::iota(0, 3)) {
            int offset = (column > 1)   ? DIGIT_OFFSET * 7
                         : (column > 0) ? DIGIT_OFFSET * 4
                                        : DIGIT_OFFSET;
            for (int row : std::views::iota(0, 8)) {
                if (i < DIGITS_LENGTH) {
                    this->digits[i].rect.x =
                        (column * (DIGIT_SIZE + DIGIT_OFFSET) + offset) *
                        multiplier;
                    this->digits[i].rect.y =
                        (row * (DIGIT_SIZE + DIGIT_OFFSET) + DIGIT_OFFSET) *
                        multiplier;
                    this->digits[i].rect.w = DIGIT_SIZE * multiplier;
                    this->digits[i].rect.h = DIGIT_SIZE * multiplier;
                    i++;
                }
            }
        }
        break;
    case 2:
        width = (DIGIT_SIZE * 8 + DIGIT_OFFSET * 9) * multiplier;
        height = (DIGIT_SIZE * 3 + DIGIT_OFFSET * 10) * multiplier;
        for (int column : std::views::iota(0, 3)) {
            int offset = (column > 1)   ? DIGIT_OFFSET * 7
                         : (column > 0) ? DIGIT_OFFSET * 4
                                        : DIGIT_OFFSET;
            for (int row : std::views::iota(0, 8)) {
                if (i < DIGITS_LENGTH) {
                    this->digits[i].rect.x =
                        (row * (DIGIT_SIZE + DIGIT_OFFSET) + DIGIT_OFFSET) *
                        multiplier;
                    this->digits[i].rect.y =
                        (column * (DIGIT_SIZE + DIGIT_OFFSET) + offset) *
                        multiplier;
                    this->digits[i].rect.w = DIGIT_SIZE * multiplier;
                    this->digits[i].rect.h = DIGIT_SIZE * multiplier;
                    i++;
                }
            }
        }
        break;
    default:
        break;
    }

    SDL_SetWindowSize(this->window.get(), width, height);
    SDL_SetWindowPosition(this->window.get(), SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
}

void Clock::generate_image(std::size_t image_index, SDL_Color front_color,
                           SDL_Color back_color, bool color, bool size,
                           bool text) {
    int digit_size = size ? DIGIT_SIZE * 2 : DIGIT_SIZE;

    this->target_surf.reset(
        SDL_CreateRGBSurface(0, digit_size, digit_size, 32, 0, 0, 0, 255));
    if (!this->target_surf) {
        auto error = std::format("Error creating a target text surface: {}",
                                 SDL_GetError());
        throw std::runtime_error(error);
    }

    if (text) {
        this->font.reset(TTF_OpenFont("fonts/freesansbold.ttf", digit_size));
        if (!this->font) {
            auto error =
                std::format("Error creating a font: {}", TTF_GetError());
            throw std::runtime_error(error);
        }

        this->text_surf.reset(
            TTF_RenderText_Blended(this->font.get(), "0", back_color));
        if (!this->target_surf) {
            auto error =
                std::format("Error creating a surface: {}", TTF_GetError());
            throw std::runtime_error(error);
        }

        if (SDL_FillRect(
                this->target_surf.get(), nullptr,
                SDL_MapRGB(this->target_surf.get()->format, 0, 0, 0)) != 0) {
            auto error =
                std::format("Error creating filled rect: {}", SDL_GetError());
            throw std::runtime_error(error);
        }

        SDL_Rect rect{0, 0, digit_size, digit_size};
        rect.x = (int)((digit_size - this->text_surf.get()->w) / 2.0);
        rect.y = (int)((digit_size - this->text_surf.get()->h) / 2.0);

        SDL_BlitSurface(this->text_surf.get(), nullptr, this->target_surf.get(),
                        &rect);

    } else {
        if (SDL_FillRect(this->target_surf.get(), nullptr,
                         SDL_MapRGB(this->target_surf.get()->format,
                                    back_color.r, back_color.g,
                                    back_color.b)) != 0) {
            auto error =
                std::format("Error creating filled rect: {}", SDL_GetError());
            throw std::runtime_error(error);
        }
    }

    this->images[0][image_index][color][size][text].reset(
        SDL_CreateTextureFromSurface(this->renderer.get(),
                                     this->target_surf.get()));
    if (!this->images[0][image_index][color][size][text]) {
        auto error =
            std::format("Error creating a texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    if (text) {
        this->text_surf.reset(
            TTF_RenderText_Blended(this->font.get(), "1", front_color));
        if (!this->target_surf) {
            auto error =
                std::format("Error creating a surface: {}", TTF_GetError());
            throw std::runtime_error(error);
        }

        if (SDL_FillRect(
                this->target_surf.get(), nullptr,
                SDL_MapRGB(this->target_surf.get()->format, 0, 0, 0)) != 0) {
            auto error =
                std::format("Error creating filled rect: {}", SDL_GetError());
            throw std::runtime_error(error);
        }

        SDL_Rect rect{0, 0, digit_size, digit_size};
        rect.x = (int)((digit_size - this->text_surf.get()->w) / 2.0);
        rect.y = (int)((digit_size - this->text_surf.get()->h) / 2.0);

        SDL_BlitSurface(this->text_surf.get(), nullptr, this->target_surf.get(),
                        &rect);

    } else {
        if (SDL_FillRect(this->target_surf.get(), nullptr,
                         SDL_MapRGB(this->target_surf.get()->format,
                                    front_color.r, front_color.g,
                                    front_color.b)) != 0) {
            auto error =
                std::format("Error creating filled rect: {}", SDL_GetError());
            throw std::runtime_error(error);
        }
    }

    this->images[1][image_index][color][size][text].reset(
        SDL_CreateTextureFromSurface(this->renderer.get(),
                                     this->target_surf.get()));
    if (!this->images[1][image_index][color][size][text]) {
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

    for (std::size_t i = 0; i < 8; ++i) {
        this->generate_image(i, colors[9], colors[10], false, false, false);

        this->generate_image(i, colors[i % 8], colors[8], true, false, false);

        this->generate_image(i, colors[9], colors[10], false, true, false);

        this->generate_image(i, colors[i % 8], colors[8], true, true, false);

        this->generate_image(i, colors[9], colors[10], false, false, true);

        this->generate_image(i, colors[i % 8], colors[8], true, false, true);

        this->generate_image(i, colors[9], colors[10], false, true, true);

        this->generate_image(i, colors[i % 8], colors[8], true, true, true);
    }
}

void Clock::toggle_size() {
    this->enable_large = !this->enable_large;
    this->update_layout(false);
}

void Clock::show_time() {
    if (this->enable_time) {
        SDL_RemoveTimer(this->timer);
    }
    this->enable_time = true;
    this->timer = SDL_AddTimer(
        5000,
        [](Uint32 interval, void *param) -> Uint32 {
            (void)interval;
            (void)param;
            SDL_Event event;
            event.type = CLOCK_TIMER_EVENT;
            event.user.code = 0;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
            return 0;
        },
        nullptr);
}

void Clock::hide_time() {
    this->enable_time = false;
    SDL_SetWindowTitle(this->window.get(), WINDOW_TITLE);
}

void Clock::update() {
    auto now = std::chrono::system_clock::now();
    std::time_t in_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm bt = *std::localtime(&in_time_t);

    std::bitset<8> hours_binary(static_cast<unsigned long>(bt.tm_hour));
    std::bitset<8> minutes_binary(static_cast<unsigned long>(bt.tm_min));
    std::bitset<8> seconds_binary(static_cast<unsigned long>(bt.tm_sec));

    for (std::size_t i = 0; i < 8; ++i) {
        this->digits[i].digit = hours_binary[7 - i];
        this->digits[i + 8].digit = minutes_binary[7 - i];
        this->digits[i + 16].digit = seconds_binary[7 - i];
    }

    if (this->enable_time) {
        std::string formatted_time =
            std::format("{:02}:{:02}:{:02}", bt.tm_hour, bt.tm_min, bt.tm_sec);
        SDL_SetWindowTitle(this->window.get(), formatted_time.c_str());
    }
}

void Clock::draw() const {
    for (std::size_t i = 0; i < DIGITS_LENGTH; ++i) {
        SDL_RenderCopy(
            this->renderer.get(),
            this->images[this->digits[i].digit][i % 8][this->enable_color]
                        [this->enable_large][this->enable_text]
                            .get(),
            nullptr, &this->digits[i].rect);
    }
}
