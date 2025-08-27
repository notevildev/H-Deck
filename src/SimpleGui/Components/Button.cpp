//
// Created by david on 11/24/2024.
//

#include <TFT_eSPI.h>

#include "Button.h"

namespace SGui {

  // Set the text of the button
  Button* Button::SetText(String text) {
    this->text_ = text;

    // Update the size of the button
    this->size_.x = tft.textWidth(text) * this->style_->text_size_;
    this->size_.y = tft.fontHeight() * this->style_->text_size_;
    return this;
  }

  // Set the text size of the button
  // Default: 1
  Button* Button::SetTextSize(uint8_t text_size) {
    this->style_->SetTextSize(text_size);

    // Update the size of the button
    this->size_.x = tft.textWidth(this->text_) * this->style_->text_size_;
    this->size_.y = tft.fontHeight() * this->style_->text_size_;
    return this;
  }

  // Set the click handler of the button
  Button* Button::SetClickHandler(e_handler_t &&handler) {
      this->click_handler_ = std::move(handler);
      return this;
  }

  // Set the button padding
  Button* Button::SetPadding(int x, int y) {
      this->style_->padding_.top = y;
      this->style_->padding_.left = x;
      this->style_->padding_.bottom = y;
      this->style_->padding_.right = x;
      return this;
  }
  Button* Button::SetPadding(int top, int left, int bottom, int right) {
      this->style_->padding_.top = top;
      this->style_->padding_.left = left;
      this->style_->padding_.bottom = bottom;
      this->style_->padding_.right = right;
      return this;
  }
  Button* Button::SetPadding(UIBoxSpacing padding) {
      this->style_->padding_ = padding;
      return this;
  }

  // Draw the button
  void Button::Draw() {
    UIStyle* style = this->style_;

    if (this->focused_) { style = this->focused_style_; }

    // Clear the button background
    tft.fillRect(this->pos_.x, this->pos_.y,
                  this->size_.x + (this->style_->padding_.left + this->style_->padding_.right),
                  this->size_.y + (this->style_->padding_.top + this->style_->padding_.bottom),
                  style->background_);

    // Draw the button border
    tft.drawRect(this->pos_.x, this->pos_.y,
                  this->size_.x + (this->style_->padding_.left + this->style_->padding_.right),
                  this->size_.y + (this->style_->padding_.top + this->style_->padding_.bottom),
                  this->style_->foreground_);

    // Draw the button text
    tft.setTextColor(style->foreground_);
    tft.setTextSize(this->style_->text_size_);
    tft.drawString(this->text_, this->pos_.x + this->style_->padding_.left, this->pos_.y + this->style_->padding_.bottom);
  }
}