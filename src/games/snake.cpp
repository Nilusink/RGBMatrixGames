#include <Arduino.h>

#include "snake.hpp"


Snake::Snake(RGB64x32MatrixPanel_I2S_DMA &mat)
 : m(mat)
{
    reset_snake();
}


void Snake::reset_snake()
{
    positions = {{0, 0}};

    positions[0] = {4, 1};
    positions[1] = {3, 1};
    positions[2] = {2, 1};

    facing = 1;
    size = 3;
}


void Snake::generate_apple()
{
    // delete last apple
    m.drawPixel(apple_pos.x, apple_pos.y, matrix::rgb(BG_COLOR));
    apple_pos = {random(0, X_SIZE), random(0, Y_SIZE)};
    matrix::clear(m, BG_COLOR);
}


void Snake::step(double delta)
{
    time_since_step += delta;

    // move steps (according to time delta)
    while (time_since_step > STEP_TIME)
    {
        time_since_step -= STEP_TIME;

        // move all parts one back
        for (int i = size; i > 0; i--)
        {
            positions[i] = positions[i-1];
        };

        // move head
        positions[0].x += (facing == 1) ? 1 : ((facing == 3) ? -1 : 0);
        positions[0].y += (facing == 0) ? -1 : ((facing == 2) ? 1 : 0);
    }


    // check for apple collision
    if (positions[0] == apple_pos)
    {
        generate_apple();
        size++;
    }
}


void Snake::render()
{
    // draw snake
    /// delete snakes end
    m.drawPixel(positions[size].x, positions[size].y, matrix::rgb(BG_COLOR));
    m.drawPixel(positions[0].x, positions[0].y, matrix::rgb(SNAKE_HEAD_COLOR));
    for (int i = 1; i < size; i++)
    {
        m.drawPixel(positions[i].x, positions[i].y, matrix::rgb(SNAKE_BODY_COLOR));
    }

    // draw apple
    m.drawPixel(apple_pos.x, apple_pos.y, matrix::rgb(APPLE_COLOR));
}