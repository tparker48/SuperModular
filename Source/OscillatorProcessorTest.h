/*
  ==============================================================================

    OscillatorProcessorTest.h
    Created: 18 Apr 2025 6:55:00pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "OscillatorProcessor.h"

class OscillatorProcessorTest : public UnitTest {
public:
    OscillatorProcessorTest() : UnitTest("OscillatorProcessor Test") {}

    void runTest() {
        beginTest("Phase increment test");
        OscillatorProcessor osc(1);
        osc.prepareToPlay(3, 0);
        osc.updateHz(1.0);
        CVOutputJack jout;
        jout.write(0.0);
        osc.setCVInputJack(1, &jout);

        osc.processSample();
        osc.processSample();
        osc.processSample();

        auto sinOut = *(osc.getCVOutputJack(0)->getPtr());
        auto expSinOut = sin(0.0);
        expect(abs(expSinOut - sinOut) < 1e-10);

        osc = OscillatorProcessor(1);
        osc.prepareToPlay(100, 0);
        osc.updateHz(1.0);
        for (int i = 0; i < 100; i++) {
            osc.processSample();
        }
        sinOut = *(osc.getCVOutputJack(0)->getPtr());
        expSinOut = sin(0.0);
        expect(abs(expSinOut - sinOut) < 1e-10);
    }
};