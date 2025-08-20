//
// Created by david on 8/29/2024.
//

#include <algorithm>

#include "../helper.h"
#include "Container.h"

namespace SGui {

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

  // Focus the next child component
  UIContainerFocusState Container::FocusNext() {
    // No children?
    if (this->children_.empty()) {
      // Trigger error state
      this->focused_state_.index = -1;
      this->focused_state_.component = nullptr;
      this->focused_state_.err_state = NO_CHILDREN;
      return this->focused_state_;
    }

    // Iterate children following focused child
    while (this->focused_state_.index < this->children_.size() - 1) {
      this->focused_state_.index++; // move to the next child

      if (this->children_[this->focused_state_.index]->type() == CONTROL) {
        if (this->focused_state_.component != nullptr) {
          this->focused_state_.component->Unfocus(); // unfocus component before updating
        }
        this->focused_state_.component = this->children_[this->focused_state_.index]->Focus();
        focused_state_.err_state = SUCCESS;
        goto end;
      }
    }
    this->focused_state_.err_state = OUT_OF_BOUNDS;
    end:
    Serial.println(focused_state_.index);
    Serial.printf("%p\n", this->children_[focused_state_.index]);
    return this->focused_state_;
  }

  // Focus the previous child component
  UIContainerFocusState Container::FocusPrev() {
    // No children?
    if (this->children_.empty()) {
      this->focused_state_.err_state = NO_CHILDREN;
      this->focused_state_.component = nullptr;
      this->focused_state_.index = -1;
      goto end;
    }

    while (this->focused_state_.index > 0) {
      this->focused_state_.index--; // move to the previous child

      // Is this an input?
      if (this->children_[this->focused_state_.index]->type() == CONTROL) {
        // update the state and return success
        this->focused_state_.index = max(this->focused_state_.index, 0);
        if (this->focused_state_.component != nullptr) {
          this->focused_state_.component->Unfocus(); // unfocus component before updating
        }
        this->focused_state_.component = this->children_[this->focused_state_.index]->Focus();
        this->focused_state_.err_state = SUCCESS;
        goto end;
      }
    }

    // don't change focus, just return out of bounds
    this->focused_state_.index = max(
          distance(this->children_.begin(), find(this->children_.begin(), this->children_.end(), this->focused_state_.component)) - 1, 0
        ); // blame C++ iterators for this garbage syntax
    this->focused_state_.err_state = OUT_OF_BOUNDS;

    end:
    Serial.println(focused_state_.index);
    Serial.println(this->children_.size());
    return this->focused_state_;
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
  void Container::DrawChildren() {
    for (Component* child : this->children_) {
      child->Draw();
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
}  // namespace SGui
