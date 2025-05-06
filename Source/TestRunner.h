/*
  ==============================================================================

    TestRunner.h
    Created: 17 Apr 2025 11:05:00pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SharedPluginStateTest.h"
#include "PluginStateTest.h"
#include "ModuleStateTest.h"
#include "StateChangeMessageTest.h"
#include "ModuleUITest.h"
#include "ModuleProcessorTest.h"
#include "CVJackComponentTest.h"
#include "PatchCableManagerTest.h"

class TestRunner : public UnitTestRunner {
public:
    void runAllTests() {
        SharedPluginStateTest sharedState;
        PluginStateTest pluginState;
        ModuleStateTest moduleState;
        StateChangeMessageTest stateChangeMessage;
        ModuleUITest moduleUI;
        ModuleProcessorTest moduleProcessor;
        CVJackComponentTest cvJackComponent;
        PatchCableManagerTest patchCableManager;

        setAssertOnFailure(false);
        UnitTestRunner::runAllTests();

        // Throw exception if we failed a test
        for (int i = 0; i < getNumResults(); i++) {
            jassert(getResult(i)->failures == 0);
        }
    }

};