/*
  ==============================================================================

    ModuleProcessorTest.h
    Created: 18 Apr 2025 6:54:34pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleProcessor.h"

class TestModuleProcessor : public ModuleProcessor {
public:
    TestModuleProcessor(int id, int cvIn, int cvOut) : ModuleProcessor(id, cvIn, cvOut) {}
    void updateFromState(ModuleState state) {}
    void processSample() {}
};

class ModuleProcessorTest : public UnitTest {
public:
    ModuleProcessorTest() : UnitTest("ModuleProcessor Test") {}

    void runTest() {
        
        TestModuleProcessor mod1(0, 2, 2);
        TestModuleProcessor mod2(1, 3, 3);
        
        beginTest("CV getters");
        expect(mod1.getCVInputJack(55) == nullptr);
        expect(mod1.getCVInputJack(0) != nullptr);
        expect(mod1.getCVOutputJack(-30) == nullptr);
        expect(mod1.getCVOutputJack(1) != nullptr);

        beginTest("CV setters");
        mod1.setCVInputJack(0, mod2.getCVOutputJack(0));
        expect(mod1.getCVInputJack(0)->read() == 0);
        mod2.getCVOutputJack(0)->write(150);
        expect(mod1.getCVInputJack(0)->read() == 150);
        mod2.getCVOutputJack(0)->write(-1);
        expect(mod1.getCVInputJack(0)->read() == -1);
    }
};