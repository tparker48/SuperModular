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
    PatchCableManager();
    ~PatchCableManager() {}

    bool isDraggingCable() { return dragSource != nullptr; }
    PatchCable* getDragCable() { return &liveDragCable; }
    void setDraggedCable(Component* source);
    void clearDraggedCable();
    void updateDraggedCablePosition(const MouseEvent& e);



private:
    Component* dragSource = nullptr;
    PatchCable liveDragCable;
};