#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

struct Digit {
        SDL_Rect rect;
        bool digit;
};

class Clock {
    public:
        Clock(std::shared_ptr<SDL_Renderer> ren)
            : renderer{ren},
              target_surf{nullptr, SDL_FreeSurface},
              images{},
              digits{},
              enable_color{true} {}

        void init();
        void update_layout();
        void update();
        void draw() const;

        void toggle_color() { this->enable_color = !this->enable_color; };

    private:
        void generate_image(int image_index, SDL_Color front_color,
                            SDL_Color back_color, bool color);
        void generate_images();

        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> target_surf;
        std::vector<std::vector<std::vector<
            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>>>>
            images;
        std::vector<Digit> digits;
        bool enable_color;
};

#endif
