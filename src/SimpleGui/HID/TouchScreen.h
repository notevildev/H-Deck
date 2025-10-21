#pragma once

#include "../Types/Input.h"
#include "../Types/UIGeometry.h"

//
// Created by rondotdll on 9/22/25.
//

namespace SGui {
namespace HID {

typedef handler_status_t (*touch_handler_t)(UIPoint);

enum SpecialKey: char {
  BACKSPACE = 0x08,
  ENTER = 0x0D,
  SPACE = 0x20,
  UPPER_DOLLARSIGN = 0x04,
};

class TouchScreen {
protected:
  input_event_queue_t* input_event_queue_;

  touch_handler_t onPress = nullptr;

  virtual void HandleEvent(input_event_t, char);

public:
  explicit TouchScreen(input_event_queue_t* input_buffer) {;
    this->input_event_queue_ = input_buffer;
  };

  // Should create / start any tasks related to keyboard input polling
  virtual void Init() = 0;
};
}  // namespace HID
}  // namespace SGui
