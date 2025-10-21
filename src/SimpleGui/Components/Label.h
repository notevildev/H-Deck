#pragma once

#include "../Types/enums.h"
#include "Component.h"

namespace SGui {

// UILabel class for displaying text on the screen
class Label : public Component {
 public:
  String text_ = "";
  int text_size_ = 1;

  explicit Label(String text) : Component() {
      this->text_ = text;
      this->size_.x = tft.textWidth(text);
      this->size_.y = tft.fontHeight();
  };

  // Set the text of the label
  Label* SetText(String text);

  // Set the text size multiplier of the label
  // Default: 1
  Label* SetTextSize(int text_size);

  // Get the label text
  String GetText() const { return this->text_; }

  // Get the text size of the label
  int GetTextSize() const { return this->text_size_; }

  // Draw the label
  void Draw() override;
};

}  // namespace SGui
