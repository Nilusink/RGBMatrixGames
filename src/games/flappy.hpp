#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include <cmath>
#include "basegame.hpp"


#define GRAVITY 0.1
#define WORLD_ACC 0.1
#define WORLD_START_VEL 10
#define WALL_DISTANCE 20
#define WALL_THICKNESS 4
#define WALL_GAP_HEIGHT 10
#define FIRST_WALL 40

#define JUMP_SPEED -.03

#define BIRD_X 10
#define BIRD_SIZE 2


#define BG_COLOR 0, 0, 0
#define WALL_COLOR 255, 255, 255
#define BIRD_COLOR 255, 0, 125
#define TEXT_COLOR 255, 0, 0


namespace flappy
{
    class Flappy : base::BaseGame
    {
        protected:
            RGB64x32MatrixPanel_I2S_DMA &m;

            double bird_y = BIRD_SIZE;
            double bird_vel_y = 0;

            double world_pos = 0;
            double world_vel = WORLD_START_VEL;

            int wall_heights[32] = {0};  // hard coded max walls, may cause problems
            int first_wall_position = FIRST_WALL;

            bool score_counted_for_wall = false;

            uint highscore = 0;
            uint score = 0;
            uint last_score = 0;

            uint random_wall_height();
            void init_walls();

        public:
            Flappy(RGB64x32MatrixPanel_I2S_DMA &mat);

            bool step(double delta) override;
            void render() override;
            void game_over_screen() override;
            void reset() override;
    };
}
