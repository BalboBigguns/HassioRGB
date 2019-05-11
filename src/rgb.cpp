#include "rgb.h"

#define RED_PIN 3
#define GREEN_PIN 16
#define BLUE_PIN 4

volatile uint8_t effect = EFFECT_NOT_DEFINED;

///////////////////////////////////////////////////////////////////////////
//   CONSTRUCTOR, INIT() AND LOOP()
///////////////////////////////////////////////////////////////////////////
RGBled::RGBled(void)
{
    analogWriteRange(255);

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void RGBled::init(void)
{
    m_state = false;
    m_brightness = 255;
    m_color.red = 255;
    m_color.green = 255;
    m_color.blue = 255;
}

void RGBled::loop(void)
{
    switch (effect) {
    case EFFECT_NOT_DEFINED:
        break;
    case EFFECT_RAINBOW:
        static unsigned short count = 0;
        static unsigned long last = millis();
        if (millis() - last > EFFECT_RAINBOW_DELAY) {
            last = millis();
            sineLED(count++);
            count %= 360;
        }
        break;
    case EFFECT_SLOW_RAINBOW:
        static unsigned short s_count = 0;
        static unsigned long s_last = millis();
        if (millis() - s_last > EFFECT_SLOW_RAINBOW_DELAY) {
            s_last = millis();
            sineLED(s_count++);
            s_count %= 360;
        }
        break;
    }
}

///////////////////////////////////////////////////////////////////////////
//   STATE
///////////////////////////////////////////////////////////////////////////
bool RGBled::getState(void)
{
    return m_state;
}

bool RGBled::setState(bool p_state)
{
    // checks if the given state is different from the actual state
    if (p_state == m_state) {
        return false;
    }

    if (p_state) {
        m_state = true;
        setColor();
    }
    else {
        m_state = false;
        analogWrite(RED_PIN, 0);
        analogWrite(GREEN_PIN, 0);
        analogWrite(BLUE_PIN, 0);
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////
//   BRIGHTNESS
///////////////////////////////////////////////////////////////////////////
uint8_t RGBled::getBrightness(void)
{
    return m_brightness;
}

bool RGBled::setBrightness(uint8_t p_brightness)
{
    // checks if the value is smaller, bigger or equal to the actual brightness value
    if (p_brightness < 0 || p_brightness > 255 || p_brightness == m_brightness) {
        return false;
    }

    // saves the new brightness value
    if (p_brightness < 4) {
        m_brightness = 4;
    }
    else {
        m_brightness = p_brightness;
    }

    return setColor();
}

///////////////////////////////////////////////////////////////////////////
//   RGB COLOR
///////////////////////////////////////////////////////////////////////////
Color RGBled::getColor(void)
{
    return m_color;
}

bool RGBled::setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue)
{
    if ((p_red < 0 || p_red > 255) || (p_green < 0 || p_green > 255) || (p_blue < 0 || p_blue > 255)) {
        return false;
    }

    // saves the new values
    m_color.red = p_red;
    m_color.green = p_green;
    m_color.blue = p_blue;

    return setColor();
}

bool RGBled::setColor()
{
    // sets the new color
    analogWrite(RED_PIN, map(m_color.red, 0, 255, 0, m_brightness));
    analogWrite(GREEN_PIN, map(m_color.green, 0, 255, 0, m_brightness));
    analogWrite(BLUE_PIN, map(m_color.blue, 0, 255, 0, m_brightness));

    return true;
}

///////////////////////////////////////////////////////////////////////////
//   COLOR TEMPERATURE
///////////////////////////////////////////////////////////////////////////
uint16_t RGBled::getColorTemperature(void)
{
    return m_colorTemperature;
}

bool RGBled::setColorTemperature(uint16_t p_colorTemperature)
{
    // checks if the value is equal to the actual color temperature
    if (p_colorTemperature < COLOR_TEMP_HA_MIN_IN_MIRED || p_colorTemperature == m_colorTemperature || p_colorTemperature > COLOR_TEMP_HA_MAX_IN_MIRED) {
        return false;
    }

    // saves the new color temperature
    m_colorTemperature = p_colorTemperature;

    unsigned char green = map(p_colorTemperature, COLOR_TEMP_HA_MAX_IN_MIRED, COLOR_TEMP_HA_MIN_IN_MIRED, 64, 255);

    if (green < 128) {
        setColor(255, green, 0);
    }
    else {
        setColor(255, green, map(green, 128, 255, 0, 255));
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////
//   EFFECTS
///////////////////////////////////////////////////////////////////////////
bool RGBled::setEffect(const char *p_effect)
{
    if (strcmp(p_effect, EFFECT_NOT_DEFINED_NAME) == 0) {
        effect = EFFECT_NOT_DEFINED;
        return true;
    }
    else if (strcmp(p_effect, EFFECT_RAINBOW_NAME) == 0) {
        effect = EFFECT_RAINBOW;
        return true;
    }
    else if (strcmp(p_effect, EFFECT_SLOW_RAINBOW_NAME) == 0) {
        effect = EFFECT_SLOW_RAINBOW;
        return true;
    }

    return false;
}

// sine wave rainbow
void RGBled::sineLED(int angle)
{
    setColor(lights[(angle + 120) % 360], lights[angle], lights[(angle + 240) % 360]);
}