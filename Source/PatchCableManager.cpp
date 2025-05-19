/*
  ==============================================================================

    PatchCableManager.cpp
    Created: 6 Apr 2025 4:31:46pm
    Author:  Tom

  ==============================================================================
*/

#include "PatchCableManager.h"

PatchCableManager::PatchCableManager() {
    liveDragCable.setColours(Colour(0x66000000), Colour(0x66FFFFFF));
    liveDragCable.setEndColour(Colour(0xFF000000));
    liveDragCable.setAsDraggerCable();
}

void PatchCableManager::setDraggedCable(Component* source) {
    if (dragSource == nullptr) {
        dragSource = source;
    }
}

void PatchCableManager::clearDraggedCable() {
    dragSource = nullptr;
    liveDragCable.setBounds(-1, -1, -1, -1);
}

void PatchCableManager::updateDraggedCablePosition(const MouseEvent& e) {
    if (dragSource != nullptr) {
        liveDragCable.setPoints(dragSource, e);
    }
}