#include "GuiManager.h"

#include <vector>
#include <unordered_map>

#include <Wire.h>

#include "HID/DPad.h"
#include "Types/input.h"
#include "Types/pins.h"

namespace SGui {

  GUIManager* GUIManager::self_ = nullptr; // <- satisfy the linker (for whatever reason)

  GUIManager* GUIManager::New() {
    if (self_) { return self_; }

    tft.init();
    tft.setRotation(1);
    clearScreen();

    self_ = new GUIManager();
    return self_;
  }

  void GUIManager::enable_keyboard_input(HID::Keyboard* keyboard) {
    // verify the keyboard is ready to use
    // this->keyboard_

  }

  void GUIManager::enable_dpad_navigation(HID::DPad* dpad) {
    this->dpad_ = dpad;
    // TODO: Bind trackball inputs to change focused component
    // this->bind_input_event(
    //   {
    //     .type=DPAD_PRESSED,
    //     .id=HID::DPAD_UP
    //   },
    //   [](GUIManager* gui) {
    //     if (gui->active_window_ == nullptr) return;
    //
    //     if (focused_state.err_state == NO_CHILDREN) return;
    //     if (focused_state.err_state == OUT_OF_BOUNDS) {
    //       gui->active_window_->FocusChild(0);
    //     }
    //   }
    // );
  }

  focus_search_status_t GUIManager::focus_next_component(UIOrientation orientation) {
    // this whole condition is just to handle edge-cases
    if (this->viewport_.empty()) {
      if (this->active_window_) {
        this->active_window_->Unfocus();

        this->active_window_ = nullptr;
        this->active_window_index_ = -1;
      }

      if (this->focused_component_) {
        this->focused_component_->Unfocus();
      }

      return NO_CHILDREN;
    }

    if (!active_window_) {
      this->set_active_window(this->viewport_[0]);
    }

    ComponentList all_children = this->active_window_->Children();

    int i = 0;
    Component* firstFocusable = nullptr;
    bool found_focused = false;
    for (Component* child : all_children) {
      if (child->type() == CONTROL) {
        if (found_focused) {
          this->focused_component_->Unfocus();
          this->focused_component_ = child->Focus();
          return SUCCESS;
        }

        if (!firstFocusable) {
          firstFocusable = child;
        }
      }

      if (child == this->focused_component_) {
        found_focused = true;
      }


      i++;
    }

    if (found_focused) return OUT_OF_BOUNDS;

    this->focused_component_->Unfocus();
    this->focused_component_ = firstFocusable->Focus();
    return SUCCESS;
  }

  // Handles a single input_event_t from the input_queue
  handler_status_t GUIManager::handle(input_event_t input) {
      uint16_t id = input.flatten();

      if (input_handlers_.find(id) != input_handlers_.end()) {
        try {
#ifdef DEBUG
          Serial.printf("Handling event %d\n", id);
          Serial.printf("GUI manager at %p\n", self_);
#endif

          input_handlers_[id](self_); // <- this is so fucking cursed lmfao
          return COMPLETE;
        } catch (...) {
          return BAD_HANDLER;
        }
      }
      return NO_HANDLER;
  }

  // Handles ALL inputs currently queued in the input_queue
  handler_status_t GUIManager::handle_inputs() {
    while (!input_queue_.empty()) {
      handler_status_t status = handle(input_queue_[0]);

      if (status == BAD_HANDLER) {
        Serial.println("ERROR");
        return status;
      }

      input_queue_.pop_first();
    }
    return COMPLETE;
  }

  // Adds a window to the viewport
  void GUIManager::add_window(Window* window) {
    this->viewport_.push_back(window);
    if (this->active_window_ == nullptr) {
      this->active_window_ = window;
      this->active_window_index_ = indexOf(this->active_window_, this->viewport_);
    }
  }

  // Removes a window from the viewport
  void GUIManager::remove_window(Window* window) {
    if (this->active_window_ == window) {
      this->active_window_ = nullptr;
      this->active_window_index_ = -1;
    }
    this->viewport_.erase(std::remove(this->viewport_.begin(), this->viewport_.end(), window), this->viewport_.end());
  }

  // Sets the active window (window to be drawn
  void GUIManager::set_active_window(Window* window) {
    this->focused_component_ = nullptr;
    this->active_window_ = window;

    if (!this->active_window_) {
      this->active_window_index_ = -1;
      return;
    }

    this->focus_next_component();
    this->active_window_index_ = indexOf(window, this->viewport_);
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
