/*
  ==============================================================================

    ModuleUITest.h
    Created: 18 Apr 2025 6:53:40pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleUI.h"

class TestModuleUI : public ModuleUI {
public:
    TestModuleUI(
        MODULE_ID id,
        ModuleGrid* mg,
        PatchCableManager* cm,
        SharedStateWriter* stateWriter,
        int numCvIns,
        int numCvOuts
    ) : ModuleUI(id, mg, cm, stateWriter, numCvIns, numCvOuts) {}

    void paint(Graphics& g) {}
    void resized() {}
};

class ModuleUITest : public UnitTest {
public:
    ModuleUITest() : UnitTest("ModuleUI Test") {}

    virtual void runTest() override {
        ModuleGrid moduleGrid;
        PatchCableManager cableManager;
        SharedPluginState sharedState;
        SharedStateWriter stateWriter(&sharedState);
        moduleGrid.setRackDimensions(1, 100, 20, 5);
        auto bounds = Rectangle<int>(100, 100, 20, 100);

        beginTest("Constructor");
        TestModuleUI testModule(15, & moduleGrid, & cableManager, & stateWriter, 12, 18);
        testModule.setBounds(Rectangle<int>(200, 300, 40, 100));
        moduleGrid.addModule(testModule.getId(), &testModule);

        expect(testModule.getId() == 15);
        expect(testModule.getNumCVInputs() == 12);
        expect(testModule.getNumCVOutputs() == 18);

        beginTest("Apply State - Bounds Setting");
        ModuleGrid testGrid;
        TestModuleUI testMod(0, &testGrid, &cableManager, &stateWriter, 0, 0);
        Rectangle<int> startingBounds(0, 0, 20, 10);
        
        testMod.setBounds(startingBounds);
        testGrid.setRackDimensions(1, 10, 10, 10);
        testGrid.addModule(testMod.getId(), &testMod);
        expect(testMod.getBounds() == startingBounds);

        Rectangle<int> targetBounds(500, 500, 20, 10);
        Rectangle<int> expectedBounds(80, 0, 20, 10);
        ModuleState newBoundsState(0, 0, targetBounds, 0, 0);
        testMod.applyState(newBoundsState);
        expect(testMod.getBounds() == expectedBounds);

        beginTest("Apply State - CV");
        TestModuleUI otherModule(30, &moduleGrid, &cableManager, &stateWriter, 5, 5);
        otherModule.setBounds(bounds);
        moduleGrid.addModule(otherModule.getId(), &otherModule);
        ModuleState testState(15, 0, bounds, 12, 18);
        testState.setInputCvConnection(5, otherModule.getId(), 4);
        testState.setOutputCvConnection(0, otherModule.getId(), 0);

        testModule.applyState(testState);
        expect(testModule.getCvInputJack(5)->getConnection()->getModuleId() == otherModule.getId());
        expect(testModule.getCvInputJack(5)->getConnection()->getId() == 4);
        expect(testModule.getCvOutputJack(0)->getConnection()->getModuleId() == otherModule.getId());
        expect(testModule.getCvOutputJack(0)->getConnection()->getId() == 0);

        // UI logic like mouse callbacks, resize, and painting are not easily unit tested.
    }
};