/*
  ==============================================================================

    PatchCable.cpp
    Created: 6 Apr 2025 4:31:31pm
    Author:  Tom

  ==============================================================================
*/

#include "PatchCable.h"

PatchCable::PatchCable() {
    setAlwaysOnTop(true);
    setRandomColor();
}

void PatchCable::setRandomColor() {
    std::vector<Colour> colours = {
        Colour(252, 117, 76),
        Colour(252, 182, 76),
        Colour(76, 252, 111),
        Colour(105, 76, 252),
        Colour(252, 76, 117)
    };
    Random rand;
    rand.setSeedRandomly();
    auto randFloat = rand.nextFloat();
    int idx = int(randFloat * colours.size());
    setColour(colours[idx]);
    setEndColour(colours[idx]);
}

void PatchCable::setAsDraggerCable() {
    isDraggerCable = true;
}

void PatchCable::setColour(Colour c) {
    cableColor = c;
}

void PatchCable::setEndColour(Colour c) {
    endColor = c;

}

void PatchCable::paint(Graphics& g) {
    if (p1.getX() < p2.getX() && p1.getY() < p2.getY() ||
        p1.getX() >= p2.getX() && p1.getY() >= p2.getY()) {
        // negative slope
        drawPatchLine(g,
            0 + margin,
            0 + margin,
            getWidth() - margin,
            getHeight() - margin);
    }
    else {
        // positive or zero slope
        drawPatchLine(g,
            0 + margin,
            getHeight() - margin,
            getWidth() - margin,
            0 + margin);
    }
}

void PatchCable::drawPatchLine(Graphics& g, int x, int y, int w, int h) {
    int radius = 10;
    g.setColour(endColor);
    g.fillEllipse(x - radius / 2, y - radius / 2, radius, radius);
    g.fillEllipse(w - radius / 2, h - radius / 2, radius, radius);

    g.setColour(cableColor);

    Path path;
    Point<float> point1 = Point<float>(x, y);
    Point<float> point2 = Point<float>(w, h);
    int droop = 0.2 * (point1.getDistanceFrom(point2) + 10.0);
    Point<float> control1 = Point<float>((x + w) / 2, (y + h) / 2 + droop);

    path.startNewSubPath(point1);
    path.cubicTo(
        control1,
        control1,
        point2);
    g.strokePath(path, PathStrokeType(5));
}

void PatchCable::setPoints(Component* c1, Component* c2) {
    p1 = c1->getBounds().getCentre() + c1->getParentComponent()->getBounds().getTopLeft();
    p2 = c2->getBounds().getCentre() + c2->getParentComponent()->getBounds().getTopLeft();
    updateBounds();
}

void PatchCable::setPoints(Component* c1, const MouseEvent e) {
    p1 = c1->getBounds().getCentre() + c1->getParentComponent()->getBounds().getTopLeft();
    auto ogError = e.getMouseDownPosition();
    auto cSize = Point<int>(c1->getWidth(), c1->getHeight()) / 2;
    p2 = p1 + e.getOffsetFromDragStart() + ogError - cSize;

    updateBounds();
}

void PatchCable::updateBounds() {
    setBounds(Rectangle<int>(
        std::min(p1.getX(), p2.getX()) - margin,
        std::min(p1.getY(), p2.getY()) - margin,
        std::abs(p2.getX() - p1.getX()) + 2 * margin,
        std::abs(p2.getY() - p1.getY()) + 2 * margin
    ));
}