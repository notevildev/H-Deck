//
// Created by David J on 9/10/25.
//

#include "Keyboard.h"

namespace SGui {
   /* Dynamically modify backlight brightness at runtime
    *
    * Setting persist to true will set the default backlight brightness level. If
    * the user sets the backlight to 0 via setKeyboardBrightness, the default
    * brightness is used when pressing ALT+B, rather than the backlight brightness
    * level set by the user. This ensures that pressing ALT+B can respond to the
    * backlight being turned on and off normally.
    *
    * Brightness Range: 30 ~ 255
    * */
  void Keyboard::setBacklightBrightness(uint8_t value, bool persist) {
    Wire.beginTransmission(KEYBOARD_I2C_ADDR);
    Wire.write(persist ? 0x02 : 0x01); // 0x02 sets the default brightness
    Wire.write(value);
    Wire.endTransmission();
  }

// Initialize the keyboard (must be called before use)
  void Keyboard::Init() {
    if (initialized_) return;
    bool ready = false;

    // Verify peripheral power is enabled
    pinMode(POWER_ON_P, OUTPUT);
    digitalWrite(POWER_ON_P, HIGH);

    // Pause to let keyboard "boot"
    delay(500);

    // Verify I2C is initialized
    Wire.begin(I2C_SDA_P, I2C_SCL_P);

    // Verify the keyboard initializes properly
    while (!ready) {
      Wire.requestFrom((int)this->keyboard_i2c_address_, 1);
      if (Wire.read() == -1) {
        Serial.println("Waiting for keyboard...");
        delay(500);
        continue;
      }
      ready = true;
    }
    // Set the default backlight brightness level.
    setBacklightBrightness(127, true);
    this->initialized_ = true;
  }


  // Read a single keypress from the T-Deck
  char Keyboard::readKey() {
    char keyValue = 0;
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);

    while (Wire.available() > 0) {
      keyValue = Wire.read();
      if (keyValue != (char)0x00) {
        Serial.print("keyValue : ");
        Serial.println(keyValue);
      }
    }
    return keyValue;
  }
}