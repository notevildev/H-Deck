#pragma once
//
// Created by rondotdll on 9/4/25.
//

#include "InputComponent.h"

namespace SGui {

class TextBox : public InputComponent {
  private:
    Observable<String> value_;
    String placeholder_ = "";
    event_handler_t value_changed_handler_ = nullptr;

  public:

    // Default constructor
    TextBox() = default;

    // Explicit
    explicit TextBox(String placeholder = "", event_handler_t &&value_changed_handler = nullptr) : InputComponent() {
      this->value_ = Observable<String>(this, "");
      this->value_changed_handler_ = std::move(value_changed_handler);

      this->style_->padding_ = {2, 2, 2, 2};

      // if parent exists, set width to parent's width
      if (this->parent_) {
        this->size_.x = this->parent_->size_.x;
      } else { this->size_.x = tft.textWidth("A") * 8; }

      this->size_.y = tft.fontHeight();
    }

    String value() const { return this->value_; };

    // Set the text size of the TextBox
    TextBox* SetText(String text);

    // Set the value changed handler of the TextBox
    TextBox* SetValueChangedHandler(event_handler_t &&handler);

    // Set the placeholder text of the TextBox
    TextBox* SetPlaceholder(String placeholder);

    UIRect GetRenderedSize() const override {
      return {
        (uint16_t)(this->size_.x + this->style_->padding_.left + this->style_->padding_.right),
        (uint16_t)(this->size_.y + this->style_->padding_.top + this->style_->padding_.bottom)
      };
    }

    void Draw() override;
  };
}
