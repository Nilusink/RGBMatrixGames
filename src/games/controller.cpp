#include "controller.hpp"


using namespace controller;


Controller::Controller(uint8_t pin_jx, uint8_t pin_jy, uint8_t pin_jb)
 : x_pin(pin_jx), y_pin(pin_jy), b_pin(pin_jb)
{
    pinMode(b_pin, INPUT_PULLUP);
}


uint8_t Controller::getButtons()
{
    // convert from joystick analog value to button
    uint16_t x_value = analogRead(y_pin);
    uint16_t y_value = analogRead(x_pin);
    bool left = (x_value < ADC_RES / 2 - THRESHOLD);
    bool right = (x_value > ADC_RES / 2 + THRESHOLD);
    bool up = (y_value > ADC_RES / 2 + THRESHOLD);
    bool down = (y_value < ADC_RES / 2 - THRESHOLD);

    return (up << 4) | (down << 3) | (left << 2) | (right << 1) | (uint8_t)(!digitalRead(b_pin));
}
