/*
  ==============================================================================

    ClockProcessor.h
    Created: 1 May 2025 5:56:07pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class ClockProcessor : public ModuleProcessor {
public:
    ClockProcessor(int id) : ModuleProcessor(id, 0, 1) {
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);
        position = 0.0;
        increment = 0.0;
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state.getProperty("hz");
        if (!newHz.isVoid()) {
            increment = newHz;
        }
    }

    void processSample() {
        position += increment;
        if (position >= sampleRate) {
            position -=sampleRate;
            pulse = 1.0;
        }
        getCVOutputJack(0)->write(pulse);
        pulse = 0.0;
    }


private:
    double position, increment;
    float pulse = 0.0;
};