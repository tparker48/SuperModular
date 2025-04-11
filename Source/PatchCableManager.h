/*
  ==============================================================================

    PatchCableManager.h
    Created: 6 Apr 2025 4:31:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PatchCable.h"

class PatchCableManager {
public:
    PatchCableManager() {
        liveDragCable.setColour(Colour(0x88FFFFFF));
        liveDragCable.setEndColour(Colour(0xFFFFFFFF));
        liveDragCable.setAsDraggerCable();
    }
    ~PatchCableManager() {}

    bool isDraggingCable() {
        return dragSource != nullptr;
    }

    Component* getDraggedCable() {
        return dragSource;
    }

    void setDraggedCable(Component* source) {
        if (dragSource == nullptr) {
            dragSource = source;
        }
    }

    void clearDraggedCable() {
        dragSource = nullptr;
        liveDragCable.setBounds(-1, -1, -1, -1);
    }

    void updateDraggedCablePosition(const MouseEvent& e) {
        if (dragSource != nullptr) {
            liveDragCable.setPoints(dragSource, e);
        }
    }

    PatchCable* getDragCable() { return &liveDragCable; }

private:
    Component* dragSource = nullptr;
    PatchCable liveDragCable;
};