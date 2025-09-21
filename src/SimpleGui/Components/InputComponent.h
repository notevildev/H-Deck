#pragma once

#include "SimpleGui/GuiManager.h"
#include "Component.h"

//
// Created by David J on 9/18/25.
//


namespace SGui {
    /* Wrapper helper-class to allow input components to access data
     * stored in a GUIManager instance.
     * (Allows input components to handle their own inputs)
     */
    class InputComponent : public Component {
    public:
      InputComponent() = default;
      explicit InputComponent(int x = 0) : Component() {};

      component_type_t type() const override { return CONTROL; };

      // Handle an input event (returns true if the event was handled)
      virtual bool handle_input_event(input_event_t event, void* manager) { return 0; };
    };
} // SGui
