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
        osc.setHz(1.0);

        osc.processSample(); // 0
        osc.processSample(); // 1
        osc.processSample(); // 2
        osc.processSample(); // 3 (wraps to 0)
        
        auto sinOut = *(osc.getCVOutputJack(0)->getPtr());
        auto expSinOut = sin(0.0);
        expect(abs(expSinOut - sinOut) < 1e-10);

        OscillatorProcessor osc2(1);
        osc2.prepareToPlay(1000, 0);
        osc2.setHz(100.0);

        for (int i = 0; i <= 1000; i++) {
            osc2.processSample();
        }
     
        sinOut = *(osc2.getCVOutputJack(0)->getPtr());
        expSinOut = sin(0.0);
        expect(abs(expSinOut - sinOut) < 1e-10);
    }
};