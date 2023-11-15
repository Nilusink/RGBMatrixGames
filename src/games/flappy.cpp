#include <Arduino.h>

#include "flappy.hpp"
#include "display.hpp"


using namespace flappy;


Flappy::Flappy(RGB64x32MatrixPanel_I2S_DMA &mat)
 : m(mat)
{
    // generate walls
    init_walls();
}


void Flappy::init_walls()
{
    for (uint n_wall = 0; n_wall < X_SIZE / WALL_DISTANCE; n_wall++)
    {
        wall_heights[n_wall] = random_wall_height();
    }
}


uint Flappy::random_wall_height()
{
    return random(2, 30 - WALL_GAP_HEIGHT);
}


bool Flappy::step(double delta)
{
    // walls
    // check if first wall is out of screen
    if (first_wall_position - floor(world_pos) <= -WALL_THICKNESS)
    {   // need new walls
        // count up score
        score++;

        first_wall_position += WALL_DISTANCE;

        // re-assign wall-heights list
        uint n_wall = 0;
        for (; n_wall < X_SIZE / WALL_DISTANCE; n_wall++)
        {
            wall_heights[n_wall] = wall_heights[n_wall + 1];
        }

        // generate new wall at end
        wall_heights[n_wall] = random_wall_height();
    }

    // physics
    bird_vel_y += GRAVITY * delta;

    if (Serial.available() && bird_vel_y > JUMP_SPEED / 5)
    {
        Serial.read();
        bird_vel_y = JUMP_SPEED;
    }

    bird_y += bird_vel_y;

    world_vel += WORLD_ACC * delta;
    world_pos += world_vel * delta;


    // check for collisions
    /// floor
    if (bird_y >= Y_SIZE)
    {
        return false;
    }

    /// walls
    //// check x then y
    if
    (
        (world_pos + BIRD_X) - BIRD_SIZE <= first_wall_position + WALL_THICKNESS
        && (world_pos + BIRD_X) + BIRD_SIZE >= first_wall_position
    )
    {
        if
        (
            bird_y + BIRD_SIZE >= wall_heights[0] + WALL_GAP_HEIGHT
            || bird_y - BIRD_SIZE <= wall_heights[0]
        )
        {
            return false;
        }
    }

    return true;
}


void Flappy::render()
{
    // walls
    int disp_pos;
    for (uint n_wall = 0; n_wall < X_SIZE / WALL_DISTANCE; n_wall++)
    {
        disp_pos = (first_wall_position - floor(world_pos)) + WALL_DISTANCE * n_wall;

        // only draw walls that are on screen
        if (disp_pos >= -WALL_THICKNESS && disp_pos <= X_SIZE)
        {
            // m.fillCircle(disp_pos, wall_heights[n_wall], 2, matrix::rgb(WALL_COLOR));
            // upper wall
            m.fillRect(
                disp_pos,
                0,
                WALL_THICKNESS,
                wall_heights[n_wall],
                matrix::rgb(WALL_COLOR)
            );

            // lower wall
            m.fillRect(
                disp_pos,
                wall_heights[n_wall] + WALL_GAP_HEIGHT,
                WALL_THICKNESS,
                Y_SIZE - (wall_heights[n_wall] + WALL_GAP_HEIGHT),
                matrix::rgb(WALL_COLOR)
            );

            // clear area to next wall
            // upper wall
            m.fillRect(
                disp_pos + WALL_THICKNESS,
                0,
                2,
                wall_heights[n_wall],
                matrix::rgb(BG_COLOR)
            );

            // lower wall
            m.fillRect(
                disp_pos + WALL_THICKNESS,
                wall_heights[n_wall] + WALL_GAP_HEIGHT,
                2,
                Y_SIZE - (wall_heights[n_wall] + WALL_GAP_HEIGHT),
                matrix::rgb(BG_COLOR)
            );
        }
    }


    // bird
    m.fillRect(
        BIRD_X - BIRD_SIZE,
        bird_y - BIRD_SIZE - 2,
        BIRD_SIZE * 2,
        2,
        matrix::rgb(BG_COLOR)
    );

    m.fillRect(
        BIRD_X - BIRD_SIZE,
        bird_y + BIRD_SIZE,
        BIRD_SIZE * 2,
        2,
        matrix::rgb(BG_COLOR)
    );

    m.fillRect(
        BIRD_X - BIRD_SIZE,
        bird_y - BIRD_SIZE,
        BIRD_SIZE * 2,
        BIRD_SIZE * 2,
        matrix::rgb(BIRD_COLOR)
    );

    // player scores
    m.setCursor(44, 0);
    m.setTextColor(matrix::rgb(TEXT_COLOR));
    m.printf("%d", score);
}


void Flappy::reset()
{
    matrix::clear(m, matrix::rgb(BG_COLOR));
    // reset game variables
    bird_y = BIRD_SIZE;
    bird_vel_y = 0;

    world_pos = 0;
    world_vel = WORLD_START_VEL;
    first_wall_position = FIRST_WALL;

    // re-generate walls
    init_walls();
}