#include "TKeyboard.h"
//
// Created by David J on 9/21/25.
//

#include <task.h>
#include <Wire.h>

#include <FreeRTOS.h>


namespace SGui {
namespace HID {

void TKeyboard::Enable() {
  // if (keyboard_ready_) return;
  bool ready = false;

  // Verify peripheral power is enabled
  pinMode(POWER_ON_P, OUTPUT);
  digitalWrite(POWER_ON_P, HIGH);

  // Pause to let keyboard "boot"
  delay(500);

  // Verify I2C is initialized
  Wire.begin(I2C_SDA_P, I2C_SCL_P);

  // Verify the keyboard initializes properly
  while (!ready) {
    Wire.requestFrom(this->wire_address_, (uint8_t)1); // compiler requires explicit cast for some reason
    if (Wire.read() == -1) {
#ifdef DEBUG
      Serial.println("Waiting for keyboard...");
#endif

      delay(500);
      continue;
    }
    ready = true;
  }
  // Set the default backlight brightness level.
  setKeyboardBacklight(127, true);
}

char TKeyboard::readKey() {
  char keyValue = 0x00;
  Wire.requestFrom(this->wire_address_, (uint8_t)1);

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

/* Dynamically modify backlight brightness at runtime
 *
 * Setting persist to true will set the default backlight brightness level. If
 * the user sets the backlight to 0 via setKeyboardBrightness, the default
 * brightness is used when pressing ALT+B, rather than the backlight brightness
 * level set by the user. This ensures that pressing ALT+B can respond to the
 * backlight being turned on and off normally.
 *
 * Brightness Range: 30 ~ 255
 */
void TKeyboard::setKeyboardBacklight(uint8_t brightness, bool persist) const {
  Wire.beginTransmission(this->wire_address_);
  Wire.write(persist ? 0x02 : 0x01); // 0x02 sets the default brightness
  Wire.write(brightness);
  Wire.endTransmission();
}

void TKeyboard::Init() {
    this->Enable();

    xTaskCreatePinnedToCore(
      [](void* arg) {
        TKeyboard* self = (TKeyboard*)arg;

        for (;;) {
          char key;
          while ((key = self->readKey()) != 0) {

            // handle native keypress event (if it exists)
            if (self->onKeyPress_) {
              if (self->onKeyPress_(key) == COMPLETE /* (COMPLETE) */) {
                continue; // skip adding to input queue if event was handled completely
              }
            }

            self->input_event_queue_->push(
              input_event_t{
                .type=KEY_PRESSED,
                .id=(uint16_t)key}
            );
          }
          vTaskDelay(pdMS_TO_TICKS(10)); // sleep for 10ms
        }
      },
      "keyboard_reader",
      2048,
      this,
      1,
      &this->tkb_poll_task_,
      APP_CPU_NUM
    );
}

}  // namespace HID
}  // namespace SGui
