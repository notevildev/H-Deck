//
// Created by rondotdll on 8/29/2024.
//

#include <algorithm>

// #include "../helper.h"
#include "Container.h"

namespace SGui {

// TODO: Focus management can be significantly optimized by taking advantage of Contianer::Children()
  focus_search_status_t Container::FocusNext(search_direction_t direction) {

    // No Children?
    if (this->children_.size() == 0) {
      if (this->focused_.component) {
        this->focused_.component->Unfocus(); // edge case
      }

      this->focused_.component = nullptr;
      this->focused_.index = -1;
      return NO_CHILDREN;
    }

    focus_search_status_t search_status;

    // is something focused? if not, we can skip a bit
    if (!this->focused_.component || this->focused_.index < 0) {
      if (direction == BACKWARD) {
        return OUT_OF_BOUNDS;
      }
      goto scan_children;
    }

    // Are we currently focusing on another container?
    if (this->focused_.component->type() == CONTAINER) {
      // [recurse]
      search_status = static_cast<Container*>(this->focused_.component)->FocusNext();

      if (search_status == SUCCESS) {
        return SUCCESS;
      }

      if (search_status == NO_CHILDREN) {
        this->focused_.component->Unfocus(); // unfocus previous
      }
    }

    // Scan for the next focusable child in the current scope
  scan_children:
    uint16_t i = (direction == FORWARD) ? (this->focused_.index + 1) : 0;

    while ((direction == FORWARD) ? i < this->children_.size() : i > this->children_.size() - 1) {
      Component* child = this->children_[i];

      // if the next scanned child is a control, go ahead and focus it.
      if (child->type() == CONTROL) {
        this->focused_.component->Unfocus(); // unfocus previous

        this->focused_.index = i;
        this->focused_.component = child->Focus();
        return SUCCESS;
      }

      // if the next scanned child is a container, recurse into it.
      if (child->type() == CONTAINER) {
        search_status = static_cast<Container*>(child)->FocusNext(direction);

        if (search_status != SUCCESS)
          i = (direction == FORWARD) ? (i + 1) : (i - 1); // move to the next child
          continue;

        this->focused_.component->Unfocus(); // unfocus previous

        this->focused_.component = child;
        this->focused_.index = i;
        return SUCCESS;
      }
    }

    return OUT_OF_BOUNDS;
  }


  Component* Container::Unfocus() {
    if (this->focused_.component) {
      this->focused_.component->Unfocus();

      this->focused_.component = nullptr;
      this->focused_.index = -1;
    }

    this->has_focus_ = false;
    return this;
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
