//
// Created by rondotdll on 8/29/2024.
//

#include <algorithm>

// #include "../helper.h"
#include "Container.h"

namespace SGui {

  Component* Container::FindNextFocusableChild(search_direction_t direction) {
    int i = this->focused_state_.index;

    // No children?
    if (this->children_.empty()) { return nullptr; }

      // handle edge case
      if (direction == BACKWARD && i == -1)
        i = (int)this->children_.size();

      // Iterate through children in the specified direction
      // (loop condition depends on scan direction)
      while (direction == FORWARD ? i < (int)this->children_.size() - 1 : i > 0) {
        i = i + (direction == FORWARD ? 1 : -1); // move to the next or previous child
        Component* child = this->children_[i];

        // Is child an input?
        if (child->type() == CONTROL) {
          // Update the state and return success
          return this->children_[i];
        }

        // Recurse into child container(s)
        if (child->type() == CONTAINER) {
          Component* subchild = FindNextFocusableChild(direction);
          if (subchild && subchild != this->focused_state_.component) {
            return subchild;
          }
        }
      }

      return this->focused_state_.component;
  }


  // Returns a list of pointers to recursive children
  // ***Starts with the component itself
  ComponentList Container::Children() {
    ComponentList output = { this };

    if (this->children_.empty())
      return output;

    for (Component* c : this->children_ ) {
      output.push_back(c);
      for (Component* c2 : c->Children()) {
        if (c2 == nullptr || c2 == c) break;
        output.push_back(c2);
      }
    }

    return output;
  }

  /* Focus the next deepest available child component
   * Will recursively search through any child containers, dynamically
   * passing focus reassignment to the deepest available focusable child.
   *
   * (Should be called on the outermost parent)
   */
  UIContainerFocusState Container::FocusNext(search_direction_t direction) {
    Component* prev_focusable_child = FindNextFocusableChild(direction);
    // No children?
    if (!prev_focusable_child) {
      // Trigger error state
      this->focused_state_.index = -1;
      this->focused_state_.component = nullptr;
      this->focused_state_.err_state = NO_CHILDREN;
      goto end;
    }


    // We're already focused on the first control, return out of bounds
    if (prev_focusable_child == this->focused_state_.component) {
      this->focused_state_.err_state = OUT_OF_BOUNDS;
      goto end;
    }

    this->focused_state_.index++;
    this->focused_state_.component->Unfocus();
    this->focused_state_.component = prev_focusable_child->Focus();
    this->focused_state_.err_state = SUCCESS;

    end:
#ifdef DEBUG
    Serial.printf("Focused Index: %d\n", this->focused_state_.index);
    Serial.printf("Error: %d\n", this->focused_state_.err_state);
    Serial.printf("Pointer: %p\n", this->focused_state_.component);
#endif

    return this->focused_state_;
  }


  /* Focus the previous deepest available child component
   * Will recursively search through any child contianers, dynamically
   * passing focus reassignment to the deepest available focusable child.
   *
   * (Should be called on the outermost parent)
   */
  UIContainerFocusState Container::FocusPrev() {
    return FocusNext(BACKWARD);
  }


  // Focus the specified child component
  UIContainerFocusState Container::FocusChild(int index) {
    // No children?
    if (this->children_.empty()) {
      this->focused_state_.component = nullptr;
      this->focused_state_.index = -1;
      this->focused_state_.err_state = NO_CHILDREN;
      return this->focused_state_;
    }

    // Verify index is within bounds
    if (index >= 0 && index < this->children_.size()) {
      // Is this an input?
      if (this->children_[index]->type() == CONTROL) {
        if (this->focused_state_.component != nullptr) {
          this->focused_state_.component->Unfocus(); // unfocus component before updating
        }
        this->focused_state_.component = this->children_[index]->Focus();
        this->focused_state_.index = index;
        this->focused_state_.err_state = SUCCESS;
        return this->focused_state_;
      }

      // don't change focus, just return delinquent child
      this->focused_state_.err_state = DELINQUENT_CHILD;
      return this->focused_state_;
    }

    // don't change focus, just return out of bounds
    this->focused_state_.err_state = OUT_OF_BOUNDS;
    this->focused_state_.index = max(
          distance(this->children_.begin(), find(this->children_.begin(), this->children_.end(), this->focused_state_.component)) - 1, 0
        ); // blame C++ iterators for this garbage syntax
    return this->focused_state_;
  }


