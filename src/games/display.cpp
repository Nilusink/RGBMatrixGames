#include <cmath>

#include "display.hpp"
#include "pinout.h"

uint16_t matrix::rgb(uint8_t r, uint8_t g, uint8_t b)
{
    // rgb as int: 5 - 6 - 5
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}


void matrix::reset()
{
    int MaxLed = 64;

    int C12[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    int C13[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

    pinMode(R1, OUTPUT);
    pinMode(G1, OUTPUT);
    pinMode(BL1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(BL2, OUTPUT);
    pinMode(CH_A, OUTPUT);
    pinMode(CH_B, OUTPUT);
    pinMode(CH_C, OUTPUT);
    pinMode(CH_D, OUTPUT);
    pinMode(CH_E, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(LAT, OUTPUT);
    pinMode(OE, OUTPUT);

    // Send Data to control register 11
    digitalWrite(OE, HIGH); // Display reset
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);
    for (int l = 0; l < MaxLed; l++)
    {
        int y = l % 16;
        digitalWrite(R1, LOW);
        digitalWrite(G1, LOW);
        digitalWrite(BL1, LOW);
        digitalWrite(R2, LOW);
        digitalWrite(G2, LOW);
        digitalWrite(BL2, LOW);
        if (C12[y] == 1)
        {
            digitalWrite(R1, HIGH);
            digitalWrite(G1, HIGH);
            digitalWrite(BL1, HIGH);
            digitalWrite(R2, HIGH);
            digitalWrite(G2, HIGH);
            digitalWrite(BL2, HIGH);
        }
        if (l > MaxLed - 12)
        {
            digitalWrite(LAT, HIGH);
        }
        else
        {
            digitalWrite(LAT, LOW);
        }
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);
    // Send Data to control register 12
    for (int l = 0; l < MaxLed; l++)
    {
        int y = l % 16;
        digitalWrite(R1, LOW);
        digitalWrite(G1, LOW);
        digitalWrite(BL1, LOW);
        digitalWrite(R2, LOW);
        digitalWrite(G2, LOW);
        digitalWrite(BL2, LOW);
        if (C13[y] == 1)
        {
            digitalWrite(R1, HIGH);
            digitalWrite(G1, HIGH);
            digitalWrite(BL1, HIGH);
            digitalWrite(R2, HIGH);
            digitalWrite(G2, HIGH);
            digitalWrite(BL2, HIGH);
        }
        if (l > MaxLed - 13)
        {
            digitalWrite(LAT, HIGH);
        }
        else
        {
            digitalWrite(LAT, LOW);
        }
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LAT, LOW);
    digitalWrite(CLK, LOW);
}


void matrix::clear(RGB64x32MatrixPanel_I2S_DMA &mat)
{
    clear(mat, 0);
}

void matrix::clear(RGB64x32MatrixPanel_I2S_DMA &mat, uint16_t color)
{
    mat.fillRect(0, 0, X_SIZE, Y_SIZE, color);
}


void matrix::fillAntiAliaseRect(RGB64x32MatrixPanel_I2S_DMA &mat, double x, double y, double width, double height, rgb_t color)
{
    const double top_multiplier = 1 - (y - floor(y));
    const double bottom_multiplier = y - floor(y);
    const double left_multiplier = 1 - (x - floor(x));
    const double right_multiplier = x - floor(x);

    // draw base rectangle
    mat.fillRect(x+1, y+1, width-1, height-1, color);

    // if over line, half-draw
    /// top
    mat.fillRect(
        x+1,
        y,
        width-1,
        1,
        rgb(
            color.r * top_multiplier,
            color.g * top_multiplier,
            color.g * top_multiplier
        )
    );

    /// bottom
    mat.fillRect(
        x,
        y + height,
        width,
        1,
        rgb(
            color.r * bottom_multiplier,
            color.g * bottom_multiplier,
            color.g * bottom_multiplier
        )
    );

    /// left
    mat.fillRect(
        x,
        y+1,
        1,
        height-1,
        rgb(
            color.r * left_multiplier,
            color.g * left_multiplier,
            color.g * left_multiplier
        )
    );

    /// right
    mat.fillRect(
        x + width,
        y,
        1,
        height,
        rgb(
            color.r * right_multiplier,
            color.g * right_multiplier,
            color.g * right_multiplier
        )
    );

    // upper left pixel
    mat.drawPixel(
        x,
        y,
        rgb(
            color.r * min(left_multiplier, top_multiplier),
            color.g * min(left_multiplier, top_multiplier),
            color.g * min(left_multiplier, top_multiplier)
        )
    );
}
