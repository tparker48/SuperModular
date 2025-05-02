/*
  ==============================================================================

    NoiseProcessor.h
    Created: 30 Apr 2025 9:57:57pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "NoiseGenerator.h"


class NoiseProcessor : public ModuleProcessor {
public:
    NoiseProcessor(int id) : ModuleProcessor(id, 0, 3) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);
        position = 0.0;
        increment = 0.0;
        holdValue = 0.0;
        fine = 0.0;
    }

    void updateFromState(ModuleState moduleState) {
        auto newRate = moduleState.state["rate"];
        if (!newRate.isVoid()) {
            increment = newRate;
        }
        auto newFine = moduleState.state["fine"];
        if (!newFine.isVoid()) {
            fine = newFine;
        }
    }

    void processSample() {
        auto white = noise.generateSampleWhite();
        auto pink = noise.generateSamplePink() * pinkGain;

        position += increment + (increment * 0.5 * fine);
        if (position >= sampleRate) {
            position = 0.0;
            holdValue = white;
        }
        
        getCVOutputJack(0)->write(pink);
        getCVOutputJack(1)->write(white);
        getCVOutputJack(2)->write(holdValue);
    }

private:
    NoiseGenerator noise;
    double position, increment, fine;
    float holdValue;
    float pinkGain = 0.2;
    float whiteGain = 0.7;
};
