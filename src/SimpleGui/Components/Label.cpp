//
// Created by rondotdll on 8/24/2024.
//

#include <TFT_eSPI.h>

#include "Label.h"

namespace SGui {
  // Set the text of the label
  Label* Label::SetText(String text) {
    this->text_ = text;
    this->size_.x = tft.textWidth(text) * this->text_size_;
    this->size_.y = tft.fontHeight() * this->text_size_;
    return this;
  }

  // Set the text size of the label (in pt)
  // Default: 1
  Label* Label::SetTextSize(int text_size) {
    this->text_size_ = text_size;
    this->size_.x = tft.textWidth(this->text_) * this->text_size_;
    this->size_.y = tft.fontHeight() * this->text_size_;
    return this;
  }

  // Draw the label
  void Label::Draw() {
    tft.setTextColor(this->style_->foreground_);
    tft.setTextSize(this->text_size_);
    tft.drawString(this->text_, this->pos_.x, this->pos_.y);
  }
}  // namespace SGui
