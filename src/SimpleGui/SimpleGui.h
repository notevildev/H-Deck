#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <utility>
#include <vector>

#include "Types/pins.h"
#include "Types/enums.h"
#include "Types/input.h"

#include "helper.h"

#include "Types/UIColor.h"
#include "Types/UIStyle.h"

#include "Components/UIComponents.h"

#include "GuiManager.h"

namespace SGui {
  // Initialize the TFT display
  // returns a pointer to the TFT_eSPI object
  GUIManager* Init();
};
