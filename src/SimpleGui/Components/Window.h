#pragma once

#include <TFT_eSPI.h>

#include "../types.h"
#include "../helper.h"
#include "Container.h"

namespace SGui {
  // Vector of ComponentLists, which are a vector of Component pointers
  typedef std::vector<ComponentList> navtree_t;

  // Window Component
  class Window : public Container {
   public:
    const UIBoxSpacing title_padding{4, 4, 4, 4};
    String title_ = ""; // Window title

    explicit Window() : Container() {
      if (this->size_.x == 0) {
        this->size_.x = TFT_WIDTH;
      }

      if (this->size_.y == 0) {
        this->size_.y = TFT_HEIGHT;
      }

      if (this->style_ == nullptr) {
        Serial.println("Style is null, panicking");
        exit(-1);
      }

      this->style_->padding_.top = title_padding.top + title_padding.bottom     // account for title padding
                                        + tft.fontHeight();     // account for title text

      if (title_ != "") {
        this->pos_.y = tft.fontHeight() + this->pos_.y + 1;
      }
    };

    // Set the title of the window
    Window* SetTitle(String title);

    // Set the color of the window
    Window* SetColor(UIColor color);

    // Set the padding of the container (accounts for window border and title bar)
    Container* SetPadding(int padding_top, int padding_right, int padding_bottom, int padding_left) override;

    // Add a child component to the window
    Container* AddChild(Component* child) override;

    // Add multiple child components to the window
    Container* AddChildren(ComponentList children) override;

    // Draw the window and its children
    void Draw() override;
  };
}  // namespace SGui
