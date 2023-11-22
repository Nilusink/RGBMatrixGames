#pragma once

#include <Arduino.h>


#define JOY_DEPRESS 0b00000001
#define JOY_RIGHT   0b00000010
#define JOY_LEFT    0b00000100
#define JOY_DOWN    0b00001000
#define JOY_UP      0b00010000
#define JOY_ARROWS  0b00011110


#define THRESHOLD 800
#define ADC_RES 4096

namespace controller
{
    class Controller {
        protected:
            uint8_t x_pin;
            uint8_t y_pin;
            uint8_t b_pin;

        public:
            Controller(uint8_t pin_jx, uint8_t pin_jy, uint8_t pin_jb);

            /**
             * @brief get all button values as up, down, left, right, depress
            */
            uint8_t getButtons();
    };
}