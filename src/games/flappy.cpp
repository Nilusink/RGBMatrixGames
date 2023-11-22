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
    // dynamicall generate walls
    int last_wall_height = wall_heights[(int)floor(X_SIZE / WALL_DISTANCE) - 1];
    double max_deviation = 2 + MAX_WALL_HEIGHT * (10 / world_vel);

    printf("speed: %f, \tmax_deviation: %f\n", world_vel, max_deviation);

    return random(
        max(2, (int)floor(last_wall_height - max_deviation)),
        min(MAX_WALL_HEIGHT, (int)(ceil(last_wall_height + max_deviation)))
    );
}


bool Flappy::step(double delta)
{
    // walls
    // check if first wall is out of screen
    if (first_wall_position - floor(world_pos) <= -WALL_THICKNESS)
    {   // need new walls
        score_counted_for_wall = false;

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
        do { Serial.read(); } while (Serial.available());

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

    // only count up once per wall :-)
    if ((world_pos + BIRD_X) - BIRD_SIZE >= first_wall_position && !score_counted_for_wall)
    {
        score_counted_for_wall = true;
        score++;
    }

    return true;
}


void Flappy::render()
{
    // walls
    float disp_pos;
    for (uint n_wall = 0; n_wall < X_SIZE / WALL_DISTANCE; n_wall++)
    {
        disp_pos = (first_wall_position - world_pos) + WALL_DISTANCE * n_wall;

        // only draw walls that are on screen
        if (disp_pos >= -WALL_THICKNESS && disp_pos <= X_SIZE)
        {
            // m.fillCircle(disp_pos, wall_heights[n_wall], 2, matrix::rgb(WALL_COLOR));
            // upper wall
            matrix::fillAntiAliaseRect(
                m,
                disp_pos,
                0,
                WALL_THICKNESS,
                wall_heights[n_wall],
                {WALL_COLOR}
            );

            // lower wall
            matrix::fillAntiAliaseRect(
                m,
                disp_pos,
                wall_heights[n_wall] + WALL_GAP_HEIGHT,
                WALL_THICKNESS,
                Y_SIZE - (wall_heights[n_wall] + WALL_GAP_HEIGHT),
                {WALL_COLOR}
            );

            // clear area to next wall
            // upper wall
            m.fillRect(
                disp_pos + WALL_THICKNESS + 1,
                0,
                2,
                wall_heights[n_wall],
                matrix::rgb(BG_COLOR)
            );

            // lower wall
            m.fillRect(
                disp_pos + WALL_THICKNESS + 1,
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

    matrix::fillAntiAliaseRect(
        m,
        BIRD_X - BIRD_SIZE,
        bird_y - BIRD_SIZE,
        BIRD_SIZE * 2,
        BIRD_SIZE * 2,
        {BIRD_COLOR}
    );

    // eyes
    if (bird_vel_y >= 0)
        m.drawPixel(
            BIRD_X,
            bird_y + 1,
            matrix::rgb(0, 255, 0)
        );

    if (bird_vel_y <= 0)
        m.drawPixel(
            BIRD_X,
            bird_y - 1,
            matrix::rgb(0, 255, 0)
        );

    /// clear previous number
    m.fillRect(
        (score > 9) ? ((score > 99) ? 46 : 52) : 58,
        0,
        17,
        16,
        matrix::rgb(BG_COLOR)
    );

    m.setCursor(((score > 9) ? ((score > 99) ? 46 : 52) : 58), 1);
    m.setTextColor(matrix::rgb(TEXT_COLOR));
    m.printf("%d", score);

    last_score = score;
}


void Flappy::game_over_screen()
{
    m.fillRect(
        (highscore > 9) ? ((highscore > 99) ? 45 : 51) : 57,
        0,
        17,
        16,
        matrix::rgb(BG_COLOR)
    );

    m.setCursor(((score > 9) ? ((score > 99) ? 46 : 52) : 58), 1);
    m.setTextColor(matrix::rgb(TEXT_COLOR));

    m.printf("%d", score);
    highscore = max(highscore, score);

    m.setCursor(((highscore > 9) ? ((highscore > 99) ? 46 : 52) : 58), 9);
    m.setTextColor(matrix::rgb(TEXT_COLOR));
    m.printf("%d", highscore);
}


void Flappy::reset()
{
    matrix::clear(m, {BG_COLOR});

    // reset game variables
    bird_y = BIRD_SIZE;
    bird_vel_y = 0;

    world_pos = 0;
    world_vel = WORLD_START_VEL;
    first_wall_position = FIRST_WALL;

    score_counted_for_wall = false;

    score = 0;

    // re-generate walls
    init_walls();
}