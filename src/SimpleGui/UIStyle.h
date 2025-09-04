#pragma once

#include "types.h"
#include "UIColor.h"

namespace SGui{
  class UIStyle {
  public:
    UIColor foreground_;
    UIColor background_;
    UIColor border_color_;

    uint8_t border_thickness_;
    uint8_t text_size_;

    UIBoxSpacing padding_;

    UIStyle(UIColor foreground, UIColor background, UIColor border_color, uint8_t border_thickness, uint8_t text_size, UIBoxSpacing padding)
    : foreground_(foreground), background_(background), border_color_(border_color), border_thickness_(border_thickness), text_size_(text_size), padding_(padding) {}

    // Returns a high contrast version of this style (NOT A POINTER)
    // inverts background and foreground
    // [used for focused components]
    __always_inline UIStyle HighContrast() const {
      return {
        this->background_,
        this->foreground_,
        this->border_color_,

        this->border_thickness_,
        this->text_size_,

        this->padding_
      };
    }

    // Set the style's foreground color
    __always_inline UIStyle* SetForeground(UIColor& color) {
      this->foreground_ = color;
      return this;
    }

    // Set the style's background color
    __always_inline UIStyle* SetBackground(UIColor& color) {
      this->background_ = color;
      return this;
    }

    // Set the style's border color
    __always_inline UIStyle* SetBorderColor(UIColor& color) {
      this->border_color_ = color;
      return this;
    }

    // Set the style's border thickness
    __always_inline UIStyle* SetBorderThickness(uint8_t thickness) {
      this->border_thickness_ = thickness;
      return this;
    }

    // Set the style's text size multiplier
    // Default: 1
    __always_inline UIStyle* SetTextSize(uint8_t text_size) {
      this->text_size_ = text_size;
      return this;
    }

    // Set the style's padding
    __always_inline UIStyle* SetPadding(UIBoxSpacing padding) {
      this->padding_ = padding;
      return this;
    }
    

  };
}