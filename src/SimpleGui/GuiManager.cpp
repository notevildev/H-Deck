// File: GuiManager.cpp

#include "GuiManager.h"
#include "types.h"

#include <Wire.h>
#include <FreeRTOS.h>

#include <vector>
#include <unordered_map>


namespace SGui {

  GUIManager* GUIManager::self_ = nullptr; // <- satisfy the linker

  void GUIManager::initialize_keyboard() const {
    if (keyboard_ready_) return;
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
      Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);
      if (Wire.read() == -1) {
        Serial.println("Waiting for keyboard...");
        delay(500);
        continue;
      }
      ready = true;
    }
    // Set the default backlight brightness level.
    setKeyboardBacklight(127, true);

    keyboard_ready_ = true;
  }

  void GUIManager::enable_keyboard_input() const {
    // verify the keyboard is ready to use
    initialize_keyboard();

    xTaskCreatePinnedToCore(
      [](void* arg) {
        GUIManager* gui = (GUIManager*)arg;
        for (;;) {
          char key;
          while ((key = Keyboard::readKey()) != 0) {
            gui->create_input_event(input_event_t{.type=KEYBOARD, .id=(uint16_t)key});
          }
          vTaskDelay(pdMS_TO_TICKS(10)); // sleep for 10ms
        }
      },
      "keyboard_reader",
      2048,
      self_,
      1,
      &keyboard_task_,
      APP_CPU_NUM
    );

  }

  void GUIManager::enable_trackball_input() {
    // Verify pins are set up for input
    pinMode(TRACKBALL_UP_P, INPUT_PULLUP);
    pinMode(TRACKBALL_DOWN_P, INPUT_PULLUP);
    pinMode(TRACKBALL_LEFT_P, INPUT_PULLUP);
    pinMode(TRACKBALL_RIGHT_P, INPUT_PULLUP);

    attachInterrupt(TRACKBALL_UP_P, [] {
        self_->create_input_event(input_event_t{.type=TRACKBALL, .id=TRACKBALL_UP});
    }, RISING);
    attachInterrupt(TRACKBALL_DOWN_P, [] {
        self_->create_input_event(input_event_t{.type=TRACKBALL, .id=TRACKBALL_DOWN});
    }, RISING);
    attachInterrupt(TRACKBALL_LEFT_P, [] {
        self_->create_input_event(input_event_t{.type=TRACKBALL, .id=TRACKBALL_LEFT});
    }, RISING);
    attachInterrupt(TRACKBALL_RIGHT_P, [] {
        self_->create_input_event(input_event_t{.type=TRACKBALL, .id=TRACKBALL_RIGHT});
    }, RISING);
    attachInterrupt(TRACKBALL_PRESS_P, [] {
      self_->create_input_event(input_event_t{.type=TRACKBALL, .id=TRACKBALL_PRESS});
    }, RISING);
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
      * */
  void GUIManager::setKeyboardBacklight(uint8_t brightness, bool persist) const {
    Wire.beginTransmission(KEYBOARD_I2C_ADDR);
    Wire.write(persist ? 0x02 : 0x01); // 0x02 sets the default brightness
    Wire.write(brightness);
    Wire.endTransmission();
  }

  // Handles a single input_event_t from the input_queue
  handler_exception_t GUIManager::handle(input_event_t input) {
      uint16_t id = input.flatten();

      if (input_handlers_.find(id) != input_handlers_.end()) {
        try {
          Serial.printf("Handling event %d\n", id);
          Serial.printf("GUI manager at %p\n", self_);
          input_handlers_[id](self_); // <- this is so fucking cursed lmfao
          return OK;
        } catch (...) {
          return BAD_HANDLER;
        }
      }
      return NO_HANDLER;
  }

  // Handles ALL inputs currently queued in the input_queue
  handler_exception_t GUIManager::handle_inputs() {
    while (!input_queue_.empty()) {
      handler_exception_t status = handle(input_queue_[0]);

      if (status == BAD_HANDLER) {
        Serial.println("ERROR");
        return status;
      }

      input_queue_.pop_first();
    }
    return OK;
  }

  // Adds a window to the viewport
  void GUIManager::add_window(Window* window) {
    this->viewport_.push_back(window);
    if (this->active_window_ == nullptr) {
      this->active_window_ = window;
    }
  }

  // Removes a window from the viewport
  void GUIManager::remove_window(Window* window) {
    if (this->active_window_ == window) {
      this->active_window_ = nullptr;
    }
    this->viewport_.erase(std::remove(this->viewport_.begin(), this->viewport_.end(), window), this->viewport_.end());
  }

  // Sets the active window (window to be drawn
  void GUIManager::set_active_window(Window* window) {
    this->active_window_ = window;
  }

  // Binds an in put event to a handler (void function pointer)
  // input: The input event to bind
  // handle_func: The handler function to bind to the input event
  void GUIManager::bind_input_event(input_event_t input, void (*handle_func)(GUIManager*)) {
    this->input_handlers_[input.flatten()] = handle_func;
  }

  // Unbinds an input event from its respective handler
  // input: The input event to remove
  void GUIManager::unbind_input_event(input_event_t input) {
    this->input_handlers_.erase(input.flatten());
  }

  // Adds an input event to the input queue
  // input: The input event to add
  void GUIManager::create_input_event(input_event_t input) {
    this->input_queue_.push(input);
  }

  // Clears the input queue
  void GUIManager::clear_input_queue() {
    this->input_queue_.clear();
  }

  // Renders the Gui (active window)
  void GUIManager::render() const{
    if (active_window_->isDirty()) {
      tft.fillScreen(TFT_BLACK);
    }
    active_window_->Render();
  }
}
