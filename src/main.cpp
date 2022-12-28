#include "Burst.h"

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <random>

TFT_eSPI tft = TFT_eSPI();
std::default_random_engine rand_engine;
std::uniform_int_distribution<uint_fast16_t> dist_x(0, 134);
std::uniform_int_distribution<uint_fast16_t> dist_y(0, 239);
std::uniform_int_distribution<uint16_t> dist_colour(0, 0xffff);

unsigned int time_counter = 0;

uint_fast16_t rand_x()
{
    return dist_x(rand_engine);
}

uint_fast16_t rand_y()
{
    return dist_y(rand_engine);
}

uint_fast16_t rand_colour()
{
    return dist_colour(rand_engine);
}

const unsigned int MAX_BURSTS = 5;
Burst bursts[MAX_BURSTS];
unsigned int nbursts = 0;

void setup()
{
    // Init display.
    tft.init();
    tft.fillScreen(TFT_BLACK);

    // Init serial port.
    Serial.begin(115200);
}

void loop()
{
    if ((nbursts < MAX_BURSTS) && (time_counter == 0))
    {
        uint_fast16_t x = rand_x();
        uint_fast16_t y = rand_y();
        uint16_t colour = rand_colour();
        bursts[nbursts] = Burst(&tft, x, y, colour, TFT_BLACK, 50);
        nbursts += 1;
    }
    for (unsigned int k = 0; k < nbursts; ++k)
    {
        if (bursts[k].update_and_redraw() == 0)
        {
            uint_fast16_t x = rand_x();
            uint_fast16_t y = rand_y();
            uint16_t colour = rand_colour();
            bursts[k] = Burst(&tft, x, y, colour, TFT_BLACK, 50);
        }
    }
    time_counter = (time_counter + 1) & 0x1f;
    delay(50);
}
