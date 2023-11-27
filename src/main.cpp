#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Arduino.h>
#include <TomThumb.h> // this should be in the adafruit-gfx library, but isn't
#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
#include <array>
#include <vector>

#include "games/controller.hpp"
#include "games/basegame.hpp"
#include "games/display.hpp"
#include "games/flappy.hpp"
#include "games/snake.hpp"
#include "games/menu.hpp"
#include "pinout.h"


#define X 64
#define Y 32


// initialize game and matrix instances
RGB64x32MatrixPanel_I2S_DMA m;
bool in_menu = true;


controller::Controller c(JX, JY, JB);

snake::Snake sn(m, c);
flappy::Flappy fl(m, c);


std::array<base::BaseGame *, 2> games = {&sn, &fl};


menu::Menu men(m, games);


void setup(){
    // setup serial for debugging
    Serial.begin(115200);

    // setup matrix
    matrix::reset();
    m.setPanelBrightness(60); // SETS THE BRIGHTNESS HERE. 60 OR LOWER IDEAL.
    m.begin(R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK); // setup the LED matrix
}


double now, last = millis();
void loop()
{
    now = (double)millis() / 1000;

    // game finished
    if (!men.getGame().step(now - last) || in_menu)
    {
        men.getGame().render();
        men.getGame().game_over_screen();

        // wait for user to click continue
        delay(500);
        uint8_t buttons = c.getButtons();
        while (!buttons)
        {
            delay(10);
            buttons = c.getButtons();
        }

        // go into menu
        bool in_settings = false;
        if (buttons & JOY_DEPRESS)
        {
            in_menu = true;
            men.reset();

            while (in_menu)
            {
                buttons = c.getButtons();

                if (in_settings)
                {
                    if (buttons & JOY_ARROWS)
                    {
                        if (!men.settingsPage(buttons))
                        {
                            in_settings = false;
                            men.reset();
                        }

                        delay(200);
                    }
                }
                else
                {
                    if (buttons & JOY_ARROWS)
                    {
                        if (buttons & JOY_LEFT)
                            men.prevPage();
                        
                        else if (buttons & JOY_RIGHT)
                            men.nextPage();
                        
                        else if (buttons & JOY_UP)
                            in_menu = false;
                        
                        else if (buttons & JOY_DOWN)
                            in_settings = true;
                        
                        delay(200);
                    }
                }
            }
        }

        // reset game status
        men.getGame().reset();
        now = (double)millis() / 1000;
    }

    men.getGame().render();

    last = now;
}

