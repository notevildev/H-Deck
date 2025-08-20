#pragma once

#include <vector>

#include "../types.h"
#include "../UIStyle.h"
#include "../DefaultStyles.h"

namespace SGui {
// Base class for all UI components
// Stores position, size, and parent component
// (All components should inherit from this class)
class Component {
protected:
  bool absolute_ = false;  // positioning mode

public:
  UIPoint pos_{0, 0}; // 2D point representing position
  UIRect size_{0, 0}; // 2D point representing size

  UIStyle* style_ = new UIStyle(*DEFAULT_STYLE);
  UIStyle* focused_style_ = new UIStyle(*DEFAULT_STYLE_FOCUSED);

  bool focused_ = false; // focused state
  Component* parent_ = nullptr;

  Component() = default; // default constructor
  explicit Component(UIPoint position, UIRect dimensions, UIStyle* style, UIStyle* focused_style = DEFAULT_STYLE_FOCUSED,
                       Component* parent = nullptr)
      : pos_(position), size_(dimensions), style_(style), focused_style_(focused_style), parent_(parent) {

  }

  // Draw the component
  virtual void Draw() = 0;

  // Get the rendered size of the component
  virtual UIRect GetRenderedSize() const { return size_; }

  bool isAbsolute() const { return absolute_; }

  virtual component_type_t type() const { return NORMAL; }

  // Enable or disable absolute positioning for the component
  Component* absolute(bool enabled = true);

  // Get the width of the component
  __always_inline uint16_t width() const { return this->size_.x; }
  // Get the height of the component
  __always_inline uint16_t height() const { return this->size_.y; }
  // Get the x position of the component
  __always_inline uint16_t x() const { return this->pos_.x; }
  // Get the y position of the component
  __always_inline uint16_t y() const {return this->pos_.y; }
  // Get the center of the component
  UIPoint get_center() const {
    return {
      (uint16_t)(this->x() - (this->width() / 2)),
      (uint16_t)(this->y() - (this->height() / 2))
    };
  }

  // Returns the component itself. (Used for Containers)
  virtual std::vector<Component*> Children() { return { this }; };

  // Change the focused state of the component
  Component* Focus(bool state = true);

  // Changes the focused state of the component to false
  Component* Unfocus();

  // Modify position to move the component into the bounds of its parent
  Component* MoveIntoParentBounds();

  // Resize the component to fill its parent
  Component* FillParent();

  // Align the component relative to its parent
  Component* AlignToParent(UIAlignment alignment);

  // Set the position of the component
  Component* SetPos(int x, int y);

  // Moves the position of the component
  // (Similar to set, but adds or subtracts relative to the current position)
  Component* MovePos(int x, int y);

  // Set the size of the component
  Component* SetSize(int w, int h);

  // Set the parent of the component
  Component* SetParent(Component* parent);
};

typedef std::vector<Component*> ComponentList;

}  // namespace SGui
