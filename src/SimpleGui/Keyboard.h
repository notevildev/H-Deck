#pragma once
//
// Created by David J on 9/10/25.
//

#include "pins.h"

#include <Wire.h>
#include <Arduino.h>

namespace SGui {

#define KEYBOARD_I2C_ADDR 0x55

  class Keyboard {
  protected:
    bool initialized_;
    uint16_t keyboard_i2c_address_;

  public:
    /*
    * Dynamically modify backlight brightness at runtime
    * Brightness Range: 0 ~ 255
    * */
    void setBacklightBrightness(uint8_t value, bool persist = false);

    // Initialize the keyboard (must be called before use)
    void Init();

    // Read a single keypress from the T-Deck
    char readKey();

    explicit Keyboard(uint16_t keyboard_address = KEYBOARD_I2C_ADDR) {
      this->initialized_ = false;
      this->keyboard_i2c_address_ = keyboard_address;
    }

    ~Keyboard() {
      if (this->initialized_) {
        setBacklightBrightness(0);
      }
    }

  };
}
