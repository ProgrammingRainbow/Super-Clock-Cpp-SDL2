#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

struct Digit {
        SDL_Rect rect;
};

class Clock {
    public:
        Clock(std::shared_ptr<SDL_Renderer> ren)
            : renderer{ren},
              target_surf{nullptr, SDL_FreeSurface},
              images{},
              digits{} {}

        void init();
        void update_layout();
        void draw() const;

    private:
        void generate_image(SDL_Color front_color, SDL_Color back_color);
        void generate_images();

        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> target_surf;
        std::vector<std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>>
            images;
        std::vector<struct Digit> digits;
};

#endif
