/*
  ==============================================================================

    PluginStateTest.h
    Created: 17 Apr 2025 11:40:34pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginState.h"


class PluginStateTest : public UnitTest {
public:
    PluginStateTest() : UnitTest("PluginState Test") {}

    virtual void runTest() override {

        
        beginTest("Default Constructor");
        PluginState defaultConstructor;
        expect(defaultConstructor.moduleStates.size() == 0);

        beginTest("XML Constructor");
        PluginState preXml;
        auto bounds1 = Rectangle<int>(1, 2, 3, 4);
        auto bounds2 = Rectangle<int>(30, 125, 0, 9);
        preXml.moduleStates.push_back(ModuleState(100, 5, bounds1, 123, 321));
        preXml.moduleStates.push_back(ModuleState(200, 15, bounds2, 1, 5));
        auto xml = preXml.toXml();
        PluginState postXml(xml);
        delete xml;
        ModuleState* mod1 = postXml.getModule(100);
        expect(mod1->getId() == 100);
        expect(mod1->getTypeId() == 5);
        expect(mod1->getBounds() == bounds1);
        expect(mod1->getNumCvInputs() == 123);
        expect(mod1->getNumCvOutputs() == 321);
        ModuleState* mod2 = postXml.getModule(200);
        expect(mod2->getId() == 200);
        expect(mod2->getTypeId() == 15);
        expect(mod2->getBounds() == bounds2);
        expect(mod2->getNumCvInputs() == 1);
        expect(mod2->getNumCvOutputs() == 5);

        beginTest("Add/Get/Remove Module");
        PluginState state;
        defaultConstructor.removeModule(1);
        expect(state.getModule(-100) == nullptr);
        state.moduleStates.push_back(ModuleState(17));
        state.removeModule(17);
        expect(state.getModule(17) == nullptr);
        state.moduleStates.push_back(ModuleState(300));
        expect(state.getModule(300)->getId() == 300);
    }
};