#include <Arduino.h>

#include "snake.hpp"
#include "display.hpp"


using namespace snake;


Snake::Snake(RGB64x32MatrixPanel_I2S_DMA &mat, controller::Controller &controller)
 : m(mat), control(controller)
{
    reset_snake();
    generate_apple();
}


void Snake::reset_snake()
{
    positions[0] = {4, 1};
    positions[1] = {3, 1};
    positions[2] = {2, 1};

    facing = 1;
    size = 3;
}


void Snake::generate_apple()
{
    // delete last apple
    apple_pos = {random(0, GAME_SIZE_X), random(0, GAME_SIZE_Y)};
}


bool Snake::step(double delta)
{
    time_since_step += delta;

    if (control.getButtons() & JOY_DEPRESS)
        return false;

    if (control.getButtons() & JOY_UP)
    {
        base::point_t new_pos = {positions[0].x, positions[0].y - 1};
        if (new_pos != positions[1])
        {
            facing = 0;
        }
    }
    else if (control.getButtons() & JOY_RIGHT)
    {
        base::point_t new_pos = {positions[0].x + 1, positions[0].y};
        if (new_pos != positions[1])
        {
            facing = 1;
        }
    }
    else if (control.getButtons() & JOY_DOWN)
    {
        base::point_t new_pos = {positions[0].x, positions[0].y + 1};
        if (new_pos != positions[1])
        {
            facing = 2;
        }
    }
    else if (control.getButtons() & JOY_LEFT)
    {
        base::point_t new_pos = {positions[0].x - 1, positions[0].y};
        if (new_pos != positions[1])
        {
            facing = 3;
        }
    }

    // move steps (according to time delta)
    while (time_since_step > step_time)
    {
        time_since_step -= step_time;

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
        m.fillRect(
            apple_pos.x,
            apple_pos.y,
            SNAKE_SIZE_MULTIPLIER,
            SNAKE_SIZE_MULTIPLIER,
            matrix::rgb(BG_COLOR)
        );

        generate_apple();
        size++;
    }

    // check for wall collision
    if
    (
        !(0 <= positions[0].x && positions[0].x < GAME_SIZE_X)
        || !(0 <= positions[0].y && positions[0].y < GAME_SIZE_Y)
    )
    {
        return false;
    }


    // check for snake collision
    for (int i = 1; i < size; i++)
    {
        if (positions[0] == positions[i])
            return false;
    }

    return true;
}


void Snake::render()
{
    // draw snake
    /// delete snakes end
    m.fillRect(
        positions[size].x * SNAKE_SIZE_MULTIPLIER,
        positions[size].y * SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER,
        matrix::rgb(BG_COLOR)
    );
    for (int i = 1; i < size; i++)
    {
        m.fillRect(
            positions[i].x * SNAKE_SIZE_MULTIPLIER,
            positions[i].y * SNAKE_SIZE_MULTIPLIER,
            SNAKE_SIZE_MULTIPLIER,
            SNAKE_SIZE_MULTIPLIER,
            matrix::rgb(SNAKE_BODY_COLOR)
        );
    }

    /// draw snake head (lt to rb)s
    m.fillRect(
        positions[0].x * SNAKE_SIZE_MULTIPLIER,
        positions[0].y * SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        (facing == 0 || facing == 3) ? matrix::rgb(SNAKE_HEAD_COLOR) : matrix::rgb(SNAKE_BODY_COLOR)
    );
    m.fillRect(
        positions[0].x * SNAKE_SIZE_MULTIPLIER + SNAKE_SIZE_MULTIPLIER / 2,
        positions[0].y * SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        (facing == 0 || facing == 1) ? matrix::rgb(SNAKE_HEAD_COLOR) : matrix::rgb(SNAKE_BODY_COLOR)
    );
    m.fillRect(
        positions[0].x * SNAKE_SIZE_MULTIPLIER,
        positions[0].y * SNAKE_SIZE_MULTIPLIER + SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        (facing == 2 || facing == 3) ? matrix::rgb(SNAKE_HEAD_COLOR) : matrix::rgb(SNAKE_BODY_COLOR)
    );
    m.fillRect(
        positions[0].x * SNAKE_SIZE_MULTIPLIER + SNAKE_SIZE_MULTIPLIER / 2,
        positions[0].y * SNAKE_SIZE_MULTIPLIER + SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        SNAKE_SIZE_MULTIPLIER / 2,
        (facing == 1 || facing == 2) ? matrix::rgb(SNAKE_HEAD_COLOR) : matrix::rgb(SNAKE_BODY_COLOR)
    );


    // draw apple
    m.fillRect(
        apple_pos.x * SNAKE_SIZE_MULTIPLIER,
        apple_pos.y * SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER,
        SNAKE_SIZE_MULTIPLIER,
        matrix::rgb(APPLE_COLOR)
    );
}


// doesn't really have a game-over screen
void Snake::game_over_screen() {};


void Snake::reset()
{
    reset_snake();
    generate_apple();

    matrix::clear(m, {BG_COLOR});
}


const char *Snake::getName()
{
    return "Snake";
}


void Snake::drawTitle()
{
    m.setCursor(18, 13);
    m.print("Snake");
}


std::vector<std::pair<const char*, std::pair<double, double>>> Snake::getSettings()
{
    return {{"time", {step_time, .01}}};
}


void Snake::setSetting(uint setting_id, double value)
{
    switch (setting_id)
    {
        case 0:
            step_time = value;
    }
}
