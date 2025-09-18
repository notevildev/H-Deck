#pragma once

#include <deque>
#include <functional>

#include <TFT_eSPI.h>

#include "pins.h"

namespace SGui {

  static TFT_eSPI tft = TFT_eSPI();  // TFT display object

  // Alignment options for UIComponents
  enum UIAlignment {
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    MID_LEFT,
    MID_CENTER,
    MID_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTER,
    BOTTOM_RIGHT
  };

  enum UIOrientation {
    HORIZONTAL,
    VERTICAL,
  };

  // Input types for input events
  enum input_type_t {
    BASIC = 1,
    TRACKBALL = 1,
    KEYBOARD = 1,
    TOUCH = 2,
  };

  // Trackball single input IDs
  enum trackball_input_id_t: uint8_t {                                        // XXXXXXXX = PIN ID
    TRACKBALL_PRESS = (16),   // 00010000
    TRACKBALL_UP =    (1),    // 00000001
    TRACKBALL_DOWN =  (2),    // 00000010
    TRACKBALL_LEFT =  (4),    // 00000100
    TRACKBALL_RIGHT = (8),    // 00001000
  };

  enum handler_exception_t {
    OK,
    NO_HANDLER,
    BAD_HANDLER,
  };

  // Brief descriptions of status values for focus related functions
  enum focus_state_status_t {
    SUCCESS,
    OUT_OF_BOUNDS,
    NO_CHILDREN,
    DELINQUENT_CHILD,
  };

  enum component_type_t {
    NORMAL,
    CONTROL,
    CONTAINER
  };

  typedef struct _Point2D {
    uint16_t x;
    uint16_t y;
  } UIRect, UIPoint;

  typedef struct _BoxSpacing {
    uint16_t top;
    uint16_t right;
    uint16_t bottom;
    uint16_t left;
  } UIBoxSpacing;

  typedef std::function<void()> e_handler_t;  // Event handler type

  // Stores specific input events (type & ID)
  typedef struct input_event_t {
    input_type_t type;
    uint16_t id;

    uint16_t flatten() const {
      return (static_cast<uint16_t>(this->type) << 8) | this->id;
    };

  } input_event_t;

  template <typename T, size_t maxSize>
  class managed_buffer {
    std::deque<T> data;

  public:

    size_t size() const { return data.size(); }
    void clear() { data.clear(); }
    bool empty() const { return data.empty(); }
    const T& front() const { return data.front(); }
    const T& back() const { return data.back(); }


    void push(const T& value) {
      if (data.size() == maxSize) {
        data.pop_front();
      }
      data.push_back(value);
    }

    void pop_first() {
      if (!data.empty()) {
        data.pop_front();
      }
    }

    void pop_last() {
      if (!data.empty()) {
        data.pop_back();
      }
    }

    managed_buffer& operator+=(const T& value) {
      if (data.size() == maxSize) {
        data.pop_front();
      }
      data.push_back(value);

      return this;
    }

    T &operator[](int index) {
      return data.at(index);
    }

    const std::deque<T>& get() const { return data; }
  };

}  // namespace SGui