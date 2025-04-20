/*
  ==============================================================================

    PatchCable.h
    Created: 6 Apr 2025 4:31:31pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PatchCable : public Component {
public:
    PatchCable();

    void setAsDraggerCable();

    void setRandomColor();

    void setColour(Colour c);

    void setEndColour(Colour c);

    void paint(Graphics& g);

    void drawPatchLine(Graphics& g, int x, int y, int w, int h);

    void setPoints(Component* c1, Component* c2);

    void setPoints(Component* c1, const MouseEvent e);

private:
    int margin = 200;
    bool isDraggerCable = false;
    Colour cableColor;
    Colour endColor;
    Point<int> p1, p2;

    void updateBounds();
};