#pragma once
//
// Created by David J on 9/4/25.
//

#include "Component.h"
#include "../helper.h" // (already includes types.h)


namespace SGui {
  class TextBox : public Component {
  private:
    Observable<String> value_;
    e_handler_t value_changed_handler_ = nullptr;

  public:

    // Default constructor
    TextBox() = default;
    // Explicit
    explicit TextBox(String placeholder = "", e_handler_t &&value_changed_handler = nullptr) : Component() {
      this->value_ = Observable<String>(this, "");
      this->value_changed_handler_ = std::move(value_changed_handler);

      this->style_->padding_ = {2, 2, 2, 2};
      this->value_changed_handler_ = std::move(value_changed_handler);

      // if parent exists, set width to parent's width
      if (this->parent_) {
        this->size_.x = this->parent_->size_.x;
      } else {this->size_.x = }

      this->size_.y = tft.fontHeight();
    }

    component_type_t type() const override { return CONTROL; };

    String value() const { return this->value_; };

    // Set the text size of the TextBox
    TextBox* SetText(String text);

    // Set the value changed handler of the TextBox
    TextBox* SetValueChangedHandler(e_handler_t &&handler);

    // Set the placeholder text of the TextBox
    TextBox* SetPlaceholder(String placeholder);


    void Draw() override;
  };
}
