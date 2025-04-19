/*
  ==============================================================================

    TestRunner.h
    Created: 17 Apr 2025 11:05:00pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleStateTest.h"
#include "PluginStateTest.h"
#include "ModuleUITest.h"

class TestRunner : public UnitTestRunner {
public:
    void runAllTests() {
        // list tests here
        ModuleStateTest moduleState;
        ModuleUITest moduleUI;
        PluginStateTest pluginState;

        setAssertOnFailure(false);
        UnitTestRunner::runAllTests();

        // Throw exception if we failed a test
        for (int i = 0; i < getNumResults(); i++) {
            jassert(getResult(i)->failures == 0);
        }
    }

};