#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


#include <Arduino.h>

#include <TomThumb.h> // this should be in the adafruit-gfx library
#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

#include "games/flappy.hpp"
#include "games/display.hpp"
#include "pinout.h"


#define X 64
#define Y 32
#define N_PIXELS (X*Y)




int PIXEL_BUFFER[N_PIXELS] = {0};

RGB64x32MatrixPanel_I2S_DMA m;
flappy::Flappy game(m);


void setup(){
    matrix::reset(); // do this before matrix.begin

    Serial.begin(115200);

    // If you experience ghosting, you will need to reduce the brightness level, not all RGB Matrix
    // Panels are the same - some seem to display ghosting artefacts at lower brightness levels.
    // In the setup() function do something like:

    m.setPanelBrightness(3); // SETS THE BRIGHTNESS HERE. 60 OR LOWER IDEAL.
    m.begin(R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK); // setup the LED matrix

    m.setTextColor(matrix::rgb(255, 255, 255));
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
        delay(500);
        while (!Serial.available()) { delay(100); }

        Serial.read();
        game.reset();
        now = (double)millis() / 1000;
    }

    game.render();

    last = now;
}

