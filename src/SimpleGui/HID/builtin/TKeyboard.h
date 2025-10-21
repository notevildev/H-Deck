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

  static bool initialized_;

  // Read a single keypress from the T-Keyboard
  char readKey();

  // Enables & setups the keyboard software to begin polling
  // Called by Init()
  void Enable() const;

 public:
  explicit TKeyboard(input_event_queue_t* input_event_queue, uint8_t wire_address = 0x55) : Keyboard(input_event_queue) {
    tkb_poll_task_ = nullptr;
    this->wire_address_ = wire_address;

    this->Enable();

    xTaskCreatePinnedToCore(
      [](void* arg) {
        TKeyboard* self = (TKeyboard*)arg;

        for (;;) {
          char key;
          while ((key = self->readKey()) != 0) {

            // handle native keypress event (if it exists)
            if (self->onKeyPress_) {
              if (self->onKeyPress_(key) == COMPLETE /* (COMPLETE) */) {
                continue; // skip adding to input queue if event was handled completely
              }
            }

            self->input_event_queue_->push(
              input_event_t{
                .type=KEY_PRESSED,
                .id=(uint8_t)key}
            );
          }
          vTaskDelay(pdMS_TO_TICKS(10)); // sleep for 10ms
        }
      },
      "keyboard_reader",
      2048,
      this,
      1,
      &this->tkb_poll_task_,
      APP_CPU_NUM
    );

  }

  ~TKeyboard() {
    if (tkb_poll_task_) {
      vTaskDelete(tkb_poll_task_);
    }
  }

  bool Ready() const override { return initialized_; }

  void setKeyboardBacklight(uint8_t brightness, bool persist) const;

};

}  // namespace HID
}  // namespace SGui
