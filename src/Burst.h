#ifndef BURST_H
#define BURST_H

#include <TFT_eSPI.h>
#include <random>

const uint_fast16_t INITIAL_SPEED_Y = -64;
const uint_fast16_t GRAVITY_ACCELERATION = 8;

class Burst
{
private:
    static std::default_random_engine rand_engine;
    static std::uniform_int_distribution<uint_fast16_t> dist_flicker;
    TFT_eSPI* tft;
    uint16_t colour;
    uint16_t background_colour;
    uint16_t remaining_time;
    // We use fast fixed-point arithmetic. The upper 10 bits are the integral
    // part. We use 2s-complement, so -1/2 = b1111_1111_1110_0000.
    uint_fast16_t pos_x[32];
    uint_fast16_t pos_y[32];
    uint_fast16_t speed_x[32];
    uint_fast16_t speed_y[32];
    static uint_fast16_t rng_flicker();

public:
    Burst();
    Burst(TFT_eSPI* tft, uint_fast16_t start_x, uint_fast16_t start_y,
          uint16_t colour, uint16_t background_colour, uint16_t glow_time);
    uint16_t update_and_redraw();
};

#endif
