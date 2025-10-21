#pragma once

namespace SGui {

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

  // Brief descriptions of status values for focus related functions
  enum focus_search_status_t {
    SUCCESS,
    NO_CHILDREN,
    OUT_OF_BOUNDS,
    // DELINQUENT_CHILD,
  };

  enum search_direction_t{
    FORWARD,
    BACKWARD,
  };

  enum component_type_t {
    NORMAL,
    CONTROL,
    CONTAINER
  };

}  // namespace SGui
