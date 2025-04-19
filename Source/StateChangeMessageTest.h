/*
  ==============================================================================

    StateChangeMessageTest.h
    Created: 18 Apr 2025 6:55:35pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "StateChangeMessage.h"


class StateChangeMessageTest : public UnitTest {
public:
    StateChangeMessageTest() : UnitTest("StateChangeMessage Test") {}

    virtual void runTest() override {
        beginTest("Test");
        ModuleState moduleToAdd(15);
        StateChangeMessage testMessage(moduleToAdd, PLUGIN_STATE_MESSAGE_OP_CODE::ADD);
        expect(testMessage.op == PLUGIN_STATE_MESSAGE_OP_CODE::ADD);
        expect(testMessage.state.getId() == 15);
    }
};