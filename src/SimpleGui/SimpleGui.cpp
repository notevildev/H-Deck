#include "SimpleGui.h"

namespace SGui {
  // Nothing to see here...
  // Initialize the TFT display
  // returns a pointer to the TFT_eSPI object
   GUIManager* Init() {
    tft.init();
    tft.setRotation(1);
    clearScreen();

     //

    return new GUIManager();
  }
}