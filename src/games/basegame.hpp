#pragma once

#include <vector>

namespace base
{
    struct point_t
    {
        int x;
        int y;


        bool operator == (const point_t &other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator != (const point_t &other) const
        {
            return !(*this == other);
        }

        point_t operator - (const int num) const
        {
            return {x - num, y - num};
        }
        point_t operator - (const point_t &other) const
        {
            return {x - other.x, y - other.y};
        }


        point_t operator + (const int num) const
        {
            return {x + num, y + num};
        }
        point_t operator + (const point_t &other) const
        {
            return {x + other.x, y + other.y};
        }
    };


    class BaseGame
    {
        public:
            /**
             * @brief iterate one step
             * 
             * @param delta time since last call (time passed for the game)
             * @return true game contniue
             * @return false game end | error
             */
            virtual bool step(double delta);

            /**
             * @brief render the current game status to the matrix
             * 
             */
            virtual void render();

            /**
             * @brief render the game-over screen
             * 
            */
            virtual void game_over_screen();

            /**
             * @brief reset the screen
             * 
            */
            virtual void reset();

            /**
             * @brief get the games name
             * 
            */
            virtual const char *getName();

            /**
             * @brief get the title position (left 8bit=x, right 8bit=y)
            */
            virtual void drawTitle();

            /**
             * @brief returns the number of settings (settings names are seperated by \t)
             * 
            */
            virtual std::vector<std::pair<const char*, std::pair<double, double>>> getSettings();

            /**
             * @brief change a setting
             * 
            */
            virtual void setSetting(uint setting_id, double value);
    };
}
