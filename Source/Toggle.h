/*
  ==============================================================================

    Toggle.h
    Created: 27 Apr 2025 2:29:55pm
    Author:  Tom

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

#include "ButtonLookAndFeel.h"

class Toggle : public ToggleButton {
public:
    Toggle() : ToggleButton() {
        auto dark = Colour(35, 35, 35);
        auto light = Colour(230, 230, 230);

        blf = new ButtonLookAndFeel(dark, light);
        this->setLookAndFeel(blf);
    }

    ~Toggle() {
        this->setLookAndFeel(nullptr);
        delete blf;
    }

    
private:
    ButtonLookAndFeel* blf;
};