#include "Burst.h"

/**
 * Fixed-point values of sin(0), sin(pi/16), sin(2*pi/16), ..., sin(8*pi/16).
 */
uint_fast16_t SIN_TABLE[9] = {
    0, 12, 24, 36, 45, 53, 59, 63, 64};

Burst::Burst(TFT_eSPI* tft, uint_fast16_t start_x, uint_fast16_t start_y,
             uint16_t colour, uint16_t background_colour, uint16_t glow_time) : tft(tft), colour(colour), background_colour(background_colour), remaining_time(glow_time)
{
    for (uint_fast8_t k = 0; k < 32; ++k)
    {
        pos_x[k] = start_x << 6;
        pos_y[k] = start_y << 6;
        if (k < 8)
        {
            speed_x[k] = SIN_TABLE[8 - k];
            speed_y[k] = SIN_TABLE[k];
        }
        else if (k < 16)
        {
            speed_x[k] = -SIN_TABLE[k - 8];
            speed_y[k] = SIN_TABLE[16 - k];
        }
        else if (k < 24)
        {
            speed_x[k] = -SIN_TABLE[24 - k];
            speed_y[k] = -SIN_TABLE[k - 16];
        }
        else
        {
            speed_x[k] = SIN_TABLE[k - 24];
            speed_y[k] = -SIN_TABLE[32 - k];
        }
        speed_y[k] += INITIAL_SPEED_Y;
    }
}

Burst::Burst() {
}

std::default_random_engine Burst::rand_engine = std::default_random_engine();

std::uniform_int_distribution<uint_fast16_t> Burst::dist_flicker = std::uniform_int_distribution<uint_fast16_t>(0, 3);

uint_fast16_t Burst::rng_flicker() {
    return dist_flicker(rand_engine);
}

/**
 * Draw over the current position of the dots with the back ground colour,
 * update the positions, draw dots in the new positions.
 * @return remaining time
 */
uint16_t Burst::update_and_redraw()
{
    for (uint_fast8_t k = 0; k < 32; ++k)
    {
        tft->drawPixel(pos_x[k] >> 6, pos_y[k] >> 6, background_colour);
    }
    if (remaining_time > 0)
    {
        for (uint_fast8_t k = 0; k < 32; ++k)
        {
            pos_x[k] += speed_x[k];
            pos_y[k] += speed_y[k];
            if(rng_flicker() != 0) {
                tft->drawPixel(pos_x[k] >> 6, pos_y[k] >> 6, colour);
            }
            speed_y[k] += GRAVITY_ACCELERATION;
        }
        remaining_time -= 1;
        return remaining_time + 1;
    }
    return 0;
}
