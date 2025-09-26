#pragma once

//
// Created by rondotdll on 9/24/25.
//


#include <FreeRTOS.h>

#include <task.h>
#include <Wire.h>

#include "../Keyboard.h"


#define I2C_SDA_P 18
#define I2C_SCL_P 8

#define POWER_ON_P 10

namespace SGui {
namespace HID {

class TKeyboard : public Keyboard {
 private:
  // internal task object
  TaskHandle_t tkb_poll_task_;

  // T-Keyboard's I2C address
  uint8_t wire_address_;

  // Read a single keypress from the T-Keyboard
  char readKey();

  // Enables & setups the keyboard software to begin polling
  // Called by Init()
  void Enable();

 public:
  explicit TKeyboard(input_event_queue_t* input_event_queue, uint8_t wire_address = 0x55): Keyboard(input_event_queue) {
    this->wire_address_ = wire_address;
  }

  ~TKeyboard() {
    if (tkb_poll_task_) {
      vTaskDelete(tkb_poll_task_);
    }
  }

  void setKeyboardBacklight(uint8_t brightness, bool persist) const;

  void Init() override;

};

}  // namespace HID
}  // namespace SGui
