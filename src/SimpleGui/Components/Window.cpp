//
// Created by david on 8/24/2024.
//

#include "Window.h"

namespace SGui {
  // Set the title of the window
  Window* Window::SetTitle(String title) {
    this->title_ = title;
    return this;
  }

  // Set the color of the window
  Window* Window::SetColor(UIColor color) {
    this->style_->SetBorderColor(color);
    return this;
  }

  // Set the padding of the container (accounts for window border and title bar)
  Container* Window::SetPadding(int padding_top, int padding_right, int padding_bottom, int padding_left) {
    this->style_->padding_.top = padding_top + this->title_padding.top + this->title_padding.bottom + tft.fontHeight();
    this->style_->padding_.right = padding_right;
    this->style_->padding_.bottom = padding_bottom;
    this->style_->padding_.left = padding_left;
    return this;
  }

  // Add a child component to the window
  Container* Window::AddChild(Component* child) {
    // No duplicate children
    if (v_includes(this->children_, child))
      return this;

    this->children_.push_back(child);
    child->SetParent(this);
    child->MoveIntoParentBounds();

    // Check if child has rel positioning
    if (!child->isAbsolute()) {
      switch(this->orientation_) {
        case VERTICAL:
          child->MovePos(0, this->content_size_.y);
          this->content_size_.y += child->GetRenderedSize().y;
          break;
        case HORIZONTAL:
          child->MovePos(this->content_size_.x, 0);
          this->content_size_.x += child->GetRenderedSize().x;
          break;

        default:
          break;
      }
    }

    return this;
  }

  // Add multiple child components to the window
  Container* Window::AddChildren(ComponentList children) {
    for (Component* child : children) {
      this->AddChild(child);
    }
    return this;
  }

  // Invokes the component to be drawn to the screen if needed
  // set force to bypass `isDirty()` check.
  void Window::Render(bool force) {
    if (force || this->isDirty()) {
      this->Draw();
      RenderChildren(this->isDirty());
      this->dirty_ = false;
      return;
    }
    RenderChildren(this->isDirty());
  }

  // Draw the window and its children
  void Window::Draw() {

    // If the window is inside a container, adjust its position accordingly.
    if (this->parent_ != nullptr) {
      this->MoveIntoParentBounds();
    }

    tft.drawRect(this->pos_.x, this->pos_.y, this->size_.x, this->size_.y, this->style_->border_color_);
    tft.setTextColor(this->style_->foreground_.Compress());

    // If title isn't null, draw the title bar
    if (title_ != nullptr && title_ != "") {
      tft.fillRect(this->pos_.x,
                   this->pos_.y,
                   this->size_.x,
                   tft.fontHeight() + (this->title_padding.bottom + this->title_padding.top),
                   this->style_->border_color_);  // Title bar separator
      tft.drawString(title_, this->pos_.x + this->title_padding.left, this->pos_.y + this->title_padding.top);
    }
  }

}  // namespace SGui
