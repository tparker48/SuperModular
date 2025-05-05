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
        hzSmooth.reset(sampleRate, 0.01);
        hzSmooth.setCurrentAndTargetValue(hz);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            setHz(newHz);
        }
        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType = getWaveType((WAVETYPE)(int)wave);
        }

        auto lfoToggle = moduleState.state.getProperty("lfo_toggle");
        if (!lfoToggle.isVoid()) {
            lfo = lfoToggle;
        }

        auto newAmAmt = moduleState.state.getProperty("amAmt");
        if (!newAmAmt.isVoid()) {
            amAmt = newAmAmt;
        }

        auto newFmAmt = moduleState.state.getProperty("fmAmt");
        if (!newFmAmt.isVoid()) {
            fmAmt = newFmAmt;
        }
    }

    void preBlockProcessing() override {
        hzSmooth.setTargetValue(lfo ? getLfoHz(hz) : hz);
        hzSmooth.skip(blockSize);
        fmAmtxSmoothed = fmAmt * hzSmooth.getCurrentValue();
        amAmtxAmplitude = amAmt * amplitude;
    }

    void processSample() {
        faustControllers[waveType]->setParamValue("hz", hzSmooth.getCurrentValue() + (cvInputs[0].read() * fmAmtxSmoothed));
        faustProcessors[waveType]->compute(1, nullptr, faustOutput);
        cvOutputs[0].write(faustOutput[0][0] * amplitude + (amAmtxAmplitude * cvInputs[1].read()));
    }

    void setHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
    }

    double getLfoHz(double hz) {
        return 60.0 * (hz - 24.0) / (20000 - 24.0);
    }

    WAVETYPE getWaveType(WAVETYPE wave) {
        // SAW and SQUARE have different logic at low freq
        if (lfo) {
            if (wave == SAW) {
                return LFOSAW;
            }
            if (wave == SQUARE) {
                return LFOSQUARE;
            }
        }
        return wave;
    }

private:
    WAVETYPE waveType = SIN;
    double fmAmtxSmoothed, amAmtxAmplitude;
    double hz = 440.0;
    double amplitude = 1.0;
    double fmAmt = 0.0;
    double amAmt = 0.0;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;
    bool lfo = false;
    faust::MapUI *faustControllers[NUM_WAVETYPES];
    faust::dsp *faustProcessors[NUM_WAVETYPES];
    float* faustOutput[1];
};
