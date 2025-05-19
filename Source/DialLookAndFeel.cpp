/*
  ==============================================================================

    DialLookAndFeel.cpp
    Created: 6 Sep 2020 11:44:05pm
    Author:  Tom

  ==============================================================================
*/

#include "DialLookAndFeel.h"


void DialLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
    float sliderPosProportional, float rotaryStartAngle,
    float rotaryEndAngle, Slider&)
{
    auto margin = height * 0.15;
    x += margin;
    y += margin;
    width -= 2 * margin;
    height -= 2 * margin;

    // dial
    g.setColour(Colours::black);
    g.fillEllipse(x+2, y+2, width, height);

    g.setColour(Colours::white);
    g.fillEllipse(x, y, width, height);

    g.setColour(Colours::black);
    g.drawEllipse(x, y, width, height, 1.0);

    auto pad = height * 0.1;
    g.drawEllipse(x+ pad, y + pad, width - 2 * pad, height - 2 * pad, 1.0);

    // marker
    float originX = x + width / 2.0;
    float originY = y + height / 2.0;
    float radius = (width / 2.0);
    float markerRadius = radius / 5.0;

    float rotationPercentage = 1.0-sliderPosProportional;
    float rotation = minRotation + rotationPercentage * (maxRotation - minRotation);

    //float markerX = (radius*.75) * -cos(rotation);
    //float markerY = (radius*.75) * -sin(rotation);
    float markerX = (radius*.80) * -cos(rotation);
    float markerY = (radius*.80) * -sin(rotation);

    g.setColour(Colours::black);
    g.drawLine(originX, originY, originX + markerX, originY + markerY, 2.0);

    return;
    g.setColour(Colours::black);
    g.fillEllipse(
        originX + markerX - markerRadius/2, 
        originY + markerY - markerRadius/2, 
        markerRadius, 
        markerRadius
        );

}