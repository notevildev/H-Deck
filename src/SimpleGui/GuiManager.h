#pragma once

#include <map>
#include <vector>

#include "Utils/managed_buffer.h"
#include "Components/Window.h"
#include "Types/input.h"
#include "HID/DPad.h"
#include "HID/Keyboard.h"

namespace SGui {
  // Vector of Window pointers
  typedef std::vector<Window*> WindowList;

  // namespace SGui
  class GUIManager {
  private:
    Window* active_window_ = nullptr;
    int16_t active_window_index_ = -1;

    Component* focused_component_;

    HID::DPad* dpad_;
    HID::Keyboard* keyboard_;

    // Current viewport (vector of pointers to each added window)
    WindowList viewport_ = {};

    // Input queue (managed buffer to prevent overflow)
    input_event_queue_t input_queue_ = {};
    std::map<uint16_t, void(*)(GUIManager*)> input_handlers_;

    static GUIManager* self_; // used to verify singleton instance

    GUIManager() {
      if (self_) { return; } // should never happen

      self_ = this;
      dpad_ = nullptr;
      keyboard_ = nullptr;
      focused_component_ = nullptr;
    };

  public:
    /* Use this function to create a new global GUIManager instance
     * Safety check verifies no accidental duplicate instances are created
     */
    static GUIManager* New();

    // Destructor to clear the instance on deletion
    ~GUIManager() {
      if (self_ == this) { // safety check, should never fail
        self_ = nullptr;
      }
    }



    // Enables keyboard input handling (this MUST be called after initialize_keyboard())
    void enable_keyboard_input(HID::Keyboard* keyboard);

    /* Adds default event handlers to enable D-Pad UI navigation
     * Requires a pointer to a DPad object (e.g. TTrackball)
     */
    void enable_dpad_navigation(HID::DPad* dpad);

    /* Focus the next available focusable components
     * Recursively seeks out the next focusable component in the
     * specified orientation (Horizontal -> Left, Vertical -> Down)
     */
    focus_search_status_t focus_next_component(UIOrientation orientation = VERTICAL);

    /* Focus the previous available focusable components
     * Recursively seeks out the next focusable component in the
     * specified orientation (Horizontal -> Left, Vertical -> Down)
     */
    void focus_prev_component(UIOrientation orientation = VERTICAL);



    /*
    * Dynamically modify the keyboard backlight brightness at runtime
    * Brightness Range: 0 ~ 255
    * */
    void setKeyboardBacklight(uint8_t brightness, bool persist = false) const;

    // Handles a single input_event_t from the input_queue
    handler_status_t handle(input_event_t input);
    // Handles ALL inputs currently queued in the input_queue
    handler_status_t handle_inputs();

    // Returns pointer to the component that is currently input focused
    Component* get_focused_component() const { return this->focused_component_; }
    // Returns pointer to the active window
    Window* get_active_window() const { return this->active_window_;}
    // Returns the current viewport (vector of pointers to each added window)
    WindowList get_viewport() const {return this->viewport_;}
    // Returns pointer to the current input queue
    const input_event_queue_t* get_input_queue() const {return &this->input_queue_;}

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
