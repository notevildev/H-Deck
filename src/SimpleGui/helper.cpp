#include <utility>
#include <TFT_eSPI.h>

#include "Types/Enums.h"
#include "helper.h"

#include <Wire.h>

#include "GuiManager.h"

namespace Keyboard {
  char readKey() {
    char keyValue = 0x00;
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);

    while (Wire.available() > 0) {
      keyValue = Wire.read();
      // verify that a key is actually pressed
      if (keyValue != (char)0x00) {
#ifdef DEBUG
        Serial.print("keyValue : ");
        Serial.printf("%c (%d)\n", keyValue, (int)keyValue);
#endif
        break;
      }
    }

    return keyValue;
  }
}

namespace SGui {

// Find the true edges of the screen (useful for drawing around a screen border)
// returns (width, height)
std::pair<int, int> GetScreenEdges() {
  int w = TFT_WIDTH;
  int h = TFT_HEIGHT;

  // if landscape orientation, invert width & height
  if (tft.getRotation() % 2 != 1) {
    w = TFT_HEIGHT;
    h = TFT_WIDTH;
  }

  return std::make_pair(w, h);
}

// Calculate the slope between two points
float slope(UIPoint p1, UIPoint p2) {
  return ((float)p2.y - (float)p1.y) / ((float)p2.x - (float)p1.x);
}

// Clear the screen to black
void clearScreen() {
  tft.fillRect(0, 0, tft.width(), tft.height(), TFT_BLACK);
}

}
