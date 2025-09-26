#pragma once

//
// Created by rondotdll on 9/22/25.
//

#include <functional>

#include <stdint.h>

#include "../Utils/managed_buffer.h"

namespace SGui {

/* Input types for input events
 * Commented lines are future implementations
 */
enum input_type_t {
  /* Individual inputs (Buttons, Switches) */
  // BASIC_DOWN,
  // BASIC_UP,
  BASIC_PRESSED,

  /* Directional inputs (D-Pad, Joystick, Trackball) */
  // DPAD_DOWN,
  // DPAD_UP,
  DPAD_PRESSED,

  /* Keyboard inputs (Keypad, Full Keyboard) */
  KEY_DOWN,
  KEY_UP,
  KEY_PRESSED,

  /* Touchscreen inputs (TFT Display) */
  // TOUCH,
};

/* Stores specific input events (type & ID)
 */
struct input_event_t {
  input_type_t type;
  uint8_t id;

  uint16_t flatten() const {
    return (static_cast<uint16_t>(this->type) << 8) | this->id;
  };
};

/* Event handler execution statuses
 * COMPLETE    - event was handled completely successfully
 * INCOMPLETE  - event was partially handled (e.g. event needs elevated to higher scope)
 * NO_HANDLER  - failed to find a suitable handler for the input event
 * BAD_HANDLER - handler found, but failed during execution
 */
enum handler_status_t {
  COMPLETE,
  INCOMPLETE,
  NO_HANDLER,
  BAD_HANDLER,
};


typedef void(*event_handler_t)();

typedef managed_buffer<input_event_t, 16> input_event_queue_t;

}
