#pragma once

#include <vector>
#include <algorithm>

#include "types.h"

namespace SGui {

// Constant storing the width of a single character
const int16_t charWidth = tft.textWidth("A");

// Find the true edges of the screen (useful for drawing around a screen border)
// returns (width, height)
std::pair<int, int> GetScreenEdges();

// Calculate the slope between two points
float slope(UIPoint p1, UIPoint p2);

// Clear the screen to black
void clearScreen();

template <typename T>
// Check if a vector contains a value
inline bool v_includes(const std::vector<T>& vec, const T& value) {
  if (vec.empty())
    return false;
  return std::find(vec.begin(), vec.end(), value) != vec.end();
}
}