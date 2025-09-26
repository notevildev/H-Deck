#pragma once

//
// Created by rondotdll on 9/24/25.
//

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


public:
  /* Initialize the Trackball pull-up resistors and interrupts

   * The T-Deck trackball is a little finicky when it comes to manually
   * polling for input, so we're using interrupts instead. As a benefit,
   * this is also notably more efficient
   */
  void Init() override;
};

} // namespace HID
} // namespace SGui
