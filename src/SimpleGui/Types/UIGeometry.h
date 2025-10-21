#pragma once

#include <functional>

#include "enums.h"

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

}
