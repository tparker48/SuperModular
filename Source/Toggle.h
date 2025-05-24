/*
  ==============================================================================

    Toggle.h
    Created: 27 Apr 2025 2:29:55pm
    Author:  Tom

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

#include "ToggleButtonLookAndFeel.h"

class Toggle : public ToggleButton {
public:
    Toggle() : ToggleButton() {
        blf = new ToggleButtonLookAndFeel();
        this->setLookAndFeel(blf);
    }

    ~Toggle() {
        this->setLookAndFeel(nullptr);
        delete blf;
    }
    
private:
    ToggleButtonLookAndFeel* blf;
};