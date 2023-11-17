#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include "Arduino.h"


#define X_SIZE 64
#define Y_SIZE 32


namespace matrix
{
    struct rgb_t
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        // convert to 16-bit integer
        operator uint16_t() const {
            return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        };

        explicit operator uint16_t*() const { return nullptr; };
    };

    uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);

    void reset();
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat);
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat, uint16_t color);

    void fillAntiAliaseRect(RGB64x32MatrixPanel_I2S_DMA &mat, double x, double y, double width, double height, rgb_t color);
}
