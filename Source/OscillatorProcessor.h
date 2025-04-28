/*
  ==============================================================================

    OscillatorProcessor.h
    Created: 16 Apr 2025 11:02:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "faust_bandlimited_osc.h"

class OscillatorProcessor : public ModuleProcessor {
public:
    OscillatorProcessor(int id) : ModuleProcessor(id, 2, 1) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);
        updateHz(440.0);

        fDSP = new mydsp();
        fDSP->init(sampleRate);
        fUI = new MapUI();
        fDSP->buildUserInterface(fUI);
        outputs = new float* [2];
        for (int channel = 0; channel < 2; ++channel) {
            outputs[channel] = new float[samplesPerBlock];
        }
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

        fUI->setParamValue("hz", hz);
        switch (waveType) {
        case 0:
            fUI->setParamValue("wave_select_1", 0);
            fUI->setParamValue("wave_select_2", 0);
            break;
        case 1:
            fUI->setParamValue("wave_select_1", 0);
            fUI->setParamValue("wave_select_2", 1);
            break;
        case 2:
            fUI->setParamValue("wave_select_1", 1);
            fUI->setParamValue("wave_select_2", 0);
            break;
        case 3:
            fUI->setParamValue("wave_select_1", 1);
            fUI->setParamValue("wave_select_2", 1);
            break;
        }
        
        fDSP->compute(1, nullptr, outputs);
        cvOutputs[0].write(outputs[0][0] *  ampMod);
    }

    void updateHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
        phaseIncrement = (hz * juce::MathConstants<double>::twoPi) / sampleRate;
    }

    float wave(double phase) {
        switch (waveType) {
        case 0:
            return sin(phase);
        case 1:
            return sin(phase);
        case 2:
            return sin(phase);
        case 3:
            return sin(phase);
        }
    }

private:
    double hz = 440.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
    int waveType = 0;
    bool lfo = false;

    MapUI* fUI;
    dsp* fDSP;
    float** outputs;
};


