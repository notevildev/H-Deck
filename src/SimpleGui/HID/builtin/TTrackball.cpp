//
// Created by rondotdll on 9/24/25.
//

#include <Esp.h>

#include "TTrackball.h"

namespace SGui {
namespace HID {

  void TTrackball::Init() {
    /* The trackball button is also the boot pin for the board,
     * which might cause issues down the road.
     *
     * poor design choice by LilyGo :/
     */

    pinMode(TRACKBALL_BUTTON_P, INPUT_PULLUP);

    /* set trackball pins to use pullup resistors
     * (required for reading input correctly)
     */
    pinMode(TRACKBALL_UP_P, INPUT_PULLUP);
    pinMode(TRACKBALL_DOWN_P, INPUT_PULLUP);
    pinMode(TRACKBALL_LEFT_P, INPUT_PULLUP);
    pinMode(TRACKBALL_RIGHT_P, INPUT_PULLUP);

    /* Attach interrupts to trackball pins */
    //////////////////////////////////////////////
    //////////////// TRACKBALL_UP ////////////////
    //////////////////////////////////////////////
    attachInterruptArg(TRACKBALL_UP_P, [](void* arg) {
      auto self = (TTrackball*)arg;

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
    this,
    FALLING);


    ////////////////////////////////////////////////
    //////////////// TRACKBALL_DOWN ////////////////
    ////////////////////////////////////////////////
    attachInterruptArg(TRACKBALL_DOWN_P, [](void* arg) {
      auto self = (TTrackball*)arg;

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
    this,
    FALLING);

    ////////////////////////////////////////////////
    //////////////// TRACKBALL_LEFT ////////////////
    ////////////////////////////////////////////////
    attachInterruptArg(TRACKBALL_LEFT_P, [](void* arg) {
      auto self = (TTrackball*)arg;

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
    this,
    FALLING);

    ////////////////////////////////////////////////
    //////////////// TRACKBALL_RIGHT ////////////////
    ////////////////////////////////////////////////
    attachInterruptArg(TRACKBALL_RIGHT_P, [](void* arg) {
      auto self = (TTrackball*)arg;

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
    this,
    FALLING);

    ////////////////////////////////////////////////
    //////////////// TRACKBALL_BUTTON ////////////////
    ////////////////////////////////////////////////
    attachInterruptArg(TRACKBALL_BUTTON_P, [](void* arg) {
      auto self = (TTrackball*)arg;

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
    this,
    FALLING);
  };


} // namespace HID
} // namespace SGui
