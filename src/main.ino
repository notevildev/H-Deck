#include <SPI.h>
#include <TFT_eSPI.h>

#include "main.h"

#include "SimpleGui/SimpleGui.h"
#include "SimpleGui/HID/DPad.h"

#define MAX_FRAMERATE 15

constexpr double frame_sleep = 1000 / MAX_FRAMERATE;

__always_inline void powerBoard() {
  pinMode(POWER_ON_P, OUTPUT);
  digitalWrite(POWER_ON_P, HIGH);
}

__always_inline void powerTFT() {
  pinMode(TFT_BACKLIGHT_P, OUTPUT);
  digitalWrite(TFT_BACKLIGHT_P, HIGH);
}

using namespace SGui;

static Window* window;
static GUIManager* gui;

void setup() {
  Serial.begin(115200);

  powerBoard();
  pinMode(TFT_BACKLIGHT_P, OUTPUT);
  digitalWrite(TFT_BACKLIGHT_P, HIGH);

  Serial.println("Initializing TFT...");
  gui = Init();
  Serial.println("Enabling inputs...");
  gui->enable_dpad_navigation(nullptr); // TODO: REMEMBER TO FIX THIS!!!

  Serial.println("Creating window...");

  window = new Window();
  window->SetColor((SGui::UIColor)RED);
  Serial.println("Setting title...");
  window->SetTitle("Example Window");

  Serial.println("Adding children...");
  window->AddChildren(
      {
        (new Label("Hello, World!"))->SetTextSize(2),
        (new Button("Button 1")),
        (new Button("Button 2")),
        (new Label("This is an example application using the SGui library!"))
      }
    );

  Serial.println("Adding window to GUI...");
  gui->add_window(window);

  Serial.println("Binding input events...");
}

void loop() {
  // Main loop
  /* 1.) Handle Input
   * 2.) Execute Task(s)
   * 3.) Update UI
   * 4.) Render
   */

  // Serial.println("Handling Inputs...");
  gui->handle_inputs();
  // Serial.println("Drawing UI...");
  gui->render();
  // Serial.println("Looping...");
  delay(frame_sleep); // Wait for the next frame (as not to overload the Display or the CPU)
}
