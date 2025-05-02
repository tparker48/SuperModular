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
        this->setSliderStyle(Slider::RotaryHorizontalDrag);
        this->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        dlf = new DialLookAndFeel(Colour(35, 35, 35), Colour(230, 230, 230));
        this->setLookAndFeel(dlf);
    }

    Dial(Colour knobColour, Colour dotColour) : Slider() {
        this->setSliderStyle(Slider::RotaryHorizontalDrag);
        this->setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        dlf = new DialLookAndFeel(knobColour, dotColour);
        this->setLookAndFeel(dlf);
    }

    ~Dial() {
        this->setLookAndFeel(nullptr);
        delete dlf;
    }

    void setColours(Colour knob, Colour dot) {
        this->setLookAndFeel(nullptr);
        delete dlf;
        dlf = new DialLookAndFeel(knob, dot);
        this->setLookAndFeel(dlf);
    }


    
private:
    DialLookAndFeel* dlf;
};