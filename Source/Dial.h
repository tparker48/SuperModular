/*
  ==============================================================================

    Dial.h
    Created: 26 Apr 2025 8:21:22pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DialLookAndFeel.h"

class Dial : public Slider {
public:
    Dial() : Slider() {
        auto dark = Colour(35, 35, 35);
        auto light = Colour(230, 230, 230);
        this->setSliderStyle(Slider::RotaryHorizontalDrag);
        this->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

        dlf = new DialLookAndFeel(dark, light);
        this->setLookAndFeel(dlf);
    }

    ~Dial() {
        this->setLookAndFeel(nullptr);
        delete dlf;
    }


    
private:
    DialLookAndFeel* dlf;
};