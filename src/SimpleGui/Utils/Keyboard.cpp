//
// Created by rondotdll on 9/21/25.
//

#include <Wire.h>

#include "Keyboard.h"

namespace Keyboard {
char readKey() {
  char keyValue = 0x00;
  Wire.requestFrom(Keyboard::wire_address, (uint8_t)1);

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