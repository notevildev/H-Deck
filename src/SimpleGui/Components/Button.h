#pragma once

#include "Component.h"
#include "../types.h"

namespace SGui {
class Button : public Component {
public:
  String text_ = "";

  bool is_focused_ = false;
  e_handler_t click_handler_ = nullptr;

  explicit Button(String text, e_handler_t &&click_handler = nullptr) : Component() {
    this->text_ = text;
    this->style_->padding_ = {2, 2, 2, 2};
    this->click_handler_ = std::move(click_handler);
    this->size_.x = tft.textWidth(text);
    this->size_.y = tft.fontHeight();
  }

  bool isInput() const override { return true; };

  // Set the text of the button
  Button* SetText(String text);

  // Set the text size of the button
  // Default: 1
  Button* SetTextSize(uint8_t text_size);

  // Set the click handler of the button
  Button* SetClickHandler(e_handler_t &&handler);

  // Set the button padding
  Button* SetPadding(int x, int y);
  Button* SetPadding(int top, int left, int bottom, int right);
  Button* SetPadding(UIBoxSpacing padding);

  // Get the button text
  String GetText() const { return this->text_; }

  // Focus the button (draws background)
  void Focus() { this->is_focused_ = true; }

  // Unfocus the button (draws border)
  void Unfocus() { this->is_focused_ = false; }

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