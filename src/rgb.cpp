#include "rgb.h"

#define RED_PIN   3
#define GREEN_PIN 16
#define BLUE_PIN  4

volatile uint8_t effect = EFFECT_NOT_DEFINED;

///////////////////////////////////////////////////////////////////////////
//   CONSTRUCTOR, INIT() AND LOOP()
///////////////////////////////////////////////////////////////////////////
RGBled::RGBled(void) {
  analogWriteRange(255);

  pinMode(RED_PIN, OUTPUT);  
  pinMode(GREEN_PIN, OUTPUT);  
  pinMode(BLUE_PIN, OUTPUT); 
}

void RGBled::init(void) {
  m_state = false;
  m_brightness = 255;
  m_color.red = 255;
  m_color.green = 255;
  m_color.blue = 255;
}

void RGBled::loop(void) {
  // TODO: handles effects
  switch(effect) {
    case EFFECT_NOT_DEFINED:
      break;
    case EFFECT_RAINBOW:
      static unsigned char count = 0;
      static unsigned long last = millis();
      if (millis() - last > EFFECT_RAINBOW_DELAY) {
        last = millis();
        rainbowEffect(count++);
        count %= 256;
      }
      break;
  }
}

///////////////////////////////////////////////////////////////////////////
//   STATE
///////////////////////////////////////////////////////////////////////////
bool RGBled::getState(void) {
  return m_state;
}

bool RGBled::setState(bool p_state) {
  // checks if the given state is different from the actual state
  if (p_state == m_state)
    return false;
  
  if (p_state) {
    m_state = true;
    setColor();
  } else {
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
uint8_t RGBled::getBrightness(void) {
  return m_brightness;
}

bool RGBled::setBrightness(uint8_t p_brightness) {
  // checks if the value is smaller, bigger or equal to the actual brightness value
  if (p_brightness < 0 || p_brightness > 255 || p_brightness == m_brightness)
    return false;

  // saves the new brightness value
  m_brightness = p_brightness;

  return setColor();
}

///////////////////////////////////////////////////////////////////////////
//   RGB COLOR
///////////////////////////////////////////////////////////////////////////
Color RGBled::getColor(void) {
  return m_color;
}

bool RGBled::setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue) {
  if ((p_red < 0 || p_red > 255) || (p_green < 0 || p_green > 255) || (p_blue < 0 || p_blue > 255))
    return false;

  // saves the new values
  m_color.red = p_red;
  m_color.green = p_green;
  m_color.blue = p_blue;

  return setColor();
}

bool RGBled::setColor() {
  // sets the new color
  analogWrite(RED_PIN, map(m_color.red, 0, 255, 0, m_brightness));
  analogWrite(GREEN_PIN, map(m_color.green, 0, 255, 0, m_brightness));
  analogWrite(BLUE_PIN, map(m_color.blue, 0, 255, 0, m_brightness));

  return true;
}

///////////////////////////////////////////////////////////////////////////
//   COLOR TEMPERATURE
///////////////////////////////////////////////////////////////////////////
uint16_t RGBled::getColorTemperature(void) {
  return m_colorTemperature;
}

bool RGBled::setColorTemperature(uint16_t p_colorTemperature) {
  // checks if the value is equal to the actual color temperature
  if (p_colorTemperature < COLOR_TEMP_HA_MIN_IN_MIRED || p_colorTemperature == m_colorTemperature || p_colorTemperature > COLOR_TEMP_HA_MAX_IN_MIRED)
    return false;
    
  // saves the new colour temperature
  m_colorTemperature = p_colorTemperature;

  // https://fr.wikipedia.org/wiki/Mired
  // http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
  // M = 1000000 / T <> T [kelvin] = 1000000 / M [mired]
      int tmpKelvin = 1000000 / m_colorTemperature;
  
      if (tmpKelvin < 1000) {
        tmpKelvin = 1000;
      } else if (tmpKelvin > 40000) {
        tmpKelvin = 40000;
      }

  //int tmpKelvin = map(p_colorTemperature, COLOR_TEMP_HA_MIN_IN_MIRED, COLOR_TEMP_HA_MAX_IN_MIRED, COLOR_TEMP_MAX_IN_KELVIN, COLOR_TEMP_MIN_IN_KELVIN);
  tmpKelvin = tmpKelvin / 100;

  // computes red
  if (tmpKelvin <= 66) {
    m_color.red = 255;
  } else {
    float red = tmpKelvin - 60;
    red = 329.698727446 * pow(red, -0.1332047592);
    if (red < 0) {
      m_color.red = 0;
    } else if (red > 255) {
      m_color.red = 255;
    } else {
      m_color.red = red;
    }
  }

  // computes green
  if (tmpKelvin <= 66) {
    float green = tmpKelvin;
    green = 99.4708025861 * log(green) - 161.1195681661;
    if (green < 0) {
      m_color.green = 0;
    } else if (green > 255) {
      m_color.green = 255;
    } else {
      m_color.green = green;
    }
  } else {
    float green = tmpKelvin - 60;
    green = 288.1221695283 * pow(green, -0.0755148492);
    if (green < 0) {
      m_color.green = 0;
    } else if (green > 255) {
      m_color.green = 255;
    } else {
      m_color.green = green;
    }
  }

  // computes blue
  if (tmpKelvin <= 66) {
    m_color.blue = 255;
  } else {
    if (tmpKelvin <= 19) {
      m_color.blue = 0;
    } else {
      float blue = tmpKelvin - 10;
      blue = 138.5177312231 * log(blue) - 305.0447927307;
      if (blue < 0) {
        m_color.blue = 0;
      } else if (blue > 255) {
        m_color.blue = 255;
      } else {
        m_color.blue = blue;
      }
    }
  }
  
  return setColor();
}

///////////////////////////////////////////////////////////////////////////
//   EFFECTS
///////////////////////////////////////////////////////////////////////////
bool RGBled::setEffect(const char* p_effect) {
  if (strcmp(p_effect, EFFECT_NOT_DEFINED_NAME) == 0) {
    effect = EFFECT_NOT_DEFINED;
    return true;
  } else if (strcmp(p_effect, EFFECT_RAINBOW_NAME) == 0) {
    effect = EFFECT_RAINBOW;
    return true;
  }

  return false;
}

// https://github.com/xoseperez/my9291/blob/master/examples/esp8285/esp8285_rainbow.cpp
void RGBled::rainbowEffect(uint8_t p_index) {
  if (p_index < 85) {
    setColor(p_index * 3, 255 - p_index * 3, 0);
  } else if (p_index < 170) {
    p_index -= 85;
    setColor(255 - p_index * 3, 0, p_index * 3);
  } else {
    p_index -= 170;
    setColor(0, p_index * 3, 255 - p_index * 3);
  }
}