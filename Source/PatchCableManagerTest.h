/*
  ==============================================================================

    PatchCableManagerTest.h
    Created: 18 Apr 2025 6:55:55pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PatchCableManager.h"


class PatchCableManagerTest : public UnitTest {
public:
    PatchCableManagerTest() : UnitTest("PatchCableManager Test") {}

    void runTest() {
        beginTest("Test");
        PatchCableManager cables;
        expect(cables.isDraggingCable() == false);
        expect(cables.getDragCable() != nullptr);
        cables.clearDraggedCable();

        Component testSource;
        cables.setDraggedCable(&testSource);
        expect(cables.isDraggingCable() == true);

        cables.clearDraggedCable();
        expect(cables.isDraggingCable() == false);
    }
};