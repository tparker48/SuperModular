/*
  ==============================================================================

    Button.h
    Created: 24 May 2025 2:06:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

#include "PushButtonLookAndFeel.h"

class PushButton : public Button {
public:
    PushButton() : Button("") {
        blf = new PushButtonLookAndFeel();
        //this->setLookAndFeel(blf);
    }

    PushButton(std::string& buttonName) : Button(buttonName) {
        blf = new PushButtonLookAndFeel();
        //this->setLookAndFeel(blf);
    }

    ~PushButton() {
        this->setLookAndFeel(nullptr);
        delete blf;
    }

    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto m = 2;

        Colour bg, fg;

        if (shouldDrawButtonAsDown) {
            bg = Colours::black;
            fg = Colours::white;
        }
        else {
            bg = Colours::white;
            fg = Colours::black;
        }

        g.setColour(Colours::black);
        g.fillRect(Rectangle<float>(m-1, m-1, getWidth() - 2*(m-1), getHeight() - 2*(m-1)));

        g.setColour(bg);
        g.fillRect(Rectangle<float>(m, m, getWidth() - 2 * m -1, getHeight() - 2 * m -1));
        
        g.setColour(fg);
        g.drawText(getButtonText(), Rectangle<float>(0, 0, getWidth(), getHeight()), Justification::centred);
        

    }

private:
    PushButtonLookAndFeel* blf;
};