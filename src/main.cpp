#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Arduino.h>
#include <TomThumb.h> // this should be in the adafruit-gfx library
#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

#include "games/display.hpp"
#include "games/flappy.hpp"
#include "games/snake.hpp"
#include "pinout.h"


#define X 64
#define Y 32


// initialize game and matrix instances
RGB64x32MatrixPanel_I2S_DMA m;
snake::Snake game(m);


void setup(){
    // setup serial for debugging
    Serial.begin(115200);

    // setup matrix
    matrix::reset();
    m.setPanelBrightness(3); // SETS THE BRIGHTNESS HERE. 60 OR LOWER IDEAL.
    m.begin(R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK); // setup the LED matrix
}


double now, last = millis();
void loop()
{
    now = (double)millis() / 1000;

    // game finished
    if (!game.step(now - last))
    {
        game.render();
        game.game_over_screen();

        // wait for user to click continue
        delay(500);
        while (!Serial.available()) { delay(100); }
        Serial.read();

        // reset game status
        game.reset();
        now = (double)millis() / 1000;
    }

    game.render();

    last = now;
}

