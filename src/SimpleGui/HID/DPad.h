#pragma once

//
// Created by rondotdll on 9/22/25.
//

#include "../Types/input.h"

#include <stdint.h>

namespace SGui {
namespace HID {

enum dpad_input_id_t: uint8_t {
  DPAD_CENTER = 16, // XXX10000
  DPAD_UP = 8,      // XXX01000
  DPAD_DOWN = 4,    // XXX00100
  DPAD_LEFT = 2,    // XXX00010
  DPAD_RIGHT = 1,   // XXX00001
};

typedef handler_status_t (*dpad_handler_t)(dpad_input_id_t);

/* Joystick cardinal direction input IDs
 *
 * IDs use are bitmasks, allowing support for diagonal inputs.
 * For example, UP + RIGHT = 00001001 = 9
 * First 3 bits are unused, then in order from left to right:
 * Center, Right, Left, Down, Up
 */

class DPad {
protected:
  input_event_queue_t* input_event_queue_;

  dpad_handler_t onPress_ = nullptr;
  dpad_handler_t onRelease_ = nullptr;

public:
  explicit DPad(input_event_queue_t* input_event_queue) {
    this->input_event_queue_ = input_event_queue;
  }

  /* Bind a function to the onPress event

   * This handler will execute as soon as the event is recognized,
   * prior to passing it up to the input_event_queue. Return `COMPLETE`
   * to prevent it from being passed to input_event_queue.
   */
  void onPress(dpad_handler_t handler);

  /* Bind a function to the onRelease event

   * This handler will execute as soon as the event is recognized,
   * prior to passing it up to the input_event_queue. Return `COMPLETE`
   * to prevent it from being passed to input_event_queue.
   */
  void onRelease(dpad_handler_t handler);


  virtual void Init() = 0;
};

} // namespace HID
} // namespace SGui