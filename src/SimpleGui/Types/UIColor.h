#pragma once

#include <cstdint>

namespace SGui {
  // bunch of pre-compressed color definitions for convenience
  // (taken from TFT_eSPI.h)
  #define BLACK         0x000000    /*   0,   0,   0 */
  #define NAVY          0x000080    /*   0,   0, 128 */
  #define DARKGREEN     0x008000    /*   0, 128,   0 */
  #define DARKCYAN      0x008080    /*   0, 128, 128 */
  #define MAROON        0x800000    /* 128,   0,   0 */
  #define PURPLE        0x800080    /* 128,   0, 128 */
  #define OLIVE         0x808000    /* 128, 128,   0 */
  #define LIGHTGREY     0xD3D3D3    /* 211, 211, 211 */
  #define DARKGREY      0x808080    /* 128, 128, 128 */
  #define BLUE          0x0000FF    /*   0,   0, 255 */
  #define GREEN         0x00FF00    /*   0, 255,   0 */
  #define CYAN          0x00FFFF    /*   0, 255, 255 */
  #define RED           0xFF0000    /* 255,   0,   0 */
  #define MAGENTA       0xFF00FF    /* 255,   0, 255 */
  #define YELLOW        0xFFFF00    /* 255, 255,   0 */
  #define WHITE         0xFFFFFF    /* 255, 255, 255 */
  #define ORANGE        0xFFB400    /* 255, 180,   0 */
  #define GREENYELLOW   0xB4FF00    /* 180, 255,   0 */
  #define PINK          0xFFC0CB    /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
  #define BROWN         0x964B00    /* 150,  75,   0 */
  #define GOLD          0xFFD700    /* 255, 215,   0 */
  #define SILVER        0xC0C0C0    /* 192, 192, 192 */
  #define SKYBLUE       0x87CEEB    /* 135, 206, 235 */
  #define VIOLET        0xB42EE2    /* 180,  46, 226 */

  class UIColor {
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;

  public:
    // Define a color using RGB format
    UIColor(uint8_t r, uint8_t g, uint8_t b)
        : red_(r), green_(g), blue_(b) {}

    // Define a color using 24-bit Hexadecimal
    explicit UIColor(uint32_t hex)
        : red_((hex & 0xFF0000) >> 16),
          green_((hex & 0x00FF00) >> 8),
          blue_(hex & 0x0000FF) {}

    // Define a color using 16-bit Hexadecimal
    explicit UIColor(uint16_t hex)
        : red_((hex & 0xF800) >> 8),       // Extract and shift red
          green_((hex & 0x07E0) >> 3),     // Extract and shift green
          blue_((hex & 0x001F) << 3) {}    // Extract and shift blue

    // Generic constructor for integer, defaults to 24-bit interpretation
    explicit UIColor(int hex)
        : UIColor(static_cast<uint32_t>(hex)) {}  // Forward to 24-bit constructor

    // Method to compress the color to 16-bit format
    uint16_t Compress() const {
      return ((red_ & 0b11111000) << 8) |
             ((green_ & 0b11111100) << 3) |
             (blue_ >> 3);
    }

    // Implicit conversion to 16-bit format
    operator uint16_t() const {
      return ((red_ & 0b11111000) << 8) |
             ((green_ & 0b11111100) << 3) |
             (blue_ >> 3);
    }
    operator uint32_t() const {
      return ((red_ & 0b11111000) << 8) |
             ((green_ & 0b11111100) << 3) |
             (blue_ >> 3);
    }

    // Determine the appropriate text color (black or white) based on background color
    UIColor GetContrast() {
      // Calculate luminance using the ITU-R BT.601 formula
      // Y = 0.299 * R + 0.587 * G + 0.114 * B
      uint8_t luminance = (uint8_t)(0.299 * this->red_ + 0.587 * this->green_ + 0.114 * this->blue_);

      // If luminance is less than 128, return white; otherwise, return black
      return (luminance < 128) ? (UIColor)WHITE : (UIColor)BLACK;
    }

    // Returns the inverse of this color (NOT A POINTER)
    UIColor Invert() const {
      return {(uint8_t)~this->red_, (uint8_t)~this->green_, (uint8_t)~this->blue_};
    }


  };
}