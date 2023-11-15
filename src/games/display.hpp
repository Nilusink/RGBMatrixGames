#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include "Arduino.h"


#define X_SIZE 64
#define Y_SIZE 32


namespace matrix
{
    uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);

    void reset();
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat);
    void clear(RGB64x32MatrixPanel_I2S_DMA &mat, uint16_t color);

    void print_number(int16_t x, int16_t y, uint number);
}
