/*
  ==============================================================================

    PatchCableTest.h
    Created: 18 Apr 2025 7:01:06pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PatchCable.h"

class PatchCableTest : public UnitTest {
public:
    PatchCableTest() : UnitTest("PatchCable Test") {}

    void runTest() {
        // PatchCable is entirely UI drawing logic, and not well suited for unit testing
    }
};