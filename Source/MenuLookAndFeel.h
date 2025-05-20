/*
  ==============================================================================

    MenuLookAndFeel.h
    Created: 19 May 2025 6:50:23pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MenuLookAndFeel : public LookAndFeel_V4 {
public:
    MenuLookAndFeel() {
        setColour(PopupMenu::backgroundColourId, Colours::black);
        setColour(PopupMenu::highlightedBackgroundColourId, Colours::white);
        setColour(PopupMenu::textColourId, Colours::white);
        setColour(PopupMenu::highlightedTextColourId, Colours::black);
    }
};