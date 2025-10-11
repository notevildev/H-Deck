//
// Created by rondotdll on 9/24/25.
//

#include <Esp.h>

#include "TTrackball.h"

namespace SGui {
namespace HID {
  /* Attach interrupts to trackball pins */
void TTrackball::AttachInterrupts() const {
  if (initialized_) return; // no duplicate interrupts!

  //////////////////////////////////////////////
  //////////////// TRACKBALL_UP ////////////////
  //////////////////////////////////////////////
  attachInterruptArg(TRACKBALL_UP_P, [](void* arg) {
    auto self = static_cast<TTrackball*>(arg);

    self->input_event_queue_->push(
      {
        .type=DPAD_PRESSED,
        .id=DPAD_UP
      }
    );

#ifdef DEBUG
    Serial.println("Trackball Scrolled Up");
#endif
  },
  const_cast<TTrackball*>(this),
  FALLING);


  ////////////////////////////////////////////////
  //////////////// TRACKBALL_DOWN ////////////////
  ////////////////////////////////////////////////
  attachInterruptArg(TRACKBALL_DOWN_P, [](void* arg) {
    auto self = static_cast<TTrackball*>(arg);

    self->input_event_queue_->push(
      {
        .type=DPAD_PRESSED,
        .id=DPAD_DOWN
      }
    );

#ifdef DEBUG
    Serial.println("Trackball Scrolled Down");
#endif
  },
  const_cast<TTrackball*>(this),
  FALLING);

  ////////////////////////////////////////////////
  //////////////// TRACKBALL_LEFT ////////////////
  ////////////////////////////////////////////////
  attachInterruptArg(TRACKBALL_LEFT_P, [](void* arg) {
    auto self = static_cast<TTrackball*>(arg);

    self->input_event_queue_->push(
      {
        .type=DPAD_PRESSED,
        .id=DPAD_LEFT
      }
    );

#ifdef DEBUG
    Serial.println("Trackball Scrolled Left");
#endif
  },
  const_cast<TTrackball*>(this),
  FALLING);

  ////////////////////////////////////////////////
  //////////////// TRACKBALL_RIGHT ////////////////
  ////////////////////////////////////////////////
  attachInterruptArg(TRACKBALL_RIGHT_P, [](void* arg) {
    auto self = static_cast<TTrackball*>(arg);

    self->input_event_queue_->push(
      {
        .type=DPAD_PRESSED,
        .id=DPAD_LEFT
      }
    );

  #ifdef DEBUG
    Serial.println("Trackball Scrolled Right");
  #endif
  },
  const_cast<TTrackball*>(this),
  FALLING);

  ////////////////////////////////////////////////
  //////////////// TRACKBALL_BUTTON ////////////////
  ////////////////////////////////////////////////
  attachInterruptArg(TRACKBALL_BUTTON_P, [](void* arg) {
    auto self = static_cast<TTrackball*>(arg);

    self->input_event_queue_->push(
      {
        .type=DPAD_PRESSED,
        .id=DPAD_CENTER
      }
    );

  #ifdef DEBUG
    Serial.println("Trackball Button Pressed");
  #endif
  },
  const_cast<TTrackball*>(this),
  FALLING);

  initialized_ = true;
};
} // namespace HID
} // namespace SGui
