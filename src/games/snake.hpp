#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include <cmath>

#include "basegame.hpp"
#include "display.hpp"


#define STEP_TIME 0.2


#define BG_COLOR 0, 0, 0
#define SNAKE_BODY_COLOR 0, 0, 255
#define SNAKE_HEAD_COLOR 0, 255, 0
#define APPLE_COLOR 255, 0, 0
#define SNAKE_SIZE_MULTIPLIER 2

#define GAME_SIZE_X X_SIZE / SNAKE_SIZE_MULTIPLIER
#define GAME_SIZE_Y Y_SIZE / SNAKE_SIZE_MULTIPLIER


namespace snake
{
    class Snake : base::BaseGame
    {
        private:
            RGB64x32MatrixPanel_I2S_DMA &m;
            base::point_t positions[GAME_SIZE_X * GAME_SIZE_Y];
            base::point_t apple_pos;
            uint8_t facing = 0;
            uint size = 0;

            float time_since_step = 0;

            void reset_snake();
            void generate_apple();

        public:
            Snake(RGB64x32MatrixPanel_I2S_DMA &mat);

            bool step(double delta) override;
            void render() override;
            void game_over_screen() override;
            void reset() override;
    };
}
