#pragma once

#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include "Arduino.h"


#define X_SIZE 64
#define Y_SIZE 32


namespace matrix
{
    uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);

    struct rgb_t
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        // convert to 16-bit integer
        operator uint16_t() const {
            return rgb(r, g, b);
        };

        explicit operator uint16_t*() const { return nullptr; };
    };

    uint16_t rgb(rgb_t color);

    void reset();
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat);
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat, rgb_t color);

    void fillAntiAliaseRect(RGB64x32MatrixPanel_I2S_DMA &mat, double x, double y, double width, double height, rgb_t color);
}
