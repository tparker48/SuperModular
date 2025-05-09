/*
  ==============================================================================

    OldOscillatorProcessor.h
    Created: 16 Apr 2025 11:02:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "Oscillator.h"

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
    OscillatorProcessor(int id) : ModuleProcessor(id, 1, 1) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);

        setHz(440.0);
        hzSmooth.reset(sampleRate, 0.01);
        hzSmooth.setCurrentAndTargetValue(hz);

        osc.prepareToPlay(sampleRate);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            setHz(newHz);
            hzSmooth.setTargetValue(lfo ? getLfoHz(hz) : hz);
        }

        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType = getWaveType((WAVETYPE)(int)wave);
        }

        auto lfoToggle = moduleState.state.getProperty("lfo_toggle");
        if (!lfoToggle.isVoid()) {
            lfo = lfoToggle;
        }

        auto newFmAmt = moduleState.state.getProperty("fmAmt");
        if (!newFmAmt.isVoid()) {
            fmAmt = newFmAmt;
        }
    }

    void processSample() {
        hzSmooth.skip(1);
        osc.incrementPhase();
        osc.setHz(hzSmooth.getCurrentValue() + (cvInputs[0].read() * fmAmt * hzSmooth.getCurrentValue()));
        switch (waveType) {
        case SIN:
            getCVOutputJack(0)->write(osc.getSinSample());
            break;
        case TRIANGLE:
            getCVOutputJack(0)->write(osc.getTriangleSample());
            break;
        case SAW:
            getCVOutputJack(0)->write(osc.getSawSample());
            break;
        case SQUARE:
            getCVOutputJack(0)->write(osc.getSquareSample());
            break;
        }
        
    }

    void setHz(double newHz) {
        jassert(newHz >= 0.0);
        hz = newHz;
    }

    double getLfoHz(double hz) {
        return 60.0 * (hz - 23.9) / (20000 - 23.9);
    }

    WAVETYPE getWaveType(WAVETYPE wave) {
        // SAW and SQUARE have different logic at low freq
        if (lfo) {
            if (wave == SAW) {
                return WAVETYPE::LFOSAW;
            }
            if (wave == SQUARE) {
                return WAVETYPE::LFOSQUARE;
            }
        }
        return wave;
    }

private:
    Oscillator osc;

    double hz = 440.0;
    double fmAmt = 0.0;
    bool lfo = false;
    WAVETYPE waveType = WAVETYPE::SIN;

    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;
};
