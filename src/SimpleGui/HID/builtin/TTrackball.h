#pragma once

//
// Created by rondotdll on 9/24/25.
//

#include <Esp.h>

#include "../DPad.h"

#define TRACKBALL_LEFT_P  1
#define TRACKBALL_RIGHT_P 2
#define TRACKBALL_UP_P    3
#define TRACKBALL_DOWN_P  15
#define TRACKBALL_BUTTON_P 0


namespace SGui {
namespace HID {

class TTrackball: public DPad {
private:
  static bool initialized_;

  void AttachInterrupts() const;

public:
  /* Initializes the Trackball pull-up resistors and interrupts

   * The T-Deck trackball is a little finicky when it comes to manually
   * polling for input, so we're using interrupts instead. As a benefit,
   * this is also notably more efficient
   */
  explicit TTrackball(input_event_queue_t* input_event_queue) : DPad(input_event_queue) {

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

    AttachInterrupts();
  };

  bool Ready() const override { return this->initialized_; }
};


} // namespace HID
} // namespace SGui