  // Focus the specified child component
  UIContainerFocusState Container::FocusChild(Component* child) {
    // No children?
    if (this->children_.empty()) {
      this->focused_state_.err_state = NO_CHILDREN;
      this->focused_state_.component = nullptr;
      this->focused_state_.index = -1;
      goto end;
    }

    if (child->type() == CONTROL) {
      for (int i = 0; i < this->children_.size(); i++) {
        if (this->children_[i] == child) {
          if (this->focused_state_.component != nullptr) {
            this->focused_state_.component->Unfocus(); // unfocus component before updating
          }
          this->focused_state_.component = child->Focus();
          this->focused_state_.index = i;
          this->focused_state_.err_state = SUCCESS;
          goto end;
        }
      }
      this->focused_state_.err_state = OUT_OF_BOUNDS;
      goto end;
    }
    this->focused_state_.err_state = DELINQUENT_CHILD;

    end:
    return this->focused_state_;
  }


  // Returns a list of pointers to direct children (not recursive)
  void Container::RenderChildren(bool force) {
    for (Component* child : this->children_) {
      child->Render(force);
    }
  }


  // Set the padding of the container
  Container* Container::SetPadding(int padding_top, int padding_right, int padding_bottom, int padding_left) {
    this->style_->padding_.top = padding_top;
    this->style_->padding_.right = padding_right;
    this->style_->padding_.bottom = padding_bottom;
    this->style_->padding_.left = padding_left;
    return this;
  }

  // Add a child component to the container
  Container* Container::AddChild(Component* child) {
    int i = 0; //  do not move (compiler error)

    // If empty, skip all logic
    if (this->children_.empty()) {
      this->children_.push_back(child);
      goto end;
    }

    switch (this->orientation_) {
      case HORIZONTAL: {
        // If relatively positioned, re-position the child to the parent
        if (!child->isAbsolute()) {
          child->MovePos(this->ContentWidth(), 0);
          this->content_size_.y += child->GetRenderedSize().y;
        }

        // Iterate through all children
        for (Component* c : this->children_) {
          // No duplicate children
          if (c == child) return this;

          if (c->x() > child->x()) {
            this->children_.insert(this->children_.begin() + i, child);
            goto end;
          }

          i += 1;
        }
      }

      case VERTICAL: {
        if (!child->isAbsolute()) {
          child->MovePos(0, this->ContentHeight());
          this->content_size_.y += child->GetRenderedSize().y;
        }

        for (Component* c : this->children_) {
          if (c == child) return this;

          if (c->y() > child->y()) {
            this->children_.insert(this->children_.begin() + i, child);
            goto end;
          }

          i += 1;
        }
      }
    }

    // If it's further down than everything else, add it to the end of the stack
    this->children_.push_back(child);

    end:
    child->SetParent(this);
    child->MoveIntoParentBounds();

    return this;
  }

  // Add multiple child components to the container
  Container* Container::AddChildren(ComponentList children) {
    for (Component* child : children) {
      Serial.printf("Adding child %p to container\n\r", child);
      this->AddChild(child);
    }
    return this;
  }

  // Invokes the container to be drawn to the screen if needed
  // set force to bypass `isDirty()` check.
  void Container::Render(bool force) {
    if (force || this->isDirty()) {
      this->Draw();
      RenderChildren(this->isDirty());
      this->dirty_ = false;
      return;
    }
    RenderChildren(this->isDirty());
  }

}  // namespace SGui
