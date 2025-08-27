#pragma once

#include <map>
#include <vector>

#include "types.h"
#include "pins.h"

#include "components.h"

namespace SGui {
  // Vector of Window pointers
  typedef std::vector<Window*> viewport_t;

  // namespace SGui
  class GUIManager {
  private:
    std::pair<Component*, UIPoint> focused_ = {nullptr, { 0, 0 }};
    Window* active_window_ = nullptr;

    viewport_t viewport_ = {};

    std::vector<input_event_t> input_queue_ = {};
    std::map<uint16_t, void(*)(GUIManager*)> input_handlers_;

    static GUIManager* self_;

  public:

    GUIManager() {
      self_ = this;
    };

    // Destructor to clear the instance on deletion
    ~GUIManager() {
      self_ = nullptr;
    }

    // Initialize the Gui (this MUST be called before use, or inputs will not function)
    static void enable_inputs();

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
    const std::vector<input_event_t>* get_input_queue() const {return &this->input_queue_;}

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
