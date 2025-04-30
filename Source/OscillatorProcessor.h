/*
  ==============================================================================

    OscillatorProcessor.h
    Created: 16 Apr 2025 11:02:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "faustSin.h"
#include "faustTriangle.h"
#include "faustSaw.h"
#include "faustSquare.h"
#include "faustLfoSaw.h"
#include "faustLfoSquare.h"

enum WAVETYPE {
    SIN,
    TRIANGLE,
    SAW,
    SQUARE,
    LFOSAW,
    LFOSQUARE
};
const int NUM_WAVETYPES = 6;

class OscillatorProcessor : public ModuleProcessor {
public:
    OscillatorProcessor(int id) : ModuleProcessor(id, 2, 1) {}

    ~OscillatorProcessor() {
        ModuleProcessor::~ModuleProcessor();
        for (int i = 0; i < NUM_WAVETYPES; i++) {
            if (faustProcessors[i]) {
                delete faustProcessors[i];
            }
            if (faustControllers[i]) {
                delete faustControllers[i];
            }
        }
        if (faustOutput[0]) {
            delete faustOutput[0];
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);

        // init faust processors
        faustProcessors[SIN] = new sindsp();
        faustProcessors[TRIANGLE] = new triangledsp();
        faustProcessors[SAW] = new sawdsp();
        faustProcessors[SQUARE] = new squaredsp();
        faustProcessors[LFOSAW] = new lfsawdsp();
        faustProcessors[LFOSQUARE] = new lfsquaredsp();

        for (int i = 0; i < NUM_WAVETYPES; i++) {
            faustControllers[i] = new MapUI();
            faustProcessors[i]->init(sampleRate);
            faustProcessors[i]->buildUserInterface(faustControllers[i]);
        }
        faustOutput[0] = new float;

        setHz(440.0);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            setHz(newHz);
        }
        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType = (WAVETYPE)(int)wave;
        }

        auto lfoToggle = moduleState.state.getProperty("lfo_toggle");
        if (!lfoToggle.isVoid()) {
            lfo = lfoToggle;
        }
    }

    void processSample() {
        auto adjustedHz = hz + hz*(cvInputs[0].read());
        adjustedHz = lfo? getLfoHz(adjustedHz) : adjustedHz;
        auto adjustedGain = (cvInputs[1].isConnected()) ? (cvInputs[1].read() + 1.0) / 2.0 : 1.0;
        auto wave = getWaveType();
        faustControllers[wave]->setParamValue("hz", adjustedHz);
        faustProcessors[wave]->compute(1, nullptr, faustOutput);
        cvOutputs[0].write(faustOutput[0][0] *  adjustedGain);
    }

    void setHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
    }

    double getLfoHz(double hz) {
        return 60.0 * (hz - 24.0) / (20000 - 24.0);
    }

    WAVETYPE getWaveType() {
        // SAW and SQUARE have different logic at low freq
        if (lfo) {
            if (waveType == SAW) {
                return LFOSAW;
            }
            if (waveType == SQUARE) {
                return LFOSQUARE;
            }
        }
        return waveType;
    }

private:
    WAVETYPE waveType = SIN;
    double hz = 440.0;
    bool lfo = false;
    faust::MapUI *faustControllers[NUM_WAVETYPES];
    faust::dsp *faustProcessors[NUM_WAVETYPES];
    float* faustOutput[1];
};
