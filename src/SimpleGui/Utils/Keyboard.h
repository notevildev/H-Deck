#pragma once

//
// Created by rondotdll on 9/21/25.
//

namespace Keyboard {

#define BACKSPACE 0x08
#define ENTER 0x0D
#define SPACE 0x20
#define UPPER_DOLLARSIGN 0x04

  uint8_t wire_address = 0x55;

  char readKey();

}
