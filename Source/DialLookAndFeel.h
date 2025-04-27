#pragma once
#include <JuceHeader.h>

class DialLookAndFeel : public juce::LookAndFeel_V4 {

public:
    DialLookAndFeel(Colour dark, Colour light)
    {
        this->dark = dark;
        this->light = light;

        minRotation = -2.0 / 3.0 * MathConstants<float>::pi;
        maxRotation = -2.0 * MathConstants<float>::pi - (1.0 / 3.0) * MathConstants<float>::pi;

        this->setColour(Slider::ColourIds::textBoxOutlineColourId, Colour());
    }

    void setColors(Colour dark, Colour light)
    {
        this->dark = dark;
        this->light = light;
    }

    void drawRotarySlider(Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, Slider&) override;

private:
    Colour dark, light;
    float minRotation, maxRotation;
};
