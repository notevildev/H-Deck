#pragma once

#include <utility>
#include <vector>

#include "Component.h"
#include "../DefaultStyles.h"

namespace SGui {

struct UIContainerFocusState{
  focus_state_status_t err_state;
  Component* component;
  int index;
};

// Expanded UIComponent class for all NESTING UI components
// Stores horizontal padding, vertical padding, and child components
// (All NESTING components should inherit from this class)
class Container : public Component {
protected:
  ComponentList children_ = {};
  UIRect content_size_ {0, 0};

  UIContainerFocusState focused_state_ = {
    NO_CHILDREN,
    nullptr,
    -1
  };

public:
  UIOrientation orientation_ = VERTICAL;

  Container() = default; // default constructor
  explicit Container(ComponentList children)
      : Component(), children_(std::move(children)) {}

  __always_inline uint16_t ContentWidth() const { return this->content_size_.x; }
  __always_inline uint16_t ContentHeight() const { return this->content_size_.y; }

  component_type_t type() const override { return CONTAINER; }

  // Returns a list of pointers to recursive children
  // ***Starts with the component itself
  ComponentList Children() override;

  // Returns a list of pointers to direct children (not recursive)
  ComponentList DirectChildren() const { return children_; }

  // Focus the next child component
  UIContainerFocusState FocusNext();
  // Focus the previous child component
  UIContainerFocusState FocusPrev();
  // Focus the specified child component
  UIContainerFocusState FocusChild(int index);
  UIContainerFocusState FocusChild(Component* child);

  // Draw just the children of the component (not the component itself)
  void RenderChildren(bool force = false);

  // Invokes the container to be drawn to the screen if needed
  // set force to bypass `isDirty()` check.
  void Render(bool force = false) override;

  // Set the padding of the container
  virtual Container* SetPadding(int padding_top, int padding_right, int padding_bottom, int padding_left);

  // Add a child component to the container
  virtual Container* AddChild(Component* child);

  // Add multiple child components to the container
  virtual Container* AddChildren(ComponentList children);

};
}  // namespace SGui
