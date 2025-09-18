#pragma once

#include <map>
#include <vector>

#include "types.h"
#include "pins.h"

#include "components.h"

#define KEYBOARD_I2C_ADDR 0x55
#define INPUT_EVENT_BUFFER_SIZE 32

namespace SGui {
  // Vector of Window pointers
  typedef std::vector<Window*> viewport_t;

  // namespace SGui
  class GUIManager {
  private:
    std::pair<Component*, UIPoint> focused_ = {nullptr, { 0, 0 }};
    Window* active_window_ = nullptr;


    // Current viewport (vector of pointers to each added window)
    viewport_t viewport_ = {};

    // Input queue (managed buffer to prevent overflow)
    managed_buffer<input_event_t, INPUT_EVENT_BUFFER_SIZE> input_queue_ = {};
    std::map<uint16_t, void(*)(GUIManager*)> input_handlers_;

    static GUIManager* self_;
    static bool keyboard_ready_;
    static bool trackball_ready_;
    static void* keyboard_task_;

  public:

    GUIManager() {
      if (self_) { return; }

      keyboard_task_ = nullptr;
      trackball_ready_ = false;
      keyboard_ready_ = false;
      self_ = this;
    };

    // Destructor to clear the instance on deletion
    ~GUIManager() {
      keyboard_task_ = nullptr;
      if (self_ == this) { // safety check, should never fail
        self_ = nullptr;
      }
    }

    // Initialize the keyboard (this MUST be called before use, or keyboard input will not work)
    void initialize_keyboard() const;

    // Enables keyboard input handling (this MUST be called after initialize_keyboard())
    void enable_keyboard_input() const;

    void initialize_trackball() const;

    // Initialize the trackball (this MUST be called before use, or trackball navigation will not work)
    void enable_trackball_input();

    /*
    * Dynamically modify the keyboard backlight brightness at runtime
    * Brightness Range: 0 ~ 255
    * */
    void setKeyboardBacklight(uint8_t brightness, bool persist = false) const;

    // Handles a single input_event_t from the input_queue
    handler_exception_t handle(input_event_t input);
    // Handles ALL inputs currently queued in the input_queue
    handler_exception_t handle_inputs();

    // Returns pointer to the component that is currently input focused
    Component* get_focused_component() const { return this->focused_.first; }
    // Returns pointer to the active window
    Window* get_active_window() const { return this->active_window_;}
    // Returns the current viewport (vector of pointers to each added window)
    viewport_t get_viewport() const {return this->viewport_;}
    // Returns pointer to the current input queue
    const managed_buffer<input_event_t, INPUT_EVENT_BUFFER_SIZE>* get_input_queue() const {return &this->input_queue_;}

    // Adds a window to the viewportf
    void add_window(Window* window);
    // Removes a window from the viewport
    void remove_window(Window* window);
    // Sets the active window (window to be drawn)
    void set_active_window(Window* window);

    // Binds an input event to a handler (void function pointer)
    // input: The input event to bind
    // handler: The handler function to bind to the input event
    void bind_input_event(input_event_t input, void (*handle_func)(GUIManager*));
    // Unbinds an input event from its respective handler
    // input: The input event to remove
    void unbind_input_event(input_event_t input);

    // Adds an input event to the input queue
    // input: The input event to add
    void create_input_event(input_event_t input);
    // Clears the input queue
    void clear_input_queue();

    // Draws the Gui (active window)
    void render() const;

  };

}
