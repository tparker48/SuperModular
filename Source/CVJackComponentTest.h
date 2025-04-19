/*
  ==============================================================================

    CVJackComponentTest.h
    Created: 18 Apr 2025 7:00:49pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CVJackComponent.h"

class CVJackComponentTest : public UnitTest {
public:
    CVJackComponentTest() : UnitTest("CVJackComponent Test") {}

    void runTest() {
        beginTest("Test");
        PatchCableManager cm;
        SharedPluginState state;
        SharedStateWriter writer(&state);
        CVJackComponent jack1(CVInput, 0, 1, &cm, &writer);
        CVJackComponent jack2(CVOutput, 0, 1, &cm, &writer);
        Component parentComponent;
        parentComponent.setBounds(0, 0, 0, 0);
        parentComponent.addAndMakeVisible(jack1);
        parentComponent.addAndMakeVisible(jack2);

        expect(jack1.getType() == CVInput);
        expect(jack2.getType() == CVOutput);
        expect(jack1.getModuleId() == 1);
        expect(jack1.getId() == 0);
        expect(jack1.getConnection()==nullptr);

        jack1.setConnection(&jack2);
        expect(jack1.getConnection() == &jack2);
        expect(jack1.isConnected());

        jack1.clearConnection();
        expect(jack1.getConnection() == nullptr);
        expect(!jack1.isConnected());
    }
};