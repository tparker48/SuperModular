/*
  ==============================================================================

    SplitterProcessor.h
    Created: 17 Apr 2025 12:37:44am
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class SplitterProcessor : public ModuleProcessor {
public:
    SplitterProcessor(int id) : ModuleProcessor(id, 1, 5) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);

    }

    void updateFromState(ModuleState moduleState) {
    }

    void processSample() {
        auto in = cvInputs[0].read();
        cvOutputs[0].write(in);
        cvOutputs[1].write(in);
        cvOutputs[2].write(in);
        cvOutputs[3].write(in);
        cvOutputs[4].write(in);

    }

private:
};

