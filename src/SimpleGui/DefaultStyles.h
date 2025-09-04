#pragma once

#include "UIStyle.h"

namespace SGui{
/* TODO:
 * Figure out how to prevent accidental overriding from other components
*/
  static UIStyle* DEFAULT_STYLE = new UIStyle(
    (UIColor)WHITE, // foreground
    (UIColor)BLACK, // background
    (UIColor)RED, // border color
    1, // border thickness
    1, // text size
    UIBoxSpacing{
      0,
      0,
      0,
      0
    } // padding
  );

  static UIStyle* DEFAULT_STYLE_FOCUSED = new UIStyle(DEFAULT_STYLE->HighContrast());
}
