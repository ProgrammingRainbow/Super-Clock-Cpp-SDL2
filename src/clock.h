#ifndef CLOCK_H
#define CLOCK_H

#include "main.h"

struct Digit {
        SDL_Rect rect;
        bool digit;
};

class Clock {
    public:
        Clock(std::shared_ptr<SDL_Window> win,
              std::shared_ptr<SDL_Renderer> ren)
            : window{win},
              renderer{ren},
              target_surf{nullptr, SDL_FreeSurface},
              text_surf{nullptr, SDL_FreeSurface},
              font{nullptr, TTF_CloseFont},
              images{},
              digits{},
              enable_color{true},
              enable_large{false},
              enable_text{false},
              layout{0},
              enable_time{false},
              timer{} {}

        void init();
        void update_layout(bool switch_laout);
        void update();
        void toggle_size();
        void draw() const;
        void show_time();
        void hide_time();

        void inline toggle_color() {
            this->enable_color = !this->enable_color;
        };
        void inline toggle_text() { this->enable_text = !this->enable_text; };

    private:
        void generate_image(std::size_t image_index, SDL_Color front_color,
                            SDL_Color back_color, bool color, bool size,
                            bool text);
        void generate_images();

        std::shared_ptr<SDL_Window> window;
        std::shared_ptr<SDL_Renderer> renderer;
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> target_surf;
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> text_surf;
        std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
        std::vector<std::vector<std::vector<std::vector<std::vector<
            std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>>>>>>
            images;
        std::vector<Digit> digits;
        bool enable_color;
        bool enable_large;
        bool enable_text;
        int layout;
        bool enable_time;
        SDL_TimerID timer;
};

#endif
