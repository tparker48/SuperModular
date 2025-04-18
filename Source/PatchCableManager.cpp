/*
  ==============================================================================

    PatchCableManager.cpp
    Created: 6 Apr 2025 4:31:46pm
    Author:  Tom

  ==============================================================================
*/

#include "PatchCableManager.h"

PatchCableManager::PatchCableManager() {
    liveDragCable.setColour(Colour(0x88FFFFFF));
    liveDragCable.setEndColour(Colour(0xFFFFFFFF));
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