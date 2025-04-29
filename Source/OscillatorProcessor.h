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

enum WAVETYPE {
    SIN,
    TRIANGLE,
    SAW,
    SQUARE
};
const int NUM_WAVETYPES = 4;

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
    }

    void processSample() {
        auto cvHzMod = (cvInputs[0].read() + 1.0); // ranges [0,2]
        auto ampMod = (cvInputs[1].isConnected()) ? (cvInputs[1].read() + 1.0) / 2.0 : 1.0;
        faustControllers[waveType]->setParamValue("hz", hz * cvHzMod);
        faustProcessors[waveType]->compute(1, nullptr, faustOutput);
        cvOutputs[0].write(faustOutput[0][0] *  ampMod);
    }

    void setHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
    }

private:
    WAVETYPE waveType = SIN;
    double hz = 440.0;
    MapUI *faustControllers[NUM_WAVETYPES];
    dsp *faustProcessors[NUM_WAVETYPES];
    float* faustOutput[1];
};
