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
        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType = wave;
        }
        auto lfoToggle = moduleState.state.getProperty("lfo_mode");
        if (!lfoToggle.isVoid()) {
            lfo = lfoToggle;
        }
    }

    void processSample() {
        auto phaseMod = (cvInputs[0].read() + 1.0);
        auto lfoMod = (lfo) ? 0.00390625 : 1.0;
        phase += phaseIncrement * phaseMod * lfoMod;

        
        while (phase > juce::MathConstants<float>::twoPi) {
            phase -= juce::MathConstants<float>::twoPi;
        }

        jassert(phase >= 0.0);

        auto ampMod = (cvInputs[1].isConnected()) ? (cvInputs[1].read() + 1.0) / 2.0 : 1.0;
        cvOutputs[0].write(wave(phase) *  ampMod);
    }

    void updateHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
        phaseIncrement = (hz * juce::MathConstants<double>::twoPi) / sampleRate;
    }

private:
    double hz = 440.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
    int waveType = 0;
    bool lfo = false;

    float wave(double phase) {
        switch (waveType) {
        case 0:
            return sin(phase);
        case 1:
            return triWave(phase);
        case 2:
            return sawWave(phase);
        case 3:
            return sqWave(phase);
        }
    }

    float triWave(double phase) {
        float triWave[] = {0.0f, 1.0f, 0.0f, -1.0f, 0.0f};
        float idx = phase / MathConstants<float>::halfPi;

        int lo = floor(idx);
        int hi = ceil(idx);
        float hi_amt = idx - lo;
        float lo_amt = 1.0f - hi_amt;
        
        if (lo < 0 || hi > 4) {
            return 0.0;
        }

        return (triWave[lo] * lo_amt) + (triWave[hi] * hi_amt);
    }

    float sawWave(double phase) {
        return jmap<double>(phase, 0, MathConstants<double>::twoPi, -1, 1);
    }

    float sqWave(double phase) {
        if (phase <= MathConstants<double>::pi) {
            return -1.0;
        }
        else {
            return 1.0;
        }
    }
};


