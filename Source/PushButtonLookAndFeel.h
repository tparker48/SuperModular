#pragma once
#include <JuceHeader.h>

class PushButtonLookAndFeel : public LookAndFeel_V4
{
public:
    void drawButtonBackground (Graphics &g, Button &b, const Colour &backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        if (shouldDrawButtonAsHighlighted) {
            g.setColour(Colours::red);
        }
        else {
            g.setColour(Colours::blue);
        }
        if (shouldDrawButtonAsDown) {
            g.setColour(Colours::yellow);
        }

        g.fillRect(b.getBounds().toFloat());
    }

};
