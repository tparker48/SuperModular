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
#include "CVJack.h"

class PatchCableManager {
public:
    JUCE_DECLARE_SINGLETON(PatchCableManager, true);

    PatchCableManager() {
        liveDragCable.setColour(juce::Colour(0x88FFFFFF));
        liveDragCable.setEndColour(juce::Colour(0xFFFFFFFF));
        liveDragCable.setAsDraggerCable();
    }
    ~PatchCableManager() { clearSingletonInstance(); }

    bool isDraggingCable() {
        return dragSource != nullptr;
    }

    CVJack* getDraggedCable() {
        return dragSource;
    }

    void setDraggedCable(CVJack* source) {
        if (dragSource == nullptr) {
            dragSource = source;
        }
    }

    void clearDraggedCable() {
        dragSource = nullptr;
        liveDragCable.setBounds(-1, -1, -1, -1);
    }

    void updateDraggedCablePosition(const juce::MouseEvent& e) {
        if (dragSource != nullptr) {
            liveDragCable.setPoints(dragSource, e);
        }
    }

    PatchCable* getDragCable() { return &liveDragCable; }

private:
    CVJack* dragSource = nullptr;
    PatchCable liveDragCable;
};