/*
  ==============================================================================

    ModuleStateTest.h
    Created: 17 Apr 2025 11:15:34pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ModuleState.h"


class ModuleStateTest : public UnitTest {
public:
    ModuleStateTest() : UnitTest("ModuleState Test") {}

    virtual void runTest() override {
        beginTest("ID Constructor");
        ModuleState idConstructor(33);
        expect(idConstructor.getId() == 33);
        idConstructor.getTypeId();
        idConstructor.getBounds();
        idConstructor.getNumCvInputs();
        idConstructor.getNumCvOutputs();

        beginTest("Full Constructor");
        auto bounds = Rectangle<int>(1, 2, 3, 4);
        ModuleState fullConstructor(-1, 50, bounds, 14, 15);
        expect(fullConstructor.getId() == -1);
        expect(fullConstructor.getTypeId() == 50);
        expect(fullConstructor.getBounds() == bounds);
        expect(fullConstructor.getNumCvInputs() == 14);
        expect(fullConstructor.getNumCvOutputs() == 15);
        
        beginTest("XML Constructor");
        bounds = Rectangle<int>(13, 12, 15, 55);
        ModuleState xmlStarter(500, 600, bounds, 17, 23);
        auto xml = xmlStarter.toXml();
        ModuleState xmlConstructor(*xml);
        expect(xmlConstructor.getId() == 500);
        expect(xmlConstructor.getTypeId() == 600);
        expect(xmlConstructor.getBounds() == bounds);
        expect(xmlConstructor.getNumCvInputs() == 17);
        expect(xmlConstructor.getNumCvOutputs() == 23);
        delete xml;

        beginTest("Set/Get Bounds");
        ModuleState boundsTestModule;
        bounds = Rectangle<int>(55, 0, 100000, -15);
        boundsTestModule.setBounds(bounds);
        expect(boundsTestModule.getBounds() == bounds);

        beginTest("Set/Get CV Connections");
        ModuleState cvTestModule(1,1,bounds, 100, 10);
        expect(cvTestModule.getInputCvConnection(10000).first == -1);
        expect(cvTestModule.getInputCvConnection(10000).second == -1);

        expect(cvTestModule.getInputCvConnection(99).first == -1);
        expect(cvTestModule.getInputCvConnection(99).second == -1);
        cvTestModule.setInputCvConnection(99, 15, 0);
        expect(cvTestModule.getInputCvConnection(99).first == 15);
        expect(cvTestModule.getInputCvConnection(99).second == 0);

        expect(cvTestModule.getOutputCvConnection(-1).first == -1);
        expect(cvTestModule.getOutputCvConnection(-1).second == -1);
        cvTestModule.setOutputCvConnection(-1, 123, 321);
        expect(cvTestModule.getOutputCvConnection(-1).first == -1);
        expect(cvTestModule.getOutputCvConnection(-1).second == -1);

        expect(cvTestModule.getOutputCvConnection(3).first == -1);
        expect(cvTestModule.getOutputCvConnection(3).second == -1);
        cvTestModule.setOutputCvConnection(3, 123, 321);
        expect(cvTestModule.getOutputCvConnection(3).first == 123);
        expect(cvTestModule.getOutputCvConnection(3).second == 321);
    }
};