#pragma once

#include "../Types/input.h"

//
// Created by rondotdll on 9/21/25.
//

namespace SGui {
namespace HID {

typedef handler_status_t (*keyboard_handler_t)(char);

enum SpecialKey: char {
  BACKSPACE = 0x08,
  ENTER = 0x0D,
  SPACE = 0x20,
  UPPER_DOLLARSIGN = 0x04,
};

class Keyboard {
 protected:
  input_event_queue_t* input_event_queue_;

  keyboard_handler_t onKeyUp_ = nullptr;
  keyboard_handler_t onKeyDown_ = nullptr;
  keyboard_handler_t onKeyPress_ = nullptr;

  virtual void HandleEvent(input_event_t, char);

 public:
  explicit Keyboard(input_event_queue_t* input_event_queue) {
    this->input_event_queue_ = input_event_queue;
  };
  virtual ~Keyboard() = default;

  /* Check if the keyboard has been successfully initialized and is ready for use
   */
  virtual bool Ready() const { return true; }

  /* Bind a function to the onKey event

   * This handler will execute as soon as the event is recognized,
   * prior to passing it up to the input_event_queue. Return `COMPLETE`
   * to prevent it from being passed to input_event_queue.
   */
  void onKey(keyboard_handler_t handler);

  /* Bind a function to the onKeyPress (key down) event

   * This handler will execute as soon as the event is recognized,
   * prior to passing it up to the input_event_queue. Return `COMPLETE`
   * to prevent it from being passed to input_event_queue.
   */
  void onKeyPress(keyboard_handler_t handler);

  /* Bind a function to the onKeyRelease (key up) event

   * This handler will execute as soon as the event is recognized,
   * prior to passing it up to the input_event_queue. Return `COMPLETE`
   * to prevent it from being passed to input_event_queue.
   */
  void onKeyRelease(keyboard_handler_t handler);
};

}  // namespace HID
}  // namespace SGui
