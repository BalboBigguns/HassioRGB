#pragma once
#ifndef _RGB_
#define _RGB_

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino
#include "config.h"

#define COLOR_TEMP_HA_MIN_IN_MIRED 154 // Home Assistant minimum color temperature
#define COLOR_TEMP_HA_MAX_IN_MIRED 500 // Home Assistant maximum color temperature
#define COLOR_TEMP_MIN_IN_KELVIN 1000  // minimum color temperature
#define COLOR_TEMP_MAX_IN_KELVIN 15000 // maximum color temperature

typedef struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

#define EFFECT_NOT_DEFINED_NAME "None"
#define EFFECT_RAINBOW_NAME "Rainbow"
#define EFFECT_SLOW_RAINBOW_NAME "Slow Rainbow"
#define EFFECT_RAINBOW_DELAY 10
#define EFFECT_SLOW_RAINBOW_DELAY 30

#define EFFECT_LIST EFFECT_RAINBOW_NAME

enum EFFECT
{
    EFFECT_NOT_DEFINED,
    EFFECT_RAINBOW,
    EFFECT_SLOW_RAINBOW
};

// rainbow lookup
const uint8_t lights[360] = {
    0, 0, 0, 0, 0, 1, 1, 2,
    2, 3, 4, 5, 6, 7, 8, 9,
    11, 12, 13, 15, 17, 18, 20, 22,
    24, 26, 28, 30, 32, 35, 37, 39,
    42, 44, 47, 49, 52, 55, 58, 60,
    63, 66, 69, 72, 75, 78, 81, 85,
    88, 91, 94, 97, 101, 104, 107, 111,
    114, 117, 121, 124, 127, 131, 134, 137,
    141, 144, 147, 150, 154, 157, 160, 163,
    167, 170, 173, 176, 179, 182, 185, 188,
    191, 194, 197, 200, 202, 205, 208, 210,
    213, 215, 217, 220, 222, 224, 226, 229,
    231, 232, 234, 236, 238, 239, 241, 242,
    244, 245, 246, 248, 249, 250, 251, 251,
    252, 253, 253, 254, 254, 255, 255, 255,
    255, 255, 255, 255, 254, 254, 253, 253,
    252, 251, 251, 250, 249, 248, 246, 245,
    244, 242, 241, 239, 238, 236, 234, 232,
    231, 229, 226, 224, 222, 220, 217, 215,
    213, 210, 208, 205, 202, 200, 197, 194,
    191, 188, 185, 182, 179, 176, 173, 170,
    167, 163, 160, 157, 154, 150, 147, 144,
    141, 137, 134, 131, 127, 124, 121, 117,
    114, 111, 107, 104, 101, 97, 94, 91,
    88, 85, 81, 78, 75, 72, 69, 66,
    63, 60, 58, 55, 52, 49, 47, 44,
    42, 39, 37, 35, 32, 30, 28, 26,
    24, 22, 20, 18, 17, 15, 13, 12,
    11, 9, 8, 7, 6, 5, 4, 3,
    2, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

class RGBled
{
public:
    RGBled(void);

    void init(void);
    void loop(void);

    bool getState(void);
    bool setState(bool p_state);

    uint8_t getBrightness(void);
    bool setBrightness(uint8_t p_brightness);

    Color getColor(void);
    bool setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue);

    bool setWhite(uint8_t p_white);

    uint16_t getColorTemperature(void);
    bool setColorTemperature(uint16_t p_colorTemperature);

    bool setEffect(const char *p_effect);

private:
    bool m_state;
    uint8_t m_brightness;
    Color m_color;
    uint16_t m_colorTemperature;

    bool setColor();

    void sineLED(int angle);
};

#endif