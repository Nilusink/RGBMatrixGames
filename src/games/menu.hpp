#pragma once

#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include <Arduino.h>
#include <array>

#include "basegame.hpp"


#define BG_COLOR 0, 0, 0
#define FG_COLOR 255, 255, 255


namespace menu
{
    class Menu
    {
        protected:
            RGB64x32MatrixPanel_I2S_DMA &m;
            const std::array<base::BaseGame *, 2> &g;
            uint8_t current_page = 0;

        public:
            Menu(
                RGB64x32MatrixPanel_I2S_DMA &mat,
                std::array<base::BaseGame *, 2> &games
            );

            void setPage(int page);
            void nextPage();
            void prevPage();
            uint8_t getPage();

            void update_page();
            void reset();

            base::BaseGame &getGame();
    };
}
