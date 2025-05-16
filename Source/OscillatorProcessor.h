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

class OscillatorProcessor : public ModuleProcessor {
public:
    OscillatorProcessor(int id) : ModuleProcessor(id, 2, 1) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);

        setHz(440.0);
        hzSmooth.reset(sampleRate, 0.01);
        hzSmooth.setCurrentAndTargetValue(hz);

        dcSmooth.reset(sampleRate, 0.01);
        dcSmooth.setCurrentAndTargetValue(0.5);

        osc.prepareToPlay(sampleRate);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            setHz(newHz);
        }

        auto lfoToggle = moduleState.state.getProperty("lfo_toggle");
        if (!lfoToggle.isVoid()) {
            lfo = lfoToggle;
        }

        auto wave = moduleState.state.getProperty("wave");
        if (!wave.isVoid()) {
            waveType = getWaveType((WAVETYPE)(int)wave);
        }

        auto newDc = moduleState.state.getProperty("dutyCycle");
        if (!newDc.isVoid()) {
            dutyCycle = newDc;
            dcSmooth.setTargetValue(dutyCycle);
        }

        auto newDcAmt = moduleState.state.getProperty("dutyCycleAmt");
        if (!newDcAmt.isVoid()) {
            dutyCycleAmt = newDcAmt;
        }


        auto newFmAmt = moduleState.state.getProperty("fmAmt");
        if (!newFmAmt.isVoid()) {
            fmAmt = newFmAmt;
        }

        hzSmooth.setTargetValue(lfo ? getLfoHz(hz) : hz);
    }

    void processSample() {
        hzSmooth.skip(1);
        dcSmooth.skip(1);

        osc.incrementPhase();
        osc.setHz(hzSmooth.getCurrentValue() + (cvInputs[1].read() * fmAmt * hzSmooth.getCurrentValue()));
        osc.setPulseDutyCycle(dcSmooth.getCurrentValue() + (dutyCycleAmt * cvInputs[0].read()));
        cvOutputs[0].write(osc.getSample(waveType));
    }

    void setHz(double newHz) {
        hz = newHz;
    }

    double getLfoHz(double hz) {
        return 60.0 * (hz - 23.9) / (6000.0 - 23.9);
    }

    WAVETYPE getWaveType(WAVETYPE wave) {
        // SAW and SQUARE have different logic at low freq
        if (lfo) {
            if (wave == SAW) {
                return WAVETYPE::LFOSAW;
            }
            if (wave == PULSE) {
                return WAVETYPE::LFOPULSE;
            }
            if (wave == TRIANGLE) {
                return WAVETYPE::LFOTRI;
            }
        }
        return wave;
    }

private:
    Oscillator osc;

    double hz = 440.0;
    double fmAmt = 0.0;
    float dutyCycle = 0.5;
    float dutyCycleAmt = 0.0;
    bool lfo = false;
    WAVETYPE waveType = WAVETYPE::SIN;

    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> dcSmooth;
};
