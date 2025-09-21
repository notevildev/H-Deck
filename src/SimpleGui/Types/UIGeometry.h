#pragma once

#include <functional>

#include "Enums.h"

namespace SGui {
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

typedef std::function<void()> e_handler_t; // Event handler type

// Stores specific input events (type & ID)
typedef struct input_event_t {
    input_type_t type;
    uint16_t id;

    uint16_t flatten() const {
        return (static_cast<uint16_t>(this->type) << 8) | this->id;
    };
} input_event_t;
}
