#pragma once

#include "InputComponent.h"
#include "../types.h"

namespace SGui {
class Button : public InputComponent {
private:
  Observable<String> text_;
  e_handler_t click_handler_ = nullptr;

public:

  explicit Button(String text, e_handler_t &&click_handler = nullptr) : InputComponent() {
    this->text_ = Observable<String>(this, text);

    this->style_->padding_ = {2, 2, 2, 2};
    this->click_handler_ = std::move(click_handler);
    this->size_.x = tft.textWidth(text);
    this->size_.y = tft.fontHeight();
  }

  // Set the text of the button
  Button* SetText(const String& text);

  // Set the text size of the button
  // Default: 1
  Button* SetTextSize(uint8_t text_size);

  // Set the click handler of the button
  Button* SetClickHandler(e_handler_t &&handler);

  // Get the button text
  String GetText() const { return this->text_; }

  // Trigger the click event
  void Click() const { this->click_handler_(); }

  // Get the actual rendered size of the button
  UIRect GetRenderedSize() const override {
    return {
      (uint16_t)(this->size_.x + this->style_->padding_.left + this->style_->padding_.right),
      (uint16_t)(this->size_.y + this->style_->padding_.top + this->style_->padding_.bottom)
    };
  };

  // Draw the button
  void Draw() override;

};
}