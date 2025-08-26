// File: GuiManager.cpp

#include "GuiManager.h"

#include <vector>
#include <unordered_map>

#include "types.h"

namespace SGui {

  GUIManager* GUIManager::self_ = nullptr; // <- satisfy the linker

  void GUIManager::enable_inputs() {
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

      input_queue_.erase(input_queue_.begin());
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
    this->input_queue_.push_back(input);
  }

  // Clears the input queue
  void GUIManager::clear_input_queue() {
    this->input_queue_.clear();
  }

  // Renders the Gui (active window)
  void GUIManager::render() const{
    // TODO: Add logic to only redraw display when necessary.
    /* disabled screen flush due to obnoxious flickering */
    // tft.fillScreen(TFT_BLACK); // flush the screen before redrawing
    active_window_->Render();
  }
}
