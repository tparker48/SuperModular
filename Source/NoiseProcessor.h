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

    }

    void updateFromState(ModuleState moduleState) {
    }

    void processSample() {
        noise.generateSampleWhite();
        noise.generateSamplePink();
    }

private:
    NoiseGenerator noise;
};