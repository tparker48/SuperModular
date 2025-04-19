/*
  ==============================================================================

    OscillatorProcessor.h
    Created: 16 Apr 2025 11:02:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class OscillatorProcessor : public ModuleProcessor {
public:
    OscillatorProcessor(int id) : ModuleProcessor(id, 2, 1) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);
        updateHz(440.0);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            updateHz(newHz);
        }   
    }

    void processSample() {
        if (phase >= juce::MathConstants<float>::twoPi) {
            phase -= juce::MathConstants<float>::twoPi;
        }

        auto phaseMod = cvInputs[0].read();
        phase += phaseIncrement + (phaseMod*0.1);

        auto ampMod = 1.0 + (cvInputs[1].read() - 0.5);
        cvOutputs[0].write(sin(phase) * ampMod);
    }

    void updateHz(double newHz) {
        hz = newHz;
        phaseIncrement = (hz * juce::MathConstants<double>::twoPi) / sampleRate;
    }

private:
    double hz = 440.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
};

